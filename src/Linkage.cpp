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

Linkage::Linkage(QString type, QString coder, QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  _color = Qt::black;
  setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  _typeInd = type;
  _coder = coder;
  _copy = false;
  _height = 0;
  _penWidth = 1;
  _penStyle = 1;
  _massHidden = false;
}

QRectF Linkage::boundingRect() const
{
  if (_height > 0)
    {
      return _strokePath.controlPointRect();
    }
  else
    {
    return QGraphicsLineItem::boundingRect(); 
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
  calculate();
  if (line().length() > 0)
    {
      _arrowHead.clear();
      _arrowHead << _ghostLine.p2() << _arrowP1 << _arrowP2;
      QPainterPath myPath;
      myPath.moveTo(_newLine.p1());
      myPath.quadTo(_controlPoint, _ghostLine.p2());
      _strokePath = myPath;
      painter->setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      painter->setBrush(_color);
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
  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(_start);
  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(_end);
  MacroEvent *startMacro = qgraphicsitem_cast<MacroEvent*>(_start);
  MacroEvent *endMacro = qgraphicsitem_cast<MacroEvent*>(_end);
  OccurrenceItem *occurrenceStart = qgraphicsitem_cast<OccurrenceItem*>(_start);
  OccurrenceItem *occurrenceEnd = qgraphicsitem_cast<OccurrenceItem*>(_end);
  if (startEvent && endEvent)
    {
      // startX is middle of start node
      qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
      // endX is middle of end node
      qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node
      qreal endStart = endEvent->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endEvent->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(startX, endEvent->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				    endEvent->pos().y())));
	}
    }
  else if (startEvent && endMacro)
    {
      // startX is middle of start node
      qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endMacro->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endMacro->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(startX, endMacro->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				    endMacro->pos().y())));
	}
    }
  else if (startMacro && endMacro)
    {
      // startX is middle of start node
      qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endMacro->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endMacro->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(startX, endMacro->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				    endMacro->pos().y())));
	}
    }
  else if (startMacro && endEvent)
    {
      // startX is middle of start node
      qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endEvent->pos().x() - 20;
      if (startX >= _end->pos().x() - 20 && startX <= _end->pos().x() - 20 + endEvent->getWidth())
	{
	  _newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(startX, endEvent->pos().y())));
	}
      else if (startX <= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
	}
      else if (startX >= endStart)
	{
	  _newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				    endEvent->pos().y())));
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
}

void Linkage::setColor(const QColor &color)
{
  _color = color;
}

QColor Linkage::getColor()
{
  return _color;
}

QGraphicsItem* Linkage::startItem() const
{
  return _start;
}

QGraphicsItem* Linkage::endItem() const
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

void Linkage::setHeight(int height)
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
