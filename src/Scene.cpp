#include "../include/Scene.h"
#include "../include/NodeLabel.h"
#include "../include/MacroLabel.h"
#include "../include/NetworkNode.h"
#include <QtCore>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
  resizeOnEvent = false;
  resizeOnMacro = false;
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
  EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(wheelEvent->scenePos(),
							      QTransform()));
  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(wheelEvent->scenePos(),
							     QTransform()));
  NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(wheelEvent->scenePos(),
							       QTransform()));
  MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(wheelEvent->scenePos(),
								 QTransform()));
  if (nodeLabel) {
    incident = nodeLabel->getNode();
  }
  if (macroLabel) {
    macro = macroLabel->getMacroEvent();
  }
  if (incident) {
    if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0 && incident->getWidth() < 1000) {
	incident->setWidth(incident->getWidth() + 1);
	emit widthIncreased(incident);
      } else if (wheelEvent->delta() < 0 && incident->getWidth() > 40) {
	incident->setWidth(incident->getWidth() - 1);
	emit widthDecreased(incident);
      } 
    }
    emit relevantChange();
    wheelEvent->accept();
  } else if (macro) {
    if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0 && macro->getWidth() < 1000) {
	macro->setWidth(macro->getWidth() + 1);
	emit widthIncreased(macro);
      } else if (wheelEvent->delta() < 0 && macro->getWidth() > 40) {
	macro->setWidth(macro->getWidth() - 1);
	emit widthDecreased(macro);
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
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(), QTransform()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(), QTransform()));
    NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->scenePos(),
								       QTransform()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (incident) {
      incident->setSelected(true);
    } else if (macro) {
      macro->setSelected(true);
    } else if (networkNode) {
      networkNode->setSelected(true);
    } else {
      this->clearSelection();
    }
  } else if (event->modifiers() & Qt::ShiftModifier) {
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(), QTransform()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(), QTransform()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (incident) {
      if (event->modifiers() & Qt::AltModifier) {
	incident->setPos(incident->getOriginalPos().x(), incident->scenePos().y());
	incident->getLabel()->setNewPos(incident->scenePos());
      } else {
	resizeOnEvent = true;
	lastMousePos = event->scenePos();
	selectedEvent = incident;
	selectedMacro = NULL;
      }
    } else if (macro) {
      if (event->modifiers() & Qt::AltModifier) {
	macro->setPos(macro->getOriginalPos().x(), macro->scenePos().y());
	macro->getLabel()->setNewPos(macro->scenePos());
      } else {
	resizeOnMacro = true;
	lastMousePos = event->scenePos();
	selectedMacro = macro;
	selectedEvent = NULL;
      }
    }
    return;
  } else {
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(), QTransform()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(), QTransform()));
    NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->scenePos(),
								       QTransform()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (incident) {
      this->clearSelection();
      incident->setSelected(true);
    } else if (macro) {
      this->clearSelection();
      macro->setSelected(true);
    } else if (networkNode) {
      this->clearSelection();
      networkNode->setSelected(true);
    }
    selectedEvent = NULL;
    selectedMacro = NULL;
    QGraphicsScene::mousePressEvent(event);
  }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  resizeOnEvent = false;
  resizeOnMacro = false;
  lastMousePos = event->scenePos();

  if (selectedEvent) {
    selectedEvent->setCursor(Qt::OpenHandCursor);
    selectedEvent = NULL;
  } else if (selectedMacro) {
    selectedMacro->setCursor(Qt::OpenHandCursor);
    selectedMacro = NULL;
  } else {
    QGraphicsScene::mouseReleaseEvent(event);
  }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (resizeOnEvent) {
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
  } else if (resizeOnMacro) {
    emit relevantChange();
    if (event->scenePos().x() - lastMousePos.x() > 0) {
      int currentX = selectedMacro->scenePos().x();
      int currentY = selectedMacro->scenePos().y();
      qreal oldX = selectedMacro->getOriginalPos().x();
      if (oldX != currentX && selectedMacro->isDislodged() == false) {
	currentX++;
	selectedMacro->setPos(currentX, currentY);
	emit posIncreased(selectedMacro);
	selectedMacro->setDislodged(true);
      } else if (oldX == currentX && selectedMacro->isDislodged() == true) {
	selectedMacro->setPos(oldX, currentY);
      } else {
	currentX++;
	selectedMacro->setPos(currentX, currentY);
	emit posIncreased(selectedMacro);
      }
    } else if (event->scenePos().x() - lastMousePos.x() < 0) {
      int currentX = selectedMacro->scenePos().x();
      int currentY = selectedMacro->scenePos().y();
      qreal oldX = selectedMacro->getOriginalPos().x();
      if (oldX != currentX && selectedMacro->isDislodged() == false) {
	currentX--;
	selectedMacro->setPos(currentX, currentY);
	emit posDecreased(selectedMacro);
	selectedMacro->setDislodged(true);
      } else if (oldX == currentX && selectedMacro->isDislodged() == true) {
	selectedMacro->setPos(oldX, currentY);
      } else {
	currentX--;
	selectedMacro->setPos(currentX, currentY);
	emit posDecreased(selectedMacro);
      }
    }
    lastMousePos = event->scenePos();
  } else {
    QGraphicsScene::mouseMoveEvent(event);
  }
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
  Arrow *arrow = qgraphicsitem_cast<Arrow*>(itemAt(event->scenePos(), QTransform()));
  NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(), QTransform()));
  MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(), QTransform()));

  if (nodeLabel) {
    incident = nodeLabel->getNode();
  }
  if (macroLabel) {
    macro = macroLabel->getMacroEvent();
  }
  if (incident) {
    QMenu menu;
    menu.addAction(EQUIVALENCEACTION);
    //menu.addAction(TOLERANCEACTION);
    menu.addAction(RECOLOREVENTSACTION);
    menu.addAction(RECOLORLABELSACTION);
    menu.addAction(SETTLEACTION);
    menu.addAction(NORMALIZEACTION);
    menu.addAction(CLOSEGAPACTION);
    if (QAction *action = menu.exec(event->screenPos())) {
      emit EventItemContextMenuAction(action->text());
    }
    // And then we'll capture some action, and send a signal to the main widget.
  } else if (macro) {
    QMenu menu;
    menu.addAction(EQUIVALENCEACTION);
    //menu.addAction(TOLERANCEACTION);
    menu.addAction(DISAGGREGATEACTION);
    menu.addAction(RECOLOREVENTSACTION);
    menu.addAction(RECOLORLABELSACTION);
    menu.addAction(SETTLEACTION);
    menu.addAction(NORMALIZEACTION);
    menu.addAction(CLOSEGAPACTION);
    if (QAction *action = menu.exec(event->screenPos())) {
      emit EventItemContextMenuAction(action->text());
    }
  } else if (arrow) {
    clearSelection();
    arrow->setSelected(true);
    QMenu menu;
    if (arrow->getColor() == QColor(Qt::darkMagenta)) {
      menu.addAction(REMOVELINKAGEACTION);
      menu.addAction(KEEPLINKAGEACTION);
    } else if (arrow->getColor() == QColor(Qt::darkRed)) {
      menu.addAction(ACCEPTLINKAGEACTION);
      menu.addAction(REJECTLINKAGEACTION);
    } else if (arrow->getColor() == QColor(Qt::darkGreen)) {
      menu.addAction(REMOVELINKAGEACTION);
      menu.addAction(KEEPLINKAGEACTION);
      menu.addAction(IGNORE);      
    } else if (arrow->getColor() == QColor(Qt::black)) {
      menu.addAction(REMOVENORMALLINKAGE);
    }
    if (QAction *action = menu.exec(event->screenPos())) {
      emit ArrowContextMenuAction(action->text());
    }
  }
}

