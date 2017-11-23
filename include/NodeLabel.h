#ifndef NODELABEL_H
#define NODELABEL_H

#include <QGraphicsTextItem>
#include "EventItem.h"

class NodeLabel : public QGraphicsTextItem {

public:
  NodeLabel(EventItem *submittedNode = new EventItem(30, "", QPointF(0,0), -1));
  ~NodeLabel() {};

  EventItem* getNode();
  void setNewPos(QPointF nodePos, qreal xOffsetIncrement = 0, qreal yOffsetIncrement = 0);

  enum {Type = UserType + 3};
  int type() const {return Type;}

  QPointF getOffset();
  void setOffset(QPointF offset);
  
private:
  EventItem *node;
  qreal xOffset;
  qreal yOffset;
};

#endif
