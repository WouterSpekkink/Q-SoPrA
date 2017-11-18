#include "../include/EventGraphWidget.h"

// Some sorting bools for qSort.
bool eventLessThan(const EventItem *itemOne, const EventItem *itemTwo) {
  int orderOne = itemOne->scenePos().x();
  int orderTwo = itemTwo->scenePos().x();
  if (orderOne < orderTwo) {
    return true;
  } else {
    return false;
  }
}

EventGraphWidget::EventGraphWidget(QWidget *parent) : QWidget(parent) {
  selectedCoder = "";
  selectedCompare = "";
  selectedType = "";

  distance = 0;
  vectorPos = 0;
    
  scene = new Scene(this);
  view = new GraphicsView(scene);
  view->setDragMode(QGraphicsView::RubberBandDrag);
  view->setRubberBandSelectionMode(Qt::ContainsItemShape);

  infoWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
  
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);

  view->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

  coderLabel = new QLabel(tr("Choose coder:"), this);
  typeLabel = new QLabel(tr("Choose linkage:"), this);
  plotLabel = new QLabel(tr("Unsaved plot"), this);
  changeLabel = new QLabel(tr(""), this);
  compareLabel = new QLabel(tr("Compare with:"), this);
  timeStampLabel = new QLabel("<b>Timing:</b>", infoWidget);
  sourceLabel = new QLabel("<b>Source:</b>", infoWidget);
  descriptionLabel = new QLabel("<b>Description:</b>", infoWidget);
  rawLabel = new QLabel("<b>Raw:</b>", infoWidget);
  commentLabel = new QLabel("<b>Comments:</b>", infoWidget);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  
  coderComboBox = new QComboBox(this);
  coderComboBox->addItem(DEFAULT);

  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);

  compareComboBox = new QComboBox(this);
  compareComboBox->addItem(DEFAULT);

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
  
  plotButton = new QPushButton(tr("Plot new"), this);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  plotButton->setEnabled(false);
  exportSvgButton = new QPushButton(tr("Export"), graphicsWidget);
  increaseDistanceButton = new QPushButton(tr("< >"), this);
  decreaseDistanceButton = new QPushButton(tr("> <"), this);
  compareButton = new QPushButton(tr("Compare"), this);
  toggleDetailsButton = new QPushButton(tr("Toggle details"), this);
  toggleDetailsButton->setCheckable(true);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  previousEventButton = new QPushButton(tr("<<"), infoWidget);
  nextEventButton = new QPushButton(tr(">>"), infoWidget);
  
  timeStampField = new QLineEdit(infoWidget);
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit(infoWidget);
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit(infoWidget);
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit(infoWidget);
  rawField->setReadOnly(true);
  commentField = new QTextEdit(infoWidget);
  commentField->installEventFilter(infoWidget);

  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(coderComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(compareComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setCompareButton()));  
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotGraph()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(compareButton, SIGNAL(clicked()), this, SLOT(compare()));
  connect(scene, SIGNAL(widthIncreased(EventItem*)), this, SLOT(increaseWidth(EventItem*)));
  connect(scene, SIGNAL(widthDecreased(EventItem*)), this, SLOT(decreaseWidth(EventItem*)));
  connect(scene, SIGNAL(posIncreased(EventItem*)), this, SLOT(increasePos(EventItem*)));
  connect(scene, SIGNAL(posDecreased(EventItem*)), this, SLOT(decreasePos(EventItem*)));
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(retrieveData()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(previousEventButton, SIGNAL(clicked()), this, SLOT(previousDataItem()));
  connect(nextEventButton, SIGNAL(clicked()), this, SLOT(nextDataItem()));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(cleanUp()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
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
  plotOptionsLayout->addSpacerItem(new QSpacerItem(100,0));
  topLayout->addLayout(plotOptionsLayout);
  plotOptionsLayout->setAlignment(Qt::AlignLeft);
  QPointer<QHBoxLayout> compareLayout = new QHBoxLayout;
  compareLayout->addWidget(compareLabel);
  compareLayout->addWidget(compareComboBox);
  compareLayout->addWidget(compareButton);
  topLayout->addLayout(compareLayout);
  compareLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(topLayout);

  QPointer<QHBoxLayout> screenLayout = new QHBoxLayout;

  QPointer<QVBoxLayout> detailsLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> navigationLayout = new QHBoxLayout;
  navigationLayout->addWidget(previousEventButton);
  navigationLayout->addWidget(nextEventButton);
  detailsLayout->addLayout(navigationLayout);
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  topFieldsLayout->addWidget(timeStampLabel);
  topFieldsLayout->addWidget(timeStampField);
  topFieldsLayout->addWidget(sourceLabel);
  topFieldsLayout->addWidget(sourceField);
  detailsLayout->addLayout(topFieldsLayout);
  detailsLayout->addWidget(descriptionLabel);
  detailsLayout->addWidget(descriptionField);
  detailsLayout->addWidget(rawLabel);
  detailsLayout->addWidget(rawField);
  detailsLayout->addWidget(commentLabel);
  detailsLayout->addWidget(commentField);
  infoWidget->setLayout(detailsLayout);
  screenLayout->addWidget(infoWidget);
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect  screenGeometry = screen->geometry();
  int screenWidth = screenGeometry.width();
  infoWidget->setMaximumWidth(screenWidth / 4);
  infoWidget->setMinimumWidth(screenWidth / 4);

  QPointer<QVBoxLayout> middleLayout = new QVBoxLayout;
  middleLayout->addWidget(view);
  view->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
  screenLayout->addLayout(middleLayout);

  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
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
  graphicsWidget->setMaximumWidth(150);
  graphicsWidget->setMinimumWidth(150);
  graphicsWidget->setLayout(graphicsControlsLayout);
  graphicsControlsLayout->setAlignment(Qt::AlignBottom);
  screenLayout->addWidget(graphicsWidget);
  
  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  drawOptionsLeftLayout->addWidget(toggleDetailsButton);
  drawOptionsLeftLayout->addWidget(increaseDistanceButton);
  drawOptionsLeftLayout->addWidget(decreaseDistanceButton);
  increaseDistanceButton->setMaximumWidth(increaseDistanceButton->sizeHint().width());
  decreaseDistanceButton->setMaximumWidth(decreaseDistanceButton->sizeHint().width());
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);
    
  setLayout(mainLayout);
  infoWidget->hide();
  graphicsWidget->hide();
}

