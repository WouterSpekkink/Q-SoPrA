#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include "EventItem.h"

class Arrow : public QGraphicsLineItem {

public:

  Arrow(EventItem *startItem, EventItem *endItem,
	QString subType, QString subCoder, QGraphicsItem *parent = 0);
  ~Arrow() {};

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void setColor(const QColor &subColor) { color = subColor; }
  EventItem *startItem() const { return start; }
  EventItem *endItem() const { return end; }

  void updatePosition();
  
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
  EventItem *start;
  EventItem *end;
  QColor color;
  QPolygonF arrowHead;
  QString type;
  QString coder;
};

#endif
