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
#include "../include/RectObject.h"

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
}

void RectObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  prepareGeometryChange();
  painter->setBrush(_fillColor);
  painter->setPen(QPen(_color, _penWidth, Qt::PenStyle(_penStyle)));
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

void RectObject::mousePressEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void RectObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *) 
{
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
