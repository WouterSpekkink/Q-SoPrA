#include "../include/EventGraphWidget.h"

// Some sorting bools for qSort.
bool eventLessThan(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) {
  qreal orderOne = itemOne->scenePos().x();
  qreal orderTwo = itemTwo->scenePos().x();
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
  selectedMacro = NULL;
  selectedIncident = 0;
  commentBool = false;
  
  distance = 0;
  vectorPos = 0;

  labelsVisible = true;
  
  scene = new Scene(this);
  view = new GraphicsView(scene);
  view->setDragMode(QGraphicsView::RubberBandDrag);
  view->setRubberBandSelectionMode(Qt::ContainsItemShape);
  view->viewport()->installEventFilter(this);
  
  infoWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
  attWidget = new QWidget(this);
  commentWidget = new QWidget(this);
  
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);

  view->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

  attributesTreeView = new DeselectableTreeView(attWidget);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->setDragEnabled(true);
  attributesTreeView->setAcceptDrops(true);
  attributesTreeView->setDropIndicatorShown(true);
  attributesTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  attributesTreeView->installEventFilter(this);

  coderLabel = new QLabel(tr("Choose coder:"), this);
  typeLabel = new QLabel(tr("Choose linkage:"), this);
  plotLabel = new QLabel(tr("Unsaved plot"), this);
  changeLabel = new QLabel(tr("*"), this);
  compareLabel = new QLabel(tr("Compare with:"), this);
  timeStampLabel = new QLabel("<b>Timing:</b>", infoWidget);
  sourceLabel = new QLabel("<b>Source:</b>", infoWidget);
  descriptionLabel = new QLabel("<b>Description:</b>", infoWidget);
  rawLabel = new QLabel("<b>Raw:</b>", infoWidget);
  commentLabel = new QLabel("<b>Comments:</b>", commentWidget);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  indexLabel = new QLabel(tr("<b>(0/0)</b>"), infoWidget);
  attributesLabel = new QLabel(tr("<b>Attributes:</b>"), attWidget);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), attWidget);
  valueLabel = new QLabel(tr("<b>Value:</b>"), attWidget);
  
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

  timeStampField = new QLineEdit(infoWidget);
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit(infoWidget);
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit(infoWidget);
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit(infoWidget);
  rawField->setReadOnly(true);
  commentField = new QTextEdit(commentWidget);
  commentField->installEventFilter(commentWidget);
  attributesFilterField = new QLineEdit(attWidget);
  valueField = new QLineEdit(attWidget);
  valueField->setEnabled(false);
  
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
  previousEventButton = new QPushButton("<<", infoWidget);
  previousEventButton->setEnabled(false);   
  nextEventButton = new QPushButton(">>", infoWidget);
  seeIncidentsButton = new QPushButton(tr("Incidents"), infoWidget);
  seeIncidentsButton->setEnabled(false);
  nextEventButton->setEnabled(false);
  plotLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  colorByAttributeButton = new QPushButton(tr("Color by attribute"), graphicsWidget);
  eventColorButton = new QPushButton(tr("Set event color"), graphicsWidget);
  labelColorButton = new QPushButton(tr("Set label color"), graphicsWidget);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  valueButton = new QPushButton(tr("Store value"), attWidget);
  valueButton->setEnabled(false);
  assignAttributeButton = new QPushButton(tr("Assign attribute"), attWidget);
  unassignAttributeButton = new QPushButton(tr("Unassign attribute"), attWidget);
  addAttributeButton = new QPushButton(tr("New attribute"), attWidget);
  editAttributeButton = new QPushButton(tr("Edit attribute"), attWidget);
  removeUnusedAttributesButton = new QPushButton(tr("Remove unused"), attWidget);
  seeAttributesButton = new QPushButton(tr("Attributes"), commentWidget);
  seeCommentsButton = new QPushButton(tr("Comments"), attWidget);
  
  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(seeAttributesButton, SIGNAL(clicked()), this, SLOT(showAttributes()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(seeCommentsButton, SIGNAL(clicked()), this, SLOT(showComments()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(coderComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(compareComboBox, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setCompareButton()));  
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotGraph()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(plotLabelsButton, SIGNAL(clicked()), this, SLOT(plotLabels()));
  connect(colorByAttributeButton, SIGNAL(clicked()), this, SLOT(colorByAttribute()));
  connect(eventColorButton, SIGNAL(clicked()), this, SLOT(setEventColor()));
  connect(labelColorButton, SIGNAL(clicked()), this, SLOT(setLabelColor()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(compareButton, SIGNAL(clicked()), this, SLOT(compare()));
  connect(scene, SIGNAL(widthIncreased(EventItem*)), this, SLOT(increaseWidth(EventItem*)));
  connect(scene, SIGNAL(widthDecreased(EventItem*)), this, SLOT(decreaseWidth(EventItem*)));
  connect(scene, SIGNAL(widthIncreased(MacroEvent*)), this, SLOT(increaseWidth(MacroEvent*)));
  connect(scene, SIGNAL(widthDecreased(MacroEvent*)), this, SLOT(decreaseWidth(MacroEvent*)));
  connect(scene, SIGNAL(posIncreased(EventItem*)), this, SLOT(increasePos(EventItem*)));
  connect(scene, SIGNAL(posDecreased(EventItem*)), this, SLOT(decreasePos(EventItem*)));
  connect(scene, SIGNAL(posIncreased(MacroEvent*)), this, SLOT(increasePos(MacroEvent*)));
  connect(scene, SIGNAL(posDecreased(MacroEvent*)), this, SLOT(decreasePos(MacroEvent*)));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(scene, SIGNAL(EventItemContextMenuAction(const QString &)),
	  this, SLOT(processEventItemContextMenu(const QString &)));
  connect(scene, SIGNAL(ArrowContextMenuAction(const QString &)),
	  this, SLOT(processArrowContextMenu(const QString &)));
  connect(previousEventButton, SIGNAL(clicked()), this, SLOT(previousDataItem()));
  connect(seeIncidentsButton, SIGNAL(clicked()), this, SLOT(seeIncidents()));
  connect(nextEventButton, SIGNAL(clicked()), this, SLOT(nextDataItem()));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
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
  detailsLayout->addWidget(indexLabel);
  indexLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  QPointer<QHBoxLayout> navigationLayout = new QHBoxLayout;
  navigationLayout->addWidget(previousEventButton);
  navigationLayout->addWidget(seeIncidentsButton);
  navigationLayout->addWidget(nextEventButton);
  detailsLayout->addLayout(navigationLayout);
  QPointer<QHBoxLayout> timeStampLayout = new QHBoxLayout;
  timeStampLayout->addWidget(timeStampLabel);
  timeStampLayout->addWidget(timeStampField);
  detailsLayout->addLayout(timeStampLayout);
  QPointer<QHBoxLayout> sourceLayout = new QHBoxLayout;
  sourceLayout->addWidget(sourceLabel);
  sourceLayout->addWidget(sourceField);
  detailsLayout->addLayout(sourceLayout);
  detailsLayout->addWidget(descriptionLabel);
  detailsLayout->addWidget(descriptionField);
  detailsLayout->addWidget(rawLabel);
  detailsLayout->addWidget(rawField);
  QPointer<QVBoxLayout> commentLayout = new QVBoxLayout;
  commentLayout->setContentsMargins(0,0,0,0);
  QPointer<QHBoxLayout> commentTopLayout = new QHBoxLayout;
  commentTopLayout->addWidget(commentLabel);
  commentTopLayout->addWidget(seeAttributesButton);
  commentTopLayout->setContentsMargins(0,0,0,0);
  commentLayout->addLayout(commentTopLayout);
  commentLayout->addWidget(commentField);
  commentWidget->setLayout(commentLayout);
  detailsLayout->addWidget(commentWidget);
  QPointer<QVBoxLayout> attributesLayout = new QVBoxLayout;
  attributesLayout->setContentsMargins(0,0,0,0);
  QPointer<QHBoxLayout> attributesTitleLayout = new QHBoxLayout;
  attributesTitleLayout->addWidget(attributesLabel);
  attributesTitleLayout->addWidget(seeCommentsButton);
  attributesLayout->addLayout(attributesTitleLayout);
  attributesLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributesFilterLabel);
  filterLayout->addWidget(attributesFilterField);
  attributesLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> valueLayout = new QHBoxLayout;
  valueLayout->addWidget(valueLabel);
  valueLayout->addWidget(valueField);
  valueLayout->addWidget(valueButton);
  attributesLayout->addLayout(valueLayout);
  QPointer<QHBoxLayout> attributesTopLayout = new QHBoxLayout;
  attributesTopLayout->addWidget(assignAttributeButton);
  attributesTopLayout->addWidget(unassignAttributeButton);
  attributesLayout->addLayout(attributesTopLayout);
  QPointer<QHBoxLayout> attributesBottomLayout = new QHBoxLayout;
  attributesBottomLayout->addWidget(addAttributeButton);
  attributesBottomLayout->addWidget(editAttributeButton);
  attributesBottomLayout->addWidget(removeUnusedAttributesButton);
  attributesLayout->addLayout(attributesBottomLayout);
  attWidget->setLayout(attributesLayout);
  detailsLayout->addWidget(attWidget);
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
  graphicsControlsLayout->addWidget(colorByAttributeButton);
  graphicsControlsLayout->addWidget(eventColorButton);
  graphicsControlsLayout->addWidget(labelColorButton);
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
  attWidget->hide();
}

void EventGraphWidget::setCommentBool() {
  commentBool = true;
}

void EventGraphWidget::setComment() {
  if (commentBool && selectedIncident != 0) {
    QString comment = commentField->toPlainText();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
    query->bindValue(":incident", selectedIncident);
    query->exec();
    query->first();
    int order = 0;
    order = query->value(0).toInt();
    query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :order");
    query->bindValue(":comment", comment);
    query->bindValue(":order", order);
    query->exec();
    commentBool = false;
    delete query;
  } else if (commentBool && selectedMacro != NULL) {
    QString comment = commentField->toPlainText();
    selectedMacro->setComment(comment);
  }
}

void EventGraphWidget::toggleDetails() {
  setComment();
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

void EventGraphWidget::showAttributes() {
  commentWidget->hide();
  attWidget->show();
}

void EventGraphWidget::showComments() {
  attWidget->hide();
  commentWidget->show();
}

void EventGraphWidget::retrieveData() {
  setComment();
  if (currentData.size() > 0) {
    currentData.clear();
  }
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
      if (event) {
	EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
	currentEvent->setSelectionColor(Qt::black);
	currentEvent->update();
	if (currentEvent->isVisible()) {
	  currentData.push_back(currentEvent);
	}
      }	else if (macro) {
	MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(it.next());
	currentMacro->setSelectionColor(Qt::black);
	currentMacro->update();
	if (currentMacro->isVisible()) {
	  currentData.push_back(currentMacro);
	}
      } else if (arrow && currentData.size() > 0) {
	scene->blockSignals(true);
	arrow->setSelected(false);
	scene->blockSignals(false);
	it.next();
          } else {
	it.next();
      }
    }
    if (currentData.size() > 0) {
      qSort(currentData.begin(), currentData.end(), eventLessThan);   
      vectorPos = 0;
      QString indexText = "(" + QString::number(vectorPos + 1) + "/"
	+ QString::number(currentData.size()) + ")";
      indexLabel->setText(indexText);
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if (currentEvent) {
	selectedMacro = NULL;
	seeIncidentsButton->setEnabled(false);
	timeStampLabel->setText("<b>Timing:</b>");
	sourceLabel->setText("<b>Source:</b>");
	rawLabel->show();
	rawField->show();

	currentEvent->setSelectionColor(Qt::red);
	currentEvent->update();
	int id = currentEvent->getId();
	selectedIncident = id;
	QSqlQuery *query = new QSqlQuery;
	query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		       "WHERE id = :id");
	query->bindValue(":id", id);
	query->exec();
	query->first();
	if (query->isNull(0)) {
	  timeStampField->setText("Incident was deleted");
	  descriptionField->setText("Incident was deleted");
	  rawField->setText("Incident was deleted");
	  commentField->setText("Incident was deleted");
	  sourceField->setText("Incident was deleted");
	} else {
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
	  resetFont(attributesTree);
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :id");
	  query->bindValue(":id", id);
	  query->exec();
	  while (query->next()) {
	    QString attribute = query->value(0).toString();
	    boldSelected(attributesTree, attribute, id, INCIDENT);
	  }
	}
	delete query;
      } else if (currentMacro) {
	selectedMacro = currentMacro;
	selectedIncident = 0;
	currentMacro->setSelectionColor(Qt::red);
	currentMacro->update();
	seeIncidentsButton->setEnabled(true);
	descriptionField->setText(currentMacro->getDescription());
	timeStampLabel->setText("<b>Duration:</b>");
	sourceLabel->setText("<b>Number of incidents:</b>");
	int id = currentMacro->getIncidents().first()->getId();
	rawLabel->hide();
	rawField->hide();
	QSqlQuery *query = new QSqlQuery;
	query->prepare("SELECT timestamp FROM incidents "
		       "WHERE id = :id");
	query->bindValue(":id", id);
	query->exec();
	query->first();
	QString begin = query->value(0).toString();
	id = currentMacro->getIncidents().last()->getId();
	query->prepare("SELECT timestamp FROM incidents "
		       "WHERE id = :id");
	query->bindValue(":id", id);
	query->exec();
	query->first();
	QString end = query->value(0).toString();
	QString duration =  "From " + begin + " to " + end;
	QString count = QString::number(currentMacro->getIncidents().size());
	timeStampField->setText(duration);
	sourceField->setText(count);
	commentField->setText(currentMacro->getComment());
	delete query;
	resetFont(attributesTree);
	QSet<QString> attributes = currentMacro->getAttributes();
	QSet<QString>::iterator it;
	id = currentMacro->getId();
	for (it = attributes.begin(); it != attributes.end(); it++) {
	  QString attribute  = *it;
	  boldSelected(attributesTree, attribute, id, MACRO);
	}
      }
      previousEventButton->setEnabled(true);
      nextEventButton->setEnabled(true);
    }
  } else {
    timeStampField->clear();
    descriptionField->clear();
    rawField->clear();
    commentField->blockSignals(true);
    commentField->clear();
    commentField->blockSignals(false);
    sourceField->clear();
    previousEventButton->setEnabled(false);
    nextEventButton->setEnabled(false);
    indexLabel->setText("(0/0)");
    selectedIncident = 0;
    selectedMacro = NULL;
    resetFont(attributesTree);
  }
}

void EventGraphWidget::seeIncidents() {
  QVector<EventItem*> tempIncidents = selectedMacro->getIncidents();
  QVectorIterator<EventItem*> it(tempIncidents);
  currentData.clear();
  while (it.hasNext()) {
    currentData.push_back(it.next());
  }
  qSort(currentData.begin(), currentData.end(), eventLessThan);   
  vectorPos = 0;
  QString indexText = "(" + QString::number(vectorPos + 1) + "/"
    + QString::number(currentData.size()) + ")";
  indexLabel->setText(indexText);
  EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
  currentEvent->getMacroEvent()->setSelectionColor(Qt::red);
  scene->blockSignals(true);
  currentEvent->getMacroEvent()->setSelected(true);
  scene->blockSignals(false);
  selectedMacro = NULL;
  seeIncidentsButton->setEnabled(false);
  timeStampLabel->setText("<b>Timing:</b>");
  sourceLabel->setText("<b>Source:</b>");
  rawLabel->show();
  rawField->show();
  currentEvent->setSelectionColor(Qt::red);
  currentEvent->update();
  int id = currentEvent->getId();
  selectedIncident = id;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		 "WHERE id = :id");
  query->bindValue(":id", id);
  query->exec();
  query->first();
  if (query->isNull(0)) {
    timeStampField->setText("Incident was deleted");
    descriptionField->setText("Incident was deleted");
    rawField->setText("Incident was deleted");
    commentField->setText("Incident was deleted");
    sourceField->setText("Incident was deleted");
  } else {
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
  }
  delete query;
}

void EventGraphWidget::previousDataItem() {
  setComment();
  if (vectorPos > 0) {
    EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if(currentEvent) {
      currentEvent->setSelectionColor(Qt::black);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::black);
      currentMacro->update();
    }
    vectorPos--;
    QString indexText = "(" + QString::number(vectorPos + 1) + "/"
      + QString::number(currentData.size()) + ")";
    indexLabel->setText(indexText);
    currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if (currentEvent) {
      currentEvent->setSelectionColor(Qt::red);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::red);
      currentMacro->update();
    }
    if (currentEvent) {
      selectedMacro = NULL;
      seeIncidentsButton->setEnabled(false);
      timeStampLabel->setText("<b>Timing:</b>");
      sourceLabel->setText("<b>Source:</b>");
      rawLabel->show();
      rawField->show();

      int id = currentEvent->getId();
      selectedIncident = id;
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      if (query->isNull(0)) {
	timeStampField->setText("Incident was deleted");
	descriptionField->setText("Incident was deleted");
	rawField->setText("Incident was deleted");
	commentField->setText("Incident was deleted");
	sourceField->setText("Incident was deleted");
      } else {
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
      }
      delete query;
    } else if (currentMacro) {
      selectedMacro = currentMacro;
      selectedIncident = 0;
      seeIncidentsButton->setEnabled(true);
      descriptionField->setText(currentMacro->getDescription());
      timeStampLabel->setText("<b>Duration:</b>");
      sourceLabel->setText("<b>Number of incidents:</b>");
      int id = currentMacro->getIncidents().first()->getId();
      rawLabel->hide();
      rawField->hide();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString begin = query->value(0).toString();
      id = currentMacro->getIncidents().last()->getId();
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString end = query->value(0).toString();
      QString duration =  "From " + begin + " to " + end;
      QString count = QString::number(currentMacro->getIncidents().size());
      timeStampField->setText(duration);
      sourceField->setText(count);
      commentField->setText(currentMacro->getComment());
      delete query;
    }
  } else {
    EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if(currentEvent) {
      currentEvent->setSelectionColor(Qt::black);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::black);
      currentMacro->update();
    }
    vectorPos = currentData.size() - 1;
    QString indexText = "(" + QString::number(vectorPos + 1) + "/"
      + QString::number(currentData.size()) + ")";
    indexLabel->setText(indexText);
    currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if (currentEvent) {
      currentEvent->setSelectionColor(Qt::red);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::red);
      currentMacro->update();
    }
    if (currentEvent) {
      selectedMacro = 0;
      seeIncidentsButton->setEnabled(false);
      timeStampLabel->setText("<b>Timing:</b>");
      sourceLabel->setText("<b>Source:</b>");
      rawLabel->show();
      rawField->show();
      
      int id = currentEvent->getId();
      selectedIncident = id;
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      if (query->isNull(0)) {
	timeStampField->setText("Incident was deleted");
	descriptionField->setText("Incident was deleted");
	rawField->setText("Incident was deleted");
	commentField->setText("Incident was deleted");
	sourceField->setText("Incident was deleted");
      } else {
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
      }
      delete query;
    } else if (currentMacro) {
      selectedMacro = currentMacro;
      selectedIncident = 0;
      seeIncidentsButton->setEnabled(true);
      descriptionField->setText(currentMacro->getDescription());
      timeStampLabel->setText("<b>Duration:</b>");
      sourceLabel->setText("<b>Number of incidents:</b>");
      int id = currentMacro->getIncidents().first()->getId();
      rawLabel->hide();
      rawField->hide();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString begin = query->value(0).toString();
      id = currentMacro->getIncidents().last()->getId();
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString end = query->value(0).toString();
      QString duration =  "From " + begin + " to " + end;
      QString count = QString::number(currentMacro->getIncidents().size());
      timeStampField->setText(duration);
      sourceField->setText(count);
      commentField->setText(currentMacro->getComment());
      delete query;
    }
  }
}

