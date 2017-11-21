#ifndef DIRECTEDEDGE_H
#define DIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include "NetworkNode.h"

class DirectedEdge : public QGraphicsLineItem {

public:

  DirectedEdge(NetworkNode *startItem, NetworkNode *endItem, int submittedHeight, QGraphicsItem *parent = 0);
  ~DirectedEdge() {};

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
  int height;
  QRectF myBoundingRect;
  QLineF oLine;
  QPointF arrowP1;
  QPointF arrowP2;
  QPointF tempStart;
  QPointF midPoint;
  qreal theta;
};

#endif
