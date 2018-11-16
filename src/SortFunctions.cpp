#include "../include/SortFunctions.h"

bool componentsSort(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) 
{
  const IncidentNode *eventOne = qgraphicsitem_cast<const IncidentNode*>(itemOne);
  const IncidentNode *eventTwo = qgraphicsitem_cast<const IncidentNode*>(itemTwo);
  const AbstractNode *abstractNodeOne = qgraphicsitem_cast<const AbstractNode*>(itemOne);
  const AbstractNode *abstractNodeTwo = qgraphicsitem_cast<const AbstractNode*>(itemTwo);
  if (eventOne && eventTwo) 
    {
      return (eventOne->getOrder() < eventTwo->getOrder());
    }
  else if (eventOne && abstractNodeTwo) 
    {
      return (eventOne->getOrder() < abstractNodeTwo->getIncidents().first()->getOrder());
    }
  else if (abstractNodeOne && abstractNodeTwo) 
    {
      return (abstractNodeOne->getIncidents().first()->getOrder() <
	      abstractNodeTwo->getIncidents().first()->getOrder());
    }
  else 
    {
      return (abstractNodeOne->getIncidents().first()->getOrder() < eventTwo->getOrder());
    }
}

bool eventLessThan(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) 
{
  qreal orderOne = itemOne->scenePos().x();
  qreal orderTwo = itemTwo->scenePos().x();
  if (orderOne < orderTwo) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool eventLessThanWidth(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) 
{
  const IncidentNode *eventOne = qgraphicsitem_cast<const IncidentNode*>(itemOne);
  const IncidentNode *eventTwo = qgraphicsitem_cast<const IncidentNode*>(itemTwo);
  const AbstractNode *abstractNodeOne = qgraphicsitem_cast<const AbstractNode*>(itemOne);
  const AbstractNode *abstractNodeTwo = qgraphicsitem_cast<const AbstractNode*>(itemTwo);
  qreal valueOne = 0.0;
  qreal valueTwo = 0.0;
  if (eventOne && eventTwo) 
    {
      valueOne = eventOne->scenePos().x() + eventOne->getWidth();
      valueTwo = eventTwo->scenePos().x() + eventTwo->getWidth();
    }
  else if (eventOne && abstractNodeTwo) 
    {
      valueOne = eventOne->scenePos().x() + eventOne->getWidth();
      valueTwo = abstractNodeTwo->scenePos().x() + abstractNodeTwo->getWidth();
    }
  else if (abstractNodeOne && abstractNodeTwo) 
    {
      valueOne = abstractNodeOne->scenePos().x() + abstractNodeOne->getWidth();
      valueTwo = abstractNodeTwo->scenePos().x() + abstractNodeTwo->getWidth();
    }
  else 
    {
      valueOne = abstractNodeOne->scenePos().x() + abstractNodeOne->getWidth();
      valueTwo = eventTwo->scenePos().x() + eventTwo->getWidth();
    }
  if (valueOne < valueTwo) 
    {
      return true;
    }
  else 
    {
      return false;
    }
} 

bool originalLessThan(const IncidentNode *itemOne, const IncidentNode *itemTwo) 
{
  qreal orderOne = itemOne->getOriginalPos().x();
  qreal orderTwo = itemTwo->getOriginalPos().x();
  if (orderOne < orderTwo) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool intMoreThan(const int itemOne, const int itemTwo) 
{
  if (itemOne > itemTwo) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool intLessThan(const int itemOne, const int itemTwo) 
{
  if (itemOne < itemTwo) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool attributeLessThan(const OccurrenceItem *itemOne, const OccurrenceItem *itemTwo) 
{
  QString attributeOne = itemOne->getAttribute();
  QString attributeTwo = itemTwo->getAttribute();
  if (attributeOne < attributeTwo) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool stringSort(const QString one, const QString two) 
{
  if (one < two) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool nodeLessThan(const NetworkNode *itemOne, const NetworkNode *itemTwo) 
{
  qreal oneX = itemOne->scenePos().x();
  qreal twoX = itemTwo->scenePos().x();
  if (oneX < twoX) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool nameLessThan(const NetworkNode *itemOne, const NetworkNode *itemTwo) 
{
  QString one = itemOne->getName();
  QString two = itemTwo->getName();
  if (one < two) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool stringLessThan(const QString one, const QString two) 
{
  if (one < two) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool modeSort(const NetworkNode *itemOne, const NetworkNode *itemTwo) 
{
  if (itemOne->getMode() < itemTwo->getMode()) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

bool occurrencesSort(const OccurrenceItem *itemOne, const OccurrenceItem *itemTwo) 
{
  if (itemOne->getOrder() < itemTwo->getOrder()) 
    {
      return true;
    }
  else 
    {
      return false;
    }
}

