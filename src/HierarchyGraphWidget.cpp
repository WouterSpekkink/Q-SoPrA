#include "../include/HierarchyGraphWidget.h"
                      
HierarchyGraphWidget::HierarchyGraphWidget(QWidget *parent) : QDialog(parent) {  
  selectedMacro = NULL;
  selectedIncident = 0;
  
  scene = new Scene(this);
  view = new BandlessGraphicsView(scene);
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
  
  infoWidget = new QWidget(this);
  attWidget = new QWidget(this);
  commentWidget = new QWidget(this);
  legendWidget = new QWidget(this); 
  
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

  timeStampLabel = new QLabel("<b>Timing:</b>", infoWidget);
  sourceLabel = new QLabel("<b>Source:</b>", infoWidget);
  descriptionLabel = new QLabel("<b>Description:</b>", infoWidget);
  rawLabel = new QLabel("<b>Raw:</b>", infoWidget);
  commentLabel = new QLabel("<b>Comments:</b>", commentWidget);
  attributesLabel = new QLabel(tr("<b>Attributes:</b>"), attWidget);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), attWidget);
  valueLabel = new QLabel(tr("<b>Value:</b>"), attWidget);
  legendLabel = new QLabel(tr("<b>Modes:</b>"), legendWidget);

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

  eventListWidget = new DeselectableListWidget(legendWidget);
  eventListWidget->setColumnCount(2);
  eventListWidget->horizontalHeader()->hide();
  eventListWidget->verticalHeader()->hide();
  eventListWidget->setColumnWidth(1, 20);
  eventListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  eventListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  eventListWidget->setStyleSheet("QTableView {gridline-color: black}");
  eventListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

  exportSvgButton = new QPushButton(tr("Export svg"), this);
  toggleDetailsButton = new QPushButton(tr("Toggle details"), this);
  toggleDetailsButton->setCheckable(true);
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  assignAttributeButton = new QPushButton(tr("Assign attribute"), attWidget);
  unassignAttributeButton = new QPushButton(tr("Unassign attribute"), attWidget);
  addAttributeButton = new QPushButton(tr("New attribute"), attWidget);
  editAttributeButton = new QPushButton(tr("Edit attribute"), attWidget);
  removeUnusedAttributesButton = new QPushButton(tr("Remove unused"), attWidget);
  seeAttributesButton = new QPushButton(tr("Attributes"), commentWidget);
  seeCommentsButton = new QPushButton(tr("Comments"), attWidget);
  removeTextButton = new QPushButton("Remove text", attWidget);
  removeTextButton->setEnabled(false);
  resetTextsButton = new QPushButton("Reset texts", attWidget);
  resetTextsButton->setEnabled(false);
  exitButton = new QPushButton(tr("Return to event graph"), this);
  exitButton->setStyleSheet("QPushButton {color: blue; font-weight: bold}");
  valueButton = new QPushButton(tr("Store value"), attWidget);
  valueButton->setEnabled(false);
  
  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(seeAttributesButton, SIGNAL(clicked()), this, SLOT(showAttributes()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(seeCommentsButton, SIGNAL(clicked()), this, SLOT(showComments()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(highlightText()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
  	  this, SLOT(changeFilter(const QString &)));
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(eventListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeModeColor(QTableWidgetItem *)));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
  connect(exitButton, SIGNAL(clicked()), this, SLOT(switchBack()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> screenLayout = new QHBoxLayout;

  QPointer<QVBoxLayout> detailsLayout = new QVBoxLayout;
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
  QPointer<QHBoxLayout> textButtonsLayout = new QHBoxLayout;
  textButtonsLayout->addWidget(removeTextButton);
  textButtonsLayout->addWidget(resetTextsButton);
  attributesLayout->addLayout(textButtonsLayout);
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

  QPointer<QVBoxLayout> legendLayout = new QVBoxLayout;
  legendLayout->addWidget(legendLabel);
  legendLayout->addWidget(eventListWidget);
  legendWidget->setMinimumWidth(175);
  legendWidget->setMaximumWidth(175);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				   
  
  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  drawOptionsLeftLayout->addWidget(toggleDetailsButton);
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(exportSvgButton);
  drawOptionsRightLayout->addWidget(exitButton);
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);
    
  setLayout(mainLayout);
  attWidget->hide();
  legendWidget->hide();
}

void HierarchyGraphWidget::toggleDetails() {
  setComment();
  if (infoWidget->isHidden()) {
    infoWidget->show();
  } else {
    infoWidget->hide();
  }
}

void HierarchyGraphWidget::toggleLegend() {
  if (legendWidget->isHidden()) {
    legendWidget->show();
  } else {
    legendWidget->hide();
  }
}

void HierarchyGraphWidget::showAttributes() {
  commentWidget->hide();
  attWidget->show();
}

void HierarchyGraphWidget::showComments() {
  attWidget->hide();
  commentWidget->show();
}

void HierarchyGraphWidget::setCommentBool() {
  commentBool = true;
}

void HierarchyGraphWidget::setComment() {
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
    QVectorIterator<MacroEvent*> it(macroVector);
    while (it.hasNext()) {
      MacroEvent *macro = it.next();
      if (macro->getId() == selectedMacro->getId()) {
	macro->setComment(comment);
      }
    }
  }
}

void HierarchyGraphWidget::retrieveData() {
  setComment();
  if (currentData.size() > 0) {
    currentData.clear();
  }
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
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
      } else {
	it.next();
      }
    }
    if (currentData.size() > 0) {
      qSort(currentData.begin(), currentData.end(), eventLessThan);
      QGraphicsItem *temp = currentData[0];
      scene->clearSelection();
      temp->setSelected(true);
      currentData.clear();
      currentData.push_back(temp);
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData[0]);
      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData[0]);
      if (currentEvent) {
	selectedMacro = NULL;
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
	descriptionField->setText(currentMacro->getDescription());
	timeStampLabel->setText("<b>Duration:</b>");
	sourceLabel->setText("<b>Number of components:</b>");
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
	int count = currentMacro->getIncidents().size();
	QVectorIterator<MacroEvent*> it(macroVector);
	while (it.hasNext()) {
	  MacroEvent *temp = it.next();
	  if (temp->getMacroEvent() == currentMacro) {
	    count++;
	  }
	}
	QString countText = QString::number(count);
	timeStampField->setText(duration);
	sourceField->setText(countText);
	commentField->setText(currentMacro->getComment());
	delete query;
	resetFont(attributesTree);
	QSet<QString> attributes = currentMacro->getAttributes();
	QSet<QString>::iterator it2;
	id = currentMacro->getId();
	for (it2 = attributes.begin(); it2 != attributes.end(); it2++) {
	  QString attribute  = *it2;
	  boldSelected(attributesTree, attribute, id, MACRO);
	}
      }
    }
  } else {
    timeStampField->clear();
    descriptionField->clear();
    rawField->clear();
    commentField->blockSignals(true);
    commentField->clear();
    commentField->blockSignals(false);
    sourceField->clear();
    selectedIncident = 0;
    selectedMacro = NULL;
    resetFont(attributesTree);
  }
}

