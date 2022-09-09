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

#ifndef TIMELINEOBJECT_H
#define TIMELINEOBJECT_H

#include <QGraphicsLineItem>

class TimeLineObject : public QGraphicsLineItem
{

public:
  // Constructor and destructor
  TimeLineObject(qreal startX = 0.0,
		 qreal endX = 0.0,
		 qreal y = 0.0,
		 qreal majorTickInterval = 100.0,
		 qreal minorTickDivision = 2.0,
		 qreal majorTickSize = 10.0,
		 qreal minorTickSize = 5.0,
		 QGraphicsItem *parent = 0);
  ~TimeLineObject() {};

  // Overriding public functions
  QRectF boundingRect() const override;
  QPainterPath shape() const override;

  // Setters
  void setColor(const QColor &color);
  void setStartX(qreal startX);
  void setEndX(qreal endX);
  void setMajorTickInterval(qreal majorTickInterval);
  void setMinorTickDivision(qreal minorTickDivision);
  void setMajorTickSize(qreal majorTickSize);
  void setMinorTickSize(qreal minorTickSize);
  void setY(qreal y);
  void setPenWidth(int width);
  void setFirstTick(bool state);
  void setForceLastTick(bool state);
  
  // Getters
  QColor getColor();
  qreal getStartX();
  qreal getEndX();
  qreal getY();
  int getPenWidth();
  qreal getMajorTickInterval();
  qreal getMinorTickDivision();
  qreal getMajorTickSize();
  qreal getMinorTickSize();
  bool getFirstTick();
  bool getForceLastTick();

  // Type checking
  enum {Type = UserType + 15};
  int type() const;
  
protected:
  // Overriding protected functions
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
  //void calculate();
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;
  
private:
  // Private variables
  qreal _startX;
  qreal _endX;
  qreal _y;
  qreal _majorTickInterval;
  qreal _minorTickDivision;
  qreal _majorTickSize;
  qreal _minorTickSize;
  QPointF _lastEventPos;
  qreal _lastStartX;
  qreal _lastEndX;
  qreal _lastY;
  QPointF _lastCenterPos;
  qreal _memStartX;
  qreal _memEndX;
  qreal _memY;
  QColor _color;
  int _penWidth;
  bool _firstTick;
  bool _forceLastTick;
  bool _moving;
  bool _manipulating;
  
};

#endif
