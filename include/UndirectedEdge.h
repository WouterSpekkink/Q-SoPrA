#ifndef UNDIRECTEDEDGE_H
#define UNDIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include "NetworkNode.h"
#include "SupportingFunctions.h"

class UndirectedEdge : public QGraphicsLineItem
{

public:

  UndirectedEdge(NetworkNode *startItem = NULL,
		 NetworkNode *endItem = NULL, QString submittedType = QString(),
		 QString submittedName = QString(), QGraphicsItem *parent = 0);
  ~UndirectedEdge() {};

  QRectF boundingRect() const override;
  void updatePosition();
  void setColor(const QColor &subColor);
  QColor getColor();
  NetworkNode *startItem() const; 
  NetworkNode *endItem() const;
  void calculate();
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
  
  enum {Type = UserType + 5};
  int type() const;

  
protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
  NetworkNode *start;
  NetworkNode *end;
  QColor color;
  QPolygonF arrowHeadOne;
  QPolygonF arrowHeadTwo;
  QPainterPath strokePath;
  int height;
  QLineF ghostLineOne;
  QLineF ghostLineTwo;
  QPointF arrowP1;
  QPointF arrowP2;
  QPointF arrowP3;
  QPointF arrowP4;
  QPointF controlPoint;
  QString relType;
  QString name;
  QString comment;
  bool filtered;
  bool massHidden;
};

#endif
