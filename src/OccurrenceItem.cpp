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
  _highlightColor = QColor(Qt::black);
  _id = id;
  _order = order;
  _occurrenceLabelPtr = NULL;
  _attribute = attribute;
  _permHidden = false;
  _grouped = false;
  _highlighted = false;

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
  if (_highlighted)
    {
      painter->setPen(QPen(_highlightColor, 5)); 
    }
  else
    {
      painter->setPen(QPen(Qt::black, 1));
    }
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
  Scene *scene = qobject_cast<Scene*>(this->scene());
  if (scene->getSelectedOccurrence() != NULL) 
    {
      bool snappedHorizontal = false;
      if (scene->isSnappingGuides())
	{
	  QRectF drawRect = sceneBoundingRect().marginsRemoved(QMargins(6,6,6,6));
	  QListIterator<QGraphicsItem*> it(scene->items());
	  while (it.hasNext())
	    {
	      QGraphicsItem *item = it.next();
	      GuideLine *guide = qgraphicsitem_cast<GuideLine*>(item);
	      if (guide)
		{
		  if (guide->isHorizontal())
		    {
		      qreal topDist = sqrt(pow(drawRect.top() - 
					       guide->getOrientationPoint().y(), 2));
		      qreal bottomDist = sqrt(pow(drawRect.bottom() -
						  guide->getOrientationPoint().y(), 2));
		      qreal eventDist = event->scenePos().y() - guide->getOrientationPoint().y();
		      if (topDist < 10 &&
			  std::abs(eventDist) < 40 &&
			  eventDist > 0)
			{
			  snappedHorizontal = true;
			  _lastPos = this->scenePos();
			  this->setPos(this->scenePos().x(),
				       guide->getOrientationPoint().y() + 20);
			  scene->occurrencePosJumped(this, this->scenePos());
			}
		      else if (bottomDist < 10 &&
			       std::abs(eventDist) < drawRect.height() &&
			       eventDist < 0)
			{
			  snappedHorizontal = true;
			  _lastPos = this->scenePos();
			  this->setPos(this->scenePos().x(),
				       guide->getOrientationPoint().y() - 20);
			  scene->occurrencePosJumped(this, this->scenePos());
			}
		    }
		}
	    }
	}
      if (!snappedHorizontal)
	{
	  QPointF currentPos = this->scenePos();
	  qreal currentX = currentPos.x();
	  QPointF newPos = event->scenePos();
	  newPos.setX(currentX);
	  _lastPos = this->scenePos();
	  this->setPos(newPos);
	  scene->occurrencePosJumped(this, this->scenePos());
	}
      if (_occurrenceLabelPtr != NULL) 
	{
	  _occurrenceLabelPtr->setNewPos(this->scenePos());
	}
      scene->relevantChange();
    }
}

void OccurrenceItem::setLastPos(QPointF lastPos)
{
  _lastPos = lastPos;
}

QPointF OccurrenceItem::getLastPos()
{
  return _lastPos;
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

void OccurrenceItem::setId(int id)
{
  _id = id;
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

void OccurrenceItem::setHighlight(QColor highlightColor)
{
  _highlighted = true;
  _highlightColor = highlightColor;
}

void OccurrenceItem::unsetHighlight()
{
  _highlighted = false;
  _highlightColor = QColor(Qt::black);
}

bool OccurrenceItem::isHighlighted()
{
  return _highlighted;
}

QColor OccurrenceItem::getHighlightColor()
{
  return _highlightColor;
}
