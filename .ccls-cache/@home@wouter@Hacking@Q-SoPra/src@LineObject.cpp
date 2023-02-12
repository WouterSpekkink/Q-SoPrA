#include "../include/LineObject.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include "../include/Scene.h"

LineObject::LineObject(QPointF startPos,
		       QPointF endPos,
		       QGraphicsItem *parent)
  : QGraphicsLineItem(parent) 
{
  _startPos = startPos;
  _endPos = endPos;
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  _color = Qt::black;
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  _arrow1On = false;
  _arrow2On = false;
  _penWidth = 1;
  _penStyle = 1;
  setAcceptHoverEvents(true);
  updatePosition();
  _moving = false;
  _manipulating = false;
}


QRectF LineObject::boundingRect() const 
{
  return _strokePath.boundingRect().marginsAdded(QMargins(50,50,50,50));
}

void LineObject::updatePosition() 
{
  calculate();
  QPainterPath myPath;
  myPath.moveTo(_startPos);
  myPath.lineTo(_endPos);
  _strokePath = myPath;
}

QPainterPath LineObject::shape() const 
{
  qreal clickTolerance = 19 + _penWidth;
  QPointF vec = _endPos - _startPos;
  vec = vec*(clickTolerance / sqrt(QPointF::dotProduct(vec, vec)));
  QPointF orthogonal(vec.y(), -vec.x());
  QPainterPath result(_startPos - vec + orthogonal);
  result.lineTo(_startPos - vec - orthogonal);
  result.lineTo(_endPos + vec - orthogonal);
  result.lineTo(_endPos + vec + orthogonal);
  result.closeSubpath();
  return result;
}

void LineObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  calculate();
  QPainterPath myPath;
  myPath.moveTo(_tempLine2.p2());
  myPath.lineTo(_tempLine1.p2());
  QPen myPen = QPen(_color, _penWidth, Qt::PenStyle(_penStyle), Qt::RoundCap, Qt::RoundJoin); 
  painter->setPen(myPen);
  painter->strokePath(myPath, myPen);
  _strokePath = myPath;
  myPen.setStyle(Qt::PenStyle(1));
  painter->setPen(myPen);
  if (_arrow1On) 
    {
      painter->drawPolyline(_arrowHead);
    }
  if (_arrow2On) 
    {
      painter->drawPolyline(_arrowHead2);
    }
  if (isSelected()) 
    {
      painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
      QPainterPath outline = shape();
      painter->drawPath(outline);
    }
}

void LineObject::calculate() 
{
  qreal arrowSize = 20 + _penWidth;
  QLineF newLine = QLineF(_startPos, _endPos);
  setLine(newLine);
  _tempLine1 = QLineF(_startPos, _endPos);
  _tempLine2 = QLineF(_endPos, _startPos);
  double angle = ::acos(_tempLine1.dx() / _tempLine1.length());
  if (_tempLine1.dy() >= 0)
    angle = (Pi * 2) - angle;
  double angle2 = ::acos(_tempLine2.dx() / _tempLine2.length());
  if (_tempLine2.dy() >= 0)
    angle2 = (Pi * 2) - angle2;
  _arrowP1 = _tempLine1.p2() - QPointF(sin(angle + Pi / 3) * arrowSize,
				     cos(angle + Pi / 3) * arrowSize);
  _arrowP2 = _tempLine1.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				     cos(angle + Pi - Pi / 3) * arrowSize);
  _arrowP3 = _tempLine2.p2() - QPointF(sin(angle2 + Pi /3) * arrowSize,
				     cos(angle2 + Pi / 3) * arrowSize);
  _arrowP4 = _tempLine2.p2() - QPointF(sin(angle2 + Pi - Pi / 3) * arrowSize,
				     cos(angle2 + Pi - Pi / 3) * arrowSize);
  _arrowHead.clear();
  _arrowHead <<  _arrowP1 << _tempLine1.p2() << _arrowP2;
  _arrowHead2.clear();
  _arrowHead2 << _arrowP3 << _tempLine2.p2() << _arrowP4;
  _tempLine1.setLength(_tempLine1.length() - _penWidth);
  _tempLine2.setLength(_tempLine2.length() - _penWidth);
  prepareGeometryChange();
}