void HierarchyGraphWidget::buildComponents(MacroEvent *submittedOrigin, int layer) {
  MacroEvent *newOrigin = new MacroEvent(40, submittedOrigin->getDescription(), QPointF(0,0),
					 submittedOrigin->getId(),
					 submittedOrigin->getConstraint(),
					 submittedOrigin->getIncidents());
  newOrigin->setCopy(true);
  newOrigin->setMode(submittedOrigin->getMode());
  newOrigin->setColor(submittedOrigin->getColor());
  newOrigin->setAttributes(submittedOrigin->getAttributes());
  newOrigin->setZValue(1);
  MacroLabel *macroLabel = new MacroLabel(newOrigin);
  newOrigin->setLabel(macroLabel);
  QTableWidgetItem *item = new QTableWidgetItem(newOrigin->getMode());
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  if (newOrigin->getMode() != "") {
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM incident_attributes "
		   "WHERE name = :name");
    query->bindValue(":name", newOrigin->getMode());
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    delete query;
    QString toolTip = "<FONT SIZE = 3>" + newOrigin->getMode()  + " - " +
      description + "</FONT SIZE>";
    item->setToolTip(toolTip);
    item->setData(Qt::DisplayRole, newOrigin->getMode());
    eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
    eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
    eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
    eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setBackground(newOrigin->getColor());
    eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
      setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
	       Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
  }
  if (newOrigin->getConstraint() == PATHS) {
    QString label = "P-" + QString::number(newOrigin->getId());
    macroLabel->setPlainText(label);
    macroLabel->setTextWidth(macroLabel->boundingRect().width());
  } else if (newOrigin->getConstraint() == SEMIPATHS) {
    QString label = "S-" + QString::number(newOrigin->getId());
    macroLabel->setPlainText(label);
    macroLabel->setTextWidth(macroLabel->boundingRect().width());
  }
  newOrigin->setPos(QPointF(newOrigin->scenePos().x(),
			    newOrigin->scenePos().y() - layer * 80));
  newOrigin->setOriginalPos(newOrigin->scenePos());
  qreal xOffset = (newOrigin->getWidth() / 2) - 20;
  macroLabel->setOffset(QPointF(xOffset,0));
  macroLabel->setNewPos(newOrigin->scenePos());
  macroLabel->setZValue(2);
  macroLabel->setDefaultTextColor(Qt::black);
  scene->addItem(newOrigin);
  scene->addItem(macroLabel);
  qreal yPos = 0 + (layer * 80);
  QVector<QGraphicsItem*> currentLayer;
  QVector<QGraphicsItem*> partners;
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) {
    MacroEvent *macro = it.next();
    if (macro->getMacroEvent() == submittedOrigin) { 
      MacroEvent *newMacro = new MacroEvent(40, macro->getDescription(), QPointF(0, yPos),
					    macro->getId(), macro->getConstraint(),
					    macro->getIncidents());
      newMacro->setCopy(true);
      newMacro->setMode(macro->getMode());
      newMacro->setColor(macro->getColor());
      newMacro->setAttributes(macro->getAttributes());
      newMacro->setPos(newMacro->getOriginalPos());
      newMacro->setZValue(1);
      bool found = false;
      if (newMacro->getMode() != "") {
	for (int i = 0; i < eventListWidget->rowCount(); i++) {
	  if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newMacro->getMode()) {
	    found = true;
	  }
	}
	if (!found) {
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT description FROM incident_attributes "
			 "WHERE name = :name");
	  query->bindValue(":name", newMacro->getMode());
	  query->exec();
	  query->first();
	  QString description = query->value(0).toString();
	  delete query;
	  QTableWidgetItem *item = new QTableWidgetItem(newMacro->getMode());
	  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	  QString toolTip = "<FONT SIZE = 3>" + newMacro->getMode() + " - " +
	    description + "</FONT SIZE>";
	  item->setToolTip(toolTip);
	  item->setData(Qt::DisplayRole, newMacro->getMode());
	  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setBackground(newMacro->getColor());
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	}
      }
      MacroLabel *newMacroLabel = new MacroLabel(newMacro);
      newMacro->setLabel(newMacroLabel);
      if (newMacro->getConstraint() == PATHS) {
	QString label = "P-" + QString::number(newMacro->getId());
	newMacroLabel->setPlainText(label);
	newMacroLabel->setTextWidth(newMacroLabel->boundingRect().width());
      } else if (newMacro->getConstraint() == SEMIPATHS) {
	QString label = "S-" + QString::number(newMacro->getId());
	newMacroLabel->setPlainText(label);
	newMacroLabel->setTextWidth(newMacroLabel->boundingRect().width());
      }
      qreal xOffset = (newMacro->getWidth() / 2) - 20;
      newMacroLabel->setOffset(QPointF(xOffset,0));
      newMacroLabel->setNewPos(newMacro->scenePos());
      newMacroLabel->setZValue(2);
      newMacroLabel->setDefaultTextColor(Qt::black);
      currentLayer.push_back(newMacro);
      Arrow *newArrow = new Arrow(newMacro, newOrigin, "Hierarchy", "", 0);
      newArrow->setCopy(true);
      scene->addItem(newArrow);
      partners.push_back(macro);
    }
  }
  QVectorIterator<EventItem*> it2(eventVector);
  while (it2.hasNext()) {
    EventItem *event = it2.next();
    if (event->getMacroEvent() == submittedOrigin) {
      EventItem *newEvent = new EventItem(40, event->data(Qt::ToolTipRole).toString(),
					  QPointF(0, yPos), event->getId(), event->getOrder());
      newEvent->setCopy(true);
      newEvent->setMode(event->getMode());
      newEvent->setColor(event->getColor());
      newEvent->setPos(newEvent->getOriginalPos());
      bool found = false;
      if (newEvent->getMode() != "") {
	for (int i = 0; i < eventListWidget->rowCount(); i++) {
	  if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newEvent->getMode()) {
	    found = true;
	  }
	}
	if (!found) {
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT description FROM incident_attributes "
			 "WHERE name = :name");
	  query->bindValue(":name", newEvent->getMode());
	  query->exec();
	  query->first();
	  QString description = query->value(0).toString();
	  delete query;
	  QTableWidgetItem *item = new QTableWidgetItem(newEvent->getMode());
	  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	  QString toolTip = "<FONT SIZE = 3>" + newEvent->getMode() + " - " +
	    description + "</FONT SIZE>";
	  item->setToolTip(toolTip);
	  item->setData(Qt::DisplayRole, newEvent->getMode());
	  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setBackground(newEvent->getColor());
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	}
      }
      QPointer<NodeLabel> text = new NodeLabel(newEvent);
      newEvent->setLabel(text);
      text->setPlainText(QString::number(newEvent->getOrder()));
      text->setTextWidth(text->boundingRect().width());
      QPointF currentPos = newEvent->scenePos();
      currentPos.setX(currentPos.x() - (text->textWidth() / 2));
      currentPos.setY(currentPos.y() -12);
      text->setPos(currentPos);    
      text->setZValue(2);
      text->setDefaultTextColor(Qt::black);
      currentLayer.push_back(newEvent);
      Arrow *newArrow = new Arrow(newEvent, newOrigin, "Hierarchy", "", 0);
      newArrow->setCopy(true);
      scene->addItem(newArrow);
      partners.push_back(event);
    }
  }
  qSort(currentLayer.begin(), currentLayer.end(), componentsSort);
  qSort(partners.begin(), partners.end(), componentsSort);
  int layerSize = currentLayer.size();
  qreal width = 120 * layerSize;
  qreal startX = 0 - width / 2 - 60;
  int count = 1;
  QVector<MacroEvent*> newLayer;
  QVector<MacroEvent*> partnerLayer;
  QVector<QGraphicsItem*>::iterator it3;
  for (QVector<QGraphicsItem*>::size_type it3 = 0; it3 != currentLayer.size(); it3++) {
    QGraphicsItem *current = currentLayer[it3];
    qreal xPos = startX + count * 120;
    current->setPos(xPos, current->scenePos().y());
    count++;
    EventItem *event = qgraphicsitem_cast<EventItem*>(current);
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
    if (event) {
      event->setOriginalPos(event->scenePos());
      event->getLabel()->setNewPos(event->scenePos());
      scene->addItem(event);
      scene->addItem(event->getLabel());
    } else if (macro) {
      macro->setOriginalPos(macro->scenePos());
      macro->getLabel()->setNewPos(macro->scenePos());
      scene->addItem(macro);
      scene->addItem(macro->getLabel());
      MacroEvent *tempMacro = qgraphicsitem_cast<MacroEvent*>(partners[it3]);
      newLayer.push_back(tempMacro);
      partnerLayer.push_back(qgraphicsitem_cast<MacroEvent*>(current));
    }
  }
  if (newLayer.size() > 0) {
    addLayer(newLayer, partnerLayer, layer + 1);
  }
}

