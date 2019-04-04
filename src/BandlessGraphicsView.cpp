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


#include "../include/BandlessGraphicsView.h"
#include "../include/Scene.h"
#include "../include/IncidentNodeLabel.h"
#include "../include/AbstractNodeLabel.h"
#include "../include/NetworkNode.h"
#include "../include/OccurrenceItem.h"
#include "../include/OccurrenceLabel.h"
#include "../include/HierarchyGraphWidget.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

BandlessGraphicsView::BandlessGraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) 
{
  _panSpeed = 4;
  _scaleFact = 1;
  _pan = false;
  _lastMousePos = QPoint(0, 0);
}

void BandlessGraphicsView::resizeEvent(QResizeEvent *) 
{
}

void BandlessGraphicsView::mousePressEvent(QMouseEvent *event) 
{
  if (event->modifiers() & Qt::ShiftModifier) 
    {
      QGraphicsView::mousePressEvent(event);
    }
  else if (event->button() == Qt::RightButton) 
    {
      Scene *scene = qobject_cast<Scene*>(this->scene());
      if (scene->isPreparingArea())
	{
	  scene->resetAreas();
	  return;
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
	  if (!incident && !abstractNode && !linkage && !networkNode && !occurrence && !occurrenceLabel &&
	      !line && !text && !ellipse && !rect) 
	    {
	      _pan = true;
	      QApplication::setOverrideCursor(Qt::ClosedHandCursor);
	      _lastMousePos = event->pos();
	      return;
	    }
	  else if (incident) 
	    {
	      incident->setSelected(true);
	      return;
	    }
	  else if (abstractNode) 
	    {
	      abstractNode->setSelected(true);
	    }
	  else if (linkage) 
	    {
	      linkage->setSelected(true);
	      return;
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
	}
    }
  else 
    {
      _pan = false;
      QGraphicsView::mousePressEvent(event);
    }
}

void BandlessGraphicsView::mouseReleaseEvent(QMouseEvent *event) 
{
  _pan = false;
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
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

void BandlessGraphicsView::mouseMoveEvent(QMouseEvent *event) 
{
  if (_pan) 
    {
      QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(_lastMousePos);
    
      mouseDelta *= _scaleFact;
      mouseDelta *= _panSpeed;
    
      setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
      QPoint newCenter(VIEW_WIDTH / 2 - mouseDelta.x(),  VIEW_HEIGHT / 2 - mouseDelta.y());
      centerOn(mapToScene(newCenter));
      _lastMousePos = event->pos();
      setTransformationAnchor(QGraphicsView::AnchorViewCenter);
      Scene *scene = qobject_cast<Scene*>(this->scene());
      QRectF currentRect = scene->itemsBoundingRect();
      currentRect.setX(currentRect.x() - 50);
      currentRect.setY(currentRect.y() - 50);
      currentRect.setWidth(currentRect.width() + 100);
      currentRect.setHeight(currentRect.height() + 100);
      this->scene()->setSceneRect(currentRect);
      update();
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

void BandlessGraphicsView::wheelEvent(QWheelEvent* event) 
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
	      this->horizontalScrollBar()->
		setValue(this->horizontalScrollBar()->value() - event->delta());
	    }
	  else 
	    {
	      this->horizontalScrollBar()->
		setValue(this->horizontalScrollBar()->value() - event->delta());
	    }
	}
    }
  else 
    {
      QGraphicsView::wheelEvent(event);
    }
}
   
bool BandlessGraphicsView::isPanning() 
{
  return _pan;
}
