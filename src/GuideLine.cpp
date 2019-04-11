#include "../include/GuideLine.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include "../include/Scene.h"

GuideLine::GuideLine(bool horizontal, QGraphicsItem *parent)
  : QGraphicsLineItem(parent) 
{
  _horizontal = horizontal;
  _orientationPoint = QPointF(0.0, 0.0);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  setAcceptHoverEvents(true);
}


QRectF GuideLine::boundingRect() const 
{
  int margin = 5;
  return QRectF(_startPos, _endPos).marginsAdded(QMargins(margin, margin, margin, margin))
    .normalized();
}

QPainterPath GuideLine::shape() const 
{
  qreal clickTolerance = 5;
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

void GuideLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  calculate();
  QPainterPath myPath;
  myPath.moveTo(_startPos);
  myPath.lineTo(_endPos);
  QPen myPen = QPen(Qt::blue, 1, Qt::PenStyle(1), Qt::FlatCap, Qt::MiterJoin); 
  painter->setPen(myPen);
  painter->strokePath(myPath, myPen);
}

void GuideLine::calculate() 
{
  QGraphicsView *view = scene()->views().first();
  QRectF visibleRect = view->mapToScene(view->rect()).boundingRect();
  if (_horizontal)
    {
      _startPos = QPointF(visibleRect.left(), _orientationPoint.y());
      _endPos = QPointF(visibleRect.right(), _orientationPoint.y());
    }
  else
    {
      _startPos = QPointF(_orientationPoint.x(), visibleRect.bottom());
      _endPos = QPointF(_orientationPoint.x(), visibleRect.top());
    }
  prepareGeometryChange();
}

int GuideLine::type() const 
{
  return Type;
}

void GuideLine::setOrientationPoint(QPointF orientation)
{
  _orientationPoint = mapFromScene(orientation);
}

QPointF GuideLine::getOrientationPoint()
{
  return _orientationPoint;
}

void GuideLine::mousePressEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void GuideLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::restoreOverrideCursor();
}

void GuideLine::hoverMoveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void GuideLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::restoreOverrideCursor();
}

