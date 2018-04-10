#include "../include/Scene.h"
#include "../include/GraphicsView.h"
#include <math.h>
#include <QtCore>



Scene::Scene(QObject *parent) : QGraphicsScene(parent) {
  resizeOnEvent = false;
  resizeOnMacro = false;
  moveOn = false;
  lineMoveOn = false;
  moveLineObject = false;
  manipulateEllipse = false;
  moveEllipse = false;
  rotateEllipse = false;
  manipulateRect = false;
  moveRect = false;
  rotateRect = false;
  rotateText = false;
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
  TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(wheelEvent->scenePos(),
							    QTransform()));
  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(wheelEvent->scenePos(),
								     QTransform()));
  LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(wheelEvent->scenePos(),
								  QTransform()));
  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(wheelEvent->scenePos(),
							       QTransform()));
  if (nodeLabel) {
    incident = nodeLabel->getNode();
  }
  if (macroLabel) {
    macro = macroLabel->getMacroEvent();
  }
  if (incident && !incident->isCopy()) {
    clearSelection();
    incident->setSelected(true);
    emit resetItemSelection();
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
    clearSelection();
    macro->setSelected(true);
    emit resetItemSelection();
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
  } else if (text) {
    clearSelection();
    text->setSelected(true);
    emit resetItemSelection();
    if (wheelEvent->modifiers() & Qt::ControlModifier) {
      if (wheelEvent->delta() > 0) {
	QFont font = text->font();
	int size = font.pointSize();
	if (size <= 39) {
	  size++;
	  font.setPointSize(size);
	  text->setFont(font);
	}
      } else if (wheelEvent->delta() < 0) {
	QFont font = text->font();
	int size = font.pointSize();
	if (size >= 9) {
	  size--;
	  font.setPointSize(size);
	  text->setFont(font);
	}
      }
    }
    if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0) {
	  int width = text->textWidth();
	  width = width + 10;
	  text->setTextWidth(width);
      } else if (wheelEvent->delta() < 0) {
	int width = text->textWidth();
	width = width - 10;
	text->setTextWidth(width);
      }
    }
    emit relevantChange();
    wheelEvent->accept();
  } else if (ellipse) {
    clearSelection();
    ellipse->setSelected(true);
    emit resetItemSelection();
    if (wheelEvent->modifiers() & Qt::ControlModifier) {
      if (wheelEvent->delta() > 0) {
	if (ellipse->getPenWidth() < 20) {
	  ellipse->setPenWidth(ellipse->getPenWidth() + 1);
	}
      } else if (wheelEvent->delta() < 0) {
	if (ellipse->getPenWidth() > 1) {
	  ellipse->setPenWidth(ellipse->getPenWidth() - 1);
	}
      }
    } else if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0) {
	if (ellipse->getPenStyle() < 5) {
	  ellipse->setPenStyle(ellipse->getPenStyle() + 1);
	} else {
	  ellipse->setPenStyle(1);
	}
      } else if (wheelEvent->delta() < 0) {
	if (ellipse->getPenStyle() > 1) {
	  ellipse->setPenStyle(ellipse->getPenStyle() - 1);
	} else {
	  ellipse->setPenStyle(5);
	}
      }
    }
    wheelEvent->accept();
  } else if (line) {
    clearSelection();
    line->setSelected(true);
    emit resetItemSelection();
    if (wheelEvent->modifiers() & Qt::ControlModifier) {
      if (wheelEvent->delta() > 0) {
	if (line->getPenWidth() < 7) {
	  line->setPenWidth(line->getPenWidth() + 1);
	}
      } else if (wheelEvent->delta() < 0) {
	if (line->getPenWidth() > 1) {
	  line->setPenWidth(line->getPenWidth() - 1);
	}
      }
    } else if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0) {
	if (line->getPenStyle() < 5) {
	  line->setPenStyle(line->getPenStyle() + 1);
	} else {
	  line->setPenStyle(1);
	}
      } else if (wheelEvent->delta() < 0) {
	if (line->getPenStyle() > 1) {
	  line->setPenStyle(line->getPenStyle() - 1);
	} else {
	  line->setPenStyle(5);
	}
      }
    }
    wheelEvent->accept();
  } else if (rect) {
    clearSelection();
    rect->setSelected(true);
    emit resetItemSelection();
    if (wheelEvent->modifiers() & Qt::ControlModifier) {
      if (wheelEvent->delta() > 0) {
	if (rect->getPenWidth() < 20) {
	  rect->setPenWidth(rect->getPenWidth() + 1);
	}
      } else if (wheelEvent->delta() < 0) {
	if (rect->getPenWidth() > 1) {
	  rect->setPenWidth(rect->getPenWidth() - 1);
	}
      }
    } else if (wheelEvent->modifiers() & Qt::ShiftModifier) {
      if (wheelEvent->delta() > 0) {
	if (rect->getPenStyle() < 5) {
	  rect->setPenStyle(rect->getPenStyle() + 1);
	} else {
	  rect->setPenStyle(1);
	}
      } else if (wheelEvent->delta() < 0) {
	if (rect->getPenStyle() > 1) {
	  rect->setPenStyle(rect->getPenStyle() - 1);
	} else {
	  rect->setPenStyle(5);
	}
      }
    }
    wheelEvent->accept();
   } else {
    wheelEvent->ignore();
  }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->modifiers() & Qt::ControlModifier) {
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(),
								QTransform()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(),
								 QTransform()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(),
							       QTransform()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(),
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
	emit resetItemSelection();
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
	emit resetItemSelection();
	macro->setSelected(true);
	selectedMacro = macro;
	moveOn = true;
      }
    } else if (networkNode) {
      networkNode->setSelected(true);
      selectedNode = networkNode;
      moveOn = true;
    } else if (occurrence) {
      clearSelection();
      occurrence->setSelected(true);
      selectedOccurrence = occurrence;
      moveOn = true;
    } else if (text) {
      clearSelection();
      text->setSelected(true);
      emit resetItemSelection();
      selectedText = text;
      rotateText = true;
      QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
      qApp->processEvents();
    } else if (ellipse) {
      if (!ellipse->getValidArea().contains(event->scenePos())) {
	clearSelection();
	ellipse->setSelected(true);
	emit resetItemSelection();
	selectedEllipse = ellipse;
	rotateEllipse = true;
	QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
	qApp->processEvents();
      }
    } else if (rect) {
      if (!rect->getValidArea().containsPoint(event->scenePos(), Qt::OddEvenFill)) {
	clearSelection();
	rect->setSelected(true);
	emit resetItemSelection();
	selectedRect = rect;
	rotateRect = true;
	QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
	qApp->processEvents();
      }
    } else {
      clearSelection();
      emit resetItemSelection();
      selectedMacro = NULL;
      selectedEvent = NULL;
      selectedNode = NULL;
      selectedOccurrence = NULL;
      selectedRect = NULL;
    }
  } else if (event->modifiers() & Qt::ShiftModifier) {
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(),
								QTransform()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(),
								 QTransform()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(),
							       QTransform()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(),
								    QTransform()));
    LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->scenePos(),
							      QTransform()));
    EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->scenePos(),
								       QTransform()));
    RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->scenePos(),
							      QTransform()));
    if (nodeLabel) {
      incident = nodeLabel->getNode();
    }
    if (macroLabel) {
      macro = macroLabel->getMacroEvent();
    }
    if (incident) {
      emit resetItemSelection();
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
      emit resetItemSelection();
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
    } else if (line) {
      clearSelection();
      selectedLine = line;
      line->setSelected(true);
      emit resetItemSelection();
      moveLineObject = true;
      lastMousePos = event->scenePos();
      QApplication::setOverrideCursor(Qt::SizeAllCursor);
      qApp->processEvents();
    } else if (ellipse) {
      clearSelection();
      if (!ellipse->getValidArea().contains(event->scenePos())) {
	selectedEllipse = ellipse;
	ellipse->setSelected(true);
	emit resetItemSelection();
	moveEllipse = true;
	lastMousePos = event->scenePos();
	QApplication::setOverrideCursor(Qt::SizeAllCursor);
	qApp->processEvents();
      }
    } else if (rect) {
      if (!rect->getValidArea().containsPoint(event->scenePos(), Qt::OddEvenFill)) {
	clearSelection();
	selectedRect = rect;
	rect->setSelected(true);
	emit resetItemSelection();
	moveRect = true;
	lastMousePos = event->scenePos();
	QApplication::setOverrideCursor(Qt::SizeAllCursor);
	qApp->processEvents();
      }
    }
    return;
  } else {
    EventItem *incident = qgraphicsitem_cast<EventItem*>(itemAt(event->scenePos(),
								QTransform()));
    NodeLabel *nodeLabel = qgraphicsitem_cast<NodeLabel*>(itemAt(event->scenePos(),
								 QTransform()));
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(itemAt(event->scenePos(),
							       QTransform()));
    MacroLabel *macroLabel = qgraphicsitem_cast<MacroLabel*>(itemAt(event->scenePos(),
								    QTransform()));
    NetworkNode *networkNode = qgraphicsitem_cast<NetworkNode*>(itemAt(event->scenePos(),
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
      clearSelection();
      incident->setSelected(true);
      emit resetItemSelection();
    } else if (macro) {
      clearSelection();
      macro->setSelected(true);
      emit resetItemSelection();
    } else if (networkNode) {
      clearSelection();
      networkNode->setSelected(true);
    } else if (occurrence) {
      clearSelection();
      occurrence->setSelected(true);
      selectedOccurrence = occurrence;
    } else if (line) {
      clearSelection();
      selectedLine = line;
      emit resetItemSelection();
      lineMoveOn = true;
    } else if (ellipse) {
      if (!ellipse->getValidArea().contains(event->scenePos())) {
	clearSelection();
	selectedEllipse = ellipse;
	ellipse->setSelected(true);
	emit resetItemSelection();
	manipulateEllipse = true;
      }
    } else if (rect) {
      if (!rect->getValidArea().containsPoint(event->scenePos(), Qt::OddEvenFill)) {
	clearSelection();
	rect->setSelected(true);
	emit resetItemSelection();
	selectedRect = rect;
	manipulateRect = true;
      }
    } else if (text) {
      QApplication::setOverrideCursor(Qt::SizeAllCursor);
      qApp->processEvents();
    }
    selectedEvent = NULL;
    selectedMacro = NULL;
    selectedNode = NULL;
    emit resetItemSelection();
    QGraphicsScene::mousePressEvent(event);
  }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  resizeOnEvent = false;
  resizeOnMacro = false;
  moveOn = false;
  lineMoveOn = false;
  moveLineObject = false;
  manipulateEllipse = false;
  moveEllipse = false;
  rotateEllipse = false;
  manipulateRect = false;
  moveRect = false;
  rotateRect = false;
  rotateText = false;
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
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
  selectedLine = NULL;
  selectedEllipse = NULL;
  selectedText = NULL;
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
  } else if (lineMoveOn) {
    emit relevantChange();
    lastMousePos = event->scenePos();
    QPointF start = selectedLine->getStartPos();
    QPointF end = selectedLine->getEndPos();
    qreal distStart = sqrt(pow((lastMousePos.x() - start.x()), 2) +
			   pow((lastMousePos.y() - start.y()), 2));
    qreal distEnd = sqrt(pow((lastMousePos.x() - end.x()), 2) +
			   pow((lastMousePos.y() - end.y()), 2));
    if (distStart < distEnd) {
      if (event->modifiers() & Qt::ControlModifier) {
	if (abs((lastMousePos.y() - end.y()) / (lastMousePos.x() - end.x())) < 1) {
	  selectedLine->setStartPos(lastMousePos.x(), end.y());
	} else {
	  selectedLine->setStartPos(end.x(), lastMousePos.y());
	}
      } else {
	selectedLine->setStartPos(lastMousePos);
      }
    } else {
      if (event->modifiers() & Qt::ControlModifier) {
	if (abs((lastMousePos.y() - start.y()) / (lastMousePos.x() - start.x())) < 1) {
	  selectedLine->setEndPos(lastMousePos.x(), start.y());
	} else {
	  selectedLine->setEndPos(start.x(), lastMousePos.y());
	}
      } else {
	selectedLine->setEndPos(lastMousePos);
      }
    }
    emit relevantChange();
  } else if (moveLineObject) {
    emit relevantChange();
    QPointF newPos = selectedLine->mapFromScene(event->scenePos());
    qreal newXDiff = newPos.x() - lastMousePos.x();
    qreal newYDiff = newPos.y() - lastMousePos.y();
    selectedLine->setStartPos(selectedLine->mapToScene(selectedLine->getStartPos() +
						       QPointF(newXDiff, newYDiff)));
    selectedLine->setEndPos(selectedLine->mapToScene(selectedLine->getEndPos() +
						     QPointF(newXDiff, newYDiff)));
    lastMousePos = event->scenePos();
  } else if (manipulateEllipse) {
    emit relevantChange();
    lastMousePos = event->scenePos();
    QPointF topLeft = selectedEllipse->mapToScene(selectedEllipse->topLeft());
    QPointF bottomLeft = selectedEllipse->mapToScene(selectedEllipse->bottomLeft());
    QPointF topRight = selectedEllipse->mapToScene(selectedEllipse->topRight());
    QPointF bottomRight = selectedEllipse->mapToScene(selectedEllipse->bottomRight());
    QPointF left = selectedEllipse->mapToScene(QPointF(selectedEllipse->getLeft(),
						       selectedEllipse->getCenter().y()));
    QPointF right = selectedEllipse->mapToScene(QPointF(selectedEllipse->getRight(),
							selectedEllipse->getCenter().y()));
    QPointF top = selectedEllipse->mapToScene(QPointF(selectedEllipse->getCenter().x(),
						      selectedEllipse->getTop()));
    QPointF bottom = selectedEllipse->mapToScene(QPointF(selectedEllipse->getCenter().x(),
							 selectedEllipse->getBottom()));
    qreal distTopLeft = sqrt(pow((lastMousePos.x() - topLeft.x()), 2) +
			     pow((lastMousePos.y() - topLeft.y()), 2));
    qreal distTopRight = sqrt(pow((lastMousePos.x() - topRight.x()), 2) +
			      pow((lastMousePos.y() - topRight.y()), 2));
    qreal distBottomLeft = sqrt(pow((lastMousePos.x() - bottomLeft.x()), 2) +
				pow((lastMousePos.y() - bottomLeft.y()), 2));
    qreal distBottomRight = sqrt(pow((lastMousePos.x() - bottomRight.x()), 2) +
				 pow((lastMousePos.y() - bottomRight.y()), 2));
    qreal distLeft = sqrt(pow((lastMousePos.x() - left.x()), 2) +
			  pow((lastMousePos.y() - left.y()), 2));
    qreal distRight = sqrt(pow((lastMousePos.x() - right.x()), 2) +
			   pow((lastMousePos.y() - right.y()), 2));
    qreal distTop = sqrt(pow((lastMousePos.x() - top.x()), 2) +
			 pow((lastMousePos.y() - top.y()), 2));
    qreal distBottom = sqrt(pow((lastMousePos.x() - bottom.x()), 2) +
			    pow((lastMousePos.y() - bottom.y()), 2));
    qreal minimum = std::min({distTopLeft, distTopRight, distBottomLeft, distBottomRight,
	  distLeft, distRight, distTop, distBottom});
    if (minimum == distTopLeft) {
      selectedEllipse->setTopLeft(selectedEllipse->mapFromScene(lastMousePos));
    } else if (minimum == distTopRight) {
      selectedEllipse->setTopRight(selectedEllipse->mapFromScene(lastMousePos));
    } else if (minimum == distBottomLeft) {
      selectedEllipse->setBottomLeft(selectedEllipse->mapFromScene(lastMousePos));
    } else if (minimum == distBottomRight) {
      selectedEllipse->setBottomRight(selectedEllipse->mapFromScene(lastMousePos));
    } else if (minimum == distLeft) {
      selectedEllipse->setLeft(selectedEllipse->mapFromScene(lastMousePos).x());
    } else if (minimum == distRight) {
      selectedEllipse->setRight(selectedEllipse->mapFromScene(lastMousePos).x());
    } else if (minimum == distTop) {
      selectedEllipse->setTop(selectedEllipse->mapFromScene(lastMousePos).y());
    } else if (minimum == distBottom) {
      selectedEllipse->setBottom(selectedEllipse->mapFromScene(lastMousePos).y());
    }
  } else if (moveEllipse) {
    emit relevantChange();
    selectedEllipse->resetTransform();
    QPointF newPos = event->scenePos();
    qreal newXDiff = newPos.x() - lastMousePos.x();
    qreal newYDiff = newPos.y() - lastMousePos.y();
    selectedEllipse->moveCenter(selectedEllipse->mapToScene(selectedEllipse->getCenter()) +
				selectedEllipse->mapFromScene(QPointF(newXDiff, newYDiff)));
    lastMousePos = event->scenePos();
    selectedEllipse->setRotationValue(selectedEllipse->getRotationValue());
  } else if (rotateEllipse) {
    emit relevantChange();
    lastMousePos = event->scenePos();
    QPointF center = selectedEllipse->mapToScene(selectedEllipse->getCenter());
    qreal dY = lastMousePos.y() - center.y();
    qreal dX = lastMousePos.x() - center.x();
    qreal angle = atan2(dY, dX);
    angle = qRadiansToDegrees(angle);
    selectedEllipse->setRotationValue(angle);
    emit relevantChange();
  } else if (manipulateRect) {
    lastMousePos = event->scenePos();
    QPointF topLeft = selectedRect->mapToScene(selectedRect->topLeft());
    QPointF bottomLeft = selectedRect->mapToScene(selectedRect->bottomLeft());
    QPointF topRight = selectedRect->mapToScene(selectedRect->topRight());
    QPointF bottomRight = selectedRect->mapToScene(selectedRect->bottomRight());
    QPointF left = selectedRect->mapToScene(QPointF(selectedRect->getLeft(),
						       selectedRect->getCenter().y()));
    QPointF right = selectedRect->mapToScene(QPointF(selectedRect->getRight(),
							selectedRect->getCenter().y()));
    QPointF top = selectedRect->mapToScene(QPointF(selectedRect->getCenter().x(),
						      selectedRect->getTop()));
    QPointF bottom = selectedRect->mapToScene(QPointF(selectedRect->getCenter().x(),
							 selectedRect->getBottom()));
    qreal distTopLeft = sqrt(pow((lastMousePos.x() - topLeft.x()), 2) +
			     pow((lastMousePos.y() - topLeft.y()), 2));
    qreal distTopRight = sqrt(pow((lastMousePos.x() - topRight.x()), 2) +
			      pow((lastMousePos.y() - topRight.y()), 2));
    qreal distBottomLeft = sqrt(pow((lastMousePos.x() - bottomLeft.x()), 2) +
				pow((lastMousePos.y() - bottomLeft.y()), 2));
    qreal distBottomRight = sqrt(pow((lastMousePos.x() - bottomRight.x()), 2) +
				 pow((lastMousePos.y() - bottomRight.y()), 2));
    qreal distLeft = sqrt(pow((lastMousePos.x() - left.x()), 2) +
			  pow((lastMousePos.y() - left.y()), 2));
    qreal distRight = sqrt(pow((lastMousePos.x() - right.x()), 2) +
			   pow((lastMousePos.y() - right.y()), 2));
    qreal distTop = sqrt(pow((lastMousePos.x() - top.x()), 2) +
			 pow((lastMousePos.y() - top.y()), 2));
    qreal distBottom = sqrt(pow((lastMousePos.x() - bottom.x()), 2) +
			    pow((lastMousePos.y() - bottom.y()), 2));
    qreal minimum = std::min({distTopLeft, distTopRight, distBottomLeft, distBottomRight,
	  distLeft, distRight, distTop, distBottom});
    if (minimum == distTopLeft) {
      selectedRect->setTopLeft(selectedRect->mapFromScene(lastMousePos));
    } else if (minimum == distTopRight) {
      selectedRect->setTopRight(selectedRect->mapFromScene(lastMousePos));
    } else if (minimum == distBottomLeft) {
      selectedRect->setBottomLeft(selectedRect->mapFromScene(lastMousePos));
    } else if (minimum == distBottomRight) {
      selectedRect->setBottomRight(selectedRect->mapFromScene(lastMousePos));
    } else if (minimum == distLeft) {
      selectedRect->setLeft(selectedRect->mapFromScene(lastMousePos).x());
    } else if (minimum == distRight) {
      selectedRect->setRight(selectedRect->mapFromScene(lastMousePos).x());
    } else if (minimum == distTop) {
      selectedRect->setTop(selectedRect->mapFromScene(lastMousePos).y());
    } else if (minimum == distBottom) {
      selectedRect->setBottom(selectedRect->mapFromScene(lastMousePos).y());
    }
  } else if (moveRect) {
    emit relevantChange();
    selectedRect->resetTransform();
    QPointF newPos = event->scenePos();
    qreal newXDiff = newPos.x() - lastMousePos.x();
    qreal newYDiff = newPos.y() - lastMousePos.y();
    selectedRect->moveCenter(selectedRect->mapToScene(selectedRect->getCenter()) +
			     selectedRect->mapFromScene(QPointF(newXDiff, newYDiff)));
    lastMousePos = event->scenePos();
    selectedRect->setRotationValue(selectedRect->getRotationValue());
  } else if (rotateRect) {
    emit relevantChange();
    lastMousePos = event->scenePos();
    QPointF center = selectedRect->mapToScene(selectedRect->getCenter());
    qreal dY = lastMousePos.y() - center.y();
    qreal dX = lastMousePos.x() - center.x();
    qreal angle = atan2(dY, dX);
    angle = qRadiansToDegrees(angle);
    selectedRect->setRotationValue(angle);
  } else if (rotateText) {
    emit relevantChange();
    lastMousePos = event->scenePos();
    QPointF center = selectedText->mapToScene(selectedText->getCenter());
    qreal dY = center.y() - lastMousePos.y();
    qreal dX = center.x() - lastMousePos.x();
    qreal angle = atan2(dY, dX);
    angle = qRadiansToDegrees(angle);
    selectedText->setRotationValue(angle);
    emit relevantChange();
  } else {
    if (selectedItems().size() > 1 && moveOn) {
      if (selectedEvent) {
	emit relevantChange();
 	emit moveItems(selectedEvent, event->scenePos());
      } else if (selectedMacro) {
	emit relevantChange();
	emit moveItems(selectedMacro, event->scenePos());
      } else if (selectedNode) {
	emit relevantChange();
	emit moveItems(selectedNode, event->scenePos());
      } else {
	moveOn = false;
      }
    } else {
      if (selectedOccurrence && moveOn) {
	emit relevantChange();
	emit moveLine(selectedOccurrence, event->scenePos());
      } else if (selectedOccurrence && !moveOn) {
	emit relevantChange();
	emit moveItems(selectedOccurrence, event->scenePos());
      }
    }
    GraphicsView *view = qobject_cast<GraphicsView*>(views()[0]);
    if (view) {
      if (!view->isPanning()) {
	QApplication::restoreOverrideCursor();
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
  LineObject *line = qgraphicsitem_cast<LineObject*>(itemAt(event->scenePos(), QTransform()));
  TextObject *text = qgraphicsitem_cast<TextObject*>(itemAt(event->scenePos(), QTransform()));
  EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(itemAt(event->scenePos(), QTransform()));
  RectObject *rect = qgraphicsitem_cast<RectObject*>(itemAt(event->scenePos(), QTransform()));
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
    QAction *action12 = new QAction(COLLIGATEPATHSACTIONATT, this);
    menu.addAction(action12);    
    QAction *action2 = new QAction(COLLIGATESEMIPATHSACTION, this);
    menu.addAction(action2);
    QAction *action13 = new QAction(COLLIGATESEMIPATHSACTIONATT, this);
    menu.addAction(action13);
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
    QAction *action14 = new QAction(ADDLINKAGE, this);
    menu.addAction(action14);
    if (selectedItems().size() > 1) {
      action3->setEnabled(false);
      action6->setEnabled(false);
      action7->setEnabled(false);
      action9->setEnabled(false);
      action10->setEnabled(false);
    }
    if (selectedItems().size() == 1) {
      action1->setEnabled(false);
      action12->setEnabled(false);
      action13->setEnabled(false);
      action2->setEnabled(false);
      action8->setEnabled(false);
    }
    action14->setEnabled(false);
    if (selectedItems().size() == 2) {
      EventItem *eventOne = qgraphicsitem_cast<EventItem*>(selectedItems()[0]);
      EventItem *eventTwo = qgraphicsitem_cast<EventItem*>(selectedItems()[1]);
      if (eventOne && eventTwo) {
	action14->setEnabled(true);
      }
    }
    if (QAction *action = menu.exec(event->screenPos())) {
      emit EventItemContextMenuAction(action->text());
    }
    // And then we'll capture some action, and send a signal to the main widget.
  } else if (macro && !macro->isCopy()) {
    QMenu menu;
    QAction *action1 = new QAction(COLLIGATEPATHSACTION, this);
    menu.addAction(action1);
    QAction *action12 = new QAction(COLLIGATEPATHSACTIONATT, this);
    menu.addAction(action12);
    QAction *action2 = new QAction(COLLIGATESEMIPATHSACTION, this);
    menu.addAction(action2);
    QAction *action13 = new QAction(COLLIGATESEMIPATHSACTIONATT, this);
    menu.addAction(action13);
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
      action12->setEnabled(false);
      action13->setEnabled(false);
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
    QAction *action1 = new QAction(HIDENODE, this);
    menu.addAction(action1);
    QAction *action2 = new QAction(SETPERSISTENT, this);
    menu.addAction(action2);
    QAction *action3 = new QAction(UNSETPERSISTENT, this);
    menu.addAction(action3);
    if (node->isPersistent()) {
      action2->setEnabled(false);
      action3->setEnabled(true);
    } else {
      action2->setEnabled(true);
      action3->setEnabled(false);
    }
    if (QAction *action = menu.exec(event->screenPos())) {
      emit NetworkNodeContextMenuAction(action->text());
    }
  } else if (line) {
    clearSelection();
    line->setSelected(true);
    QMenu menu;
    QAction *action1 = new QAction(CHANGELINECOLOR, this);
    menu.addAction(action1);
    QAction *action2 = new QAction(TOGGLEARROW1, this);
    menu.addAction(action2);
    QAction *action3 = new QAction(TOGGLEARROW2, this);
    menu.addAction(action3);
    QAction *action4 = new QAction(DELETELINE, this);
    menu.addAction(action4);
    if (QAction *action = menu.exec(event->screenPos())) {
      emit LineContextMenuAction(action->text());
    }
  } else if (text) {
    clearSelection();
    text->setSelected(true);
    QMenu menu;
    QAction *action1 = new QAction(CHANGETEXT, this);
    menu.addAction(action1);
    QAction *action2 = new QAction(CHANGETEXTCOLOR, this);
    menu.addAction(action2);
    QAction *action3 = new QAction(DELETETEXT, this);
    menu.addAction(action3);
    if (QAction *action = menu.exec(event->screenPos())) {
      emit TextContextMenuAction(action->text());
    }
  } else if (ellipse) {
    clearSelection();
    ellipse->setSelected(true);
    QMenu menu;
    QAction *action1 = new QAction(CHANGEELLIPSECOLOR, this);
    menu.addAction(action1);
    QAction *action2 = new QAction(DELETEELLIPSE, this);
    menu.addAction(action2);
    if (QAction *action = menu.exec(event->screenPos())) {
      emit EllipseContextMenuAction(action->text());
    }
  } else if (rect) {
    clearSelection();
    rect->setSelected(true);
    QMenu menu;
    QAction *action1 = new QAction(CHANGERECTCOLOR, this);
    menu.addAction(action1);
    QAction *action2 = new QAction(DELETERECT, this);
    menu.addAction(action2);
    if (QAction *action = menu.exec(event->screenPos())) {
      emit RectContextMenuAction(action->text());
    }
  }
}

OccurrenceItem* Scene::getSelectedOccurrence() {
  return selectedOccurrence;
}
