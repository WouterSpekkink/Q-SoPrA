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

#ifndef GuideLine_H
#define GuideLine_H

#include <QGraphicsLineItem>

class GuideLine : public QGraphicsLineItem
{

public:
  // Constructor and destructor
  GuideLine(bool horizontal = true, QGraphicsItem *parent = 0);
  ~GuideLine() {};

  // Overriding public functions
  QRectF boundingRect() const override;
  QPainterPath shape() const override;

  // Setters
  void setOrientationPoint(QPointF orientation);

  // Getters
  QPointF getOrientationPoint();

  // New function for correct drawing
  void updatePosition();
  
  // Type checking
  enum {Type = UserType + 16};
  int type() const;
  
protected:
  // Overriding protected functions
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
  void calculate();
  void mousePressEvent(QGraphicsSceneMouseEvent *) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;

private:
  QPainterPath _strokePath;
  QPointF _orientationPoint;
  QPointF _startPos;
  QPointF _endPos;
  bool _horizontal;
};

#endif
