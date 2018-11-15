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

#include <QtWidgets>
#include "../include/EventItem.h"
#include "../include/Scene.h"
#include "../include/NodeLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

EventItem::EventItem(int width,
		     QString toolTip,
		     QPointF originalPosition,
		     int id,
		     int order,
		     QGraphicsItem *parent)
  : QGraphicsItem(parent) 
{
  _color = QColor(255, 255, 255);
  _width = width;
  _originalPos = originalPosition;
  _mode = "";
  _selectionColor = QColor(Qt::black);
  _id = id;
  _order = order;
  _dislodged = false;
  _labelPtr = NULL;
  _macroEventPtr = NULL;
  _copy = false;
  setToolTip(toolTip);
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF EventItem::boundingRect() const 
{
  return QRectF(-26, -26, _width + 12, 52);
}

QPainterPath EventItem::shape() const 
{
  QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

void EventItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(_color));
  painter->drawEllipse(-20, -20, _width, 40);
  if (isSelected()) 
    {
      painter->setPen(QPen(_selectionColor, 1, Qt::DashLine));
      painter->setBrush(QBrush(Qt::transparent));
      painter->drawPath(shape());
      update();
    }
  else 
    {
      _selectionColor = QColor(Qt::black);
      update();
    }
}

// Only to set the cursor to a different graphic.
void EventItem::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton) 
    {
      setSelected(true);
    }
}

void EventItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
{
  if ((event->modifiers() & Qt::AltModifier) ||
      (isCopy() && (event->modifiers() & Qt::ControlModifier))) 
    {
      setCursor(Qt::SizeAllCursor);
      QPointF newPos = event->scenePos();
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() != _originalPos.x()) 
	{
	  _dislodged = true;
	}
      else 
	{
	  _dislodged = false;
	}
    }
  else 
    {
      setCursor(Qt::SizeVerCursor);
      qreal oldX = _originalPos.x();
      QPointF currentPos = this->scenePos();
      qreal currentX = currentPos.x();
      QPointF newPos = event->scenePos();
      newPos.setX(currentX);
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() == oldX) 
	{
	  _dislodged = false;
	}
      else 
	{
	  _dislodged = true;
	}
    }
  Scene *myScene = qobject_cast<Scene*>(scene());
  myScene->relevantChange();
}

void EventItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  
{
  if (event->modifiers() & Qt::AltModifier) 
    {
      QPointF newPos = event->scenePos();
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() != _originalPos.x()) 
	{
	  _dislodged = true;
	}
      else 
	{
	  _dislodged = false;
	}
    }
  else 
    {
      qreal oldX = _originalPos.x();
      QPointF currentPos = this->scenePos();
      qreal currentX = currentPos.x();
      QPointF newPos = event->scenePos();
      newPos.setX(currentX);
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() == oldX) 
	{
	  _dislodged = false;
	}
      else 
	{
	  _dislodged = true;
	}
    }
  setCursor(Qt::OpenHandCursor);
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

int EventItem::getCorrection() 
{
  return _width - 39;
}

QPointF EventItem::getOriginalPos() const 
{
  return _originalPos;
}

int EventItem::getId() const 
{
  return _id;
}

int EventItem::getOrder() const 
{
  return _order;
}

void EventItem::setOriginalPos(QPointF originalPos) 
{
  _originalPos = originalPos;
}

bool EventItem::isDislodged() 
{
  return _dislodged;
}

void EventItem::setDislodged(bool state) 
{
  _dislodged = state;
}

void EventItem::setWidth(int width) 
{
  prepareGeometryChange();
  _width = width;
  update();
}

int EventItem::getWidth() const 
{
  return _width;
}

void EventItem::setLabel(NodeLabel *labelPtr) 
{
  _labelPtr = labelPtr;
}

NodeLabel* EventItem::getLabel() 
{
  return _labelPtr;
}

void EventItem::setColor(const QColor &color) 
{
  _color = color;
}

QColor EventItem::getColor() 
{
  return _color;
}

void EventItem::setSelectionColor(const QColor &color) 
{
  _selectionColor = color;
}

int EventItem::type() const 
{
  return Type;
}

void EventItem::setMacroEvent(MacroEvent* eventPtr) 
{
  _macroEventPtr = eventPtr;
}

MacroEvent* EventItem::getMacroEvent() 
{
  return _macroEventPtr;
}

void EventItem::setMode(const QString mode) 
{
  _mode = mode;
}

QString EventItem::getMode() const 
{
  return _mode;
}

bool EventItem::isCopy() 
{
  return _copy;
}

void EventItem::setCopy(bool state) 
{
  _copy = state;
}