void EventGraphWidget::nextDataItem() {
  setComment();
  if (vectorPos != currentData.size() - 1) {
    EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if(currentEvent) {
      currentEvent->setSelectionColor(Qt::black);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::black);
      currentMacro->update();
    }
    vectorPos++;
    QString indexText = "(" + QString::number(vectorPos + 1) + "/"
      + QString::number(currentData.size()) + ")";
    indexLabel->setText(indexText);
    currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if (currentEvent) {
      currentEvent->setSelectionColor(Qt::red);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::red);
      currentMacro->update();
    }
    if (currentEvent) {
      selectedMacro = NULL;
      seeIncidentsButton->setEnabled(false);
      timeStampLabel->setText("<b>Timing:</b>");
      sourceLabel->setText("<b>Source:</b>");
      rawLabel->show();
      rawField->show();
      
      int id = currentEvent->getId();
      selectedIncident = id;
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      if (query->isNull(0)) {
	timeStampField->setText("Incident was deleted");
	descriptionField->setText("Incident was deleted");
	rawField->setText("Incident was deleted");
	commentField->setText("Incident was deleted");
	sourceField->setText("Incident was deleted");
      } else {
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
      }
      delete query;
    } else if (currentMacro) {
      selectedMacro = currentMacro;
      seeIncidentsButton->setEnabled(true);
      descriptionField->setText(currentMacro->getDescription());
      timeStampLabel->setText("<b>Duration:</b>");
      sourceLabel->setText("<b>Number of incidents:</b>");
      int id = currentMacro->getIncidents().first()->getId();
      rawLabel->hide();
      rawField->hide();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString begin = query->value(0).toString();
      id = currentMacro->getIncidents().last()->getId();
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString end = query->value(0).toString();
      QString duration =  "From " + begin + " to " + end;
      QString count = QString::number(currentMacro->getIncidents().size());
      timeStampField->setText(duration);
      sourceField->setText(count);
      commentField->setText(currentMacro->getComment());
      delete query;
    }
  } else {
    EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if(currentEvent) {
      currentEvent->setSelectionColor(Qt::black);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::black);
      currentMacro->update();
    }
    vectorPos = 0;
    QString indexText = "(" + QString::number(vectorPos + 1) + "/"
      + QString::number(currentData.size()) + ")";
    indexLabel->setText(indexText);
    currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
    currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
    if (currentEvent) {
      currentEvent->setSelectionColor(Qt::red);
      currentEvent->update();
    } else if (currentMacro) {
      currentMacro->setSelectionColor(Qt::red);
      currentMacro->update();
    }
    if (currentEvent) {
      selectedMacro = NULL;
      seeIncidentsButton->setEnabled(false);
      timeStampLabel->setText("<b>Timing:</b>");
      sourceLabel->setText("<b>Source:</b>");
      rawLabel->show();
      rawField->show();

      int id = currentEvent->getId();
      selectedIncident = id;
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      if (query->isNull(0)) {
	timeStampField->setText("Incident was deleted");
	descriptionField->setText("Incident was deleted");
	rawField->setText("Incident was deleted");
	commentField->setText("Incident was deleted");
	sourceField->setText("Incident was deleted");
      } else {
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
    } else if (currentMacro) {
      selectedMacro = currentMacro;
      selectedIncident = 0;
      seeIncidentsButton->setEnabled(true);
      descriptionField->setText(currentMacro->getDescription());
      timeStampLabel->setText("<b>Duration:</b>");
      sourceLabel->setText("<b>Number of incidents:</b>");
      int id = currentMacro->getIncidents().first()->getId();
      rawLabel->hide();
      rawField->hide();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString begin = query->value(0).toString();
      id = currentMacro->getIncidents().last()->getId();
      query->prepare("SELECT timestamp FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString end = query->value(0).toString();
      QString duration =  "From " + begin + " to " + end;
      QString count = QString::number(currentMacro->getIncidents().size());
      timeStampField->setText(duration);
      sourceField->setText(count);
      commentField->setText(currentMacro->getComment());
      delete query;
    }
  }
}

