#include "../include/OccurrenceLabel.h"
#include <QGraphicsScene>
#include <QtCore>

OccurrenceLabel::OccurrenceLabel(OccurrenceItem *submittedOccurrence) {
  occurrence = submittedOccurrence;
  xOffset = 0;
  yOffset = -30;
};

OccurrenceItem* OccurrenceLabel::getOccurrence() {
  return occurrence;
}

void OccurrenceLabel::setNewPos(QPointF nodePos, qreal xOffsetincrement, qreal yOffsetincrement) {
  xOffset += xOffsetincrement;
  yOffset += yOffsetincrement;
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() - (textWidth() / 2) + xOffset);
  newPos.setY(newPos.y() - 12 + yOffset);
  this->setPos(newPos);
}

QPointF OccurrenceLabel::getOffset() {
  return QPointF(xOffset, yOffset);
}

void OccurrenceLabel::setOffset(QPointF offset) {
  xOffset = offset.x();
  yOffset = offset.y();
}
