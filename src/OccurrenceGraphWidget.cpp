#include "../include/OccurrenceGraphWidget.h"

/*
  Notes:
  I want to fetch the macro events from saved event graph plots later,
  and then group occurrence in the events that are visible.
*/

bool occurrenceLessThan(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) {
  qreal orderOne = itemOne->scenePos().x();
  qreal orderTwo = itemTwo->scenePos().x();
  if (orderOne < orderTwo) {
    return true;
  } else {
    return false;
  }
}

bool attributeLessThan(const OccurrenceItem *itemOne, const OccurrenceItem *itemTwo) {
  QString attributeOne = itemOne->getAttribute();
  QString attributeTwo = itemTwo->getAttribute();
  if (attributeOne < attributeTwo) {
    return true;
  } else {
    return false;
  }
}

bool stringSort(const QString one, const QString two) {
  if (one < two) {
    return true;
  } else {
    return false;
  }
}

OccurrenceGraphWidget::OccurrenceGraphWidget(QWidget *parent) : QWidget(parent) {
  distance = 70;
  labelsVisible = true;
  
  scene = new Scene(this);
  view = new GraphicsView(scene);
  view->setDragMode(QGraphicsView::RubberBandDrag);
  view->setRubberBandSelectionMode(Qt::ContainsItemShape);
  view->viewport()->installEventFilter(this);
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
  view->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

  legendWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
  
  attributeListWidget = new DeselectableListWidget(legendWidget);
  attributeListWidget->setColumnCount(2);
  attributeListWidget->horizontalHeader()->hide();
  attributeListWidget->verticalHeader()->hide();
  attributeListWidget->setColumnWidth(1, 20);
  attributeListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  attributeListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  attributeListWidget->setStyleSheet("QTableView {gridline-color: black}");
  attributeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  
  lowerRangeDial = new QDial(graphicsWidget);
  lowerRangeDial->setEnabled(false);
  lowerRangeDial->setSingleStep(1);
  upperRangeDial = new QDial(graphicsWidget);
  upperRangeDial->setSingleStep(1);
  upperRangeDial->setEnabled(false);
  lowerRangeSpinBox = new QSpinBox(graphicsWidget);
  lowerRangeSpinBox->setEnabled(false);
  upperRangeSpinBox = new QSpinBox(graphicsWidget);
  upperRangeSpinBox->setEnabled(false);
  
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  toggleLegendButton->setChecked(true);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  addAttributeButton = new QPushButton(tr("Add to plot"), legendWidget);
  removeModeButton = new QPushButton(tr("Remove mode"), legendWidget);
  removeModeButton->setEnabled(false);
  getEventsButton = new QPushButton(tr("Match event graph"), this);
  plotLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  increaseDistanceButton = new QPushButton(tr("< >"), this);
  decreaseDistanceButton = new QPushButton(tr("> <"), this);
  exportSvgButton = new QPushButton(tr("Export"), graphicsWidget);
  
  // connections
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(attributeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setModeButton(QTableWidgetItem *)));
  connect(attributeListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableModeButton()));
  connect(removeModeButton, SIGNAL(clicked()), this, SLOT(removeMode()));
  connect(getEventsButton, SIGNAL(clicked()), this, SLOT(getEvents()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(plotLabelsButton, SIGNAL(clicked()), this, SLOT(plotLabels()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
    
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  /*  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(coderLabel);
  plotOptionsLayout->addWidget(coderComboBox);
  plotOptionsLayout->addWidget(typeLabel);
  plotOptionsLayout->addWidget(typeComboBox);
  plotOptionsLayout->addWidget(plotButton);
  plotOptionsLayout->addWidget(savePlotButton);
  plotOptionsLayout->addWidget(seePlotsButton);
  plotOptionsLayout->addWidget(plotLabel);
  plotOptionsLayout->addWidget(changeLabel);
  plotOptionsLayout->addWidget(incongruencyLabel);
  plotOptionsLayout->addSpacerItem(new QSpacerItem(100,0));
  topLayout->addLayout(plotOptionsLayout);
  plotOptionsLayout->setAlignment(Qt::AlignLeft);*/

  QPointer<QHBoxLayout> screenLayout = new QHBoxLayout;

  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  leftLayout->addWidget(view);
  view->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
  screenLayout->addLayout(leftLayout);

  QPointer<QVBoxLayout> legendLayout = new QVBoxLayout;
  legendLayout->addWidget(attributeListWidget);
  legendLayout->addWidget(addAttributeButton);
  legendLayout->addWidget(removeModeButton);
  legendWidget->setMinimumWidth(175);
  legendWidget->setMaximumWidth(175);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				   
  
  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(backgroundColorButton);
  graphicsControlsLayout->addWidget(plotLabelsButton);
  graphicsControlsLayout->addWidget(upperRangeLabel);
  upperRangeLabel->setAlignment(Qt::AlignHCenter);
  QPointer<QHBoxLayout> upperRangeLayout = new QHBoxLayout;
  upperRangeLayout->addWidget(upperRangeDial);
  upperRangeLayout->addWidget(upperRangeSpinBox);
  graphicsControlsLayout->addLayout(upperRangeLayout);
  graphicsControlsLayout->addWidget(lowerRangeLabel);
  lowerRangeLabel->setAlignment(Qt::AlignHCenter);
  QPointer<QHBoxLayout> lowerRangeLayout = new QHBoxLayout;
  lowerRangeLayout->addWidget(lowerRangeDial);
  lowerRangeLayout->addWidget(lowerRangeSpinBox);
  graphicsControlsLayout->addLayout(lowerRangeLayout);
  graphicsControlsLayout->addWidget(exportSvgButton);
  graphicsWidget->setMaximumWidth(175);
  graphicsWidget->setMinimumWidth(175);
  graphicsWidget->setLayout(graphicsControlsLayout);
  graphicsControlsLayout->setAlignment(Qt::AlignBottom);
  screenLayout->addWidget(graphicsWidget);
  
  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  drawOptionsLeftLayout->addWidget(increaseDistanceButton);
  drawOptionsLeftLayout->addWidget(decreaseDistanceButton);
  increaseDistanceButton->setMaximumWidth(increaseDistanceButton->sizeHint().width());
  decreaseDistanceButton->setMaximumWidth(decreaseDistanceButton->sizeHint().width());
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(getEventsButton);
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);
    
  setLayout(mainLayout);
  graphicsWidget->hide();
}

void OccurrenceGraphWidget::toggleLegend() {
  if (legendWidget->isVisible()) {
    legendWidget->hide();
  } else {
    legendWidget->show();
  }
}

void OccurrenceGraphWidget::toggleGraphicsControls() {
  if (graphicsWidget->isVisible()) {
    graphicsWidget->hide();
  } else {
    graphicsWidget->show();
  }
}

void OccurrenceGraphWidget::addAttribute() {
  view->blockSignals(true);
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, INCIDENT);
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) {
    QColor color = attributeColorDialog->getColor();
    QColor textColor = attributeColorDialog->getTextColor();
    QString attribute = attributeColorDialog->getAttribute();
    if (!presentAttributes.contains(attribute)) {
      presentAttributes.push_back(attribute);
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT description FROM incident_attributes "
		     "WHERE name = :name");
      query->bindValue(":name", attribute);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QVector<QString> attributeVector;
      attributeVector.push_back(attribute);
      findChildren(attribute, &attributeVector);
      QVectorIterator<QString> it(attributeVector);
      QVector<OccurrenceItem*> tempVec;
      QVector<int> orders;
      while (it.hasNext()) {
	QString currentAttribute = it.next();
	query->prepare("SELECT incident FROM attributes_to_incidents "
		       "WHERE attribute = :currentAttribute");
	query->bindValue(":currentAttribute", currentAttribute);
	query->exec();
	while (query->next()) {
	  int currentIncident = query->value(0).toInt();
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT ch_order, description FROM incidents WHERE id = :id");
	  query2->bindValue(":id", currentIncident);
	  query2->exec();
	  query2->first();
	  int order = query2->value(0).toInt();
	  QString incidentDescription = query2->value(1).toString();
	  if (!orders.contains(order)) {
	    orders.push_back(order);
	    delete query2;
	    QPointF position = QPointF((order * distance), 0);
	    OccurrenceItem *newOccurrence = new OccurrenceItem(40, incidentDescription, position,
							       currentIncident, order, attribute);
	    newOccurrence->setPos(newOccurrence->getOriginalPos());
	    newOccurrence->setColor(color);
	    newOccurrence->setZValue(1);
	    occurrenceVector.push_back(newOccurrence);
	    tempVec.push_back(newOccurrence);
	    scene->addItem(newOccurrence);
	    OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
	    QString text = QString::number(order) + " - " + attribute;
	    label->setPlainText(text);
	    label->setDefaultTextColor(textColor);
	    label->setTextWidth(label->boundingRect().width());
	    label->setNewPos(newOccurrence->scenePos());
	    labelVector.push_back(label);
	    newOccurrence->setLabel(label);
	    label->setZValue(2);
	    labelVector.push_back(label);
	    scene->addItem(label);
	  }
	}
      }
      bool found = false;
      for (int i = 0; i < attributeListWidget->rowCount(); i++) {
	if (attributeListWidget->item(i, 0)->data(Qt::DisplayRole) == attribute) {
	  found = true;
	  QTableWidgetItem *item = attributeListWidget->item(i,0);
	  QString toolTip = "<FONT SIZE = 3>" + attribute + " - " + description + "</FONT SIZE>";
	  item->setToolTip(toolTip);
	  attributeListWidget->item(i, 1)->setBackground(color);
	  break;
	}
      }
      if (!found) {
	QTableWidgetItem *item = new QTableWidgetItem(attribute);
	item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	QString toolTip = "<FONT SIZE = 3>" + attribute + " - " + description + "</FONT SIZE>";
	item->setToolTip(toolTip);
	item->setData(Qt::DisplayRole, attribute);
	attributeListWidget->setRowCount(attributeListWidget->rowCount() + 1);
	attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 0, item);
	attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->setBackground(color);
	attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->
	  setFlags(attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->flags() ^
		   Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
      }
      delete query;
      groupOccurrences();
      wireLinkages();
    }
  }
  delete attributeColorDialog;
  view->blockSignals(false);
  setRangeControls();
}

