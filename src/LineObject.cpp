#include "../include/LineObject.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include "../include/Scene.h"

LineObject::LineObject(QPointF subStartPos,
		       QPointF subEndPos,
		       QGraphicsItem *parent)
  : QGraphicsLineItem(parent) {
  startPos = subStartPos;
  endPos = subEndPos;
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  color = Qt::black;
  setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  setCursor(Qt::OpenHandCursor);
}

QRectF LineObject::boundingRect() const {
  qreal extra = (pen().width() + 20) / 2.0;
  
  return QRectF(startPos, QSizeF(endPos.x() - startPos.x(),
				 endPos.y() - startPos.y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

QPainterPath LineObject::shape() const {
  QPainterPath path = QGraphicsLineItem::shape();
  path.addPolygon(arrowHead);
  return path;
}

void LineObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  calculate();
  QPen myPen = pen();
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);

  arrowHead.clear();
  arrowHead << tempLine1.p2() << arrowP1 << tempLine1.p2() << arrowP2;
  
  arrowHead2.clear();
  arrowHead2 << tempLine2.p2() << arrowP3 << tempLine2.p2() << arrowP4;
  
  QPainterPath myPath;
  myPath.moveTo(tempLine2.p2());
  myPath.lineTo(tempLine1.p2());
  painter->drawPolyline(arrowHead);
  painter->strokePath(myPath, QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawPolyline(arrowHead2);
}

void LineObject::calculate() {
  prepareGeometryChange();
  qreal arrowSize = 20;
  QLineF newLine = QLineF(startPos, endPos);
  setLine(newLine);
  tempLine1 = QLineF(startPos, endPos);
  tempLine2 = QLineF(endPos, startPos);
  double angle = ::acos(tempLine1.dx() / tempLine1.length());
  if (tempLine1.dy() >= 0)
    angle = (Pi * 2) - angle;
  double angle2 = ::acos(tempLine2.dx() / tempLine2.length());
  if (tempLine2.dy() >= 0)
    angle2 = (Pi * 2) - angle2;
  arrowP1 = tempLine1.p2() - QPointF(sin(angle + Pi / 3) * arrowSize,
				     cos(angle + Pi / 3) * arrowSize);
  arrowP2 = tempLine1.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				     cos(angle + Pi - Pi / 3) * arrowSize);

  
  arrowP3 = tempLine2.p2() - QPointF(sin(angle2 + Pi /3) * arrowSize,
				     cos(angle2 + Pi / 3) * arrowSize);
  arrowP4 = tempLine2.p2() - QPointF(sin(angle2 + Pi - Pi / 3) * arrowSize,
				     cos(angle2 + Pi - Pi / 3) * arrowSize);  
  prepareGeometryChange();
}

void LineObject::setColor(const QColor &subColor) {
  color = subColor;
}

QColor LineObject::getColor() {
  return color;
}

QPointF LineObject::getStartPos() {
  return startPos;
}

QPointF LineObject::getEndPos() {
  return endPos;
}

void LineObject::setStartPos(QPointF subPoint) {
  startPos = subPoint;
}

void LineObject::setEndPos(QPointF subPoint) {
  endPos = subPoint;
}

void LineObject::setStartPos(qreal x, qreal y) {
  startPos = QPointF(x, y);
}

void LineObject::setEndPos(qreal x, qreal y) {
  endPos = QPointF(x, y);
}

int LineObject::type() const {
  return Type;
}


