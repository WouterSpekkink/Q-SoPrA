#include "../include/OccurrenceGraphWidget.h"

/*
  Notes:
  I want to fetch the macro events from saved event graph plots later,
  and then group occurrence in the events that are visible.
*/

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

  legendLabel = new QLabel(tr("<b>Legend:</b>"), this);
  plotLabel = new QLabel(tr("Unsaved plot"), this);
  changeLabel = new QLabel(tr("*"), this);
  incongruencyLabel = new QLabel(tr(""), this);
  incongruencyLabel->setStyleSheet("QLabel {color : red;}");
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
  removeModeButton = new QPushButton(tr("Remove from plot"), legendWidget);
  removeModeButton->setEnabled(false);
  getEventsButton = new QPushButton(tr("Match current event graph"), this);
  restoreButton = new QPushButton(tr("Restore to original"), this);
  plotLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  increaseDistanceButton = new QPushButton(tr("< >"), this);
  decreaseDistanceButton = new QPushButton(tr("> <"), this);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  exportMatrixButton = new QPushButton(tr("Export matrix"), graphicsWidget);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  
  // connections
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(attributeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setModeButton(QTableWidgetItem *)));
  connect(attributeListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableModeButton()));
  connect(attributeListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeModeColor(QTableWidgetItem *)));
  connect(removeModeButton, SIGNAL(clicked()), this, SLOT(removeMode()));
  connect(getEventsButton, SIGNAL(clicked()), this, SLOT(getEvents()));
  connect(restoreButton, SIGNAL(clicked()), this, SLOT(restore()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(scene, SIGNAL(moveLine(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveLine(QGraphicsItem *, QPointF)));
  connect(plotLabelsButton, SIGNAL(clicked()), this, SLOT(plotLabels()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(exportMatrixButton, SIGNAL(clicked()), this, SLOT(exportMatrix()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
    
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(savePlotButton);
  plotOptionsLayout->addWidget(seePlotsButton);
  plotOptionsLayout->addWidget(plotLabel);
  plotOptionsLayout->addWidget(changeLabel);
  plotOptionsLayout->addWidget(incongruencyLabel);
  plotOptionsLayout->addSpacerItem(new QSpacerItem(100,0));
  topLayout->addLayout(plotOptionsLayout);
  plotOptionsLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(topLayout);

  QPointer<QHBoxLayout> screenLayout = new QHBoxLayout;

  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  leftLayout->addWidget(view);
  view->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
  screenLayout->addLayout(leftLayout);

  QPointer<QVBoxLayout> legendLayout = new QVBoxLayout;
  legendLayout->addWidget(legendLabel);
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
  QPointer<QFrame> sepLine = new QFrame();
  sepLine->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine);
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
  QPointer<QFrame> sepLine2 = new QFrame();
  sepLine2->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine2);
  graphicsControlsLayout->addWidget(exportSvgButton);
  graphicsControlsLayout->addWidget(exportMatrixButton);
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
  drawOptionsRightLayout->addWidget(restoreButton);
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);

  setLayout(mainLayout);
  graphicsWidget->hide();
}

void OccurrenceGraphWidget::checkCongruency() {
  qApp->setOverrideCursor(Qt::WaitCursor);
  if (occurrenceVector.size() > 0) {
    QSqlQuery *query = new QSqlQuery;
    QVectorIterator<OccurrenceItem*> it(occurrenceVector);
    while (it.hasNext()) {
      OccurrenceItem* current = it.next();
      int id = current->getId();
      if (id >= 0) {
	int order = current->getOrder();
	QString attribute = current->getAttribute();
	QVector<QString> attributeVector;
	attributeVector.push_back(attribute);
	findChildren(attribute, &attributeVector);
	QVectorIterator<QString> it2(attributeVector);
	bool found = false;
	while (it2.hasNext()) {
	  QString currentAttribute = it2.next();
	  query->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			 "WHERE attribute = :attribute AND incident = :incident");
	  query->bindValue(":attribute", currentAttribute);
	  query->bindValue(":incident", id);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) {
	    found = true;
	  }
	}
	if (!found) {
	  incongruencyLabel->setText("Incongruency detected");
	  qApp->restoreOverrideCursor();
	  qApp->processEvents();
	  return;
	}
	query->prepare("SELECT ch_order FROM incidents "
		       "WHERE id = :incident");
	query->bindValue(":incident", id);
	query->exec();
	query->first();
	if (query->isNull(0)) {
	  incongruencyLabel->setText("Incongruency detected");
	  qApp->restoreOverrideCursor();
	  qApp->processEvents();
	  return;
	} else if (query->value(0).toInt() != order) {
	  incongruencyLabel->setText("Incongruency detected");
	  qApp->restoreOverrideCursor();
	  qApp->processEvents();
	  return;
	}
      }
    }
    for (int i = 0; i != attributeListWidget->rowCount(); i++) {
      QString text = attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
      query->prepare("SELECT incident FROM attributes_to_incident "
		     "WHERE attribute = :attribute");
      query->bindValue(":attribute", text);
      query->exec();
      while (query->next()) {
	int current = query->value(0).toInt();
	bool found = false;
	QVectorIterator<OccurrenceItem*> it2(occurrenceVector);
	while (it2.hasNext()) {
	  OccurrenceItem *item = it2.next();
	  if (item->getId() == current &&
	      item->getAttribute() == text) {
	    found = true;
	    break;
	  }
	}
	if (!found) {
	  incongruencyLabel->setText("Incongruency detected");
	  qApp->restoreOverrideCursor();
	  qApp->processEvents();
	  return;
	}
      }
    }
    delete query;
    incongruencyLabel->setText("");
  }
  qApp->restoreOverrideCursor();
  qApp->processEvents();
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
  setChangeLabel();
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, INCIDENT);
  attributeColorDialog->setWindowTitle("Add attribute to plot");
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) {
    reset();
    QColor color = attributeColorDialog->getColor();
    QColor textColor = attributeColorDialog->getTextColor();
    QString attribute = attributeColorDialog->getAttribute();
    if (!presentAttributes.contains(attribute)) {
      presentAttributes.push_back(attribute);
      savePlotButton->setEnabled(true);
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
	    scene->addItem(label);
	  }
	}
      }
      bool found = false;
      for (int i = 0; i < attributeListWidget->rowCount(); i++) {
	if (attributeListWidget->item(i, 0)->data(Qt::DisplayRole) == attribute) {
	  found = true;
	  QTableWidgetItem *item = attributeListWidget->item(i,0);
	  QString toolTip = breakString(attribute + " - " + description);
	  item->setToolTip(toolTip);
	  attributeListWidget->item(i, 1)->setBackground(color);
	  break;
	}
      }
      if (!found) {
	QTableWidgetItem *item = new QTableWidgetItem(attribute);
	item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	QString toolTip = breakString(attribute + " - " + description);
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
  setRangeControls();
  scene->update();
  view->update();
  checkCongruency();
}

