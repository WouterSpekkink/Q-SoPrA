#include <QtWidgets>
#include "../include/EventItem.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

EventItem::EventItem(int subWidth, QString toolTip, QPointF originalPosition, int subId) : color(255, 255, 255) {
  width = subWidth;
  setToolTip(toolTip);
  originalPos = originalPosition;

  id = subId;
  dislodged = false;
  
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

/*
  I assume that this is for collision or
  selection purposes?
*/

QRectF EventItem::boundingRect() const {
  return QRectF(-17.5, -17.5, width + 5, 35);
}

void EventItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(color));
  painter->drawEllipse(-15, -15, width, 30);
  if (isSelected()) {
    painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
    painter->setBrush(QBrush(Qt::transparent));
    painter->drawEllipse(-17, -17, width + 4, 34);
  }
}

// Only to set the cursor to a different graphic.
void EventItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setSelected(true);
    //    setCursor(Qt::ClosedHandCursor);
  }
}

void EventItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->modifiers() & Qt::AltModifier) {
    setCursor(Qt::SizeAllCursor);
    QPointF newPos = event->scenePos();
    this->setPos(newPos);
    if (newPos.x() != originalPos.x()) {
      dislodged = true;
    } else {
      dislodged = false;
    }
  } else {
    setCursor(Qt::SizeVerCursor);
    qreal oldX = originalPos.x();
    QPointF currentPos = this->scenePos();
    qreal currentX = currentPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(currentX);
    this->setPos(newPos);
    if (newPos.x() == oldX) {
      dislodged = false;
    } else {
      dislodged = true;
    }
  }
}

void EventItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  {
  if (event->modifiers() & Qt::AltModifier) {
    QPointF newPos = event->scenePos();
    this->setPos(newPos);
    if (newPos.x() != originalPos.x()) {
      dislodged = true;
    } else {
      dislodged = false;
    }
    setCursor(Qt::OpenHandCursor);
  } else if (event->modifiers() & Qt::ShiftModifier) {
    setCursor(Qt::OpenHandCursor);
  } else {
    qreal oldX = originalPos.x();
    QPointF currentPos = this->scenePos();
    qreal currentX = currentPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(currentX);
    this->setPos(newPos);
    if (newPos.x() == oldX) {
      dislodged = false;
    } else {
      dislodged = true;
    }
    setCursor(Qt::OpenHandCursor);
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

void EventItem::resetOriginalPos(QPointF &newPos) {
  originalPos = newPos;
}

bool EventItem::isDislodged() {
  return dislodged;
}

void EventItem::setDislodged(bool state) {
  dislodged = state;
}