void OccurrenceGraphWidget::findChildren(QString father, QVector<QString> *children) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM incident_attributes WHERE father = :father");
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) {
    QString currentChild = query->value(0).toString();
    children->push_back(currentChild);
    findChildren(currentChild, children);
  }
  delete query;
}

void OccurrenceGraphWidget::removeMode() {
  //setChangeLabel();
  QString text = attributeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  for (QVector<OccurrenceItem*>::iterator it = occurrenceVector.begin();
       it != occurrenceVector.end();) {
    OccurrenceItem *current = *it;
    if (current->getAttribute() == text) {
      scene->removeItem(current->getLabel());
      labelVector.removeOne(current->getLabel());
      scene->removeItem(current);
      occurrenceVector.removeOne(current);
    } else {
      it++;
    }
  }
  for (int i = 0; i != attributeListWidget->rowCount();) {
    if (attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text) {
      attributeListWidget->removeRow(i);
    }
    if (i != attributeListWidget->rowCount()) {
      i++;
    }
  }
  wireLinkages();
}

void OccurrenceGraphWidget::disableModeButton() {
  removeModeButton->setEnabled(false);
}

void OccurrenceGraphWidget::setModeButton(QTableWidgetItem *item) {
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") {
    removeModeButton->setEnabled(true);
  } else {
    removeModeButton->setEnabled(false);
  }
}

