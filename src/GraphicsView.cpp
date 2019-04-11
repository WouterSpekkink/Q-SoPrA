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

#include "../include/GraphicsView.h"
#include "../include/Scene.h"
#include "../include/IncidentNodeLabel.h"
#include "../include/AbstractNodeLabel.h"
#include "../include/NetworkNode.h"
#include "../include/OccurrenceItem.h"
#include "../include/OccurrenceLabel.h"
#include "../include/EventGraphWidget.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

GraphicsView::GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) 
{
  _panSpeed = 4.0f;
  _scaleFact = 1.0f;
  _pan = false;
}

void GraphicsView::resizeEvent(QResizeEvent *) 
{};

void GraphicsView::mousePressEvent(QMouseEvent *event) 
{
  if (event->modifiers() & Qt::ShiftModifier) 
    {
      this->setDragMode(QGraphicsView::NoDrag);
      QGraphicsView::mousePressEvent(event);
    }
  if (event->button() == Qt::RightButton)
    {
      Scene *scene = qobject_cast<Scene*>(this->scene());
      if (scene->isPreparingArea())
	{
	  scene->resetAreas();
	  return;
	}
      if (event->modifiers() & Qt::ControlModifier)
	{
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(event->pos()));
	  IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(event->pos()));
	  Linkage *linkage = qgraphicsitem_cast<Linkage*>(itemAt(event->pos()));
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(event->pos()));
	  AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(event->pos()));
	  NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->pos()));
	  OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->pos()));
	  OccurrenceLabel *occurrenceLabel = qgraphicsitem_cast<OccurrenceLabel*>(itemAt(event->pos()));
	  LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->pos()));
	  TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->pos()));
	  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->pos()));
	  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->pos()));
	  TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(itemAt(event->pos()));
	  GuideLine *guide = qgraphicsitem_cast<GuideLine*>(itemAt(event->pos()));
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
	      incident->setSelected(true);
	    }
	  else if (abstractNode) 
	    {
	      abstractNode->setSelected(true);
	    }
	  else if (linkage) 
	    {
	      linkage->setSelected(true);
	    }
	  else if (occurrence) 
	    {
	      occurrence->setSelected(true);
	    }
	  else if (networkNode) 
	    {
	      networkNode->setSelected(true);
	    }
	  else if (line) 
	    {
	      line->setSelected(true);
	    }
	  else if (text) 
	    {
	      text->setSelected(true);
	    }
	  else if (ellipse) 
	    {
	      ellipse->setSelected(true);
		}
	  else if (rect) 
	    {
	      rect->setSelected(true);
	    }
	  else if (timeline)
	    {
	      timeline->setSelected(true);
	    }
	  else if (guide)
	    {
	      guide->setSelected(true);
	    }
	  QGraphicsView::mousePressEvent(event);
	}
      else
	{
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(event->pos()));
	  IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(event->pos()));
	  Linkage *linkage = qgraphicsitem_cast<Linkage*>(itemAt(event->pos()));
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(event->pos()));
	  AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(event->pos()));
	  NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->pos()));
	  OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->pos()));
	  OccurrenceLabel *occurrenceLabel = qgraphicsitem_cast<OccurrenceLabel*>(itemAt(event->pos()));
	  LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->pos()));
	  TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->pos()));
	  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->pos()));
	  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->pos()));
	  TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(itemAt(event->pos()));
	  GuideLine *guide = qgraphicsitem_cast<GuideLine*>(itemAt(event->pos()));
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
	  if (!incident && !abstractNode && !linkage && !networkNode && !occurrence &&
	      !occurrenceLabel && !line && !text && !ellipse && !rect && !timeline && !guide) 
	    {
	      _pan = true;
	      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
	      _lastMousePos = event->pos();
	      return;
	    }
	  else
	    {
	      Scene *scene = qobject_cast<Scene*>(this->scene());
	      if (incident) 
		{
		  if (!incident->isSelected())
		    {
		      scene->clearSelection();
		      incident->setSelected(true);
		    }
		}
	      else if (abstractNode) 
		{
		  if (!abstractNode->isSelected())
		    {
		      scene->clearSelection();
		      abstractNode->setSelected(true);
		    }
		}
	      else if (linkage) 
		{
		  scene->clearSelection();
		  linkage->setSelected(true);
		}
	      else if (occurrence) 
		{
		  scene->clearSelection();
		  occurrence->setSelected(true);
		}
	      else if (networkNode) 
		{
		  if (!networkNode->isSelected())
		    {
		      scene->clearSelection();
		      networkNode->setSelected(true);
		    }
		}
	      else if (line) 
		{
		  scene->clearSelection();
		  line->setSelected(true);
		}
	      else if (text) 
		{
		  scene->clearSelection();
		  text->setSelected(true);
		}
	      else if (ellipse) 
		{
		  scene->clearSelection();
		  ellipse->setSelected(true);
		}
	      else if (rect) 
		{
		  scene->clearSelection();
		  rect->setSelected(true);
		}
	      else if (timeline)
		{
		  scene->clearSelection();
		  timeline->setSelected(true);
		}
	      else if(guide)
		{
		  scene->clearSelection();
		  guide->setSelected(true);
		}
	      scene->resetItemSelection();
	    }
	}
    }
  else if (event->button() == Qt::LeftButton)
    {
      Scene *scene = qobject_cast<Scene*>(this->scene());
      if (scene->isPreparingArea())
	{
	  this->setDragMode(QGraphicsView::NoDrag);
	}
      IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(itemAt(event->pos()));
      IncidentNodeLabel *incidentNodeLabel = qgraphicsitem_cast<IncidentNodeLabel*>(itemAt(event->pos()));
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(itemAt(event->pos()));
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(itemAt(event->pos()));
      AbstractNodeLabel *abstractNodeLabel = qgraphicsitem_cast<AbstractNodeLabel*>(itemAt(event->pos()));
      NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->pos()));
      OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->pos()));
      OccurrenceLabel *occurrenceLabel = qgraphicsitem_cast<OccurrenceLabel*>(itemAt(event->pos()));
      LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->pos()));
      TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->pos()));
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->pos()));
      RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->pos()));
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(itemAt(event->pos()));
      GuideLine *guide = qgraphicsitem_cast<GuideLine*>(itemAt(event->pos()));
      if (incident || incidentNodeLabel || linkage || abstractNode ||
	  abstractNodeLabel || networkNode || occurrence ||
	  occurrenceLabel || line || text || ellipse || rect || timeline || guide)
	{
	  this->setDragMode(QGraphicsView::NoDrag);
	}
      QGraphicsView::mousePressEvent(event);
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) 
{
  _pan = false;
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
  this->setDragMode(QGraphicsView::RubberBandDrag);
  if (event->button() == Qt::RightButton) 
    {
      _lastMousePos = event->pos();
      return;
    }
  else 
    {
      QGraphicsView::mouseReleaseEvent(event);
    }
}