void EventGraphWidget::toggleDetails() {
  if (infoWidget->isHidden()) {
    infoWidget->show();
  } else {
    infoWidget->hide();
  }
}

void EventGraphWidget::toggleGraphicsControls() {
  if (graphicsWidget->isHidden()) {
    graphicsWidget->show();
  } else {
    graphicsWidget->hide();
  }
}

void EventGraphWidget::retrieveData() {
  if (currentData.size() > 0) {
    currentData.clear();
  }
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
      currentEvent->setSelectionColor(Qt::black);
      currentEvent->update();
      Arrow *currentArrow = qgraphicsitem_cast<Arrow*>(currentEvent);
      if (currentEvent && !(currentArrow)) {
	currentData.push_back(currentEvent);
      }
    }
    if (currentData.size() > 0) {
      qSort(currentData.begin(), currentData.end(), eventLessThan);
    
      vectorPos = 0;
      EventItem *currentEvent = currentData.at(vectorPos);
      currentEvent->setSelectionColor(Qt::red);
      currentEvent->update();
      int id = currentEvent->getId();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString timeStamp = query->value(0).toString();
      QString description = query->value(1).toString();
      QString raw = query->value(2).toString();
      QString comment = query->value(3).toString();
      QString source = query->value(4).toString();
      timeStampField->setText(timeStamp);
      descriptionField->setText(description);
      rawField->setText(raw);
      commentField->setText(comment);
      sourceField->setText(source);
      delete query;
    }
  } else {
    timeStampField->clear();
    descriptionField->clear();
    rawField->clear();
    commentField->clear();
    sourceField->clear();
  }
}

