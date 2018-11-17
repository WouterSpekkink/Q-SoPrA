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
#include "../include/OccurrenceItem.h"
#include "../include/Scene.h"
#include "../include/OccurrenceLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

OccurrenceItem::OccurrenceItem(int width, QString toolTip, QPointF originalPos,
			       int id, int order, QString attribute)
{
  _color = QColor(Qt::black);
  _width = width;
  setToolTip(breakString(toolTip));
  _originalPos = originalPos;
  _selectionColor = QColor(Qt::black);
  _id = id;
  _order = order;
  _occurrenceLabelPtr = NULL;
  _attribute = attribute;
  _permHidden = false;
  _grouped = false;

  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF OccurrenceItem::boundingRect() const 
{
  return QRectF(-26, -26, _width + 12, 52);
}

QPainterPath OccurrenceItem::shape() const 
{
  QPainterPath path;
  path.addEllipse(-20, -20, _width, 40);
  return path;
}

void OccurrenceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(_color));
  painter->drawEllipse(-20, -20, _width, 40);
}

// Only to set the cursor to a different graphic.
void OccurrenceItem::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton) 
    {
      setSelected(true);
    }
}

void OccurrenceItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
{
  Scene *myScene = qobject_cast<Scene*>(scene());
  if (myScene->getSelectedOccurrence() != NULL) 
    {
      setCursor(Qt::SizeVerCursor);
      QPointF currentPos = this->scenePos();
      qreal currentX = currentPos.x();
      QPointF newPos = event->scenePos();
      newPos.setX(currentX);
      this->setPos(newPos);
      if (_occurrenceLabelPtr != NULL) 
	{
	  _occurrenceLabelPtr->setNewPos(newPos);
	}
      myScene->relevantChange();
    }
}

void OccurrenceItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  
{
  Scene *myScene = qobject_cast<Scene*>(scene());
  if (myScene->getSelectedOccurrence() != NULL) 
    {
      QPointF currentPos = this->scenePos();
      qreal currentX = currentPos.x();
      QPointF newPos = event->scenePos();
      newPos.setX(currentX);
      this->setPos(newPos);
      if (_occurrenceLabelPtr != NULL) 
	{
	  _occurrenceLabelPtr->setNewPos(newPos);
	}
      setCursor(Qt::OpenHandCursor);
      update();
      QGraphicsItem::mouseReleaseEvent(event);
    }
  else 
    {
      setCursor(Qt::OpenHandCursor);
    }
}

int OccurrenceItem::getCorrection() 
{
  return _width - 39;
}

QPointF OccurrenceItem::getOriginalPos() const 
{
  return _originalPos;
}

int OccurrenceItem::getId() const 
{
  return _id;
}

int OccurrenceItem::getOrder() const 
{
  return _order;
}

void OccurrenceItem::setOriginalPos(const QPointF &originalPos) 
{
  _originalPos = originalPos;
}

void OccurrenceItem::setWidth(const int &width) 
{
  _width = width;
}

int OccurrenceItem::getWidth() const 
{
  return _width;
}

void OccurrenceItem::setLabel(OccurrenceLabel *occurrenceLabelPtr) 
{
  _occurrenceLabelPtr = occurrenceLabelPtr;
}

OccurrenceLabel* OccurrenceItem::getLabel() 
{
  return _occurrenceLabelPtr;
}

void OccurrenceItem::setColor(const QColor &color) 
{
  _color = color;
  update();
}

QColor OccurrenceItem::getColor() 
{
  return _color;
}

void OccurrenceItem::setSelectionColor(const QColor &color) 
{
  _selectionColor = color;
}

int OccurrenceItem::type() const 
{
  return Type;
}

QString OccurrenceItem::getAttribute() const 
{
  return _attribute;  
}

void OccurrenceItem::setAttribute(const QString &attribute) 
{
  _attribute = attribute;
}

void OccurrenceItem::setPermHidden(bool state) 
{
  _permHidden = state;
}

bool OccurrenceItem::isPermHidden() 
{
  return _permHidden;
}

void OccurrenceItem::setGrouped(bool state) 
{
  _grouped = state;
}

bool OccurrenceItem::isGrouped() 
{
  return _grouped;
}

