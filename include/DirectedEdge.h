#ifndef DIRECTEDEDGE_H
#define DIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include "NetworkNode.h"

class DirectedEdge : public QGraphicsLineItem {

public:

  DirectedEdge(NetworkNode *startItem, NetworkNode *endItem, QString submittedType,
	       QString submittedName, QGraphicsItem *parent = 0);
  ~DirectedEdge() {};

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void setColor(const QColor &subColor);
  NetworkNode *startItem() const;
  NetworkNode *endItem() const;
  void calc();
  QString getType();
  void setHeight(int submittedHeight);
  int getHeight();
  QString getName();
  bool isFiltered();
  void setFiltered(bool state);
  
  enum {Type = UserType + 4};
  int type() const;
  
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
  NetworkNode *start;
  NetworkNode *end;
  QColor color;
  QPolygonF arrowHead;
  int height;
  QLineF oLine;
  QPointF arrowP1;
  QPointF arrowP2;
  QPointF tempStart;
  QPointF midPoint;
  qreal theta;
  QString relType;
  QString name;
  bool filtered;
};

#endif
