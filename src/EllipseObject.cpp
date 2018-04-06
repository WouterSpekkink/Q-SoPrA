#include <QtWidgets>
#include "../include/EllipseObject.h"

EllipseObject::EllipseObject() {
  drawRect = QRectF(0, 0, 100, 100);
  color = QColor(0, 0, 0, 255);
  setCursor(Qt::OpenHandCursor);
  setFlag(QGraphicsItem::ItemIsMovable, false);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void EllipseObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  prepareGeometryChange();
  painter->setPen(QPen(color, 1));
  painter->drawEllipse(drawRect);
  if (isSelected()) {
    painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
    painter->drawPath(shape());
    update();
  }
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
  drawRect = drawRect.normalized();
}

qreal EllipseObject::getRight() {
  return drawRect.right();
}

void EllipseObject::setRight(qreal newRight) {
  drawRect.setRight(newRight);
  drawRect = drawRect.normalized();
}

qreal EllipseObject::getTop() {
  return drawRect.top();
}

void EllipseObject::setTop(qreal newTop) {
  drawRect.setTop(newTop);
  drawRect = drawRect.normalized();
}

qreal EllipseObject::getBottom() {
  return drawRect.bottom();
}

void EllipseObject::setBottom(qreal newBottom) {
  drawRect.setBottom(newBottom);
  drawRect = drawRect.normalized();
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
  drawRect = drawRect.normalized();
}

void EllipseObject::setTopRight(QPointF topRight) {
  drawRect.setTopRight(topRight);
  drawRect = drawRect.normalized();
}

void EllipseObject::setBottomLeft(QPointF bottomLeft) {
  drawRect.setBottomLeft(bottomLeft);
  drawRect = drawRect.normalized();
}

void EllipseObject::setBottomRight(QPointF bottomRight) {
  drawRect.setBottomRight(bottomRight);
  drawRect = drawRect.normalized();
}

QPointF EllipseObject::getCenter() {
  return boundingRect().center();
}

void EllipseObject::moveCenter(QPointF newCenter) {
  drawRect.moveCenter(newCenter);
  drawRect = drawRect.normalized();
}

void EllipseObject::setRotationValue(qreal rotation) {
  QTransform transform;
  QPointF center = drawRect.center();
  QPointF oldC = mapToScene(drawRect.center());
  
  transform.translate(center.x(), center.y());
  transform.rotate(rotation);
  transform.translate(-center.x(), -center.y());
  setTransform(transform);
  drawRect = drawRect.normalized();

  QPointF newC = mapToScene(drawRect.center());
  QPointF offset = oldC - newC;
  moveBy(offset.x(), offset.y());
}

QRectF EllipseObject::boundingRect() const {
  QRectF myRect = drawRect.normalized().adjusted(-10,-10, 10, 10);
  return myRect;
}

QPainterPath EllipseObject::shape() const {
  QPainterPath path;
  QRectF myRect = drawRect.adjusted(-1, -1, 1, 1);
  path.addRect(myRect);
  return path;
}

int EllipseObject::type() const {
  return Type;
}