void GraphicsView::centerMe() {
  QPoint newCenter(VIEW_WIDTH, VIEW_HEIGHT);
  centerOn(mapToScene(newCenter));
  setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  Scene *scene = qobject_cast<Scene*>(this->scene());
  QRectF currentRect = scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  this->scene()->setSceneRect(currentRect);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) 
{
  if (_pan) 
    {
      QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(_lastMousePos);
      mouseDelta *= _scaleFact;
      mouseDelta *= _panSpeed;
      setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
      QPoint newCenter(VIEW_WIDTH / 2 - mouseDelta.x(),  VIEW_HEIGHT / 2 - mouseDelta.y());
      centerOn(mapToScene(newCenter));
      setTransformationAnchor(QGraphicsView::AnchorViewCenter);
      Scene *scene = qobject_cast<Scene*>(this->scene());
      QRectF currentRect = scene->itemsBoundingRect();
      currentRect.setX(currentRect.x() - 50);
      currentRect.setY(currentRect.y() - 50);
      currentRect.setWidth(currentRect.width() + 100);
      currentRect.setHeight(currentRect.height() + 100);
      this->scene()->setSceneRect(currentRect);
      update();
      _lastMousePos = event->pos();
      return;
    }
  else 
    {
      Scene *scene = qobject_cast<Scene*>(this->scene());
      QRectF currentRect = scene->itemsBoundingRect();
      currentRect.setX(currentRect.x() - 50);
      currentRect.setY(currentRect.y() - 50);
      currentRect.setWidth(currentRect.width() + 100);
      currentRect.setHeight(currentRect.height() + 100);
      this->scene()->setSceneRect(currentRect);
      QGraphicsView::mouseMoveEvent(event);
    }
}

void GraphicsView::wheelEvent(QWheelEvent* event) 
{
  if (event->modifiers() & Qt::ControlModifier) 
    {
      event->ignore();
      QGraphicsSceneWheelEvent wheelEvent(QEvent::GraphicsSceneWheel);
      wheelEvent.setWidget(viewport());
      wheelEvent.setScenePos(mapToScene(event->pos()));
      wheelEvent.setScreenPos(event->globalPos());
      wheelEvent.setButtons(event->buttons());
      wheelEvent.setModifiers(event->modifiers());
      wheelEvent.setDelta(event->delta());
      wheelEvent.setOrientation(event->orientation());
      wheelEvent.setAccepted(false);
      qApp->sendEvent(this->scene(), &wheelEvent);
      event->setAccepted(wheelEvent.isAccepted());
      if (!(event->isAccepted())) 
	{
	  this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	  double scaleFactor = 1.15;
	  if (event->delta() > 0) 
	    {
	      this->scale(scaleFactor, scaleFactor);
	    }
	  else 
	    {
	      this->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
	    }
	}
    }
  else if (event->modifiers() & Qt::ShiftModifier) 
    {
      event->ignore();
      QGraphicsSceneWheelEvent wheelEvent(QEvent::GraphicsSceneWheel);
      wheelEvent.setWidget(viewport());
      wheelEvent.setScenePos(mapToScene(event->pos()));
      wheelEvent.setScreenPos(event->globalPos());
      wheelEvent.setButtons(event->buttons());
      wheelEvent.setModifiers(event->modifiers());
      wheelEvent.setDelta(event->delta());
      wheelEvent.setOrientation(event->orientation());
      wheelEvent.setAccepted(false);
      qApp->sendEvent(this->scene(), &wheelEvent);
      event->setAccepted(wheelEvent.isAccepted());
      if (!(event->isAccepted())) 
	{
	  if (event->delta() > 0) 
	    {
	      this->horizontalScrollBar()
		->setValue(this->horizontalScrollBar()->value() - event->delta());
	    }
	  else 
	    {
	      this->horizontalScrollBar()
		->setValue(this->horizontalScrollBar()->value() - event->delta());
	    }
	  emit changedView();
	}
    }
  else 
    {
      QGraphicsView::wheelEvent(event);
    }
}
   
bool GraphicsView::isPanning() 
{
  return _pan;
}
