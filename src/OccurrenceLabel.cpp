/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../include/OccurrenceLabel.h"
#include <QGraphicsScene>
#include <QtCore>

OccurrenceLabel::OccurrenceLabel(OccurrenceItem *occurrencePtr)
{
  _occurrencePtr = occurrencePtr;
  _xOffset = 0;
  _yOffset = -30;
}

OccurrenceItem* OccurrenceLabel::getOccurrence()
{
  return _occurrencePtr;
}

void OccurrenceLabel::setNewPos(QPointF nodePos, qreal xOffsetincrement, qreal yOffsetincrement)
{
  _xOffset += xOffsetincrement;
  _yOffset += yOffsetincrement;
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() - (textWidth() / 2) + _xOffset);
  newPos.setY(newPos.y() - 12 + _yOffset);
  setPos(newPos);
}

QPointF OccurrenceLabel::getOffset()
{
  return QPointF(_xOffset, _yOffset);
}

void OccurrenceLabel::setOffset(QPointF offset)
{
  _xOffset = offset.x();
  _yOffset = offset.y();
}
