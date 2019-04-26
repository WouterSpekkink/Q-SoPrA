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

#ifndef INCIDENTNODELABEL_H
#define INCIDENTNODELABEL_H

#include <QGraphicsTextItem>
#include <QFontMetrics>
#include "IncidentNode.h"

class IncidentNodeLabel : public QGraphicsTextItem
{

public:
  // Constructor and destructor
  IncidentNodeLabel(IncidentNode *incidentNode = new IncidentNode(30, "", QPointF(0,0), -1, -1));
  ~IncidentNodeLabel() {};

  // Setters
  void setNewPos(QPointF nodePos, qreal xOffsetIncrement = 0, qreal yOffsetIncrement = 0);
  void setOffset(QPointF offset);
  void setFontSize(int size);
  
  // Getters
  IncidentNode* getNode();
  QPointF getOffset();
  int getFontSize();

  // Type checking
  enum {Type = UserType + 3};
  int type() const {return Type;}
 
private:
  // Private variables
  qreal _xOffset;
  qreal _yOffset;

  // Created by other class
  // Do not delete.
  IncidentNode *_node;
};

#endif
