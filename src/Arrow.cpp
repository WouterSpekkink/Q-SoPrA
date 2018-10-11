#include "../include/Arrow.h"
#include <math.h>
#include <QtCore>
#include <QPen>
#include <QPainter>
#include "../include/Scene.h"

Arrow::Arrow(QString subType, QString subCoder, QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  color = Qt::black;
  setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  typeInd = subType;
  coder = subCoder;
  copy = false;
  height = 0;
  penWidth = 1;
  penStyle = 1;
  massHidden = false;
}

QRectF Arrow::boundingRect() const
{
  if (height > 0)
    {
      return strokePath.controlPointRect();
    }
  else
    {
    return QGraphicsLineItem::boundingRect(); 
    }
}

QPainterPath Arrow::shape() const
{
  QPainterPathStroker stroker;
  stroker.setWidth(20);
  return stroker.createStroke(strokePath);
}

void Arrow::updatePosition()
{
  calculate();
  QPainterPath myPath;
  myPath.moveTo(start->pos());
  myPath.quadTo(controlPoint, ghostLine.p2());
  strokePath = myPath;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  calculate();
  if (line().length() > 0)
    {
      arrowHead.clear();
      arrowHead << ghostLine.p2() << arrowP1 << arrowP2;
      QPainterPath myPath;
      myPath.moveTo(newLine.p1());
      myPath.quadTo(controlPoint, ghostLine.p2());
      strokePath = myPath;
      painter->setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      painter->setBrush(color);
      painter->drawPolygon(arrowHead);
      QPen myPen = QPen(color, penWidth, Qt::PenStyle(penStyle), Qt::RoundCap, Qt::RoundJoin);
      painter->strokePath(myPath, myPen);
      if (penStyle == 1 && isSelected())
	{
	  painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
      else if (penStyle == 4)
	{
	  painter->setPen(QPen(QColor(Qt::darkRed), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
      else if (penStyle == 3)
	{
	  painter->setPen(QPen(QColor(Qt::darkBlue), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
      else if (penStyle == 2)
	{
	  painter->setPen(QPen(QColor(Qt::darkGreen), 1, Qt::DashLine));
	  painter->setBrush(Qt::transparent);
	  painter->drawPath(shape());
	}
    }
}

void Arrow::calculate() {
  prepareGeometryChange();
  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(start);
  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(end);
  MacroEvent *startMacro = qgraphicsitem_cast<MacroEvent*>(start);
  MacroEvent *endMacro = qgraphicsitem_cast<MacroEvent*>(end);
  OccurrenceItem *occurrenceStart = qgraphicsitem_cast<OccurrenceItem*>(start);
  OccurrenceItem *occurrenceEnd = qgraphicsitem_cast<OccurrenceItem*>(end);
  if (startEvent && endEvent)
    {
      // startX is middle of start node
      qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
      // endX is middle of end node
      qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node
      qreal endStart = endEvent->pos().x() - 20;
      if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endEvent->getWidth())
	{
	  newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(startX, endEvent->pos().y())));
	}
      else if (startX <= endStart)
	{
	  newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
	}
      else if (startX >= endStart)
	{
	  newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				    endEvent->pos().y())));
	}
    }
  else if (startEvent && endMacro)
    {
      // startX is middle of start node
      qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endMacro->pos().x() - 20;
      if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endMacro->getWidth())
	{
	  newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(startX, endMacro->pos().y())));
	}
      else if (startX <= endStart)
	{
	  newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
	}
      else if (startX >= endStart)
	{
	  newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			   (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				    endMacro->pos().y())));
	}
    }
  else if (startMacro && endMacro)
    {
      // startX is middle of start node
      qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endMacro->pos().x() - 20;
      if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endMacro->getWidth())
	{
	  newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(startX, endMacro->pos().y())));
	}
      else if (startX <= endStart)
	{
	  newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
	}
      else if (startX >= endStart)
	{
	  newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				    endMacro->pos().y())));
	}
    }
  else if (startMacro && endEvent)
    {
      // startX is middle of start node
      qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
      // endX is middle of end node;
      qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node.
      qreal endStart = endEvent->pos().x() - 20;
      if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endEvent->getWidth())
	{
	  newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(startX, endEvent->pos().y())));
	}
      else if (startX <= endStart)
	{
	  newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
	}
      else if (startX >= endStart)
	{
	  newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			   (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				    endEvent->pos().y())));
	}
    }
  else if (occurrenceStart && occurrenceEnd)
    {
      // startX is middle of start node
      qreal startX = occurrenceStart->pos().x() + (occurrenceStart->getWidth() / 2) - 20;
      // endX is middle of end node
      qreal endX = occurrenceEnd->pos().x() + (occurrenceEnd->getWidth() / 2) - 20;
      // endStart is the left-most edge of the end node
      qreal endStart = occurrenceEnd->pos().x() - 20;
      if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + occurrenceEnd->getWidth())
	{
	  newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
			   (QPointF(startX, occurrenceEnd->pos().y())));
	}
      else if (startX <= endStart)
	{
	  newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
			   (QPointF(endX - (occurrenceEnd->getWidth() / 2) + 20,
				    occurrenceEnd->pos().y())));
	}
      else if (startX >= endStart)
	{
	  newLine = QLineF(QPointF(startX, occurrenceStart->pos().y() - 20),
			   (QPointF(endX + (occurrenceEnd->getWidth() / 2) - 20,
				    occurrenceEnd->pos().y())));
	}
    }
  //  newLine.setLength(newLine.length() - 20);
  qreal dY = newLine.p2().y() - newLine.p1().y();
  qreal dX = newLine.p2().x() - newLine.p1().x();
  qreal mX = (newLine.p1().x() + newLine.p2().x()) / 2;
  qreal mY = (newLine.p1().y() + newLine.p2().y()) / 2;
  qreal distance = sqrt(pow(dX, 2) + pow(dY, 2));
  qreal cX = height * (-1 * (dY / distance)) + mX;
  qreal cY = height * (dX / distance) + mY;
  controlPoint = QPointF(cX, cY);
  ghostLine = QLineF(controlPoint, newLine.p2());
  ghostLine.setLength(ghostLine.length() - 28);
  double angle = ::acos(ghostLine.dx() / ghostLine.length());
  if (ghostLine.dy() >= 0)
    {
      angle = (Pi * 2) - angle;
    }
  qreal arrowSize = 10;
  arrowP1 = ghostLine.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
				     cos(angle + Pi / 3) * arrowSize);
  arrowP2 = ghostLine.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				     cos(angle + Pi - Pi / 3) * arrowSize);
  setLine(newLine);
}

void Arrow::setColor(const QColor &subColor)
{
  color = subColor;
}

QColor Arrow::getColor()
{
  return color;
}

QGraphicsItem* Arrow::startItem() const
{
  return start;
}

QGraphicsItem* Arrow::endItem() const
{
  return end;
}

int Arrow::type() const
{
  return Type;
}

bool Arrow::isCopy()
{
  return copy;
}

void Arrow::setCopy(bool status)
{
  copy = status;
}

QString Arrow::getType()
{
  return typeInd;
}

void Arrow::setStartItem(QGraphicsItem *subStart)
{
  start = subStart;
}

void Arrow::setEndItem(QGraphicsItem *subEnd)
{
  end = subEnd;
}

void Arrow::setPenStyle(int style)
{
  penStyle = style;
}

int Arrow::getPenStyle()
{
  return penStyle;
}

void Arrow::setHeight(int submittedHeight)
{
  height = submittedHeight;
}

int Arrow::getHeight()
{
  return height;
}

void Arrow::setMassHidden(bool status)
{
  massHidden = status;
}

bool Arrow::isMassHidden()
{
  return massHidden;
}