void EventGraphWidget::setTree() {
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE'");
  while (query->next()) {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *father = new QStandardItem(name);    
    attributesTree->appendRow(father);
    QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
    father->setToolTip(hint);
    father->setEditable(false);
    buildHierarchy(father, name);
  }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void EventGraphWidget::buildHierarchy(QStandardItem *top, QString name) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) {
    QString childName = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *child = new QStandardItem(childName);
    top->setChild(children, child);
    QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
    child->setToolTip(hint);
    child->setEditable(false);
    children++;
    buildHierarchy(child, childName);
  }
  delete query;
}

void EventGraphWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) {
  for(int i = 0; i != model->rowCount(parent); i++) {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
    QFont font;
    font.setBold(false);
    font.setItalic(false);
    currentAttribute->setFont(font);
    if (model->hasChildren(index)) {
      resetFont(model, index);
    }
  }
}

void EventGraphWidget::boldSelected(QAbstractItemModel *model, QString name,
				    int event, QString type, QModelIndex parent) {
  for(int i = 0; i != model->rowCount(parent); i++) {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
    QFont font;
    font.setBold(true);
    QFont font2;
    font2.setItalic(true);
    QFont font3;
    font3.setBold(true);
    font3.setItalic(true);
    if (name == currentName) {
      if (currentAttribute->font().italic()) {
	currentAttribute->setFont(font3);
      } else {
	currentAttribute->setFont(font);
      }
      if (currentAttribute->parent()) {
	while (currentAttribute->parent()) {
          currentAttribute = currentAttribute->parent();
	  QString parentName = currentAttribute->data(Qt::DisplayRole).toString();
	  if (type == INCIDENT) {
	    QSqlQuery *query = new QSqlQuery;
	    query->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			   "WHERE attribute = :attribute AND incident = :incident");
	    query->bindValue(":attribute", parentName);
	    query->bindValue(":incident", event);
	    query->exec();
	    query->first();
	    if (query->isNull(0)) {
	      currentAttribute->setFont(font2);      
	    } else {
	      currentAttribute->setFont(font3);
	    }
	  } else if (type == MACRO) {
	    QSet<QString> attributes = selectedMacro->getAttributes();
	    QSet<QString>::iterator it;
	    bool found = false;
	    for (it = attributes.begin(); it != attributes.end(); it++) {
	      QString current = *it;
	      if (current == parentName) {
		found = true;
	      }
	    }
	    if (!found) {
	      currentAttribute->setFont(font2);      
	    } else {
	      currentAttribute->setFont(font3);
	    }
	  }
        }
      }
    }
    if (model->hasChildren(index)) {
      boldSelected(model, name, event, type, index);
    }
  }
}

void EventGraphWidget::assignAttribute() {
  if (selectedIncident != 0) {
    if (attributesTreeView->currentIndex().isValid()) {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      QSqlQuery *query = new QSqlQuery;
      bool empty = false;
      query->prepare("SELECT attribute FROM "
		     "attributes_to_incidents WHERE "
		     "attribute = :att AND incident = :incident");
      query->bindValue(":att", attribute);
      query->bindValue(":incident", selectedIncident);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (empty) {
	query->prepare("INSERT INTO attributes_to_incidents (attribute, incident) "
		       "VALUES (:attribute, :incident)");
	query->bindValue(":attribute", attribute);
	query->bindValue(":incident", selectedIncident);
	query->exec();
	boldSelected(attributesTree, attribute, selectedIncident, INCIDENT);
	valueField->setEnabled(true);
      }
      delete query;
    }
  } else if (selectedMacro != NULL) {
    if (attributesTreeView->currentIndex().isValid()) {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      QSet<QString> attributes = selectedMacro->getAttributes();
      if (!attributes.contains(attribute)) {
	selectedMacro->insertAttribute(attribute);
	boldSelected(attributesTree, attribute, selectedMacro->getId(), MACRO);
	valueField->setEnabled(true);
      }
    }
  }
}

void EventGraphWidget::unassignAttribute() {
  if (selectedIncident != 0) {
    if (attributesTreeView->currentIndex().isValid()) {
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2 = new QSqlQuery;
      QString attribute = attributesTreeView->currentIndex().data().toString();
      bool empty = false;
      query->prepare("SELECT attribute, incident "
		     "FROM attributes_to_incidents "
		     "WHERE attribute = :att AND incident = :incident");
      query->bindValue(":att", attribute);
      query->bindValue(":incident", selectedIncident);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	query->prepare("DELETE FROM attributes_to_incidents "
		       "WHERE attribute = :att AND incident = :incident");
	query->bindValue(":att", attribute);
	query->bindValue(":incident", selectedIncident);
	query->exec();
	query2->prepare("DELETE FROM attributes_to_incidents_sources "
			"WHERE attribute = :att AND incident = :incident");
        query2->bindValue(":att", attribute);
        query2->bindValue(":inc", selectedIncident);
	query2->exec();
	resetFont(attributesTree);
	query2->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			"WHERE incident = :incident");
	query2->bindValue(":incident", selectedIncident);
	query2->exec();
	while (query2->next()) {
	  QString attribute = query2->value(0).toString();
	  boldSelected(attributesTree, attribute, selectedIncident, INCIDENT);
	}
	valueField->setText("");
	valueField->setEnabled(false);
	valueButton->setEnabled(false);
      }
      delete query;
      delete query2;
    }
  } else if (selectedMacro != NULL) {
    if (attributesTreeView->currentIndex().isValid()) {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      QSet<QString> attributes = selectedMacro->getAttributes();
      if (attributes.contains(attribute)) {
	selectedMacro->removeAttribute(attribute);
	QSet<QString>::iterator it;
	resetFont(attributesTree);
	attributes = selectedMacro->getAttributes();
	for (it = attributes.begin(); it != attributes.end(); it++) {
	  QString current = *it;
	  boldSelected(attributesTree, current, selectedMacro->getId(), MACRO);	  
	}
	valueField->setText("");
	valueField->setEnabled(false);
	valueButton->setEnabled(false);
      }
    }
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
    EventItem *currentItem = new EventItem(40, toolTip, position, id);
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

void EventGraphWidget::getLabels() {
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    int id = currentItem->getId();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT ch_order, description FROM incidents "
		   "WHERE id = :id");
    query->bindValue(":id", id);
    query->exec();
    query->first();
    QString order = query->value(0).toString();
    QPointF currentPos = currentItem->scenePos();
    QPointer<NodeLabel> text = new NodeLabel(currentItem);
    currentItem->setLabel(text);
    text->setPlainText(order);
    text->setTextWidth(text->boundingRect().width());
    currentPos.setX(currentPos.x() - (text->textWidth() / 2));
    currentPos.setY(currentPos.y() -12);
    text->setPos(currentPos);    
    text->setZValue(2);
    text->setDefaultTextColor(Qt::black);
    nodeLabelVector.push_back(text);
    delete query;
  }
}

void EventGraphWidget::addLabels() {
  QVectorIterator<NodeLabel*> it2(nodeLabelVector);
  while (it2.hasNext()) {
    NodeLabel *currentItem = it2.next();
    scene->addItem(currentItem);
  }
}

void EventGraphWidget::cleanUp() {
  scene->clearSelection();
  currentData.clear();
  qDeleteAll(macroVector);
  macroVector.clear();
  qDeleteAll(macroLabelVector);
  macroLabelVector.clear();
  qDeleteAll(eventVector);
  eventVector.clear();
  qDeleteAll(edgeVector);
  edgeVector.clear();
  qDeleteAll(nodeLabelVector);
  nodeLabelVector.clear();
  qDeleteAll(compareVector);
  compareVector.clear();
  selectedType = "";
  selectedCoder = "";
  selectedCompare = "";
  selectedIncident = 0;
  selectedMacro = NULL;
}

void EventGraphWidget::increaseWidth(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  NodeLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original, 0.5);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::increaseWidth(MacroEvent *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  MacroLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original, 0.5);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}


void EventGraphWidget::decreaseWidth(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  NodeLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original, -0.5);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::decreaseWidth(MacroEvent *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  MacroLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original, -0.5);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}


void EventGraphWidget::increasePos(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  NodeLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::increasePos(MacroEvent *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  MacroLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}


void EventGraphWidget::decreasePos(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  NodeLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::decreasePos(MacroEvent *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  MacroLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      NodeLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) {
    MacroEvent *currentItem = it2.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      MacroLabel *currentLabel = currentItem->getLabel();
      currentLabel->setNewPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::increaseDistance() {
  QVector<QGraphicsItem*> temp;
  QVectorIterator<EventItem*> it(eventVector);
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it.hasNext()) {
    temp.push_back(it.next());
  }
  while (it2.hasNext()) {
    temp.push_back(it2.next());
  }
  qSort(temp.begin(), temp.end(), eventLessThan);
  QVectorIterator<QGraphicsItem*> it3(temp);  
  int unit = 1;
  if (distance < 100) {
    while (it3.hasNext()) {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it3.peekNext());
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it3.peekNext());
      if (event) {
	event = qgraphicsitem_cast<EventItem*>(it3.next());
	QPointF newPos = QPointF(event->pos().x() + unit, event->pos().y());
	NodeLabel *currentLabel = event->getLabel();
	currentLabel->setNewPos(newPos);
	event->setOriginalPos(newPos);
	event->setPos(newPos);
	unit++;
      } else if (macro) {
	macro = qgraphicsitem_cast<MacroEvent*>(it3.next());
	QPointF newPos = QPointF(macro->pos().x() + unit, macro->pos().y());
	MacroLabel *currentLabel = macro->getLabel();
	currentLabel->setNewPos(newPos);
	macro->setOriginalPos(newPos);
	macro->setPos(newPos);
	unit++;
      }
    }
  }
  distance++;
}

