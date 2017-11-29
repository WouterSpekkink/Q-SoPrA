#include <QtWidgets>
#include "../include/NetworkNode.h"
#include "../include/Arrow.h"
#include "../include/NetworkNodeLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/


NetworkNode::NetworkNode(QString submittedName, QString submittedDescription) : color(255, 255, 255) {
  name = submittedName;
  description = submittedDescription;
  QString toolTip = "<FONT SIZE = 3>" + name + " - " + description + "</FONT>";
  setToolTip(toolTip);
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  originalPos = QPointF(0, 0);
  previousPos = originalPos;
}

/*
  I assume that this is for collision or
  selection purposes?
*/

QRectF NetworkNode::boundingRect() const {
  return QRectF(-18, -18, 36, 36);
}

void NetworkNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(color));
  painter->drawEllipse(-15, -15, 30, 30);
}

// Only to set the cursor to a different graphic.
void NetworkNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  previousPos = event->scenePos();
  setCursor(Qt::ClosedHandCursor);
}

void NetworkNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QPointF newPos = event->scenePos();
  qreal x = newPos.x();
  qreal y = newPos.y();
  bool trespass = false;

  for (int i = 0; i < scene()->items().count(); i++) {
    QGraphicsItem *item = scene()->items()[i];
    NetworkNode *currentItem = qgraphicsitem_cast<NetworkNode*>(item);
    Arrow *no = qgraphicsitem_cast<Arrow*>(item);
    if (currentItem && !(no) && item != this && item->isVisible()) {
      int dist = qSqrt(qPow(currentItem->pos().x()-x,2)+qPow(currentItem->pos().y()-y,2));
      if (dist <= 40) {
	trespass = true;
      } else {
	previousPos = this->scenePos();
      }
    }
  }
  if (trespass) {
    this->setPos(previousPos);
  } else {
    this->setPos(newPos);
  }
  if (label != NULL) {
    label->setNewPos(this->scenePos());
  }
  update();
  setCursor(Qt::OpenHandCursor);
}

void NetworkNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  {
  QPointF newPos = event->scenePos();
  qreal x = newPos.x();
  qreal y = newPos.y();

  bool trespass = false;
   for (int i = 0; i < scene()->items().count(); i++) {
    QGraphicsItem *item = scene()->items()[i];
    NetworkNode *currentItem = qgraphicsitem_cast<NetworkNode*>(item);
    Arrow *no = qgraphicsitem_cast<Arrow*>(item);
    if (currentItem && !(no) && item != this) {
      int dist = qSqrt(qPow(currentItem->pos().x()-x,2)+qPow(currentItem->pos().y()-y,2));
      if (dist <= 40) {
	trespass = true;
      } else {
	previousPos = this->scenePos();
      }
    }
  }
  if (trespass) {
    this->setPos(previousPos);
  } else {
    this->setPos(newPos);
  }
  if (label != NULL) {
    label->setNewPos(this->scenePos());
  }
  setCursor(Qt::OpenHandCursor);
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

void NetworkNode::setOriginalPos(qreal x, qreal y) {
  originalPos = QPointF(x, y);
}

int NetworkNode::getCorrection() {
  return width - 29;
}

void NetworkNode::setColor(QColor newColor) {
  color = newColor;
}

QString NetworkNode::getName() {
  return name;
}

int NetworkNode::type() const {
  return Type;
}

void NetworkNode::setLabel(NetworkNodeLabel *newLabel) {
  label = newLabel;
}

NetworkNodeLabel* NetworkNode::getLabel() {
  return label;
}
