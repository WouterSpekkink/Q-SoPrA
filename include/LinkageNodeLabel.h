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

#ifndef LINKAGENODELABEL_H
#define LINKAGENODELABEL_H

#include <QGraphicsTextItem>
#include <QFontMetrics>
#include "LinkageNode.h"

class LinkageNodeLabel : public QGraphicsTextItem
{

public:
  // Constructor and destructor
  LinkageNodeLabel(LinkageNode *linkageNode = new LinkageNode(QPointF(0,0), QString(), -1, -1));
  ~LinkageNodeLabel() {};

  // Setters
  void setNewPos(QPointF nodePos);
  
  // Getters
  LinkageNode* getNode();
  QPointF getOffset();
  int getFontSize();

  // Type checking
  enum {Type = UserType + 19};
  int type() const {return Type;}
 
private:
  // Created by other class
  // Do not delete.
  LinkageNode *_node;
};

#endif
