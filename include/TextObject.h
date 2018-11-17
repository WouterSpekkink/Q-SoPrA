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

#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include <QGraphicsTextItem>
#include <QApplication>

class TextObject : public QGraphicsTextItem
{

public:
  // Constructor and destructor
  TextObject(const QString &text = QString(), QGraphicsTextItem *parent = 0);
  ~TextObject() {};

  // Setters
  void setRotationValue(const qreal &rotation);
  
  // Getters
  qreal getRotationValue();
  QPointF getCenter();

  // Type checking
  enum {Type = UserType + 12};
  int type() const;

protected:
  // Overriding events
  void mousePressEvent(QGraphicsSceneMouseEvent *) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
  
private:
  // Private variable
  qreal _rotation;
};

#endif