void OccurrenceGraphWidget::groupOccurrences() {
  qSort(presentAttributes.begin(), presentAttributes.end(), stringSort);
  QVectorIterator<OccurrenceItem*> it(occurrenceVector);
  while (it.hasNext()) {
    OccurrenceItem *current = it.next();
    QVectorIterator<QString> it2(presentAttributes);
    int dist = 0;
    while (it2.hasNext()) {
      QString text = it2.next();
      if (text == current->getAttribute()) {
	current->setPos(current->scenePos().x(), dist);
	current->getLabel()->setNewPos(current->scenePos());
	break;
      } else {
	dist -= 250;
      }
    }
  }
  QVectorIterator<OccurrenceItem*> it3(occurrenceVector);
  while (it3.hasNext()) {
    OccurrenceItem *first = it3.next();
    QVectorIterator<OccurrenceItem*> it4(occurrenceVector);
    QVector<OccurrenceItem*> temp;
    if (!temp.contains(first)) {
      temp.push_back(first);
      QVector<OccurrenceItem*> rem;
      while (it4.hasNext()) {
	OccurrenceItem *second = it4.next();
	if (first != second && second->scenePos().x() == first->scenePos().x()) {
	  if (second->getAttribute() != first->getAttribute()) {
	    temp.push_back(second);
	  } else {
	    rem.push_back(second);
	  }
	}
      }
      QVectorIterator<OccurrenceItem*> it5(rem);
      while (it5.hasNext()) {
	OccurrenceItem *candidate = it5.next();
	if (candidate != rem.first()) {
	  candidate->hide();
	  candidate->setPermHidden(true);
	  candidate->getLabel()->hide();
	}
      }
      qSort(temp.begin(), temp.end(), attributeLessThan);
      QVectorIterator<OccurrenceItem*> it6(temp);
      qreal x = temp.first()->scenePos().x();
      qreal startY = temp.first()->scenePos().y();
      int dist = 0;
      while (it6.hasNext()) {
	OccurrenceItem *current = it6.next();
	current->setPos(x, startY - dist);
	current->getLabel()->setNewPos(current->scenePos());
	dist += 80;
      }
    }
  }
}

