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
#include "../include/EllipseObject.h"

EllipseObject::EllipseObject() 
{
  _drawRect = QRectF(0, 0, 100, 100);
  _color = QColor(0, 0, 0, 255);
  _fillColor = QColor(Qt::transparent);
  _rotation = 0;
  _penWidth = 1;
  _penStyle = 1;
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setAcceptHoverEvents(true);
}

void EllipseObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  prepareGeometryChange();
  painter->setBrush(_fillColor);
  painter->setPen(QPen(_color, _penWidth, Qt::PenStyle(_penStyle)));
  painter->drawEllipse(_drawRect);
  if (isSelected()) 
    {
      QRectF selectRect = _drawRect.adjusted(-_penWidth / 2,
					     -_penWidth / 2,
					     _penWidth / 2,
					     _penWidth / 2);
      painter->setBrush(QColor(Qt::transparent));
      painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
      QPainterPath outline;
      outline.addRect(selectRect);
      painter->drawPath(outline);
      update();
    }
}

QColor EllipseObject::getColor() 
{
  return _color;
}

void EllipseObject::setColor(const QColor &color) 
{
  _color = color;
}

QColor EllipseObject::getFillColor() 
{
  return _fillColor;
}

void EllipseObject::setFillColor(const QColor &color) 
{
  _fillColor = color;
}

qreal EllipseObject::getLeft() 
{
  return _drawRect.left();
}

void EllipseObject::setLeft(const qreal &newLeft) 
{
  _drawRect.setLeft(newLeft);
  _drawRect = _drawRect.normalized();
}

qreal EllipseObject::getRight() 
{
  return _drawRect.right();
}

void EllipseObject::setRight(const qreal &newRight) 
{
  _drawRect.setRight(newRight);
  _drawRect = _drawRect.normalized();
}

qreal EllipseObject::getTop() 
{
  return _drawRect.top();
}

void EllipseObject::setTop(const qreal &newTop) 
{
  _drawRect.setTop(newTop);
  _drawRect = _drawRect.normalized();
}

qreal EllipseObject::getBottom() 
{
  return _drawRect.bottom();
}

void EllipseObject::setBottom(const qreal &newBottom) 
{
  _drawRect.setBottom(newBottom);
  _drawRect = _drawRect.normalized();
}

QPointF EllipseObject::topLeft() 
{
  return _drawRect.topLeft();
}

QPointF EllipseObject::topRight() 
{
  return _drawRect.topRight();
}

QPointF EllipseObject::bottomLeft() 
{
  return _drawRect.bottomLeft();
}

QPointF EllipseObject::bottomRight() 
{
  return _drawRect.bottomRight();
}

void EllipseObject::setTopLeft(const QPointF &topLeft) 
{
  _drawRect.setTopLeft(topLeft);
  _drawRect = _drawRect.normalized();
}

void EllipseObject::setTopRight(const QPointF &topRight) 
{
  _drawRect.setTopRight(topRight);
  _drawRect = _drawRect.normalized();
}

void EllipseObject::setBottomLeft(const QPointF &bottomLeft) 
{
  _drawRect.setBottomLeft(bottomLeft);
  _drawRect = _drawRect.normalized();
}

void EllipseObject::setBottomRight(const QPointF &bottomRight) 
{
  _drawRect.setBottomRight(bottomRight);
  _drawRect = _drawRect.normalized();
}

QPointF EllipseObject::getCenter() 
{
  return boundingRect().center();
}

void EllipseObject::moveCenter(const QPointF &newCenter) 
{
  _drawRect.moveCenter(newCenter);
  _drawRect = _drawRect.normalized();
}

qreal EllipseObject::getRotationValue() 
{
  return _rotation;
}

void EllipseObject::setRotationValue(const qreal &rotation) 
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

QRectF EllipseObject::boundingRect() const 
{
  QRectF myRect = _drawRect.normalized().adjusted(-10,-10, 10, 10);
  return myRect;
}

QPainterPath EllipseObject::shape() const 
{
  QPainterPath path;
  QPainterPathStroker stroker;
  stroker.setWidth(_penWidth + 15);
  QRectF myRect = _drawRect.adjusted(-_penWidth / 2, -_penWidth / 2, _penWidth / 2, _penWidth / 2);
  QRectF myRectTwo = _drawRect.adjusted(1, 1, -1, -1);
  path.addRect(myRect);
  path.addEllipse(myRectTwo);
  path.setFillRule(Qt::OddEvenFill); 
  return stroker.createStroke(path);
}

int EllipseObject::getPenWidth() 
{
  return _penWidth;
}

void EllipseObject::setPenWidth(const int &width) 
{
  _penWidth = width;
}

int EllipseObject::getPenStyle() 
{
  return _penStyle;
}

void EllipseObject::setPenStyle(const int &style) 
{
  _penStyle = style;
}

int EllipseObject::type() const 
{
  return Type;
}

void EllipseObject::mousePressEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void EllipseObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::restoreOverrideCursor();
}

void EllipseObject::hoverMoveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void EllipseObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *) 
{
  QApplication::restoreOverrideCursor();
}

