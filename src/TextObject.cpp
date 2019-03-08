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

#include "../include/TextObject.h"

TextObject::TextObject(const QString &text,
		       QGraphicsTextItem *parent)
  : QGraphicsTextItem(parent) 
{
  setPlainText(text);
  setFlag(QGraphicsTextItem::ItemIsSelectable, true);
  _rotation = 0;
  setAcceptHoverEvents(true);
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

void TextObject::mousePressEvent(QGraphicsSceneMouseEvent *) 
{
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
}

void TextObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) 
{
  QApplication::restoreOverrideCursor();
  QGraphicsTextItem::mouseReleaseEvent(event);
}

void TextObject::hoverMoveEvent(QGraphicsSceneHoverEvent *) 
{
  setCursor(Qt::OpenHandCursor);
}
