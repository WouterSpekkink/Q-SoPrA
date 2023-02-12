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
#include "../include/NetworkNode.h"
#include "../include/Scene.h"
#include "../include/NetworkNodeLabel.h"

NetworkNode::NetworkNode(QString name, QString description)
{
  _color = QColor(Qt::black);
  _name = name;
  _description = description;
  QString toolTip = breakString(_name + " - " + _description);
  setToolTip(toolTip);
  _previousPos = QPointF(0, 0);
  _mode = "";
  _selectionColor = QColor(Qt::black);
  _persistent = false;
  _massHidden = false;
  _networkNodeLabelPtr = NULL;

  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF NetworkNode::boundingRect() const 
{
  return QRectF(-18, -18, 36, 36);
}

void NetworkNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(_color));
  painter->drawEllipse(-15, -15, 30, 30);

  if (isSelected()) 
    {
      painter->setPen(QPen(_selectionColor, 1, Qt::DashLine));
      painter->setBrush(QBrush(Qt::transparent));
      painter->drawEllipse(-18, -18, 36, 36);
      update();
    }
  else 
    {
      _selectionColor = QColor(Qt::black);
      update();
    }
}

// Only to set the cursor to a different graphic.
void NetworkNode::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton) 
    {
      setSelected(true);
    }
  _previousPos = event->scenePos();
  setCursor(Qt::ClosedHandCursor);
  
}

