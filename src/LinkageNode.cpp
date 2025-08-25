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
#include "../include/LinkageNode.h"
#include "../include/Scene.h"
#include "../include/LinkageNodeLabel.h"

LinkageNode::LinkageNode(QPointF originalPosition,
                         QString toolTip,
                         int id,
                         int order,
                         int filteredOrder,
                         QGraphicsItem *parent)
  : QGraphicsItem(parent) 
{
  _color = QColor(255, 255, 255);
  _width = 40;
  _originalPos = originalPosition;
  _id = id;
  _order = order;
  _filteredOrder = filteredOrder;
  _labelPtr = NULL;
  _first = false;
  _last = false;
  setToolTip(toolTip);
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF LinkageNode::boundingRect() const 
{
  if (_tail || _head)
  {
    return QRectF(-26, -56, _width + 12, 102);
  }
  else
  {
    return QRectF(-26, -26, _width + 12, 52);
  }
}

QPainterPath LinkageNode::shape() const 
{
  QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

void LinkageNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  if (_valid)
  {
    _color = QColor(255, 255, 255);
  }
  else
  {
    _color = QColor(128, 128, 128);
  }
  if (_tail) 
  {
    _color = QColor(151, 74, 189, 255);
  }
  else if (_head)
  {
    _color = QColor(74, 189, 81, 255);
  }
  painter->setBrush(QBrush(_color));  
  painter->drawEllipse(-20, -20, _width, 40);
  if (_tail)
  {
    QFont font = painter->font();
    font.setPointSize(14);
    painter->setFont(font);
    QRectF textRect = QRectF(-20, -50, 100, 100);
    QRectF boundingRect;
    QString text = "Tail";
    painter->drawText(textRect, 0, text, &boundingRect);
  }
  else if (_head)
  {
    QFont font = painter->font();
    font.setPointSize(14);
    painter->setFont(font);
    QRectF textRect = QRectF(-20, -50, 100, 100);
    QString text = "Head";
    QRectF boundingRect;
    painter->drawText(textRect, 0, text, &boundingRect);
  }
  update();
}

// Only to set the cursor to a different graphic.
void LinkageNode::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton) 
  {
    setSelected(true);
  }
}

void LinkageNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->modifiers() & Qt::ShiftModifier) 
  {
    setCursor(Qt::SizeAllCursor);
    QPointF newPos = event->scenePos();
    this->setPos(newPos);
    if (_labelPtr != NULL)
    {
      _labelPtr->setNewPos(this->scenePos());
    }
  }
  else 
  {
    setCursor(Qt::SizeVerCursor);
    QPointF currentPos = this->scenePos();
    qreal currentX = currentPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(currentX);
    this->setPos(newPos);
    if (_labelPtr != NULL)
    {
      _labelPtr->setNewPos(this->scenePos());
    }
  }
}

QPointF LinkageNode::getOriginalPos() const 
{
  return _originalPos;
}

int LinkageNode::getId() const 
{
  return _id;
}

int LinkageNode::getOrder() const 
{
  return _order;
}

int LinkageNode::getFilteredOrder() const
{
  return _filteredOrder;
}

void LinkageNode::setFilteredOrder(int filteredOrder)
{
  _filteredOrder = filteredOrder;
}

void LinkageNode::setOriginalPos(QPointF originalPos) 
{
  _originalPos = originalPos;
}

void LinkageNode::setLabel(LinkageNodeLabel *labelPtr) 
{
  _labelPtr = labelPtr;
}

LinkageNodeLabel* LinkageNode::getLabel() 
{
  return _labelPtr;
}

int LinkageNode::type() const 
{
  return Type;
}

void LinkageNode::setFirst()
{
  _last = false;
  _first = true;
}

void LinkageNode::setLast()
{
  _first = false;
  _last = true;
}

void LinkageNode::setMiddle()
{
  _first = false;
  _last = false;
}

bool LinkageNode::isFirst() const
{
  return _first;
}

bool LinkageNode::isLast() const
{
  return _last;
}

void LinkageNode::setDirection(QString direction)
{
  _direction = direction;
}

QString LinkageNode::getDirection()
{
  return _direction;
}

void LinkageNode::setTail()
{
  _tail = true;
  _head = false;
}

void LinkageNode::setHead()
{
  _tail = false;
  _head = true;
}

void LinkageNode::setValid(bool valid)
{
  _valid = valid;
}

bool LinkageNode::isValid() const
{
  return _valid;
}

void LinkageNode::setUnselected()
{
  _tail = false;
  _head = false;
}