void HierarchyGraphWidget::addLayer(QVector<MacroEvent*> presentLayer,
				    QVector<MacroEvent*> partLayer,
				    int layer) {
  qreal yPos = 0 + (layer * 80);
  QVector<QGraphicsItem*> currentLayer;
  QVector<QGraphicsItem*> partners;
  for (QVector<MacroEvent*>::size_type it = 0; it != presentLayer.size(); it++) {
    MacroEvent *currentFather = presentLayer[it];
    qDebug() << currentFather->getDescription();
    QVectorIterator<MacroEvent*> it2(macroVector);
    while (it2.hasNext()) {
      MacroEvent *macro = it2.next();
      if (macro->getMacroEvent() == currentFather) { 
	MacroEvent *newMacro = new MacroEvent(40, macro->getDescription(), QPointF(0,yPos),
					      macro->getId(), macro->getConstraint(),
					      macro->getIncidents());
	newMacro->setCopy(true);
	newMacro->setMode(macro->getMode());
	newMacro->setColor(macro->getColor());
	newMacro->setAttributes(macro->getAttributes());
	newMacro->setPos(newMacro->getOriginalPos());
	newMacro->setZValue(1);
	bool found = false;
	if (newMacro->getMode() != "") {
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT description FROM incident_attributes "
			 "WHERE name = :name");
	  query->bindValue(":name", newMacro->getMode());
	  query->exec();
	  query->first();
	  QString description = query->value(0).toString();
	  delete query;
	  for (int i = 0; i < eventListWidget->rowCount(); i++) {
	    if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newMacro->getMode()) {
	      found = true;
	    }
	  }
	  if (!found) {
	    QTableWidgetItem *item = new QTableWidgetItem(newMacro->getMode());
	    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	    QString toolTip = "<FONT SIZE = 3>" + newMacro->getMode() + " - " +
	      description + "</FONT SIZE>";
	    item->setToolTip(toolTip);
	    item->setData(Qt::DisplayRole, newMacro->getMode());
	    eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	    eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	    eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	    eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	      setBackground(newMacro->getColor());
	    eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	      setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		       Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	  }
	}
	MacroLabel *newMacroLabel = new MacroLabel(newMacro);
	newMacro->setLabel(newMacroLabel);
	if (newMacro->getConstraint() == PATHS) {
	  QString label = "P-" + QString::number(newMacro->getId());
	  newMacroLabel->setPlainText(label);
	  newMacroLabel->setTextWidth(newMacroLabel->boundingRect().width());
	} else if (newMacro->getConstraint() == SEMIPATHS) {
	  QString label = "S-" + QString::number(newMacro->getId());
	  newMacroLabel->setPlainText(label);
	  newMacroLabel->setTextWidth(newMacroLabel->boundingRect().width());
	}
	qreal xOffset = (newMacro->getWidth() / 2) - 20;
	newMacroLabel->setOffset(QPointF(xOffset, 0));
	newMacroLabel->setNewPos(newMacro->scenePos());
	newMacroLabel->setZValue(2);
	newMacroLabel->setDefaultTextColor(Qt::black);
	currentLayer.push_back(newMacro);
	partners.push_back(macro);
	Arrow *newArrow = new Arrow(newMacro, partLayer[it] , "Hierarchy", "", 0);
	newArrow->setCopy(true);
	scene->addItem(newArrow);
      }
    }
    QVectorIterator<EventItem*> it3(eventVector);
    while (it3.hasNext()) {
      EventItem *event = it3.next();
      if (event->getMacroEvent() == currentFather) {
	QVectorIterator<QGraphicsItem*> it4(partners);
	bool found = false;
	while (it4.hasNext()) {
	  MacroEvent *tempMacro = qgraphicsitem_cast<MacroEvent*>(it4.next());
	  if (tempMacro) {
	    QVector<EventItem*> tempIncidents = tempMacro->getIncidents();
	    if (tempIncidents.contains(event)) {
	      found = true;
	    }
	  }
	}
	if (!found) {
	  EventItem *newEvent = new EventItem(40, event->data(Qt::ToolTipRole).toString(),
					      QPointF(0, yPos), event->getId(), event->getOrder());
	  newEvent->setCopy(true);
	  newEvent->setMode(event->getMode());
	  newEvent->setColor(event->getColor());
	  newEvent->setPos(newEvent->getOriginalPos());
	  bool found = false;
	  if (newEvent->getMode() != "") {
	    QSqlQuery *query = new QSqlQuery;
	    query->prepare("SELECT description FROM incident_attributes "
			   "WHERE name = :name");
	    query->bindValue(":name", newEvent->getMode());
	    query->exec();
	    query->first();
	    QString description = query->value(0).toString();
	    delete query;
	    for (int i = 0; i < eventListWidget->rowCount(); i++) {
	      if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newEvent->getMode()) {
		found = true;
	      }
	    }
	    if (!found) {
	      QTableWidgetItem *item = new QTableWidgetItem(newEvent->getMode());
	      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	      QString toolTip = "<FONT SIZE = 3>" + newEvent->getMode() + " - " +
		description + "</FONT SIZE>";
	      item->setToolTip(toolTip);
	      item->setData(Qt::DisplayRole, newEvent->getMode());
	      eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	      eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	      eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		setBackground(newEvent->getColor());
	      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	    }
	  }
	  QPointer<NodeLabel> text = new NodeLabel(newEvent);
	  newEvent->setLabel(text);
	  text->setPlainText(QString::number(newEvent->getOrder()));
	  text->setTextWidth(text->boundingRect().width());
	  QPointF currentPos = newEvent->scenePos();
	  currentPos.setX(currentPos.x() - (text->textWidth() / 2));
	  currentPos.setY(currentPos.y() -12);
	  text->setPos(currentPos);    
	  text->setZValue(2);
	  text->setDefaultTextColor(Qt::black);
	  currentLayer.push_back(newEvent);
	  partners.push_back(event);
	  Arrow *newArrow = new Arrow(newEvent, partLayer[it], "Hierarchy", "", 0);
	  newArrow->setCopy(true);
	  scene->addItem(newArrow);
	}
      }
    }
  }
  qSort(currentLayer.begin(), currentLayer.end(), componentsSort);
  qSort(partners.begin(), partners.end(), componentsSort);
  QVector<MacroEvent*> newLayer;
  QVector<MacroEvent*> partnerLayer;
  int layerSize = currentLayer.size();
  qreal width = 120 * layerSize;
  qreal startX = 0 - width / 2 - 60;
  int count = 1;
  QVector<QGraphicsItem*>::iterator it5;
  for (QVector<QGraphicsItem*>::size_type it5 = 0; it5 != currentLayer.size(); it5++) {
    QGraphicsItem *current = currentLayer[it5];
    qreal xPos = startX + count * 120;
    current->setPos(xPos, current->scenePos().y());
    count++;
    EventItem *event = qgraphicsitem_cast<EventItem*>(current);
    MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
    if (event) {
      event->setOriginalPos(event->scenePos());
      event->getLabel()->setNewPos(event->scenePos());
      scene->addItem(event);
      scene->addItem(event->getLabel());
    } else if (macro) {
      macro->setOriginalPos(macro->scenePos());
      macro->getLabel()->setNewPos(macro->scenePos());
      scene->addItem(macro);
      scene->addItem(macro->getLabel());
      MacroEvent *tempMacro = qgraphicsitem_cast<MacroEvent*>(partners[it5]);
      newLayer.push_back(tempMacro);
      partnerLayer.push_back(qgraphicsitem_cast<MacroEvent*>(current));
    }
  }
  if (newLayer.size() > 0) {
    addLayer(newLayer, partnerLayer, layer + 1);
  }
}

