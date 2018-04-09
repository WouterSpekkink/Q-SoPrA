#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include <QGraphicsTextItem>
#include <QApplication>

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

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
  
private:
  qreal rotation;
};

#endif
