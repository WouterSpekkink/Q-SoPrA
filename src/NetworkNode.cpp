#include <QtWidgets>
#include "../include/NetworkNode.h"
#include "../include/Scene.h"
#include "../include/NetworkNodeLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/


NetworkNode::NetworkNode(QString submittedName, QString submittedDescription) : color(255, 255, 255) 
{
  name = submittedName;
  description = submittedDescription;
  QString toolTip = breakString(name + " - " + description);
  setToolTip(toolTip);
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  previousPos = QPointF(0, 0);
  mode = "";
  selectionColor = QColor(Qt::black);

  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  persistent = false;
}

/*
  I assume that this is for collision or
  selection purposes?
*/

QRectF NetworkNode::boundingRect() const 
{
  return QRectF(-18, -18, 36, 36);
}

void NetworkNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(color));
  painter->drawEllipse(-15, -15, 30, 30);

  if (isSelected()) 
    {
      painter->setPen(QPen(selectionColor, 1, Qt::DashLine));
      painter->setBrush(QBrush(Qt::transparent));
      painter->drawEllipse(-18, -18, 36, 36);
      update();
    }
  else 
    {
      selectionColor = QColor(Qt::black);
      update();
    }

}

// Only to set the cursor to a different graphic.
void NetworkNode::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton) 
    {
      setSelected(true);
    }
  previousPos = event->scenePos();
  setCursor(Qt::ClosedHandCursor);
  
}

void NetworkNode::move(QPointF newPos) 
{
  this->setPos(newPos);
  this->getLabel()->setNewPos(newPos);
  QListIterator<QGraphicsItem*> it(scene()->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *item = it.next();
      NetworkNode *first = this;
      NetworkNode *second = qgraphicsitem_cast<NetworkNode*>(item);
      if (second && first != second && second->isVisible()) 
	{
	  qreal dist = qSqrt(qPow(first->scenePos().x() -
				  second->scenePos().x(), 2) +
			     qPow(first->scenePos().y() -
				  second->scenePos().y(), 2));
	  if (dist <= 30) 
	    {
	      QPointF firstPoint = first->scenePos();
	      QPointF secondPoint = second->scenePos();
	      qreal mX = (firstPoint.x() + secondPoint.x()) / 2;
	      qreal mY = (firstPoint.y() + secondPoint.y()) / 2;
	      QPointF midPoint = QPointF(mX, mY);
	      qreal secondXDiff = secondPoint.x() - midPoint.x();
	      qreal secondYDiff = secondPoint.y() - midPoint.y();
	      qreal xDiff = 0;
	      qreal yDiff = 0;
	      if (secondXDiff > 0) 
		{
		  xDiff = 3;
		}
	      else if (secondXDiff < 0) 
		{
		  xDiff = -3;
		}
	      if (secondYDiff > 0) 
		{
		  yDiff = 3;
		}
	      else if (secondYDiff < 0) 
		{
		  yDiff = -3;
		}
	      QPointF temp = QPointF(second->scenePos().x() + xDiff,
				     second->scenePos().y() + yDiff);
	      second->move(temp);
	    }
	}
    }
  Scene *myScene = qobject_cast<Scene*>(scene());
  myScene->relevantChange();
  update();
  setCursor(Qt::ClosedHandCursor);
}

void NetworkNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
{
  this->move(event->scenePos());
  setCursor(Qt::ClosedHandCursor);  
}

void NetworkNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  
{
  QPointF newPos = event->scenePos();
  qreal x = newPos.x();
  qreal y = newPos.y();

  bool trespass = false;
  for (int i = 0; i < scene()->items().count(); i++) 
    {
      QGraphicsItem *item = scene()->items()[i];
      NetworkNode *currentItem = qgraphicsitem_cast<NetworkNode*>(item);
      Arrow *no = qgraphicsitem_cast<Arrow*>(item);
      if (currentItem && !(no) && item != this) 
	{
	  qreal dist = qSqrt(qPow(currentItem->pos().x()-x,2)+qPow(currentItem->pos().y()-y,2));
	  if (dist <= 40) 
	    {
	      trespass = true;
	    }
	  else 
	    {
	      previousPos = this->scenePos();
	    }
	}
    }
  if (trespass) 
    {
      this->setPos(previousPos);
    }
  else 
    {
      this->setPos(newPos);
    }
  if (label != NULL) 
    {
      label->setNewPos(this->scenePos());
    }
  setCursor(Qt::OpenHandCursor);
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

int NetworkNode::getCorrection() 
{
  return 30 - 29;
}

void NetworkNode::setColor(QColor newColor) 
{
  color = newColor;
}

QString NetworkNode::getName() const 
{
  return name;
}

QString NetworkNode::getDescription() 
{
  return description;
}

int NetworkNode::type() const 
{
  return Type;
}

void NetworkNode::setLabel(NetworkNodeLabel *newLabel) 
{
  label = newLabel;
}

NetworkNodeLabel* NetworkNode::getLabel() 
{
  return label;
}

void NetworkNode::setMode(const QString &submittedMode) 
{
  mode = submittedMode;
}

QString NetworkNode::getMode() const 
{
  return mode;
}

QColor NetworkNode::getColor() 
{
  return color;
}

void NetworkNode::setSelectionColor(const QColor &subColor) 
{
  selectionColor = subColor;
}

bool NetworkNode::isPersistent() 
{
  return persistent; 
}

void NetworkNode::setPersistent(bool state) 
{
  persistent = state;
}