void HierarchyGraphWidget::changeModeColor(QTableWidgetItem *item) {
  if (item->column() == 1) {
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    colorDialog->setCurrentColor(item->background().color());
    if (colorDialog->exec()) {
      QColor color = colorDialog->selectedColor();
      item->setBackground(color);
      QTableWidgetItem* neighbour = eventListWidget->item(item->row(), 0);
      QString mode = neighbour->data(Qt::DisplayRole).toString();
      QListIterator<QGraphicsItem*> it(scene->items());
      while (it.hasNext()) {
	EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	if (event) {
	  event = qgraphicsitem_cast<EventItem*>(it.next());
	  if (event->getMode() == mode) {
	    event->setColor(color);
	  }
	} else if (macro) {
	  macro = qgraphicsitem_cast<MacroEvent*>(it.next());
	  if (macro->getMode() == mode) {
	    macro->setColor(color);
	  }
	} else {
	  it.next();
	}
      }
    }
  }
}

void HierarchyGraphWidget::exportSvg() {
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

void HierarchyGraphWidget::assignAttribute() {
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
      QTextCursor cursPos = rawField->textCursor();
      if (empty) {
	query->prepare("INSERT INTO attributes_to_incidents (attribute, incident) "
		       "VALUES (:attribute, :incident)");
	query->bindValue(":attribute", attribute);
	query->bindValue(":incident", selectedIncident);
	query->exec();
	sourceText(attribute, selectedIncident);
	boldSelected(attributesTree, attribute, selectedIncident, INCIDENT);
	valueField->setEnabled(true);
      } else {
        sourceText(attribute, selectedIncident);
        highlightText();
	rawField->setTextCursor(cursPos);
      }
      delete query;
      setButtons();
    }
  } else if (selectedMacro != NULL) {
    if (attributesTreeView->currentIndex().isValid()) {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      QSet<QString> attributes = selectedMacro->getAttributes();
      if (!attributes.contains(attribute)) {
	selectedMacro->insertAttribute(attribute);
	QVectorIterator<MacroEvent*> it(macroVector);
	while (it.hasNext()) {
	  MacroEvent *macro = it.next();
	  if (macro->getId() == selectedMacro->getId()) {
	    macro->insertAttribute(attribute);							 
	  }
	} 
	boldSelected(attributesTree, attribute, selectedMacro->getId(), MACRO);
	valueField->setEnabled(true);
      }
    }
    setButtons();
  }
}

