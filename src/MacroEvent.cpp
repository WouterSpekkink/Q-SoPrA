#include <QtWidgets>
#include "../include/MacroEvent.h"
#include "../include/Scene.h"
#include "../include/MacroLabel.h"

/*
  In this case we assign a random colour. I will want to change that
  in real applications.
*/

MacroEvent::MacroEvent(int subWidth, QString submittedDescription, QPointF originalPosition, int subId,
		       QString submittedConstraint, QVector<EventItem*> submittedIncidents)
  : color(255, 255, 255) {
  width = subWidth;
  description = submittedDescription;
  QString tip = "<FONT SIZE = 3>" + description + "</FONT>";
  setToolTip(tip);
  originalPos = originalPosition;
  order = 0;
  selectionColor = QColor(Qt::black);
  id = subId;
  dislodged = false;
  label = NULL;
  incidents = submittedIncidents;
  macroEvent = NULL;
  mode = "";
  constraint = submittedConstraint;
  
  setCursor(Qt::OpenHandCursor);
  setAcceptedMouseButtons(Qt::LeftButton);
  setFlag(QGraphicsItem::ItemIsSelectable);
  setFlag(QGraphicsItem::ItemIsMovable);
  //  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF MacroEvent::boundingRect() const {
  return QRectF(-26, -26, width + 12, 52);
}


void MacroEvent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setPen(Qt::NoPen);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(color));
  painter->drawEllipse(-20, -20, width, 40);
  if (isSelected()) {
    painter->setPen(QPen(selectionColor, 1, Qt::DashLine));
    painter->setBrush(QBrush(Qt::transparent));
    painter->drawEllipse(-24, -24, width + 8, 48);
    update();
  } else {
    selectionColor = QColor(Qt::black);
    update();
  }
}

// Only to set the cursor to a different graphic.
void MacroEvent::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    setSelected(true);
  }
}

void MacroEvent::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (event->modifiers() & Qt::AltModifier) {
    setCursor(Qt::SizeAllCursor);
    QPointF newPos = event->scenePos();
    this->setPos(newPos);
    if (label != NULL) {
      label->setNewPos(newPos);
    }
    if (newPos.x() != originalPos.x()) {
      dislodged = true;
    } else {
      dislodged = false;
    }
  } else {
    setCursor(Qt::SizeVerCursor);
    qreal oldX = originalPos.x();
    QPointF currentPos = this->scenePos();
    qreal currentX = currentPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(currentX);
    this->setPos(newPos);
    if (label != NULL) {
      label->setNewPos(newPos);
    }
    if (newPos.x() == oldX) {
      dislodged = false;
    } else {
      dislodged = true;
    }
  }
  Scene *myScene = qobject_cast<Scene*>(scene());
  myScene->relevantChange();
}

void MacroEvent::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)  {
  if (event->modifiers() & Qt::AltModifier) {
    QPointF newPos = event->scenePos();
    this->setPos(newPos);
    if (label != NULL) {
      label->setNewPos(newPos);
    }
    if (newPos.x() != originalPos.x()) {
      dislodged = true;
    } else {
      dislodged = false;
    }
    setCursor(Qt::OpenHandCursor);
  } else if (event->modifiers() & Qt::ShiftModifier) {
    setCursor(Qt::OpenHandCursor);
  } else {
    qreal oldX = originalPos.x();
    QPointF currentPos = this->scenePos();
    qreal currentX = currentPos.x();
    QPointF newPos = event->scenePos();
    newPos.setX(currentX);
    this->setPos(newPos);
    if (label != NULL) {
      label->setNewPos(newPos);
    }
    if (newPos.x() == oldX) {
      dislodged = false;
    } else {
      dislodged = true;
    }
    setCursor(Qt::OpenHandCursor);
  }
  update();
  QGraphicsItem::mouseReleaseEvent(event);
}

int MacroEvent::getCorrection() {
  return width - 39;
}

QPointF MacroEvent::getOriginalPos() {
  return originalPos;
}

int MacroEvent::getId() {
  return id;
}

void MacroEvent::setOriginalPos(QPointF newPos) {
  originalPos = newPos;
}

bool MacroEvent::isDislodged() {
  return dislodged;
}

void MacroEvent::setDislodged(bool state) {
  dislodged = state;
}

void MacroEvent::setWidth(int newWidth) {
  width = newWidth;
}

int MacroEvent::getWidth() const {
  return width;
}

void MacroEvent::setLabel(MacroLabel *submittedLabel) {
  label = submittedLabel;
}

MacroLabel* MacroEvent::getLabel() {
  return label;
}

void MacroEvent::setColor(const QColor &subColor) {
  color = subColor;
}

QColor MacroEvent::getColor() {
  return color;
}

void MacroEvent::setSelectionColor(const QColor &subColor) {
  selectionColor = subColor;
}

int MacroEvent::type() const {
  return Type;
}

void MacroEvent::setIncidents(QVector<EventItem*> submittedIncidents) {
  incidents = submittedIncidents;
}

QVector<EventItem*> MacroEvent::getIncidents() const {
  return incidents;
}

void MacroEvent::setDescription(const QString text) {
  description = text;
  QString tip = "<FONT SIZE = 3>" + description + "</FONT>";
  setToolTip(tip);
}

QString MacroEvent::getDescription() {
  return description;
}

void MacroEvent::setComment(const QString text) {
  comment = text;
}

QString MacroEvent::getComment() {
  return comment;
}

void MacroEvent::setMacroEvent(MacroEvent* submittedEvent) {
  macroEvent = submittedEvent;
}

MacroEvent* MacroEvent::getMacroEvent() {
  return macroEvent;
}

void MacroEvent::setOrder(int submittedOrder) {
  order = submittedOrder;
}

int MacroEvent::getOrder() {
  return order;
}

void MacroEvent::insertAttribute(QString attribute) {
  attributes.insert(attribute);
}

void MacroEvent::removeAttribute(QString attribute) {
  attributes.remove(attribute);
}

QSet<QString> MacroEvent::getAttributes() {
  return attributes;
}

void MacroEvent::insertValue(QString attribute, QString value) {
  values.insert(attribute, value);
}

void MacroEvent::removeValue(QString attribute) {
  values.remove(attribute);
}

QMap<QString, QString> MacroEvent::getValues() {
  return values;
}

void MacroEvent::setAttributes(QSet<QString> submittedAttributes) {
  attributes = submittedAttributes;
}

void MacroEvent::setValues(QMap<QString, QString> submittedValues) {
  values = submittedValues;
}

void MacroEvent::setMode(const QString submittedMode) {
  mode = submittedMode;
}

QString MacroEvent::getMode() const {
  return mode;
}

QString MacroEvent::getConstraint() const {
  return constraint;
}