void EventGraphWidget::decreaseDistance() {
  QVector<QGraphicsItem*> temp;
  QVectorIterator<EventItem*> it(eventVector);
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it.hasNext()) {
    temp.push_back(it.next());
  }
  while (it2.hasNext()) {
    temp.push_back(it2.next());
  }
  qSort(temp.begin(), temp.end(), eventLessThan);
  QVectorIterator<QGraphicsItem*> it3(temp);  
  int unit = 1;
  if (distance > 35) {
    while (it3.hasNext()) {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it3.peekNext());
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it3.peekNext());
      if (event) {
	event = qgraphicsitem_cast<EventItem*>(it3.next());
	QPointF newPos = QPointF(event->pos().x() - unit, event->pos().y());
	NodeLabel *currentLabel = event->getLabel();
	currentLabel->setNewPos(newPos);
	event->setOriginalPos(newPos);
	event->setPos(newPos);
	unit++;
      } else if (macro) {
	macro = qgraphicsitem_cast<MacroEvent*>(it3.next());
	QPointF newPos = QPointF(macro->pos().x() - unit, macro->pos().y());
	MacroLabel *currentLabel = macro->getLabel();
	currentLabel->setNewPos(newPos);
	macro->setOriginalPos(newPos);
	macro->setPos(newPos);
	unit++;
      }
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
  getLabels();
  addLabels();
  compareComboBox->setCurrentIndex(0);
  savePlotButton->setEnabled(true);
  setRangeControls();
  plotLabel->setText("Unsaved plot");
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
	  EventItem *tempS = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	  EventItem *tempT = qgraphicsitem_cast<EventItem*>(currentEdge->endItem());
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
  QVectorIterator<Arrow*> it2(edgeVector);
  while (it2.hasNext()) {
    Arrow *currentEdge = it2.next();
    if (currentEdge->getColor() == QColor(Qt::black)) {
      currentEdge->setColor(Qt::darkMagenta);
    } else {
      currentEdge->setColor(Qt::black);
    }
  }
}

