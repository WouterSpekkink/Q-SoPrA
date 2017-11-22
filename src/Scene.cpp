#include "../include/Scene.h"
#include "../include/NodeLabel.h"
#include <QtCore>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
  resizeOn = false;
}

QRectF Scene::itemsBoundingRect() const {
  QRectF boundingRect;
  const auto items_ = items();
  for (QGraphicsItem *item : items_)
    if (item->isVisible()) {
      boundingRect |= item->sceneBoundingRect();
    }
  return boundingRect;
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
  EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(wheelEvent->scenePos(), QTransform()));
  Arrow *no = qgraphicsitem_cast<Arrow*>(itemAt(wheelEvent->scenePos(), QTransform()));
  NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(itemAt(wheelEvent->scenePos(), QTransform()));
  if (text && !(no)) {
    item = text->getNode();
  }
  if (item && !(no)) {
    if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0 && item->getWidth() < 1000) {
	item->setWidth(item->getWidth() + 1);
	emit widthIncreased(item);
      } else if (wheelEvent->delta() < 0 && item->getWidth() > 40) {
	item->setWidth(item->getWidth() - 1);
	emit widthDecreased(item);
      } 
    }
    emit relevantChange();
    wheelEvent->accept();
  } else {
    wheelEvent->ignore();
  }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->modifiers() & Qt::ControlModifier) {
    EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    Arrow *no = qgraphicsitem_cast<Arrow*>(itemAt(event->scenePos(), QTransform()));
    NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
    if (text && !(no)) {
      item = text->getNode();
    }
    if (item && !(no)) {
      item->setSelected(true);
    }
  } else {
    this->clearSelection();
  }
  
  if (event->modifiers() & Qt::ShiftModifier) {
    EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    Arrow *no = qgraphicsitem_cast<Arrow*>(itemAt(event->scenePos(), QTransform()));
    NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
    if (text && !(no)) {
      item = text->getNode();
    }
    if (item && !(no)) {
      if (event->modifiers() & Qt::AltModifier) {
	item->setPos(item->getOriginalPos().x(), item->scenePos().y());
      } else {
	resizeOn = true;
	lastMousePos = event->scenePos();
	selectedEvent = item;
      }
    }
    return;
  } else {
    selectedEvent = NULL;
    QGraphicsScene::mousePressEvent(event);
  }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  resizeOn = false;
  lastMousePos = event->scenePos();
  if (selectedEvent) {
    selectedEvent->setCursor(Qt::OpenHandCursor);
    selectedEvent = NULL;
  } else {
    QGraphicsScene::mouseReleaseEvent(event);
  }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (resizeOn) {
    emit relevantChange();
    if (event->scenePos().x() - lastMousePos.x() > 0) {
      int currentX = selectedEvent->scenePos().x();
      int currentY = selectedEvent->scenePos().y();
      qreal oldX = selectedEvent->getOriginalPos().x();
      if (oldX != currentX && selectedEvent->isDislodged() == false) {
	currentX++;
	selectedEvent->setPos(currentX, currentY);
	emit posIncreased(selectedEvent);
	selectedEvent->setDislodged(true);
      } else if (oldX == currentX && selectedEvent->isDislodged() == true) {
	selectedEvent->setPos(oldX, currentY);
      } else {
	currentX++;
	selectedEvent->setPos(currentX, currentY);
	emit posIncreased(selectedEvent);
      }
    } else if (event->scenePos().x() - lastMousePos.x() < 0) {
      int currentX = selectedEvent->scenePos().x();
      int currentY = selectedEvent->scenePos().y();
      qreal oldX = selectedEvent->getOriginalPos().x();
      if (oldX != currentX && selectedEvent->isDislodged() == false) {
	currentX--;
	selectedEvent->setPos(currentX, currentY);
	emit posDecreased(selectedEvent);
	selectedEvent->setDislodged(true);
      } else if (oldX == currentX && selectedEvent->isDislodged() == true) {
	selectedEvent->setPos(oldX, currentY);
      } else {
	currentX--;
	selectedEvent->setPos(currentX, currentY);
	emit posDecreased(selectedEvent);
      }
    }
    lastMousePos = event->scenePos();
  } else {
    QGraphicsScene::mouseMoveEvent(event);
  }
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
  Arrow *no = qgraphicsitem_cast<Arrow*>(itemAt(event->scenePos(), QTransform()));
  NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
 if (text && !(no)) {
    item = text->getNode();
  }
  if (item && !(no)) {
    QMenu menu;
    menu.addAction(COLLIGATEACTION);
    menu.addAction(RECOLOREVENTSACTION);
    menu.addAction(RECOLORLABELSACTION);

    if (QAction *action = menu.exec(event->screenPos())) {
      emit EventItemContextMenuAction(action->text());
    }
    
    // And then we'll capture some action, and send a signal to the main widget.
    

  }
}

