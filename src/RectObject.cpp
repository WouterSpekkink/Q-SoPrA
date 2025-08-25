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
#include "../include/RectObject.h"
#include "../include/Scene.h"

RectObject::RectObject() 
{
  _drawRect = QRectF(0, 0, 100, 100);
  _color = QColor(0, 0, 0, 255);
  _fillColor = QColor(Qt::transparent);
  _rotation = 0;
  _penWidth = 1;
  _penStyle = 1;
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
  _moving = false;
  _rotating = false;
  _manipulating = false;
}

void RectObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  prepareGeometryChange();
  painter->setBrush(_fillColor);
  painter->setPen(QPen(_color, _penWidth, Qt::PenStyle(_penStyle), Qt::FlatCap, Qt::MiterJoin));
  painter->drawRect(_drawRect);
  if (isSelected()) 
    {
      painter->setBrush(QColor(Qt::transparent));
      painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
      painter->drawLine(topLeft(), bottomRight());
      painter->drawLine(topRight(), bottomLeft());
      update();
    }
}

QColor RectObject::getColor() 
{
  return _color;
}

void RectObject::setColor(const QColor &color) 
{
  _color = color;
}

QColor RectObject::getFillColor() 
{
  return _fillColor;
}

void RectObject::setFillColor(const QColor &color) 
{
  _fillColor = color;
}

qreal RectObject::getLeft() 
{
  return _drawRect.left();
}

void RectObject::setLeft(const qreal &left) 
{
  _drawRect.setLeft(left);
  _drawRect = _drawRect.normalized();
}

qreal RectObject::getRight() 
{
  return _drawRect.right();
}

void RectObject::setRight(const qreal &right) 
{
  _drawRect.setRight(right);
  _drawRect = _drawRect.normalized();
}

qreal RectObject::getTop() 
{
  return _drawRect.top();
}

void RectObject::setTop(const qreal &top) 
{
  _drawRect.setTop(top);
  _drawRect = _drawRect.normalized();
}

qreal RectObject::getBottom() 
{
  return _drawRect.bottom();
}

void RectObject::setBottom(const qreal &bottom) 
{
  _drawRect.setBottom(bottom);
  _drawRect = _drawRect.normalized();
}

QPointF RectObject::topLeft() 
{
  return _drawRect.topLeft();
}

QPointF RectObject::topRight() 
{
  return _drawRect.topRight();
}

QPointF RectObject::bottomLeft() 
{
  return _drawRect.bottomLeft();
}

QPointF RectObject::bottomRight() 
{
  return _drawRect.bottomRight();
}

void RectObject::setTopLeft(const QPointF &topLeft) 
{
  _drawRect.setTopLeft(topLeft);
  _drawRect = _drawRect.normalized();
}

void RectObject::setTopRight(const QPointF &topRight) 
{
  _drawRect.setTopRight(topRight);
  _drawRect = _drawRect.normalized();
}

void RectObject::setBottomLeft(const QPointF &bottomLeft) 
{
  _drawRect.setBottomLeft(bottomLeft);
  _drawRect = _drawRect.normalized();
}

void RectObject::setBottomRight(const QPointF &bottomRight) 
{
  _drawRect.setBottomRight(bottomRight);
  _drawRect = _drawRect.normalized();
}

QPointF RectObject::getCenter() 
{
  return boundingRect().center();
}

void RectObject::moveCenter(const QPointF &center) 
{
  _drawRect.moveCenter(center);
  _drawRect = _drawRect.normalized();
}

qreal RectObject::getRotationValue() 
{
  return _rotation;
}

void RectObject::setRotationValue(const qreal &rotation) 
{
  _rotation = rotation;
  QTransform transform;
  QPointF center = _drawRect.center();
  QPointF oldC = mapToScene(_drawRect.center());
  
  transform.translate(center.x(), center.y());
  transform.rotate(_rotation);
  transform.translate(-center.x(), -center.y());
  setTransform(transform);
  _drawRect = _drawRect.normalized();

  QPointF newC = mapToScene(_drawRect.center());
  QPointF offset = oldC - newC;
  moveBy(offset.x(), offset.y());
}

QRectF RectObject::boundingRect() const 
{
  QRectF myRect = _drawRect.normalized().adjusted(-10,-10, 10, 10);
  return myRect;
}

