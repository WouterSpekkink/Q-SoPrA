#include "../include/NodeLabel.h"
#include <QGraphicsScene>
#include <QtCore>

NodeLabel::NodeLabel(EventItem *submittedNode) {
  node = submittedNode;
  xOffset = 0;
  yOffset = 0;
};

EventItem* NodeLabel::getNode() {
  return node;
}

void NodeLabel::setNewPos(QPointF nodePos, qreal xOffsetincrement, qreal yOffsetincrement) {
  xOffset += xOffsetincrement;
  yOffset += yOffsetincrement;
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() - (textWidth() / 2) + xOffset);
  newPos.setY(newPos.y() - 12 + yOffset);
  this->setPos(newPos);
}

int NodeLabel::type() {
  return 3;
}

QPointF NodeLabel::getOffset() {
  return QPointF(xOffset, yOffset);
}

void NodeLabel::setOffset(QPointF offset) {
  xOffset = offset.x();
  yOffset = offset.y();
}