void EventGraphWidget::previousDataItem() {
  if (vectorPos > 0) {
    EventItem *currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::black);
    currentEvent->update();
    vectorPos--;
    currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::red);
    currentEvent->update();
    int id = currentEvent->getId();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		   "WHERE id = :id");
    query->bindValue(":id", id);
    query->exec();
    query->first();
    QString timeStamp = query->value(0).toString();
    QString description = query->value(1).toString();
    QString raw = query->value(2).toString();
    QString comment = query->value(3).toString();
    QString source = query->value(4).toString();
    timeStampField->setText(timeStamp);
    descriptionField->setText(description);
    rawField->setText(raw);
    commentField->setText(comment);
    sourceField->setText(source);
    delete query;
  } else {
    EventItem *currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::black);
    currentEvent->update();
    vectorPos = currentData.size() - 1;
    currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::red);
    currentEvent->update();
    scene->update();
    int id = currentEvent->getId();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		   "WHERE id = :id");
    query->bindValue(":id", id);
    query->exec();
    query->first();
    QString timeStamp = query->value(0).toString();
    QString description = query->value(1).toString();
    QString raw = query->value(2).toString();
    QString comment = query->value(3).toString();
    QString source = query->value(4).toString();
    timeStampField->setText(timeStamp);
    descriptionField->setText(description);
    rawField->setText(raw);
    commentField->setText(comment);
    sourceField->setText(source);
    delete query;
  }
}

void EventGraphWidget::nextDataItem() {
  //  QVectorIterator<EventItem*> it2(currentData);
  // Need to do this differently. Scrolling buttons.
  //  while (it2.hasNext()) {
  if (vectorPos != currentData.size() - 1) {
    EventItem *currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::black);
    currentEvent->update();
    vectorPos++;
    currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::red);
    currentEvent->update();
    int id = currentEvent->getId();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		   "WHERE id = :id");
    query->bindValue(":id", id);
    query->exec();
    query->first();
    QString timeStamp = query->value(0).toString();
    QString description = query->value(1).toString();
    QString raw = query->value(2).toString();
    QString comment = query->value(3).toString();
    QString source = query->value(4).toString();
    timeStampField->setText(timeStamp);
    descriptionField->setText(description);
    rawField->setText(raw);
    commentField->setText(comment);
    sourceField->setText(source);
    delete query;
  } else {
    EventItem *currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::black);
    currentEvent->update();
    vectorPos = 0;
    currentEvent = currentData.at(vectorPos);
    currentEvent->setSelectionColor(Qt::red);
    currentEvent->update();
    int id = currentEvent->getId();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		   "WHERE id = :id");
    query->bindValue(":id", id);
    query->exec();
    query->first();
    QString timeStamp = query->value(0).toString();
    QString description = query->value(1).toString();
    QString raw = query->value(2).toString();
    QString comment = query->value(3).toString();
    QString source = query->value(4).toString();
    timeStampField->setText(timeStamp);
    descriptionField->setText(description);
    rawField->setText(raw);
    commentField->setText(comment);
    sourceField->setText(source);
    delete query;
  }
}

    
void EventGraphWidget::getEvents() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT ch_order, description FROM incidents ORDER BY ch_order");
  while (query->next()) {
    int order = query->value(0).toInt();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT id FROM incidents WHERE ch_order = :order");
    query2->bindValue(":order", order);
    query2->exec();
    query2->first();
    int id = query2->value(0).toInt();
    QString toolTip = "<FONT SIZE = 3>" + query->value(1).toString() + "</FONT>";
    int vertical = qrand() % 240 - 120;
    QPointF position = QPointF((order * distance), vertical);
    EventItem *currentItem = new EventItem(30, toolTip, position, id);
    currentItem->setPos(currentItem->getOriginalPos());
    eventVector.push_back(currentItem);
    delete query2;
  }
  delete query;
}


void EventGraphWidget::plotEvents() {
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    //currentItem->setPos(currentItem->getOriginalPos());
    currentItem->setZValue(1);
    scene->addItem(currentItem);
  }
}

