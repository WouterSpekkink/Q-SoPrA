#include "../include/HierarchyGraphWidget.h"

bool componentsSortHier(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) {
  const EventItem *eventOne = qgraphicsitem_cast<const EventItem*>(itemOne);
  const EventItem *eventTwo = qgraphicsitem_cast<const EventItem*>(itemTwo);
  const MacroEvent *macroOne = qgraphicsitem_cast<const MacroEvent*>(itemOne);
  const MacroEvent *macroTwo = qgraphicsitem_cast<const MacroEvent*>(itemTwo);
  if (eventOne && eventTwo) {
    return (eventOne->getOrder() < eventTwo->getOrder());
  } else if (eventOne && macroTwo) {
    return (eventOne->getOrder() < macroTwo->getIncidents().first()->getOrder());
  } else if (macroOne && macroTwo) {
    return (macroOne->getIncidents().first()->getOrder() <
	    macroTwo->getIncidents().first()->getOrder());
  } else {
    return (macroOne->getIncidents().first()->getOrder() < eventTwo->getOrder());
  }
}

bool eventLessThanHier(const QGraphicsItem *itemOne, const QGraphicsItem *itemTwo) {
  qreal orderOne = itemOne->scenePos().x();
  qreal orderTwo = itemTwo->scenePos().x();
  if (orderOne < orderTwo) {
    return true;
  } else {
    return false;
  }
}
                      
HierarchyGraphWidget::HierarchyGraphWidget(QWidget *parent) : QDialog(parent) {  
  vectorPos = 0;
  selectedMacro = NULL;
  selectedIncident = 0;
  
  scene = new Scene(this);
  view = new GraphicsView(scene);
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

  //  exportSvgButton = new QPushButton(tr("Export"), graphicsWidget);
  toggleDetailsButton = new QPushButton(tr("Toggle details"), this);
  toggleDetailsButton->setCheckable(true);
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
  exitButton = new QPushButton(tr("Exit"), this);
  
  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(seeAttributesButton, SIGNAL(clicked()), this, SLOT(showAttributes()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  //connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  //connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  //connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  //connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  //connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  //connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(seeCommentsButton, SIGNAL(clicked()), this, SLOT(showComments()));
  //  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  /*  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(highlightText()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
  this, SLOT(changeFilter(const QString &)));*/
  //  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  //  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(cleanUp()));
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

  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  drawOptionsLeftLayout->addWidget(toggleDetailsButton);
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(exitButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);
    
  setLayout(mainLayout);  
}

void HierarchyGraphWidget::toggleDetails() {
  //  setComment();
  if (infoWidget->isHidden()) {
    infoWidget->show();
  } else {
    infoWidget->hide();
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

void HierarchyGraphWidget::retrieveData() {
  //  setComment();
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
      qSort(currentData.begin(), currentData.end(), eventLessThanHier);   
      vectorPos = 0;
      QString indexText = "(" + QString::number(vectorPos + 1) + "/"
	+ QString::number(currentData.size()) + ")";
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
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
  newOrigin->setAttributes(submittedOrigin->getAttributes());
  newOrigin->setZValue(1);
  MacroLabel *macroLabel = new MacroLabel(newOrigin);
  newOrigin->setLabel(macroLabel);
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
      newMacro->setAttributes(macro->getAttributes());
      newMacro->setPos(newMacro->getOriginalPos());
      newMacro->setZValue(1);
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
      newEvent->setPos(newEvent->getOriginalPos());
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
      scene->addItem(newArrow);
      partners.push_back(event);
    }
  }
  qSort(currentLayer.begin(), currentLayer.end(), componentsSortHier);
  qSort(partners.begin(), partners.end(), componentsSortHier);
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
      event->getLabel()->setNewPos(event->scenePos());
      scene->addItem(event);
      scene->addItem(event->getLabel());
    } else if (macro) {
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
	newMacro->setAttributes(macro->getAttributes());
	newMacro->setPos(newMacro->getOriginalPos());
	newMacro->setZValue(1);
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
	  newEvent->setPos(newEvent->getOriginalPos());
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
	  scene->addItem(newArrow);
	}
      }
    }
  }
  qSort(currentLayer.begin(), currentLayer.end(), componentsSortHier);
  qSort(partners.begin(), partners.end(), componentsSortHier);
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
      event->getLabel()->setNewPos(event->scenePos());
      scene->addItem(event);
      scene->addItem(event->getLabel());
    } else if (macro) {
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
      //      setButtons();
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
    //setButtons();
  }
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

void HierarchyGraphWidget::cleanUp() {
  QGraphicsItemGroup *group = NULL;
  if (scene->items().size() > 0) {
    group = scene->createItemGroup(scene->items());
  }
  if (group != NULL) {
    scene->destroyItemGroup(group);
  }
  scene->clear();
  currentData.clear();
  vectorPos = 0;
  selectedMacro = NULL;
  selectedIncident = 0;
  // also empty text fields.
}

void HierarchyGraphWidget::switchBack() {
  emit goToEventGraph();
}

bool HierarchyGraphWidget::eventFilter(QObject *object, QEvent *event) {
  if (object == view->viewport() && event->type() == QEvent::MouseButtonRelease) {
    QMouseEvent *mouseEvent = (QMouseEvent*) event;
    if (mouseEvent->button() == Qt::LeftButton) {
      retrieveData();
      //setButtons();
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