QPainterPath RectObject::shape() const 
{
  QPainterPath path;
  QPainterPathStroker stroker;
  stroker.setWidth(_penWidth + 15);
  path.addRect(_drawRect);
  return stroker.createStroke(path);
}

int RectObject::getPenWidth() 
{
  return _penWidth;
}

void RectObject::setPenWidth(const int &width) 
{
  _penWidth = width;
}

int RectObject::getPenStyle() 
{
  return _penStyle;
}

void RectObject::setPenStyle(const int &style) 
{
  _penStyle = style;
}


int RectObject::type() const 
{
  return Type;
}

void RectObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
  else
    {
      _lastPos = this->mapToScene(this->getCenter());
      _lastEventPos = event->scenePos();
      _moving = true;
      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
}

void RectObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  Scene *scene = qobject_cast<Scene*>(this->scene());
  if (_moving)
    {
      this->resetTransform();
      QPointF newPos = event->scenePos();
      qreal xDist = sqrt(pow(newPos.x() - _lastPos.x(), 2));
      qreal yDist = sqrt(pow(newPos.y() - _lastPos.y(), 2));		 
      QPointF currentPos = this->mapToScene(this->getCenter());
      qreal newXDiff = newPos.x() - _lastEventPos.x();
      qreal newYDiff = newPos.y() - _lastEventPos.y();
      if (event->modifiers() & Qt::ControlModifier)
	{
	  if (std::abs(xDist) > std::abs(yDist))
	    {
	      QPointF newCenter = QPointF(currentPos.x() + newXDiff,
					  _lastPos.y());
	      this->moveCenter(this->mapFromScene(newCenter));
	    }
	  else if (std::abs(xDist) < std::abs(yDist))
	    {
	      QPointF newCenter = QPointF(_lastPos.x(),
					  currentPos.y() + newYDiff);
	      this->moveCenter(this->mapFromScene(newCenter));
	    }
	}
      else
	{
	  bool snappedHorizontal = false;
	  bool snappedVertical = false;
	  if (scene->isSnappingGuides() && this->getRotationValue() == 0)
	    {
	      qreal height = std::abs(this->getTop() - this->getBottom());
	      qreal width = std::abs(this->getRight() - this->getLeft());
	      QListIterator<QGraphicsItem*> it(scene->items());
	      while (it.hasNext())
		{
		  QGraphicsItem *item = it.next();
		  GuideLine *guide = qgraphicsitem_cast<GuideLine*>(item);
		  if (guide)
		    {
		      if (guide->isHorizontal())
			{
			  qreal topDist = sqrt(pow(this->getTop() -
						   guide->getOrientationPoint().y(), 2));
			  qreal bottomDist = sqrt(pow(this->getBottom() -
						      guide->getOrientationPoint().y(), 2));
			  qreal eventDist = event->scenePos().y() - guide->getOrientationPoint().y();
			  if (topDist < 10 &&
			      std::abs(eventDist) < height &&
			      eventDist > 0)
			    {
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->moveCenter(QPointF(_memPos.x(),
							   guide->getOrientationPoint().y() + height / 2));
				  _memPos = this->mapToScene(this->getCenter());
				}
			      else
				{
				  this->moveCenter(QPointF(this->mapToScene(this->getCenter()).x() + newXDiff,
							   guide->getOrientationPoint().y() + height / 2));
				  _memPos = this->mapToScene(this->getCenter());
				}
			    }
			  else if (bottomDist < 10 &&
				   std::abs(eventDist) <
				   height &&
				   eventDist < 0)
			    {
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->moveCenter(QPointF(_memPos.x(),
							   guide->getOrientationPoint().y() - height / 2));
				  _memPos = this->mapToScene(this->getCenter());
				}
			      else
				{
				  this->moveCenter(QPointF(this->mapToScene(this->getCenter()).x() + newXDiff,
							   guide->getOrientationPoint().y() - height / 2));
				  _memPos = this->mapToScene(this->getCenter());
				}
			    }
			}
		      else
			{
			  qreal leftDist = sqrt(pow(this->getLeft() -
						    guide->getOrientationPoint().x(), 2));
			  qreal rightDist = sqrt(pow(this->getRight() -
						     guide->getOrientationPoint().x(), 2));
			  qreal eventDist = event->scenePos().x() - guide->getOrientationPoint().x();
			  if (leftDist < 10 &&
			      std::abs(eventDist) < width &&
			      eventDist > 0)
			    {
			      snappedVertical = true;
			      if (snappedHorizontal)
				{
				  this->moveCenter(QPointF(guide->getOrientationPoint().x() + width / 2,
							   _memPos.y()));							       
				  _memPos = this->mapToScene(this->getCenter());
				}
			      else
				{
				  this->moveCenter(QPointF(guide->getOrientationPoint().x() + width / 2,
							   this->mapToScene(this->getCenter()).y() + newYDiff));
				  _memPos = this->mapToScene(this->getCenter());
				}
			    }
			  else if (rightDist < 10 &&
				   std::abs(eventDist) <
				   width &&
				   eventDist < 0)
			    {
			      snappedVertical = true;
			      if (snappedHorizontal)
				{
				  this->moveCenter(QPointF(guide->getOrientationPoint().x() - width / 2,
							   _memPos.y()));				
				  _memPos = this->mapToScene(this->getCenter());
				}
			      else
				{
				  this->moveCenter(QPointF(guide->getOrientationPoint().x() - width / 2,
							   this->mapToScene(this->getCenter()).y() + newYDiff));
				  _memPos = this->mapToScene(this->getCenter());
				}
			    }
			}
		    }
		}
	    }
	  if (!snappedHorizontal && !snappedVertical)
	    {			  
	      this->moveCenter(this->mapToScene(this->getCenter()) +
			       this->mapFromScene(QPointF(newXDiff, newYDiff)));
	    }
	}	
      _lastEventPos = event->scenePos();
      this->setRotationValue(this->getRotationValue());
      scene->relevantChange();
    }
  else if (_rotating)
    {
      QPointF center = this->mapToScene(this->getCenter());
      qreal dY = event->scenePos().y() - center.y();
      qreal dX = event->scenePos().x() - center.x();
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
	  qreal dist_180n = std::abs(angle + 180);
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
      this->resetTransform();
      QPointF topLeft = this->mapToScene(this->topLeft());
      QPointF bottomLeft = this->mapToScene(this->bottomLeft());
      QPointF topRight = this->mapToScene(this->topRight());
      QPointF bottomRight = this->mapToScene(this->bottomRight());
      QPointF left = this->mapToScene(QPointF(this->getLeft(),
					      this->getCenter().y()));
      QPointF right = this->mapToScene(QPointF(this->getRight(),
					       this->getCenter().y()));
      QPointF top = this->mapToScene(QPointF(this->getCenter().x(),
					     this->getTop()));
      QPointF bottom = this->mapToScene(QPointF(this->getCenter().x(),
						this->getBottom()));
      qreal distTopLeft = sqrt(pow((event->scenePos().x() - topLeft.x()), 2) +
			       pow((event->scenePos().y() - topLeft.y()), 2));
      qreal distTopRight = sqrt(pow((event->scenePos().x() - topRight.x()), 2) +
				pow((event->scenePos().y() - topRight.y()), 2));
      qreal distBottomLeft = sqrt(pow((event->scenePos().x() - bottomLeft.x()), 2) +
				  pow((event->scenePos().y() - bottomLeft.y()), 2));
      qreal distBottomRight = sqrt(pow((event->scenePos().x() - bottomRight.x()), 2) +
				   pow((event->scenePos().y() - bottomRight.y()), 2));
      qreal distLeft = sqrt(pow((event->scenePos().x() - left.x()), 2) +
			    pow((event->scenePos().y() - left.y()), 2));
      qreal distRight = sqrt(pow((event->scenePos().x() - right.x()), 2) +
			     pow((event->scenePos().y() - right.y()), 2));
      qreal distTop = sqrt(pow((event->scenePos().x() - top.x()), 2) +
			   pow((event->scenePos().y() - top.y()), 2));
      qreal distBottom = sqrt(pow((event->scenePos().x() - bottom.x()), 2) +
			      pow((event->scenePos().y() - bottom.y()), 2));
      QVector<qreal> temp;
      temp.push_back(distTopLeft);
      temp.push_back(distTopRight);
      temp.push_back(distBottomLeft);
      temp.push_back(distBottomRight);
      temp.push_back(distLeft);
      temp.push_back(distRight);
      temp.push_back(distTop);
      temp.push_back(distBottom);
      qreal minimum = -1.0;
      QVectorIterator<qreal> it(temp);
      while (it.hasNext()) 
	{
	  qreal current = it.next();
	  if (minimum == -1 || current < minimum) 
	    {
	      minimum = current;
	    }
	}
      if (minimum == distTopLeft) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setTopLeft(event->scenePos());
	  qreal xDist = sqrt(pow(_lastEventPos.x() - event->scenePos().x(), 2));
	  qreal yDist = sqrt(pow(_lastEventPos.y() - event->scenePos().y(), 2));
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      if (xDist > yDist)
		{
		  drawArea.setHeight(drawArea.width());
		}
	      else if (xDist < yDist)
		{
		  drawArea.setWidth(drawArea.height());
		}
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	    }
	}
      else if (minimum == distTopRight) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setTopRight(event->scenePos());
	  qreal xDist = sqrt(pow(_lastEventPos.x() - event->scenePos().x(), 2));
	  qreal yDist = sqrt(pow(_lastEventPos.y() - event->scenePos().y(), 2));
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      if (xDist > yDist)
		{
		  drawArea.setHeight(drawArea.width());
		}
	      else if (xDist < yDist)
		{
		  drawArea.setWidth(drawArea.height());
		}
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setTopRight(this->
				mapFromScene(drawArea.topRight()));
	    }
	}
      else if (minimum == distBottomLeft) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setBottomLeft(event->scenePos());
	  qreal xDist = sqrt(pow(_lastEventPos.x() - event->scenePos().x(), 2));
	  qreal yDist = sqrt(pow(_lastEventPos.y() - event->scenePos().y(), 2));
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      if (xDist > yDist)
		{
		  drawArea.setHeight(drawArea.width());
		}
	      else if (xDist < yDist)
		{
		  drawArea.setWidth(drawArea.height());
		}
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setBottomLeft(this->
				  mapFromScene(drawArea.bottomLeft()));
	    }
	}
      else if (minimum == distBottomRight) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setBottomRight(event->scenePos());
	  qreal xDist = sqrt(pow(_lastEventPos.x() - event->scenePos().x(), 2));
	  qreal yDist = sqrt(pow(_lastEventPos.y() - event->scenePos().y(), 2));
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      if (xDist > yDist)
		{
		  drawArea.setHeight(drawArea.width());
		}
	      else if (xDist < yDist)
		{
		  drawArea.setWidth(drawArea.height());
		}
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	}
      else if (minimum == distLeft) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setLeft(event->scenePos().x());
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      drawArea.setHeight(drawArea.width());
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setLeft(this->
			    mapFromScene(event->scenePos()).x());
	    }
	}
      else if (minimum == distRight) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setRight(event->scenePos().x());
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      drawArea.setHeight(drawArea.width());
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setRight(this->mapFromScene(event->scenePos()).x());
	    }
	}
      else if (minimum == distTop) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setTop(event->scenePos().y());
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      drawArea.setWidth(drawArea.height());
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setTop(this->mapFromScene(event->scenePos()).y());
	    }
	}
      else if (minimum == distBottom) 
	{
	  QPointF anchor = this->mapToScene(this->getCenter());
	  QRectF drawArea = QRectF(this->mapToScene(this->topLeft()),
				   this->mapToScene(this->bottomRight()));
	  drawArea.setBottom(event->scenePos().y());
	  if (event->modifiers() & Qt::ControlModifier)
	    {
	      drawArea.setWidth(drawArea.height());
	      drawArea.moveCenter(anchor);
	      this->setTopLeft(this->
			       mapFromScene(drawArea.topLeft()));
	      this->setBottomRight(this->
				   mapFromScene(drawArea.bottomRight()));
	    }
	  else
	    {
	      this->setBottom(this->mapFromScene(event->scenePos()).y());
	    }
	}
      this->setRotationValue(this->getRotationValue());
      scene->relevantChange();
    }
}

void RectObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
  _moving = false;
  _rotating = false;
  _manipulating = false;
  QApplication::restoreOverrideCursor();
}

void RectObject::hoverMoveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void RectObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::restoreOverrideCursor();
}

void RectObject::setRotating(bool state)
{
  _rotating = state;
}

bool RectObject::getRotating()
{
  return _rotating;
}
