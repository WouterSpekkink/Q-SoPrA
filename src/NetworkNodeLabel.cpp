#include "../include/NetworkNodeLabel.h"
#include <QGraphicsScene>
#include <QtCore>
#include "../include/Scene.h"

NetworkNodeLabel::NetworkNodeLabel(NetworkNode *submittedNode) {
  //setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  node = submittedNode;
  xOffset = 20;
  yOffset = -15;
  fontSize = 12;
  QFont newFont = this->font();
  newFont.setPointSizeF(fontSize);
  this->setFont(newFont);
}

NetworkNode* NetworkNodeLabel::getNode() {
  return node;
}

void NetworkNodeLabel::setNewPos(QPointF nodePos) {
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() + xOffset);
  newPos.setY(newPos.y() + yOffset);
  this->setPos(newPos);
}

void NetworkNodeLabel::setOriginalPos(QPointF nodePos) {
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() + xOffset);
  newPos.setY(newPos.y() + yOffset);
  this->setPos(newPos);
}

QPointF NetworkNodeLabel::getOffset() {
  return QPointF(xOffset, yOffset);
}

void NetworkNodeLabel::setOffset(QPointF offset) {
  xOffset = offset.x();
  yOffset = offset.y();
}

int NetworkNodeLabel::type() const {
  return Type;
}


void NetworkNodeLabel::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QPointF newPos = event->scenePos();

  QPointF nodePos = this->getNode()->scenePos();

  qreal xDiff = newPos.x() - nodePos.x();
  qreal yDiff = newPos.y() - nodePos.y();

  xOffset = xDiff;
  yOffset = yDiff;

  this->setPos(newPos);
  
  update();
  Scene *myScene = qobject_cast<Scene*>(scene());
  myScene->relevantChange();
}

void NetworkNodeLabel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QPointF newPos = event->scenePos();

  QPointF nodePos = this->getNode()->scenePos();

  qreal xDiff = newPos.x() - nodePos.x();
  qreal yDiff = newPos.y() - nodePos.y();

  xOffset = xDiff;
  yOffset = yDiff;

  this->setPos(newPos);

  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

void NetworkNodeLabel::increaseFontSize() {
  if (fontSize != 24) {
    fontSize++;
  }
  QFont newFont = this->font();
  newFont.setPointSizeF(fontSize);
  this->setFont(newFont);
}

void NetworkNodeLabel::decreaseFontSize() {
  if (fontSize != 12) {
    fontSize--;
  }
  QFont newFont = this->font();
  newFont.setPointSizeF(fontSize);
  this->setFont(newFont);
}

void NetworkNodeLabel::setFontSize(int size) {
  fontSize = size;
  QFont newFont = this->font();
  newFont.setPointSizeF(fontSize);
  this->setFont(newFont);
}

int NetworkNodeLabel::getFontSize() {
  return fontSize;
}