void OccurrenceGraphWidget::wireLinkages() {
  qDeleteAll(edgeVector);
  edgeVector.clear();
  qSort(occurrenceVector.begin(), occurrenceVector.end(), occurrenceLessThan);
  QVectorIterator<OccurrenceItem*> it(occurrenceVector);
  while (it.hasNext()) {
    OccurrenceItem *tempSource = it.next();
    OccurrenceItem *tempTarget = NULL;
    if (tempSource->isVisible()) {
      QVectorIterator<OccurrenceItem*> it2(occurrenceVector);
      while (it2.hasNext()) {
	OccurrenceItem *temp = it2.next();
	if (temp->isVisible()) {
	  if (tempSource->getAttribute() == temp->getAttribute()) {
	    if (tempTarget == NULL && temp->scenePos().x() > tempSource->scenePos().x()) {
	      tempTarget = temp;
	    } else {
	      if (temp->scenePos().x() > tempSource->scenePos().x() &&
		  temp->scenePos().x() < tempTarget->scenePos().x() &&
		  tempSource->getAttribute() == temp->getAttribute()) {
		tempTarget = temp;
	      }
	    }
	  }
	}
      }
      if (tempTarget != NULL) {
	Arrow *newArrow = new Arrow(tempSource, tempTarget, tempSource->getAttribute(), "");
	edgeVector.push_back(newArrow);
	scene->addItem(newArrow);
      }
    }
  }
}

