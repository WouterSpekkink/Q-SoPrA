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

#include "../include/Linkage.h"
#include <math.h>
#include <QtCore>
#include <QPen>
#include <QPainter>
#include "../include/Scene.h"
#include <QStyleOptionGraphicsItem>

Linkage::Linkage(QString type, QString coder, QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  _color = Qt::black;
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  _typeInd = type;
  _coder = coder;
  _copy = false;
  _height = 0;
  _penWidth = 1;
  _penStyle = 1;
  _massHidden = false;
  _antialiasing = true;
}

QRectF Linkage::boundingRect() const
{
  if (_height > 0)
    {
      return _strokePath.controlPointRect();
    }
  else
    {
      return _strokePath.boundingRect().marginsAdded(QMargins(20,20,20,20));
    }
}

QPainterPath Linkage::shape() const
{
  QPainterPathStroker stroker;
  stroker.setWidth(20);
  return stroker.createStroke(_strokePath);
}

void Linkage::updatePosition()
{
  calculate();
  QPainterPath myPath;
  myPath.moveTo(_start->pos());
  myPath.quadTo(_controlPoint, _ghostLine.p2());
  _strokePath = myPath;
}

void Linkage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  painter->setRenderHint(QPainter::Antialiasing, _antialiasing);
  calculate();
  if (line().length() > 0)
    {
      QPainterPath myPath;
      myPath.moveTo(_newLine.p1());
      myPath.quadTo(_controlPoint, _ghostLine.p2());
      _strokePath = myPath;
      painter->setPen(QPen(_color, _penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      painter->setBrush(QBrush(_color, Qt::SolidPattern));
      painter->drawPolygon(_arrowHead);
      QPen myPen = QPen(_color, _penWidth, Qt::PenStyle(_penStyle), Qt::RoundCap, Qt::RoundJoin);
      painter->strokePath(myPath, myPen);
      if (_penStyle == 1 && isSelected())
	{
	  painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
      else if (_penStyle == 4)
	{
	  painter->setPen(QPen(QColor(Qt::darkRed), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
      else if (_penStyle == 3)
	{
	  painter->setPen(QPen(QColor(Qt::darkBlue), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
      else if (_penStyle == 2)
	{
	  painter->setPen(QPen(QColor(Qt::darkGreen), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
    }
}

void Linkage::calculate() {
  prepareGeometryChange();
  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode*>(_start);
  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode*>(_end);
  AbstractNode *startAbstractNode = qgraphicsitem_cast<AbstractNode*>(_start);
  AbstractNode *endAbstractNode = qgraphicsitem_cast<AbstractNode*>(_end);
  OccurrenceItem *occurrenceStart = qgraphicsitem_cast<OccurrenceItem*>(_start);
  OccurrenceItem *occurrenceEnd = qgraphicsitem_cast<OccurrenceItem*>(_end);
  if (startIncidentNode && endIncidentNode)
    {
      // startX is middle of start node
      qreal startX = startIncidentNode->pos().x() + (startIncidentNode->getWidth() / 2) - 20;
      // endX is middle of end node
      qreal endX = endIncidentNode->pos().x() + (endIncidentNode->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node
      qreal endStart = endIncidentNode->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endIncidentNode->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startIncidentNode->pos().y()),
			   (QPointF(startX, endIncidentNode->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startIncidentNode->pos().y()),
			   (QPointF(endX - (endIncidentNode->getWidth() / 2) + 20, endIncidentNode->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startIncidentNode->pos().y()),
			   (QPointF(endX + (endIncidentNode->getWidth() / 2) - 20,
				    endIncidentNode->pos().y())));
	}
    }
  else if (startIncidentNode && endAbstractNode)
    {
      // startX is middle of start node
      qreal startX = startIncidentNode->pos().x() + (startIncidentNode->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endAbstractNode->pos().x() + (endAbstractNode->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endAbstractNode->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endAbstractNode->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startIncidentNode->pos().y()),
			   (QPointF(startX, endAbstractNode->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startIncidentNode->pos().y()),
			   (QPointF(endX - (endAbstractNode->getWidth() / 2) + 20, endAbstractNode->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startIncidentNode->pos().y()),
			   (QPointF(endX + (endAbstractNode->getWidth() / 2) - 20,
				    endAbstractNode->pos().y())));
	}
    }
  else if (startAbstractNode && endAbstractNode)
    {
      // startX is middle of start node
      qreal startX = startAbstractNode->pos().x() + (startAbstractNode->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endAbstractNode->pos().x() + (endAbstractNode->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endAbstractNode->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endAbstractNode->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startAbstractNode->pos().y()),
			   (QPointF(startX, endAbstractNode->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startAbstractNode->pos().y()),
			   (QPointF(endX - (endAbstractNode->getWidth() / 2) + 20, endAbstractNode->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startAbstractNode->pos().y()),
			   (QPointF(endX + (endAbstractNode->getWidth() / 2) - 20,
				    endAbstractNode->pos().y())));
	}
    }
  else if (startAbstractNode && endIncidentNode)
    {
      // startX is middle of start node
      qreal startX = startAbstractNode->pos().x() + (startAbstractNode->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endIncidentNode->pos().x() + (endIncidentNode->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endIncidentNode->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endIncidentNode->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startAbstractNode->pos().y()),
			   (QPointF(startX, endIncidentNode->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startAbstractNode->pos().y()),
			   (QPointF(endX - (endIncidentNode->getWidth() / 2) + 20, endIncidentNode->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startAbstractNode->pos().y()),
			   (QPointF(endX + (endIncidentNode->getWidth() / 2) - 20,
				    endIncidentNode->pos().y())));
	}
    }
  else if (occurrenceStart && occurrenceEnd)
    {
      // startX is middle of start node
      qreal startX = occurrenceStart->pos().x() + (occurrenceStart->getWidth() / 2) - 20;
      // endX is middle of end node
      qreal endX = occurrenceEnd->pos().x() + (occurrenceEnd->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node
      qreal endStart = occurrenceEnd->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + occurrenceEnd->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
			   (QPointF(startX, occurrenceEnd->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
			   (QPointF(endX - (occurrenceEnd->getWidth() / 2) + 20,
				    occurrenceEnd->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, occurrenceStart->pos().y() - 20),
			   (QPointF(endX + (occurrenceEnd->getWidth() / 2) - 20,
				    occurrenceEnd->pos().y())));
	}
    }
  qreal dY = _newLine.p2().y() - _newLine.p1().y();
  qreal dX = _newLine.p2().x() - _newLine.p1().x();
  qreal mX = (_newLine.p1().x() + _newLine.p2().x()) / 2;
  qreal mY = (_newLine.p1().y() + _newLine.p2().y()) / 2;
  qreal distance = sqrt(pow(dX, 2) + pow(dY, 2));
  qreal cX = _height * (-1 * (dY / distance)) + mX;
  qreal cY = _height * (dX / distance) + mY;
  _controlPoint = QPointF(cX, cY);
  _ghostLine = QLineF(_controlPoint, _newLine.p2());
  _ghostLine.setLength(_ghostLine.length() - 28);
  double angle = ::acos(_ghostLine.dx() / _ghostLine.length());
  if (_ghostLine.dy() >= 0)
    {
      angle = (Pi * 2) - angle;
    }
  qreal arrowSize = 10;
  _arrowP1 = _ghostLine.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
				     cos(angle + Pi / 3) * arrowSize);
  _arrowP2 = _ghostLine.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				     cos(angle + Pi - Pi / 3) * arrowSize);
  setLine(_newLine);
  _arrowHead.clear();
  _arrowHead << _ghostLine.p2() << _arrowP1 << _arrowP2;
  _ghostLine.setLength(_ghostLine.length() - 2);
  prepareGeometryChange();
}

void Linkage::setColor(const QColor &color)
{
  _color = color;
}

QColor Linkage::getColor()
{
  return _color;
}

QGraphicsItem* Linkage::getStart() const
{
  return _start;
}

QGraphicsItem* Linkage::getEnd() const
{
  return _end;
}

int Linkage::type() const
{
  return Type;
}

bool Linkage::isCopy()
{
  return _copy;
}

void Linkage::setCopy(bool state)
{
  _copy = state;
}

QString Linkage::getType()
{
  return _typeInd;
}

void Linkage::setStartItem(QGraphicsItem *start)
{
  _start = start;
}

void Linkage::setEndItem(QGraphicsItem *end)
{
  _end = end;
}

void Linkage::setPenStyle(int penStyle)
{
  _penStyle = penStyle;
}

int Linkage::getPenStyle()
{
  return _penStyle;
}

void Linkage::setPenWidth(int penWidth)
{
  _penWidth = penWidth;
}

int Linkage::getPenWidth()
{
  return _penWidth;
}

void Linkage::setHeight(const int &height)
{
  _height = height;
}

int Linkage::getHeight()
{
  return _height;
}

void Linkage::setMassHidden(bool state)
{
  _massHidden = state;
}

bool Linkage::isMassHidden()
{
  return _massHidden;
}

void Linkage::setAntialiasing(bool state)
{
  _antialiasing = state;
}
