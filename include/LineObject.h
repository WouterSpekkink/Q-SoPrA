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

#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#include <QGraphicsLineItem>

class LineObject : public QGraphicsLineItem
{

public:
  // Constructor and destructor
  LineObject(QPointF startPos = QPointF(0, 0),
	     QPointF endPos = QPointF(0, 10),
	     QGraphicsItem *parent = 0);
  ~LineObject() {};

  // Overriding public functions
  QRectF boundingRect() const override;
  QPainterPath shape() const override;

  // Setters
  void setColor(const QColor &color);
  void setStartPos(QPointF startPos);
  void setEndPos(QPointF endPos);
  void setStartPos(qreal x, qreal y);
  void setEndPos(qreal x, qreal y);
  void setArrow1(bool state);
  void setArrow2(bool state);  
  void setPenWidth(int width);
  void setPenStyle(int style);
  
  // Getters
  QColor getColor();
  QPointF getStartPos();
  QPointF getEndPos();
  bool arrow1();
  bool arrow2();
  int getPenWidth();
  int getPenStyle();
  
  // New function for correct drawing
  void updatePosition();
  
  // Type checking
  enum {Type = UserType + 11};
  int type() const;
  
protected:
  // Overriding protected functions
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
  void calculate();
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;
  
private:
  // Private variables
  QPainterPath _strokePath;
  QPointF _startPos;
  QPointF _endPos;
  QPointF _lastEventPos;
  QPointF _lastStartPos;
  QPointF _lastCenterPos;
  QPointF _lastEndPos;
  QPointF _memStartPos;
  QPointF _memEndPos;
  QColor _color;
  QLineF _tempLine1;
  QLineF _tempLine2;
  QPolygonF _arrowHead;
  QPolygonF _arrowHead2;
  QPointF _arrowP1;
  QPointF _arrowP2;
  QPointF _arrowP3;
  QPointF _arrowP4;
  bool _arrow1On;
  bool _arrow2On;
  bool _moving;
  bool _manipulating;
  int _penWidth;
  int _penStyle;
  
};

#endif
