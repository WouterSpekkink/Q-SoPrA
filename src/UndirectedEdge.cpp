#include "../include/UndirectedEdge.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include <QtCore>

const qreal Pi = 3.14;

UndirectedEdge::UndirectedEdge(NetworkNode *startItem, NetworkNode *endItem,
			       QString submittedType, QString submittedName,
			       QGraphicsItem *parent)
  : QGraphicsLineItem(parent) 
{
  start = startItem;
  end = endItem;
  color = Qt::black;
  setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  height = 20;
  relType = submittedType;
  name = submittedName;
  filtered = true;
  massHidden = false;
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  comment = "";
}

QRectF UndirectedEdge::boundingRect() const 
{
  return strokePath.controlPointRect(); 
}

void UndirectedEdge::updatePosition() 
{
  calculate();
  QPainterPath myPath;
  myPath.moveTo(ghostLineTwo.p2());
  myPath.quadTo(controlPoint, ghostLineOne.p2());
  myPath.quadTo(controlPoint, ghostLineOne.p2());
  strokePath = myPath;
}

void UndirectedEdge::calculate() 
{
  // Let us first calculate the distance between our two points.
  qreal dX = end->pos().x() - start->pos().x();
  qreal dY = end->pos().y() - start->pos().y();
  qreal distance = sqrt(pow(dX, 2) + pow(dY, 2));

  QLineF newLine = QLineF(start->pos(), end->pos());
  newLine.setLength(newLine.length() - 18);
  qreal mX = (start->pos().x() + newLine.p2().x()) / 2;
  qreal mY = (start->pos().y() + newLine.p2().y()) / 2;
  qreal cX = height * (-1 * (dY / distance)) + mX;
  qreal cY = height * (dX / distance) + mY;
  controlPoint = QPointF(cX, cY);
  ghostLineOne = QLineF(controlPoint, end->pos());
  ghostLineOne.setLength(ghostLineOne.length() - 18);
  
  newLine = QLineF(end->pos(), start->pos());
  newLine.setLength(newLine.length() - 18);
  mX = (end->pos().x() + newLine.p2().x()) / 2;
  mY = (end->pos().y() + newLine.p2().y()) / 2;
  cX = height * (-1 * (dY / distance)) + mX;
  cY = height * (dX / distance) + mY;
  controlPoint = QPointF(cX, cY);
  ghostLineTwo = QLineF(controlPoint, start->pos());
  ghostLineTwo.setLength(ghostLineTwo.length() - 18);
  // Then we do some calculations to determine how our arrows are drawn.
  double angle = ::acos(ghostLineOne.dx() / ghostLineOne.length());
  if (ghostLineOne.dy() >= 0)
    angle = (Pi * 2) - angle;
  double angle2 = ::acos(ghostLineTwo.dx() / ghostLineTwo.length());
  if (ghostLineTwo.dy() >= 0)
    angle2 = (Pi * 2) - angle2;
  qreal arrowSize = 10;
  arrowP1 = ghostLineOne.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
					cos(angle + Pi / 3) * arrowSize);
  arrowP2 = ghostLineOne.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
					cos(angle + Pi - Pi / 3) * arrowSize);
  arrowP3 = ghostLineTwo.p2() - QPointF(sin(angle2 + Pi /3) * arrowSize,
					cos(angle2 + Pi / 3) * arrowSize);
  arrowP4 = ghostLineTwo.p2() - QPointF(sin(angle2 + Pi - Pi / 3) * arrowSize,
					cos(angle2 + Pi - Pi / 3) * arrowSize);
  prepareGeometryChange();
}

void UndirectedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) 
{
  QPen myPen = pen();
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);
  calculate();
  arrowHeadOne.clear();
  arrowHeadOne << ghostLineOne.p2() << arrowP1 << arrowP2;
  arrowHeadTwo.clear();
  arrowHeadTwo << ghostLineTwo.p2() << arrowP3 << arrowP4;
  QPainterPath myPath;
  myPath.moveTo(ghostLineTwo.p2());
  myPath.quadTo(controlPoint, ghostLineOne.p2());
  strokePath = myPath;
  painter->drawPolygon(arrowHeadOne);
  painter->strokePath(myPath, QPen(color));
  painter->drawPolygon(arrowHeadTwo);
}

NetworkNode* UndirectedEdge::startItem() const 
{
  return start;
}

NetworkNode* UndirectedEdge::endItem() const 
{
  return end;
}

void UndirectedEdge::setColor(const QColor &subColor) 
{
  color = subColor;
}

QString UndirectedEdge::getType() 
{
  return relType;
}

void UndirectedEdge::setType(const QString submittedType) 
{
  relType = submittedType;
}

void UndirectedEdge::setHeight(int submittedHeight) 
{
  height = submittedHeight;
}

int UndirectedEdge::getHeight() 
{
  return height;
}

int UndirectedEdge::type() const 
{
  return Type;
}

QString UndirectedEdge::getName() 
{
  return name;
}

void UndirectedEdge::setName(const QString submittedName) 
{
  name = submittedName;
}

bool UndirectedEdge::isFiltered() 
{
  return filtered;
}

void UndirectedEdge::setFiltered(bool state) 
{
  filtered = state;
}

bool UndirectedEdge::isMassHidden() 
{
  return massHidden;
}

void UndirectedEdge::setMassHidden(bool state) 
{
  massHidden = state;
}

void UndirectedEdge::setComment(const QString submittedComment) 
{
  comment = submittedComment;
  QString toolTip = breakString(comment);
  this->setToolTip(toolTip);
}

QString UndirectedEdge::getComment() 
{
  return comment;
}

QColor UndirectedEdge::getColor() 
{
  return color;
}
