#include "../include/TimeLineObject.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include "../include/Scene.h"

TimeLineObject::TimeLineObject(qreal startX,
			       qreal endX,
			       qreal y,
			       qreal majorTickInterval,
			       qreal minorTickDivision,
			       qreal majorTickSize,
			       qreal minorTickSize,
			       QGraphicsItem *parent)
  : QGraphicsLineItem(parent) 
{
  _startX = startX;
  _endX = endX;
  _y = y;
  _majorTickInterval = majorTickInterval;
  _minorTickDivision = minorTickDivision;
  _majorTickSize = majorTickSize;
  _minorTickSize = minorTickSize;
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  _color = Qt::black;
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  _penWidth = 1;
  setAcceptHoverEvents(true);
  _firstTick = true;
  _forceLastTick = false;
  _moving = false;
  _manipulating = false;
}

QRectF TimeLineObject::boundingRect() const 
{
  return shape().boundingRect();
}

QPainterPath TimeLineObject::shape() const 
{
  QPainterPath path;
  QPointF start = QPointF();
  QPointF end = QPointF();
  if (_startX <= _endX)
    {
      start = QPointF(_startX - 10, _y);
      end = QPointF(_endX + 10, _y);
    }
  else
    {
      start = QPointF(_endX - 10, _y);
      end = QPointF(_startX + 10, _y);
    }
  path.moveTo(start);
  if (_majorTickSize >= _minorTickSize)
    {
      path.lineTo(QPointF(start.x(), _y + (_majorTickSize / 2) + 10.0));
      path.lineTo(QPointF(end.x(), _y + (_majorTickSize / 2) + 10.0));
      path.lineTo(QPointF(end.x(), _y - (_majorTickSize / 2) - 10.0));
      path.lineTo(QPointF(start.x(), _y - (_majorTickSize / 2) - 10.0));
      path.lineTo(QPointF(start));
    }
  else
    {
      path.lineTo(QPointF(start.x(), _y + (_minorTickSize / 2) + 10.0));
      path.lineTo(QPointF(end.x(), _y + (_minorTickSize / 2) + 10.0));
      path.lineTo(QPointF(end.x(), _y - (_minorTickSize / 2) - 10.0));
      path.lineTo(QPointF(start.x(), _y - (_minorTickSize / 2) - 10.0));
      path.lineTo(QPointF(start));
    }
  path.closeSubpath();
  return path;
}

void TimeLineObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  prepareGeometryChange();
  QPainterPath myPath;
  // First we draw the line
  myPath.moveTo(QPointF(_startX, _y));
  myPath.lineTo(QPointF(_endX, _y));
  // Then we draw the major ticks
  myPath.moveTo(QPointF(_startX, (_y - _majorTickSize / 2)));
  if (_firstTick)
    {
      myPath.lineTo(QPointF(_startX, (_y + _majorTickSize / 2)));
    }
  if (_majorTickInterval < (abs(_endX - _startX)))
    {
      while (myPath.currentPosition().x() + _majorTickInterval <= _endX)
	{
	  myPath.moveTo(QPointF(myPath.currentPosition().x() + _majorTickInterval, _y));
	  myPath.moveTo(QPointF(myPath.currentPosition().x(), (_y - _majorTickSize / 2)));
	  myPath.lineTo(QPointF(myPath.currentPosition().x(), (_y + _majorTickSize / 2)));
	}
      // Then we draw the minor ticks
      if (_minorTickDivision > 1)
	{
	  qreal minorTickInterval = _majorTickInterval / _minorTickDivision;
	  int count = 1;
	  myPath.moveTo(QPointF(_startX + minorTickInterval, (_y - _minorTickSize / 2)));
	  if (_startX + minorTickInterval <= _endX)
	    {
	      myPath.lineTo(QPointF(_startX + minorTickInterval, (_y + _minorTickSize / 2)));
	    }
	  while (myPath.currentPosition().x() + minorTickInterval <= _endX)
	    {
	      myPath.moveTo(QPointF(myPath.currentPosition().x() + minorTickInterval, _y));
	      count++;
	      if (count != _minorTickDivision)
		{
		  myPath.moveTo(QPointF(myPath.currentPosition().x(), (_y - _minorTickSize / 2)));
		  myPath.lineTo(QPointF(myPath.currentPosition().x(), (_y + _minorTickSize / 2)));
		}
	      else
		{
		  count = 0;
		}
	    }
	}
    }
  else
    {
      // We can stil draw the minor ticks
      if (_minorTickDivision > 1)
	{
	  qreal minorTickInterval = _majorTickInterval / _minorTickDivision;
	  int count = 1;
	  myPath.moveTo(QPointF(_startX + minorTickInterval, (_y - _minorTickSize / 2)));
	  if (_startX + minorTickInterval <= _endX)
	    {
	      myPath.lineTo(QPointF(_startX + minorTickInterval, (_y + _minorTickSize / 2)));
	    }
	  while (myPath.currentPosition().x() + minorTickInterval <= _endX)
	    {
	      myPath.moveTo(QPointF(myPath.currentPosition().x() + minorTickInterval, _y));
	      count++;
	      if (count != _minorTickDivision)
		{
		  myPath.moveTo(QPointF(myPath.currentPosition().x(), (_y - _minorTickSize / 2)));
		  myPath.lineTo(QPointF(myPath.currentPosition().x(), (_y + _minorTickSize / 2)));
		}
	      else
		{
		  count = 0;
		}
	    }
	}
    }
  if (_forceLastTick)
    {
      myPath.moveTo(QPointF(_endX, _y + _majorTickSize / 2));
      myPath.lineTo(QPointF(_endX, _y - _majorTickSize / 2));
    }
  QPen myPen = QPen(_color, _penWidth, Qt::PenStyle(1), Qt::FlatCap, Qt::MiterJoin); 
  painter->setPen(myPen);
  painter->strokePath(myPath, myPen);
  if (isSelected()) 
    {
      painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
      QPainterPath outline = shape();
      painter->drawPath(outline);
    }
}

