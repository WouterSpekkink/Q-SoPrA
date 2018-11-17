/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SORTFUNCTIONS_H
#define SORTFUNCTIONS_H
#include <QGraphicsItem>
#include "IncidentNode.h"
#include "OccurrenceItem.h"
#include "AbstractNode.h"
#include "NetworkNode.h"

bool componentsSort(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo);
bool eventLessThan(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo);
bool eventLessThanWidth(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo);
bool originalLessThan(const IncidentNode *itemOne, const IncidentNode *itemTwo);
bool intMoreThan(const int itemOne, const int itemTwo);
bool intLessThan(const int itemOne, const int itemTwo);
bool attributeLessThan(const OccurrenceItem *itemOne, const OccurrenceItem *itemTwo);
bool stringSort(const QString one, const QString two);
bool nodeLessThan(const NetworkNode *itemOne, const NetworkNode *itemTwo);
bool nameLessThan(const NetworkNode *itemOne, const NetworkNode *itemTwo);
bool modeSort(const NetworkNode *itemOne, const NetworkNode *itemTwo);
bool occurrencesSort(const OccurrenceItem *itemOne, const OccurrenceItem *itemTwo);

#endif
