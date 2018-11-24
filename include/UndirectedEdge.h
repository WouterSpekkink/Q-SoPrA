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

#ifndef UNDIRECTEDEDGE_H
#define UNDIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include <QSet>
#include "NetworkNode.h"
#include "SupportingFunctions.h"
#include "Constants.h"

class UndirectedEdge : public QGraphicsLineItem
{

public:
  // Constructor and destructor
  UndirectedEdge(NetworkNode *start = NULL, NetworkNode *end = NULL,
		 QString yype = QString(), QString name = QString(), QGraphicsItem *parent = 0);
  ~UndirectedEdge() {};

  // Overriding some public functions
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  
  // Setters
  void setColor(const QColor &color);
  void setType(const QString type);
  void setHeight(int height);
  void setName(const QString name);
  void setFiltered(bool state);
  void setMassHidden(bool state);
  void setComment(const QString comment);
  void setPenWidth(const qreal &width);
  void setIncidents(const QSet<int> &incidents);
  void insertIncidents(const QSet<int> &incidents);
  void insertIncident(const int &incident);
  
  // Getters
  QColor getColor();
  NetworkNode *getStart() const; 
  NetworkNode *getEnd() const;
  QString getType();
  int getHeight();
  QString getName();
  bool isFiltered();
  bool isMassHidden();
  QString getComment();
  QSet<int> getIncidents();
  
  // New public function for correct drawing
  void updatePosition();
  
  enum {Type = UserType + 5};
  int type() const;

private slots:
  // Calculation function to support paint function
  void calculate();
  
protected:
  // Overriding paint function
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
  // Private variables
  QColor _color;
  QPolygonF _arrowHeadOne;
  QPolygonF _arrowHeadTwo;
  QPainterPath _strokePath;
  int _height;
  QLineF _ghostLineOne;
  QLineF _ghostLineTwo;
  QPointF _arrowP1;
  QPointF _arrowP2;
  QPointF _arrowP3;
  QPointF _arrowP4;
  QPointF _controlPoint;
  QString _relType;
  QString _name;
  QString _comment;
  bool _filtered;
  bool _massHidden;
  qreal _penWidth;

  // Private data set
  QSet<int> _incidents;
  
  // Pointers to objects created by other class
  // Do not delete
  NetworkNode *_start;
  NetworkNode *_end;
};

#endif