void OccurrenceGraphWidget::getEvents() {
  QVector<EventItem*> incidents = eventGraph->getEventItems();
  if (incidents.size() > 0) {
    QVectorIterator<EventItem*> it(incidents);
    while (it.hasNext()) {
      EventItem *incident = it.next();
      QVectorIterator<OccurrenceItem*> it2(occurrenceVector);
      while (it2.hasNext()) {
	OccurrenceItem *occurrence = it2.next();
	occurrence->setPermHidden(false); // We reset this here.
	if (incident->getId() == occurrence->getId()) {
	  if (incident->getMacroEvent() != NULL) {
	    occurrence->setPos(incident->getMacroEvent()->scenePos().x() +
			       (incident->getMacroEvent()->getWidth() / 2),
			       occurrence->scenePos().y());
	    occurrence->getLabel()->setNewPos(occurrence->scenePos());
	  } else {
	    occurrence->setPos(incident->scenePos().x(), occurrence->scenePos().y());
	    occurrence->getLabel()->setNewPos(occurrence->scenePos());
	  }
	}
      }
    }
    groupOccurrences();
    wireLinkages();
  }
}


void OccurrenceGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) {
  OccurrenceItem *source = NULL;
  QVectorIterator<OccurrenceItem*> it(occurrenceVector);
  while (it.hasNext()) {
    OccurrenceItem *temp = it.next();
    if (temp == item) {
      source = temp;
    }
  }
  if (source != NULL) {
    qreal currentY = source->scenePos().y();
    qreal newY = pos.y();
    qreal yDiff = newY - currentY;
    
    QVectorIterator<OccurrenceItem*> it2(occurrenceVector);
    while (it2.hasNext()) {
      OccurrenceItem *current = it2.next();
      if (current->scenePos().x() == source->scenePos().x()) {
	current->setPos(current->scenePos().x(), current->scenePos().y() + yDiff);
	current->getLabel()->setNewPos(current->scenePos());
      }
    }
  }
}

void OccurrenceGraphWidget::plotLabels() {
  QVectorIterator<OccurrenceLabel*> it(labelVector);
  while (it.hasNext()) {
    OccurrenceLabel *currentItem = it.next();
    OccurrenceItem *currentOccurrence = currentItem->getOccurrence();
    if (currentOccurrence->isVisible()) {
      if (labelsVisible) {
	currentOccurrence->hide();
      } else {
	currentOccurrence->show();
      }
    }
  }
  labelsVisible = !(labelsVisible);
}

void OccurrenceGraphWidget::setBackgroundColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    QColor color = colorDialog->selectedColor();
    view->setBackgroundBrush(color);
  }
  delete colorDialog;
}

void OccurrenceGraphWidget::increaseDistance() {
  qSort(occurrenceVector.begin(), occurrenceVector.end(), occurrenceLessThan);
  QVectorIterator<OccurrenceItem*> it(occurrenceVector);  
  int unit = 0;
  qreal last = -9999;
  if (distance < 300) {
    while (it.hasNext()) {
      OccurrenceItem *current = it.next();
      qreal oldX = current->scenePos().x();
      if (last == -9999) {
	last = current->scenePos().x();
      }
      if (last != oldX) {
	unit++;
      }
      QPointF newPos = QPointF(oldX + unit, current->pos().y());
      current->getLabel()->setNewPos(newPos);
      current->setOriginalPos(newPos);
      current->setPos(newPos);
      last = oldX;
    }
  }
  distance++;
}

void OccurrenceGraphWidget::decreaseDistance() {
  qSort(occurrenceVector.begin(), occurrenceVector.end(), occurrenceLessThan);
  QVectorIterator<OccurrenceItem*> it(occurrenceVector);  
  int unit = 0;
  qreal last = -9999;
  if (distance > 40) {
    while (it.hasNext()) {
      OccurrenceItem *current = it.next();
      qreal oldX = current->scenePos().x();
      if (last == -9999) {
	last = current->scenePos().x();
      }
      if (last != oldX) {
	unit++;
      }      
      QPointF newPos = QPointF(oldX - unit, current->pos().y());
      current->getLabel()->setNewPos(newPos);
      current->setOriginalPos(newPos);
      current->setPos(newPos);
      last = oldX;
    }
  }
  distance--;
}

