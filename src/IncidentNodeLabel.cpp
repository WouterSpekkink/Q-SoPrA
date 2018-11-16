#include "../include/IncidentNodeLabel.h"
#include <QGraphicsScene>
#include <QtCore>

IncidentNodeLabel::IncidentNodeLabel(IncidentNode *submittedNode)
{
  node = submittedNode;
  xOffset = 0;
  yOffset = 0;
}

IncidentNode* IncidentNodeLabel::getNode()
{
  return node;
}

void IncidentNodeLabel::setNewPos(QPointF nodePos, qreal xOffsetincrement, qreal yOffsetincrement)
{
  xOffset += xOffsetincrement;
  yOffset += yOffsetincrement;
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() - (textWidth() / 2) + xOffset);
  newPos.setY(newPos.y() - 12 + yOffset);
  setPos(newPos);
}

QPointF IncidentNodeLabel::getOffset()
{
  return QPointF(xOffset, yOffset);
}

void IncidentNodeLabel::setOffset(QPointF offset)
{
  xOffset = offset.x();
  yOffset = offset.y();
}
