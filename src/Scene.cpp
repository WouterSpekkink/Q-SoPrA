#include "../include/Scene.h"
#include <QtCore>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
  resizeOn = false;
};

void Scene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
  EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(wheelEvent->scenePos(), QTransform()));
  Arrow *no = qgraphicsitem_cast<Arrow*>(itemAt(wheelEvent->scenePos(), QTransform()));
  if (item && !(no)) {
    if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0 && item->width < 1000) {
	item->width++;
	emit widthIncreased(item);
      } else if (wheelEvent->delta() < 0 && item->width > 30) {
	item->width--;
	emit widthDecreased(item);
      } 
    }
    item->update();
    wheelEvent->accept();
  } else {
    wheelEvent->ignore();
  }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  this->clearSelection();
  if (event->modifiers() & Qt::ShiftModifier) {
    EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    Arrow *no = qgraphicsitem_cast<Arrow*>(itemAt(event->scenePos(), QTransform()));
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
