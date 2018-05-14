#include <QtWidgets>
#include "../include/RectObject.h"

RectObject::RectObject() {
  drawRect = QRectF(0, 0, 100, 100);
  color = QColor(0, 0, 0, 255);
  rotation = 0;
  penWidth = 1;
  penStyle = 1;
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
}

void RectObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  prepareGeometryChange();
  QPainterPath path;
  path.addRect(drawRect);
  painter->strokePath(path, QPen(color, penWidth, Qt::PenStyle(penStyle)));
  if (isSelected()) {
    painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
    painter->drawLine(topLeft(), bottomRight());
    painter->drawLine(topRight(), bottomLeft());
    update();
  }
}

QColor RectObject::getColor() {
  return color;
}

void RectObject::setColor(const QColor &subColor) {
  color = subColor;
}

qreal RectObject::getLeft() {
  return drawRect.left();
}

void RectObject::setLeft(qreal newLeft) {
  drawRect.setLeft(newLeft);
  drawRect = drawRect.normalized();
}

qreal RectObject::getRight() {
  return drawRect.right();
}

void RectObject::setRight(qreal newRight) {
  drawRect.setRight(newRight);
  drawRect = drawRect.normalized();
}

qreal RectObject::getTop() {
  return drawRect.top();
}

void RectObject::setTop(qreal newTop) {
  drawRect.setTop(newTop);
  drawRect = drawRect.normalized();
}

qreal RectObject::getBottom() {
  return drawRect.bottom();
}

void RectObject::setBottom(qreal newBottom) {
  drawRect.setBottom(newBottom);
  drawRect = drawRect.normalized();
}

QPointF RectObject::topLeft() {
  return drawRect.topLeft();
}

QPointF RectObject::topRight() {
  return drawRect.topRight();
}

QPointF RectObject::bottomLeft() {
  return drawRect.bottomLeft();
}

QPointF RectObject::bottomRight() {
  return drawRect.bottomRight();
}

void RectObject::setTopLeft(QPointF topLeft) {
  drawRect.setTopLeft(topLeft);
  drawRect = drawRect.normalized();
}

void RectObject::setTopRight(QPointF topRight) {
  drawRect.setTopRight(topRight);
  drawRect = drawRect.normalized();
}

void RectObject::setBottomLeft(QPointF bottomLeft) {
  drawRect.setBottomLeft(bottomLeft);
  drawRect = drawRect.normalized();
}

void RectObject::setBottomRight(QPointF bottomRight) {
  drawRect.setBottomRight(bottomRight);
  drawRect = drawRect.normalized();
}

QPointF RectObject::getCenter() {
  return boundingRect().center();
}

void RectObject::moveCenter(QPointF newCenter) {
  drawRect.moveCenter(newCenter);
  drawRect = drawRect.normalized();
}

qreal RectObject::getRotationValue() {
  return rotation;
}

void RectObject::setRotationValue(qreal newRotation) {
  rotation = newRotation;
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

QRectF RectObject::boundingRect() const {
  QRectF myRect = drawRect.normalized().adjusted(-10,-10, 10, 10);
  return myRect;
}

QPainterPath RectObject::shape() const {
  QPainterPath path;
  QPainterPathStroker stroker;
  stroker.setWidth(penWidth + 15);
  path.addRect(drawRect);
  return stroker.createStroke(path);
}

int RectObject::getPenWidth() {
  return penWidth;
}

void RectObject::setPenWidth(int width) {
  penWidth = width;
}

int RectObject::getPenStyle() {
  return penStyle;
}

void RectObject::setPenStyle(int style) {
  penStyle = style;
}


int RectObject::type() const {
  return Type;
}

void RectObject::mousePressEvent(QGraphicsSceneMouseEvent *) {
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void RectObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
  QApplication::restoreOverrideCursor();
}

void RectObject::hoverMoveEvent(QGraphicsSceneHoverEvent *) {
  QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void RectObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  QApplication::restoreOverrideCursor();
}