void EventGraphWidget::getEdges(QString coder, QString type) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", coder);
  query->bindValue(":type", type);
  query->exec();
  while (query->next()) {
    int tail = query->value(0).toInt();
    int head = query->value(1).toInt();
    QSqlQuery *query2 =  new QSqlQuery;
    query2->prepare("SELECT comment, coder FROM linkage_comments "
		    "WHERE tail = :tail AND head = :head AND type = :type");
    query2->bindValue(":tail", tail);
    query2->bindValue(":head", head);
    query2->bindValue(":type", type);
    query2->exec();
    query2->first();
    QString comment = "";
    QString commentCoder = "";
    QString toolTip = "";
    if (!(query2->isNull(0))) {
      comment = query2->value(0).toString();
      commentCoder = query2->value(1).toString();
      toolTip = "<FONT SIZE = 3>" + commentCoder + " - " + comment + "</FONT>";
    } else {
      toolTip = "No comments";
    }
    QVectorIterator<EventItem*> it(eventVector);
    EventItem *tempSource = NULL;
    EventItem *tempTarget = NULL;
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      if (currentItem->getId() == tail) {
	tempSource = currentItem;
      } else if (currentItem->getId() == head) {
	tempTarget = currentItem;
      }
      if (tempSource != NULL && tempTarget != NULL) {
	int sourceHeight = tempSource->getOriginalPos().y();
	int newTargetHeight = sourceHeight + qrand() % 240 - 120;
	int targetOrder = tempTarget->getOriginalPos().x();
	tempTarget->setPos(targetOrder, newTargetHeight);
	Arrow *currentEdge = new Arrow(tempSource, tempTarget, type, coder);
	currentEdge->setToolTip(toolTip);
	edgeVector.push_back(currentEdge);
	break;
      }
    }
    delete query2;
  }
  delete query;
}

void EventGraphWidget::plotEdges() {
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) {
    Arrow *currentEdge = it.next();
    scene->addItem(currentEdge);
  }
}

void EventGraphWidget::cleanUp() {
  scene->clearSelection();
  qDeleteAll(currentData);
  currentData.clear();
  qDeleteAll(eventVector);
  qDeleteAll(edgeVector);
  eventVector.clear();
  edgeVector.clear();
  qDeleteAll(compareVector);
  compareVector.clear();
  selectedType = "";
  selectedCoder = "";
  selectedCompare = "";
}

void EventGraphWidget::increaseWidth(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::decreaseWidth(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::increasePos(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::decreasePos(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());

      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::increaseDistance() {
  QVectorIterator<EventItem*> it(eventVector);
  int unit = 1;
  if (distance < 100) {
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      QPointF newPos = QPointF(currentItem->pos().x() + unit, currentItem->pos().y());
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
      unit++;
    }
  }
  distance++;
}

void EventGraphWidget::decreaseDistance() {
  QVectorIterator<EventItem*> it(eventVector);
  int unit = 1;
  if (distance > 35) {
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      QPointF newPos = QPointF(currentItem->pos().x() - unit, currentItem->pos().y());
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
      unit++;
    }
  }
  distance--;
}

void EventGraphWidget::setPlotButton() {
  if (typeComboBox->currentText() != DEFAULT && coderComboBox->currentText() != DEFAULT) {
    plotButton->setEnabled(true);
  } else {
    plotButton->setEnabled(false);
  }
  if (coderComboBox->currentText() != DEFAULT) {
    compareComboBox->clear();
    compareComboBox->addItem(DEFAULT);
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT name FROM coders");
    while (query->next()) {
      QString currentCoder = query->value(0).toString();
      if (currentCoder != coderComboBox->currentText()) {
	compareComboBox->addItem(currentCoder);
      }
    }
    delete query;
  }
}

void EventGraphWidget::getLinkageDetails() {
  coderComboBox->clear();
  coderComboBox->addItem(DEFAULT);
  typeComboBox->clear();
  typeComboBox->addItem(DEFAULT);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM coders");
  while (query->next()) {
    QString currentCoder = query->value(0).toString();
    coderComboBox->blockSignals(true);
    coderComboBox->addItem(currentCoder);
    coderComboBox->blockSignals(false);
  }
  query->exec("SELECT name FROM linkage_types");
  while (query->next()) {
    QString currentType = query->value(0).toString();
    typeComboBox->addItem(currentType);
  }
  delete query;
}

void EventGraphWidget::plotGraph() {
  cleanUp();
  selectedCoder = coderComboBox->currentText();
  selectedType = typeComboBox->currentText();
  distance = 70;
  getEvents();
  plotEvents(); // Should allow for range to be set here.
  getEdges(selectedCoder, selectedType);
  plotEdges();
  compareComboBox->setCurrentIndex(0);
  savePlotButton->setEnabled(true);
  setRangeControls();
}

void EventGraphWidget::setCompareButton() {
  if (compareComboBox->currentText() != DEFAULT && selectedCoder != "") {
    compareButton->setEnabled(true);
  } else {
    compareButton->setEnabled(false);
  }
}

void EventGraphWidget::compare() {
  selectedCompare = compareComboBox->currentText();
  getCompareEdges(selectedCompare, selectedType);
  plotCompareEdges();
  compareButton->setEnabled(false);
}

void EventGraphWidget::getCompareEdges(QString coder, QString type) {
  qDeleteAll(compareVector);
  compareVector.clear();

  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", coder);
  query->bindValue(":type", type);
  query->exec();
  while (query->next()) {
    int tail = query->value(0).toInt();
    int head = query->value(1).toInt();
    QSqlQuery *query2 =  new QSqlQuery;
    query2->prepare("SELECT comment, coder FROM linkage_comments "
		    "WHERE tail = :tail AND head = :head AND type = :type");
    query2->bindValue(":tail", tail);
    query2->bindValue(":head", head);
    query2->bindValue(":type", type);
    query2->exec();
    query2->first();
    QString comment = "";
    QString commentCoder = "";
    QString toolTip = "";
    if (!(query2->isNull(0))) {
      comment = query2->value(0).toString();
      commentCoder = query2->value(1).toString();
      toolTip = "<FONT SIZE = 3>" + commentCoder + " - " + comment + "</FONT>";
    } else {
      toolTip = "No comments";
    }
    QVectorIterator<EventItem*> it(eventVector);
    EventItem *tempSource = NULL;
    EventItem *tempTarget = NULL;
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      if (currentItem->getId() == tail) {
	tempSource = currentItem;
      } else if (currentItem->getId() == head) {
	tempTarget = currentItem;
      }
      if (tempSource != NULL && tempTarget != NULL) {
	bool sameFound = false;
	QVectorIterator<Arrow*> it2(edgeVector);
	while (it2.hasNext()) {
	  Arrow *currentEdge = it2.next();
	  EventItem *tempS = currentEdge->startItem();
	  EventItem *tempT = currentEdge->endItem();
	  if (tempSource == tempS && tempTarget == tempT) {
	    currentEdge->setColor(Qt::darkGreen);
	    sameFound = true;
	  }
	}
	if (sameFound == false) {
	  Arrow *compareEdge = new Arrow(tempSource, tempTarget, type, coder);
	  compareEdge->setColor(Qt::darkRed);
	  compareVector.push_back(compareEdge);
	  compareEdge->setToolTip(toolTip);
	}
	break;
      }
    }
    delete query2;
  }
  delete query;
}

