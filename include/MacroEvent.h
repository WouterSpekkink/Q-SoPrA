#ifndef MACROEVENT_H
#define MACROEVENT_H

#include <QGraphicsItem>
#include <QWheelEvent>

/*
  So graphics items are things that can be draw. They are classes, and 
  here we subclass one.
*/


class MacroLabel;
class EventItem;

class MacroEvent : public QGraphicsItem {

public:
  MacroEvent(int subWidth, QString submittedDescription, QPointF originalPosition, int subId,
	     QVector<EventItem*> submittedIncidents);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setColor(const QColor &subColor); 
  void setSelectionColor(const QColor &subColor);
  int getCorrection();
  QPointF getOriginalPos();
  void setOriginalPos(QPointF &newPos);
  int getId();
  void setWidth(int newWidth);
  int getWidth();
  bool isDislodged();
  void setDislodged(bool state);
  void setLabel(MacroLabel *submittedLabel);
  MacroLabel* getLabel();
  void setIncidents(QVector<EventItem*> submittedIncidents);
  QVector<EventItem*> getIncidents();
  QColor getColor();
  void setDescription(const QString text);
  QString getDescription();
  void setComment(const QString text);
  QString getComment();
  void setMacroEvent(MacroEvent *submittedEvent);
  MacroEvent* getMacroEvent();

  enum {Type = UserType + 8};
  int type() const;
  
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  MacroLabel *label;
  MacroEvent *macroEvent;
  QVector<EventItem*> incidents;
  QColor color;
  QColor selectionColor;
  QPointF originalPos;
  int id;
  int width;
  bool dislodged;
  QString description;
  QString comment;
};

#endif
