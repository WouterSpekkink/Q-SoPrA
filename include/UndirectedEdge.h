#ifndef UNDIRECTEDEDGE_H
#define UNDIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include "NetworkNode.h"

class UndirectedEdge : public QGraphicsLineItem {

public:

  UndirectedEdge(NetworkNode *startItem, NetworkNode *endItem, int submittedHeight, QGraphicsItem *parent = 0);
  ~UndirectedEdge() {};

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void setColor(const QColor &subColor) { color = subColor; }
  NetworkNode *startItem() const { return start; }
  NetworkNode *endItem() const { return end; }
  void calc();
  
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
  NetworkNode *start;
  NetworkNode *end;
  QColor color;
  QPolygonF arrowHead;
  QPolygonF arrowHead2;
  int height;
  QLineF oLine;
  QPointF arrowP1;
  QPointF arrowP2;
  QPointF tempStart;
  QPointF midPoint;
  qreal theta;
};

#endif
