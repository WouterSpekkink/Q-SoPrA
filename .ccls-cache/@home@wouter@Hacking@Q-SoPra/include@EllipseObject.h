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

#ifndef ELLIPSEOBJECT
#define ELLIPSEOBJECT

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>

class EllipseObject : public QGraphicsItem
{

public:

  // Constructor and destructor
  EllipseObject();
  ~EllipseObject() {};
  
  // Overrides of public functions
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  
  // Setters
  void setColor(const QColor &color);
  void setFillColor(const QColor &color);
  void setLeft(const qreal &left);
  void setRight(const qreal &right);
  void setTop(const qreal &top);
  void setBottom(const qreal &bottom);
  void setTopLeft(const QPointF &topLeft);
  void setTopRight(const QPointF &topRight);
  void setBottomLeft(const QPointF &bottomLeft);
  void setBottomRight(const QPointF &bottomRight);
  void setRotationValue(const qreal &rotation);
  void setPenWidth(const int &width);
  void setPenStyle(const int &style);
  void setRotating(bool state);

  // Getters
  QColor getColor();
  QColor getFillColor();
  qreal getLeft();
  qreal getRight();
  qreal getTop();
  qreal getBottom();
  QPointF topLeft();
  QPointF topRight();
  QPointF bottomLeft();
  QPointF bottomRight();
  QPointF getCenter();
  qreal getRotationValue();
  int getPenWidth();
  int getPenStyle();
  bool isRotating();

  // Custom function to move object
  void moveCenter(const QPointF &newCenter);

  // Type checking  
  enum {Type = UserType + 13};
  int type() const;

protected:
  // Override of paint function
  void paint(QPainter *painter,
	     const QStyleOptionGraphicsItem *option,
	     QWidget *widget) override;
  // Overrides of events
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;
  
private:
  // Private variables
  QRectF _drawRect;
  QPointF _lastPos;
  QPointF _lastEventPos;
  QPointF _memPos;
  QColor _color;
  QColor _fillColor;
  qreal _rotation;
  int _penWidth;
  int _penStyle;
  bool _moving;
  bool _rotating;
  bool _manipulating;
};

#endif
