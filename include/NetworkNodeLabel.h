/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2025 Wouter Spekkink  

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

#ifndef NETWORKNODELABEL_H
#define NETWORKNODELABEL_H

#include <QGraphicsTextItem>
#include "NetworkNode.h"
#include <QGraphicsSceneMouseEvent>

class NetworkNode;

class NetworkNodeLabel : public QGraphicsTextItem
{
  
public:
  // Constructor and destructor
  NetworkNodeLabel(NetworkNode *networkNodePtr = new NetworkNode("", ""));
  ~NetworkNodeLabel() {};

  // Setters
  void setNewPos(QPointF nodePos);
  void setOriginalPos(QPointF nodePos);
  void setOffset(QPointF offset);
  int getFontSize();
  
  // Getters
  NetworkNode* getNode();
  QPointF getOffset();
  void increaseFontSize();
  void decreaseFontSize();
  void setFontSize(int size);
  
  // Type checking
  enum {Type = UserType + 7};
  int type() const;
    
private:
  // Private variables
  qreal _xOffset;
  qreal _yOffset;
  int _fontSize;

  // Pointer to object created with other class
  // Do not delete
  NetworkNode *_networkNodePtr;
};

#endif
