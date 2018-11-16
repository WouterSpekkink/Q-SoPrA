#ifndef INCIDENTNODELABEL_H
#define INCIDENTNODELABEL_H

#include <QGraphicsTextItem>
#include "IncidentNode.h"

class IncidentNodeLabel : public QGraphicsTextItem
{

public:
  IncidentNodeLabel(IncidentNode *submittedNode = new IncidentNode(30, "", QPointF(0,0), -1, -1));
  ~IncidentNodeLabel() {};

  IncidentNode* getNode();
  void setNewPos(QPointF nodePos, qreal xOffsetIncrement = 0, qreal yOffsetIncrement = 0);

  enum {Type = UserType + 3};
  int type() const {return Type;}

  QPointF getOffset();
  void setOffset(QPointF offset);
  
private:
  IncidentNode *node;
  qreal xOffset;
  qreal yOffset;
};

#endif
