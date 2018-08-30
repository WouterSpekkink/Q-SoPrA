#include "../include/TextObject.h"

TextObject::TextObject(const QString &text,
		       QGraphicsTextItem *parent)
  : QGraphicsTextItem(parent) 
{
  setPlainText(text);
  setFlag(QGraphicsTextItem::ItemIsSelectable, true);
  rotation = 0;
  setAcceptHoverEvents(true);
}

qreal TextObject::getRotationValue() 
{
  return rotation;
}

void TextObject::setRotationValue(qreal newRotation) 
{
  rotation = newRotation;
  QTransform transform;
  QPointF center = boundingRect().center();
  QPointF oldC = mapToScene(boundingRect().center());
  
  transform.translate(center.x(), center.y());
  transform.rotate(rotation);
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
