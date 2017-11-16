#ifndef EVENTITEM_H
#define EVENTITEM_H

#include <QGraphicsItem>

/*
  So graphics items are things that can be draw. They are classes, and 
  here we subclass one.
*/


class EventItem : public QGraphicsItem {

public:
  EventItem(int subWidth, QString toolTip, QPointF originalPosition, int subId);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setOriginalPos(qreal x, qreal y);
  int getCorrection();
  QPointF getOriginalPos();
  int getId();
  
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  QColor color;
  int width;
  QPointF originalPos;
  int id; 
 
};

#endif
