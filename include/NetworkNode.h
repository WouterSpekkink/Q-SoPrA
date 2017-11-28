#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <QGraphicsItem>
//#include "NetworkNodeLabel.h"

/*
  So graphics items are things that can be draw. They are classes, and 
  here we subclass one.
*/


class NetworkNodeLabel;

class NetworkNode : public QGraphicsItem {

public:
  NetworkNode(QString submittedName, QString submittedDescription);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setColor(QColor newColor);
  void setOriginalPos(qreal x, qreal y);
  int getCorrection();
  QString getName();

  void setLabel(NetworkNodeLabel *newLabel);
  NetworkNodeLabel* getLabel();
  
  enum {Type = UserType + 6};
  int type() const;
  
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  NetworkNodeLabel *label;
  
  QColor color;
  QPointF originalPos;
  int width;
  QPointF previousPos;
  
  QString name;
  QString description;
};

#endif
