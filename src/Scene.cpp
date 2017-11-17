#include "../include/Scene.h"
#include <QtCore>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
  resizeOn = false;
};

void Scene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
  EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(wheelEvent->scenePos(), QTransform()));
  if (item) {
    if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0 && item->width <= 1000) {
	item->width++;
	emit widthIncreased(item);
      } else if (wheelEvent->delta() < 0 && item->width >= 30) {
	item->width--;
	emit widthDecreased(item);
      } 
    }
    item->update();
  }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->modifiers() & Qt::ShiftModifier) {
    resizeOn = true;
    lastMousePos = event->scenePos();
    return;
  } else {
    QGraphicsScene::mousePressEvent(event);
  }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (event->modifiers() & Qt::ShiftModifier) {
    resizeOn = false;
    lastMousePos = event->scenePos();
    return;
  } else {
    QGraphicsScene::mouseReleaseEvent(event);
  }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (resizeOn) {
    EventItem *item = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    Arrow *no = qgraphicsitem_cast<Arrow*>(itemAt(event->scenePos(), QTransform()));
    if (item && !(no)) {
      if (event->scenePos().x() - lastMousePos.x() > 0) {
	if (item->width <= 1000) {
	  item->width++;
	  item->update();
	  emit widthIncreased(item);
	}
      } else if (event->scenePos().x() - lastMousePos.x() < 0) {
	if (item->width >= 30) {
	  item->width--;
	  item->update();
	  emit widthDecreased(item);
	}
      }
      lastMousePos = event->scenePos();
    } else {
      return;
    }
    //      item->width = item->width + mouseDelta.x();
  } else {
    QGraphicsScene::mouseMoveEvent(event);
  }
}
