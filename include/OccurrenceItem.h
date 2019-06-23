#ifndef OCCURRENCEITEM_H
#define OCCURRENCEITEM_H

#include <QGraphicsItem>
#include <QWheelEvent>
#include "SupportingFunctions.h"

/*
  So graphics items are things that can be draw. They are classes, and 
  here we subclass one.
*/


class OccurrenceLabel;

class OccurrenceItem : public QGraphicsItem
{

public:
  // Constructor and destructor
  OccurrenceItem(int width, QString toolTip, QPointF originalPos, int id,
		 int order, QString attribute);

  // Overrides of some public functions
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  QPainterPath shape() const override;
  
  // Setters
  void setColor(const QColor &color); 
  void setSelectionColor(const QColor &color);
  void setOriginalPos(const QPointF &originalPos);
  void setWidth(const int &width);
  void setLabel(OccurrenceLabel *occurrenceLabelPtr);
  void setAttribute(const QString &attribute);
  void setPermHidden(bool state);
  void setGrouped(bool state);
  void setLastPos(QPointF lastPos);
  void setId(int id);
  
  // Getters
  int getCorrection();
  QPointF getOriginalPos() const;
  int getId() const;
  int getOrder() const;
  int getWidth() const;
  OccurrenceLabel* getLabel();
  QColor getColor();
  QString getAttribute() const;
  bool isPermHidden();
  bool isGrouped();
  QPointF getLastPos();

  // Type checking
  enum {Type = UserType + 10};
  int type() const;
 
protected:
  // Overrides of events
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  // Private variables
  QColor _color;
  QColor _selectionColor;
  QPointF _originalPos;
  QPointF _lastPos;
  int _id;
  int _width;
  int _order;
  QString _attribute;
  bool _permHidden;
  bool _grouped;

  // Pointer to label, created by other class
  // Do not delete.
  OccurrenceLabel *_occurrenceLabelPtr;
  
};

#endif
