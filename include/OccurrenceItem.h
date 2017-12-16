#ifndef OCCURRENCEITEM_H
#define OCCURRENCEITEM_H

#include <QGraphicsItem>
#include <QWheelEvent>

/*
  So graphics items are things that can be draw. They are classes, and 
  here we subclass one.
*/


class OccurrenceLabel;

class OccurrenceItem : public QGraphicsItem {

public:
  OccurrenceItem(int subWidth, QString toolTip, QPointF originalPosition, int subId,
		 int subOrder, QString submittedAttribute);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setColor(const QColor &subColor); 
  void setSelectionColor(const QColor &subColor);
  int getCorrection();
  QPointF getOriginalPos() const;
  void setOriginalPos(QPointF newPos);
  int getId() const;
  int getOrder() const;
  void setWidth(int newWidth);
  int getWidth() const;
  void setLabel(OccurrenceLabel *submittedLabel);
  OccurrenceLabel* getLabel();
  QColor getColor();
  enum {Type = UserType + 10};
  int type() const;
  QString getAttribute() const;
  void setAttribute(const QString submittedAttribute);
  QPainterPath shape() const override;
  void setPermHidden(bool status);
  bool isPermHidden();
  
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  OccurrenceLabel *label;
  QColor color;
  QColor selectionColor;
  QPointF originalPos;
  int id;
  int width;
  int order;
  QString attribute;
  bool permHidden;
  
};

#endif
