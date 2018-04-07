#ifndef RECTOBJECT
#define RECTOBJECT

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>

class RectObject : public QGraphicsItem {

public:
  RectObject();
  ~RectObject() {};

  void paint(QPainter *painter,
	     const QStyleOptionGraphicsItem *option,
	     QWidget *widget) override;
  QRectF boundingRect() const override;
  QColor getColor();
  void setColor(const QColor &subColor);
  qreal getLeft();
  void setLeft(qreal newLeft);
  qreal getRight();
  void setRight(qreal newRight);
  qreal getTop();
  void setTop(qreal newTop);
  qreal getBottom();
  void setBottom(qreal newBottom);
  QPointF topLeft();
  QPointF topRight();
  QPointF bottomLeft();
  QPointF bottomRight();
  void setTopLeft(QPointF topLeft);
  void setTopRight(QPointF topRight);
  void setBottomLeft(QPointF bottomLeft);
  void setBottomRight(QPointF bottomRight);
  QPointF getCenter();
  void moveCenter(QPointF newCenter);
  qreal getRotationValue();
  void setRotationValue(qreal newRotation);

  QPainterPath shape() const override;
  
  enum {Type = UserType + 14};
  int type() const;

private:
  QRectF drawRect;
  QColor color;
  qreal rotation;
};

#endif
