#ifndef MACROEVENT_H
#define MACROEVENT_H

#include <QGraphicsItem>
#include <QWheelEvent>
#include "SupportingFunctions.h"

/*
  So graphics items are things that can be draw. They are classes, and 
  here we subclass one.
*/


class MacroLabel;
class EventItem;

class MacroEvent : public QGraphicsItem {

public:
  MacroEvent(int subWidth, QString submittedDescription, QPointF originalPosition, int subId,
	     QString submittedConstraint, QVector<EventItem*> submittedIncidents);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void setColor(const QColor &subColor); 
  void setSelectionColor(const QColor &subColor);
  int getCorrection();
  QPointF getOriginalPos();
  void setOriginalPos(QPointF newPos);
  int getId();
  void setWidth(int newWidth);
  int getWidth() const;
  bool isDislodged();
  void setDislodged(bool state);
  void setLabel(MacroLabel *submittedLabel);
  MacroLabel* getLabel();
  void setIncidents(QVector<EventItem*> submittedIncidents);
  QVector<EventItem*> getIncidents() const;
  QColor getColor();
  void setDescription(const QString text);
  QString getDescription();
  void setComment(const QString text);
  QString getComment();
  void setMacroEvent(MacroEvent *submittedEvent);
  MacroEvent* getMacroEvent();
  void setOrder(int submittedOrder);
  int getOrder();
  enum {Type = UserType + 8};
  int type() const;
  void insertAttribute(QString attribute);
  void removeAttribute(QString attribute);
  void setAttributes(QSet<QString> submittedAttributes);
  QSet<QString> getAttributes();
  void insertValue(QString attribute, QString value);
  void removeValue(QString attribute);
  void setValues(QMap<QString, QString> submittedValue);
  QMap<QString, QString> getValues();
  void setMode(const QString submittedMode);
  QString getMode() const;
  QString getConstraint() const;
  bool isCopy();
  void setCopy(bool status);
  void setDistAncestor(qreal dist);
  qreal getDistAncestor();
  
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  MacroLabel *label;
  MacroEvent *macroEvent;
  QVector<EventItem*> incidents;
  QSet<QString> attributes;
  QMap<QString, QString> values;
  QColor color;
  QColor selectionColor;
  QPointF originalPos;
  int id;
  int width;
  int order;
  bool dislodged;
  QString description;
  QString comment;
  QString mode;
  QString constraint;
  bool copy;
  qreal distAncestor;
};

#endif
