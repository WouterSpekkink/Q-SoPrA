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
#include "../include/IncidentNode.h"
#include "../include/Scene.h"
#include "../include/IncidentNodeLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

IncidentNode::IncidentNode(int width,
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
  _abstractNodePtr = NULL;
  _copy = false;
  setToolTip(toolTip);
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF IncidentNode::boundingRect() const 
{
  return QRectF(-26, -26, _width + 12, 52);
}

QPainterPath IncidentNode::shape() const 
{
  QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

void IncidentNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
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
void IncidentNode::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton) 
    {
      setSelected(true);
    }
}

void IncidentNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
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

void IncidentNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  
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

int IncidentNode::getCorrection() 
{
  return _width - 39;
}

QPointF IncidentNode::getOriginalPos() const 
{
  return _originalPos;
}

int IncidentNode::getId() const 
{
  return _id;
}

int IncidentNode::getOrder() const 
{
  return _order;
}

void IncidentNode::setOriginalPos(QPointF originalPos) 
{
  _originalPos = originalPos;
}

bool IncidentNode::isDislodged() 
{
  return _dislodged;
}

void IncidentNode::setDislodged(bool state) 
{
  _dislodged = state;
}

void IncidentNode::setWidth(int width) 
{
  prepareGeometryChange();
  _width = width;
  update();
}

int IncidentNode::getWidth() const 
{
  return _width;
}

void IncidentNode::setLabel(IncidentNodeLabel *labelPtr) 
{
  _labelPtr = labelPtr;
}

IncidentNodeLabel* IncidentNode::getLabel() 
{
  return _labelPtr;
}

void IncidentNode::setColor(const QColor &color) 
{
  _color = color;
}

QColor IncidentNode::getColor() 
{
  return _color;
}

void IncidentNode::setSelectionColor(const QColor &color) 
{
  _selectionColor = color;
}

int IncidentNode::type() const 
{
  return Type;
}

void IncidentNode::setAbstractNode(AbstractNode* eventPtr) 
{
  _abstractNodePtr = eventPtr;
}

AbstractNode* IncidentNode::getAbstractNode() 
{
  return _abstractNodePtr;
}

void IncidentNode::setMode(const QString mode) 
{
  _mode = mode;
}

QString IncidentNode::getMode() const 
{
  return _mode;
}

bool IncidentNode::isCopy() 
{
  return _copy;
}

void IncidentNode::setCopy(bool state) 
{
  _copy = state;
}