void HierarchyGraphWidget::unassignAttribute() {
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
      setButtons();
      delete query;
      delete query2;
    }
  } else if (selectedMacro != NULL) {
    if (attributesTreeView->currentIndex().isValid()) {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      QSet<QString> attributes = selectedMacro->getAttributes();
      if (attributes.contains(attribute)) {
	selectedMacro->removeAttribute(attribute);
	QVectorIterator<MacroEvent*> it(macroVector);
	while (it.hasNext()) {
	  MacroEvent *macro = it.next();
	  if (macro->getId() == selectedMacro->getId()) {
	    macro->removeAttribute(attribute);							 
	  }
	} 
	QSet<QString>::iterator it2;
	resetFont(attributesTree);
	attributes = selectedMacro->getAttributes();
	for (it2 = attributes.begin(); it2 != attributes.end(); it2++) {
	  QString current = *it2;
	  boldSelected(attributesTree, current, selectedMacro->getId(), MACRO);	  
	}
	setButtons();
	valueField->setText("");
	valueField->setEnabled(false);
	valueButton->setEnabled(false);
      }
    }
  }
}

void HierarchyGraphWidget::newAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString currentParent = treeFilter->
      mapToSource(attributesTreeView->currentIndex()).data().toString();
    QString name = "";
    QString description = "";
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *attribute = new QStandardItem(name);    
      attribute->setToolTip(description);
      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
      QStandardItem *father = attributesTree->
	itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
      father->appendRow(attribute);
      attribute->setToolTip(hint);
      attribute->setEditable(false);
      QSqlQuery *query = new QSqlQuery;
      query->prepare("INSERT INTO incident_attributes "
		     "(name, description, father) "
		     "VALUES (:name, :description, :father)");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":father", currentParent);
      query->exec();
      delete query;
      attributesWidget->resetTree();
      eventGraph->resetTree();
    }
    delete attributeDialog;
  } else {
    QString name = "";
    QString description = "";
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
    attributeDialog->exec();    
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("INSERT INTO incident_attributes "
		     "(name, description, father) "
		     "VALUES (:name, :description, 'NONE')");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->exec();
      QStandardItem *attribute = new QStandardItem(name);    
      attributesTree->appendRow(attribute);
      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
      delete query;
      attribute->setToolTip(hint);
      attribute->setEditable(false);
      attributesWidget->resetTree();
      eventGraph->resetTree();
    }
    delete attributeDialog;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void HierarchyGraphWidget::editAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString name = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM incident_attributes WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
    attributeDialog->submitName(name);
    attributeDialog->setDescription(description);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      QString newName = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *currentAttribute = attributesTree->
	itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
      currentAttribute->setData(newName);
      currentAttribute->setData(newName, Qt::DisplayRole);
      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
      currentAttribute->setToolTip(hint);
      query->prepare("UPDATE incident_attributes "
		     "SET name = :newname, description = :newdescription "
		     "WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdescription", description);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE incident_attributes "
		     "SET father = :newname "
		     "WHERE father = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_incidents "
		     "SET attribute = :newname "
		     "WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_incidents_sources "
		     "SET attribute = :newname "
		     "WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE saved_eg_plots_attributes_to_macro_events "
		     "SET attribute = :newname "
		     "WHERE attribute = :oldname");
      this->setCursor(Qt::WaitCursor);
      retrieveData();
      this->setCursor(Qt::ArrowCursor);
      attributesWidget->resetTree();
      eventGraph->resetTree();
    }
    delete query;
    delete attributeDialog;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void HierarchyGraphWidget::removeUnusedAttributes() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  QSet<QString> takenAttributes;
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) {
    MacroEvent* current = it.next();
    QSet<QString> attributes = current->getAttributes();
    QSet<QString>::iterator it2;
    for (it2 = attributes.begin(); it2 != attributes.end(); it2++) {
      takenAttributes.insert(*it2);
    }
  }
  while (unfinished) {
    query->exec("SELECT name FROM incident_attributes "
		"EXCEPT SELECT attribute FROM attributes_to_incidents "
		"EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_macro_events "
		"EXCEPT SELECT father FROM incident_attributes");
    QSet<QString> temp;
    while (query->next()) {
      QString current = query->value(0).toString();			   
      temp.insert(current);
    }
    QSet<QString>::iterator it3;
    bool found = false;
    for (it3 = temp.begin(); it3 != temp.end(); it3++) {
      if (!takenAttributes.contains(*it3)) {
	found = true;
	query2->prepare("DELETE FROM incident_attributes WHERE name = :current");
	query2->bindValue(":current", *it3);
	query2->exec();
      }
    }
    if (!found) {
      unfinished = false;
    }
  }
  this->setCursor(Qt::WaitCursor);
  attributesTreeView->setSortingEnabled(false);
  resetTree();
  attributesWidget->resetTree();
  eventGraph->resetTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
  delete query;  
  delete query2;
}

