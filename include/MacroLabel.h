#ifndef MACROLABEL_H
#define MACROLABEL_H

#include <QGraphicsTextItem>
#include "MacroEvent.h"

class MacroLabel : public QGraphicsTextItem
{

public:
  MacroLabel(MacroEvent *submittedEvent = NULL);
  ~MacroLabel() {};

  MacroEvent* getMacroEvent();
  void setNewPos(QPointF nodePos, qreal xOffsetIncrement = 0, qreal yOffsetIncrement = 0);

  enum {Type = UserType + 9};
  int type() const {return Type;}

  QPointF getOffset();
  void setOffset(QPointF offset);
  
private:
  MacroEvent *macroEvent;
  qreal xOffset;
  qreal yOffset;
};

#endif
