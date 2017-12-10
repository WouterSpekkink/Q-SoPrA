#ifndef EVENTITEM_H
#define EVENTITEM_H

#include <QGraphicsItem>
#include <QWheelEvent>

/*
  So graphics items are things that can be draw. They are classes, and 
  here we subclass one.
*/


class NodeLabel;
class MacroEvent;

class EventItem : public QGraphicsItem {

public:
  EventItem(int subWidth, QString toolTip, QPointF originalPosition, int subId, int subOrder);

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
  int getWidth();
  bool isDislodged();
  void setDislodged(bool state);
  void setLabel(NodeLabel *submittedLabel);
  NodeLabel* getLabel();
  QColor getColor();
  enum {Type = UserType + 1};
  int type() const;
  void setMacroEvent(MacroEvent* submittedEvent);
  MacroEvent* getMacroEvent();
  
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  MacroEvent *macroEvent;
  NodeLabel *label;
  QColor color;
  QColor selectionColor;
  QPointF originalPos;
  int id;
  int width;
  bool dislodged;
  int order;
 
};

#endif
