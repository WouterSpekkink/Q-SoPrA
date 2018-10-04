#include "../include/GraphicsView.h"
#include "../include/Scene.h"
#include "../include/NodeLabel.h"
#include "../include/MacroLabel.h"
#include "../include/NetworkNode.h"
#include "../include/OccurrenceItem.h"
#include "../include/OccurrenceLabel.h"
#include "../include/EventGraphWidget.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

GraphicsView::GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) 
{
  panSpeed = 4;
  scaleFact = 1;
  pan = false;
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
  else if (event->modifiers() & Qt::ControlModifier) 
    {
      if (event->button() == Qt::RightButton) 
	{
	  /* 
	     We only want to add the context menu below for the event graph widget.
	     I could of course expand the conditionals here to also add context menu actions
	     to other widgets.
	  */
	  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(parent());
	  NetworkGraphWidget *ngw = qobject_cast<NetworkGraphWidget*>(parent());
	  OccurrenceGraphWidget *ogw = qobject_cast<OccurrenceGraphWidget*>(parent());
	  QPoint mousePos = mapFromGlobal(event->globalPos());
	  if (egw && egw->getEventItems().size() > 0) 
	    {
	      QMenu menu;
	      QAction *action1 = new QAction(ADDLINE, this);
	      QAction *action2 = new QAction(ADDSINGLEARROW, this);
	      QAction *action3 = new QAction(ADDDOUBLEARROW, this);
	      QAction *action4 = new QAction(ADDTEXT, this);
	      QAction *action5 = new QAction(ADDELLIPSE, this);
	      QAction *action6 = new QAction(ADDRECT, this);
	      menu.addAction(action1);
	      menu.addAction(action2);
	      menu.addAction(action3);
	      menu.addAction(action4);
	      menu.addAction(action5);
	      menu.addAction(action6);
	      if (QAction *action = menu.exec(event->globalPos())) 
		{
		  emit EventGraphContextMenuAction(action->text(), mousePos);
		}
	    }
	  else if (ngw && ngw->typesPresent()) 
	    {
	      QMenu menu;
	      QAction *action1 = new QAction(ADDLINE, this);
	      QAction *action2 = new QAction(ADDSINGLEARROW, this);
	      QAction *action3 = new QAction(ADDDOUBLEARROW, this);
	      QAction *action4 = new QAction(ADDTEXT, this);
	      QAction *action5 = new QAction(ADDELLIPSE, this);
	      QAction *action6 = new QAction(ADDRECT, this);
	      menu.addAction(action1);
	      menu.addAction(action2);
	      menu.addAction(action3);
	      menu.addAction(action4);
	      menu.addAction(action5);
	      menu.addAction(action6);
	      if (QAction *action = menu.exec(event->globalPos())) 
		{
		  emit NetworkGraphContextMenuAction(action->text(), mousePos);
		}
	    }
	  else if (ogw && (ogw->attributesPresent() || ogw->relationshipsPresent())) 
	    {
	      QMenu menu;
	      QAction *action1 = new QAction(ADDLINE, this);
	      QAction *action2 = new QAction(ADDSINGLEARROW, this);
	      QAction *action3 = new QAction(ADDDOUBLEARROW, this);
	      QAction *action4 = new QAction(ADDTEXT, this);
	      QAction *action5 = new QAction(ADDELLIPSE, this);
	      QAction *action6 = new QAction(ADDRECT, this);
	      menu.addAction(action1);
	      menu.addAction(action2);
	      menu.addAction(action3);
	      menu.addAction(action4);
	      menu.addAction(action5);
	      menu.addAction(action6);
	      if (QAction *action = menu.exec(event->globalPos())) 
		{
		  emit OccurrenceGraphContextMenuAction(action->text(), mousePos);
		}
	    }
	}
      else 
	{
	  this->setDragMode(QGraphicsView::NoDrag);
	  QGraphicsView::mousePressEvent(event);
	}
    }
  else if (event->button() == Qt::RightButton) 
    {
      EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->pos()));
      NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->pos()));
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(itemAt(event->pos()));
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->pos()));
      MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->pos()));
      NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->pos()));
      OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->pos()));
      OccurrenceLabel *occurrenceLabel = qgraphicsitem_cast<OccurrenceLabel*>(itemAt(event->pos()));
      LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->pos()));
      TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->pos()));
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->pos()));
      RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->pos()));
      if (nodeLabel) 
	{
	  incident = nodeLabel->getNode();
	}
      if (macroLabel) 
	{
	  macro = macroLabel->getMacroEvent();
	}
      if (occurrenceLabel) 
	{
	  occurrence = occurrenceLabel->getOccurrence();
	}
      if (!incident && !macro && !arrow && !networkNode && !occurrence &&
	  !occurrenceLabel && !line && !text && !ellipse && !rect) 
	{
	  pan = true;
	  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
	  lastMousePos = event->pos();
	  return;
	}
      else if (incident) 
	{
	  incident->setSelected(true);
	  return;
	}
      else if (macro) 
	{
	  macro->setSelected(true);
	}
      else if (arrow) 
	{
	  arrow->setSelected(true);
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
  else 
    {
      pan = false;
      QGraphicsView::mousePressEvent(event);
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) 
{
  pan = false;
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
  this->setDragMode(QGraphicsView::RubberBandDrag);
  if (event->button() == Qt::RightButton) 
    {
      lastMousePos = event->pos();
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
  if (pan) 
    {
      QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(lastMousePos);
      mouseDelta *= scaleFact;
      mouseDelta *= panSpeed;
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
      lastMousePos = event->pos();
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
  return pan;
}
