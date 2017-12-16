#include "../include/GraphicsView.h"
#include "../include/Scene.h"
#include "../include/NodeLabel.h"
#include "../include/MacroLabel.h"
#include "../include/NetworkNode.h"
#include "../include/OccurrenceItem.h"
#include "../include/OccurrenceLabel.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

GraphicsView::GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) {
  panSpeed = 4;
  scaleFact = 1;
}

void GraphicsView::resizeEvent(QResizeEvent *) {};

void GraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->modifiers() & Qt::ShiftModifier) {
    this->setDragMode(QGraphicsView::NoDrag);
    QGraphicsView::mousePressEvent(event);
  } else if (event->modifiers() & Qt::ControlModifier) {
    this->setDragMode(QGraphicsView::NoDrag);
    QGraphicsView::mousePressEvent(event);
  } else if (event->button() == Qt::RightButton) {
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->pos()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->pos()));
    Arrow *arrow = qgraphicsitem_cast<Arrow*>(itemAt(event->pos()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->pos()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->pos()));
    NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->pos()));
    OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->pos()));
    OccurrenceLabel *occurrenceLabel = qgraphicsitem_cast<OccurrenceLabel*>(itemAt(event->pos()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (occurrenceLabel) {
      occurrence = occurrenceLabel->getOccurrence();
    }
    if (!incident && !macro && !arrow && !networkNode && !occurrence && !occurrenceLabel) {
      pan = true;
      setCursor(Qt::ClosedHandCursor);
      lastMousePos = event->pos();
      return;
    } else if (incident) {
      incident->setSelected(true);
      return;
    } else if (macro) {
      macro->setSelected(true);
    } else if (arrow) {
      arrow->setSelected(true);
      return;
    } else if (occurrence) {
      occurrence->setSelected(true);
    } else if (networkNode) {
      networkNode->setSelected(true);
    }
  } else {
    pan = false;
    QGraphicsView::mousePressEvent(event);
  }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  pan = false;
  this->setDragMode(QGraphicsView::RubberBandDrag);
  if (event->button() == Qt::RightButton) {
    setCursor(Qt::ArrowCursor);
    lastMousePos = event->pos();
    return;
  } else {
    QGraphicsView::mouseReleaseEvent(event);
  }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
  if (pan) {
    QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(lastMousePos);
    
    mouseDelta *= scaleFact;
    mouseDelta *= panSpeed;
    
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - mouseDelta.x(),  VIEW_HEIGHT / 2 - mouseDelta.y());
    centerOn(mapToScene(newCenter));
    lastMousePos = event->pos();
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
  } else {
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

void GraphicsView::wheelEvent(QWheelEvent* event) {
  if (event->modifiers() & Qt::ControlModifier) {
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;
    if (event->delta() > 0) {
      this->scale(scaleFactor, scaleFactor);
    } else {
      this->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
  } else if (event->modifiers() & Qt::ShiftModifier) {
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
    if (!(event->isAccepted())) {
      if (event->delta() > 0) {
	this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - event->delta());
      } else {
	this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - event->delta());
      }
    }
  } else {
    QGraphicsView::wheelEvent(event);
  }
}
   