void TimeLineObject::setColor(const QColor &color) 
{
  _color = color;
}

QColor TimeLineObject::getColor() 
{
  return _color;
}

qreal TimeLineObject::getStartX() 
{
  return _startX;
}

qreal TimeLineObject::getEndX() 
{
  return _endX;
}

qreal TimeLineObject::getY() 
{
  return _y;
}

void TimeLineObject::setStartX(qreal startX) 
{
  _startX = startX;
}

void TimeLineObject::setEndX(qreal endX) 
{
  _endX = endX;
}

void TimeLineObject::setY(qreal y) 
{
  _y = y;
}

int TimeLineObject::getPenWidth() 
{
  return _penWidth;
}

void TimeLineObject::setPenWidth(int width) 
{
  _penWidth = width;
}

void TimeLineObject::setFirstTick(bool state)
{
  _firstTick = state;
}

bool TimeLineObject::getFirstTick()
{
  return _firstTick;
}

void TimeLineObject::setForceLastTick(bool state)
{
  _forceLastTick = state;
}

bool TimeLineObject::getForceLastTick()
{
  return _forceLastTick;
}

int TimeLineObject::type() const 
{
  return Type;
}

void TimeLineObject::setMajorTickInterval(qreal majorTickInterval)
{
  _majorTickInterval = majorTickInterval;
}

void TimeLineObject::setMinorTickDivision(qreal minorTickDivision)
{
  _minorTickDivision = minorTickDivision;
}

qreal TimeLineObject::getMajorTickInterval()
{
  return _majorTickInterval;
}

qreal TimeLineObject::getMinorTickDivision()
{
  return _minorTickDivision;
}

void TimeLineObject::setMajorTickSize(qreal majorTickSize)
{
  _majorTickSize = majorTickSize;
}

void TimeLineObject::setMinorTickSize(qreal minorTickSize)
{
  _minorTickSize = minorTickSize;
}

qreal TimeLineObject::getMajorTickSize()
{
  return _majorTickSize;
}

qreal TimeLineObject::getMinorTickSize()
{
  return _minorTickSize;
}

