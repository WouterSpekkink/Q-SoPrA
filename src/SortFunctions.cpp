#include "../include/SortFunctions.h"

bool componentsSort(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) {
  const EventItem *eventOne = qgraphicsitem_cast<const EventItem*>(itemOne);
  const EventItem *eventTwo = qgraphicsitem_cast<const EventItem*>(itemTwo);
  const MacroEvent *macroOne = qgraphicsitem_cast<const MacroEvent*>(itemOne);
  const MacroEvent *macroTwo = qgraphicsitem_cast<const MacroEvent*>(itemTwo);
  if (eventOne && eventTwo) {
    return (eventOne->getOrder() < eventTwo->getOrder());
  } else if (eventOne && macroTwo) {
    return (eventOne->getOrder() < macroTwo->getIncidents().first()->getOrder());
  } else if (macroOne && macroTwo) {
    return (macroOne->getIncidents().first()->getOrder() <
	    macroTwo->getIncidents().first()->getOrder());
  } else {
    return (macroOne->getIncidents().first()->getOrder() < eventTwo->getOrder());
  }
}

bool eventLessThan(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) {
  qreal orderOne = itemOne->scenePos().x();
  qreal orderTwo = itemTwo->scenePos().x();
  if (orderOne < orderTwo) {
    return true;
  } else {
    return false;
  }
}

bool eventLessThanWidth(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) {
  const EventItem *eventOne = qgraphicsitem_cast<const EventItem*>(itemOne);
  const EventItem *eventTwo = qgraphicsitem_cast<const EventItem*>(itemTwo);
  const MacroEvent *macroOne = qgraphicsitem_cast<const MacroEvent*>(itemOne);
  const MacroEvent *macroTwo = qgraphicsitem_cast<const MacroEvent*>(itemTwo);
  qreal valueOne = 0.0;
  qreal valueTwo = 0.0;
  if (eventOne && eventTwo) {
    valueOne = eventOne->scenePos().x() + eventOne->getWidth();
    valueTwo = eventTwo->scenePos().x() + eventTwo->getWidth();
  } else if (eventOne && macroTwo) {
    valueOne = eventOne->scenePos().x() + eventOne->getWidth();
    valueTwo = macroTwo->scenePos().x() + macroTwo->getWidth();
  } else if (macroOne && macroTwo) {
    valueOne = macroOne->scenePos().x() + macroOne->getWidth();
    valueTwo = macroTwo->scenePos().x() + macroTwo->getWidth();
  } else {
    valueOne = macroOne->scenePos().x() + macroOne->getWidth();
    valueTwo = eventTwo->scenePos().x() + eventTwo->getWidth();
  }
  if (valueOne < valueTwo) {
    return true;
  } else {
    return false;
  }
} 

bool originalLessThan(const EventItem *itemOne, const EventItem *itemTwo) {
  qreal orderOne = itemOne->getOriginalPos().x();
  qreal orderTwo = itemTwo->getOriginalPos().x();
  if (orderOne < orderTwo) {
    return true;
  } else {
    return false;
  }
}

bool intMoreThan(const int itemOne, const int itemTwo) {
  if (itemOne > itemTwo) {
    return true;
  } else {
    return false;
  }
}

bool attributeLessThan(const OccurrenceItem *itemOne, const OccurrenceItem *itemTwo) {
  QString attributeOne = itemOne->getAttribute();
  QString attributeTwo = itemTwo->getAttribute();
  if (attributeOne < attributeTwo) {
    return true;
  } else {
    return false;
  }
}

bool stringSort(const QString one, const QString two) {
  if (one < two) {
    return true;
  } else {
    return false;
  }
}

