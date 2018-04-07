#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include <QGraphicsTextItem>

class TextObject : public QGraphicsTextItem {

public:
  TextObject(const QString &text = QString(),
	     QGraphicsTextItem *parent = 0);
  ~TextObject() {};

  qreal getRotationValue();
  void setRotationValue(qreal newRotation);
  QPointF getCenter();
  
  enum {Type = UserType + 12};
  int type() const;

private:
  qreal rotation;
};

#endif
