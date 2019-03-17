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

#include "../include/Scene.h"
#include "../include/GraphicsView.h"
#include "../include/BandlessGraphicsView.h"
#include <math.h>
#include <QtCore>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) 
{
  _resizeOnIncidentNode = false;
  _resizeOnAbstractNode = false;
  _moveOn = false;
  _lineMoveOn = false;
  _moveLineObject = false;
  _manipulateEllipse = false;
  _moveEllipse = false;
  _rotateEllipse = false;
  _manipulateRect = false;
  _moveRect = false;
  _rotateRect = false;
  _moveText = false;
  _rotateText = false;
  _hierarchyMove = false;
  _eventWidthChange = false;
  _moveNetworkNodeLabel = false;
}

QRectF Scene::itemsBoundingRect() const 
{
  QRectF boundingRect;
  const auto items_ = items();
  for (QGraphicsItem *item : items_)
    if (item->isVisible()) 
      {
	boundingRect |= item->sceneBoundingRect();
      }
  return boundingRect.adjusted(-500, -500, 500, 500);
}

void Scene::modEventWidth(QGraphicsItem *item) 
{
  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(item);
  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(item);
  if (incident) 
    {
      _eventWidthChange = true;
      _selectedIncidentNodePtr = incident;
      clearSelection();
      incident->setSelected(true);
    }
  else if (abstractNode) 
    {
      _selectedAbstractNodePtr = abstractNode;
      _eventWidthChange = true;
      clearSelection();
      abstractNode->setSelected(true);
    }
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) 
{
  if (_eventWidthChange) 
    {
      _eventWidthChange = false;
    }
  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(wheelEvent->scenePos(),
							      QTransform()));
  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(wheelEvent->scenePos(),
							     QTransform()));
  IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(wheelEvent->scenePos(),
							       QTransform()));
  AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(wheelEvent->scenePos(),
								  QTransform()));
  TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(wheelEvent->scenePos(),
							    QTransform()));
  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(wheelEvent->scenePos(),
								     QTransform()));
  LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(wheelEvent->scenePos(),
							    QTransform()));
  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(wheelEvent->scenePos(),
							    QTransform()));
  if (incidentNodeLabel) 
    {
      incident = incidentNodeLabel->getNode();
    }
  if (abstractNodeLabel) 
    {
      abstractNode = abstractNodeLabel->getAbstractNode();
    }
  if (incident && !incident->isCopy()) 
    {
      if (wheelEvent->modifiers() & Qt::ShiftModifier) 
	{
	  if (incident->isSelected()) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  incident->setWidth(incident->getWidth() + 5);
		  QPointF original = incident->scenePos();
		  incident->getLabel()->setNewPos(original, 2.5);
		}
	      else if (wheelEvent->delta() < 0 && incident->getWidth() > 40) 
		{
		  incident->setWidth(incident->getWidth() - 5);
		  QPointF original = incident->scenePos();
		  incident->getLabel()->setNewPos(original, -2.5);
		}
	      emit relevantChange();
	      wheelEvent->accept();
	    }
	  else 
	    {
	      wheelEvent->ignore();
	    }
	}
      else if (wheelEvent->modifiers() & Qt::ControlModifier) 
	{
	  wheelEvent->ignore();
	}
    }
  else if (abstractNode && !abstractNode->isCopy()) 
    {
      if (wheelEvent->modifiers() & Qt::ShiftModifier) 
	{
	  if (abstractNode->isSelected()) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  abstractNode->setWidth(abstractNode->getWidth() + 5);
		  QPointF original = abstractNode->scenePos();
		  abstractNode->getLabel()->setNewPos(original, 2.5);
		}
	      else if (wheelEvent->delta() < 0 && abstractNode->getWidth() > 40) 
		{
		  abstractNode->setWidth(abstractNode->getWidth() - 5);
		  QPointF original = abstractNode->scenePos();
		  abstractNode->getLabel()->setNewPos(original, -2.5);
		}
	      emit relevantChange();
	      wheelEvent->accept();
	    }
	  else 
	    {
	      wheelEvent->ignore();
	    }
	}
      else if (wheelEvent->modifiers() & Qt::ControlModifier) 
	{
	  wheelEvent->ignore();
	}
    }
  else if (text) 
    {
      if (text->isSelected()) 
	{
	  if (wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  QFont font = text->font();
		  int size = font.pointSize();
		  if (size <= 999) 
		    {
		      size++;
		      font.setPointSize(size);
		      text->setFont(font);
		    }
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  QFont font = text->font();
		  int size = font.pointSize();
		  if (size >= 9) 
		    {
		      size--;
		      font.setPointSize(size);
		      text->setFont(font);
		    }
		}
	    }
	  if (wheelEvent->modifiers() & Qt::ShiftModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  int width = text->textWidth();
		  width = width + 10;
		  text->setTextWidth(width);
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  int width = text->textWidth();
		  width = width - 10;
		  text->setTextWidth(width);
		}
	    }
	  emit relevantChange();
	  wheelEvent->accept();
	}
    }
  else if (ellipse) 
    {
      if (ellipse->isSelected()) 
	{
	  if (wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  if (ellipse->getPenWidth() < 20) 
		    {
		      ellipse->setPenWidth(ellipse->getPenWidth() + 1);
		    }
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  if (ellipse->getPenWidth() > 1) 
		    {
		      ellipse->setPenWidth(ellipse->getPenWidth() - 1);
		    }
		}
	    }
	  else if (wheelEvent->modifiers() & Qt::ShiftModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  if (ellipse->getPenStyle() < 5) 
		    {
		      ellipse->setPenStyle(ellipse->getPenStyle() + 1);
		    }
		  else 
		    {
		      ellipse->setPenStyle(1);
		    }
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  if (ellipse->getPenStyle() > 1) 
		    {
		      ellipse->setPenStyle(ellipse->getPenStyle() - 1);
		    }
		  else 
		    {
		      ellipse->setPenStyle(5);
		    }
		}
	    }
	  wheelEvent->accept();
	  emit relevantChange();
	}
    }
  else if (line) 
    {
      if (line->isSelected()) 
	{
	  if (wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  if (line->getPenWidth() < 7) 
		    {
		      line->setPenWidth(line->getPenWidth() + 1);
		    }
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  if (line->getPenWidth() > 1) 
		    {
		      line->setPenWidth(line->getPenWidth() - 1);
		    }
		}
	    }
	  else if (wheelEvent->modifiers() & Qt::ShiftModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  if (line->getPenStyle() < 5) 
		    {
		      line->setPenStyle(line->getPenStyle() + 1);
		    }
		  else 
		    {
		      line->setPenStyle(1);
		    }
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  if (line->getPenStyle() > 1) 
		    {
		      line->setPenStyle(line->getPenStyle() - 1);
		    }
		  else 
		    {
		      line->setPenStyle(5);
		    }
		}
	    }
	  emit relevantChange();
	  wheelEvent->accept();
	}
    }
  else if (rect) 
    {
      if (rect->isSelected()) 
	{
	  if (wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  if (rect->getPenWidth() < 20) 
		    {
		      rect->setPenWidth(rect->getPenWidth() + 1);
		    }
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  if (rect->getPenWidth() > 1) 
		    {
		      rect->setPenWidth(rect->getPenWidth() - 1);
		    }
		}
	    }
	  else if (wheelEvent->modifiers() & Qt::ShiftModifier) 
	    {
	      if (wheelEvent->delta() > 0) 
		{
		  if (rect->getPenStyle() < 5) 
		    {
		      rect->setPenStyle(rect->getPenStyle() + 1);
		    }
		  else 
		    {
		      rect->setPenStyle(1);
		    }
		}
	      else if (wheelEvent->delta() < 0) 
		{
		  if (rect->getPenStyle() > 1) 
		    {
		      rect->setPenStyle(rect->getPenStyle() - 1);
		    }
		  else 
		    {
		      rect->setPenStyle(5);
		    }
		}
	    }
	  emit relevantChange();
	  wheelEvent->accept();
	}
    }
  else 
    {
      wheelEvent->ignore();
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
  if (event->button() == Qt::LeftButton)
    {
      if (_eventWidthChange) 
	{
	  _eventWidthChange = false;
	}
      if (event->modifiers() & Qt::ControlModifier) 
	{
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(event->scenePos(),
								      QTransform()));
	  IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(event->scenePos(),
								       QTransform()));
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(event->scenePos(),
								     QTransform()));
	  AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(event->scenePos(),
									  QTransform()));
	  NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->scenePos(),
									     QTransform()));
	  OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->scenePos(),
										  QTransform()));
	  TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->scenePos(),
								    QTransform()));
	  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->scenePos(),
									     QTransform()));
	  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->scenePos(),
								    QTransform()));
	  Linkage *linkage = qgraphicsitem_cast<Linkage*>(itemAt(event->scenePos(),
							   QTransform()));
	  if (incidentNodeLabel) 
	    {
	      incident = incidentNodeLabel->getNode();
	    }
	  if (abstractNodeLabel) 
	    {
	      abstractNode = abstractNodeLabel->getAbstractNode();
	    }
	  if (linkage) 
	    {
	      clearSelection();
	      linkage->setSelected(true);
	      if (!linkage->isCopy()) 
		{
		  emit resetItemSelection();
		}
	    }
	  if (incident) 
	    {
	      if (incident->isCopy()) 
		{
		  clearSelection();
		  incident->setSelected(true);
		  _selectedIncidentNodePtr = incident;
		  _hierarchyMove = true;
		}
	      else 
		{
		  emit resetItemSelection();
		  incident->setSelected(true);
		  _selectedIncidentNodePtr = incident;
		  _moveOn = true;
		}
	    }
	  else if (abstractNode) 
	    {
	      if (abstractNode->isCopy()) 
		{
		  clearSelection();
		  abstractNode->setSelected(true);
		  _selectedAbstractNodePtr = abstractNode;
		  _hierarchyMove = true;
		}
	      else 
		{
		  emit resetItemSelection();
		  abstractNode->setSelected(true);
		  _selectedAbstractNodePtr = abstractNode;
		  _moveOn = true;
		}
	    }
	  else if (networkNode) 
	    {
	      networkNode->setSelected(true);
	      _selectedNetworkNodePtr = networkNode;
	      _moveOn = true;
	    }
	  else if (occurrence) 
	    {
	      clearSelection();
	      occurrence->setSelected(true);
	      _selectedOccurrencePtr = occurrence;
	      _moveOn = true;
	    }
	  else if (text) 
	    {
	      clearSelection();
	      text->setSelected(true);
	      emit resetItemSelection();
	      _selectedTextPtr = text;
	      _rotateText = true;
	      QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
	      qApp->processEvents();
	    }
	  else if (ellipse) 
	    {
	      clearSelection();
	      ellipse->setSelected(true);
	      emit resetItemSelection();
	      _selectedEllipsePtr = ellipse;
	      _rotateEllipse = true;
	      QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
	      qApp->processEvents();
	    }
	  else if (rect) 
	    {
	      clearSelection();
	      rect->setSelected(true);
	      emit resetItemSelection();
	      _selectedRectPtr = rect;
	      _rotateRect = true;
	      QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
	      qApp->processEvents();
	    }
	}
      else if (event->modifiers() & Qt::ShiftModifier) 
	{
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(event->scenePos(),
								      QTransform()));
	  IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(event->scenePos(),
								       QTransform()));
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(event->scenePos(),
								     QTransform()));
	  AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(event->scenePos(),
									  QTransform()));
	  LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->scenePos(),
								    QTransform()));
	  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->scenePos(),
									     QTransform()));
	  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->scenePos(),
								    QTransform()));
	  if (incidentNodeLabel) 
	    {
	      incident = incidentNodeLabel->getNode();
	    }
	  if (abstractNodeLabel) 
	    {
	      abstractNode = abstractNodeLabel->getAbstractNode();
	    }
	  if (incident) 
	    {
	      emit resetItemSelection();
	      if (event->modifiers() & Qt::AltModifier) 
		{
		  if (incident->isCopy()) 
		    {
		      incident->setPos(incident->getOriginalPos());
		      incident->getLabel()->setNewPos(incident->scenePos());
		    }
		  else 
		    {
		      incident->setPos(incident->getOriginalPos().x(), incident->scenePos().y());
		      incident->getLabel()->setNewPos(incident->scenePos());
		    }
		}
	      else 
		{
		  if (!incident->isCopy()) 
		    {
		      _resizeOnIncidentNode = true;
		      _lastMousePos = event->scenePos();
		      _selectedIncidentNodePtr = incident;
		      _selectedAbstractNodePtr = NULL;
		    }
		}
	    }
	  else if (abstractNode) 
	    {
	      emit resetItemSelection();
	      if (event->modifiers() & Qt::AltModifier) 
		{
		  if (abstractNode->isCopy()) 
		    {
		      abstractNode->setPos(abstractNode->getOriginalPos());
		      abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
		    }
		  else 
		    {
		      abstractNode->setPos(abstractNode->getOriginalPos().x(), abstractNode->scenePos().y());
		      abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
		    }
		}
	      else 
		{
		  if (!abstractNode->isCopy()) 
		    {
		      _resizeOnAbstractNode = true;
		      _lastMousePos = event->scenePos();
		      _selectedAbstractNodePtr = abstractNode;
		      _selectedIncidentNodePtr = NULL;
		    }
		}
	    }
	  else if (line) 
	    {
	      clearSelection();
	      line->setSelected(true);
	      _selectedLinePtr = line;
	      emit resetItemSelection();
	      _lineMoveOn = true;
	    }
	  else if (ellipse) 
	    {
	      clearSelection();
	      _selectedEllipsePtr = ellipse;
	      ellipse->setSelected(true);
	      emit resetItemSelection();
	      _manipulateEllipse = true;
	    }
	  else if (rect) 
	    {
	      clearSelection();
	      rect->setSelected(true);
	      emit resetItemSelection();
	      _selectedRectPtr = rect;
	      _manipulateRect = true;
	    }
	}
      else 
	{
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(event->scenePos(),
								      QTransform()));
	  IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(event->scenePos(),
								       QTransform()));
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(event->scenePos(),
								     QTransform()));
	  AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(event->scenePos(),
									  QTransform()));
	  NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->scenePos(),
									     QTransform()));
	  NetworkNodeLabel *networkLabel = qgraphicsitem_cast<NetworkNodeLabel*>(itemAt(event->scenePos(),
											QTransform()));
	  OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->scenePos(),
										  QTransform()));
	  OccurrenceLabel *occurrenceLabel = qgraphicsitem_cast<OccurrenceLabel*>(itemAt(event->scenePos(),
											 QTransform()));
	  LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->scenePos(),
								    QTransform()));
	  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->scenePos(),
									     QTransform()));
	  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->scenePos(),
								    QTransform()));
	  TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->scenePos(),
								    QTransform()));
	  if (incidentNodeLabel) 
	    {
	      incident = incidentNodeLabel->getNode();
	    }
	  if (abstractNodeLabel) 
	    {
	      abstractNode = abstractNodeLabel->getAbstractNode();
	    }
	  if (occurrenceLabel) 
	    {
	      occurrence = occurrenceLabel->getOccurrence();
	    }
	  if (incident) 
	    {
	      clearSelection();
	      incident->setSelected(true);
	      emit resetItemSelection();
	    }
	  else if (abstractNode) 
	    {
	      clearSelection();
	      abstractNode->setSelected(true);
	      emit resetItemSelection();
	    }
	  else if (networkNode) 
	    {
	      clearSelection();
	      networkNode->setSelected(true);
	    }
	  else if (networkLabel)
	    {
	      clearSelection();
	      _moveNetworkNodeLabel = true;
	      _selectedNetworkNodeLabelPtr = networkLabel;
	      _lastMousePos = event->scenePos();
	    }
	  else if (occurrence) 
	    {
	      clearSelection();
	      occurrence->setSelected(true);
	      _selectedOccurrencePtr = occurrence;
	    }
	  else if (line) 
	    {
	      clearSelection();
	      _selectedLinePtr = line;
	      line->setSelected(true);
	      emit resetItemSelection();
	      _moveLineObject = true;
	      _lastMousePos = event->scenePos();
	      QApplication::setOverrideCursor(Qt::SizeAllCursor);
	      qApp->processEvents();
	    }
	  else if (ellipse) 
	    {
	      clearSelection();
	      _selectedEllipsePtr = ellipse;
	      ellipse->setSelected(true);
	      emit resetItemSelection();
	      _moveEllipse = true;
	      _lastMousePos = event->scenePos();
	      QApplication::setOverrideCursor(Qt::SizeAllCursor);
	      qApp->processEvents();
	    }
	  else if (rect) 
	    {
	      clearSelection();
	      _selectedRectPtr = rect;
	      rect->setSelected(true);
	      emit resetItemSelection();
	      _moveRect = true;
	      _lastMousePos = event->scenePos();
	      QApplication::setOverrideCursor(Qt::SizeAllCursor);
	      qApp->processEvents();
	    }
	  else if (text) 
	    {
	      clearSelection();
	      text->setSelected(true);
	      _selectedTextPtr = text;
	      _lastMousePos = event->scenePos();
	      _moveText = true;
	    }
	  return;
	  _selectedIncidentNodePtr = NULL;
	  _selectedAbstractNodePtr = NULL;
	  _selectedNetworkNodePtr = NULL;
	  emit resetItemSelection();
	  QGraphicsScene::mousePressEvent(event);
	}
    }
  else
    {
      Scene::contextMenuEvent((QGraphicsSceneContextMenuEvent*) event);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) 
{
  _resizeOnIncidentNode = false;
  _resizeOnAbstractNode = false;
  _moveOn = false;
  _lineMoveOn = false;
  _moveLineObject = false;
  _manipulateEllipse = false;
  _moveEllipse = false;
  _rotateEllipse = false;
  _manipulateRect = false;
  _moveRect = false;
  _rotateRect = false;
  _rotateText = false;
  _hierarchyMove = false;
  _moveText = false;
  _moveNetworkNodeLabel = false;
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
  QListIterator<QGraphicsItem*> it(this->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *current = it.next();
      IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(current);
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(current);
      OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(current);
      if (incident) 
	{
	  incident->setCursor(Qt::OpenHandCursor);
	}
      else if (abstractNode) 
	{
	  abstractNode->setCursor(Qt::OpenHandCursor);
	}
      else if (networkNode) 
	{
	  networkNode->setCursor(Qt::OpenHandCursor);
	}
      else if (occurrence) 
	{
	  occurrence->setCursor(Qt::OpenHandCursor);
	}
    }
  _selectedIncidentNodePtr = NULL;
  _selectedAbstractNodePtr = NULL;
  _selectedNetworkNodePtr = NULL;
  _selectedOccurrencePtr = NULL;
  _selectedLinePtr = NULL;
  _selectedEllipsePtr = NULL;
  _selectedTextPtr = NULL;
  _selectedNetworkNodeLabelPtr = NULL;
  QGraphicsScene::mouseReleaseEvent(event);
}
  
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) 
{
  if (_eventWidthChange) 
    {
      if (_selectedIncidentNodePtr != NULL) 
	{
	  QPointF eventPos = _selectedIncidentNodePtr->scenePos();
	  if (event->scenePos().x() > eventPos.x() && _selectedIncidentNodePtr->getWidth() >= 40) 
	    {
	      int newWidth = event->scenePos().x() - _selectedIncidentNodePtr->scenePos().x();
	      if (newWidth >= 40) 
		{
		  _selectedIncidentNodePtr->setWidth(newWidth);
		  _selectedIncidentNodePtr->getLabel()->setOffset(QPointF(newWidth / 2 - 20, 0));
		  _selectedIncidentNodePtr->getLabel()->setNewPos(_selectedIncidentNodePtr->scenePos());
		}
	    }
	  else 
	    {
	      _selectedIncidentNodePtr->setWidth(40);
	      _selectedIncidentNodePtr->getLabel()->setOffset(QPointF(40 / 2 - 20, 0));
	      _selectedIncidentNodePtr->getLabel()->setNewPos(_selectedIncidentNodePtr->scenePos());
	    }
	}
      else if (_selectedAbstractNodePtr != NULL) 
	{
	  QPointF abstractNodePos = _selectedAbstractNodePtr->scenePos();
	  if (event->scenePos().x() > abstractNodePos.x() && _selectedAbstractNodePtr->getWidth() >= 40) 
	    {
	      int newWidth = event->scenePos().x() - _selectedAbstractNodePtr->scenePos().x();
	      if (newWidth >= 40) 
		{
		  _selectedAbstractNodePtr->setWidth(newWidth);
		  _selectedAbstractNodePtr->getLabel()->setOffset(QPointF(newWidth / 2 - 20, 0));
		  _selectedAbstractNodePtr->getLabel()->setNewPos(_selectedAbstractNodePtr->scenePos());
		}
	    }
	  else 
	    {
	      _selectedAbstractNodePtr->setWidth(40);
	      _selectedAbstractNodePtr->getLabel()->setOffset(QPointF(40 / 2 - 20, 0));
	      _selectedAbstractNodePtr->getLabel()->setNewPos(_selectedAbstractNodePtr->scenePos());
	    }
	}
    }
  else if (_resizeOnIncidentNode) 
    {
      qreal dist = event->scenePos().x() - _lastMousePos.x();
      int currentY = _selectedIncidentNodePtr->scenePos().y();
      _selectedIncidentNodePtr->setPos(event->scenePos().x(), currentY);
      emit posChanged(_selectedIncidentNodePtr, dist);
      _selectedIncidentNodePtr->setDislodged(true);
      _lastMousePos = event->scenePos();
      emit relevantChange();
    }
  else if (_resizeOnAbstractNode) 
    {
      qreal dist = event->scenePos().x() - _lastMousePos.x();
      int currentY = _selectedAbstractNodePtr->scenePos().y();
      _selectedAbstractNodePtr->setPos(event->scenePos().x(), currentY);
      emit posChanged(_selectedAbstractNodePtr, dist);
      _selectedAbstractNodePtr->setDislodged(true);
      _lastMousePos = event->scenePos();
      emit relevantChange();
    }
  else if (_hierarchyMove) 
    {
      if (_selectedIncidentNodePtr) 
	{
	  emit moveItems(_selectedIncidentNodePtr, event->scenePos());
	}
      else if (_selectedAbstractNodePtr) 
	{
	  emit moveItems(_selectedAbstractNodePtr, event->scenePos());
	}
      emit relevantChange();
    }
  else if (_lineMoveOn) 
    {
      _lastMousePos = event->scenePos();
      QPointF start = _selectedLinePtr->getStartPos();
      QPointF end = _selectedLinePtr->getEndPos();
      qreal distStart = sqrt(pow((_lastMousePos.x() - start.x()), 2) +
			     pow((_lastMousePos.y() - start.y()), 2));
      qreal distEnd = sqrt(pow((_lastMousePos.x() - end.x()), 2) +
			   pow((_lastMousePos.y() - end.y()), 2));
      if (distStart < distEnd) 
	{
	  if (event->modifiers() & Qt::ControlModifier) 
	    {
	      if (abs((_lastMousePos.y() - end.y()) / (_lastMousePos.x() - end.x())) < 1) 
		{
		  _selectedLinePtr->setStartPos(_lastMousePos.x(), end.y());
		}
	      else 
		{
		  _selectedLinePtr->setStartPos(end.x(), _lastMousePos.y());
		}
	    }
	  else 
	    {
	      _selectedLinePtr->setStartPos(_lastMousePos);
	    }
	}
      else 
	{
	  if (event->modifiers() & Qt::ControlModifier) 
	    {
	      if (abs((_lastMousePos.y() - start.y()) / (_lastMousePos.x() - start.x())) < 1) 
		{
		  _selectedLinePtr->setEndPos(_lastMousePos.x(), start.y());
		}
	      else 
		{
		  _selectedLinePtr->setEndPos(start.x(), _lastMousePos.y());
		}
	    }
	  else 
	    {
	      _selectedLinePtr->setEndPos(_lastMousePos);
	    }
	}
      emit relevantChange();
    }
  else if (_moveLineObject) 
    {
      QPointF newPos = _selectedLinePtr->mapFromScene(event->scenePos());
      qreal newXDiff = newPos.x() - _lastMousePos.x();
      qreal newYDiff = newPos.y() - _lastMousePos.y();
      _selectedLinePtr->setStartPos(_selectedLinePtr->mapToScene(_selectedLinePtr->getStartPos() +
							 QPointF(newXDiff, newYDiff)));
      _selectedLinePtr->setEndPos(_selectedLinePtr->mapToScene(_selectedLinePtr->getEndPos() +
						       QPointF(newXDiff, newYDiff)));
      _lastMousePos = event->scenePos();
      emit relevantChange();
    }
  else if (_manipulateEllipse) 
    {
      _lastMousePos = event->scenePos();
      QPointF topLeft = _selectedEllipsePtr->mapToScene(_selectedEllipsePtr->topLeft());
      QPointF bottomLeft = _selectedEllipsePtr->mapToScene(_selectedEllipsePtr->bottomLeft());
      QPointF topRight = _selectedEllipsePtr->mapToScene(_selectedEllipsePtr->topRight());
      QPointF bottomRight = _selectedEllipsePtr->mapToScene(_selectedEllipsePtr->bottomRight());
      QPointF left = _selectedEllipsePtr->mapToScene(QPointF(_selectedEllipsePtr->getLeft(),
							 _selectedEllipsePtr->getCenter().y()));
      QPointF right = _selectedEllipsePtr->mapToScene(QPointF(_selectedEllipsePtr->getRight(),
							  _selectedEllipsePtr->getCenter().y()));
      QPointF top = _selectedEllipsePtr->mapToScene(QPointF(_selectedEllipsePtr->getCenter().x(),
							_selectedEllipsePtr->getTop()));
      QPointF bottom = _selectedEllipsePtr->mapToScene(QPointF(_selectedEllipsePtr->getCenter().x(),
							   _selectedEllipsePtr->getBottom()));
      qreal distTopLeft = sqrt(pow((_lastMousePos.x() - topLeft.x()), 2) +
			       pow((_lastMousePos.y() - topLeft.y()), 2));
      qreal distTopRight = sqrt(pow((_lastMousePos.x() - topRight.x()), 2) +
				pow((_lastMousePos.y() - topRight.y()), 2));
      qreal distBottomLeft = sqrt(pow((_lastMousePos.x() - bottomLeft.x()), 2) +
				  pow((_lastMousePos.y() - bottomLeft.y()), 2));
      qreal distBottomRight = sqrt(pow((_lastMousePos.x() - bottomRight.x()), 2) +
				   pow((_lastMousePos.y() - bottomRight.y()), 2));
      qreal distLeft = sqrt(pow((_lastMousePos.x() - left.x()), 2) +
			    pow((_lastMousePos.y() - left.y()), 2));
      qreal distRight = sqrt(pow((_lastMousePos.x() - right.x()), 2) +
			     pow((_lastMousePos.y() - right.y()), 2));
      qreal distTop = sqrt(pow((_lastMousePos.x() - top.x()), 2) +
			   pow((_lastMousePos.y() - top.y()), 2));
      qreal distBottom = sqrt(pow((_lastMousePos.x() - bottom.x()), 2) +
			      pow((_lastMousePos.y() - bottom.y()), 2));
      QVector<qreal> temp;
      temp.push_back(distTopLeft);
      temp.push_back(distTopRight);
      temp.push_back(distBottomLeft);
      temp.push_back(distBottomRight);
      temp.push_back(distLeft);
      temp.push_back(distRight);
      temp.push_back(distTop);
      temp.push_back(distBottom);
      qreal minimum = -1.0;
      QVectorIterator<qreal> it(temp);
      while (it.hasNext()) 
	{
	  qreal current = it.next();
	  if (minimum == -1 || current < minimum) 
	    {
	      minimum = current;
	    }
	}
      if (minimum == distTopLeft) 
	{
	  _selectedEllipsePtr->setTopLeft(_selectedEllipsePtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distTopRight) 
	{
	  _selectedEllipsePtr->setTopRight(_selectedEllipsePtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distBottomLeft) 
	{
	  _selectedEllipsePtr->setBottomLeft(_selectedEllipsePtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distBottomRight) 
	{
	  _selectedEllipsePtr->setBottomRight(_selectedEllipsePtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distLeft) 
	{
	  _selectedEllipsePtr->setLeft(_selectedEllipsePtr->mapFromScene(_lastMousePos).x());
	}
      else if (minimum == distRight) 
	{
	  _selectedEllipsePtr->setRight(_selectedEllipsePtr->mapFromScene(_lastMousePos).x());
	}
      else if (minimum == distTop) 
	{
	  _selectedEllipsePtr->setTop(_selectedEllipsePtr->mapFromScene(_lastMousePos).y());
	}
      else if (minimum == distBottom) 
	{
	  _selectedEllipsePtr->setBottom(_selectedEllipsePtr->mapFromScene(_lastMousePos).y());
	}
      emit relevantChange();
    }
  else if (_moveEllipse) 
    {
      _selectedEllipsePtr->resetTransform();
      QPointF newPos = event->scenePos();
      qreal newXDiff = newPos.x() - _lastMousePos.x();
      qreal newYDiff = newPos.y() - _lastMousePos.y();
      _selectedEllipsePtr->moveCenter(_selectedEllipsePtr->mapToScene(_selectedEllipsePtr->getCenter()) +
				  _selectedEllipsePtr->mapFromScene(QPointF(newXDiff, newYDiff)));
      _lastMousePos = event->scenePos();
      _selectedEllipsePtr->setRotationValue(_selectedEllipsePtr->getRotationValue());
      emit relevantChange();
    }
  else if (_rotateEllipse) 
    {
      _lastMousePos = event->scenePos();
      QPointF center = _selectedEllipsePtr->mapToScene(_selectedEllipsePtr->getCenter());
      qreal dY = _lastMousePos.y() - center.y();
      qreal dX = _lastMousePos.x() - center.x();
      qreal angle = atan2(dY, dX);
      angle = qRadiansToDegrees(angle);
      _selectedEllipsePtr->setRotationValue(angle);
      emit relevantChange();
    }
  else if (_manipulateRect) 
    {
      _lastMousePos = event->scenePos();
      QPointF topLeft = _selectedRectPtr->mapToScene(_selectedRectPtr->topLeft());
      QPointF bottomLeft = _selectedRectPtr->mapToScene(_selectedRectPtr->bottomLeft());
      QPointF topRight = _selectedRectPtr->mapToScene(_selectedRectPtr->topRight());
      QPointF bottomRight = _selectedRectPtr->mapToScene(_selectedRectPtr->bottomRight());
      QPointF left = _selectedRectPtr->mapToScene(QPointF(_selectedRectPtr->getLeft(),
						      _selectedRectPtr->getCenter().y()));
      QPointF right = _selectedRectPtr->mapToScene(QPointF(_selectedRectPtr->getRight(),
						       _selectedRectPtr->getCenter().y()));
      QPointF top = _selectedRectPtr->mapToScene(QPointF(_selectedRectPtr->getCenter().x(),
						     _selectedRectPtr->getTop()));
      QPointF bottom = _selectedRectPtr->mapToScene(QPointF(_selectedRectPtr->getCenter().x(),
							_selectedRectPtr->getBottom()));
      qreal distTopLeft = sqrt(pow((_lastMousePos.x() - topLeft.x()), 2) +
			       pow((_lastMousePos.y() - topLeft.y()), 2));
      qreal distTopRight = sqrt(pow((_lastMousePos.x() - topRight.x()), 2) +
				pow((_lastMousePos.y() - topRight.y()), 2));
      qreal distBottomLeft = sqrt(pow((_lastMousePos.x() - bottomLeft.x()), 2) +
				  pow((_lastMousePos.y() - bottomLeft.y()), 2));
      qreal distBottomRight = sqrt(pow((_lastMousePos.x() - bottomRight.x()), 2) +
				   pow((_lastMousePos.y() - bottomRight.y()), 2));
      qreal distLeft = sqrt(pow((_lastMousePos.x() - left.x()), 2) +
			    pow((_lastMousePos.y() - left.y()), 2));
      qreal distRight = sqrt(pow((_lastMousePos.x() - right.x()), 2) +
			     pow((_lastMousePos.y() - right.y()), 2));
      qreal distTop = sqrt(pow((_lastMousePos.x() - top.x()), 2) +
			   pow((_lastMousePos.y() - top.y()), 2));
      qreal distBottom = sqrt(pow((_lastMousePos.x() - bottom.x()), 2) +
			      pow((_lastMousePos.y() - bottom.y()), 2));
      QVector<qreal> temp;
      temp.push_back(distTopLeft);
      temp.push_back(distTopRight);
      temp.push_back(distBottomLeft);
      temp.push_back(distBottomRight);
      temp.push_back(distLeft);
      temp.push_back(distRight);
      temp.push_back(distTop);
      temp.push_back(distBottom);
      qreal minimum = -1.0;
      QVectorIterator<qreal> it(temp);
      while (it.hasNext()) 
	{
	  qreal current = it.next();
	  if (minimum == -1 || current < minimum) 
	    {
	      minimum = current;
	    }
	}
      if (minimum == distTopLeft) 
	{
	  _selectedRectPtr->setTopLeft(_selectedRectPtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distTopRight) 
	{
	  _selectedRectPtr->setTopRight(_selectedRectPtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distBottomLeft) 
	{
	  _selectedRectPtr->setBottomLeft(_selectedRectPtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distBottomRight) 
	{
	  _selectedRectPtr->setBottomRight(_selectedRectPtr->mapFromScene(_lastMousePos));
	}
      else if (minimum == distLeft) 
	{
	  _selectedRectPtr->setLeft(_selectedRectPtr->mapFromScene(_lastMousePos).x());
	}
      else if (minimum == distRight) 
	{
	  _selectedRectPtr->setRight(_selectedRectPtr->mapFromScene(_lastMousePos).x());
	}
      else if (minimum == distTop) 
	{
	  _selectedRectPtr->setTop(_selectedRectPtr->mapFromScene(_lastMousePos).y());
	}
      else if (minimum == distBottom) 
	{
	  _selectedRectPtr->setBottom(_selectedRectPtr->mapFromScene(_lastMousePos).y());
	}
      emit relevantChange();
    }
  else if (_moveRect) 
    {
      _selectedRectPtr->resetTransform();
      QPointF newPos = event->scenePos();
      qreal newXDiff = newPos.x() - _lastMousePos.x();
      qreal newYDiff = newPos.y() - _lastMousePos.y();
      _selectedRectPtr->moveCenter(_selectedRectPtr->mapToScene(_selectedRectPtr->getCenter()) +
			       _selectedRectPtr->mapFromScene(QPointF(newXDiff, newYDiff)));
      _lastMousePos = event->scenePos();
      _selectedRectPtr->setRotationValue(_selectedRectPtr->getRotationValue());
      emit relevantChange();
    }
  else if (_rotateRect) 
    {
      _lastMousePos = event->scenePos();
      QPointF center = _selectedRectPtr->mapToScene(_selectedRectPtr->getCenter());
      qreal dY = _lastMousePos.y() - center.y();
      qreal dX = _lastMousePos.x() - center.x();
      qreal angle = atan2(dY, dX);
      angle = qRadiansToDegrees(angle);
      _selectedRectPtr->setRotationValue(angle);
      emit relevantChange();
    }
  else if (_moveText) 
    {
      _selectedTextPtr->resetTransform();
      QPointF newPos = event->scenePos();
      qreal newXDiff = newPos.x() - _lastMousePos.x();
      qreal newYDiff = newPos.y() - _lastMousePos.y();
      _selectedTextPtr->setPos(_selectedTextPtr->scenePos() + QPointF(newXDiff, newYDiff));
      _lastMousePos = event->scenePos();
      _selectedTextPtr->setRotationValue(_selectedTextPtr->getRotationValue());
      emit relevantChange();
    }
  else if (_rotateText) 
    {
      _lastMousePos = event->scenePos();
      QPointF center = _selectedTextPtr->mapToScene(_selectedTextPtr->getCenter());
      qreal dY = center.y() - _lastMousePos.y();
      qreal dX = center.x() - _lastMousePos.x();
      qreal angle = atan2(dY, dX);
      angle = qRadiansToDegrees(angle);
      _selectedTextPtr->setRotationValue(angle);
      emit relevantChange();
    }
  else if (_moveNetworkNodeLabel) 
    {
      QPointF newPos = event->scenePos();
      qreal newXDiff = newPos.x() - _lastMousePos.x();
      qreal newYDiff = newPos.y() - _lastMousePos.y();
      _selectedNetworkNodeLabelPtr->setPos(_selectedNetworkNodeLabelPtr->scenePos() + QPointF(newXDiff, newYDiff));
      qreal xDist = _selectedNetworkNodeLabelPtr->scenePos().x() -
	_selectedNetworkNodeLabelPtr->getNode()->scenePos().x();
      qreal yDist = _selectedNetworkNodeLabelPtr->scenePos().y() -
	_selectedNetworkNodeLabelPtr->getNode()->scenePos().y();
      _selectedNetworkNodeLabelPtr->setOffset(QPointF(xDist, yDist));
      _lastMousePos = event->scenePos();
      emit relevantChange();
    }
  else 
    {
      if (selectedItems().size() > 1 && _moveOn) 
	{
	  if (_selectedIncidentNodePtr) 
	    {
	      emit moveItems(_selectedIncidentNodePtr, event->scenePos());
	      emit relevantChange();
	    }
	  else if (_selectedAbstractNodePtr) 
	    {
	      emit moveItems(_selectedAbstractNodePtr, event->scenePos());
	      emit relevantChange();
	    }
	  else if (_selectedNetworkNodePtr) 
	    {
	      emit moveItems(_selectedNetworkNodePtr, event->scenePos());
	      emit relevantChange();
	    }
	  else 
	    {
	      _moveOn = false;
	    }
	}
      else 
	{
	  if (_selectedOccurrencePtr && _moveOn) 
	    {
	      emit moveLine(_selectedOccurrencePtr, event->scenePos());
	      emit relevantChange();
	    }
	  else if (_selectedOccurrencePtr && !_moveOn) 
	    {
	      emit moveItems(_selectedOccurrencePtr, event->scenePos());
	      emit relevantChange();
	    }
	}
      GraphicsView *view = qobject_cast<GraphicsView*>(views()[0]);
      BandlessGraphicsView *bandless = qobject_cast<BandlessGraphicsView*>(views()[0]);
      if (view) 
	{
	  if (!view->isPanning()) 
	    {
	      QApplication::restoreOverrideCursor();
	    }
	}
      else if (bandless) 
	{
	  if (!bandless->isPanning()) 
	    {
	      QApplication::restoreOverrideCursor();
	    }
	}
      QGraphicsScene::mouseMoveEvent(event);
    }
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) 
{
  if (!(event->modifiers() & Qt::ControlModifier)) 
    {
      IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(event->scenePos(), QTransform()));
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(itemAt(event->scenePos(), QTransform()));
      IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(event->scenePos(), QTransform()));
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(event->scenePos(), QTransform()));
      AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(event->scenePos(), QTransform()));
      NetworkNode *node = qgraphicsitem_cast<NetworkNode*>(itemAt(event->scenePos(), QTransform()));
      LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->scenePos(), QTransform()));
      TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->scenePos(), QTransform()));
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->scenePos(),
									 QTransform()));
      RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->scenePos(), QTransform()));
      if (incidentNodeLabel) 
	{
	  incident = incidentNodeLabel->getNode();
	}
      if (abstractNodeLabel) 
	{
	  abstractNode = abstractNodeLabel->getAbstractNode();
	}
      if (incident && !incident->isCopy()) 
	{
	  QMenu menu;
	  QMenu colligationMenu("Abstraction");
	  QMenu colorMenu("Colouring");
	  QMenu posMenu("Positioning");
	  QMenu selectionMenu("Selection");
	  QMenu linkageMenu("Linkages");
	  QMenu descMenu("Description");
	  menu.addMenu(&colligationMenu);
	  menu.addMenu(&colorMenu);
	  menu.addMenu(&posMenu);
	  menu.addMenu(&selectionMenu);
	  menu.addMenu(&linkageMenu);
	  menu.addMenu(&descMenu);
	  QAction *action1 = new QAction(COLLIGATEACTION, this);
	  colligationMenu.addAction(action1);
	  QAction *action2 = new QAction(MAKEABSTRACTNODEACTION, this);
	  colligationMenu.addAction(action2);
	  QAction *action3= new QAction(RECOLOREVENTSACTION, this);
	  colorMenu.addAction(action3);
	  QAction *action4 = new QAction(RECOLORLABELSACTION, this);
	  colorMenu.addAction(action4);
	  QAction *action5 = new QAction(COLORLINEAGEACTION, this);
	  colorMenu.addAction(action5);
	  QAction *action6 = new QAction(SETWIDTHACTION, this);
	  posMenu.addAction(action6);
	  QAction *action7 = new QAction(ORIGINALPOSACTION, this);
	  posMenu.addAction(action7);
	  QAction *action8= new QAction(SETTLEACTION, this);
	  posMenu.addAction(action8);
	  QAction *action9 = new QAction(PARALLELACTION, this);
	  posMenu.addAction(action9);
	  QAction *action10 = new QAction(NORMALIZEACTION, this);
	  posMenu.addAction(action10);
	  QAction *action11 = new QAction(CLOSEGAPACTION, this);
	  posMenu.addAction(action11);
	  QAction *action12 = new QAction(ADDLINKAGEACTION, this);
	  linkageMenu.addAction(action12);
	  QAction *action13 = new QAction(SELECTFOLLOWERSACTION, this);
	  selectionMenu.addAction(action13);
	  QAction *action14 = new QAction(SELECTPREDECESSORSACTION, this);
	  selectionMenu.addAction(action14);
	  QAction *action15 = new QAction(COPYDESCRIPTIONTOTEXTACTION, this);
	  descMenu.addAction(action15);
	  if (selectedItems().size() > 1) 
	    {
	      action2->setEnabled(false);
	      action5->setEnabled(false);
	      action6->setEnabled(false);
	      action8->setEnabled(false);
	      action10->setEnabled(false);
	      action11->setEnabled(false);
	      action13->setEnabled(false);
	      action14->setEnabled(false);
	      action15->setEnabled(false);
	    }
	  if (selectedItems().size() == 1) 
	    {
	      action1->setEnabled(false);
	      action9->setEnabled(false);
	    }
	  action12->setEnabled(false);
	  if (selectedItems().size() == 2) 
	    {
	      IncidentNode *incidentNodeOne = qgraphicsitem_cast<IncidentNode*>(selectedItems()[0]);
	      IncidentNode *incidentNodeTwo = qgraphicsitem_cast<IncidentNode*>(selectedItems()[1]);
	      if (incidentNodeOne && incidentNodeTwo) 
		{
		  action12->setEnabled(true);
		}
	    }
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit IncidentNodeContextMenuAction(action->text());
	    }
	  // And then we'll capture some action, and send a signal to the main widget.
	}
      else if (abstractNode && !abstractNode->isCopy()) 
	{
	  QMenu menu;
	  QMenu colligationMenu("Abstraction");
	  QMenu colorMenu("Colouring");
	  QMenu posMenu("Positioning");
	  QMenu selectionMenu("Selection");
	  QMenu descMenu("Description");
	  menu.addMenu(&colligationMenu);
	  menu.addMenu(&colorMenu);
	  menu.addMenu(&posMenu);
	  menu.addMenu(&selectionMenu);
	  menu.addMenu(&descMenu);
	  QAction *action1 = new QAction(COLLIGATEACTION, this);
	  colligationMenu.addAction(action1);
	  QAction *action2 = new QAction(DISAGGREGATEACTION, this);
	  colligationMenu.addAction(action2);
	  QAction *action3= new QAction(RECOLOREVENTSACTION, this);
	  colorMenu.addAction(action3);
	  QAction *action4 = new QAction(RECOLORLABELSACTION, this);
	  colorMenu.addAction(action4);
	  QAction *action5 = new QAction(COLORLINEAGEACTION, this);
	  colorMenu.addAction(action5);
	  QAction *action6 = new QAction(SETWIDTHACTION, this);
	  posMenu.addAction(action6);
	  QAction *action7= new QAction(SETTLEACTION, this);
	  posMenu.addAction(action7);
	  QAction *action8 = new QAction(PARALLELACTION, this);
	  posMenu.addAction(action8);
	  QAction *action9 = new QAction(NORMALIZEACTION, this);
	  posMenu.addAction(action9);
	  QAction *action10 = new QAction(CLOSEGAPACTION, this);
	  posMenu.addAction(action10);
	  QAction *action11 = new QAction(CHANGEDESCRIPTIONACTION, this);
	  descMenu.addAction(action11);
	  QAction *action12 = new QAction(SELECTFOLLOWERSACTION, this);
	  selectionMenu.addAction(action12);
	  QAction *action13 = new QAction(SELECTPREDECESSORSACTION, this);
	  selectionMenu.addAction(action13);
	  QAction *action14 = new QAction(COPYDESCRIPTIONTOTEXTACTION, this);
	  descMenu.addAction(action14);
	  if (selectedItems().size() > 1) 
	    {
	      action2->setEnabled(false);
	      action5->setEnabled(false);
	      action6->setEnabled(false);
	      action7->setEnabled(false);
	      action9->setEnabled(false);
	      action10->setEnabled(false);
	      action11->setEnabled(false);
	      action12->setEnabled(false);
	      action13->setEnabled(false);
	      action14->setEnabled(false);
	    }
	  if (selectedItems().size() == 1) 
	    {
	      action1->setEnabled(false);
	      action8->setEnabled(false);
	    }
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit IncidentNodeContextMenuAction(action->text());
	    }
	}
      else if (linkage && !linkage->isCopy()) 
	{
	  clearSelection();
	  linkage->setSelected(true);
	  QMenu menu;
	  QAction *action1 = new QAction(REMOVELINKAGEACTION, this);
	  QAction *action2 = new QAction(KEEPLINKAGEACTION, this);
	  QAction *action3 = new QAction(ACCEPTLINKAGEACTION, this);
	  QAction *action4 = new QAction(REJECTLINKAGEACTION, this);
	  QAction *action5 = new QAction(IGNOREMEACTION, this);
	  QAction *action6 = new QAction(REMOVENORMALLINKAGEACTION, this);
	  QAction *action7 = new QAction(CHANGECOMMENTACTION, this);
	  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode*>(linkage->getStart());
	  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode*>(linkage->getEnd());
	  if (linkage->getPenStyle() == 3) 
	    {
	      menu.addAction(action1);
	      menu.addAction(action2);
	    }
	  else if (linkage->getPenStyle() == 4) 
	    {
	      menu.addAction(action3);
	      menu.addAction(action4);
	    }
	  else if (linkage->getPenStyle() == 2) 
	    {
	      menu.addAction(action1);
	      menu.addAction(action2);
	      menu.addAction(action5);
	    }
	  else if (linkage->getPenStyle() == 1 && startIncidentNode && endIncidentNode) 
	    {
	      menu.addAction(action6);
	      menu.addAction(action7);
	    }
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit LinkageContextMenuAction(action->text());
	    }
	}
      else if (node) 
	{
	  QMenu menu;
	  QMenu persistenceMenu("Persistence");
	  QMenu colorMenu("Colouring");
	  menu.addMenu(&persistenceMenu);
	  menu.addMenu(&colorMenu);
	  QAction *action1 = new QAction(SETPERSISTENTACTION, this);
	  persistenceMenu.addAction(action1);
	  QAction *action2 = new QAction(UNSETPERSISTENTACTION, this);
	  persistenceMenu.addAction(action2);
	  QAction *action3 = new QAction(RECOLORNODESACTION, this);
	  colorMenu.addAction(action3);
	  QAction *action4 = new QAction(RECOLORNODELABELSACTION, this);
	  colorMenu.addAction(action4);
	  if (selectedItems().size() == 1) 
	    {
	      if (node->isPersistent()) 
		{
		  action1->setEnabled(false);
		  action2->setEnabled(true);
		}
	      else 
		{
		  action1->setEnabled(true);
		  action2->setEnabled(false);
		}
	    }
	  else
	    {
	      action1->setEnabled(false);
	      action2->setEnabled(false);
	    }
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit NetworkNodeContextMenuAction(action->text());
	    }
	}
      else if (line) 
	{
	  clearSelection();
	  line->setSelected(true);
	  QMenu menu;
	  QMenu editMenu("Edit");
	  QMenu positionMenu("Position");
	  menu.addMenu(&editMenu);
	  menu.addMenu(&positionMenu);
	  QAction *action1 = new QAction(CHANGELINECOLOR, this);
	  editMenu.addAction(action1);
	  QAction *action2 = new QAction(TOGGLEARROW1, this);
	  editMenu.addAction(action2);
	  QAction *action3 = new QAction(TOGGLEARROW2, this);
	  editMenu.addAction(action3);
	  QAction *action4 = new QAction(COPYOBJECT, this);
	  menu.addAction(action4);
	  QAction *action5 = new QAction(DELETELINE, this);
	  menu.addAction(action5);
	  QAction *action6 = new QAction(ONEFORWARD, this);
	  positionMenu.addAction(action6);
	  QAction *action7 = new QAction(ONEBACKWARD, this);
	  positionMenu.addAction(action7);
	  QAction *action8 = new QAction(BRINGFORWARD, this);
	  positionMenu.addAction(action8);
	  QAction *action9 = new QAction(BRINGBACKWARD, this);
	  positionMenu.addAction(action9);
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit LineContextMenuAction(action->text());
	    }
	}
      else if (text) 
	{
	  clearSelection();
	  text->setSelected(true);
	  QMenu menu;
	  QMenu editMenu("Edit");
	  QMenu positionMenu("Position");
	  menu.addMenu(&editMenu);
	  menu.addMenu(&positionMenu);
	  QAction *action1 = new QAction(CHANGETEXT, this);
	  editMenu.addAction(action1);
	  QAction *action2 = new QAction(CHANGETEXTCOLOR, this);
	  editMenu.addAction(action2);
	  QAction *action3 = new QAction(COPYOBJECT, this);
	  menu.addAction(action3);
	  QAction *action4 = new QAction(DELETETEXT, this);
	  menu.addAction(action4);
	  QAction *action5 = new QAction(ONEFORWARD, this);
	  positionMenu.addAction(action5);
	  QAction *action6 = new QAction(ONEBACKWARD, this);
	  positionMenu.addAction(action6);
	  QAction *action7 = new QAction(BRINGFORWARD, this);
	  positionMenu.addAction(action7);
	  QAction *action8 = new QAction(BRINGBACKWARD, this);
	  positionMenu.addAction(action8);
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit TextContextMenuAction(action->text());
	    }
	}
      else if (ellipse) 
	{
	  clearSelection();
	  ellipse->setSelected(true);
	  QMenu menu;
	  QMenu editMenu("Edit");
	  QMenu positionMenu("Position");
	  menu.addMenu(&editMenu);
	  menu.addMenu(&positionMenu);
	  QAction *action1 = new QAction(CHANGEELLIPSECOLOR, this);
	  editMenu.addAction(action1);
	  QAction *action2 = new QAction(CHANGEELLIPSEFILLCOLOR, this);
	  editMenu.addAction(action2);
	  QAction *action3 = new QAction(COPYOBJECT, this);
	  menu.addAction(action3);
	  QAction *action4 = new QAction(DELETEELLIPSE, this);
	  menu.addAction(action4);
	  QAction *action5 = new QAction(ONEFORWARD, this);
	  positionMenu.addAction(action5);
	  QAction *action6 = new QAction(ONEBACKWARD, this);
	  positionMenu.addAction(action6);
	  QAction *action7 = new QAction(BRINGFORWARD, this);
	  positionMenu.addAction(action7);
	  QAction *action8 = new QAction(BRINGBACKWARD, this);
	  positionMenu.addAction(action8);
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit EllipseContextMenuAction(action->text());
	    }
	}
      else if (rect) 
	{
	  clearSelection();
	  rect->setSelected(true);
	  QMenu menu;
	  QMenu editMenu("Edit");
	  QMenu positionMenu("Position");
	  menu.addMenu(&editMenu);
	  menu.addMenu(&positionMenu);
	  QAction *action1 = new QAction(CHANGERECTCOLOR, this);
	  editMenu.addAction(action1);
	  QAction *action2 = new QAction(CHANGERECTFILLCOLOR, this);
	  editMenu.addAction(action2);
	  QAction *action3 = new QAction(COPYOBJECT, this);
	  menu.addAction(action3);
	  QAction *action4 = new QAction(DELETERECT, this);
	  menu.addAction(action4);
	  QAction *action5 = new QAction(ONEFORWARD, this);
	  positionMenu.addAction(action5);
	  QAction *action6 = new QAction(ONEBACKWARD, this);
	  positionMenu.addAction(action6);
	  QAction *action7 = new QAction(BRINGFORWARD, this);
	  positionMenu.addAction(action7);
	  QAction *action8 = new QAction(BRINGBACKWARD, this);
	  positionMenu.addAction(action8);
	  if (QAction *action = menu.exec(event->screenPos())) 
	    {
	      emit RectContextMenuAction(action->text());
	    }
	}
    }
}

OccurrenceItem* Scene::getSelectedOccurrence() 
{
  return _selectedOccurrencePtr;
}
