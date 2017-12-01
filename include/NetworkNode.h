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
  QColor getColor();
  void setSelectionColor(const QColor &subColor);
  void setOriginalPos(qreal x, qreal y);
  int getCorrection();
  QString getName();
  QString getDescription();
  void setMode(const QString &submittedMode);
  QString getMode();

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
  QColor selectionColor;
  QPointF originalPos;
  int width;
  QPointF previousPos;
  
  QString name;
  QString description;
  QString mode;
};

#endif