void NetworkNode::move(QPointF newPos) 
{
  Scene *scene = qobject_cast<Scene*>(this->scene());
  bool snappedHorizontal = false;
  bool snappedVertical = false;
  QRectF drawRect = sceneBoundingRect().marginsRemoved(QMargins(6,6,6,6));
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *item = it.next();
      NetworkNode *first = this;
      NetworkNode *second = qgraphicsitem_cast<NetworkNode*>(item);
      GuideLine *guide = qgraphicsitem_cast<GuideLine*>(item);
      if (scene->isSnappingGuides())
	{
	  if (guide)
	    {
	      if (guide->isHorizontal())
		{
		  qreal topDist = sqrt(pow(drawRect.top() - 
					   guide->getOrientationPoint().y(), 2));
		  qreal bottomDist = sqrt(pow(drawRect.bottom() -
					      guide->getOrientationPoint().y(), 2));
		  qreal eventDist = newPos.y() - guide->getOrientationPoint().y();
		  if (topDist < 10 &&
		      std::abs(eventDist) < 40 &&
		      eventDist > 0)
		    {
		      snappedHorizontal = true;
		      if (snappedVertical)
			{
			  this->setPos(_lastPos.x(),
				       guide->getOrientationPoint().y() + 20);
			  _lastPos = this->scenePos();
			}
		      else
			{
			  this->setPos(newPos.x(),
				       guide->getOrientationPoint().y() + 20);
			  _lastPos = this->scenePos();
			}
		    }
		  else if (bottomDist < 10 &&
			   std::abs(eventDist) < drawRect.height() &&
			   eventDist < 0)
		    {
		      snappedHorizontal = true;
		      if (snappedVertical)
			{
			  this->setPos(_lastPos.x(),
				       guide->getOrientationPoint().y() - 20);
			  _lastPos = this->scenePos();
			}
		      else
			{
			  this->setPos(newPos.x(),
				       guide->getOrientationPoint().y() - 20);
			  _lastPos = this->scenePos();
			}			  
		    }
		}
	      else
		{
		  qreal leftDist = sqrt(pow(drawRect.left() -
					    guide->getOrientationPoint().x(), 2));
		  qreal rightDist = sqrt(pow(drawRect.right() -
					     guide->getOrientationPoint().x(), 2));
		  qreal eventDist = newPos.x() -
		    guide->getOrientationPoint().x();
		  if (leftDist < 10 &&
		      std::abs(eventDist) < drawRect.width() &&
		      eventDist > 0)
		    {
		      snappedVertical = true;
		      if (snappedHorizontal)
			{
			  this->setPos(guide->getOrientationPoint().x() + 18,
				       _lastPos.y());
			  _lastPos = this->scenePos();
			}
		      else
			{
			  this->setPos(guide->getOrientationPoint().x() + 18,
				       newPos.y());
			  _lastPos = this->scenePos();
			}
		    }
		  else if (rightDist < 10 &&
			   std::abs(eventDist) < drawRect.width() &&
			   eventDist < 0)
		    {
		      snappedVertical = true;
		      if (snappedHorizontal)
			{
			  this->setPos(guide->getOrientationPoint().x() - 18,
				       _lastPos.y());
			  _lastPos = this->scenePos();
			}
		      else
			{
			  this->setPos(guide->getOrientationPoint().x() - 18,
				       newPos.y());
			  _lastPos = this->scenePos();
			}
		    }
		}
	    }
	}
      if (!snappedHorizontal && !snappedVertical)
	{
	  this->setPos(newPos);
       	}
      this->getLabel()->setNewPos(this->scenePos());
      if (second && first != second && second->isVisible()) 
	  {
	  qreal dist = qSqrt(qPow(first->scenePos().x() -
				  second->scenePos().x(), 2) +
			     qPow(first->scenePos().y() -
				  second->scenePos().y(), 2));
	  if (dist <= 30) 
	    {
	      QPointF firstPoint = first->scenePos();
	      QPointF secondPoint = second->scenePos();
	      qreal mX = (firstPoint.x() + secondPoint.x()) / 2;
	      qreal mY = (firstPoint.y() + secondPoint.y()) / 2;
	      QPointF midPoint = QPointF(mX, mY);
	      qreal secondXDiff = secondPoint.x() - midPoint.x();
	      qreal secondYDiff = secondPoint.y() - midPoint.y();
	      qreal xDiff = 0;
	      qreal yDiff = 0;
	      if (secondXDiff > 0) 
		{
		  xDiff = 3;
		}
	      else if (secondXDiff < 0) 
		{
		  xDiff = -3;
		}
	      if (secondYDiff > 0) 
		{
		  yDiff = 3;
		}
	      else if (secondYDiff < 0) 
		{
		  yDiff = -3;
		}
	      QPointF temp = QPointF(second->scenePos().x() + xDiff,
				     second->scenePos().y() + yDiff);
	      second->move(temp);
	    }
	}
    }
  scene->relevantChange();
  update();
  setCursor(Qt::ClosedHandCursor);
}

void NetworkNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
{
  this->move(event->scenePos());
  setCursor(Qt::ClosedHandCursor);  
}

int NetworkNode::getCorrection() 
{
  return 30 - 29;
}

void NetworkNode::setColor(const QColor &color) 
{
  _color = color;
}

QString NetworkNode::getName() const 
{
  return _name;
}

QString NetworkNode::getDescription() const
{
  return _description;
}

int NetworkNode::type() const 
{
  return Type;
}

void NetworkNode::setLabel(NetworkNodeLabel *networkNodeLabelPtr) 
{
  _networkNodeLabelPtr = networkNodeLabelPtr;
}

NetworkNodeLabel* NetworkNode::getLabel() 
{
  return _networkNodeLabelPtr;
}

void NetworkNode::setMode(const QString &mode) 
{
  _mode = mode;
}

QString NetworkNode::getMode() const 
{
  return _mode;
}

QColor NetworkNode::getColor() 
{
  return _color;
}

void NetworkNode::setSelectionColor(const QColor &color) 
{
  _selectionColor = color;
}

bool NetworkNode::isPersistent() 
{
  return _persistent; 
}

void NetworkNode::setPersistent(bool state) 
{
  _persistent = state;
}

bool NetworkNode::isMassHidden() 
{
  return _massHidden; 
}

void NetworkNode::setMassHidden(bool state) 
{
  _massHidden = state;
}


