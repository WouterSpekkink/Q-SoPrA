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
  EventItem(int subWidth = 40,
	    QString toolTip = QString(),
	    QPointF originalPosition = QPointF(),
	    int subId = -1,
	    int subOrder = -1,
	    QGraphicsItem *parent = 0);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  QColor getColor();
  void setColor(const QColor &subColor); 
  void setSelectionColor(const QColor &subColor);
  int getCorrection();
  QPointF getOriginalPos() const;
  void setOriginalPos(QPointF newPos);
  int getId() const;
  int getOrder() const;
  void setWidth(int newWidth);
  int getWidth() const;
  bool isDislodged();
  void setDislodged(bool state);
  void setLabel(NodeLabel *submittedLabel);
  NodeLabel* getLabel();
  void setMacroEvent(MacroEvent* submittedEvent);
  MacroEvent* getMacroEvent();
  void setMode(const QString submittedMode);
  QString getMode() const;
  bool isCopy();
  void setCopy(bool status);

  QPainterPath shape() const override;
  
  enum {Type = UserType + 1};
  int type() const;
  
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
  QString mode;
  bool copy;
};

#endif
