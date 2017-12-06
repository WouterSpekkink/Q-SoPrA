#include "../include/MacroLabel.h"
#include <QGraphicsScene>
#include <QtCore>

MacroLabel::MacroLabel(MacroEvent *submittedEvent) {
  macroEvent = submittedEvent;
  xOffset = 0;
  yOffset = 0;
};

MacroEvent* MacroLabel::getMacroEvent() {
  return macroEvent;
}

void MacroLabel::setNewPos(QPointF nodePos, qreal xOffsetincrement, qreal yOffsetincrement) {
  xOffset += xOffsetincrement;
  yOffset += yOffsetincrement;
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() - (textWidth() / 2) + xOffset);
  newPos.setY(newPos.y() - 12 + yOffset);
  this->setPos(newPos);
}

QPointF MacroLabel::getOffset() {
  return QPointF(xOffset, yOffset);
}

void MacroLabel::setOffset(QPointF offset) {
  xOffset = offset.x();
  yOffset = offset.y();
}
