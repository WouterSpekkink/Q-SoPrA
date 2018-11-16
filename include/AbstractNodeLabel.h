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

#ifndef ABSTRACTNODELABEL_H
#define ABSTRACTNODELABEL_H

#include <QGraphicsTextItem>
#include "AbstractNode.h"

class AbstractNodeLabel : public QGraphicsTextItem
{

public:
  // Constructor and destructor
  AbstractNodeLabel(AbstractNode *submittedEvent = NULL);
  ~AbstractNodeLabel() {};

  // Setters
  void setNewPos(QPointF nodePos, qreal xOffsetIncrement = 0, qreal yOffsetIncrement = 0);
  void setOffset(QPointF offset);
  
  // Getters
  AbstractNode* getAbstractNode();
  QPointF getOffset();
  
  // Type checking
  enum {Type = UserType + 9};
  int type() const {return Type;}
  
private:
  // Private variables
  qreal _xOffset;
  qreal _yOffset;

  // Created by other class
  // Do not delete
  AbstractNode *_abstractNode;
};

#endif
