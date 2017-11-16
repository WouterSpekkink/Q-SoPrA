#include <QtWidgets>
#include "../include/EventItem.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

EventItem::EventItem(int subWidth, QString toolTip, QPointF originalPosition, int subId) : color(255, 255, 255) {
  width = subWidth;

  setToolTip(toolTip);
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  //  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  originalPos = originalPosition;
  id = subId;
}

/*
  I assume that this is for collision or
  selection purposes?
*/

QRectF EventItem::boundingRect() const {
  return QRectF(-15.5, -15.5, width + 4, 34);
}

void EventItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(color));
  painter->drawEllipse(-15, -15, width, 30);
}

// Only to set the cursor to a different graphic.
void EventItem::mousePressEvent(QGraphicsSceneMouseEvent *) {
    setCursor(Qt::ClosedHandCursor);
}

void EventItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->modifiers() & Qt::AltModifier) {
    QPointF newPos = event->scenePos();
    this->setPos(newPos);
    setCursor(Qt::OpenHandCursor);
  } else {
    qreal oldX = originalPos.x();
    QPointF newPos = event->scenePos();
    
    newPos.setX(oldX);
    this->setPos(newPos);
    setCursor(Qt::OpenHandCursor);
  }
}

void EventItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  {
  if (event->modifiers() & Qt::AltModifier) {
    QPointF newPos = event->scenePos();
    this->setPos(newPos);
    setCursor(Qt::OpenHandCursor);
  } else {
    qreal oldX = originalPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(oldX);
    this->setPos(newPos);
  }
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

void EventItem::setOriginalPos(qreal x, qreal y) {
  originalPos = QPointF(x, y);
}

int EventItem::getCorrection() {
  return width - 29;
}

QPointF EventItem::getOriginalPos() {
  return originalPos;
}

int EventItem::getId() {
  return id;
}

