#include "../include/DirectedEdge.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include <QtCore>

const qreal Pi = 3.14;

DirectedEdge::DirectedEdge(NetworkNode *startItem, NetworkNode *endItem, QString submittedType,
			   QString submittedName, QGraphicsItem *parent)
  : QGraphicsLineItem(parent) {
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

QRectF DirectedEdge::boundingRect() const {
  return strokePath.controlPointRect(); 
}

void DirectedEdge::updatePosition() {
  calculate();
  QPainterPath myPath;
  myPath.moveTo(start->pos());
  myPath.quadTo(controlPoint, ghostLine.p2());
  strokePath = myPath;
}

void DirectedEdge::calculate() {
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
  ghostLine = QLineF(controlPoint, end->pos());
  ghostLine.setLength(ghostLine.length() - 18);
  double angle = ::acos(ghostLine.dx() / ghostLine.length());
  if (ghostLine.dy() >= 0)
    angle = (Pi * 2) - angle;
  qreal arrowSize = 10;
  arrowP1 = ghostLine.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
				     cos(angle + Pi / 3) * arrowSize);
  arrowP2 = ghostLine.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				     cos(angle + Pi - Pi / 3) * arrowSize);
  setLine(newLine);
  prepareGeometryChange();
}

void DirectedEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen myPen = pen();
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);
  calculate();
  arrowHead.clear();
  arrowHead << ghostLine.p2() << arrowP1 << arrowP2;
  QPainterPath myPath;
  myPath.moveTo(start->pos());
  myPath.quadTo(controlPoint, ghostLine.p2());
  strokePath = myPath;
  painter->drawPolygon(arrowHead);
  painter->strokePath(myPath, QPen(color));
}

NetworkNode* DirectedEdge::startItem() const {
  return start;
}

NetworkNode* DirectedEdge::endItem() const {
  return end;
}

void DirectedEdge::setColor(const QColor &subColor) {
  color = subColor;
}

QString DirectedEdge::getType() {
  return relType;
}

void DirectedEdge::setType(const QString submittedType) {
  relType = submittedType;
}

void DirectedEdge::setHeight(int submittedHeight) {
  height = submittedHeight;
}

int DirectedEdge::getHeight() {
  return height;
}

int DirectedEdge::type() const {
  return Type;
}

QString DirectedEdge::getName() {
  return name;
}

void DirectedEdge::setName(const QString submittedName) {
  name = submittedName;
}

bool DirectedEdge::isFiltered() {
  return filtered;
}

void DirectedEdge::setFiltered(bool state) {
  filtered = state;
}

bool DirectedEdge::isMassHidden() {
  return massHidden;
}

void DirectedEdge::setMassHidden(bool state) {
  massHidden = state;
}

void DirectedEdge::setComment(const QString submittedComment) {
  comment = submittedComment;
  QString toolTip = breakString(comment);
  this->setToolTip(toolTip);
}

QString DirectedEdge::getComment() {
  return comment;
}

QColor DirectedEdge::getColor() {
  return color;
}
