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
                      
HierarchyGraphWidget::HierarchyGraphWidget(QWidget *parent) : QDialog(parent) {  
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
  //setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  //  attributesTreeView->installEventFilter(this);*/

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
  //  connect(seeAttributesButton, SIGNAL(clicked()), this, SLOT(showAttributes()));
  //connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  //connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  //connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  //connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  //connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  //connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  //connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  //connect(seeCommentsButton, SIGNAL(clicked()), this, SLOT(showComments()));
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

void HierarchyGraphWidget::buildComponents(MacroEvent *submittedOrigin, int layer) {
  qreal yPos = 0 + layer * - 80;
  QVector<QGraphicsItem*> currentLayer;
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) {
    MacroEvent *macro = it.next();
    if (macro->getMacroEvent() == submittedOrigin) { 
      MacroEvent *temp = new MacroEvent(40, macro->getDescription(), QPointF(0,yPos),
					macro->getId(), macro->getConstraint(),
					macro->getIncidents());
      temp->setZValue(1);
      MacroLabel *tempLabel = new MacroLabel(temp);
      temp->setLabel(tempLabel);
      if (temp->getConstraint() == PATHS) {
	QString label = "P-" + QString::number(temp->getId());
	tempLabel->setPlainText(label);
	tempLabel->setTextWidth(tempLabel->boundingRect().width());
      } else if (temp->getConstraint() == SEMIPATHS) {
	QString label = "S-" + QString::number(temp->getId());
	tempLabel->setPlainText(label);
	tempLabel->setTextWidth(tempLabel->boundingRect().width());
      }
      qreal xOffset = (temp->getWidth() / 2) - 20;
      tempLabel->setOffset(QPointF(xOffset,0));
      tempLabel->setNewPos(temp->scenePos());
      tempLabel->setZValue(2);
      tempLabel->setDefaultTextColor(Qt::black);
      currentLayer.push_back(temp);
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
    }
  }
  qSort(currentLayer.begin(), currentLayer.end(), componentsSortHier);
  int layerSize = currentLayer.size();
  int count = 1;
  QVectorIterator<QGraphicsItem*> it3(currentLayer);
  while (it3.hasNext()) {
    QGraphicsItem *current = it3.next();
    if (count < layerSize / 2) {
      qreal xPos = (layerSize / 2 - count) * 80;
      current->setPos(xPos, current->scenePos().y());
    } else if (count > layerSize / 2) {
      qreal xPos = count - (layerSize / 2) * 80;
      current->setPos(xPos, current->scenePos().y());
    }
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
      buildComponents(macro, layer + 1);
    }
  }
}

void HierarchyGraphWidget::setOrigin(MacroEvent *submittedOrigin) {
  origin = submittedOrigin;
  MacroEvent *newOrigin = new MacroEvent(40, origin->getDescription(), QPointF(0,0),
					 origin->getId(), origin->getConstraint(),
					 origin->getIncidents());
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
  qreal xOffset = (newOrigin->getWidth() / 2) - 20;
  macroLabel->setOffset(QPointF(xOffset,0));
  macroLabel->setNewPos(newOrigin->scenePos());
  macroLabel->setZValue(2);
  macroLabel->setDefaultTextColor(Qt::black);
  scene->addItem(newOrigin);
  scene->addItem(macroLabel);
  buildComponents(origin, 1);
}

void HierarchyGraphWidget::setEvents(QVector<EventItem*> submittedEvents) {
  eventVector = submittedEvents;
}

void HierarchyGraphWidget::setMacros(QVector<MacroEvent*> submittedMacros) {
  macroVector = submittedMacros;
}

void HierarchyGraphWidget::cleanUp() {
  scene->clear();
  // also empty text fields.
}
