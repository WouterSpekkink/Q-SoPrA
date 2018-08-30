#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#include <QGraphicsLineItem>

class LineObject : public QGraphicsLineItem
{

public: 
  LineObject(QPointF subStartPos = QPointF(0, 0),
	     QPointF subEndPos = QPointF(0, 10),
	     QGraphicsItem *parent = 0);
  ~LineObject() {};

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void setColor(const QColor &subColor);
  QColor getColor();
  QPointF getStartPos();
  QPointF getEndPos();
  void setStartPos(QPointF subPoint);
  void setEndPos(QPointF subPoint);
  void setStartPos(qreal x, qreal y);
  void setEndPos(qreal x, qreal y);
  bool arrow1();
  void setArrow1(bool status);
  bool arrow2();
  void setArrow2(bool status);  
  int getPenWidth();
  void setPenWidth(int width);
  int getPenStyle();
  void setPenStyle(int style);
  
  enum {Type = UserType + 11};
  int type() const;
  
protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
  void calculate();
  void mousePressEvent(QGraphicsSceneMouseEvent *) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;
  
private:
  QPointF startPos;
  QPointF endPos;
  QColor color;
  QLineF tempLine1;
  QLineF tempLine2;
  QPolygonF arrowHead;
  QPolygonF arrowHead2;
  QPointF arrowP1;
  QPointF arrowP2;
  QPointF arrowP3;
  QPointF arrowP4;
  bool arrow1On;
  bool arrow2On;
  int penWidth;
  int penStyle;
  
};

#endif
