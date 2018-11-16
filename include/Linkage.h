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

***********

The technique for drawing arrowhead polygons is based upon examples 
provided by the Qt Company under the BSD license, 
with the copyright shown below:

** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/

http://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-arrow-cpp.html
http://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-arrow-h.html

*/


#ifndef LINKAGE_H
#define LINKAGE_H

#include <QGraphicsLineItem>
#include "IncidentNode.h"
#include "AbstractNode.h"
#include "OccurrenceItem.h"

class Linkage : public QGraphicsLineItem
{

public:
  // Constructor and desconstructor
  Linkage(QString type, QString coder, QGraphicsItem *parent = 0);
  // Although this class does have pointers
  // we don't want to delete these, because they may still be
  // in use by others.
  // The memory addressed they point to are not allocated
  // by this class.
  ~Linkage() {};

  // Overrides of public functions
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void setPenStyle(int penStyle);

  // New function required for correct drawing
  void updatePosition();
    
  // Setters
  void setStartItem(QGraphicsItem *start);
  void setEndItem(QGraphicsItem *end);
  void setCopy(bool status);
  void setHeight(const int &height);
  void setMassHidden(bool state);
  void setColor(const QColor &color);
  
  // Getters
  QGraphicsItem *getStart() const;
  QGraphicsItem *getEnd() const;
  bool isCopy();
  int getHeight();
  QString getType();
  int getPenStyle();
  bool isMassHidden();
  QColor getColor();
  
  // Type check
  enum {Type = UserType + 2};
  int type() const;
  
protected:
  // Override of the paint function
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  // Function that supports paint function with necessary calculations
  void calculate();

private:
  // Private variables
  QColor _color;
  QPolygonF _arrowHead;
  QPainterPath _strokePath;
  QString _typeInd;
  QString _coder;
  QLineF _newLine;
  QLineF _ghostLine;
  QPointF _arrowP1;
  QPointF _arrowP2;
  QPointF _controlPoint;
  bool _copy;
  bool _massHidden;
  int _penWidth;
  int _penStyle;
  int _height;

  // Pointers to memory addresses that are maintained by other classes
  // Do not delete.
  QGraphicsItem *_start;
  QGraphicsItem *_end;

};

#endif
