#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <QGraphicsItem>
#include "SupportingFunctions.h"

class NetworkNodeLabel;

class NetworkNode : public QGraphicsItem
{

public:
  NetworkNode(QString submittedName, QString submittedDescription);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setColor(QColor newColor);
  QColor getColor();
  void setSelectionColor(const QColor &subColor);
  int getCorrection();
  QString getName() const;
  QString getDescription();
  void setMode(const QString &submittedMode);
  QString getMode() const;
  void move(QPointF newPos);
  bool isPersistent();
  void setPersistent(bool state);
  bool isMassHidden();
  void setMassHidden(bool state);
  
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
  QPointF previousPos;
  
  QString name;
  QString description;
  QString mode;

  bool persistent;
  bool massHidden;
};

#endif
