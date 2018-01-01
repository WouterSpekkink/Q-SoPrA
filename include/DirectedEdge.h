#ifndef DIRECTEDEDGE_H
#define DIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include "NetworkNode.h"
#include "SupportingFunctions.h"

class DirectedEdge : public QGraphicsLineItem {

public:

  DirectedEdge(NetworkNode *startItem, NetworkNode *endItem, QString submittedType,
	       QString submittedName, QGraphicsItem *parent = 0);
  ~DirectedEdge() {};

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void setColor(const QColor &subColor);
  QColor getColor();
  NetworkNode *startItem() const;
  NetworkNode *endItem() const;
  void calc();
  QString getType();
  void setType(const QString submittedType);
  void setHeight(int submittedHeight);
  int getHeight();
  QString getName();
  void setName(const QString submittedName);
  bool isFiltered();
  void setFiltered(bool state);
  bool isMassHidden();
  void setMassHidden(bool state);
  void setComment(const QString submittedComment);
  QString getComment();
  
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
  QString comment;
  bool filtered;
  bool massHidden;
};

#endif
