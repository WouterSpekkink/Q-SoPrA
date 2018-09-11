#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include "EventItem.h"
#include "MacroEvent.h"
#include "OccurrenceItem.h"

class Arrow : public QGraphicsLineItem
{

public: 
  Arrow(QString subType, QString subCoder, QGraphicsItem *parent = 0);
  ~Arrow() {};

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void updatePosition();
  void setColor(const QColor &subColor);
  QColor getColor();
  QGraphicsItem *startItem() const;
  QGraphicsItem *endItem() const;

  enum {Type = UserType + 2};
  int type() const;
  bool isCopy();
  void setCopy(bool status);
  QString getType();

  void setStartItem(QGraphicsItem *subStart);
  void setEndItem(QGraphicsItem *subEnd);

  void setPenStyle(int style);
  int getPenStyle();

  void setHeight(int submittedHeight);
  int getHeight();
  bool isMassHidden();
  void setMassHidden(bool status);
  
protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
  void calculate();

private:
  QGraphicsItem *start;
  QGraphicsItem *end;
  QColor color;
  QPolygonF arrowHead;
  QPainterPath strokePath;
  QString typeInd;
  QString coder;
  QLineF newLine;
  QLineF ghostLine;
  QPointF arrowP1;
  QPointF arrowP2;
  QPointF controlPoint;
  int height;
  bool copy;
  bool deleted;
  int penWidth;
  int penStyle;
  bool massHidden;
};

#endif
