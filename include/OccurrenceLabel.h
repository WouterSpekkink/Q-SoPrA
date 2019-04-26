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

#ifndef OCCURRENCELABEL_H
#define OCCURRENCELABEL_H

#include <QGraphicsTextItem>
#include "OccurrenceItem.h"

class OccurrenceLabel : public QGraphicsTextItem
{

public:
  // Constructor and destructor
  OccurrenceLabel(OccurrenceItem *occurrencePtr = new OccurrenceItem(30, "", QPointF(0,0), -1, -1, ""));
  ~OccurrenceLabel() {};

  // Setters
  void setNewPos(QPointF nodePos, qreal xOffsetIncrement = 0, qreal yOffsetIncrement = 0);
  void setOffset(QPointF offset);
  void setFontSize(int size);
  
  // Getters
  OccurrenceItem* getOccurrence();
  QPointF getOffset();
  int getFontSize();
  
  // Type checking
  enum {Type = UserType + 3};
  int type() const {return Type;}
  
private:
  // Private variables
  qreal _xOffset;
  qreal _yOffset;

  // Pointer to occurrence item, created by other class
  // Do not delete
  OccurrenceItem *_occurrencePtr;  
};

#endif