void EventGraphWidget::plotCompareEdges() {
  QVectorIterator<Arrow*> it(compareVector);
  while (it.hasNext()) {
    Arrow *currentEdge = it.next();
    scene->addItem(currentEdge);
  }
}

void EventGraphWidget::saveCurrentPlot() {
  QPointer<SimpleTextDialog> saveDialog = new SimpleTextDialog(this);
  saveDialog->setWindowTitle("Save current plot");
  saveDialog->setLabel("Plot name:");
  saveDialog->exec();
  if (saveDialog->getExitStatus() == 0) {
    QString name = saveDialog->getText();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT plot FROM saved_plots WHERE plot = :name");
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
      warningBox->setText("<h2>Plot exists?</h2>");
      warningBox->setInformativeText("A plot with this name already exists? Do you want to overwrite this plot?");
      if (warningBox->exec() == QMessageBox::Yes) {
	delete warningBox;
      } else {
	delete warningBox;
	return;
      }
    }
    if (!empty) {
      // Reset plot name and details, and clear out all data before writing.
      query->prepare("UPDATE saved_plots SET linkage = :linkage, coder = :coder "
		     "WHERE plot = :name");
      query->bindValue(":linkage", selectedType);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":name", name);
      query->exec();
      // saved_plots_event_items
      query->prepare("DELETE FROM saved_plots_event_items "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_plots_edges
      query->prepare("DELETE FROM saved_plots_edges "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
    } else {
      // Insert new data into saved_plots and then write data.
      query->prepare("INSERT INTO saved_plots (plot, linkage, coder) "
		     "VALUES (:name, :linkage, :coder)");
      query->bindValue(":name", name);
      query->bindValue(":linkage", selectedType);
      query->bindValue(":coder", selectedCoder);
      query->exec();
    }
    QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, eventVector.size());
    saveProgress->setWindowTitle("Saving event items");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    int counter = 1;
    saveProgress->show();

    QVectorIterator<EventItem*> it(eventVector);
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      int currentX = currentItem->pos().x();
      int currentY = currentItem->pos().y();
      int originalX = currentItem->getOriginalPos().x();
      int originalY = currentItem->getOriginalPos().y();
      int incident = currentItem->getId();
      int width = currentItem->getWidth();
      int dislodged = 0;
      if (currentItem->isDislodged()) {
	dislodged = 1;
      }
      query->prepare("INSERT INTO saved_plots_event_items (plot, incident, width, curxpos, curypos, orixpos, oriypos, dislodged) "
		     "VALUES (:plot, :incident, :width, :curxpos, :curypos, :orixpos, :oriypos, :dislodged)");
      query->bindValue(":plot", name);
      query->bindValue(":incident", incident);
      query->bindValue(":width", width);
      query->bindValue(":curxpos", currentX);
      query->bindValue(":curypos", currentY);
      query->bindValue(":orixpos", originalX);
      query->bindValue(":oriypos", originalY);
      query->bindValue(":dislodged", dislodged);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, edgeVector.size());
    saveProgress->setWindowTitle("Saving edge items");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();

    QVectorIterator<Arrow*> it2(edgeVector);
    while (it2.hasNext()) {
      Arrow *currentEdge = it2.next();
      EventItem *currentTail = currentEdge->startItem();
      EventItem *currentHead = currentEdge->endItem();
      int tail = currentTail->getId();
      int head = currentHead->getId();
      query->prepare("INSERT INTO saved_plots_edges "
		     "(plot, tail, head) "
		     "VALUES (:plot, :tail, :head)");
      query->bindValue(":plot", name);
      query->bindValue(":tail", tail);
      query->bindValue(":head", head);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    plotLabel->setText(name);
    changeLabel->setText("");
    delete saveProgress;
    delete query;
  }
  delete saveDialog;
}