void EventGraphWidget::saveCurrentPlot() {
  QPointer<SimpleTextDialog> saveDialog = new SimpleTextDialog(this);
  saveDialog->setWindowTitle("Save current plot");
  saveDialog->submitText(plotLabel->text());
  saveDialog->setLabel("Plot name:");
  saveDialog->exec();
  if (saveDialog->getExitStatus() == 0) {
    QString name = saveDialog->getText();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT plot FROM saved_eg_plots WHERE plot = :name");
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
      // Reset plot name and details, and clear out all data before writing.
      query->prepare("UPDATE saved_eg_plots SET linkage = :linkage, coder = :coder "
		     "WHERE plot = :name");
      query->bindValue(":linkage", selectedType);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":name", name);
      query->exec();
      // saved_eg_plots_event_items
      query->prepare("DELETE FROM saved_eg_plots_event_items "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_eg_plots_edges
      query->prepare("DELETE FROM saved_eg_plots_edges "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_eg_plots_event_labels
      query->prepare("DELETE FROM saved_eg_plots_event_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_eg_plots_macro_events
      query->prepare("DELETE FROM saved_eg_plots_macro_events "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_eg_plots_incidents_to_macro_events
      query->prepare("DELETE FROM saved_eg_plots_incidents_to_macro_events "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_eg_plots_attributes_to_macro_events
      query->prepare("DELETE FROM saved_eg_plots_incidents_to_macro_events "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_eg_plots_macro_labels
      query->prepare("DELETE FROM saved_eg_plots_macro_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
    } else {
      // Insert new data into saved_eg_plots and then write data.
      query->prepare("INSERT INTO saved_eg_plots (plot, linkage, coder) "
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
      qreal currentX = currentItem->pos().x();
      qreal currentY = currentItem->pos().y();
      qreal originalX = currentItem->getOriginalPos().x();
      qreal originalY = currentItem->getOriginalPos().y();
      int incident = currentItem->getId();
      int width = currentItem->getWidth();
      int dislodged = 0;
      QColor color = currentItem->getColor();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      int alpha = color.alpha();
      int hidden = 1;
      if (currentItem->isDislodged()) {
	dislodged = 1;
      }
      if (currentItem->isVisible()) {
	hidden = 0;
      }
      query->prepare("INSERT INTO saved_eg_plots_event_items "
		     "(plot, incident, width, curxpos, curypos, orixpos, oriypos, dislodged, red, "
		     "green, blue, alpha, hidden) "
		     "VALUES (:plot, :incident, :width, :curxpos, :curypos, :orixpos, "
		     ":oriypos, :dislodged, :red, :green, :blue, :alpha, :hidden)");
      query->bindValue(":plot", name);
      query->bindValue(":incident", incident);
      query->bindValue(":width", width);
      query->bindValue(":curxpos", currentX);
      query->bindValue(":curypos", currentY);
      query->bindValue(":orixpos", originalX);
      query->bindValue(":oriypos", originalY);
      query->bindValue(":dislodged", dislodged);
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
    saveProgress = new ProgressBar(0, 1, nodeLabelVector.size());
    saveProgress->setWindowTitle("Saving node labels");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    QVectorIterator<NodeLabel*> it2(nodeLabelVector);
    while (it2.hasNext()) {
      NodeLabel *currentLabel = it2.next();
      int id = currentLabel->getNode()->getId();
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
      query->prepare("INSERT INTO saved_eg_plots_event_labels "
		     "(plot, incident, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden) "
		     "VALUES (:plot, :incident, :label, :curxpos, :curypos, "
		     ":xoffset, :yoffset, :red, :green, :blue, :alpha, :hidden)");
      query->bindValue(":plot", name);
      query->bindValue(":incident", id);
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
    saveProgress = new ProgressBar(0, 1, edgeVector.size());
    saveProgress->setWindowTitle("Saving edge items");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();

    QVectorIterator<Arrow*> it3(edgeVector);
    while (it3.hasNext()) {
      Arrow *currentEdge = it3.next();
      EventItem *eventTail = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
      EventItem *eventHead = qgraphicsitem_cast<EventItem*>(currentEdge->endItem());
      MacroEvent *macroTail = qgraphicsitem_cast<MacroEvent*>(currentEdge->startItem());
      MacroEvent *macroHead = qgraphicsitem_cast<MacroEvent*>(currentEdge->endItem());
      int tail = 0;
      int head = 0;
      int mTail = 0;
      int mHead = 0;
      int hidden = 1;
      if (eventTail) {
	tail = eventTail->getId();
      } else if (macroTail) {
	tail = macroTail->getId();
	mTail = 1;
      }
      if (eventHead) {
	head = eventHead->getId();
      } else if (macroHead) {
	head = macroHead->getId();
	mHead = 1;
      }
      if (currentEdge->isVisible()) {
	hidden = 0;
      }
      query->prepare("INSERT INTO saved_eg_plots_edges "
		     "(plot, tail, head, tailmacro, headmacro, hidden) "
		     "VALUES (:plot, :tail, :head, :tmacro, :hmacro, :hidden)");
      query->bindValue(":plot", name);
      query->bindValue(":tail", tail);
      query->bindValue(":head", head);
      query->bindValue(":tmacro", mTail);
      query->bindValue(":hmacro", mHead);
      query->bindValue(":hidden", hidden);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, macroVector.size());
    saveProgress->setWindowTitle("Saving macro events");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    QVectorIterator<MacroEvent*> it4(macroVector);
    while (it4.hasNext()) {
      MacroEvent *currentMacro = it4.next();
      QVector<EventItem*> incidents = currentMacro->getIncidents();
      QVectorIterator<EventItem*> tit(incidents);
      while (tit.hasNext()) {
	EventItem* currentIncident = tit.next();
	query->prepare("INSERT INTO saved_eg_plots_incidents_to_macro_events "
		       "(plot, incident, macro) "
		       "VALUES (:plot, :incident, :macro)");
	query->bindValue(":plot", name);
	query->bindValue(":incident", currentIncident->getId());
	query->bindValue(":macro", currentMacro->getId());
	query->exec();
      }
      QSet<QString> attributes = currentMacro->getAttributes();
      QMap<QString, QString> values = currentMacro->getValues();
      QSet<QString>::iterator tit2;
      for (tit2 = attributes.begin(); tit2 != attributes.end(); tit2++) {
	QString attribute = *tit2;
	QString value = values.value(attribute);
	query->prepare("INSERT INTO saved_eg_plots_attributes_to_macro_events "
		       "(plot, attribute, macro, value) "
		       "VALUES(:plot, :attribute, :macro, :value)");
	query->bindValue(":plot", name);
	query->bindValue(":attribute", attribute);
	query->bindValue(":macro", currentMacro->getId());
	query->bindValue(":value", value);
	query->exec();
      }
      QString description = currentMacro->getDescription();
      QString comment = currentMacro->getComment();
      int width = currentMacro->getWidth();
      qreal currentX = currentMacro->pos().x();
      qreal currentY = currentMacro->pos().y();
      qreal originalX = currentMacro->getOriginalPos().x();
      qreal originalY = currentMacro->getOriginalPos().y();
      int dislodged = 0;
      QColor color = currentMacro->getColor();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      int alpha = color.alpha();
      int hidden = 1;
      if (currentMacro->isDislodged()) {
	dislodged = 1;
      }
      if (currentMacro->isVisible()) {
	hidden = 0;
      }
      query->prepare("INSERT INTO saved_eg_plots_macro_events "
		     "(plot, eventid, ch_order, description, comment, width, curxpos, curypos, "
		     "orixpos, oriypos, dislodged, red, green, blue, alpha, hidden) "
		     "VALUES (:plot, :eventid, :ch_order, :description, :comment, :width, "
		     ":curxpos, :curypos, :orixpos, :oriypos, :dislodged, "
		     ":red, :green, :blue, :alpha, :hidden)");;
      query->bindValue(":plot", name);
      query->bindValue(":eventid", currentMacro->getId());
      query->bindValue(":ch_order", currentMacro->getOrder());
      query->bindValue(":description", description);
      query->bindValue(":comment", comment);
      query->bindValue(":width", width);
      query->bindValue(":curxpos", currentX);
      query->bindValue(":curypos", currentY);
      query->bindValue(":orixpos", originalX);
      query->bindValue(":oriypos", originalY);
      query->bindValue(":dislodged", dislodged);
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
    saveProgress = new ProgressBar(0, 1, macroLabelVector.size());
    saveProgress->setWindowTitle("Saving macro labels");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    QVectorIterator<MacroLabel*> it5(macroLabelVector);
    while (it5.hasNext()) {
      MacroLabel *currentLabel = it5.next();
      int id = currentLabel->getMacroEvent()->getId();
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
      query->prepare("INSERT INTO saved_eg_plots_macro_labels "
		     "(plot, eventid, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden) "
		     "VALUES (:plot, :eventid, :label, :curxpos, :curypos, :xoffset, :yoffset, "
		     ":red, :green, :blue, :alpha, :hidden)");
      query->bindValue(":plot", name);
      query->bindValue(":eventid", id);
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
    plotLabel->setText(name);
    changeLabel->setText("");
    delete saveProgress;
    delete query;
  }
  delete saveDialog;
}

void EventGraphWidget::seePlots() {
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this, EVENTGRAPH);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) {
    savePlotButton->setEnabled(true);
    cleanUp();
    scene->clear();
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT linkage, coder FROM saved_eg_plots "
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
    query->prepare("SELECT incident, width, curxpos, curypos, orixpos, oriypos, dislodged, red, "
		   "green, blue, alpha, hidden "
		   "FROM saved_eg_plots_event_items "
		   "WHERE plot = :plot ");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int id = query->value(0).toInt();
      int width = query->value(1).toInt();
      qreal currentX = query->value(2).toReal();
      qreal currentY = query->value(3).toReal();
      qreal originalX = query->value(4).toReal();
      qreal originalY = query->value(5).toReal();
      int dislodged = query->value(6).toInt();
      int red = query->value(7).toInt();
      int green = query->value(8).toInt();
      int blue = query->value(9).toInt();
      int alpha = query->value(10).toInt();
      int hidden = query->value(11).toInt();
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
      currentItem->setColor(QColor(red, green, blue, alpha));
      currentItem->setZValue(1);
      eventVector.push_back(currentItem);
      scene->addItem(currentItem);
      if (dislodged == 1) {
	currentItem->setDislodged(true);
      }
      if (hidden == 1) {
	currentItem->hide();
      } else {
	currentItem->show();
      }
    }
    query->prepare("SELECT incident, label, curxpos, curypos, xoffset, yoffset, "
		   "red, green, blue, alpha, hidden "
		   "FROM saved_eg_plots_event_labels "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int id = query->value(0).toInt();
      QString text = query->value(1).toString();
      qreal currentX = query->value(2).toReal();
      qreal currentY = query->value(3).toReal();
      qreal xOffset = query->value(4).toReal();
      qreal yOffset = query->value(5).toReal();
      int red = query->value(6).toInt();
      int green = query->value(7).toInt();
      int blue = query->value(8).toInt();
      int alpha = query->value(9).toInt();
      int hidden = query->value(10).toInt();
      QVectorIterator<EventItem*> it(eventVector);
      while (it.hasNext()) {
	EventItem *currentItem = it.next();
	int nodeId = currentItem->getId();
	if (nodeId == id) {
	  NodeLabel *currentLabel = new NodeLabel(currentItem);
	  currentLabel->setPlainText(text);
	  currentLabel->setTextWidth(currentLabel->boundingRect().width());
	  currentLabel->setPos(QPointF(currentX, currentY));
	  currentLabel->setOffset(QPointF(xOffset, yOffset));
	  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
	  currentLabel->setZValue(2);
	  currentItem->setLabel(currentLabel);
	  nodeLabelVector.push_back(currentLabel);
	  scene->addItem(currentLabel);
	  if (hidden == 1) {
	    currentLabel->hide();
	  } else {
	    currentLabel->show();
	  }
	  break;
	}
      }
    }
    query->prepare("SELECT eventid, ch_order, description, comment, width, curxpos, curypos, "
		   "orixpos, oriypos, dislodged, red, green, blue, alpha, hidden "
		   "FROM saved_eg_plots_macro_events "
		   "WHERE plot = :plot ");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int id = query->value(0).toInt();
      int order = query->value(1).toInt();
      QString description = query->value(2).toString();
      QString comment = query->value(3).toString();
      int width = query->value(4).toInt();
      qreal currentX = query->value(5).toReal();
      qreal currentY = query->value(6).toReal();
      qreal originalX = query->value(7).toReal();
      qreal originalY = query->value(8).toReal();
      int dislodged = query->value(9).toInt();
      int red = query->value(10).toInt();
      int green = query->value(11).toInt();
      int blue = query->value(12).toInt();
      int alpha = query->value(13).toInt();
      int hidden = query->value(14).toInt();
      QPointF currentPos = QPointF(currentX, currentY);
      QPointF originalPos = QPointF(originalX, originalY);
      QColor color = QColor(red, green, blue, alpha);
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT incident FROM saved_eg_plots_incidents_to_macro_events "
		      "WHERE plot = :plot AND macro = :id");
      query2->bindValue(":plot", plot);
      query2->bindValue(":id", id);
      query2->exec();
      QVector<EventItem*> incidents;
      while (query2->next()) {
	int incidentId = query2->value(0).toInt();
	QVectorIterator<EventItem*> it(eventVector);
	while (it.hasNext()) {
	  EventItem *currentEvent = it.next();
	  if (incidentId == currentEvent->getId()) {
	    incidents.push_back(currentEvent);
	    break;
	  }
	}
      }
      qSort(incidents.begin(), incidents.end(), eventLessThan);
      query2->prepare("SELECT attribute, value FROM saved_eg_plots_attributes_to_macro_events "
		      "WHERE plot = :plot AND macro = :id");
      query2->bindValue(":plot", plot);
      query2->bindValue(":id", id);
      query2->exec();
      QSet<QString> attributes;
      QMap<QString, QString> values;
      while (query2->next()) {
	QString attribute = query2->value(0).toString();
	QString value = query2->value(1).toString();
	attributes.insert(attribute);
	values.insert(attribute, value);
      }
      delete query2;
      MacroEvent* newMacro = new MacroEvent(width, description, originalPos, macroVector.size(),
					    incidents);
      QVectorIterator<EventItem*> it(incidents);
      while (it.hasNext()) {
	EventItem *currentEvent = it.next();
	currentEvent->setMacroEvent(newMacro);
	currentEvent->hide();
      }
      newMacro->setOriginalPos(originalPos);
      newMacro->setPos(currentPos);
      newMacro->setZValue(1);
      newMacro->setColor(color);
      newMacro->setAttributes(attributes);
      newMacro->setValues(values);
      newMacro->setOrder(order);
      if (hidden == 1) {
	newMacro->hide();
      }
      if (dislodged == 1) {
	newMacro->setDislodged(true);
      }
      scene->addItem(newMacro);
      macroVector.push_back(newMacro);
    }
    query->prepare("SELECT eventid, label, curxpos, curypos, xoffset, yoffset, "
		   "red, green, blue, alpha, hidden "
		   "FROM saved_eg_plots_macro_labels "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int event = query->value(0).toInt();
      QString text = query->value(1).toString();
      qreal currentX = query->value(2).toReal();
      qreal currentY = query->value(3).toReal();
      qreal xOffset = query->value(4).toReal();
      qreal yOffset = query->value(5).toReal();
      int red = query->value(6).toInt();
      int green = query->value(7).toInt();
      int blue = query->value(8).toInt();
      int alpha = query->value(9).toInt();
      int hidden = query->value(10).toInt();
      QVectorIterator<MacroEvent*> it(macroVector);
      while (it.hasNext()) {
	MacroEvent *currentMacro = it.next();
	int eventId = currentMacro->getId();
	if (eventId == event) {
	  MacroLabel *currentLabel = new MacroLabel(currentMacro);
	  currentLabel->setPlainText(text);
	  currentLabel->setTextWidth(currentLabel->boundingRect().width());
	  currentLabel->setPos(QPointF(currentX, currentY));
	  currentLabel->setOffset(QPointF(xOffset, yOffset));
	  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
	  currentLabel->setZValue(2);
	  currentMacro->setLabel(currentLabel);
	  macroLabelVector.push_back(currentLabel);
	  scene->addItem(currentLabel);
	  if (hidden == 1) {
	    currentLabel->hide();
	  } else {
	    currentLabel->show();
	  }
	  break;
	}
      }
    }
    qSort(macroVector.begin(), macroVector.end(), eventLessThan);
    query->prepare("SELECT tail, head, tailmacro, headmacro, hidden "
		   "FROM saved_eg_plots_edges "
		   "WHERE plot = :plot ");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      int tail = query->value(0).toInt();
      int head = query->value(1).toInt();
      int tM = query->value(2).toInt();
      int hM = query->value(3).toInt();
      int hidden = query->value(4).toInt();
      bool tailMacro = false;
      bool headMacro = false;
      if (tM == 1) {
	tailMacro = true;
      }
      if (hM == 1) {
	headMacro = true;
      }
      QString toolTip = "";
      if (!(tailMacro) && !(headMacro)) {
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
	if (!(query2->isNull(0))) {
	  comment = query2->value(0).toString();
	  commentCoder = query2->value(1).toString();
	  toolTip = "<FONT SIZE = 3>" + commentCoder + " - " + comment + "</FONT>";
	} else {
	  toolTip = "No comments";
	}
	delete query2;
      }
      QVectorIterator<EventItem*> it(eventVector);
      QVectorIterator<MacroEvent*> it2(macroVector);
      QGraphicsItem *tempSource = NULL;
      QGraphicsItem *tempTarget = NULL;
      if (tailMacro && !(headMacro)) {
	while (it2.hasNext()) {
	  MacroEvent *currentSource = it2.next();
	  if (currentSource->getId() == tail) {
	    tempSource = currentSource;
	  }
	}
	while (it.hasNext()) {
	  EventItem *currentTarget = it.next();
	  if (currentTarget->getId() == head) {
	    tempTarget = currentTarget;
	  }
	}
      } else if (!(tailMacro) && headMacro) {
	while (it.hasNext()) {
	  EventItem *currentSource = it.next();
	  if (currentSource->getId() == tail) {
	    tempSource = currentSource;
	  }
	}
	while (it2.hasNext()) {
	  MacroEvent *currentTarget = it2.next();
	  if (currentTarget->getId() == head) {
	    tempTarget = currentTarget;
	  }
	}
      } else if (tailMacro && headMacro) {
	while (it2.hasNext()) {
	  MacroEvent *currentSource = it2.next();
	  if (currentSource->getId() == tail) {
	    tempSource = currentSource;
	  }
	}
	it2.toFront();
	while (it2.hasNext()) {
	  MacroEvent *currentTarget = it2.next();
	  if (currentTarget->getId() == head) {
	    tempTarget = currentTarget;
	  }
	}
      } else if (!(tailMacro) && !(headMacro)) {
	while (it.hasNext()) {
	  EventItem *currentSource = it.next();
	  if (currentSource->getId() == tail) {
	    tempSource = currentSource;
	  }
	}
	it.toFront();
	while (it.hasNext()) {
	  EventItem *currentTarget = it.next();
	  if (currentTarget->getId() == head) {
	    tempTarget = currentTarget;
	  }
	}
      }
      if (tempSource != NULL && tempTarget != NULL) {
	Arrow *currentEdge = new Arrow(tempSource, tempTarget, selectedType, selectedCoder);
	currentEdge->setToolTip(toolTip);
	if (hidden == 1) {
	  currentEdge->hide();
	} else {
	  currentEdge->show();
	}
	edgeVector.push_back(currentEdge);
	scene->addItem(currentEdge);
      }
    }
     distance = 70;
   plotLabel->setText(plot);
   changeLabel->setText("");
   scene->update();
   setVisibility();
   setRangeControls();
   delete query;
  } else if (savedPlotsDialog->getExitStatus() == 2) {
    // DON'T FORGET TO UPDATE THIS FUNCTION!!!!
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    // saved_eg_plots
    query->prepare("DELETE FROM saved_eg_plots "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_eg_plots_event_items
    query->prepare("DELETE FROM saved_eg_plots_event_items "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_eg_plots_edges
    query->prepare("DELETE FROM saved_eg_plots_edges "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    // saved_eg_plots_event_labels
    query->prepare("DELETE FROM saved_eg_plots_event_labels "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_eg_plots_macro_events
    query->prepare("DELETE FROM saved_eg_plots_macro_events "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_eg_plots_incidents_to_macro_events
    query->prepare("DELETE FROM saved_eg_plots_incidents_to_macro_events "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_eg_plots_attributes_to_macro_events
    query->prepare("DELETE FROM saved_eg_plots_incidents_to_macro_events "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_eg_plots_macro_labels
    query->prepare("DELETE FROM saved_eg_plots_macro_labels "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    delete query;
    seePlots();
  }
}

void EventGraphWidget::setChangeLabel() {
  if (changeLabel->text() == "" && eventVector.size() > 0) {
    changeLabel->setText("*");
  }
}

void EventGraphWidget::colorByAttribute() {
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, INCIDENT);
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) {
    QColor color = attributeColorDialog->getColor();
    QColor textColor = attributeColorDialog->getTextColor();
    QString attribute = attributeColorDialog->getAttribute();
    QVector<QString> attributeVector;
    attributeVector.push_back(attribute);
    findChildren(attribute, &attributeVector);
    QVectorIterator<QString> it(attributeVector);
    while (it.hasNext()) {
      QString currentAttribute = it.next();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT incident FROM attributes_to_incidents "
		     "WHERE attribute = :currentAttribute");
      query->bindValue(":currentAttribute", currentAttribute);
      query->exec();
      while (query->next()) {
	int currentIncident = query->value(0).toInt();
	QVectorIterator<EventItem*> it2(eventVector);
	while (it2.hasNext()) {
	  EventItem* currentEvent = it2.next();
	  if (currentEvent->getId() == currentIncident) {
	    currentEvent->setColor(color);
	    currentEvent->getLabel()->setDefaultTextColor(textColor);
	  }
	}
      }
      delete query;
      QVectorIterator<MacroEvent*> it3(macroVector);
      while (it3.hasNext()) {
	MacroEvent *currentMacro = it3.next();
	QSet<QString> attributes = currentMacro->getAttributes();
	if (attributes.contains(currentAttribute)) {
	  currentMacro->setColor(color);
	  currentMacro->getLabel()->setDefaultTextColor(textColor);
	}
      }
    }
  }
  delete attributeColorDialog;
}


void EventGraphWidget::findChildren(QString father, QVector<QString> *children) {
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

void EventGraphWidget::setEventColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    QColor color = colorDialog->selectedColor();
    QVectorIterator<EventItem*> it(eventVector);
    while (it.hasNext()) {
      EventItem *currentEvent = it.next();
      currentEvent->setColor(color);
    }
  }
  delete colorDialog;
}

void EventGraphWidget::setLabelColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    QColor color = colorDialog->selectedColor();
    QVectorIterator<NodeLabel*> it(nodeLabelVector);
    while (it.hasNext()) {
      NodeLabel *currentItem = it.next();
      currentItem->setDefaultTextColor(color);
    }
  }
  delete colorDialog;
}

void EventGraphWidget::setBackgroundColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    QColor color = colorDialog->selectedColor();
    view->setBackgroundBrush(color);
  }
  delete colorDialog;
}

void EventGraphWidget::plotLabels() {
  QVectorIterator<NodeLabel*> it(nodeLabelVector);
  while (it.hasNext()) {
    NodeLabel *currentItem = it.next();
    EventItem *currentEvent = currentItem->getNode();
    if (currentEvent->isVisible()) {
      if (labelsVisible) {
	currentItem->hide();
      } else {
	currentItem->show();
      }
    }
  }
  QVectorIterator<MacroLabel*> it2(macroLabelVector);
  while (it2.hasNext()) {
    MacroLabel *currentItem = it2.next();
    MacroEvent *currentEvent = currentItem->getMacroEvent();
    if (currentEvent->isVisible()) {
      if (labelsVisible) {
	currentItem->hide();
      } else {
	currentItem->show();
      }
    }
  }
  labelsVisible = !(labelsVisible);
}

void EventGraphWidget::processLowerRange(int value) {
  lowerRangeDial->setValue(value);
  lowerRangeSpinBox->setValue(value);
  if (upperRangeDial->value() <= value) {
    upperRangeDial->setValue(value + 1);
    upperRangeSpinBox->setValue(value + 1);
  }
  setVisibility();
}
 
void EventGraphWidget::processUpperRange(int value) {
  upperRangeDial->setValue(value);
  upperRangeSpinBox->setValue(value);
  if (lowerRangeDial->value() >= value) {
    lowerRangeDial->setValue(value - 1);
    lowerRangeSpinBox->setValue(value - 1);
  }
  setVisibility();
} 

void EventGraphWidget::setVisibility() {
  QVectorIterator<EventItem*> it(eventVector);
  int counter = 0;
  while (it.hasNext()) {
    counter++;
    EventItem* currentItem = it.next();
    if (currentItem->getMacroEvent() == NULL) {
      if (counter >= lowerRangeDial->value() && counter <= upperRangeDial->value()) {
	currentItem->show();
      }  else {
	currentItem->hide();
      }
    } else {
      currentItem->hide();
      if (counter >= lowerRangeDial->value() && counter <= upperRangeDial->value()) {
	currentItem->getMacroEvent()->show();
      } else {
	currentItem->getMacroEvent()->hide();
      }
    }
  }
  QVectorIterator<Arrow*> it3(edgeVector);
  while (it3.hasNext()) {
    Arrow *currentEdge = it3.next();
    EventItem *eventTail = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
    EventItem *eventHead = qgraphicsitem_cast<EventItem*>(currentEdge->endItem());
    MacroEvent *macroTail = qgraphicsitem_cast<MacroEvent*>(currentEdge->startItem());
    MacroEvent *macroHead = qgraphicsitem_cast<MacroEvent*>(currentEdge->endItem());
    bool show = true;
    if (eventTail) {
      if(!eventTail->isVisible()) {
	show = false;
      }
    } else if (macroTail) {
      if(!macroTail->isVisible()) {
	show = false;
      }
    }
    if (eventHead) {
      if (!eventHead->isVisible()) {
	show = false;
      }
    } else if (macroHead) {
      if (!macroHead->isVisible()) {
	show = false;
      }
    }
    if (!show) {
      currentEdge->hide();
    } else {
      currentEdge->show();
    }
  }
  QVectorIterator<NodeLabel*> it4(nodeLabelVector);
  while (it4.hasNext()) {
    NodeLabel *currentText = it4.next();
    EventItem *currentParent = currentText->getNode();
    if (!(currentParent->isVisible())) {
      currentText->hide();
    } else {
      currentText->show();
    }
  }
  QVectorIterator<MacroLabel*> it5(macroLabelVector);
  while (it5.hasNext()) {
    MacroLabel *currentText = it5.next();
    MacroEvent *currentParent = currentText->getMacroEvent();
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

void EventGraphWidget::setRangeControls() {
  lowerRangeDial->setEnabled(true);
  upperRangeDial->setEnabled(true);
  lowerRangeSpinBox->setEnabled(true);
  upperRangeSpinBox->setEnabled(true);
  lowerRangeDial->setRange(1, eventVector.size() - 1);
  upperRangeDial->setRange(2, eventVector.size());
  lowerRangeSpinBox->setRange(1, eventVector.size() - 1);
  upperRangeSpinBox->setRange(2, eventVector.size());
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

void EventGraphWidget::processEventItemContextMenu(const QString &action) {
  if (action == COLLIGATEACTION) {
    colligateEvents();
  } else if (action == RECOLOREVENTSACTION) {
    recolorEvents();
  } else if (action == RECOLORLABELSACTION) {
    recolorLabels();
  }
}

void EventGraphWidget::colligateEvents() {
  if (currentData.size() > 1) {
    QPointer<LargeTextDialog> textDialog = new LargeTextDialog(this);
    textDialog->setWindowTitle("Event description");
    textDialog->setLabel("Event description:");
    textDialog->exec();
    if (textDialog->getExitStatus() == 0) {
      QString description = textDialog->getText();
      delete textDialog;
      QVector<EventItem*> tempIncidents;
      QVectorIterator<QGraphicsItem*> it(currentData);
      while (it.hasNext()) {
	EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	if (event) {
	  EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
	  tempIncidents.push_back(currentEvent);
	}
	if (macro) {
	  MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(it.next());
	  QVectorIterator<EventItem*> it2(currentMacro->getIncidents());
	  while (it2.hasNext()) {
	    tempIncidents.push_back(it2.next());
	  }
	}
      }
      qSort(tempIncidents.begin(), tempIncidents.end(), eventLessThan);
      if (checkConstraints(tempIncidents)) {
	qreal lowestX = 0.0;
	qreal highestX = 0.0;
	qreal lowestY = 0.0;
	qreal highestY = 0.0;
	QVectorIterator<EventItem*> it2(tempIncidents);
	while (it2.hasNext()) {
	  EventItem *current = it2.next();
	  if (lowestX == 0.0) {
	    lowestX = current->scenePos().x();
	  }
	  if (highestX == 0.0) {
	    highestX = current->scenePos().x();
	  }
	  if (lowestY == 0.0) {
	    lowestY = current->scenePos().y();
	  }
	  if (highestY == 0.0) {
	    highestY = current->scenePos().y();
	  }
	  if (current->scenePos().x() < lowestX) {
	    lowestX = current->scenePos().x();
	  }
	  if (current->scenePos().x() > highestX) {
	    highestX = current->scenePos().x();
	  }
	  if (current->scenePos().y() < lowestY) {
	    lowestY = current->scenePos().y();
	  }
	  if (current->scenePos().y() > highestY) {
	    highestY = current->scenePos().y();
	  }
	}
	int width = highestX - lowestX + tempIncidents.last()->getWidth();
	qreal xPos = lowestX;
	qreal yPos = lowestY + ((highestY - lowestY) / 2);
	QPointF originalPos = QPointF(xPos, yPos);
   	MacroEvent* current = new MacroEvent(width, description, originalPos, macroVector.size(),
					     tempIncidents);
	qSort(macroVector.begin(), macroVector.end(), eventLessThan);
	current->setPos(originalPos);
	current->setZValue(1);
	int order = 1;
	QVectorIterator<MacroEvent*> mit(macroVector);
	if (macroVector.size() == 0) {
	  current->setOrder(order);
	} else {
	  while (mit.hasNext()) {
	    MacroEvent *temp = mit.next();
	    if (temp->scenePos().x() <= current->scenePos().x()) {
	      order = temp->getOrder() + 1;
	    } else {
	      temp->setOrder(temp->getOrder() + 1);
	      QString label = "E-" + QString::number(temp->getOrder());
	      temp->getLabel()->setPlainText(label);
	      temp->getLabel()->setPlainText(label);
	      temp->getLabel()->setTextWidth(temp->getLabel()->boundingRect().width());

	    }
	  }
	  current->setOrder(order);
	}
	QVectorIterator<EventItem*> it3(tempIncidents);
	while (it3.hasNext()) {
	  EventItem* item = it3.next();
	  if (item->getMacroEvent() != NULL) {
	    item->getMacroEvent()->hide();
	  }
	  item->setMacroEvent(current);
	  item->hide();
	}
	macroVector.push_back(current);
	scene->addItem(current);
	MacroLabel *macroLabel = new MacroLabel(current);
	current->setLabel(macroLabel);
	QString label = "E-" + QString::number(order);
	macroLabel->setPlainText(label);
	macroLabel->setTextWidth(macroLabel->boundingRect().width());
	qreal xOffset = (current->getWidth() / 2) - 20;
	macroLabel->setOffset(QPointF(xOffset,0));
	macroLabel->setNewPos(current->scenePos());
	macroLabel->setZValue(2);
	macroLabel->setDefaultTextColor(Qt::black);
	macroLabelVector.push_back(macroLabel);
	scene->addItem(macroLabel);
	rewireLinkages(current, tempIncidents);
	setVisibility();
      }
    } else {
      return;
    }
  } else {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<b>At least two incidents required</b>");
    warningBox->setInformativeText("You should select at least two incidents to "
				   "be able to perform this action.");
    warningBox->exec();
    delete warningBox;
  }
}

bool EventGraphWidget::checkConstraints(QVector<EventItem*> incidents) {
  /* 
     Check whether colligating these events breaks constrains set for colligation
     First we check internal consistency.
  */
  QVector<int> incidentId;
  QVectorIterator<EventItem*> it(incidents);
  while (it.hasNext()) {
    EventItem *event = qgraphicsitem_cast<EventItem*>(it.next());
    if (event) {
      incidentId.push_back(event->getId());
    }
  }

  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  QVectorIterator<EventItem*> dit(incidents);

  QVector<int> markOne;
  while (dit.hasNext()) {
    EventItem *departure = dit.next();
    // First we check the internal consistency;
    findPastPaths(&markOne, departure->getId());
    QVectorIterator<EventItem*> cit(incidents);
    while (cit.hasNext()) {
      EventItem *current = cit.next();
      bool found = false;
      if (current != departure) {
	if (markOne.contains(current->getId())) {
	  found = true;
	}
	if (direction == PAST) {
	  if (!found && current->getOriginalPos().x() < departure->getOriginalPos().x()) {
	    QPointer <QMessageBox> warningBox = new QMessageBox(this);
	    warningBox->addButton(QMessageBox::Ok);
	    warningBox->setIcon(QMessageBox::Warning);
	    warningBox->setText("<b>Constraints not met.</b>");
	    warningBox->setInformativeText("Colligating these incidents breaks the constraints "
					   "that were set for colligation.");
	    warningBox->exec();
	    delete warningBox;
	    return false;
	  }
	} else if (direction == FUTURE) {
	  if (!found && current->getOriginalPos().x() > departure->getOriginalPos().x()) {
	    QPointer <QMessageBox> warningBox = new QMessageBox(this);
	    warningBox->addButton(QMessageBox::Ok);
	    warningBox->setIcon(QMessageBox::Warning);
	    warningBox->setText("<b>Constraints not met.</b>");
	    warningBox->setInformativeText("Colligating these incidents breaks the constraints "
					   "that were set for colligation.");
	    warningBox->exec();
	    delete warningBox;
	    return false;
	  }
	}
      }
    }
    // Then we check the external consistency.
    query->prepare("SELECT tail FROM linkages "
		   "WHERE head = :head AND type = :type AND coder = :coder");
    query->bindValue(":head", departure->getId());
    query->bindValue(":type", selectedType);
    query->bindValue(":coder", selectedCoder);
    query->exec();
    while (query->next()) {
      QVector<int> markTwo;
      int currentTail = query->value(0).toInt();
      if (!(incidentId.contains(currentTail))) {
	findPastPaths(&markTwo, currentTail);
	QVectorIterator<EventItem*> kit(incidents);
	while (kit.hasNext()) {
	  EventItem *current = kit.next();
	  bool found = false;
	  if (markTwo.contains(current->getId())) {
	    found = true;
	  }
	  if (!found) {
	    QPointer <QMessageBox> warningBox = new QMessageBox(this);
	    warningBox->addButton(QMessageBox::Ok);
	    warningBox->setIcon(QMessageBox::Warning);
	    warningBox->setText("<b>Constraints not met.</b>");
	    warningBox->setInformativeText("Colligating these incidents breaks the constraints "
					   "that were set for colligation.");
	    warningBox->exec();
	    delete warningBox;
	    return false;
	  }
	}
      }
    }
    query->prepare("SELECT head FROM linkages "
		   "WHERE tail = :tail AND type = :type AND coder = :coder");
    query->bindValue(":tail", departure->getId());
    query->bindValue(":type", selectedType);
    query->bindValue(":coder", selectedCoder);
    query->exec();
    while (query->next()) {
      int currentHead = query->value(0).toInt();
      if (!(incidentId.contains(currentHead))) {
	QVectorIterator<EventItem*> lit(incidents);
	while (lit.hasNext()) {
	  EventItem *current = lit.next();
	  if (current != departure) {
	    QVector<int> markTwo;
	    findPastPaths(&markTwo, current->getId());
	    bool found = false;
	    if (markTwo.contains(currentHead)) {
	      found = true;
	    }
	    if (!found) {
	      QPointer <QMessageBox> warningBox = new QMessageBox(this);
	      warningBox->addButton(QMessageBox::Ok);
	      warningBox->setIcon(QMessageBox::Warning);
	      warningBox->setText("<b>Constraints not met.</b>");
	      warningBox->setInformativeText("Colligating these incidents breaks the constraints "
					     "that were set for colligation.");
	      warningBox->exec();
	      delete warningBox;
	      return false;
	    }
	  }
	}
      }
    }
  }
  return true;
}

void EventGraphWidget::rewireLinkages(MacroEvent *macro, QVector<EventItem*> incidents) {
  QVector<int> incidentId;
  QVectorIterator<EventItem*> tit(incidents);
  while (tit.hasNext()) {
    EventItem *event = qgraphicsitem_cast<EventItem*>(tit.next());
    if (event) {
      incidentId.push_back(event->getId());
    }
  }

  QVectorIterator<EventItem*> it(incidents);
  while (it.hasNext()) {
    EventItem *current = it.next();
    QSqlQuery *query = new QSqlQuery;
    int incident = current->getId();
    query->prepare("SELECT tail FROM linkages "
		   "WHERE head = :head AND coder = :coder AND type = :type");
    query->bindValue(":head", incident);
    query->bindValue(":type", selectedType);
    query->bindValue(":coder", selectedCoder);
    query->exec();
    QSet<int> tailSet;
    while (query->next()) {
      int tail = query->value(0).toInt();
      if (!(incidentId.contains(tail))) {
	tailSet.insert(tail);
      }
    }
    QList<int> tailList = tailSet.toList();
    QListIterator<int> it2(tailList);
    while (it2.hasNext()) {
      QGraphicsItem *tempTarget = macro;
      QGraphicsItem *tempSource = NULL;
      int currentTail = it2.next();
      QVectorIterator<EventItem*> et(eventVector);
      while (et.hasNext()) {
	EventItem *currentEvent = et.next();
	if (currentEvent->getId() == currentTail) {
	  if (currentEvent->getMacroEvent() == NULL) {
	    tempSource = currentEvent;
	  } else {
	    tempSource = currentEvent->getMacroEvent();
	  }
	  Arrow *newEdge = new Arrow(tempSource, tempTarget, selectedType, selectedCoder);
	  scene->addItem(newEdge);
	  edgeVector.push_back(newEdge);

	}
      }
    }
    query->prepare("SELECT head FROM linkages "
		   "WHERE tail = :tail AND coder = :coder AND type = :type");
    query->bindValue(":tail", incident);
    query->bindValue(":type", selectedType);
    query->bindValue(":coder", selectedCoder);
    query->exec();
    QSet<int> headSet;
    while (query->next()) {
      int head = query->value(0).toInt();
      if (!(incidentId.contains(head))) {
	headSet.insert(head);
      }
    }
    QList<int> headList = headSet.toList();
    QListIterator<int> it3(headList);
    while (it3.hasNext()) {
      QGraphicsItem *tempTarget = NULL;
      QGraphicsItem *tempSource = macro;
      int currentHead = it3.next();
      QVectorIterator<EventItem*> et(eventVector);
      while (et.hasNext()) {
	EventItem *currentEvent = et.next();
	if (currentEvent->getId() == currentHead) {
	  if (currentEvent->getMacroEvent() == NULL) {
	    tempTarget = currentEvent;
	  } else {
	    tempTarget = currentEvent->getMacroEvent();
	  }
	  Arrow *newEdge = new Arrow(tempSource, tempTarget, selectedType, selectedCoder);
	  scene->addItem(newEdge);
	  edgeVector.push_back(newEdge);
	}
      }
    }
    delete query; 
  }
}

void EventGraphWidget::recolorEvents() {
  if (scene->selectedItems().size() > 0) {
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    if (colorDialog->exec()) {
      QColor color = colorDialog->selectedColor();
      delete colorDialog;
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) {
	EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	MacroEvent *macro =  qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	if (event) {
	  EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
	  currentEvent->setColor(color);
	} else if (macro) {
	  MacroEvent *currentEvent = qgraphicsitem_cast<MacroEvent*>(it.next());
	  currentEvent->setColor(color);
	} else {
	  it.next();
	}
      }
    }
  }
}

void EventGraphWidget::recolorLabels() {
  if (scene->selectedItems().size() > 0) {
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    if (colorDialog->exec()) {
      QColor color = colorDialog->selectedColor();
      delete colorDialog;
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) {
	EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	if (event) {
	  EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
	  NodeLabel *currentLabel = currentEvent->getLabel();
	  currentLabel->setDefaultTextColor(color);
	} else if (macro) {
	  MacroEvent *currentEvent = qgraphicsitem_cast<MacroEvent*>(it.next());
	  MacroLabel *currentLabel = currentEvent->getLabel();
	  currentLabel->setDefaultTextColor(color);

	} else {
	  it.next();
	}
      }
    }
  }
}

void EventGraphWidget::processArrowContextMenu(const QString &action) {
  if (action == REMOVELINKAGEACTION) {
    removeLinkage();
  } else if (action == KEEPLINKAGEACTION) {
    keepLinkage();
  } else if (action == ACCEPTLINKAGEACTION) {
    acceptLinkage();
  } else if (action == REJECTLINKAGEACTION) {
    rejectLinkage();
  } else if (action == IGNORE) {
    ignoreLinkage();
  } else if (action == REMOVENORMALLINKAGE) {
    removeNormalLinkage();
  }
}

void EventGraphWidget::removeLinkage() {
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
      if (arrow) {
	Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	EventItem *startEvent = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	EventItem *endEvent = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	int tail = startEvent->getId();
	int head = endEvent->getId();
	QSqlQuery *query =  new QSqlQuery;
	query->prepare("DELETE FROM linkages "
		       "WHERE tail = :tail AND head = :head AND coder = :coder AND type = :type");
	query->bindValue(":tail", tail);
	query->bindValue(":head", head);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	delete query;
	// If the edge is not between eventItems, then it does not exist in the linkages table.
	scene->removeItem(currentEdge);
	edgeVector.removeOne(currentEdge);
      } else {
	it.next();
      }
    }
  }
}

void EventGraphWidget::removeNormalLinkage() {
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
      if (arrow) {
	Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	QPointer<QMessageBox> warningBox = new QMessageBox(this);
	warningBox->addButton(QMessageBox::Yes);
	warningBox->addButton(QMessageBox::No);
	warningBox->setIcon(QMessageBox::Warning);
	warningBox->setText("<h2>Are you sure?</h2>");
	warningBox->setInformativeText("Are you sure you want to remove this linkage?");
	if (warningBox->exec() == QMessageBox::Yes) {
	  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	  if (startEvent && endEvent) {
	    int tail = startEvent->getId();
	    int head = endEvent->getId();
	  
	    QSqlQuery *query =  new QSqlQuery;
	    query->prepare("DELETE FROM linkages "
			   "WHERE tail = :tail AND head = :head AND coder = :coder AND type = :type");
	    query->bindValue(":tail", tail);
	    query->bindValue(":head", head);
	    query->bindValue(":coder", selectedCoder);
	    query->bindValue(":type", selectedType);
	    query->exec();
	    delete query;
	  }
	  scene->removeItem(currentEdge);
	  edgeVector.removeOne(currentEdge);
	  delete warningBox;
	} else {
	  delete warningBox;
	  return;
	}
      } else {
	it.next();
      }
    }
  }
}

void EventGraphWidget::ignoreLinkage() {
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
      NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(it.peekNext());
      if (arrow && !(event) && !(text)) {
	Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	currentEdge->setColor(Qt::black);
      } else {
	it.next();
      }
    }
  }
}

