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
  filtered = true;
  massHidden = false;
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  comment = "";
}

QRectF DirectedEdge::boundingRect() const {
  qreal extra = (pen().width() + height + 20) / 2.0;
  
  return QRectF(start->pos(), QSizeF(end->pos().x() - start->pos().x(),
				     end->pos().y() - start->pos().y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

void DirectedEdge::calc() {
  // Let us first calculate the distance between our two points.
  qreal xDiff = end->pos().x() - start->pos().x();
  qreal yDiff = end->pos().y() - start->pos().y();
  qreal distance = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
  /* 
     Then we draw two new points that are on a horizontal line.
     This makes it easier to draw a control point perpendicular to the line.

     First, we calculate the control point
  */
  qreal controlX = (0.0 + distance) / 2;
  controlPoint = QPointF(controlX, height);
  /* 
     We need to rotate and translate the painter such that it draws
     the line between the correct points. Here we calculate the amount
     of degrees that we need to rotate the painter.
  */
  theta = atan2(yDiff, xDiff);
  theta = qRadiansToDegrees(theta);
  /* 
     The ghost line is a line from the control point to the end point.
     We do not draw it, but we need it to position the arrowhead.
  */
  ghostLine = QLineF(controlPoint, QPointF(distance, 0));
  ghostLine.setLength(ghostLine.length() - 18);
  // Then we do some calculations to determine how the arrow is drawn.  
  double angle = ::acos(ghostLine.dx() / ghostLine.length());
  if (ghostLine.dy() >= 0)
    angle = (Pi * 2) - angle;
  qreal arrowSize = 10;
  arrowP1 = ghostLine.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
				     cos(angle + Pi / 3) * arrowSize);
  arrowP2 = ghostLine.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				     cos(angle + Pi - Pi / 3) * arrowSize);
  prepareGeometryChange();
}

void DirectedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen myPen = pen();
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);
  calc();

  arrowHead.clear();
  arrowHead << ghostLine.p2() << arrowP1 << arrowP2;

  QPainterPath myPath;
  myPath.moveTo(QPointF(0,0));
  myPath.quadTo(controlPoint, ghostLine.p2());
  painter->translate(start->pos() - QPointF(0, 0));
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

void DirectedEdge::setType(const QString submittedType) {
  relType = submittedType;
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

void DirectedEdge::setName(const QString submittedName) {
  name = submittedName;
}

bool DirectedEdge::isFiltered() {
  return filtered;
}

void DirectedEdge::setFiltered(bool state) {
  filtered = state;
}

bool DirectedEdge::isMassHidden() {
  return massHidden;
}

void DirectedEdge::setMassHidden(bool state) {
  massHidden = state;
}

void DirectedEdge::setComment(const QString submittedComment) {
  comment = submittedComment;
  QString toolTip = breakString(comment);
  this->setToolTip(toolTip);
}

QString DirectedEdge::getComment() {
  return comment;
}

QColor DirectedEdge::getColor() {
  return color;
}
