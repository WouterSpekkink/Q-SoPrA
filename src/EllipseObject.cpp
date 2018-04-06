#include <QtWidgets>
#include "../include/EllipseObject.h"

EllipseObject::EllipseObject() {
  drawRect = QRectF(0, 0, 100, 100);
  color = QColor(0, 0, 0, 255);
  setCursor(Qt::OpenHandCursor);
  setFlag(QGraphicsTextItem::ItemIsMovable, false);
  setFlag(QGraphicsTextItem::ItemIsSelectable, true);
}

void EllipseObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  painter->setPen(QPen(color, 1));
  painter->drawEllipse(drawRect);
  if (isSelected()) {
    painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
    painter->drawRect(drawRect);
    update();
  }
}

QRectF EllipseObject::boundingRect() const {
  return drawRect.normalized().adjusted(-50, -50, 50, 50);
}

QColor EllipseObject::getColor() {
  return color;
}

void EllipseObject::setColor(const QColor &subColor) {
  color = subColor;
}

qreal EllipseObject::getLeft() {
  return drawRect.left();
}

void EllipseObject::setLeft(qreal newLeft) {
  drawRect.setLeft(newLeft);
}

qreal EllipseObject::getRight() {
  return drawRect.right();
}

void EllipseObject::setRight(qreal newRight) {
  drawRect.setRight(newRight);
}

qreal EllipseObject::getTop() {
  return drawRect.top();
}

void EllipseObject::setTop(qreal newTop) {
  drawRect.setTop(newTop);
}

qreal EllipseObject::getBottom() {
  return drawRect.bottom();
}

void EllipseObject::setBottom(qreal newBottom) {
  drawRect.setBottom(newBottom);
}

QPointF EllipseObject::topLeft() {
  return drawRect.topLeft();
}

QPointF EllipseObject::topRight() {
  return drawRect.topRight();
}

QPointF EllipseObject::bottomLeft() {
  return drawRect.bottomLeft();
}

QPointF EllipseObject::bottomRight() {
  return drawRect.bottomRight();
}

void EllipseObject::setTopLeft(QPointF topLeft) {
  drawRect.setTopLeft(topLeft);
}

void EllipseObject::setTopRight(QPointF topRight) {
  drawRect.setTopRight(topRight);
}

void EllipseObject::setBottomLeft(QPointF bottomLeft) {
  drawRect.setBottomLeft(bottomLeft);
}

void EllipseObject::setBottomRight(QPointF bottomRight) {
  drawRect.setBottomRight(bottomRight);
}

void EllipseObject::moveCenter(QPointF newCenter) {
  drawRect.moveCenter(newCenter);
}

int EllipseObject::type() const {
  return Type;
}
