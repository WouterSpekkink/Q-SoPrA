#include <QtWidgets>
#include "../include/OccurrenceItem.h"
#include "../include/Scene.h"
#include "../include/OccurrenceLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

OccurrenceItem::OccurrenceItem(int subWidth, QString toolTip, QPointF originalPosition,
			       int subId, int subOrder, QString submittedAttribute)
  : color(255, 255, 255) {
  width = subWidth;
  setToolTip(breakString(toolTip));
  originalPos = originalPosition;
  selectionColor = QColor(Qt::black);
  id = subId;
  order = subOrder;
  label = NULL;
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  attribute = submittedAttribute;
  permHidden = false;
  grouped = false;
}

QRectF OccurrenceItem::boundingRect() const {
  return QRectF(-26, -26, width + 12, 52);
}

QPainterPath OccurrenceItem::shape() const {
  QPainterPath path;
  path.addEllipse(-20, -20, width, 40);
  return path;
}

void OccurrenceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(color));
  painter->drawEllipse(-20, -20, width, 40);
}

// Only to set the cursor to a different graphic.
void OccurrenceItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setSelected(true);
  }
}

void OccurrenceItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  Scene *myScene = qobject_cast<Scene*>(scene());
  if (myScene->getSelectedOccurrence() != NULL) {
    setCursor(Qt::SizeVerCursor);
    QPointF currentPos = this->scenePos();
    qreal currentX = currentPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(currentX);
    this->setPos(newPos);
    if (label != NULL) {
      label->setNewPos(newPos);
    }
    myScene->relevantChange();
  }
}

void OccurrenceItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  {
 Scene *myScene = qobject_cast<Scene*>(scene());
  if (myScene->getSelectedOccurrence() != NULL) {
    QPointF currentPos = this->scenePos();
    qreal currentX = currentPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(currentX);
    this->setPos(newPos);
    if (label != NULL) {
      label->setNewPos(newPos);
    }
    setCursor(Qt::OpenHandCursor);
    update();
    QGraphicsItem::mouseReleaseEvent(event);
  } else {
    setCursor(Qt::OpenHandCursor);
  }
}

int OccurrenceItem::getCorrection() {
  return width - 39;
}

QPointF OccurrenceItem::getOriginalPos() const {
  return originalPos;
}

int OccurrenceItem::getId() const {
  return id;
}

int OccurrenceItem::getOrder() const {
  return order;
}

void OccurrenceItem::setOriginalPos(QPointF newPos) {
  originalPos = newPos;
}

void OccurrenceItem::setWidth(int newWidth) {
  width = newWidth;
}

int OccurrenceItem::getWidth() const {
  return width;
}

void OccurrenceItem::setLabel(OccurrenceLabel *submittedLabel) {
  label = submittedLabel;
}

OccurrenceLabel* OccurrenceItem::getLabel() {
  return label;
}

void OccurrenceItem::setColor(const QColor &subColor) {
  color = subColor;
}

QColor OccurrenceItem::getColor() {
  return color;
}

void OccurrenceItem::setSelectionColor(const QColor &subColor) {
  selectionColor = subColor;
}

int OccurrenceItem::type() const {
  return Type;
}

QString OccurrenceItem::getAttribute() const {
  return attribute;  
}

void OccurrenceItem::setAttribute(const QString submittedAttribute) {
  attribute = submittedAttribute;
}

void OccurrenceItem::setPermHidden(bool status) {
  permHidden = status;
}

bool OccurrenceItem::isPermHidden() {
  return permHidden;
}

void OccurrenceItem::setGrouped(bool status) {
  grouped = status;
}

bool OccurrenceItem::isGrouped() {
  return grouped;
}
