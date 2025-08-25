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

  ***********

  The technique for drawing arrowhead polygons is based upon examples
  provided by the Qt Company under the BSD license,
  with the copyright shown below:

  ** Copyright (C) 2016 The Qt Company Ltd.
  ** Contact: https://www.qt.io/licensing/

  http://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-arrow-cpp.html
  http://doc.qt.io/qt-5/qtwidgets-graphicsview-diagramscene-arrow-h.html

*/

#include "../include/DirectedEdge.h"
#include <cmath>
#include <QtMath> 
#include <QPen>
#include <QPainter>
#include <QtCore>

constexpr qreal PI = 3.14159265358979323846;

DirectedEdge::DirectedEdge(NetworkNode *start, NetworkNode *end, QString type,
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
  _antialiasing = true;
  _loopTheta = 0.0;
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF DirectedEdge::boundingRect() const 
{
  if (_start == _end) {
    const qreal m = qMax<qreal>(8.0, _penWidth + 4.0);  // smaller than before
    return _strokePath.boundingRect().marginsAdded(QMarginsF(m, m, m, m));
  }
  return _strokePath.controlPointRect().marginsAdded(QMarginsF(5.0, 5.0, 5.0, 5.0));
}

QRectF DirectedEdge::ellipseRectThroughAnchor(const QPointF& anchor,
                                              qreal rx, qreal ry,
                                              qreal thetaRad)
{
    const qreal cx = anchor.x() - rx * std::cos(thetaRad);
    const qreal cy = anchor.y() - ry * std::sin(thetaRad);
    return QRectF(cx - rx, cy - ry, 2*rx, 2*ry);
}

void DirectedEdge::updatePosition()
{
  QPainterPath myPath;
  if (_start == _end) {
    constexpr qreal NODE_RADIUS = 10.0;    // node’s visual radius
    constexpr qreal MIN_GAP = 1.5;         // small offset from node edge
    constexpr qreal SELFLOOP_SCALE = 0.15; 

    const qreal theta = _loopTheta;
    const qreal r =
        NODE_RADIUS + MIN_GAP + SELFLOOP_SCALE * qMax<qreal>(0.0, _height / 2.0);

    const QPointF nodeCenter =
        _start->mapToScene(_start->boundingRect().center());

    prepareGeometryChange();
    QPainterPath myPath;
    myPath.addEllipse(ellipseRectThroughAnchor(nodeCenter, r, r, theta));
    _strokePath = myPath;
    return;
  }
  // non self-loop
  myPath.moveTo(_start->pos());
  calculate();
  myPath.quadTo(_controlPoint, _ghostLine.p2());
  _strokePath = myPath;
}

void DirectedEdge::calculate() 
{
  // Calculate the distance between the two nodes
  qreal dX = _end->pos().x() - _start->pos().x();
  qreal dY = _end->pos().y() - _start->pos().y();
  qreal distance = std::hypot(dX, dY);
  // We 'imagine' a line between the two nodes but reduce its length
  QLineF newLine = QLineF(_start->pos(), _end->pos());
  newLine.setLength(newLine.length() - 18 - (_penWidth - 2.0f));
  // We calculate the midpoint of this shortened line
  qreal mX = (_start->pos().x() + newLine.p2().x()) / 2;
  qreal mY = (_start->pos().y() + newLine.p2().y()) / 2;
  // And then we calculate the control point somewhere 'above' it
  qreal cX = _height * (-1 * (dY / distance)) + mX;
  qreal cY = _height * (dX / distance) + mY;
  _controlPoint = QPointF(cX, cY);
  // We make another imaginary line from the control point to the end point
  // This is to orient our arrow
  _ghostLine = QLineF(_controlPoint, _end->pos());
  if (_ghostLine.length() > 18)
  {
    _ghostLine.setLength(_ghostLine.length() - 18);
  }
  // Now we make our arrow
  double angle = std::acos(_ghostLine.dx() / _ghostLine.length());
  if (_ghostLine.dy() >= 0) {
    angle = (2 * PI) - angle;
  }
  qreal arrowSize = 10 + _penWidth;
  _arrowP1 = _ghostLine.p2() - QPointF(std::sin(angle + PI/3) * arrowSize,
				       std::cos(angle + PI/3) * arrowSize);
  _arrowP2 = _ghostLine.p2() - QPointF(std::sin(angle + PI - PI/3) * arrowSize,
				       std::cos(angle + PI - PI/3) * arrowSize);
  _arrowHead.clear();
  _arrowHead << _ghostLine.p2() << _arrowP1 << _arrowP2;
  // I believe this part is to prevent an error
  if (_ghostLine.length() > 5)
  {
    _ghostLine.setLength(_ghostLine.length() - 5);
  }
  prepareGeometryChange();
}

void DirectedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  painter->setRenderHint(QPainter::Antialiasing, _antialiasing);

  QPen edgePen(_color, _penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  painter->setPen(edgePen);
  painter->setBrush(_color);

  QPainterPath myPath;
  if (_start == _end) {
    constexpr qreal NODE_RADIUS = 10.0;
    constexpr qreal MIN_GAP = 1.5;
    constexpr qreal SELFLOOP_SCALE = 0.15;

    const qreal theta = _loopTheta;
    const qreal r =
        NODE_RADIUS + MIN_GAP + SELFLOOP_SCALE * qMax<qreal>(0.0, _height / 2.0);

    const QPointF nodeCenter =
        _start->mapToScene(_start->boundingRect().center());

    QPainterPath myPath;
    myPath.addEllipse(
        DirectedEdge::ellipseRectThroughAnchor(nodeCenter, r, r, theta));
    _strokePath = myPath;
    painter->strokePath(myPath, edgePen);
    return;
  }
  // non self-loop
  myPath.moveTo(_start->pos());
  calculate();
  myPath.quadTo(_controlPoint, _ghostLine.p2());
  _strokePath = myPath;

  painter->strokePath(myPath, edgePen);

  QPen arrowPen(_color, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  painter->setPen(arrowPen);
  painter->setBrush(_color);
  painter->drawPolygon(_arrowHead);
}

QPainterPath DirectedEdge::shape() const
{
  QPainterPathStroker stroker;
  stroker.setWidth(20);
  return stroker.createStroke(_strokePath);
}

NetworkNode* DirectedEdge::getStart() const 
{
  return _start;
}

NetworkNode* DirectedEdge::getEnd() const 
{
  return _end;
}

void DirectedEdge::setColor(const QColor &color) 
{
  _color = color;
}

QString DirectedEdge::getType() 
{
  return _relType;
}

void DirectedEdge::setType(const QString &type) 
{
  _relType = type;
}

void DirectedEdge::setHeight(const int &height) 
{
  _height = height;
}

int DirectedEdge::getHeight() 
{
  return _height;
}

int DirectedEdge::type() const 
{
  return Type;
}

QString DirectedEdge::getName() 
{
  return _name;
}

void DirectedEdge::setName(const QString &name) 
{
  _name = name;
}

bool DirectedEdge::isFiltered() 
{
  return _filtered;
}

void DirectedEdge::setFiltered(bool state) 
{
  _filtered = state;
}

bool DirectedEdge::isMassHidden() 
{
  return _massHidden;
}

void DirectedEdge::setMassHidden(bool state) 
{
  _massHidden = state;
}

void DirectedEdge::setComment(const QString &comment) 
{
  _comment = comment;
}

QString DirectedEdge::getComment() 
{
  return _comment;
}

QColor DirectedEdge::getColor() 
{
  return _color;
}

void DirectedEdge::setPenWidth(const qreal &width)
{
  _penWidth = width;
}

void DirectedEdge::setIncidents(const QSet<int> &incidents)
{
  _incidents = incidents;
}

void DirectedEdge::insertIncidents(const QSet<int> &incidents)
{
  QSetIterator<int> it(incidents);
  while (it.hasNext())
  {
    int incident = it.next();
    _incidents.insert(incident);
  }
}

void DirectedEdge::insertIncident(const int &incident)
{
  _incidents.insert(incident);
}

QSet<int> DirectedEdge::getIncidents()
{
  return _incidents;
}

void DirectedEdge::setAntialiasing(bool state)
{
  _antialiasing = state;
}

void DirectedEdge::setLoopAngle(qreal thetaRad)
{
  _loopTheta = thetaRad; 
}