void EventGraphWidget::keepLinkage() {
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
      if (arrow) {
	Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());
	EventItem *startEvent = qgraphicsitem_cast<EventItem*>(arrow->startItem());
	int tail = startEvent->getId();
	currentEdge->setColor(Qt::black);
	QSqlQuery *query = new QSqlQuery;
	query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	query->bindValue(":type", selectedType);
	query->exec();
	query->first();
	QString direction = query->value(0).toString();
	QVector<int> markOne;
	QVector<int> markTwo;
	if (direction == PAST) {
	  findPastPaths(&markOne, tail);
	  findFuturePaths(&markTwo, tail);
	} else if (direction == FUTURE) {
	  findFuturePaths(&markOne, tail);
	  findPastPaths(&markTwo, tail);
	}
	QVectorIterator<int> it2(markOne);
	bool found = false;
	while (it2.hasNext()) {
	  int currentPathMate = it2.next();
	  query->prepare("SELECT head FROM linkages "
			 "WHERE coder = :coder AND type = :type "
			 "AND tail = :tail");
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->bindValue(":tail", currentPathMate);
	  query->exec();
	  while (query->next()) {
	    int mateHead = query->value(0).toInt();
	    QSqlQuery *query2 = new QSqlQuery;
	    query2->prepare("SELECT tail, head FROM linkages "
			    "WHERE coder = :coder AND type = :type "
			    "AND tail = :tail AND head = :head");
	    query2->bindValue(":coder", selectedCoder);
	    query2->bindValue(":type", selectedType);
	    query2->bindValue(":tail", tail);
	    query2->bindValue(":head", mateHead);
	    query2->exec();
	    query2->first();
	    if (!(query2->isNull(0))) {
	      found = true;
	    }
	  }
	  if (found) {
	    currentEdge->setColor(Qt::darkGreen);
	  }
	}
	QVectorIterator<int> it3(markTwo);
        found = false;
	while (it3.hasNext()) {
	  int currentPathMate = it3.next();
	  query->prepare("SELECT head FROM linkages "
			 "WHERE coder = :coder AND type = :type "
			 "AND tail = :tail");
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->bindValue(":tail", currentPathMate);
	  query->exec();
	  while (query->next()) {
	    int mateHead = query->value(0).toInt();
	    QSqlQuery *query2 = new QSqlQuery;
	    query2->prepare("SELECT tail, head FROM linkages "
			    "WHERE coder = :coder AND type = :type "
			    "AND tail = :tail AND head = :head");
	    query2->bindValue(":coder", selectedCoder);
	    query2->bindValue(":type", selectedType);
	    query2->bindValue(":tail", tail);
	    query2->bindValue(":head", mateHead);
	    query2->exec();
	    query2->first();
	    if (!(query2->isNull(0))) {
	      found = true;
	    }
	  }
	  if (found) {
	    currentEdge->setColor(Qt::darkGreen);
	  }
	}
	delete query;
      } else {
	it.next();
      }
    }
  }
}