void LineObject::setColor(const QColor &color) 
{
  _color = color;
}

QColor LineObject::getColor() 
{
  return _color;
}

QPointF LineObject::getStartPos() 
{
  return _startPos;
}

QPointF LineObject::getEndPos() 
{
  return _endPos;
}

void LineObject::setStartPos(QPointF startPos) 
{
  _startPos = startPos;
  updatePosition();
}

void LineObject::setEndPos(QPointF endPos) 
{
  _endPos = endPos;
  updatePosition();
}

void LineObject::setStartPos(qreal x, qreal y) 
{
  _startPos = QPointF(x, y);
  updatePosition();
}

void LineObject::setEndPos(qreal x, qreal y) 
{
  _endPos = QPointF(x, y);
  updatePosition();
}

bool LineObject::arrow1() 
{
  return _arrow1On;
}

void LineObject::setArrow1(bool state) 
{
  _arrow1On = state;
}

bool LineObject::arrow2() 
{
  return _arrow2On;
}

void LineObject::setArrow2(bool state) 
{
  _arrow2On = state;
}

int LineObject::getPenWidth() 
{
  return _penWidth;
}

void LineObject::setPenWidth(int width) 
{
  _penWidth = width;
}

int LineObject::getPenStyle() 
{
  return _penStyle;
}

void LineObject::setPenStyle(int style) 
{
  _penStyle = style;
}

int LineObject::type() const 
{
  return Type;
}

void LineObject::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->modifiers() & Qt::ShiftModifier)
    {
      _manipulating = true;
    }
  else
    {
      _moving = true;
      _lastEventPos = event->scenePos();
      _lastStartPos = this->getStartPos();
      _lastEndPos = this->getEndPos();
      _lastCenterPos = event->scenePos();
      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    }
}

void LineObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  Scene *scene = qobject_cast<Scene*>(this->scene());
  if (_moving)
    {
      QPointF newPos = event->scenePos();
      qreal xDist = sqrt(pow(newPos.x() - _lastCenterPos.x(), 2));
      qreal yDist = sqrt(pow(newPos.y() - _lastCenterPos.y(), 2));
      qreal newXDiff = newPos.x() - _lastEventPos.x();
      qreal newYDiff = newPos.y() - _lastEventPos.y();
      if (event->modifiers() & Qt::ControlModifier)
	{
	  if (std::abs(xDist) > std::abs(yDist))
	    {
	      this->setStartPos(this->mapToScene(QPointF(this->getStartPos().x() + newXDiff,
							 _lastStartPos.y())));
	      this->setEndPos(this->mapToScene(this->getEndPos().x() + newXDiff,
					       _lastEndPos.y()));
	    }
	  else if (std::abs(xDist) < std::abs(yDist))
	    {
	      this->setStartPos(this->mapToScene(QPointF(_lastStartPos.x(),
							 this->getStartPos().y() + newYDiff)));
	      this->setEndPos(this->mapToScene(QPointF(_lastEndPos.x(),
						       this->getEndPos().y() + newYDiff)));
	    }
	}
      else
	{
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
			  qreal startDist = sqrt(pow(_startPos.y() -
						   guide->getOrientationPoint().y(), 2));
			  qreal endDist = sqrt(pow(_endPos.y() -
						   guide->getOrientationPoint().y(), 2));
			  qreal eventDist = event->scenePos().y() - guide->getOrientationPoint().y();
			  if (startDist < 10 &&
			      abs(eventDist) < 100 &&
			      eventDist > 0)
			    {
			      QPointF diffPoint = this->getEndPos() - this->getStartPos();
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setStartPos(QPointF(_memStartPos.x(),
							    guide->getOrientationPoint().y()));
				  this->setEndPos(this->getStartPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			      else
				{
				  this->setStartPos(QPointF(this->getStartPos().x() + newXDiff,
							    guide->getOrientationPoint().y()));
				  this->setEndPos(this->getStartPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			    }
			  else if (endDist < 10 &&
			      abs(eventDist) < 100 &&
			      eventDist < 0)
			    {
			      QPointF diffPoint = this->getStartPos() - this->getEndPos();
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setEndPos(QPointF(_memEndPos.x(),
							  guide->getOrientationPoint().y()));
				  this->setStartPos(this->getEndPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			      else
				{
				  this->setEndPos(QPointF(this->getEndPos().x() + newXDiff,
							  guide->getOrientationPoint().y()));
				  this->setStartPos(this->getEndPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			    }
			}
		      else
			{
			  qreal startDist = sqrt(pow(_startPos.x() -
						     guide->getOrientationPoint().x(), 2));
			  qreal endDist = sqrt(pow(_endPos.x() -
						   guide->getOrientationPoint().x(), 2));
			  qreal eventDist = event->scenePos().x() - guide->getOrientationPoint().x();
			  if (startDist < 10 &&
			      abs(eventDist) < 100 &&
			      eventDist > 0)
			    {
			      QPointF diffPoint = this->getEndPos() - this->getStartPos();
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setStartPos(QPointF(guide->getOrientationPoint().x(),
							    _memStartPos.y()));
				  this->setEndPos(this->getStartPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			      else
				{
				  this->setStartPos(QPointF(guide->getOrientationPoint().x(),
							    this->getStartPos().y() + newYDiff));
				  this->setEndPos(this->getStartPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			    }
			  else if (endDist < 10 &&
				   abs(eventDist) < 100 &&
				   eventDist < 0)
			    {
			      QPointF diffPoint = this->getStartPos() - this->getEndPos();
			      snappedHorizontal = true;
			      if (snappedVertical)
				{
				  this->setEndPos(QPointF(guide->getOrientationPoint().x(),
							  _memEndPos.y()));
				  this->setStartPos(this->getEndPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			      else
				{
				  this->setEndPos(QPointF(guide->getOrientationPoint().x(),
							  this->getEndPos().y() + newYDiff));
				  this->setStartPos(this->getEndPos() + diffPoint);
				  _memStartPos = this->getStartPos();
				  _memEndPos = this->getEndPos();
				}
			    }
			}
		    }
		}
	    }
	  if (!snappedHorizontal && !snappedVertical)
	    {
	      this->setStartPos(this->mapToScene(this->getStartPos() +
						 QPointF(newXDiff, newYDiff)));
	      this->setEndPos(this->mapToScene(this->getEndPos() +
					       QPointF(newXDiff, newYDiff)));
	    }
	}
      _lastEventPos = event->scenePos();
      scene->relevantChange();
    }
  else if (_manipulating)
    {
      QPointF start = this->getStartPos();
      QPointF end = this->getEndPos();
      qreal distStart = sqrt(pow((event->scenePos().x() - start.x()), 2) +
			     pow((event->scenePos().y() - start.y()), 2));
      qreal distEnd = sqrt(pow((event->scenePos().x() - end.x()), 2) +
			   pow((event->scenePos().y() - end.y()), 2));
      if (distStart < distEnd) 
	{
	  if (event->modifiers() & Qt::ControlModifier) 
	    {
	      if (abs((event->scenePos().y() - end.y()) / (event->scenePos().x() - end.x())) < 1) 
		{
		  this->setStartPos(event->scenePos().x(), end.y());
		}
	      else 
		{
		  this->setStartPos(end.x(), event->scenePos().y());
		}
	    }
	  else 
	    {
	      this->setStartPos(event->scenePos());
	    }
	}
      else 
	{
	  if (event->modifiers() & Qt::ControlModifier) 
	    {
	      if (abs((event->scenePos().y() - start.y()) / (event->scenePos().x() - start.x())) < 1) 
		{
		  this->setEndPos(event->scenePos().x(), start.y());
		}
	      else 
		{
		  this->setEndPos(start.x(), event->scenePos().y());
		}
	    }
	  else 
	    {
	      this->setEndPos(event->scenePos());
	    }
	}
      scene->relevantChange();
    }
}

void LineObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
  _moving = false;
  _manipulating = false;
  QApplication::restoreOverrideCursor();
}

void LineObject::hoverMoveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void LineObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::restoreOverrideCursor();
}