void TimeLineObject::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->modifiers() & Qt::ShiftModifier)
    {
      _manipulating = true;
    }
  else
    {
      _moving = true;
      _lastEventPos = event->scenePos();
      _lastStartX = this->getStartX();
      _lastEndX = this->getEndX();
      _lastY = this->getY();
      _lastCenterPos = event->scenePos();
    }
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void TimeLineObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  Scene *scene = qobject_cast<Scene*>(this->scene());
  if (_moving)
    {
      QPointF newPos = event->scenePos();
      qreal newXDiff = newPos.x() - _lastEventPos.x();
      qreal xDist = sqrt(pow(newPos.x() - _lastCenterPos.x(), 2));
      qreal yDist = sqrt(pow(newPos.y() - _lastCenterPos.y(), 2));
      if (event->modifiers() & Qt::ControlModifier)
	{
	  if (std::abs(xDist) > std::abs(yDist))
	    {
	      this->setStartX(this->getStartX() + newXDiff);
	      this->setEndX(this->getEndX() + newXDiff);
	      this->setY(_lastY);
	    }
	  else if (std::abs(xDist) < std::abs(yDist))
	    {
	      this->setStartX(_lastStartX);
	      this->setEndX(_lastEndX);
	      this->setY(event->scenePos().y());
	    }
	}
      else
	{
	  QRectF drawRect = this->sceneBoundingRect().marginsRemoved(QMargins(10, 10, 10, 10));
	  qreal length = this->getEndX() - this->getStartX();
	  bool snappedHorizontal = false;
	  bool snappedVertical = false;
	  if (scene->isSnappingGuides())
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
			  qreal topDist = sqrt(pow(drawRect.top() -
						   guide->getOrientationPoint().y(), 2));
			  qreal bottomDist = sqrt(pow(drawRect.bottom() -
						      guide->getOrientationPoint().y(), 2));
			  qreal eventDist = event->scenePos().y() - guide->getOrientationPoint().y();
			  if (topDist < 10 &&
			      abs(eventDist) < drawRect.height() &&
			      eventDist > 0)
			    {
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setStartX(_memStartX);
				  this->setEndX(_memEndX);
				  this->setY(guide->getOrientationPoint().y() +
					     drawRect.height() / 2);
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			      else
				{
				  this->setStartX(this->getStartX() + newXDiff);
				  this->setEndX(this->getEndX() + newXDiff);
				  this->setY(guide->getOrientationPoint().y() +
					     drawRect.height() / 2);
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			    }
			  else if (bottomDist < 10 &&
			      abs(eventDist) < drawRect.height() &&
			      eventDist < 0)
			    {
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setStartX(_memStartX);
				  this->setEndX(_memStartX);
				  this->setY(guide->getOrientationPoint().y() -
					     drawRect.height() / 2);
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			      else
				{
				  this->setStartX(this->getStartX() + newXDiff);
				  this->setEndX(this->getEndX() + newXDiff);
				  this->setY(guide->getOrientationPoint().y() -
					     drawRect.height() / 2);
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			    }
			}
		      else
			{
			  qreal leftDist = sqrt(pow(drawRect.left() -
						    guide->getOrientationPoint().x(), 2));
			  qreal rightDist = sqrt(pow(drawRect.right() -
						     guide->getOrientationPoint().x(), 2));
			  qreal eventDist = event->scenePos().x() - guide->getOrientationPoint().x();
			  if (leftDist < 10 &&
			      abs(eventDist) < 200 &&
			      eventDist > 0)
			    {
			      snappedVertical = true;
			      if (snappedHorizontal)
				{
				  this->setStartX(guide->getOrientationPoint().x());
				  this->setEndX(this->getStartX() + length);
				  this->setY(_memY);
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			      else
				{
				  this->setStartX(guide->getOrientationPoint().x());
				  this->setEndX(this->getStartX() + length);
				  this->setY(event->scenePos().y());
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			    }
			  else if (rightDist < 10 &&
			      abs(eventDist) < 200 &&
			      eventDist < 0)
			    {
			      snappedVertical = true;
			      if (snappedHorizontal)
				{
				  this->setEndX(guide->getOrientationPoint().x());
				  this->setStartX(this->getEndX() - length);
				  this->setY(_memY);
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			      else
				{
				  this->setEndX(guide->getOrientationPoint().x());
				  this->setStartX(this->getEndX() - length);
				  this->setY(event->scenePos().y());
				  _memStartX = this->getStartX();
				  _memEndX = this->getEndX();
				  _memY = this->getY();
				}
			    }
			}
		    }
		}
	    }
     	  if (!snappedHorizontal && !snappedVertical)
	    {
	      this->setStartX(this->getStartX() + newXDiff);
	      this->setEndX(this->getEndX() + newXDiff);
	      this->setY(event->scenePos().y());
	    }
	}
      _lastEventPos = event->scenePos();
      scene->relevantChange();
    }
  else if (_manipulating)
    {
      qreal startX = this->getStartX();
      qreal endX = this->getEndX();
      qreal distStartX = sqrt(pow(event->scenePos().x() - startX, 2));
      qreal distEndX = sqrt(pow(event->scenePos().x() - endX, 2));
      if (distStartX < distEndX) 
	{
	  this->setStartX(event->scenePos().x());
	}
      else 
	{
	  this->setEndX(event->scenePos().x());
	}
      scene->relevantChange();
    }
}

void TimeLineObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
  _moving = false;
  _manipulating = false;
  QApplication::restoreOverrideCursor();
}

void TimeLineObject::hoverMoveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void TimeLineObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::restoreOverrideCursor();
}

