#ifndef UNDIRECTEDEDGE_H
#define UNDIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include "NetworkNode.h"

class UndirectedEdge : public QGraphicsLineItem {

public:

  UndirectedEdge(NetworkNode *startItem, NetworkNode *endItem, QString submittedType,
		 QString submittedName, QGraphicsItem *parent = 0);
  ~UndirectedEdge() {};

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
  
  enum {Type = UserType + 5};
  int type() const;

  
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
  QLineF sLine;
  QPointF arrowP1;
  QPointF arrowP2;
  QPointF arrowP3;
  QPointF arrowP4;
  QPointF tempStart;
  QPointF midPoint;
  qreal theta;
  QString relType;
  QString name;
  bool filtered;
};

#endif