void EventGraphWidget::seePlots() {
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) {
    savePlotButton->setEnabled(true);
    cleanUp();
    scene->clear();
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT linkage, coder FROM saved_plots "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    query->first();
    QString type = query->value(0).toString();
    QString coder = query->value(1).toString();
    selectedType = type;
    selectedCoder = coder;
    int index = coderComboBox->findText(coder);
    coderComboBox->setCurrentIndex(index);
    index = typeComboBox->findText(type);
    typeComboBox->setCurrentIndex(index);
    query->prepare("SELECT incident, width, curxpos, curypos, orixpos, oriypos, dislodged "
		   "FROM saved_plots_event_items "
		   "WHERE plot = :plot ");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int id = query->value(0).toInt();
      int width = query->value(1).toInt();
      int currentX = query->value(2).toInt();
      int currentY = query->value(3).toInt();
      int originalX = query->value(4).toInt();
      int originalY = query->value(5).toInt();
      int dislodged = query->value(6).toInt();

      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT description FROM incidents WHERE id = :id");
      query2->bindValue(":id", id);
      query2->exec();
      query2->first();
      QString toolTip = "<FONT SIZE = 3>" + query2->value(0).toString() + "</FONT>";
      delete query2;
      QPointF currentPos = QPointF(currentX, currentY);
      QPointF originalPos = QPointF(originalX, originalY);
      EventItem *currentItem = new EventItem(width, toolTip, originalPos, id);
      currentItem->setPos(currentPos);
      if (dislodged == 1) {
	currentItem->setDislodged(true);
      }
      eventVector.push_back(currentItem);
    }
    query->prepare("SELECT tail, head "
		   "FROM saved_plots_edges "
		   "WHERE plot = :plot ");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int tail = query->value(0).toInt();
      int head = query->value(1).toInt();
      QSqlQuery *query2 =  new QSqlQuery;
      query2->prepare("SELECT comment, coder FROM linkage_comments "
		      "WHERE tail = :tail AND head = :head AND type = :type");
      query2->bindValue(":tail", tail);
      query2->bindValue(":head", head);
      query2->bindValue(":type", type);
      query2->exec();
      query2->first();
      QString comment = "";
      QString commentCoder = "";
      QString toolTip = "";
      if (!(query2->isNull(0))) {
	comment = query2->value(0).toString();
      commentCoder = query2->value(1).toString();
      toolTip = "<FONT SIZE = 3>" + commentCoder + " - " + comment + "</FONT>";
      } else {
	toolTip = "No comments";
      }
      QVectorIterator<EventItem*> it(eventVector);
      EventItem *tempSource = NULL;
      EventItem *tempTarget = NULL;
      while (it.hasNext()) {
	EventItem *currentItem = it.next();
	if (currentItem->getId() == tail) {
	  tempSource = currentItem;
	} else if (currentItem->getId() == head) {
	  tempTarget = currentItem;
	}
	if (tempSource != NULL && tempTarget != NULL) {
	  Arrow *currentEdge = new Arrow(tempSource, tempTarget, selectedType, selectedCoder);
	  currentEdge->setToolTip(toolTip);
	  edgeVector.push_back(currentEdge);
	  break;
	}
      }
      delete query2;
    }
    distance = 70;
    plotLabel->setText(plot);
    changeLabel->setText("");
    plotEvents();
    plotEdges();
    setRangeControls();
    delete query;
  } else if (savedPlotsDialog->getExitStatus() == 2) {
    // DON'T FORGET TO UPDATE THIS FUNCTION!!!!
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    // saved_plots
    query->prepare("DELETE FROM saved_plots "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_plots_event_items
    query->prepare("DELETE FROM saved_plots_event_items "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_plots_edges
    query->prepare("DELETE FROM saved_plots_edges "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    delete query;
  }
}

void EventGraphWidget::setChangeLabel() {
  if (changeLabel->text() == "" && eventVector.size() > 0) {
    changeLabel->setText("*");
  }
}

void EventGraphWidget::processLowerRange(int value) {
  lowerRangeDial->setValue(value);
  lowerRangeSpinBox->setValue(value);
  if (upperRangeDial->value() <= value) {
    upperRangeDial->setValue(value + 1);
    upperRangeSpinBox->setValue(value + 1);
  }
  QVectorIterator<EventItem*> it(eventVector);
  int counter = 0;
  while (it.hasNext()) {
    counter++;
    EventItem* currentItem = it.next();
    if (counter > value && counter < upperRangeDial->value()) {
      currentItem->show();
    }  else {
      currentItem->hide();
    }
  }
  QVectorIterator<Arrow*> it2(edgeVector);
  while (it2.hasNext()) {
    Arrow *currentEdge = it2.next();
    EventItem *source = currentEdge->startItem();
    EventItem *target = currentEdge->endItem();
    if (!(source->isVisible()) || !(target->isVisible())) {
      currentEdge->hide();
    } else {
      currentEdge->show();
    }
  }
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
}

void EventGraphWidget::processUpperRange(int value) {
  upperRangeDial->setValue(value);
  upperRangeSpinBox->setValue(value);
  if (lowerRangeDial->value() >= value) {
    lowerRangeDial->setValue(value - 1);
    lowerRangeSpinBox->setValue(value - 1);
  }
  QVectorIterator<EventItem*> it(eventVector);
  int counter = 0;
  while (it.hasNext()) {
    EventItem* currentItem = it.next();
    counter++;
    if (counter < value && counter > lowerRangeDial->value()) {
      currentItem->show();
    }  else {
      currentItem->hide();
    }
  }
  QVectorIterator<Arrow*> it2(edgeVector);
  while (it2.hasNext()) {
    Arrow *currentEdge = it2.next();
    EventItem *source = currentEdge->startItem();
    EventItem *target = currentEdge->endItem();
    if (!(source->isVisible()) || !(target->isVisible())) {
      currentEdge->hide();
    } else {
      currentEdge->show();
    }
  }
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
} 

void EventGraphWidget::setRangeControls() {
  lowerRangeDial->setEnabled(true);
  upperRangeDial->setEnabled(true);
  lowerRangeSpinBox->setEnabled(true);
  upperRangeSpinBox->setEnabled(true);
  lowerRangeDial->setRange(0, eventVector.size() - 1);
  upperRangeDial->setRange(1, eventVector.size());
  lowerRangeSpinBox->setRange(0, eventVector.size() - 1);
  upperRangeSpinBox->setRange(1, eventVector.size());
  upperRangeDial->setValue(eventVector.size());
  upperRangeSpinBox->setValue(eventVector.size());
}

void EventGraphWidget::exportSvg() {
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