void OccurrenceGraphWidget::setChangeLabel() {
  if (changeLabel->text() == "" && occurrenceVector.size() > 0) {
    changeLabel->setText("*");
  }
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
  setChangeLabel();
  QString text = attributeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  for (QVector<OccurrenceItem*>::iterator it = occurrenceVector.begin();
       it != occurrenceVector.end();) {
    OccurrenceItem *current = *it;
    if (current->getAttribute() == text) {
      scene->removeItem(current->getLabel());
      labelVector.removeOne(current->getLabel());
      scene->removeItem(current);
      occurrenceVector.removeOne(current);
      presentAttributes.removeOne(text);
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
  groupOccurrences();
  wireLinkages();
  if (presentAttributes.size() == 0) {
    savePlotButton->setEnabled(false);
  }
  removeModeButton->setEnabled(false);
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

void OccurrenceGraphWidget::changeModeColor(QTableWidgetItem *item) {
  if (item->column() == 1) {
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    colorDialog->setCurrentColor(item->background().color());
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    if (colorDialog->exec()) {
      QColor color = colorDialog->selectedColor();
      item->setBackground(color);
      QTableWidgetItem* neighbour = attributeListWidget->item(item->row(), 0);
      QString attribute = neighbour->data(Qt::DisplayRole).toString();
      QVectorIterator<OccurrenceItem*> it(occurrenceVector);
      while (it.hasNext()) {
	OccurrenceItem *current = it.next();
	if (current->getAttribute() == attribute) {
	  current->setColor(color);
	}
      }
    }
  }
}

void OccurrenceGraphWidget::groupOccurrences() {
  std::sort(presentAttributes.begin(), presentAttributes.end(), stringSort);
  QVectorIterator<OccurrenceItem*> it(occurrenceVector);
  while (it.hasNext()) {
    OccurrenceItem *current = it.next();
    current->setGrouped(false);
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
  QVector<OccurrenceItem*> keep;
  QVectorIterator<OccurrenceItem*> it3(occurrenceVector);
  while (it3.hasNext()) {
    OccurrenceItem *first = it3.next();
    QVectorIterator<OccurrenceItem*> it4(occurrenceVector);
    QVector<OccurrenceItem*> temp;
    temp.push_back(first);
    if (!keep.contains(first)) {
      keep.push_back(first);
    }
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
      if (!keep.contains(candidate)) {
	candidate->hide();
	candidate->setPermHidden(true);
	candidate->getLabel()->hide();
      }
    }
    std::sort(temp.begin(), temp.end(), attributeLessThan);
    QVectorIterator<OccurrenceItem*> it6(temp);
    qreal x = temp.first()->scenePos().x();
    qreal startY = temp.first()->scenePos().y();
    int dist = 0;
    while (it6.hasNext()) {
      OccurrenceItem *current = it6.next();
      if (temp.size() > 1) {
	current->setGrouped(true);
      }
      if (current->isVisible()) {
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
  std::sort(occurrenceVector.begin(), occurrenceVector.end(), eventLessThan);
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
	newArrow->setCopy(true);
	edgeVector.push_back(newArrow);
	scene->addItem(newArrow);
      }
    }
  }
}

void OccurrenceGraphWidget::restore() {
  reset();
  groupOccurrences();
  wireLinkages();
}

void OccurrenceGraphWidget::reset() {
  QVector<OccurrenceItem*>::iterator it;
  for (it = occurrenceVector.begin(); it != occurrenceVector.end();) {
    OccurrenceItem *current = *it;
    current->hide();
    current->getLabel()->hide();
    if (current->getId() < 0) {
      scene->removeItem(current->getLabel());
      scene->removeItem(current);
      labelVector.removeOne(current->getLabel());
      occurrenceVector.removeOne(current);
    } else {
      current->setPos(QPointF((current->getOrder() * distance), 0));
      current->setPermHidden(false); // We reset this here.
      QString text = QString::number(current->getOrder()) + " - " + current->getAttribute();
      QColor textColor = current->getLabel()->defaultTextColor();
      scene->removeItem(current->getLabel());
      labelVector.removeOne(current->getLabel());
      OccurrenceLabel *newLabel = new OccurrenceLabel(current);
      newLabel->setPlainText(text);
      newLabel->setTextWidth(current->getLabel()->boundingRect().width());
      newLabel->setDefaultTextColor(textColor);
      current->show();
      current->setLabel(newLabel);
      current->getLabel()->show();
      current->getLabel()->setNewPos(current->scenePos());
      scene->addItem(current->getLabel());
      labelVector.push_back(current->getLabel());
      it++;
    }
  }
}

void OccurrenceGraphWidget::getEvents() {
  reset();
  QVector<EventItem*> incidents = eventGraph->getEventItems();
  if (incidents.size() > 0) {
    QVectorIterator<EventItem*> it(incidents);
    while (it.hasNext()) {
      EventItem *incident = it.next();
      QVectorIterator<OccurrenceItem*> it2(occurrenceVector);
      while (it2.hasNext()) {
	OccurrenceItem *occurrence = it2.next();
	if (incident->getId() == occurrence->getId()) {
	  if (incident->getMacroEvent() != NULL) {
	    MacroEvent *macro = incident->getMacroEvent();
	    while (macro->getMacroEvent() != NULL) {
	      macro = macro->getMacroEvent();
	    }
	    QString type = "";
	    if (macro->getConstraint() == PATHS) {
	      type = "P";
	    } else if (macro->getConstraint() == SEMIPATHS) {
	      type = "S";
	    }
	    QString text = type + QString::number(macro->getId()) + " - " +
	      occurrence->getAttribute();
	    QColor textColor = occurrence->getLabel()->defaultTextColor();
	    scene->removeItem(occurrence->getLabel());
	    labelVector.removeOne(occurrence->getLabel());
	    OccurrenceLabel *newLabel = new OccurrenceLabel(occurrence);
	    newLabel->setPlainText(text);
	    newLabel->setDefaultTextColor(textColor);
	    occurrence->setLabel(newLabel);
	    newLabel->setTextWidth(occurrence->getLabel()->boundingRect().width());
	    occurrence->show();
	    occurrence->getLabel()->show();
	    occurrence->setPos(macro->scenePos().x(), 0);
  	    occurrence->getLabel()->setNewPos(occurrence->scenePos());
	    scene->addItem(occurrence->getLabel());
	    labelVector.push_back(occurrence->getLabel());
	  } else {
	    occurrence->setPos(incident->scenePos().x(), 0);
	    occurrence->getLabel()->setNewPos(occurrence->scenePos());
	  }
	}
      }
    }
  }
  QVector<MacroEvent*> macros = eventGraph->getMacros();
  if (macros.size() > 0) {
    QVectorIterator<MacroEvent*> it(macros);
    while (it.hasNext()) {
      MacroEvent *macro = it.next();
      QSet<QString> attributes = macro->getAttributes();
      QSetIterator<QString> it2(attributes);
      while (it2.hasNext()) {
	QString currentAttribute = it2.next();
	QColor color = QColor();
	bool found = false;
	for (int i = 0; i != attributeListWidget->rowCount(); i++) {
	  QTableWidgetItem *item = attributeListWidget->item(i, 0);
	  QString title = item->data(Qt::DisplayRole).toString();
	  if (title == currentAttribute) {
	    color = attributeListWidget->item(i, 1)->background().color();
	    found = true;
	  }
	}
	if (found) {
	  while (macro->getMacroEvent() != NULL) {
	    macro = macro->getMacroEvent();
	  }
	  qDebug() << macro->getId();
	  qDebug() << macro->getDescription();
	  qDebug() << macro->getOrder();
	  QPointF position = QPointF(macro->scenePos().x(), 0);
	  // I am setting macro id's to negatives to distinguish them from the incident ids.
	  OccurrenceItem *newOccurrence = new OccurrenceItem(40, macro->getDescription(),
							     position, (macro->getId() * -1),
							     macro->getOrder(),
							     currentAttribute);
	  newOccurrence->setPos(newOccurrence->getOriginalPos());
	  newOccurrence->setColor(color);
	  newOccurrence->setZValue(1);
	  occurrenceVector.push_back(newOccurrence);
	  scene->addItem(newOccurrence);
	  OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
	  QString type = "";
	  if (macro->getConstraint() == PATHS) {
	    type = "P";
	  } else if (macro->getConstraint() == SEMIPATHS) {
	    type = "S";
	  }
	  QString text = type + QString::number(macro->getId()) + " - " + currentAttribute;
	  qDebug() << text;
	  label->setPlainText(text);
	  label->setDefaultTextColor(Qt::black);
	  label->setTextWidth(label->boundingRect().width());
	  label->setNewPos(newOccurrence->scenePos());
	  labelVector.push_back(label);
	  newOccurrence->setLabel(label);
	  label->setZValue(2);
	  scene->addItem(label);
	}
      }
    }
  }
  groupOccurrences();
  wireLinkages();
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

void OccurrenceGraphWidget::processMoveLine(QGraphicsItem *item, QPointF pos) {
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
    if (!source->isGrouped()) {
      QVectorIterator<OccurrenceItem*> it2(occurrenceVector);
      while (it2.hasNext()) {
	OccurrenceItem *current = it2.next();
	if (current->getAttribute() == source->getAttribute() &&
	    !current->isGrouped()) {
	  current->setPos(current->scenePos().x(), current->scenePos().y() + yDiff);
	  current->getLabel()->setNewPos(current->scenePos());
	  QVectorIterator<OccurrenceItem*> it3(occurrenceVector);
	}
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
	currentItem->hide();
      } else {
	currentItem->show();
      }
    }
  }
  labelsVisible = !(labelsVisible);
}

void OccurrenceGraphWidget::setBackgroundColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) {
    QColor color = colorDialog->selectedColor();
    view->setBackgroundBrush(color);
  }
  delete colorDialog;
}

void OccurrenceGraphWidget::increaseDistance() {
  std::sort(occurrenceVector.begin(), occurrenceVector.end(), eventLessThan);
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
  std::sort(occurrenceVector.begin(), occurrenceVector.end(), eventLessThan);
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

void OccurrenceGraphWidget::exportMatrix() {
  // First we make a matrix (in the form of a vector of vectors).
  QVector<QVector<int>> matrix;
  QVector<QString> headerRow;
  for (int i = 0; i != attributeListWidget->rowCount(); i++) {
    QVector<int> currentRow;
    QString currentAttribute = attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
    headerRow.push_back(currentAttribute);
    for (int j = 0; j != attributeListWidget->rowCount(); j++) {
      QString currentPartner = attributeListWidget->item(j,0)->data(Qt::DisplayRole).toString();
      int count = 0;
      QVectorIterator<OccurrenceItem*> it(occurrenceVector);
      while (it.hasNext()) {
	OccurrenceItem *first = it.next();
	if (first->getAttribute() == currentAttribute) {
	  QVectorIterator<OccurrenceItem*> it2(occurrenceVector);
	  while (it2.hasNext()) {
	    OccurrenceItem *second = it2.next();
	    if (second->getAttribute() == currentPartner &&
		first->scenePos().x() == second->scenePos().x()) {
	      count++;
	    }
	  }
	}
      }
      currentRow.push_back(count);
    }
    matrix.push_back(currentRow);
  }
  // Then we can create a file.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) {
    if(!fileName.endsWith(".csv")) {
      fileName.append(".csv");
    }
    // And we create a file outstream.  
    std::ofstream fileOut(fileName.toStdString().c_str());
    // We first write the header.
    QVectorIterator<QString> it3(headerRow);
    while (it3.hasNext()) {
      QString currentHeader = it3.next();
      fileOut << "," << "\"" << doubleQuote(currentHeader).toStdString() << "\"";
    }
    fileOut << "\n"; // End the header with a newline symbol.
    // Then we write the other data.
    int counter = 0;
    QVectorIterator<QVector<int>> it4(matrix);
    while (it4.hasNext()) {
      QVector<int> currentRow = it4.next();
      fileOut << doubleQuote(headerRow[counter]).toStdString(); // The first row should always be the attribute label.
      counter++;
      QVectorIterator<int> it5(currentRow);
      while (it5.hasNext()) {
	int currentValue = it5.next();
	fileOut << "," << currentValue;
      }
      fileOut << "\n"; // End each row with a newline symbol.
    }
    // And that should be it!
    fileOut.close();
  }
}

void OccurrenceGraphWidget::saveCurrentPlot() {
  QPointer<SimpleTextDialog> saveDialog = new SimpleTextDialog(this);
  saveDialog->setWindowTitle("Save current plot");
  saveDialog->submitText(plotLabel->text());
  saveDialog->setLabel("Plot name:");
  saveDialog->exec();
  if (saveDialog->getExitStatus() == 0) {
    QString name = saveDialog->getText();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT plot FROM saved_og_plots WHERE plot = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
    bool empty = false;
    if (query->isNull(0)) {
      empty = true;
    } else {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Plot exists</h2>");
      warningBox->setInformativeText("A plot with this name already exists "
				     "Do you want to overwrite this plot?");
      if (warningBox->exec() == QMessageBox::Yes) {
	delete warningBox;
      } else {
	delete warningBox;
	return;
      }
    }
    if (!empty) {
      // Clear out all data before writing.
      // saved_og_plots_occurence_items
      query->prepare("DELETE FROM saved_og_plots_occurrence_items "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_og_plots_occurrence_labels
      query->prepare("DELETE FROM saved_og_plots_occurrence_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_og_plots_legend
      query->prepare("DELETE FROM saved_og_plots_legend "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
    } else {
      // Insert new data into saved_og_plots and then write data.
      query->prepare("INSERT INTO saved_og_plots (plot) "
		     "VALUES (:name)");
      query->bindValue(":name", name);
      query->exec();
    }
    QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, occurrenceVector.size());
    saveProgress->setWindowTitle("Saving occurrence items");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    int counter = 1;
    saveProgress->show();

    QVectorIterator<OccurrenceItem*> it(occurrenceVector);
    while (it.hasNext()) {
      OccurrenceItem *currentItem = it.next();
      qreal currentX = currentItem->pos().x();
      qreal currentY = currentItem->pos().y();
      qreal originalX = currentItem->getOriginalPos().x();
      qreal originalY = currentItem->getOriginalPos().y();
      int incident = currentItem->getId();
      int order = currentItem->getOrder();
      QString attribute = currentItem->getAttribute();
      int width = currentItem->getWidth();
      QColor color = currentItem->getColor();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      int alpha = color.alpha();
      int hidden = 1;
      int perm = 0;
      if (currentItem->isVisible()) {
	hidden = 0;
      }
      if (currentItem->isPermHidden()) {
	perm = 1;
      }
      query->prepare("INSERT INTO saved_og_plots_occurrence_items "
		     "(plot, incident, ch_order, attribute, width, curxpos, curypos, orixpos, "
		     "oriypos, red, green, blue, alpha, hidden, perm) "
		     "VALUES (:plot, :incident, :order, :attribute, :width, :curxpos, :curypos, "
		     ":orixpos, :oriypos, :red, :green, :blue, :alpha, :hidden, :perm)");
      query->bindValue(":plot", name);
      query->bindValue(":incident", incident);
      query->bindValue(":order", order);
      query->bindValue(":attribute", attribute);
      query->bindValue(":width", width);
      query->bindValue(":curxpos", currentX);
      query->bindValue(":curypos", currentY);
      query->bindValue(":orixpos", originalX);
      query->bindValue(":oriypos", originalY);
      query->bindValue(":red", red);
      query->bindValue(":green", green);
      query->bindValue(":blue", blue);
      query->bindValue(":alpha", alpha);
      query->bindValue(":hidden", hidden);
      query->bindValue(":perm", perm);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, labelVector.size());
    saveProgress->setWindowTitle("Saving labels");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    QVectorIterator<OccurrenceLabel*> it2(labelVector);
    while (it2.hasNext()) {
      OccurrenceLabel *currentLabel = it2.next();
      int id = currentLabel->getOccurrence()->getId();
      QString attribute = currentLabel->getOccurrence()->getAttribute();
      QString text = currentLabel->toPlainText();
      qreal currentX = currentLabel->scenePos().x();
      qreal currentY = currentLabel->scenePos().y();
      qreal xOffset = currentLabel->getOffset().x();
      qreal yOffset = currentLabel->getOffset().y();
      int red = currentLabel->defaultTextColor().red();
      int green = currentLabel->defaultTextColor().green();
      int blue = currentLabel->defaultTextColor().blue();
      int alpha = currentLabel->defaultTextColor().alpha();
      int hidden = 1;
      if (currentLabel->isVisible()) {
	hidden = 0;
      }
      query->prepare("INSERT INTO saved_og_plots_occurrence_labels "
		     "(plot, incident, attribute, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden) "
		     "VALUES (:plot, :incident, :attribute, :label, :curxpos, :curypos, "
		     ":xoffset, :yoffset, :red, :green, :blue, :alpha, :hidden)");
      query->bindValue(":plot", name);
      query->bindValue(":incident", id);
      query->bindValue(":attribute", attribute);
      query->bindValue(":label", text);
      query->bindValue(":curxpos", currentX);
      query->bindValue(":curypos", currentY);
      query->bindValue(":xoffset", xOffset);
      query->bindValue(":yoffset", yOffset);
      query->bindValue(":red", red);
      query->bindValue(":green", green);
      query->bindValue(":blue", blue);
      query->bindValue(":alpha", alpha);
      query->bindValue(":hidden", hidden);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    
    saveProgress->close();
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, attributeListWidget->rowCount());
    saveProgress->setWindowTitle("Saving legend");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    for (int i = 0; i != attributeListWidget->rowCount(); i++) {
      QTableWidgetItem *item = attributeListWidget->item(i, 0);
      QString title = item->data(Qt::DisplayRole).toString();
      QString tip = item->data(Qt::ToolTipRole).toString();
      QColor color = attributeListWidget->item(i, 1)->background().color();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      int alpha = color.alpha();
      query->prepare("INSERT INTO saved_og_plots_legend (plot, name, tip, "
		     "red, green, blue, alpha) "
		     "VALUES (:plot, :name, :tip, :red, :green, :blue, :alpha)");
      query->bindValue(":plot", name);
      query->bindValue(":name", title);
      query->bindValue(":tip", tip);
      query->bindValue(":red", red);
      query->bindValue(":green", green);
      query->bindValue(":blue", blue);
      query->bindValue(":alpha", alpha);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    delete saveProgress;
    plotLabel->setText(name);
    changeLabel->setText("");
    delete saveProgress;
    delete query;
  }
  delete saveDialog;
}

void OccurrenceGraphWidget::seePlots() {
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this, OCCURRENCEGRAPH);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) {
    savePlotButton->setEnabled(true);
    cleanUp();
    scene->clear();
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT incident, ch_order, attribute, width, curxpos, curypos, orixpos, "
		   "oriypos, red, green, blue, alpha, hidden, perm "
		   "FROM saved_og_plots_occurrence_items "
		   "WHERE plot = :plot ");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int id = query->value(0).toInt();
      int order = query->value(1).toInt();
      QString attribute = query->value(2).toString();
      int width = query->value(3).toInt();
      qreal currentX = query->value(4).toReal();
      qreal currentY = query->value(5).toReal();
      qreal originalX = query->value(6).toReal();
      qreal originalY = query->value(7).toReal();
      int red = query->value(8).toInt();
      int green = query->value(9).toInt();
      int blue = query->value(10).toInt();
      int alpha = query->value(11).toInt();
      int hidden = query->value(12).toInt();
      int perm = query->value(13).toInt();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT description FROM incidents WHERE id = :id");
      query2->bindValue(":id", id);
      query2->exec();
      query2->first();
      QString toolTip = "";
      if (query2->isNull(0)) {
	toolTip = "Incident was deleted";
      } else {
	toolTip = query2->value(0).toString();
      }
      delete query2;
      QPointF currentPos = QPointF(currentX, currentY);
      QPointF originalPos = QPointF(originalX, originalY);
      OccurrenceItem *currentItem = new OccurrenceItem(width, toolTip, originalPos,
						       id, order, attribute);
      currentItem->setPos(currentPos);
      currentItem->setColor(QColor(red, green, blue, alpha));
      currentItem->setZValue(1);
      occurrenceVector.push_back(currentItem);
      scene->addItem(currentItem);
      if (hidden == 1) {
	currentItem->hide();
      } else {
	currentItem->show();
      }
      if (perm == 1) {
	currentItem->setPermHidden(true);
      } else {
	currentItem->setPermHidden(false);
      }
    }
    query->prepare("SELECT incident, attribute, label, curxpos, curypos, xoffset, yoffset, "
		   "red, green, blue, alpha, hidden "
		   "FROM saved_og_plots_occurrence_labels "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int id = query->value(0).toInt();
      QString attribute = query->value(1).toString();
      QString text = query->value(2).toString();
      qreal currentX = query->value(3).toReal();
      qreal currentY = query->value(4).toReal();
      qreal xOffset = query->value(5).toReal();
      qreal yOffset = query->value(6).toReal();
      int red = query->value(7).toInt();
      int green = query->value(8).toInt();
      int blue = query->value(9).toInt();
      int alpha = query->value(10).toInt();
      int hidden = query->value(11).toInt();
      QVectorIterator<OccurrenceItem*> it(occurrenceVector);
      while (it.hasNext()) {
	OccurrenceItem *currentItem = it.next();
	if (currentItem->getId() == id && currentItem->getAttribute() == attribute) {
	  OccurrenceLabel *currentLabel = new OccurrenceLabel(currentItem);
	  currentLabel->setPlainText(text);
	  currentLabel->setTextWidth(currentLabel->boundingRect().width());
	  currentLabel->setPos(QPointF(currentX, currentY));
	  currentLabel->setOffset(QPointF(xOffset, yOffset));
	  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
	  currentLabel->setZValue(2);
	  currentItem->setLabel(currentLabel);
	  labelVector.push_back(currentLabel);
	  scene->addItem(currentLabel);
	  if (hidden == 1) {
	    currentLabel->hide();
	  } else {
	    currentLabel->hide();
	  }
	  break;
	}
      }
    }
    query->prepare("SELECT name, tip, red, green, blue, alpha "
		   "FROM saved_og_plots_legend "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      QString name = query->value(0).toString();
      QString tip = query->value(1).toString();
      int red = query->value(2).toInt();
      int green = query->value(3).toInt();
      int blue = query->value(4).toInt();
      int alpha = query->value(5).toInt();
      QColor color = QColor(red, green, blue, alpha);
      QTableWidgetItem *item = new QTableWidgetItem(name);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
      item->setToolTip(tip);
      item->setData(Qt::DisplayRole, name);
      attributeListWidget->setRowCount(attributeListWidget->rowCount() + 1);
      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 0, item);
      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->setBackground(color);
      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->
	setFlags(attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->flags() ^
		 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
      presentAttributes.push_back(name);
    }
    distance = 70;
    plotLabel->setText(plot);
    changeLabel->setText("");
    scene->update();
    groupOccurrences();
    wireLinkages();
    setRangeControls();
    setVisibility();
    checkCongruency();
    delete query;
  } else if (savedPlotsDialog->getExitStatus() == 2) {
    // DON'T FORGET TO UPDATE THIS FUNCTION!!!!
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    // saved_og_plots
    query->prepare("DELETE FROM saved_og_plots "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_og_plots_occurrence_items
    query->prepare("DELETE FROM saved_og_plots_occurrence_items "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_eg_plots_occurrence_labels
    query->prepare("DELETE FROM saved_eg_plots_occurrence_labels "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_og_plots_legend
    query->prepare("DELETE FROM saved_og_plots_legend "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    delete query;
    seePlots();
  }
}


void OccurrenceGraphWidget::setEventGraph(EventGraphWidget *egw) {
  eventGraph = egw;
}

void OccurrenceGraphWidget::cleanUp() {
  scene->clearSelection();
  qDeleteAll(occurrenceVector);
  occurrenceVector.clear();
  qDeleteAll(labelVector);
  labelVector.clear();
  qDeleteAll(edgeVector);
  edgeVector.clear();
  attributeListWidget->setRowCount(0);
  presentAttributes.clear();
}

void OccurrenceGraphWidget::finalBusiness() {
  cleanUp();
}