void EventGraphWidget::acceptLinkage() {
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
      NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(it.peekNext());
      if (arrow && !(event) && !(text)) {
	Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	EventItem *startEvent = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	EventItem *endEvent = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	int tail = startEvent->getId();
	int head = endEvent->getId();
	currentEdge->setColor(Qt::black);
	edgeVector.push_back(currentEdge);
	compareVector.removeOne(currentEdge);
	QSqlQuery *query =  new QSqlQuery;
	query->prepare("INSERT INTO linkages (tail, head, coder, type) "
		       "VALUES (:tail, :head, :coder, :type)");
	query->bindValue(":tail", tail);
	query->bindValue(":head", head);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
       	query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	query->bindValue(":type", selectedType);
	query->exec();
	query->first();
	QString direction = query->value(0).toString();
	QVector<int> markOne;
	QVector<int> markTwo;
	if (direction == PAST) {
	  findPastPaths(&markOne, tail);
	  findFuturePaths(&markTwo, tail);
	} else if (direction == FUTURE) {
	  findFuturePaths(&markOne, tail);
	  findPastPaths(&markTwo, tail);
	}
	QVectorIterator<int> it2(markOne);
	bool found = false;
	while (it2.hasNext()) {
	  int currentPathMate = it2.next();
	  query->prepare("SELECT head FROM linkages "
			 "WHERE coder = :coder AND type = :type "
			 "AND tail = :tail");
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->bindValue(":tail", currentPathMate);
	  query->exec();
	  while (query->next()) {
	    int mateHead = query->value(0).toInt();
	    QSqlQuery *query2 = new QSqlQuery;
	    query2->prepare("SELECT tail, head FROM linkages "
			    "WHERE coder = :coder AND type = :type "
			    "AND tail = :tail AND head = :head");
	    query2->bindValue(":coder", selectedCoder);
	    query2->bindValue(":type", selectedType);
	    query2->bindValue(":tail", tail);
	    query2->bindValue(":head", mateHead);
	    query2->exec();
	    query2->first();
	    if (!(query2->isNull(0))) {
	      found = true;
	    }
	  }
	  if (found) {
	    currentEdge->setColor(Qt::darkGreen);
	  }
	}
	QVectorIterator<int> it3(markTwo);
        found = false;
	while (it3.hasNext()) {
	  int currentPathMate = it3.next();
	  query->prepare("SELECT head FROM linkages "
			 "WHERE coder = :coder AND type = :type "
			 "AND tail = :tail");
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->bindValue(":tail", currentPathMate);
	  query->exec();
	  while (query->next()) {
	    int mateHead = query->value(0).toInt();
	    QSqlQuery *query2 = new QSqlQuery;
	    query2->prepare("SELECT tail, head FROM linkages "
			    "WHERE coder = :coder AND type = :type "
			    "AND tail = :tail AND head = :head");
	    query2->bindValue(":coder", selectedCoder);
	    query2->bindValue(":type", selectedType);
	    query2->bindValue(":tail", tail);
	    query2->bindValue(":head", mateHead);
	    query2->exec();
	    query2->first();
	    if (!(query2->isNull(0))) {
	      found = true;
	    }
	  }
	  if (found) {
	    currentEdge->setColor(Qt::darkGreen);
	  }
	}
	delete query;
      } else {
	it.next();
      }
    }
  }
}