void OccurrenceGraphWidget::setRangeControls() {
  lowerRangeDial->setEnabled(true);
  upperRangeDial->setEnabled(true);
  lowerRangeSpinBox->setEnabled(true);
  upperRangeSpinBox->setEnabled(true);
  lowerRangeDial->setRange(1, occurrenceVector.size() - 1);
  upperRangeDial->setRange(2, occurrenceVector.size());
  lowerRangeSpinBox->setRange(1, occurrenceVector.size() - 1);
  upperRangeSpinBox->setRange(2, occurrenceVector.size());
  upperRangeDial->setValue(occurrenceVector.size());
  upperRangeSpinBox->setValue(occurrenceVector.size());
}

void OccurrenceGraphWidget::processLowerRange(int value) {
  lowerRangeDial->setValue(value);
  lowerRangeSpinBox->setValue(value);
  if (upperRangeDial->value() <= value) {
    upperRangeDial->setValue(value + 1);
    upperRangeSpinBox->setValue(value + 1);
  }
  setVisibility();
}
 
void OccurrenceGraphWidget::processUpperRange(int value) {
  upperRangeDial->setValue(value);
  upperRangeSpinBox->setValue(value);
  if (lowerRangeDial->value() >= value) {
    lowerRangeDial->setValue(value - 1);
    lowerRangeSpinBox->setValue(value - 1);
  }
  setVisibility();
} 

void OccurrenceGraphWidget::setVisibility() {
  QVectorIterator<OccurrenceItem*> it(occurrenceVector);
  int counter = 0;
  while (it.hasNext()) {
    counter++;
    OccurrenceItem* currentItem = it.next();
    if (currentItem->isPermHidden()) {
      currentItem->hide();
    } else {
      if (counter >= lowerRangeDial->value() && counter <= upperRangeDial->value()) {
	currentItem->show();
      }  else {
	currentItem->hide();
      }
    }
  }
  QVectorIterator<Arrow*> it2(edgeVector);
  while (it2.hasNext()) {
    Arrow *currentEdge = it2.next();
    OccurrenceItem *tail = qgraphicsitem_cast<OccurrenceItem*>(currentEdge->startItem());
    OccurrenceItem *head = qgraphicsitem_cast<OccurrenceItem*>(currentEdge->endItem());
    bool show = true;
    if (tail) {
      if(!tail->isVisible()) {
	show = false;
      }
      if (head) {
	if (!head->isVisible()) {
	  show = false;
	}
      }
      if (!show) {
	currentEdge->hide();
      } else {
	currentEdge->show();
      }
    }
  }
  QVectorIterator<OccurrenceLabel*> it3(labelVector);
  while (it3.hasNext()) {
    OccurrenceLabel *currentText = it3.next();
    OccurrenceItem *currentParent = currentText->getOccurrence();
    if (!(currentParent->isVisible())) {
      currentText->hide();
    } else {
      currentText->show();
    }
  }
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
}

void OccurrenceGraphWidget::exportSvg() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("New svg file"),"", tr("svg files (*.)"));
  if (!fileName.trimmed().isEmpty()) {
    if (!fileName.endsWith(".svg")) {
      fileName.append(".svg");
    }
    QSvgGenerator gen;
    gen.setFileName(fileName);
    QRectF currentRect = this->scene->itemsBoundingRect();
    currentRect.setX(currentRect.x());
    currentRect.setY(currentRect.y());
    currentRect.setWidth(currentRect.width());
    currentRect.setHeight(currentRect.height());
    gen.setSize(QSize(currentRect.width(), currentRect.height()));
    QPainter painter;
    painter.begin(&gen);
    scene->render(&painter);
    painter.end();
  }
}

void OccurrenceGraphWidget::setEventGraph(EventGraphWidget *egw) {
  eventGraph = egw;
}

void OccurrenceGraphWidget::cleanUp() {
  scene->clearSelection();
  qDeleteAll(edgeVector);
  edgeVector.clear();
  qDeleteAll(occurrenceVector);
  occurrenceVector.clear();
  qDeleteAll(labelVector);
  labelVector.clear();
}

void OccurrenceGraphWidget::finalBusiness() {
  cleanUp();
}
