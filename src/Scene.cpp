#include "../include/Scene.h"
#include "../include/NodeLabel.h"
#include "../include/MacroLabel.h"
#include "../include/OccurrenceLabel.h"
#include <QtCore>

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
  resizeOnEvent = false;
  resizeOnMacro = false;
  moveOn = false;
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
  if (incident && !incident->isCopy()) {
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
  } else if (macro && !macro->isCopy()) {
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
    OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->scenePos(),
									    QTransform()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (incident) {
      if (incident->isCopy()) {
	clearSelection();
	incident->setSelected(true);
	selectedEvent = incident;
      } else {
	incident->setSelected(true);
	selectedEvent = incident;
	moveOn = true;
      }
    } else if (macro) {
      if (macro->isCopy()) {
	clearSelection();
	macro->setSelected(true);
	selectedMacro = macro;
      } else {
	macro->setSelected(true);
	selectedMacro = macro;
	moveOn = true;
      }
    } else if (networkNode) {
      networkNode->setSelected(true);
      selectedNode = networkNode;
      moveOn = true;
    } else if (occurrence) {
      this->clearSelection();
      occurrence->setSelected(true);
      selectedOccurrence = occurrence;
      moveOn = true;
    } else {
      this->clearSelection();
      selectedMacro = NULL;
      selectedEvent = NULL;
      selectedNode = NULL;
      selectedOccurrence = NULL;
    }
  } else if (event->modifiers() & Qt::ShiftModifier) {
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(), QTransform()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(),
								    QTransform()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (incident) {
      if (event->modifiers() & Qt::AltModifier) {
	if (incident->isCopy()) {
	  incident->setPos(incident->getOriginalPos());
	  incident->getLabel()->setNewPos(incident->scenePos());
	} else {
	  incident->setPos(incident->getOriginalPos().x(), incident->scenePos().y());
	  incident->getLabel()->setNewPos(incident->scenePos());
	}
      } else {
	if (!incident->isCopy()) {
	  resizeOnEvent = true;
	  lastMousePos = event->scenePos();
	  selectedEvent = incident;
	  selectedMacro = NULL;
	}
      }
    } else if (macro) {
      if (event->modifiers() & Qt::AltModifier) {
	if (macro->isCopy()) {
	  macro->setPos(macro->getOriginalPos());
	  macro->getLabel()->setNewPos(macro->scenePos());
	} else {
	  macro->setPos(macro->getOriginalPos().x(), macro->scenePos().y());
	  macro->getLabel()->setNewPos(macro->scenePos());
	}
      } else {
	if (!macro->isCopy()) {
	  resizeOnMacro = true;
	  lastMousePos = event->scenePos();
	  selectedMacro = macro;
	  selectedEvent = NULL;
	}
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
    OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(itemAt(event->scenePos(),
									    QTransform()));
    OccurrenceLabel *occurrenceLabel = qgraphicsitem_cast<OccurrenceLabel*>(itemAt(event->scenePos(),
										   QTransform()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (occurrenceLabel) {
      occurrence = occurrenceLabel->getOccurrence();
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
    } else if (occurrence) {
      this->clearSelection();
      occurrence->setSelected(true);
      selectedOccurrence = occurrence;
    }
    selectedEvent = NULL;
    selectedMacro = NULL;
    selectedNode  = NULL;
    QGraphicsScene::mousePressEvent(event);
  }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  resizeOnEvent = false;
  resizeOnMacro = false;
  moveOn = false;
  QListIterator<QGraphicsItem*> it(this->items());
  while (it.hasNext()) {
    QGraphicsItem *current = it.next();
    EventItem *incident = qgraphicsitem_cast<EventItem*>(current);
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
    NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(current);
    OccurrenceItem *occurrence = qgraphicsitem_cast<OccurrenceItem*>(current);    
    if (incident) {
      incident->setCursor(Qt::OpenHandCursor);
    } else if (macro) {
      macro->setCursor(Qt::OpenHandCursor);
    } else if (networkNode) {
      networkNode->setCursor(Qt::OpenHandCursor);
    } else if (occurrence) {
      occurrence->setCursor(Qt::OpenHandCursor);
    }
  }
  selectedEvent = NULL;
  selectedMacro = NULL;
  selectedNode = NULL;
  selectedOccurrence = NULL;
  QGraphicsScene::mouseReleaseEvent(event);
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
    if (selectedItems().size() > 1 && moveOn) {
      if (selectedEvent) {
 	emit moveItems(selectedEvent, event->scenePos());
      } else if (selectedMacro) {
	emit moveItems(selectedMacro, event->scenePos());
      } else if (selectedNode) {
	emit moveItems(selectedNode, event->scenePos());
      } else {
	moveOn = false;
      }
    } else {
      if (selectedOccurrence && moveOn) {
	emit moveLine(selectedOccurrence, event->scenePos());
      } else if (selectedOccurrence && !moveOn) {
	emit moveItems(selectedOccurrence, event->scenePos());
      }
    }
    QGraphicsScene::mouseMoveEvent(event);
  }
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(), QTransform()));
  Arrow *arrow = qgraphicsitem_cast<Arrow*>(itemAt(event->scenePos(), QTransform()));
  NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(), QTransform()));
  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(), QTransform()));
  MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(), QTransform()));
  NetworkNode *node = qgraphicsitem_cast<NetworkNode*>(itemAt(event->scenePos(), QTransform()));
  if (nodeLabel) {
    incident = nodeLabel->getNode();
  }
  if (macroLabel) {
    macro = macroLabel->getMacroEvent();
  }
  if (incident && !incident->isCopy()) {
    QMenu menu;
    QAction *action1 = new QAction(COLLIGATEPATHSACTION, this);
    menu.addAction(action1);
    QAction *action2 = new QAction(COLLIGATESEMIPATHSACTION, this);
    menu.addAction(action2);
    QAction *action3 = new QAction(MAKEMACROACTION, this);
    menu.addAction(action3);
    QAction *action4= new QAction(RECOLOREVENTSACTION, this);
    menu.addAction(action4);
    QAction *action5 = new QAction(RECOLORLABELSACTION, this);
    menu.addAction(action5);
    QAction *action6 = new QAction(COLORLINEAGEACTION, this);
    menu.addAction(action6);
    QAction *action7 = new QAction(SETTLEACTION, this);
    menu.addAction(action7);
    QAction *action8 = new QAction(PARALLELACTION, this);
    menu.addAction(action8);
    QAction *action9 = new QAction(NORMALIZEACTION, this);
    menu.addAction(action9);
    QAction *action10 = new QAction(CLOSEGAPACTION, this);
    menu.addAction(action10);
    if (selectedItems().size() > 1) {
      action3->setEnabled(false);
      action6->setEnabled(false);
      action7->setEnabled(false);
      action9->setEnabled(false);
      action10->setEnabled(false);
    }
    if (selectedItems().size() == 1) {
      action1->setEnabled(false);
      action2->setEnabled(false);
      action8->setEnabled(false);
    }
    if (QAction *action = menu.exec(event->screenPos())) {
      emit EventItemContextMenuAction(action->text());
    }
    // And then we'll capture some action, and send a signal to the main widget.
  } else if (macro && !macro->isCopy()) {
    QMenu menu;
    QAction *action1 = new QAction(COLLIGATEPATHSACTION, this);
    menu.addAction(action1);
    QAction *action2 = new QAction(COLLIGATESEMIPATHSACTION, this);
    menu.addAction(action2);
    QAction *action3 = new QAction(DISAGGREGATEACTION, this);
    menu.addAction(action3);
    QAction *action4 = new QAction(RECOLOREVENTSACTION, this);
    menu.addAction(action4);
    QAction *action5 = new QAction(RECOLORLABELSACTION, this);
    menu.addAction(action5);
    QAction *action6 = new QAction(COLORLINEAGEACTION, this);
    menu.addAction(action6);
    QAction *action7 = new QAction(SETTLEACTION, this);
    menu.addAction(action7);
    QAction *action8 = new QAction(PARALLELACTION, this);
    menu.addAction(action8);
    QAction *action9 = new QAction(NORMALIZEACTION, this);
    menu.addAction(action9);
    QAction *action10 = new QAction(CLOSEGAPACTION, this);
    menu.addAction(action10);
    QAction *action11 = new QAction(CHANGEDESCRIPTIONACTION, this);
    menu.addAction(action11);
    if (selectedItems().size() > 1) {
      action3->setEnabled(false);
      action6->setEnabled(false);
      action7->setEnabled(false);
      action9->setEnabled(false);
      action10->setEnabled(false);
      action11->setEnabled(false);
    }
    if (selectedItems().size() == 1) {
      action1->setEnabled(false);
      action2->setEnabled(false);
      action8->setEnabled(false);
    }
    if (QAction *action = menu.exec(event->screenPos())) {
      emit EventItemContextMenuAction(action->text());
    }
  } else if (arrow && !arrow->isCopy()) {
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
      menu.addAction(IGNOREME);      
    } else if (arrow->getColor() == QColor(Qt::black)) {
      menu.addAction(REMOVENORMALLINKAGE);
    }
    if (QAction *action = menu.exec(event->screenPos())) {
      emit ArrowContextMenuAction(action->text());
    }
  } else if (node) {
    clearSelection();
    node->setSelected(true);
    QMenu menu;
    menu.addAction(HIDENODE);
    if (QAction *action = menu.exec(event->screenPos())) {
      emit NetworkNodeContextMenuAction(action->text());
    }
  }
}

OccurrenceItem* Scene::getSelectedOccurrence() {
  return selectedOccurrence;
}
