#include "../include/UndirectedEdge.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include <QtCore>

const qreal Pi = 3.14;

UndirectedEdge::UndirectedEdge(NetworkNode *startItem, NetworkNode *endItem,
			       QString submittedType, QString submittedName,
			       QGraphicsItem *parent)
  : QGraphicsLineItem(parent) {
  start = startItem;
  end = endItem;
  color = Qt::black;
  setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  height = 20;
  relType = submittedType;
  name = submittedName;
  filtered = true;
  massHidden = false;
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  comment = "";
}

QRectF UndirectedEdge::boundingRect() const {
  qreal extra = (pen().width() + height + 20) / 2.0;
  
  return QRectF(start->pos(), QSizeF(end->pos().x() - start->pos().x(),
				     end->pos().y() - start->pos().y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

void UndirectedEdge::calc() {
  // Let us first calculate the distance between our two points.
  qreal xDiff = end->pos().x() - start->pos().x();
  qreal yDiff = end->pos().y() - start->pos().y();

  qreal distance = sqrt(pow(xDiff, 2) + pow(yDiff, 2));

  // Then we draw two new points that are on a horizontal line.
  // This makes it easier to draw a control point perpendicular to the line.
  tempStart = QPointF(0, 0);
  QPointF tempEnd = QPointF(distance, 0);

  // Calculate midpoint
  qreal mX = (tempStart.x() + tempEnd.x()) / 2;
  qreal mY = (tempStart.y() + tempEnd.y()) / 2;
  midPoint = QPointF(mX, mY);
  midPoint.setY(midPoint.y() + height);

  QLineF newLine = QLineF(tempStart, tempEnd);
  setLine(newLine);

  // But then we still need to rotate and translate the painter such that it draws
  // the line between the correct points. 
  qreal dY = end->pos().y() - start->pos().y();
  qreal dX = end->pos().x() - start->pos().x();
  
  // theta is initially the angle in radians, so we convert it to degrees.
  theta = atan2(dY, dX);
  theta = qRadiansToDegrees(theta);

  oLine = QLineF(midPoint, tempEnd);
  oLine.setLength(oLine.length() - 18);
  sLine = QLineF(midPoint, tempStart);
  sLine.setLength(sLine.length() - 18);
  
  double angle = ::acos(oLine.dx() / oLine.length());
  if (oLine.dy() >= 0)
    angle = (Pi * 2) - angle;
  double angle2 = ::acos(sLine.dx() / sLine.length());
  if (sLine.dy() >= 0)
    angle2 = (Pi * 2) - angle2;
  
  qreal arrowSize = 10;
  
  arrowP1 = oLine.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
				 cos(angle + Pi / 3) * arrowSize);
  arrowP2 = oLine.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				 cos(angle + Pi - Pi / 3) * arrowSize);
  arrowP3 = sLine.p2() - QPointF(sin(angle2 + Pi /3) * arrowSize,
				 cos(angle2 + Pi / 3) * arrowSize);
  arrowP4 = sLine.p2() - QPointF(sin(angle2 + Pi - Pi / 3) * arrowSize,
				 cos(angle2 + Pi - Pi / 3) * arrowSize);
  prepareGeometryChange();
}

void UndirectedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen myPen = pen();
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);
  calc();
  
  arrowHead.clear();
  arrowHead << oLine.p2() << arrowP1 << arrowP2;
  
  arrowHead2.clear();
  arrowHead2 << sLine.p2() << arrowP3 << arrowP4;
  
  QPainterPath myPath;
  myPath.moveTo(sLine.p2());
  myPath.quadTo(midPoint, oLine.p2());
  painter->translate(start->pos() - tempStart);
  painter->rotate(theta);
  painter->drawPolygon(arrowHead);
  painter->strokePath(myPath, QPen(color));
  painter->drawPolygon(arrowHead2);
}

NetworkNode* UndirectedEdge::startItem() const {
  return start;
}

NetworkNode* UndirectedEdge::endItem() const {
  return end;
}

void UndirectedEdge::setColor(const QColor &subColor) {
  color = subColor;
}

QString UndirectedEdge::getType() {
  return relType;
}

void UndirectedEdge::setType(const QString submittedType) {
  relType = submittedType;
}

void UndirectedEdge::setHeight(int submittedHeight) {
  height = submittedHeight;
}

int UndirectedEdge::getHeight() {
  return height;
}

int UndirectedEdge::type() const {
  return Type;
}

QString UndirectedEdge::getName() {
  return name;
}

void UndirectedEdge::setName(const QString submittedName) {
  name = submittedName;
}

bool UndirectedEdge::isFiltered() {
  return filtered;
}

void UndirectedEdge::setFiltered(bool state) {
  filtered = state;
}

bool UndirectedEdge::isMassHidden() {
  return massHidden;
}

void UndirectedEdge::setMassHidden(bool state) {
  massHidden = state;
}

void UndirectedEdge::setComment(const QString submittedComment) {
  comment = submittedComment;
  QString toolTip = breakString(comment);
  this->setToolTip(toolTip);
}

QString UndirectedEdge::getComment() {
  return comment;
}

QColor UndirectedEdge::getColor() {
  return color;
}
