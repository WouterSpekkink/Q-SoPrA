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

#include "../include/UndirectedEdge.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include <QtCore>

UndirectedEdge::UndirectedEdge(NetworkNode *start, NetworkNode *end, QString type,
			       QString name, QGraphicsItem *parent)
  : QGraphicsLineItem(parent) 
{
  _start = start;
  _end = end;
  _color = Qt::black;
  _penWidth = 1.0f;
  _height = 20;
  _relType = type;
  _name = name;
  _filtered = true;
  _massHidden = false;
  _comment = "";
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF UndirectedEdge::boundingRect() const 
{
  return _strokePath.controlPointRect(); 
}

void UndirectedEdge::updatePosition() 
{
  calculate();
  QPainterPath myPath;
  myPath.moveTo(_ghostLineTwo.p2());
  myPath.quadTo(_controlPoint, _ghostLineOne.p2());
  myPath.quadTo(_controlPoint, _ghostLineOne.p2());
  _strokePath = myPath;
}

void UndirectedEdge::calculate() 
{
  // Let us first calculate the distance between our two points.
  qreal dX = _end->pos().x() - _start->pos().x();
  qreal dY = _end->pos().y() - _start->pos().y();
  qreal distance = sqrt(pow(dX, 2) + pow(dY, 2));

  QLineF newLine = QLineF(_start->pos(), _end->pos());
  newLine.setLength(newLine.length() - 18 - ((_penWidth / 2) - 2.0f));
  qreal mX = (_start->pos().x() + newLine.p2().x()) / 2;
  qreal mY = (_start->pos().y() + newLine.p2().y()) / 2;
  qreal cX = _height * (-1 * (dY / distance)) + mX;
  qreal cY = _height * (dX / distance) + mY;
  _controlPoint = QPointF(cX, cY);
  _ghostLineOne = QLineF(_controlPoint, _end->pos());
  _ghostLineOne.setLength(_ghostLineOne.length() - 18 - ((_penWidth / 2) - 2.0f));
  
  newLine = QLineF(_end->pos(), _start->pos());
  newLine.setLength(newLine.length() - 18 - ((_penWidth / 2) - 2.0f));
  mX = (_end->pos().x() + newLine.p2().x()) / 2;
  mY = (_end->pos().y() + newLine.p2().y()) / 2;
  cX = _height * (-1 * (dY / distance)) + mX;
  cY = _height * (dX / distance) + mY;
  _controlPoint = QPointF(cX, cY);
  _ghostLineTwo = QLineF(_controlPoint, _start->pos());
  _ghostLineTwo.setLength(_ghostLineTwo.length() - 18 - ((_penWidth / 2) - 2.0f));
  // Then we do some calculations to determine how our arrows are drawn.
  double angle = ::acos(_ghostLineOne.dx() / _ghostLineOne.length());
  if (_ghostLineOne.dy() >= 0)
    angle = (Pi * 2) - angle;
  double angle2 = ::acos(_ghostLineTwo.dx() / _ghostLineTwo.length());
  if (_ghostLineTwo.dy() >= 0)
    angle2 = (Pi * 2) - angle2;
  qreal arrowSize = 10;
  _arrowP1 = _ghostLineOne.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
					cos(angle + Pi / 3) * arrowSize);
  _arrowP2 = _ghostLineOne.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
					cos(angle + Pi - Pi / 3) * arrowSize);
  _arrowP3 = _ghostLineTwo.p2() - QPointF(sin(angle2 + Pi /3) * arrowSize,
					cos(angle2 + Pi / 3) * arrowSize);
  _arrowP4 = _ghostLineTwo.p2() - QPointF(sin(angle2 + Pi - Pi / 3) * arrowSize,
					cos(angle2 + Pi - Pi / 3) * arrowSize);
  prepareGeometryChange();
}

void UndirectedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  setPen(QPen(_color, _penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->setPen(pen());
  painter->setBrush(_color);
  calculate();
  _arrowHeadOne.clear();
  _arrowHeadOne << _ghostLineOne.p2() << _arrowP1 << _arrowP2;
  _arrowHeadTwo.clear();
  _arrowHeadTwo << _ghostLineTwo.p2() << _arrowP3 << _arrowP4;
  QPainterPath myPath;
  myPath.moveTo(_ghostLineTwo.p2());
  myPath.quadTo(_controlPoint, _ghostLineOne.p2());
  _strokePath = myPath;
  painter->drawPolygon(_arrowHeadOne);
  painter->strokePath(myPath, pen());
  painter->drawPolygon(_arrowHeadTwo);
}

QPainterPath UndirectedEdge::shape() const
{
  QPainterPathStroker stroker;
  stroker.setWidth(20);
  return stroker.createStroke(_strokePath);
}


NetworkNode* UndirectedEdge::getStart() const 
{
  return _start;
}

NetworkNode* UndirectedEdge::getEnd() const 
{
  return _end;
}

void UndirectedEdge::setColor(const QColor &color) 
{
  _color = color;
}

QString UndirectedEdge::getType() 
{
  return _relType;
}

void UndirectedEdge::setType(const QString type) 
{
  _relType = type;
}

void UndirectedEdge::setHeight(int height) 
{
  _height = height;
}

int UndirectedEdge::getHeight() 
{
  return _height;
}

int UndirectedEdge::type() const 
{
  return Type;
}

QString UndirectedEdge::getName() 
{
  return _name;
}

void UndirectedEdge::setName(const QString name) 
{
  _name = name;
}

bool UndirectedEdge::isFiltered() 
{
  return _filtered;
}

void UndirectedEdge::setFiltered(bool state) 
{
  _filtered = state;
}

bool UndirectedEdge::isMassHidden() 
{
  return _massHidden;
}

void UndirectedEdge::setMassHidden(bool state) 
{
  _massHidden = state;
}

void UndirectedEdge::setComment(const QString comment) 
{
  _comment = comment;
}

QString UndirectedEdge::getComment() 
{
  return _comment;
}

QColor UndirectedEdge::getColor() 
{
  return _color;
}

void UndirectedEdge::setPenWidth(const qreal &width)
{
  _penWidth = width;
}

void UndirectedEdge::setIncidents(const QSet<int> &incidents)
{
  _incidents = incidents;
}


void UndirectedEdge::insertIncidents(const QSet<int> &incidents)
{
  QSetIterator<int> it(incidents);
  while (it.hasNext())
    {
      int incident = it.next();
      _incidents.insert(incident);
    }
}

void UndirectedEdge::insertIncident(const int &incident)
{
  _incidents.insert(incident);
}

QSet<int> UndirectedEdge::getIncidents()
{
  return _incidents;
}

