#include "../include/DirectedEdge.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include <QtCore>

const qreal Pi = 3.14;

DirectedEdge::DirectedEdge(NetworkNode *startItem, NetworkNode *endItem, QString submittedType,
			   QString submittedName, QGraphicsItem *parent)
  : QGraphicsLineItem(parent) {
  start = startItem;
  end = endItem;
  color = Qt::black;
  setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  height = 20;
  relType = submittedType;
  name = submittedName;
}

QRectF DirectedEdge::boundingRect() const {
  qreal extra = (pen().width() + height + 20) / 2.0;
  
  return QRectF(start->pos(), QSizeF(end->pos().x() - start->pos().x(),
				     end->pos().y() - start->pos().y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

QPainterPath DirectedEdge::shape() const {
  QPainterPath path = QGraphicsLineItem::shape();
  path.addPolygon(arrowHead);
  return path;
}

void DirectedEdge::calc() {
  prepareGeometryChange();
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
  
  double angle = ::acos(oLine.dx() / oLine.length());
  if (oLine.dy() >= 0)
    angle = (Pi * 2) - angle;

  qreal arrowSize = 10;
  
  arrowP1 = oLine.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
    cos(angle + Pi / 3) * arrowSize);
  arrowP2 = oLine.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
  cos(angle + Pi - Pi / 3) * arrowSize);
}

void DirectedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen myPen = pen();
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);
  calc();
  
  arrowHead.clear();
  arrowHead << oLine.p2() << arrowP1 << arrowP2;

  QPainterPath myPath;
  myPath.moveTo(tempStart);
  myPath.quadTo(midPoint, oLine.p2());
  painter->translate(start->pos() - tempStart);
  painter->rotate(theta);
  painter->drawPolygon(arrowHead);
  painter->strokePath(myPath, QPen(color));
}

NetworkNode* DirectedEdge::startItem() const {
  return start;
}

NetworkNode* DirectedEdge::endItem() const {
  return end;
}

void DirectedEdge::setColor(const QColor &subColor) {
  color = subColor;
}

QString DirectedEdge::getType() {
  return relType;
}

void DirectedEdge::setHeight(int submittedHeight) {
  height = submittedHeight;
}

int DirectedEdge::getHeight() {
  return height;
}

int DirectedEdge::type() const {
  return Type;
}

QString DirectedEdge::getName() {
  return name;
}
