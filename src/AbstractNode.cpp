
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
#include "../include/AbstractNode.h"
#include "../include/Scene.h"
#include "../include/AbstractNodeLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

AbstractNode::AbstractNode(int width,
			   QString description,
			   QPointF originalPosition,
			   int id,
			   QString constraint,
			   QVector<IncidentNode*> incidents,
			   QGraphicsItem *parent)
  : QGraphicsItem(parent) 
{
  _color = QColor(255, 255, 255);
  _width = width;
  _description = description;
  _timing = "";
  QString tip = breakString(_description);
  setToolTip(tip);
  _originalPos = originalPosition;
  _order = 0;
  _selectionColor = QColor(Qt::black);
  _id = id;
  _dislodged = false;
  _labelPtr = NULL;
  _incidents = incidents;
  _abstractNodePtr = NULL;
  _mode = "";
  _constraint = constraint;
  _copy = false;
  
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF AbstractNode::boundingRect() const 
{
  return QRectF(-26, -26, _width + 12, 52);
}

QPainterPath AbstractNode::shape() const 
{
  QPainterPath path;
  path.addEllipse(boundingRect());
  return path;
}

void AbstractNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(_color));
  painter->drawEllipse(-20, -20, _width, 40);
  if (isSelected()) 
    {
      painter->setPen(QPen(_selectionColor, 1, Qt::DashLine));
      painter->setBrush(QBrush(Qt::transparent));
      painter->drawPath(shape());
      update();
    }
  else 
    {
      _selectionColor = QColor(Qt::black);
      update();
    }
}

// Only to set the cursor to a different graphic.
void AbstractNode::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton) 
    {
      setSelected(true);
    }
}

void AbstractNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->modifiers() & Qt::AltModifier) 
    {
      setCursor(Qt::SizeAllCursor);
      QPointF newPos = event->scenePos();
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() != _originalPos.x()) 
	{
	  _dislodged = true;
	}
      else 
	{
	  _dislodged = false;
	}
    }
  else 
    {
      setCursor(Qt::SizeVerCursor);
      qreal oldX = _originalPos.x();
      QPointF currentPos = this->scenePos();
      qreal currentX = currentPos.x();
      QPointF newPos = event->scenePos();
      newPos.setX(currentX);
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() == oldX) 
	{
	  _dislodged = false;
	}
      else 
	{
	  _dislodged = true;
	}
    }
  Scene *myScene = qobject_cast<Scene*>(scene());
  myScene->relevantChange();
}

void AbstractNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  
{
  if (event->modifiers() & Qt::AltModifier) 
    {
      QPointF newPos = event->scenePos();
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() != _originalPos.x()) 
	{
	  _dislodged = true;
	}
      else 
	{
	  _dislodged = false;
	}
      setCursor(Qt::OpenHandCursor);
    }
  else if (event->modifiers() & Qt::ShiftModifier) 
    {
      setCursor(Qt::OpenHandCursor);
    }
  else 
    {
      qreal oldX = _originalPos.x();
      QPointF currentPos = this->scenePos();
      qreal currentX = currentPos.x();
      QPointF newPos = event->scenePos();
      newPos.setX(currentX);
      this->setPos(newPos);
      if (_labelPtr != NULL) 
	{
	  _labelPtr->setNewPos(newPos);
	}
      if (newPos.x() == oldX) 
	{
	  _dislodged = false;
	}
      else 
	{
	  _dislodged = true;
	}
      setCursor(Qt::OpenHandCursor);
    }
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

int AbstractNode::getCorrection() 
{
  return _width - 39;
}

QPointF AbstractNode::getOriginalPos() 
{
  return _originalPos;
}

int AbstractNode::getId() 
{
  return _id;
}

void AbstractNode::setOriginalPos(const QPointF &originalPos) 
{
  _originalPos = originalPos;
}

bool AbstractNode::isDislodged() 
{
  return _dislodged;
}

void AbstractNode::setDislodged(bool state) 
{
  _dislodged = state;
}

void AbstractNode::setWidth(const int &width) 
{
  prepareGeometryChange();
  _width = width;
  update();
}

int AbstractNode::getWidth() const 
{
  return _width;
}

void AbstractNode::setLabel(AbstractNodeLabel *labelPtr) 
{
  _labelPtr = labelPtr;
}

AbstractNodeLabel* AbstractNode::getLabel() 
{
  return _labelPtr;
}

void AbstractNode::setColor(const QColor &color) 
{
  _color = color;
}

QColor AbstractNode::getColor() 
{
  return _color;
}

void AbstractNode::setSelectionColor(const QColor &color) 
{
  _selectionColor = color;
}

int AbstractNode::type() const 
{
  return Type;
}

void AbstractNode::setIncidents(QVector<IncidentNode*> incidents) 
{
  _incidents = incidents;
}

QVector<IncidentNode*> AbstractNode::getIncidents() const 
{
  return _incidents;
}

void AbstractNode::setDescription(const QString description) 
{
  _description = description;
  QString tip = breakString(_description);
  setToolTip(tip);
}

QString AbstractNode::getDescription() 
{
  return _description;
}

void AbstractNode::setComment(const QString comment) 
{
  _comment = comment;
}

QString AbstractNode::getComment() 
{
  return _comment;
}

void AbstractNode::setAbstractNode(AbstractNode* abstractNodePtr) 
{
  _abstractNodePtr = abstractNodePtr;
}

AbstractNode* AbstractNode::getAbstractNode() 
{
  return _abstractNodePtr;
}

void AbstractNode::setOrder(const int &order) 
{
  _order = order;
}

int AbstractNode::getOrder() 
{
  return _order;
}

void AbstractNode::insertAttribute(const QString &attribute) 
{
  _attributes.insert(attribute);
}

void AbstractNode::removeAttribute(const QString &attribute) 
{
  _attributes.remove(attribute);
}

const QSet<QString> AbstractNode::getAttributes() 
{
  return _attributes;
}

void AbstractNode::insertValue(const QString &attribute, const QString &value) 
{
  _values.insert(attribute, value);
}

void AbstractNode::removeValue(const QString &attribute) 
{
  _values.remove(attribute);
}

QMap<QString, QString> AbstractNode::getValues() 
{
  return _values;
}

void AbstractNode::setAttributes(const QSet<QString> &attributes) 
{
  _attributes = attributes;
}

void AbstractNode::setValues(const QMap<QString, QString> &values) 
{
  _values = values;
}

void AbstractNode::setMode(const QString &mode) 
{
  _mode = mode;
}

QString AbstractNode::getMode() const 
{
  return _mode;
}

QString AbstractNode::getConstraint() const 
{
  return _constraint;
}

bool AbstractNode::isCopy() 
{
  return _copy;
}

void AbstractNode::setCopy(bool state) 
{
  _copy = state;
}

void AbstractNode::setNewId(const int &id) 
{
  _id = id;
}

void AbstractNode::setTiming(const QString &timing)
{
  _timing = timing;
}

QString AbstractNode::getTiming()
{
  return _timing;
}
