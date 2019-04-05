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
  myPath.lineTo(QPointF(_startX, (_y + _majorTickSize / 2)));
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
	  myPath.lineTo(QPointF(_startX + minorTickInterval, (_y + _minorTickSize / 2)));
	  while (myPath.currentPosition().x() + minorTickInterval <= _endX)
	    {
	      myPath.moveTo(QPointF(myPath.currentPosition().x() + minorTickInterval, _y));
	      count++;
	      if (minorTickInterval * count != _majorTickInterval)
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

void TimeLineObject::mousePressEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void TimeLineObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
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

