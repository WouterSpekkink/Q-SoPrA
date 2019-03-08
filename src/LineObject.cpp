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
}

QRectF LineObject::boundingRect() const 
{
  return shape().boundingRect();
}

QPainterPath LineObject::shape() const 
{
  static const qreal clickTolerance = 20;
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
  QPen myPen = QPen(_color, _penWidth, Qt::PenStyle(1), Qt::RoundCap, Qt::RoundJoin); 
  painter->setPen(myPen);
  painter->strokePath(myPath, myPen);
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
  prepareGeometryChange();
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
}

void LineObject::setEndPos(QPointF endPos) 
{
  _endPos = endPos;
}

void LineObject::setStartPos(qreal x, qreal y) 
{
  _startPos = QPointF(x, y);
}

void LineObject::setEndPos(qreal x, qreal y) 
{
  _endPos = QPointF(x, y);
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

void LineObject::mousePressEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void LineObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
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

