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

#include "../include/TextObject.h"
#include <QGraphicsSceneMouseEvent>
#include "../include/Scene.h"
#include <math.h>
#include <QtMath>

TextObject::TextObject(const QString &text,
		       QGraphicsTextItem *parent)
  : QGraphicsTextItem(parent) 
{
  setPlainText(text);
  setFlag(QGraphicsTextItem::ItemIsSelectable, true);
  _rotation = 0;
  setAcceptHoverEvents(true);
  _moving = false;
  _rotating = false;
  _manipulating = false;
}

QPainterPath TextObject::shape() const 
{
  QPainterPath path;
  path.addRect(boundingRect());
  return path;
}

qreal TextObject::getRotationValue() 
{
  return _rotation;
}

void TextObject::setRotationValue(const qreal &rotation) 
{
  _rotation = rotation;
  QTransform transform;
  QPointF center = boundingRect().center();
  QPointF oldC = mapToScene(boundingRect().center());
  
  transform.translate(center.x(), center.y());
  transform.rotate(_rotation);
  transform.translate(-center.x(), -center.y());
  setTransform(transform);

  QPointF newC = mapToScene(boundingRect().center());
  QPointF offset = oldC - newC;
  moveBy(offset.x(), offset.y());
}

QPointF TextObject::getCenter() 
{
  return boundingRect().center();
}

int TextObject::type() const 
{
  return Type;
}

void TextObject::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (_rotating)
    {
      _rotating = false;
    }
  if (event->button() == Qt::MiddleButton)
    {
      _rotating = true;
      QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
    }
  else if (event->modifiers() & Qt::ShiftModifier)
    {
      _manipulating = true;
      _lastEventPos = event->scenePos();
      _lastPos = event->scenePos();
      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
  else
    {
      this->resetTransform();
      _lastPos = this->scenePos();
      this->setRotationValue(this->getRotationValue());
      _lastEventPos = event->scenePos();
      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
      _moving = true;
    }
}

void TextObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  Scene *scene = qobject_cast<Scene*>(this->scene());
  QRectF boundsRect = this->sceneBoundingRect();
  if (_moving)
    {
      this->resetTransform();
      QPointF newPos = event->scenePos();
      qreal xDist = sqrt(std::pow(newPos.x() - _lastPos.x(), 2));
      qreal yDist = sqrt(std::pow(newPos.y() - _lastPos.y(), 2));		 
      QPointF currentPos = this->scenePos();
      qreal newXDiff = newPos.x() - _lastEventPos.x();
      qreal newYDiff = newPos.y() - _lastEventPos.y();
      if (event->modifiers() & Qt::ControlModifier)
	{
	  if (abs(xDist) > abs(yDist))
	    {
	      QPointF newCenter = QPointF(currentPos.x() + newXDiff,
					  _lastPos.y());
	      this->setPos(newCenter);
	    }
	  else if (abs(xDist) < abs(yDist))
	    {
	      QPointF newCenter = QPointF(_lastPos.x(),
					  currentPos.y() + newYDiff);
	      this->setPos(newCenter);
	    }
	}
      else
	{
	  bool snappedHorizontal = false;
	  bool snappedVertical = false;
	  if (scene->isSnappingGuides() && (this->getRotationValue() == 0))
	    {
	      QListIterator<QGraphicsItem*> it(scene->items());
	      while (it.hasNext())
		{
		  QGraphicsItem *item = it.next();
		  GuideLine *guide = qgraphicsitem_cast<GuideLine*>(item);
		  if (guide)
		    {
		      if (guide->isHorizontal())
			{
			  qreal topDist = sqrt(pow(boundsRect.top() -
						   guide->getOrientationPoint().y(), 2));
			  qreal bottomDist = sqrt(pow(boundsRect.bottom() -
						      guide->getOrientationPoint().y(), 2));
			  qreal eventDist = event->scenePos().y() - guide->getOrientationPoint().y();
			  if (topDist < 10 &&
			      abs(eventDist) < boundsRect.height() &&
			      eventDist > 0)
			    {
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setPos(_memPos.x(),
							   guide->getOrientationPoint().y());
				  _memPos = this->scenePos();
				}
			      else
				{
				  this->setPos(currentPos.x() + newXDiff,
							   guide->getOrientationPoint().y());
				  _memPos = this->scenePos();
				}
			    }
			  else if (bottomDist < 10 &&
			      abs(eventDist) < boundsRect.height() &&
			      eventDist < 0)
			    {
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setPos(_memPos.x(),
							   guide->getOrientationPoint().y() -
							   boundsRect.height());
				  _memPos = this->scenePos();
				}
			      else
				{
				  this->setPos(currentPos.x() + newXDiff,
							   guide->getOrientationPoint().y() -
							   boundsRect.height());
				  _memPos = this->scenePos();
				}
			    }
			}
		      else
			{
			  qreal leftDist = sqrt(pow(boundsRect.left() -
						    guide->getOrientationPoint().x(), 2));
			  qreal rightDist = sqrt(pow(boundsRect.right() -
						    guide->getOrientationPoint().x(), 2));
			  qreal eventDist = event->scenePos().x() - guide->getOrientationPoint().x();
			  if (leftDist < 10 &&
			      abs(eventDist) < boundsRect.width() &&
			      eventDist > 0)
			    {
			      snappedVertical = true;
			      if (snappedHorizontal)
				{
				  this->setPos(guide->getOrientationPoint().x(),
							   _memPos.y());
				  _memPos = this->scenePos();
				}
			      else
				{
				  this->setPos(guide->getOrientationPoint().x(),
							   currentPos.y() + newYDiff);
				  _memPos = this->scenePos();
				}
			    }
			  else if (rightDist < 10 &&
			      abs(eventDist) < boundsRect.width() &&
			      eventDist < 0)
			    {
			      snappedVertical = true;
			      if (snappedHorizontal)
				{
				  this->setPos(guide->getOrientationPoint().x() -
							   boundsRect.width(),
							   _memPos.y());
				  _memPos = this->scenePos();
				}
			      else
				{
				  this->setPos(guide->getOrientationPoint().x() -
							   boundsRect.width(),
							   currentPos.y() + newYDiff);
				  _memPos = this->scenePos();
				}
			    }
			}
		    }
		}
	    }
	  if (!snappedHorizontal && !snappedVertical)
	    {
	      this->setPos(currentPos + QPointF(newXDiff, newYDiff));
	    }
	}
      _lastEventPos = event->scenePos();
      this->setRotationValue(this->getRotationValue());
      scene->relevantChange();
    }
  else if (_rotating)
    {
      QPointF center = this->mapToScene(this->getCenter());
      qreal dY = center.y() - event->scenePos().y();
      qreal dX = center.x() - event->scenePos().x();
      qreal angle = atan2(dY, dX);
      angle = qRadiansToDegrees(angle);
      if (event->modifiers() & Qt::ControlModifier)
	{
	  QVector<qreal> distVector;
	  qreal dist_0 = std::abs(angle - 0);
	  qreal dist_45 = std::abs(angle - 45);
	  distVector.push_back(dist_45);
	  qreal dist_90 = std::abs(angle - 90);
	  distVector.push_back(dist_90);
	  qreal dist_135 = std::abs(angle - 135);
	  distVector.push_back(dist_135);
	  qreal dist_180 = std::abs(angle - 180);
	  distVector.push_back(dist_180);
	  qreal dist_45n = std::abs(angle + 45);
	  distVector.push_back(dist_45n);
	  qreal dist_90n = std::abs(angle + 90);
	  distVector.push_back(dist_90n);
	  qreal dist_135n = std::abs(angle + 135);
	  distVector.push_back(dist_135n);
	  qreal dist_180n = std::abs (angle + 180);
	  distVector.push_back(dist_180n);
	  qreal minDist = dist_0;
	  QVectorIterator<qreal> it(distVector);
	  while (it.hasNext())
	    {
	      qreal currentDist = it.next();
	      if (currentDist < minDist)
		{
		  minDist = currentDist;
		}
	    }
	  if (minDist == dist_0)
	    {
	      this->setRotationValue(0);
	    }
	  else if (minDist == dist_45)
	    {
	      this->setRotationValue(45);
	    }
	  else if (minDist == dist_90)
	    {
	      this->setRotationValue(90);
	    }
	  else if (minDist == dist_135)
	    {
	      this->setRotationValue(135);
	    }
	  else if (minDist == dist_180)
	    {
	      this->setRotationValue(180);
	    }
	  else if (minDist == dist_45n)
	    {
	      this->setRotationValue(-45);
	    }
	  else if (minDist == dist_90n)
	    {
	      this->setRotationValue(-90);
	    }
	  else if (minDist == dist_135n)
	    {
	      this->setRotationValue(-135);
	    }
	  else if (minDist == dist_180n)
	    {
	      this->setRotationValue(-180);
	    }
	}
      else
	{
	  this->setRotationValue(angle);
	  scene->relevantChange();
	}
    }
  else if (_manipulating)
    {
      _lastEventPos = event->scenePos();
      qreal newRight = this->scenePos().x() + _lastEventPos.x();
      qreal newHeight = ((event->scenePos().y() - _lastPos.y()) / 2);
      if (newRight >= 10)
	{
	  this->setTextWidth(_lastEventPos.x() - this->scenePos().x());
	}
      if (newHeight >= 9)
	{
	  QFont font = this->font();
	  font.setPointSize(newHeight);
	  this->setFont(font);
	}
    }
}

void TextObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) 
{
  _moving = false;
  _rotating = false;
  _manipulating = false;
  QApplication::restoreOverrideCursor();
  QGraphicsTextItem::mouseReleaseEvent(event);
}

void TextObject::hoverMoveEvent(QGraphicsSceneHoverEvent *) 
{
  setCursor(Qt::OpenHandCursor);
}

void TextObject::setRotating(bool state)
{
  _rotating = state;
}

bool TextObject::isRotating()
{
  return _rotating;
}
