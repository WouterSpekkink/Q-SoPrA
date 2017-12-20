#ifndef SORTFUNCTIONS_H
#define SORTFUNCTIONS_H
#include <QGraphicsItem>
#include "EventItem.h"
#include "OccurrenceItem.h"
#include "MacroEvent.h"

bool componentsSort(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo);
bool eventLessThan(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo);
bool eventLessThanWidth(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo);
bool originalLessThan(const EventItem *itemOne, const EventItem *itemTwo);
bool intMoreThan(const int itemOne, const int itemTwo);
bool attributeLessThan(const OccurrenceItem *itemOne, const OccurrenceItem *itemTwo);
bool stringSort(const QString one, const QString two);

#endif