void HierarchyGraphWidget::changeFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void HierarchyGraphWidget::sourceText(const QString &attribute, const int &incident) {
  if (selectedIncident != 0) {
    if (rawField->textCursor().selectedText().trimmed() != "") {
      QSqlQuery *query = new QSqlQuery;
      int end = 0;
      int begin = 0;
      QTextCursor selectCursor = rawField->textCursor();
      if (rawField->textCursor().anchor() >= rawField->textCursor().position()) {
	begin = rawField->textCursor().position();
	end = rawField->textCursor().anchor();
      } else {
	begin = rawField->textCursor().anchor();
	end = rawField->textCursor().position();
      }
      begin++;
      end--;   
      selectCursor.setPosition(begin);
      selectCursor.movePosition(QTextCursor::StartOfWord);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
      rawField->setTextCursor(selectCursor);
      QString sourceText = rawField->textCursor().selectedText().trimmed();
    
      query->prepare("INSERT INTO attributes_to_incidents_sources "
		     "(attribute, incident, source_text)"
		     "VALUES (:att, :inc, :text)");
      query->bindValue(":att", attribute);
      query->bindValue(":inc", incident);
      query->bindValue(":text", sourceText);
      query->exec();
      delete query;
    }
  }
}

void HierarchyGraphWidget::highlightText() {
  if (selectedIncident != 0) {
    QTextCursor currentPos = rawField->textCursor();
    if (attributesTreeView->currentIndex().isValid()) {
      QStandardItem *currentAttribute = attributesTree->
	itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
      QString currentName = attributesTreeView->currentIndex().data().toString();
      if (currentAttribute->font().bold()) {
	QTextCharFormat format;
	format.setFontWeight(QFont::Normal);
	format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	rawField->selectAll();
	rawField->textCursor().mergeCharFormat(format);
	QTextCursor cursor = rawField->textCursor();
	cursor.movePosition(QTextCursor::Start);
	rawField->setTextCursor(cursor);
	QSqlQuery *query = new QSqlQuery;
	query->prepare("SELECT source_text "
			"FROM attributes_to_incidents_sources "
			"WHERE attribute = :attribute AND incident = :id");
	query->bindValue(":attribute", currentName);
	query->bindValue(":id", selectedIncident);
	query->exec();
	while (query->next()) {
	  QString currentText = query->value(0).toString();
	  while (rawField->find(currentText, QTextDocument::FindWholeWords)) {
	    format.setFontWeight(QFont::Bold);
	    format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
	    format.setUnderlineColor(Qt::blue);
	    rawField->textCursor().mergeCharFormat(format);
	  }
	  cursor = rawField->textCursor();
	  cursor.movePosition(QTextCursor::Start);
	  rawField->setTextCursor(cursor);
	}
	rawField->setTextCursor(currentPos);
	delete query;
      } else {
	QString currentSelected = rawField->textCursor().selectedText();
	QTextCharFormat format;
	format.setFontWeight(QFont::Normal);
	format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	rawField->selectAll();
	rawField->textCursor().mergeCharFormat(format);
	QTextCursor cursor = rawField->textCursor();
	cursor.movePosition(QTextCursor::Start);
	rawField->setTextCursor(cursor);
	rawField->find(currentSelected);
	rawField->setTextCursor(currentPos);
      }
    } else {
      QTextCharFormat format;
      format.setFontWeight(QFont::Normal);
      format.setUnderlineStyle(QTextCharFormat::NoUnderline);
      rawField->selectAll();
      rawField->textCursor().mergeCharFormat(format);
      QTextCursor cursor = rawField->textCursor();
      cursor.movePosition(QTextCursor::Start);
      rawField->setTextCursor(cursor);
      rawField->setTextCursor(currentPos);
    }
  }
}