void EventGraphWidget::rejectLinkage() {
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
      NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(it.peekNext());
      if (arrow && !(event) && !(text)) {
	Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	scene->removeItem(currentEdge);
	compareVector.removeOne(currentEdge);
      } else {
	it.next();
      }
    }
  }
}

void EventGraphWidget::findPastPaths(QVector<int> *pMark, int currentIncident) {
  int currentTail = currentIncident;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM linkages "
		 "WHERE tail = :tail AND type = :type AND coder = :coder");
  query->bindValue(":tail", currentTail);
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) {
    int currentHead = 0;
    currentHead = query->value(0).toInt();
    results.push_back(currentHead);
  }
  delete query;
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) {
    pMark->push_back(*it);
    findPastPaths(pMark, *it);
  }
}

void EventGraphWidget::findFuturePaths(QVector<int> *pMark, int currentIncident) {
  int currentHead = currentIncident;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM linkages "
		 "WHERE head = :head AND type = :type AND coder = :coder");
  query->bindValue(":head", currentHead);
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) {
    int currentTail = 0;
    currentTail = query->value(0).toInt();
    results.push_back(currentTail);
  }
  delete query;
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) {
    pMark->push_back(*it);
    findFuturePaths(pMark, *it);
  }
}

bool EventGraphWidget::eventFilter(QObject *object, QEvent *event) {
  if (object == view->viewport() && event->type() == QEvent::MouseButtonRelease) {
    retrieveData();
  }
  return false;
}

QVector<MacroEvent*> EventGraphWidget::getMacros() {
  return macroVector;
}

void EventGraphWidget::setAttributesWidget(AttributesWidget *aw) {
  attributesWidget = aw;
}

void EventGraphWidget::resetTree() {
  scene->clearSelection();
  retrieveData();
  delete attributesTree;
  setTree();
}
