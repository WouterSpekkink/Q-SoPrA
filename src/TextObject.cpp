#include "../include/TextObject.h"

TextObject::TextObject(const QString &text,
		       QGraphicsTextItem *parent)
  : QGraphicsTextItem(parent) {
  setPlainText(text);
  setFlag(QGraphicsTextItem::ItemIsMovable, true);
  setFlag(QGraphicsTextItem::ItemIsSelectable, true);
}

int TextObject::type() const {
  return Type;
}