void HierarchyGraphWidget::removeText() {
  if (selectedIncident != 0) {
    if (attributesTreeView->currentIndex().isValid()) {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      if (rawField->textCursor().selectedText().trimmed() != "") {
	QSqlQuery *query = new QSqlQuery;
	int end = 0;
	int begin = 0;
	QTextCursor selectCursor = rawField->textCursor();
	if (rawField->textCursor().anchor() >= rawField->textCursor().position()) {
	  begin = rawField->textCursor().position();
	  end = rawField->textCursor().anchor();
	} else {
	  begin = rawField->textCursor().anchor();
	  end = rawField->textCursor().position();
	}
	begin++;
	end--;
	selectCursor.setPosition(begin);
	selectCursor.movePosition(QTextCursor::StartOfWord);
	selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	rawField->setTextCursor(selectCursor);
	QString sourceText = rawField->textCursor().selectedText().trimmed();
	query->prepare("DELETE FROM attributes_to_incidents_sources "
		       "WHERE attribute = :att AND incident = :inc AND source_text = :text");
	query->bindValue(":att", attribute);
	query->bindValue(":inc", selectedIncident);
	query->bindValue(":text", sourceText);
	query->exec();
	delete query;
      }
      setButtons();
      highlightText();
    }
  }
}

void HierarchyGraphWidget::resetTexts() {
  if (attributesTreeView->currentIndex().isValid()) {
    if (selectedIncident != 0) {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("Resetting source texts cannot be undone. "
				     "Are you sure you want to proceed?");
      if (warningBox->exec() == QMessageBox::Yes) {
	QSqlQuery *query = new QSqlQuery;
	if (!(query->isNull(0))) {
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  query->prepare("DELETE FROM attributes_to_incidents_sources "
			 "WHERE attribute = :att AND incident = :inc");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", selectedIncident);
	  query->exec();
	}
	highlightText();
	delete query;
      }
      setButtons();
      delete warningBox;
    }
  }
}

