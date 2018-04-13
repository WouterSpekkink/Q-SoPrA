#include <QtWidgets>
#include "../include/EllipseObject.h"

EllipseObject::EllipseObject() {
  drawRect = QRectF(0, 0, 100, 100);
  color = QColor(0, 0, 0, 255);
  rotation = 0;
  penWidth = 1;
  penStyle = 1;
  setFlag(QGraphicsItem::ItemIsMovable, false);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
}

void EllipseObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  prepareGeometryChange();
  painter->setPen(QPen(color, penWidth, Qt::PenStyle(penStyle)));
  QPainterPath path;
  path.addEllipse(drawRect);
  painter->strokePath(path, QPen(color, penWidth, Qt::PenStyle(penStyle)));
  if (isSelected()) {
    QRectF selectRect = drawRect.adjusted(-penWidth / 2, -penWidth / 2, penWidth / 2, penWidth / 2);
    painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
    QPainterPath outline;
    outline.addRect(selectRect);
    painter->drawPath(outline);
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

qreal EllipseObject::getRotationValue() {
  return rotation;
}

void EllipseObject::setRotationValue(qreal newRotation) {
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

QRectF EllipseObject::boundingRect() const {
  QRectF myRect = drawRect.normalized().adjusted(-10,-10, 10, 10);
  return myRect;
}

QPainterPath EllipseObject::shape() const {
  QPainterPath path;
  QPainterPathStroker stroker;
  stroker.setWidth(penWidth + 5);
  QRectF myRect = drawRect.adjusted(-penWidth / 2, -penWidth / 2, penWidth / 2, penWidth / 2);
  QRectF myRectTwo = drawRect.adjusted(1, 1, -1, -1);
  path.addRect(myRect);
  path.addEllipse(myRectTwo);
  path.setFillRule(Qt::OddEvenFill); 
  return stroker.createStroke(path);
}

int EllipseObject::getPenWidth() {
  return penWidth;
}

void EllipseObject::setPenWidth(int width) {
  penWidth = width;
}

int EllipseObject::getPenStyle() {
  return penStyle;
}

void EllipseObject::setPenStyle(int style) {
  penStyle = style;
}

int EllipseObject::type() const {
  return Type;
}

void EllipseObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QPainterPath path;
  QRectF myRect = drawRect.adjusted(10, 10, -10, -10);
  path.addEllipse(myRect);
  if (mapToScene(path).contains(event->scenePos())) {
    event->ignore();
  } else {
    event->accept();
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  }
}

void EllipseObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void EllipseObject::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
  QPainterPath path;
  QRectF myRect = drawRect.adjusted(10, 10, -10, -10);
  path.addEllipse(myRect);
  if (mapToScene(path).contains(event->scenePos())) {
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
  } else {
    QApplication::setOverrideCursor(Qt::OpenHandCursor);
  }
}

void EllipseObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

