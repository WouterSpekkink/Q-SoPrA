#ifndef OCCURRENCELABEL_H
#define OCCURRENCELABEL_H

#include <QGraphicsTextItem>
#include "OccurrenceItem.h"

class OccurrenceLabel : public QGraphicsTextItem {

public:
  OccurrenceLabel(OccurrenceItem *submittedOccurrence =
		  new OccurrenceItem(30, "", QPointF(0,0), -1, -1, ""));
  ~OccurrenceLabel() {};

  OccurrenceItem* getOccurrence();
  void setNewPos(QPointF nodePos, qreal xOffsetIncrement = 0, qreal yOffsetIncrement = 0);

  enum {Type = UserType + 3};
  int type() const {return Type;}

  QPointF getOffset();
  void setOffset(QPointF offset);
  
private:
  OccurrenceItem *occurrence;
  qreal xOffset;
  qreal yOffset;
};

#endif