void HierarchyGraphWidget::setOrigin(MacroEvent *submittedOrigin) {
  origin = submittedOrigin;
  buildComponents(origin, 1);
}

void HierarchyGraphWidget::setEvents(QVector<EventItem*> submittedEvents) {
  eventVector = submittedEvents;
}

void HierarchyGraphWidget::setMacros(QVector<MacroEvent*> submittedMacros) {
  macroVector = submittedMacros;
}

void HierarchyGraphWidget::setTree() {
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

void HierarchyGraphWidget::resetTree() {
  scene->clearSelection();
  retrieveData();
  delete attributesTree;
  setTree();
}

void HierarchyGraphWidget::buildHierarchy(QStandardItem *top, QString name) {
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

void HierarchyGraphWidget::fixTree() {
  resetFont(attributesTree);
  if (selectedIncident != 0) {
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_incidents "
		   "WHERE incident = :id");
    query->bindValue(":id", selectedIncident);
    query->exec();
    while (query->next()) {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute);
    }
    delete query;
  } else if (selectedMacro != NULL) {
    QSet<QString> attributes = selectedMacro->getAttributes();
    QSet<QString>::iterator it;
    for (it = attributes.begin(); it != attributes.end(); it++) {
      boldSelected(attributesTree, *it);
    }
  }
}

void HierarchyGraphWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) {
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

void HierarchyGraphWidget::boldSelected(QAbstractItemModel *model, QString name,
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

void HierarchyGraphWidget::setButtons() {
  if (attributesTreeView->currentIndex().isValid()) {
    if (selectedIncident != 0) {
      QString currentAttribute = attributesTreeView->currentIndex().data().toString();
      QSqlQuery *query = new QSqlQuery;
      bool empty = false;
      query->prepare("SELECT attribute, incident FROM "
		     "attributes_to_incidents "
		     "WHERE attribute = :att AND incident = :inc  ");
      query->bindValue(":att", currentAttribute);
      query->bindValue(":inc", selectedIncident);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	unassignAttributeButton->setEnabled(true);
      } else {
	unassignAttributeButton->setEnabled(false);
      }
      query->prepare("SELECT attribute, incident FROM "
		     "attributes_to_incidents_sources "
		     "WHERE attribute = :att AND incident = :inc");
      query->bindValue(":att", currentAttribute);
      query->bindValue(":inc", selectedIncident);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	removeTextButton->setEnabled(true);
	resetTextsButton->setEnabled(true);
      } else {
	removeTextButton->setEnabled(false);
	resetTextsButton->setEnabled(false);
      }
      assignAttributeButton->setEnabled(true);
      editAttributeButton->setEnabled(true);
      delete query;
    } else if (selectedMacro != NULL) {
      QString currentAttribute = attributesTreeView->currentIndex().data().toString();
      QSet<QString> attributes = selectedMacro->getAttributes();
      if (attributes.contains(currentAttribute)) {
	unassignAttributeButton->setEnabled(true);
      } else {
	unassignAttributeButton->setEnabled(false);
      }
      assignAttributeButton->setEnabled(true);
      editAttributeButton->setEnabled(true);
      removeTextButton->setEnabled(false);
      resetTextsButton->setEnabled(false);
    }
  } else {
    assignAttributeButton->setEnabled(false);
    editAttributeButton->setEnabled(false);
    unassignAttributeButton->setEnabled(false);
    removeTextButton->setEnabled(false);
    resetTextsButton->setEnabled(false);
  }
}

void HierarchyGraphWidget::cleanUp() {
  QGraphicsItemGroup *group = NULL;
  setComment();
  if (scene->items().size() > 0) {
    group = scene->createItemGroup(scene->items());
  }
  if (group != NULL) {
    scene->destroyItemGroup(group);
  }
  scene->clear();
  currentData.clear();
  selectedMacro = NULL;
  selectedIncident = 0;
  retrieveData();
  eventListWidget->setRowCount(0);
}

void HierarchyGraphWidget::switchBack() {
  setComment();
  emit goToEventGraph();
}

bool HierarchyGraphWidget::eventFilter(QObject *object, QEvent *event) {
  if (object == view->viewport() && event->type() == QEvent::MouseButtonRelease) {
    QMouseEvent *mouseEvent = (QMouseEvent*) event;
    if (mouseEvent->button() == Qt::LeftButton) {
      retrieveData();
      setButtons();
    }
  } else if (object == attributesTreeView && event->type() == QEvent::ChildRemoved) {
    fixTree();
  } else if (event->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = (QWheelEvent*) event;
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
    if (textEdit) {
      if(wheelEvent->modifiers() & Qt::ControlModifier) {
        if (wheelEvent->angleDelta().y() > 0) {
	  textEdit->zoomIn(1);
	} else if (wheelEvent->angleDelta().y() < 0) {
	  textEdit->zoomOut(1);
	}
      }
    }
  }
  return false;
}

void HierarchyGraphWidget::setAttributesWidget(AttributesWidget *aw) {
  attributesWidget = aw;
}

void HierarchyGraphWidget::setEventGraph(EventGraphWidget *egw) {
  eventGraph = egw;
}

void HierarchyGraphWidget::finalBusiness() {
  cleanUp();
}
