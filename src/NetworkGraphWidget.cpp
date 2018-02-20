#include "../include/NetworkGraphWidget.h"

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) : QWidget(parent) {
  selectedType = "";
  selectedEntityName = "";
  labelsShown = false;
  massMove = false;

  minOrder = 0;
  maxOrder = 0;
  vectorPos = 0;
  
  scene = new Scene(this);
  view = new GraphicsView(scene);
  view->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
  view->setDragMode(QGraphicsView::RubberBandDrag);
  view->setRubberBandSelectionMode(Qt::ContainsItemShape);
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  view->viewport()->installEventFilter(this);

  infoWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
  legendWidget = new QWidget(this);

  attributesTreeView = new DeselectableTreeViewEntities(infoWidget);
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
  
  typeLabel = new QLabel(tr("Select type"), this);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  nodeLegendLabel = new QLabel(tr("<b>Modes:</b>"), legendWidget);
  edgeLegendLabel = new QLabel(tr("<b>Edge legend:</b>"), legendWidget);
  nameLabel = new QLabel(tr("<b>Name:</b<"), infoWidget);
  descriptionLabel = new QLabel(tr("<b>Description:</b>"), infoWidget);
  attributesLabel = new QLabel(tr("<b>Attributes:</b>"), infoWidget);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), infoWidget);
  valueLabel = new QLabel(tr("<b>Value:</b>"), infoWidget);
  plotLabel = new QLabel(tr("Unsaved plot"), this);
  changeLabel = new QLabel(tr("*"), this);
  incongruencyLabel = new QLabel(tr(""), this);
  incongruencyLabel->setStyleSheet("QLabel {color : red;}");
  
  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);

  nameField = new QLineEdit(infoWidget);
  nameField->setReadOnly(true);
  attributesFilterField = new QLineEdit(infoWidget);
  valueField = new QLineEdit(infoWidget);
  valueField->setEnabled(false);
  
  descriptionField = new QTextEdit(infoWidget);
  descriptionField->setReadOnly(true);

  plotButton = new QPushButton(tr("Plot new"), this);
  plotButton->setEnabled(false);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  toggleDetailsButton = new QPushButton(tr("Toggle details"), this);
  toggleDetailsButton->setCheckable(true);
  previousNodeButton = new QPushButton(tr("<<"), infoWidget);
  previousNodeButton->setEnabled(false);   
  nextNodeButton = new QPushButton(tr(">>"), infoWidget);
  nextNodeButton->setEnabled(false);
  valueButton = new QPushButton(tr("Store value"), infoWidget);
  valueButton->setEnabled(false);
  assignAttributeButton = new QPushButton(tr("Assign attribute"), infoWidget);
  unassignAttributeButton = new QPushButton(tr("Unassign attribute"), infoWidget);
  addAttributeButton = new QPushButton(tr("New attribute"), infoWidget);
  editAttributeButton = new QPushButton(tr("Edit attribute"), infoWidget);
  removeUnusedAttributesButton = new QPushButton(tr("Remove unused"), infoWidget);
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  addButton = new QPushButton(tr("Add relationship type"), this);
  addButton->setEnabled(false);
  toggleLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  increaseFontSizeButton = new QPushButton(tr("+"));
  decreaseFontSizeButton = new QPushButton(tr("-"));
  colorByAttributeButton = new QPushButton(tr("Create mode"), legendWidget);
  nodeColorButton = new QPushButton(tr("Set node color"), graphicsWidget);
  labelColorButton = new QPushButton(tr("Set label color"), graphicsWidget);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  exportNodesButton = new QPushButton(tr("Export nodes"), graphicsWidget);
  exportEdgesButton = new QPushButton(tr("Export edges"), graphicsWidget);
  setFilteredButton = new QPushButton(tr("Filter on"), legendWidget);
  setFilteredButton->setCheckable(true);
  setFilteredButton->setChecked(true);
  setFilteredButton->setEnabled(false);
  unsetFilteredButton = new QPushButton(tr("Filter off"), legendWidget);
  unsetFilteredButton->setCheckable(true);
  unsetFilteredButton->setChecked(false);
  unsetFilteredButton->setEnabled(false);
  hideTypeButton = new QPushButton(tr("Hide"), legendWidget);
  hideTypeButton->setCheckable(true);
  hideTypeButton->setChecked(false);
  showTypeButton = new QPushButton(tr("Show"), legendWidget);
  showTypeButton->setCheckable(true);
  showTypeButton->setChecked(true);
  multimodeButton = new QPushButton(tr("Multimode trans."), legendWidget);
  removeModeButton = new QPushButton(tr("Remove mode"), legendWidget);
  removeModeButton->setEnabled(false);
  mergeButton = new QPushButton(tr("Merge"), legendWidget);
  simpleLayoutButton = new QPushButton(tr("Spring layout"), graphicsWidget);
  circularLayoutButton = new QPushButton(tr("Circular layout"), graphicsWidget);
  expandLayoutButton = new QPushButton(tr("Expand"), graphicsWidget);
  contractLayoutButton = new QPushButton(tr("Contract"), graphicsWidget);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  plotButton->setEnabled(false);
  removeTypeButton = new QPushButton(tr("Remove"), legendWidget);
  removeTypeButton->setEnabled(false);
  restoreModeColorsButton = new QPushButton(tr("Restore colors"), legendWidget);
  moveModeUpButton = new QPushButton(tr("Up"), this);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton = new QPushButton(tr("Down"), this);
  moveModeDownButton->setEnabled(false);
  
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

  nodeListWidget = new DeselectableListWidget(legendWidget);
  edgeListWidget = new DeselectableListWidget(legendWidget);

  nodeListWidget->setColumnCount(2);
  nodeListWidget->horizontalHeader()->hide();
  nodeListWidget->verticalHeader()->hide();
  nodeListWidget->setColumnWidth(1, 20);
  nodeListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  nodeListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  nodeListWidget->setStyleSheet("QTableView {gridline-color: black}");
  nodeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  
  edgeListWidget->setColumnCount(2);
  edgeListWidget->horizontalHeader()->hide();
  edgeListWidget->verticalHeader()->hide();
  edgeListWidget->setColumnWidth(1, 20);
  edgeListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  edgeListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  edgeListWidget->setStyleSheet("QTableView {gridline-color: black}");
  edgeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setFilter(const QString &)));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(getValue()));
  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(valueField, SIGNAL(textChanged(const QString &)), this, SLOT(setValueButton()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(previousNodeButton, SIGNAL(clicked()), this, SLOT(previousDataItem()));
  connect(nextNodeButton, SIGNAL(clicked()), this, SLOT(nextDataItem()));
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(simpleLayoutButton, SIGNAL(clicked()), this, SLOT(simpleLayout()));
  connect(circularLayoutButton, SIGNAL(clicked()), this, SLOT(circularLayout()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotNewGraph()));
  connect(addButton, SIGNAL(clicked()), this, SLOT(addRelationshipType()));
  connect(removeTypeButton, SIGNAL(clicked()), this, SLOT(removeRelationshipType()));
  connect(colorByAttributeButton, SIGNAL(clicked()), this, SLOT(colorByAttribute()));
  connect(nodeColorButton, SIGNAL(clicked()), this, SLOT(setNodeColor()));
  connect(labelColorButton, SIGNAL(clicked()), this, SLOT(setLabelColor()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(toggleLabelsButton, SIGNAL(clicked()), this, SLOT(toggleLabels()));
  connect(increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseLabelSize()));
  connect(decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseLabelSize()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(multimodeButton, SIGNAL(clicked()), this, SLOT(multimodeTransformation()));
  connect(removeModeButton, SIGNAL(clicked()), this, SLOT(removeMode()));
  connect(mergeButton, SIGNAL(clicked()), this, SLOT(mergeRelationships()));
  connect(edgeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setFilterButtons(QTableWidgetItem *)));
  connect(edgeListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeEdgeColor(QTableWidgetItem *)));
  connect(edgeListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableFilterButtons()));
  connect(nodeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setModeButtons(QTableWidgetItem *)));
  connect(nodeListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableModeButtons()));
  connect(nodeListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeModeColor(QTableWidgetItem *)));
  connect(setFilteredButton, SIGNAL(clicked()), this, SLOT(activateFilter()));
  connect(unsetFilteredButton, SIGNAL(clicked()), this, SLOT(deactivateFilter()));
  connect(hideTypeButton, SIGNAL(clicked()), this, SLOT(hideType()));
  connect(showTypeButton, SIGNAL(clicked()), this, SLOT(showType()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(exportNodesButton, SIGNAL(clicked()), this, SLOT(exportNodes()));
  connect(exportEdgesButton, SIGNAL(clicked()), this, SLOT(exportEdges()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(expandLayoutButton, SIGNAL(clicked()), this, SLOT(expandLayout()));
  connect(restoreModeColorsButton, SIGNAL(clicked()), this, SLOT(restoreModeColors()));
  connect(moveModeUpButton, SIGNAL(clicked()), this, SLOT(moveModeUp()));
  connect(moveModeDownButton, SIGNAL(clicked()), this, SLOT(moveModeDown()));
  connect(contractLayoutButton, SIGNAL(clicked()), this, SLOT(contractLayout()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(typeLabel);
  plotOptionsLayout->addWidget(typeComboBox);
  plotOptionsLayout->addWidget(plotButton);
  plotOptionsLayout->addWidget(addButton);
  plotOptionsLayout->addWidget(savePlotButton);
  plotOptionsLayout->addWidget(seePlotsButton);
  plotOptionsLayout->addWidget(plotLabel);
  plotOptionsLayout->addWidget(changeLabel);
  plotOptionsLayout->addWidget(incongruencyLabel);

  topLayout->addLayout(plotOptionsLayout);
  plotOptionsLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(topLayout);

  QPointer<QHBoxLayout> screenLayout = new QHBoxLayout;

  QPointer<QVBoxLayout> detailsLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> navigationLayout = new QHBoxLayout;
  navigationLayout->addWidget(previousNodeButton);
  navigationLayout->addWidget(nextNodeButton);
  detailsLayout->addLayout(navigationLayout);
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  topFieldsLayout->addWidget(nameLabel);
  topFieldsLayout->addWidget(nameField);
  detailsLayout->addLayout(topFieldsLayout);
  detailsLayout->addWidget(descriptionLabel);
  detailsLayout->addWidget(descriptionField);
  detailsLayout->addWidget(attributesLabel);
  detailsLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributesFilterLabel);
  filterLayout->addWidget(attributesFilterField);
  detailsLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> valueLayout = new QHBoxLayout;
  valueLayout->addWidget(valueLabel);
  valueLayout->addWidget(valueField);
  valueLayout->addWidget(valueButton);
  detailsLayout->addLayout(valueLayout);
  QPointer<QHBoxLayout> attributesTopLayout = new QHBoxLayout;
  attributesTopLayout->addWidget(assignAttributeButton);
  attributesTopLayout->addWidget(unassignAttributeButton);
  detailsLayout->addLayout(attributesTopLayout);
  QPointer<QHBoxLayout> attributesBottomLayout = new QHBoxLayout;
  attributesBottomLayout->addWidget(addAttributeButton);
  attributesBottomLayout->addWidget(editAttributeButton);
  attributesBottomLayout->addWidget(removeUnusedAttributesButton);
  detailsLayout->addLayout(attributesBottomLayout);
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
  legendLayout->addWidget(nodeLegendLabel);
  legendLayout->addWidget(nodeListWidget);
  QPointer<QHBoxLayout> modeButtonsLayout = new QHBoxLayout;
  modeButtonsLayout->addWidget(moveModeUpButton);
  modeButtonsLayout->addWidget(moveModeDownButton);
  legendLayout->addLayout(modeButtonsLayout);
  legendLayout->addWidget(colorByAttributeButton);
  legendLayout->addWidget(multimodeButton);
  legendLayout->addWidget(removeModeButton);
  legendLayout->addWidget(restoreModeColorsButton);
  legendLayout->addWidget(edgeLegendLabel);
  legendLayout->addWidget(edgeListWidget);
  legendLayout->addWidget(setFilteredButton);
  legendLayout->addWidget(unsetFilteredButton);
  QPointer<QFrame> sepLine = new QFrame();
  sepLine->setFrameShape(QFrame::HLine);
  legendLayout->addWidget(sepLine);
  legendLayout->addWidget(hideTypeButton);
  legendLayout->addWidget(showTypeButton);
  QPointer<QFrame> sepLine2 = new QFrame();
  sepLine2->setFrameShape(QFrame::HLine);
  legendLayout->addWidget(sepLine2);
  legendLayout->addWidget(mergeButton);
  legendLayout->addWidget(removeTypeButton);
  legendWidget->setMinimumWidth(175);
  legendWidget->setMaximumWidth(175);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				   
  
  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(simpleLayoutButton);
  graphicsControlsLayout->addWidget(circularLayoutButton);
  QPointer<QHBoxLayout> expansionLayout = new QHBoxLayout;
  expansionLayout->addWidget(expandLayoutButton);
  expansionLayout->addWidget(contractLayoutButton);
  graphicsControlsLayout->addLayout(expansionLayout);
  QPointer<QFrame> sepLine3 = new QFrame();
  sepLine3->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine3);
  graphicsControlsLayout->addWidget(nodeColorButton);
  graphicsControlsLayout->addWidget(labelColorButton);
  graphicsControlsLayout->addWidget(backgroundColorButton);
  graphicsControlsLayout->addWidget(toggleLabelsButton);
  QPointer<QHBoxLayout> fontSizeLayout = new QHBoxLayout;
  fontSizeLayout->addWidget(increaseFontSizeButton);
  fontSizeLayout->addWidget(decreaseFontSizeButton);
  graphicsControlsLayout->addLayout(fontSizeLayout);
  QPointer<QFrame> sepLine4 = new QFrame();
  sepLine4->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine4);
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
  QPointer<QFrame> sepLine5 = new QFrame();
  sepLine5->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine5);
  graphicsControlsLayout->addWidget(exportSvgButton);
  graphicsControlsLayout->addWidget(exportNodesButton);
  graphicsControlsLayout->addWidget(exportEdgesButton);
  graphicsWidget->setMaximumWidth(175);
  graphicsWidget->setMinimumWidth(175);
  graphicsWidget->setLayout(graphicsControlsLayout);
  graphicsControlsLayout->setAlignment(Qt::AlignBottom);
  screenLayout->addWidget(graphicsWidget);

  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  drawOptionsLeftLayout->addWidget(toggleDetailsButton);
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);

  setLayout(mainLayout);

  getTypes();

  infoWidget->hide();
  graphicsWidget->hide();
  legendWidget->hide();
}

void NetworkGraphWidget::checkCongruency() {
  if (nodeVector.size() > 0) {
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT name, description FROM entities ORDER BY name ASC");
    QVector<QString> tempNames;
    QVector<QString> tempDescs;
    while (query->next()) {
      QString name = query->value(0).toString();
      QString desc = query->value(1).toString();
      tempNames.push_back(name);
      tempDescs.push_back(desc);
    }
    QVector<QString> tempDirected;
    QVector<QString> tempUndirected;  
    query->prepare("SELECT name FROM relationship_types "
		   "WHERE directedness = :directed");
    query->bindValue(":directed", DIRECTED);
    query->exec();
    while (query->next()) {
      QString type = query->value(0).toString();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT name "
		      "FROM entity_relationships "
		      "WHERE type = :type");
      query2->bindValue(":type", type);
      query2->exec();
      while (query2->next()) {
	QString relationship = query2->value(0).toString();
	tempDirected.push_back(relationship);
      }
      delete query2;
    }
    query->prepare("SELECT name FROM relationship_types "
		   "WHERE directedness = :undirected");
    query->bindValue(":undirected", UNDIRECTED);
    query->exec();
    while (query->next()) {
      QString type = query->value(0).toString();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT name "
		      "FROM entity_relationships "
		      "WHERE type = :type");
      query2->bindValue(":type", type);
      query2->exec();
      while (query2->next()) {
	QString relationship = query2->value(0).toString();
	tempUndirected.push_back(relationship);
      }
      delete query2;
    }
    std::sort(nodeVector.begin(), nodeVector.end(), nameLessThan);
    if (tempNames.size() != nodeVector.size()) {
      incongruencyLabel->setText("Incongruency detected");
      return;
    }
    for (QVector<NetworkNode*>::size_type i = 0; i != nodeVector.size(); i++) {
      NetworkNode *current = nodeVector[i];
      if (current->getName() != tempNames[i]) {
	incongruencyLabel->setText("Incongruency detected");
	return;
      }
      if (current->getDescription() != tempDescs[i]) {
	incongruencyLabel->setText("Incongruency detected");
	return;
      }
    }
    query->exec("SELECT COUNT(*) FROM entity_relationships");
    query->first();
    int originalRelationships = query->value(0).toInt();
    int count = 0;
    QVectorIterator<DirectedEdge*> it(directedVector);
    while (it.hasNext()) {
      DirectedEdge *current = it.next();
      if (current->getName() != CREATED) {
	count++;
      }
    }
    QVectorIterator<UndirectedEdge*> it2(undirectedVector);
    while (it2.hasNext()) {
      UndirectedEdge *current = it2.next();
      if (current->getName() != CREATED) {
	count++;
      }
    }
    if (count != originalRelationships) {
      incongruencyLabel->setText("Incongruency detected");
      return;
    }
    delete query;
    incongruencyLabel->setText("");
  }
}

void NetworkGraphWidget::toggleDetails() {
  if (infoWidget->isHidden()) {
    infoWidget->show();
  } else {
    infoWidget->hide();
  }
  view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void NetworkGraphWidget::toggleGraphicsControls() {
  if (graphicsWidget->isHidden()) {
    graphicsWidget->show();
  } else {
    graphicsWidget->hide();
  }
  view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void NetworkGraphWidget::toggleLegend() {
  if (legendWidget->isHidden()) {
    legendWidget->show();
  } else {
    legendWidget->hide();
  }
  view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void NetworkGraphWidget::retrieveData() {
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->blockSignals(false);
  valueField->setEnabled(false);
  valueButton->setEnabled(false);
  attributesTreeView->clearSelection();
  if (currentData.size() > 0) {
    currentData.clear();
  }
  if (scene->selectedItems().size() > 0) {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext()) {
      NetworkNode *node = qgraphicsitem_cast<NetworkNode*>(it.peekNext());
      DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(it.peekNext());
      UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(it.peekNext());
      if (node && !(directed) && !(undirected)) {
	NetworkNode *currentNode = qgraphicsitem_cast<NetworkNode*>(it.next());
	currentNode->setSelectionColor(Qt::black);
	currentNode->update();
	currentData.push_back(currentNode);
      } else {
	it.next();
      }
    }
    if (currentData.size() > 0) {
      std::sort(currentData.begin(), currentData.end(), nodeLessThan);
    
      vectorPos = 0;
      NetworkNode *currentNode = currentData.at(vectorPos);
      currentNode->setSelectionColor(Qt::red);
      currentNode->update();
      QString name = currentNode->getName();
      selectedEntityName = name;
      QString description = currentNode->getDescription();
      nameField->setText(name);
      descriptionField->setText(description);
      resetFont(attributesTree);
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute FROM attributes_to_entities "
		     "WHERE entity = :name");
      query->bindValue(":name", name);
      query->exec();
      while (query->next()) {
	QString attribute = query->value(0).toString();
	boldSelected(attributesTree, attribute);
      }
      delete query;      
      previousNodeButton->setEnabled(true);
      nextNodeButton->setEnabled(true);
    }
  } else {
    nameField->clear();
    selectedEntityName = "";
    descriptionField->clear();
    previousNodeButton->setEnabled(false);
    nextNodeButton->setEnabled(false);
    resetFont(attributesTree);
  }
}

void NetworkGraphWidget::setTree() {
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM entity_attributes WHERE father = 'NONE'");
  while (query->next()) {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *father = new QStandardItem(name);    
    attributesTree->appendRow(father);
    QString hint = breakString(description);
    father->setToolTip(hint);
    father->setEditable(false);
    buildHierarchy(father, name);
  }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void NetworkGraphWidget::resetTree() {
  delete attributesTree;
  setTree();
}

void NetworkGraphWidget::buildHierarchy(QStandardItem *top, QString name) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entity_attributes WHERE  father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) {
    QString childName = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *child = new QStandardItem(childName);
    top->setChild(children, child);
    QString hint = breakString(description);
    child->setToolTip(hint);
    child->setEditable(false);
    children++;
    buildHierarchy(child, childName);
  }
  delete query;
}

void NetworkGraphWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) {
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

void NetworkGraphWidget::boldSelected(QAbstractItemModel *model, QString name,
				      QString entity, QModelIndex parent) {
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
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT attribute, entity FROM attributes_to_entities "
			 "WHERE attribute = :attribute AND entity = :entity");
	  query->bindValue(":attribute", parentName);
	  query->bindValue(":entity", entity);
	  query->exec();
	  query->first();
	  if (query->isNull(0)) {
	    currentAttribute->setFont(font2);      
	  } else {
	    currentAttribute->setFont(font3);
	  }
        }
      }
    }
    if (model->hasChildren(index)) {
      boldSelected(model, name, entity, index);
    }
  }
}

void NetworkGraphWidget::fixTree(QString entity) {
  resetFont(attributesTree);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT attribute FROM attributes_to_entities "
		 "WHERE entity = :entity");
  query->bindValue(":entity", entity);
  query->exec();
  while (query->next()) {
    QString attribute = query->value(0).toString();
    boldSelected(attributesTree, attribute, entity);
  }
  delete query;
}

void NetworkGraphWidget::setFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void NetworkGraphWidget::previousDataItem() {
  if (vectorPos > 0) {
    NetworkNode *currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    vectorPos--;
    currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    selectedEntityName = name;
    descriptionField->setText(description);
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
		   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next()) {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;      
  } else {
    NetworkNode *currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    vectorPos = currentData.size() - 1;
    currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    scene->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    selectedEntityName = name;
    descriptionField->setText(description);
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
		   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next()) {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;      
  }
}

void NetworkGraphWidget::nextDataItem() {
  if (vectorPos != currentData.size() - 1) {
    NetworkNode *currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    vectorPos++;
    currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    selectedEntityName = name;
    descriptionField->setText(description);
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
		   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next()) {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;      
  } else {
    NetworkNode *currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    vectorPos = 0;
    currentNode = currentData.at(vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    selectedEntityName = name;
    descriptionField->setText(description);
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
		   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next()) {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;      
  }
}

void NetworkGraphWidget::setValueButton() {
  valueButton->setEnabled(true);
}

void NetworkGraphWidget::setValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQuery *query = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    query->prepare("UPDATE attributes_to_entities "
		   "SET value = :val "
		   "WHERE attribute = :attribute AND entity = :entity");
    query->bindValue(":val", valueField->text());
    query->bindValue(":attribute", attribute);
    query->bindValue(":entity", selectedEntityName);
    query->exec();
    valueButton->setEnabled(false);
    delete query;
  }
}

void NetworkGraphWidget::getValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    valueField->blockSignals(true);
    valueField->setText("");
    valueField->blockSignals(false);
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute, value "
		   "FROM attributes_to_entities "
		   "WHERE attribute =:att AND entity = :entity");
    query->bindValue(":att", attribute);
    query->bindValue(":entity", selectedEntityName);
    query->exec();
    query->first();
    if (!(query->isNull(0))) {
      valueField->setEnabled(true);
    } else {
      valueField->setEnabled(false);
      valueField->setText("");
    }
    if (!(query->isNull(1))) {
      QString value = query->value(1).toString();
      valueField->setText(value);
    } else {
      valueField->setText("");
    }
    valueButton->setEnabled(false);
    delete query;
  } else {
    valueField->setText("");
    valueButton->setEnabled(false);
  }
}

void NetworkGraphWidget::assignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    bool empty = false;
    query->prepare("SELECT attribute FROM "
		   "attributes_to_entities WHERE "
		   "attribute = :att AND entity = :entity");
    query->bindValue(":att", attribute);
    query->bindValue(":entity", selectedEntityName);
    query->exec();
    query->first();
    empty = query->isNull(0);
    if (empty) {
      query->prepare("INSERT INTO attributes_to_entities (attribute, entity) "
		     "VALUES (:attribute, :entity)");
      query->bindValue(":attribute", attribute);
      query->bindValue(":entity", selectedEntityName);
      query->exec();
      boldSelected(attributesTree, attribute, selectedEntityName);
      valueField->setEnabled(true);
    }
    delete query;
  }
}

void NetworkGraphWidget::unassignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQuery *query = new QSqlQuery;
    QSqlQuery *query2 = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    bool empty = false;
    query->prepare("SELECT attribute, entity "
		   "FROM attributes_to_entities "
		   "WHERE attribute = :att AND entity = :entity");
    query->bindValue(":att", attribute);
    query->bindValue(":entity", selectedEntityName);
    query->exec();
    query->first();
    empty = query->isNull(0);
    if (!empty) {
      query->prepare("DELETE FROM attributes_to_entities "
		     "WHERE attribute = :att AND entity = :entity");
      query->bindValue(":att", attribute);
      query->bindValue(":entity", selectedEntityName);
      query->exec();
      resetFont(attributesTree);
      query2->prepare("SELECT attribute, entity FROM attributes_to_entities "
		      "WHERE entity = :entity");
      query2->bindValue(":entity", selectedEntityName);
      query2->exec();
      while (query2->next()) {
	QString attribute = query2->value(0).toString();
	boldSelected(attributesTree, attribute, selectedEntityName);
      }
      valueField->setText("");
      valueField->setEnabled(false);
      valueButton->setEnabled(false);
    }
    delete query;
    delete query2;
  }
}

void NetworkGraphWidget::addAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString currentParent = treeFilter->mapToSource(attributesTreeView->currentIndex()).data().toString();
    QString name = "";
    QString description = "";
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, ENTITY);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      QSqlQuery *query = new QSqlQuery;
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *attribute = new QStandardItem(name);    
      attribute->setToolTip(description);
      QStandardItem *father = attributesTree->itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
      father->appendRow(attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);      
      query->prepare("INSERT INTO entity_attributes (name, description, father) "
		     "VALUES (:name, :description, :father");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":father", currentParent);
      query->exec();
      delete query;
    }
    delete attributeDialog;
  } else {
    QSqlQuery *query = new QSqlQuery;
    QString name = "";
    QString description = "";
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, ENTITY);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      query->prepare("INSERT INTO entity_attributes "
		     "(name, description, father) "
		     "VALUES (:name, :description, :father)");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":father", "NONE");
      query->exec();
      QStandardItem *attribute = new QStandardItem(name);    
      attributesTree->appendRow(attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);
    }
    delete attributeDialog;
    delete query;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void NetworkGraphWidget::editAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString name = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entity_attributes WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, ENTITY);
    attributeDialog->submitName(name);
    attributeDialog->setDescription(description);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      QString newName = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
      currentAttribute->setData(newName);
      currentAttribute->setData(newName, Qt::DisplayRole);      
      currentAttribute->setToolTip(description);
      query->prepare("UPDATE entity_attributes "
		     "SET name = :newname, description = :newdescription "
		     "WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdescription", description);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE entity_attributes "
		     "SET father = :newname "
		     "WHERE father = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_entities "
		     "SET attribute = :newname "
		     "WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
    }
    delete attributeDialog;
    delete query;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void NetworkGraphWidget::removeUnusedAttributes() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  while (unfinished) {
    query->exec("SELECT name FROM entity_attributes EXCEPT SELECT attribute "
		"FROM attributes_to_entities EXCEPT SELECT father "
		"FROM entity_attributes");
    while (query->next()) {
      QString current = query->value(0).toString();
      query2->prepare("DELETE FROM entity_attributes WHERE name = :current");
      query2->bindValue(":current", current);
      query2->exec();
    }
    query->first();
    if (query->isNull(0)) {
      unfinished = false;
    }
  }
  this->setCursor(Qt::WaitCursor);
  attributesTreeView->setSortingEnabled(false);
  resetTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  this->setCursor(Qt::ArrowCursor);
  delete query;  
  delete query2;
}

void NetworkGraphWidget::getTypes() {
  typeComboBox->clear();
  typeComboBox->addItem(DEFAULT);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM relationship_types ORDER BY name ASC");
  while (query->next()) {
    QString currentType = query->value(0).toString();
    typeComboBox->addItem(currentType);
  }
  delete query;
}

void NetworkGraphWidget::getEntities() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM entities");
  while (query->next()) {
    QString name = query->value(0).toString();    
    QString description = query->value(1).toString();
    NetworkNode *currentNode = new NetworkNode(name, description);
    NetworkNodeLabel *label = new NetworkNodeLabel(currentNode);
    currentNode->setColor(Qt::black);
    label->setPlainText(name);
    label->setDefaultTextColor(Qt::black);
    labelVector.push_back(label);
    currentNode->setLabel(label);
    label->setZValue(2);
    label->hide();
    currentNode->setZValue(1);
    currentNode->hide();
    nodeVector.push_back(currentNode);
  }
  delete query;
}

void NetworkGraphWidget::plotEntities() {
  QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode *currentNode = it.next();
    scene->addItem(currentNode);
    scene->addItem(currentNode->getLabel());
  }
}

void NetworkGraphWidget::getDirectedEdges() {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM relationship_types "
		 "WHERE directedness = :directed");
  query->bindValue(":directed", "Directed");
  query->exec();
  while (query->next()) {
    QString type = query->value(0).toString();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT DISTINCT relationship, incident "
		    "FROM relationships_to_incidents "
		    "WHERE type = :type");
    query2->bindValue(":type", type);
    query2->exec();
    while (query2->next()) {
      QString relationship = query2->value(0).toString();
      int incident = query2->value(1).toInt();
      QSqlQuery *query3 = new QSqlQuery;
      query3->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
      query3->bindValue(":incident", incident);
      query3->exec();
      query3->first();
      int order = query3->value(0).toInt();
      if (minOrder == 0) {
	minOrder = order;
      } else if (order < minOrder) {
	minOrder = order;
      }
      if (maxOrder == 0) {
	maxOrder = order;
      } else if (order > maxOrder) {
	maxOrder = order;
      }
      delete query3;
    }
    query2->prepare("SELECT name, source, target, comment "
		    "FROM entity_relationships "
		    "WHERE type = :type");
    query2->bindValue(":type", type);
    query2->exec();
    while (query2->next()) {
      QString name  = query2->value(0).toString();
      QString source = query2->value(1).toString();
      QString target = query2->value(2).toString();
      QString comment = query2->value(3).toString();
      QVectorIterator<NetworkNode*> it(nodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext()) {
	NetworkNode* currentNode = it.next();
	if (currentNode->getName() == source) {
	  tempSource = currentNode;
	} else if (currentNode->getName() == target) {
	  tempTarget = currentNode;
	}
	if (tempSource != NULL && tempTarget != NULL) {
	  DirectedEdge *currentEdge = new DirectedEdge(tempSource, tempTarget, type, name);
	  currentEdge->setComment(comment);
	  currentEdge->hide();
	  directedVector.push_back(currentEdge);
	  break;
	}
      }
    }      
    delete query2;
  }
  delete query;
}

void NetworkGraphWidget::getUndirectedEdges() {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM relationship_types "
		 "WHERE directedness = :directed");
  query->bindValue(":directed", "Undirected");
  query->exec();
  while (query->next()) {
    QString type = query->value(0).toString();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT DISTINCT relationship, incident "
		    "FROM relationships_to_incidents "
		    "WHERE type = :type");
    query2->bindValue(":type", type);
    query2->exec();
    while (query2->next()) {
      QString relationship = query2->value(0).toString();
      int incident = query2->value(1).toInt();
      QSqlQuery *query3 = new QSqlQuery;
      query3->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
      query3->bindValue(":incident", incident);
      query3->exec();
      query3->first();
      int order = query3->value(0).toInt();
      if (minOrder == 0) {
	minOrder = order;
      } else if (order < minOrder) {
	minOrder = order;
      }
      if (maxOrder == 0) {
	maxOrder = order;
      } else if (order > maxOrder) {
	maxOrder = order;
      }
      delete query3;
    }
    query2->prepare("SELECT name, source, target, comment "
		    "FROM entity_relationships "
		    "WHERE type = :type");
    query2->bindValue(":type", type);
    query2->exec();
    while (query2->next()) {
      QString name = query2->value(0).toString();
      QString source = query2->value(1).toString();
      QString target = query2->value(2).toString();
      
      QVectorIterator<NetworkNode*> it(nodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext()) {
	NetworkNode* currentNode = it.next();
	if (currentNode->getName() == source) {
	  tempSource = currentNode;
	} else if (currentNode->getName() == target) {
	  tempTarget = currentNode;
	}
	if (tempSource != NULL && tempTarget != NULL) {
	  UndirectedEdge *currentEdge;
	  if (tempSource->getName() < tempTarget->getName()) {
	    currentEdge = new UndirectedEdge(tempSource, tempTarget, type, name);
	  } else {
	    currentEdge = new UndirectedEdge(tempTarget, tempSource, type, name);
	  }
	  currentEdge->hide();
	  undirectedVector.push_back(currentEdge);
	  break;
	}
      }
    }
    delete query2;
  }
  delete query;
}

void NetworkGraphWidget::plotDirectedEdges(QString type, QColor color) {
  QVectorIterator<DirectedEdge*> it(directedVector);
  while (it.hasNext()) {
    DirectedEdge* currentEdge = it.next();
    if (currentEdge->getType() == type) {
      /*QListIterator<QGraphicsItem*> it2(scene->items());
      while (it2.hasNext()) {
	DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(it2.peekNext());
	UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(it2.peekNext());
	if (directed) {
	  DirectedEdge *current = qgraphicsitem_cast<DirectedEdge*>(it2.next());
	  if (current->startItem() == currentEdge->startItem() &&
	      current->endItem() == currentEdge->endItem()) {
	    int newHeight = currentEdge->getHeight() + 60;
	    currentEdge->setHeight(newHeight);
	  }
	} else if (undirected) {
	  UndirectedEdge *current = qgraphicsitem_cast<UndirectedEdge*>(it2.next());
	  if (current->startItem() == currentEdge->startItem() &&
	      current->endItem() == currentEdge->endItem()) {
	    int newHeight = currentEdge->getHeight() + 60;
	    currentEdge->setHeight(newHeight);
	  }
	} else {
	  it2.next();
	}
	}*/
      currentEdge->setColor(color);
      currentEdge->show();
      currentEdge->startItem()->show();
      currentEdge->endItem()->show();
      if (labelsShown) {
	currentEdge->startItem()->getLabel()->show();
	currentEdge->endItem()->getLabel()->show();
      } else {
	currentEdge->startItem()->getLabel()->hide();
	currentEdge->endItem()->getLabel()->hide();
      }
      scene->addItem(currentEdge);
    }
  }
  processHeights();
}

void NetworkGraphWidget::plotUndirectedEdges(QString type, QColor color) {
  QVectorIterator<UndirectedEdge*> it(undirectedVector);
  while (it.hasNext()) {
    UndirectedEdge* currentEdge = it.next();
    if (currentEdge->getType() == type) {
      QListIterator<QGraphicsItem*> it2(scene->items());
      while (it2.hasNext()) {
	DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(it2.peekNext());
	UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(it2.peekNext());
	if (directed) {
	  DirectedEdge *current = qgraphicsitem_cast<DirectedEdge*>(it2.next());
	  if (current->startItem() == currentEdge->startItem() &&
	      current->endItem() == currentEdge->endItem()) {
	    int newHeight = currentEdge->getHeight() + 60;
	    currentEdge->setHeight(newHeight);
	  }
	} else if (undirected) {
	  UndirectedEdge *current = qgraphicsitem_cast<UndirectedEdge*>(it2.next());
	  if (current->startItem() == currentEdge->startItem() &&
	      current->endItem() == currentEdge->endItem()) {
	    int newHeight = currentEdge->getHeight() + 60;
	    currentEdge->setHeight(newHeight);
	  }
	} else {
	  it2.next();
	}
      }
      currentEdge->setColor(color);
      currentEdge->show();
      currentEdge->startItem()->show();
      currentEdge->endItem()->show();
      if (labelsShown) {
	currentEdge->startItem()->getLabel()->show();
	currentEdge->endItem()->getLabel()->show();
      } else {
	currentEdge->startItem()->getLabel()->hide();
	currentEdge->endItem()->getLabel()->hide();
      }
      scene->addItem(currentEdge);
    }
  }
}

void NetworkGraphWidget::simpleLayout() {
  QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode* currentNode = it.next();
    qreal x = (qrand() % 5000) - 2500;
    qreal y = (qrand() % 5000) - 2500;
    currentNode->setPos(x, y);
  }
  for(int i = 0; i != 7; i++) {
    QListIterator<QGraphicsItem*> it(scene->items());
    while (it.hasNext()) {
      DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(it.peekNext());
      UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(it.peekNext());
      if (directed) {
	DirectedEdge *currentEdge = qgraphicsitem_cast<DirectedEdge*>(it.next());
	NetworkNode *currentSource = currentEdge->startItem();
	NetworkNode *currentTarget = currentEdge->endItem();
	qreal dist = qSqrt(qPow(currentSource->pos().x() -
				currentTarget->pos().x(), 2) +
			   qPow(currentSource->pos().y() -
				currentTarget->pos().y(), 2));
	if (dist > 50) {
 	  QPointF sourcePos = currentSource->scenePos();
	  QPointF targetPos = currentTarget->scenePos();
	  qreal mX = (sourcePos.x() + targetPos.x()) / 2;
	  qreal mY = (sourcePos.y() + targetPos.y()) / 2;
	  QPointF midPoint = QPointF(mX, mY);
	  qreal mX2 = (sourcePos.x() + midPoint.x()) / 2;
	  qreal mY2 = (sourcePos.y() + midPoint.y()) / 2;
	  QPointF sourcePoint = QPoint(mX2, mY2);
	  qreal mX3 = (targetPos.x() + midPoint.x()) / 2;
	  qreal mY3 = (targetPos.y() + midPoint.y()) / 2;
	  QPointF targetPoint = QPoint(mX3, mY3);
	  currentSource->setPos(sourcePoint);
	  currentSource->getLabel()->setNewPos(currentSource->scenePos());
	  currentTarget->setPos(targetPoint);
	  currentTarget->getLabel()->setNewPos(currentTarget->scenePos());
	}
	QVectorIterator<NetworkNode*> it(nodeVector);
	while (it.hasNext()) {
	  NetworkNode *first = it.next();
	  QVectorIterator<NetworkNode*> it2(nodeVector);
	  while (it2.hasNext()) {
	    NetworkNode *second = it2.next();
	    dist = qSqrt(qPow(first->pos().x() -
			      second->pos().x(), 2) +
			 qPow(first->pos().y() -
			      second->pos().y(), 2));
	    if (dist < 80) {
	      QPointF firstPoint = first->scenePos();
	      QPointF secondPoint = second->scenePos();
	      qreal mX = (firstPoint.x() + secondPoint.x()) / 2;
	      qreal mY = (firstPoint.y() + secondPoint.y()) / 2;
	      QPointF midPoint = QPointF(mX, mY);
	      qreal firstXDiff = firstPoint.x() - midPoint.x();
	      qreal firstYDiff = firstPoint.y() - midPoint.y();
	      first->setPos(first->scenePos().x() + firstXDiff,
			    first->scenePos().y() + firstYDiff);
	      first->getLabel()->setNewPos(first->scenePos());
	      qreal secondXDiff = secondPoint.x() - midPoint.x();
	      qreal secondYDiff = secondPoint.y() - midPoint.y();
	      second->setPos(second->scenePos().x() + secondXDiff,
			     second->scenePos().y() + secondYDiff);
	      second->getLabel()->setNewPos(first->scenePos());
	    }
	  }
	}
      } else if (undirected) {
	UndirectedEdge *currentEdge = qgraphicsitem_cast<UndirectedEdge*>(it.next());
	NetworkNode *currentSource = currentEdge->startItem();
	NetworkNode *currentTarget = currentEdge->endItem();
	qreal dist = qSqrt(qPow(currentSource->pos().x() -
				currentTarget->pos().x(), 2) +
			   qPow(currentSource->pos().y() -
				currentTarget->pos().y(), 2));
	if (dist > 50) {
	  QPointF sourcePos = currentSource->scenePos();
	  QPointF targetPos = currentTarget->scenePos();
	  qreal mX = (sourcePos.x() + targetPos.x()) / 2;
	  qreal mY = (sourcePos.y() + targetPos.y()) / 2;
	  QPointF midPoint = QPointF(mX, mY);
	  qreal mX2 = (sourcePos.x() + midPoint.x()) / 2;
	  qreal mY2 = (sourcePos.y() + midPoint.y()) / 2;
	  QPointF sourcePoint = QPoint(mX2, mY2);
	  qreal mX3 = (targetPos.x() + midPoint.x()) / 2;
	  qreal mY3 = (targetPos.y() + midPoint.y()) / 2;
	  QPointF targetPoint = QPoint(mX3, mY3);
	  currentSource->setPos(sourcePoint);
	  currentSource->getLabel()->setNewPos(currentSource->scenePos());
	  currentTarget->setPos(targetPoint);
	  currentTarget->getLabel()->setNewPos(currentTarget->scenePos());
	}
	QVectorIterator<NetworkNode*> it(nodeVector);
	while (it.hasNext()) {
	  NetworkNode *first = it.next();
	  QVectorIterator<NetworkNode*> it2(nodeVector);
	  while (it2.hasNext()) {
	    NetworkNode *second = it2.next();
	    dist = qSqrt(qPow(first->pos().x() -
			      second->pos().x(), 2) +
			 qPow(first->pos().y() -
			      second->pos().y(), 2));
	    if (dist < 80) {
	      QPointF firstPoint = first->scenePos();
	      QPointF secondPoint = second->scenePos();
	      qreal mX = (firstPoint.x() + secondPoint.x()) / 2;
	      qreal mY = (firstPoint.y() + secondPoint.y()) / 2;
	      QPointF midPoint = QPointF(mX, mY);
	      qreal firstXDiff = firstPoint.x() - midPoint.x();
	      qreal firstYDiff = firstPoint.y() - midPoint.y();
	      first->setPos(first->scenePos().x() + firstXDiff,
			    first->scenePos().y() + firstYDiff);
	      first->getLabel()->setNewPos(first->scenePos());
	      qreal secondXDiff = secondPoint.x() - midPoint.x();
	      qreal secondYDiff = secondPoint.y() - midPoint.y();
	      second->setPos(second->scenePos().x() + secondXDiff,
			     second->scenePos().y() + secondYDiff);
	      second->getLabel()->setNewPos(first->scenePos());
	    }
	  }
	}
      } else {
	it.next();
      }
    }
  }
  view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void NetworkGraphWidget::circularLayout() {
  const qreal Pi = 3.14;
  QVector<NetworkNode*> visible;
  QVectorIterator<NetworkNode*> it(nodeVector);
  while(it.hasNext()) {
    NetworkNode *current = it.next();
    if (current->isVisible()) {
      visible.push_back(current);
    }
  }
  std::sort(visible.begin(), visible.end(), modeSort);
  QVectorIterator<NetworkNode*> it2(visible);  
  int count = 1;
  while (it2.hasNext()) {
    NetworkNode *current = it2.next();
    if (current->isVisible()) {
      qreal x = 100 * cos(count * 2 * Pi / visible.size());
      qreal y = 100 * sin(count * 2 * Pi / visible.size());
      current->setPos(QPointF(x, y));
      current->getLabel()->setNewPos(current->scenePos());
      count++;
    }
  }
  NetworkNode *first = visible[0];
  NetworkNode *second = visible[1];
  qreal dist = qSqrt(qPow(first->scenePos().x() -
			  second->scenePos().x(), 2) +
		     qPow(first->scenePos().y() -
			  second->scenePos().y(), 2));
  while (dist < 50) {
    expandLayout();
    dist = qSqrt(qPow(first->scenePos().x() -
		      second->scenePos().x(), 2) +
		 qPow(first->scenePos().y() -
		      second->scenePos().y(), 2));
  }
}

/*
  The idea behind this function below was inspired on the code for expanding
  layouts in Gephi.
*/
void NetworkGraphWidget::expandLayout() {
  QPointF virtualCenter = QPointF(0,0);
  int total = 0;
  QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode *current = it.next();
    virtualCenter += current->scenePos();
    total++;
  }
  virtualCenter /= total;
  QVectorIterator<NetworkNode*> it2(nodeVector);
  while (it2.hasNext()) {
    NetworkNode *current = it2.next();
    QPointF currentPoint = current->scenePos();
    qreal diffX = (currentPoint.x() - virtualCenter.x()) * 1.1;
    qreal diffY = (currentPoint.y() - virtualCenter.y()) * 1.1;
    current->setPos(virtualCenter.x() + diffX, virtualCenter.y() + diffY);
    current->getLabel()->setNewPos(current->scenePos());
  }
}

/*
  The idea behind this function below was inspired on the code for contracting
  layouts in Gephi.
*/
void NetworkGraphWidget::contractLayout() {
  QPointF virtualCenter = QPointF(0,0);
  int total = 0;
  QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode *current = it.next();
    virtualCenter += current->scenePos();
    total++;
  }
  virtualCenter /= total;
  QVectorIterator<NetworkNode*> it2(nodeVector);
  while (it2.hasNext()) {
    NetworkNode *current = it2.next();
    QPointF currentPoint = current->scenePos();
    qreal diffX = (currentPoint.x() - virtualCenter.x()) * 0.9;
    qreal diffY = (currentPoint.y() - virtualCenter.y()) * 0.9;
    current->setPos(virtualCenter.x() + diffX, virtualCenter.y() + diffY);
    current->getLabel()->setNewPos(current->scenePos());
  }
}

void NetworkGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) {
  massMove = true;
  if (currentData.size() > 0) {
    QGraphicsItem *source = NULL;
    QVectorIterator<NetworkNode*> it(currentData);
    while (it.hasNext()) {
      NetworkNode *temp = it.next();
      if (temp == item) {
	source = temp;
      }
    }
    if (source != NULL) {
      qreal currentY = source->scenePos().y();
      qreal currentX = source->scenePos().x();
      qreal newY = pos.y();
      qreal newX = pos.x();
      qreal yDiff = newY - currentY;
      qreal xDiff = newX - currentX;

      QVectorIterator<NetworkNode*> it2(currentData);
      while (it2.hasNext()) {
	NetworkNode *current = it2.next();
	current->setPos(current->scenePos().x() + xDiff, current->scenePos().y() + yDiff);
	current->getLabel()->setNewPos(current->scenePos());
      }
    }
    QVectorIterator<NetworkNode*> it3(nodeVector);
    while (it3.hasNext()) {
      NetworkNode *first = it3.next();
      if (first->isSelected()) {
	QVectorIterator<NetworkNode*> it4(nodeVector);
	while (it4.hasNext()) {
	  NetworkNode *second = it4.next();
	  if (!second->isSelected()) {
	    qreal dist = qSqrt(qPow(first->scenePos().x() -
				    second->scenePos().x(), 2) +
			       qPow(first->scenePos().y() -
				    second->scenePos().y(), 2));
	    if (dist <= 40) {
	      QPointF firstPoint = first->scenePos();
	      QPointF secondPoint = second->scenePos();
	      qreal mX = (firstPoint.x() + secondPoint.x()) / 2;
	      qreal mY = (firstPoint.y() + secondPoint.y()) / 2;
	      QPointF midPoint = QPointF(mX, mY);
	      qreal secondXDiff = secondPoint.x() - midPoint.x();
	      qreal secondYDiff = secondPoint.y() - midPoint.y();
	      qreal xDiff = 0;
	      qreal yDiff = 0;
	      if (secondXDiff > 0) {
		xDiff = 5;
	      } else if (secondXDiff < 0) {
		xDiff = -5;
	      }
	      if (secondYDiff > 0) {
		yDiff = 5;
	      } else if (secondYDiff < 0) {
		yDiff = -5;
	      }
	      QPointF temp = QPointF(second->scenePos().x() + xDiff,
				     second->scenePos().y() + yDiff);
	      second->move(temp);
	    }
	  }
	}
      }
    }
  }
}

void NetworkGraphWidget::colorByAttribute() {
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, ENTITY);
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) {
    setChangeLabel();
    QColor color = attributeColorDialog->getColor();
    QColor textColor = attributeColorDialog->getTextColor();
    QString attribute = attributeColorDialog->getAttribute();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entity_attributes "
		   "WHERE name = :name");
    query->bindValue(":name", attribute);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QVector<QString> attributeVector;
    attributeVector.push_back(attribute);
    findChildren(attribute, &attributeVector);
    QVectorIterator<QString> it(attributeVector);
    while (it.hasNext()) {
      QString currentAttribute = it.next();
      query->prepare("SELECT entity FROM attributes_to_entities "
		     "WHERE attribute = :currentAttribute");
      query->bindValue(":currentAttribute", currentAttribute);
      query->exec();
      while (query->next()) {
	QString currentName = query->value(0).toString();
	QVectorIterator<NetworkNode*> it2(nodeVector);
	while (it2.hasNext()) {
	  NetworkNode* currentNode = it2.next();
	  if (currentNode->getName() == currentName) {
	    currentNode->setColor(color);
	    currentNode->setMode(attribute);
	    currentNode->getLabel()->setDefaultTextColor(textColor);
	  }
	}
      }
    }
    bool found = false;
    for (int i = 0; i < nodeListWidget->rowCount(); i++) {
      if (nodeListWidget->item(i, 0)->data(Qt::DisplayRole) == attribute) {
	found = true;
	QTableWidgetItem *item = nodeListWidget->item(i,0);
	QString toolTip = breakString(attribute + " - " + description);
	item->setToolTip(toolTip);
	nodeListWidget->item(i, 1)->setBackground(color);
	break;
      }
    }
    if (!found) {
      QTableWidgetItem *item = new QTableWidgetItem(attribute);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
      QString toolTip = breakString(attribute + " - " + description);
      item->setToolTip(toolTip);
      item->setData(Qt::DisplayRole, attribute);
      nodeListWidget->setRowCount(nodeListWidget->rowCount() + 1);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 0, item);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setBackground(color);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->
	setFlags(nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->flags() ^
		 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    }
    delete query;
  }
  delete attributeColorDialog;
}

void NetworkGraphWidget::findChildren(QString father, QVector<QString> *children) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM entity_attributes WHERE father = :father");
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) {
    QString currentChild = query->value(0).toString();
    children->push_back(currentChild);
    findChildren(currentChild, children);
  }
  delete query;
}

void NetworkGraphWidget::multimodeTransformation() {
  QVector<QString> modeVector;
  for (int i = 0; i != nodeListWidget->rowCount(); i++) {
    QString currentMode = nodeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    bool found = false;
    QVectorIterator<NetworkNode*> mit(nodeVector);
    while (mit.hasNext()) {
      NetworkNode* currentNode = mit.next();
      if (currentNode->isVisible() && currentNode->getMode() == currentMode) {
	found = true;
      }
    }
    if (found) {
      modeVector.push_back(currentMode);
    }
  }
  QPointer<MultimodeDialog> multimodeDialog = new MultimodeDialog(this, &directedVector,
								  &undirectedVector);
  multimodeDialog->setModes(modeVector);
  multimodeDialog->exec();
  if (multimodeDialog->getExitStatus() == 0) {
    QString modeOne = multimodeDialog->getModeOne();
    QString modeTwo = multimodeDialog->getModeTwo();
    QString relationshipOne = multimodeDialog->getRelationshipOne(); 
    QString relationshipTwo = multimodeDialog->getRelationshipTwo(); 
    QString name = multimodeDialog->getName();
    QString directedness = multimodeDialog->getDirectedness();
    for (int i = 0; i != edgeListWidget->rowCount(); i++) {
      QString currentRel = edgeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
      if (currentRel ==  name) {
	QPointer <QMessageBox> warningBox = new QMessageBox(this);
	warningBox->addButton(QMessageBox::Ok);
	warningBox->setIcon(QMessageBox::Warning);
	warningBox->setText("Name already exists.");
	warningBox->setInformativeText("You already have a relationship type with that name.");
	warningBox->exec();
	delete warningBox;
	return;
      }
    }
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    QColor color = QColor(Qt::black);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    if (colorDialog->exec()) {
      color = colorDialog->selectedColor();
    } else {
      delete colorDialog;
      return;
    }
    delete colorDialog;    
    QString description = multimodeDialog->getDescription();

    QVectorIterator<DirectedEdge*> it(directedVector);
    while (it.hasNext()) {
      DirectedEdge *first = it.next();
      if (first->getType() == relationshipOne) {
	if (first->startItem()->getMode() == modeOne &&
	    first->endItem()->getMode() == modeTwo) {
	  QVectorIterator<DirectedEdge*> it2(directedVector);
	  while (it2.hasNext()) {
	    DirectedEdge *second = it2.next();
	    if (second->getType() == relationshipTwo) {
	      if (second->endItem() == first->endItem() &&
		  second->startItem() != first->startItem() &&
		  second->startItem()->getMode() == modeOne &&
		  second->endItem()->getMode() == modeTwo &&
		  first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		if (directedness == DIRECTED) {
		  DirectedEdge *newEdge = new DirectedEdge(first->startItem(), second->startItem(),
						       name, CREATED);
		  bool found = false;
		  QVectorIterator<DirectedEdge*> it3(directedVector);
		  while (it3.hasNext()) {
		    DirectedEdge* alter = it3.next();
		    if (alter->getType() == name) {
		      if (alter->startItem() == newEdge->startItem() &&
			  alter->endItem() == newEdge->endItem()) { 
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    directedVector.push_back(newEdge);
		  }
		} else if (directedness == UNDIRECTED) {
		  UndirectedEdge *newEdge;
		  if (first->startItem()->getName() < second->startItem()->getName()) {
		    newEdge = new UndirectedEdge(first->startItem(),
						 second->startItem(),
						 name, CREATED);
		  } else {
		    newEdge = new UndirectedEdge(second->startItem(),
						 first->startItem(),
						 name, CREATED);
		  }
		  bool found = false;
		  QVectorIterator<UndirectedEdge*> it3(undirectedVector);
		  while (it3.hasNext()) {
		    UndirectedEdge* alter = it3.next();
		    if (alter->getType() == name) {
		      if ((alter->startItem() == newEdge->startItem() &&
			   alter->endItem() == newEdge->endItem()) ||
			  (alter->startItem() == newEdge->endItem() &&
			   alter->endItem() == newEdge->startItem())) {
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    undirectedVector.push_back(newEdge);
		  }
		}
	      }
	    }
	  }
	} else if (first->startItem()->getMode() == modeTwo &&
		    first->endItem()->getMode() == modeOne) {
	  QVectorIterator<DirectedEdge*> it2(directedVector);
	  while (it2.hasNext()) {
	    DirectedEdge *second = it2.next();
	    if (second->getType() == relationshipTwo) {
	      if (second->startItem() == first->startItem() &&
		  second->endItem() != first->endItem() &&
		  second->endItem()->getMode() == modeOne &&
		  second->startItem()->getMode() == modeTwo &&
		  first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		if (directedness == DIRECTED) {
		  DirectedEdge *newEdge = new DirectedEdge(first->endItem(), second->endItem(),
						   name, CREATED);
		  bool found = false;
		  QVectorIterator<DirectedEdge*> it3(directedVector);
		  while (it3.hasNext()) {
		    DirectedEdge* alter = it3.next();
		    if (alter->getType() == name) {
		      if (alter->startItem() == newEdge->startItem() &&
			  alter->endItem() == newEdge->endItem()) { 
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    directedVector.push_back(newEdge);
		  }
		} else if (directedness == UNDIRECTED) {
		  UndirectedEdge *newEdge;
		  if (first->endItem()->getName() < second->endItem()->getName()) {
		    newEdge = new UndirectedEdge(first->endItem(),
						 second->endItem(),
						 name, CREATED);
		  } else {
		    newEdge = new UndirectedEdge(second->endItem(),
						 first->endItem(),
						 name, CREATED);
		  }
		  bool found = false;
		  QVectorIterator<UndirectedEdge*> it3(undirectedVector);
		  while (it3.hasNext()) {
		    UndirectedEdge* alter = it3.next();
		    if (alter->getType() == name) {
		      if ((alter->startItem() == newEdge->startItem() &&
			   alter->endItem() == newEdge->endItem()) ||
			  (alter->startItem() == newEdge->endItem() &&
			   alter->endItem() == newEdge->startItem())) {
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    undirectedVector.push_back(newEdge);
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    QVectorIterator<UndirectedEdge*> it3(undirectedVector);
    while (it3.hasNext()) {
      UndirectedEdge *first = it3.next();
      if (first->getType() == relationshipOne) {
	if (first->startItem()->getMode() == modeOne &&
	    first->endItem()->getMode() == modeTwo) {
	  QVectorIterator<UndirectedEdge*> it4(undirectedVector);
	  while (it4.hasNext()) {
	    UndirectedEdge *second = it4.next();
	    if (second->getType() == relationshipTwo) { 
	      if (second->endItem() == first->endItem() &&
		  second->startItem() != first->startItem() &&
		  second->startItem()->getMode() == modeOne &&
		  second->endItem()->getMode() == modeTwo &&
		  first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		if (directedness == DIRECTED) {
		  DirectedEdge *newEdge = new DirectedEdge(first->startItem(), second->startItem(),
						   name, CREATED);
		  bool found = false;
		  QVectorIterator<DirectedEdge*> it3(directedVector);
		  while (it3.hasNext()) {
		    DirectedEdge* alter = it3.next();
		    if (alter->getType() == name) {
		      if (alter->startItem() == newEdge->startItem() &&
			  alter->endItem() == newEdge->endItem()) { 
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    directedVector.push_back(newEdge);
		  }
		} else if (directedness == UNDIRECTED) {
		  UndirectedEdge *newEdge;
		  if (first->startItem()->getName() < second->startItem()->getName()) {
		    newEdge = new UndirectedEdge(first->startItem(),
						 second->startItem(),
						 name, CREATED);
		  } else {
		    newEdge = new UndirectedEdge(second->startItem(),
						 first->startItem(),
						 name, CREATED);
		  }
		  QVectorIterator<UndirectedEdge*> it3(undirectedVector);
		  bool found = false;
		  while (it3.hasNext()) {
		    UndirectedEdge *alter = it3.next();
		    if (alter->getType() == name) {
		      if ((alter->startItem() == newEdge->startItem() &&
			   alter->endItem() == newEdge->endItem()) ||
			  (alter->startItem() == newEdge->endItem() &&
			   alter->endItem() == newEdge->startItem())) {
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    undirectedVector.push_back(newEdge);
		  }
		}
	      }
	    }
	  }
	} else if (first->startItem()->getMode() == modeTwo &&
		   first->endItem()->getMode() == modeOne) {
	  QVectorIterator<UndirectedEdge*> it4(undirectedVector);
	  while (it4.hasNext()) {
	    UndirectedEdge *second = it4.next();
	    if (second->getType() == relationshipTwo) {
	      if (second->endItem() == first->endItem() &&
		  second->startItem() != first->startItem() &&
		  second->endItem()->getMode() == modeOne &&
		  second->startItem()->getMode() == modeTwo &&
		first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		if (directedness == DIRECTED) {
		  DirectedEdge *newEdge = new DirectedEdge(first->endItem(), second->endItem(),
						   name, CREATED);
		  bool found = false;
		  QVectorIterator<DirectedEdge*> it3(directedVector);
		  while (it3.hasNext()) {
		    DirectedEdge* alter = it3.next();
		    if (alter->getType() == name) {
		      if (alter->startItem() == newEdge->startItem() &&
			  alter->endItem() == newEdge->endItem()) { 
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    directedVector.push_back(newEdge);
		  }
		} else if (directedness == UNDIRECTED) {
		  UndirectedEdge *newEdge;
		  if (first->endItem()->getName() < second->endItem()->getName()) {
		    newEdge = new UndirectedEdge(first->endItem(),
						 second->endItem(),
						 name, CREATED);
		  } else {
		    newEdge = new UndirectedEdge(second->endItem(),
						 first->endItem(),
						 name, CREATED);
		  }
		  bool found = false;
		  QVectorIterator<UndirectedEdge*> it3(undirectedVector);
		  while (it3.hasNext()) {
		    UndirectedEdge* alter = it3.next();
		    if (alter->getType() == name) {
		      if ((alter->startItem() == newEdge->startItem() &&
			   alter->endItem() == newEdge->endItem()) ||
			  (alter->startItem() == newEdge->endItem() &&
			   alter->endItem() == newEdge->startItem())) {
			found = true;
		      }
		    }
		  }
		  if (!found) {
		    undirectedVector.push_back(newEdge);
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    presentTypes.push_back(name);
    plotDirectedEdges(name, color);
    plotUndirectedEdges(name, color);
    QTableWidgetItem *item = new QTableWidgetItem(selectedType);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    QString toolTip = breakString(name + " (Undirected) - " + description);
    item->setToolTip(toolTip);
    item->setData(Qt::DisplayRole, name);
    edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
      setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags()
	       ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    setVisibility();
  }
}

void NetworkGraphWidget::removeMode() {
  setChangeLabel();
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode *current = it.next();
    if (current->getMode() == text) {
      current->setColor(Qt::black);
      current->setMode("");
    }
  }
  for (int i = 0; i != nodeListWidget->rowCount();) {
    if (nodeListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text) {
      nodeListWidget->removeRow(i);
    }
    if (i != nodeListWidget->rowCount()) {
      i++;
    }
  }
}

void NetworkGraphWidget::setModeButtons(QTableWidgetItem *item) {
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") {
    removeModeButton->setEnabled(true);
  } else {
    removeModeButton->setEnabled(false);
  }
  if (text != nodeListWidget->item(0, 0)->data(Qt::DisplayRole).toString()) {
    moveModeUpButton->setEnabled(true);
  } else {
    moveModeUpButton->setEnabled(false);
  }
  if (text != nodeListWidget->item(nodeListWidget->rowCount() - 1, 0)
      ->data(Qt::DisplayRole).toString()) {
    moveModeDownButton->setEnabled(true);
  } else {
    moveModeDownButton->setEnabled(false);
  }
}

void NetworkGraphWidget::disableModeButtons() {
  removeModeButton->setEnabled(false);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton->setEnabled(false);
}

void NetworkGraphWidget::restoreModeColors() {
  for (int i = 0; i != nodeListWidget->rowCount(); i++) {
    QString currentMode = nodeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
    QColor color = nodeListWidget->item(i, 1)->background().color();
    QVector<QString> attributeVector;
    attributeVector.push_back(currentMode);
    findChildren(currentMode, &attributeVector);
    QVectorIterator<QString> it3(attributeVector);
    while (it3.hasNext()) {
      QString currentAttribute = it3.next();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT entity FROM attributes_to_entities "
		     "WHERE attribute = :currentAttribute");
      query->bindValue(":currentAttribute", currentAttribute);
      query->exec();
      while (query->next()) {
	QString entity = query->value(0).toString();
	QVectorIterator<NetworkNode*> it4(nodeVector);
	while (it4.hasNext()) {
	  NetworkNode* currentEntity = it4.next();
	  if (currentEntity->getName() == entity) {
	    currentEntity->setColor(color);
	    currentEntity->setMode(currentMode);
	  }
	}
      }
      delete query;
    }
  }
  for (int i = 0; i < nodeListWidget->rowCount(); i++) {
    QString mode = nodeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    QColor color = nodeListWidget->item(i, 1)->background().color();
    QVectorIterator<NetworkNode*> it(nodeVector);
    while (it.hasNext()) {
      NetworkNode *node = it.next();
      if (node->getMode() == mode) {
	node->setColor(color);
      }
    }
  }
}

void NetworkGraphWidget::moveModeUp() {
  setChangeLabel();
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != nodeListWidget->item(0,0)->data(Qt::DisplayRole).toString()) {
    int currentRow = nodeListWidget->row(nodeListWidget->currentItem());
    QTableWidgetItem *currentItem = nodeListWidget->takeItem(currentRow,0);
    QColor currentColor = nodeListWidget->item(currentRow, 1)->background().color();
    int newRow = currentRow - 1;
    QTableWidgetItem *otherItem = nodeListWidget->takeItem(newRow, 0);
    QColor otherColor = nodeListWidget->item(newRow, 1)->background().color();
    nodeListWidget->setItem(newRow, 0, currentItem);
    nodeListWidget->item(newRow, 1)->setBackground(currentColor);
    nodeListWidget->setItem(currentRow, 0, otherItem);
    nodeListWidget->item(currentRow, 1)->setBackground(otherColor);
    restoreModeColors();
  }
}

void NetworkGraphWidget::moveModeDown() {
  setChangeLabel();
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != nodeListWidget->item(nodeListWidget->rowCount() - 1, 0)->
      data(Qt::DisplayRole).toString()) {
    int currentRow = nodeListWidget->row(nodeListWidget->currentItem());
    QTableWidgetItem *currentItem = nodeListWidget->takeItem(currentRow, 0);
    QColor currentColor = nodeListWidget->item(currentRow, 1)->background().color();
    int newRow = currentRow + 1;
    QTableWidgetItem *otherItem = nodeListWidget->takeItem(newRow, 0);
    QColor otherColor = nodeListWidget->item(newRow, 1)->background().color();;
    nodeListWidget->setItem(newRow, 0, currentItem);
    nodeListWidget->item(newRow, 1)->setBackground(currentColor);
    nodeListWidget->setItem(currentRow, 0, otherItem);
    nodeListWidget->item(currentRow, 1)->setBackground(otherColor);
    restoreModeColors();
  }
}


void NetworkGraphWidget::mergeRelationships() {
  QVector<QString> relVector;
  QVector<QString> directednessVector;
  for (int i = 0; i != edgeListWidget->rowCount(); i++) {
    QString currentType = edgeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    bool found = false;
    QVectorIterator<DirectedEdge*> rit(directedVector);
    QString directedness = "";
    while (rit.hasNext()) {
      DirectedEdge* directed = rit.next();
      if (directed->isVisible() && directed->getType() == currentType) {
	found = true;
	directedness = DIRECTED;
      }
    }
    QVectorIterator<UndirectedEdge*> rit2(undirectedVector);
    while (rit2.hasNext()) {
      UndirectedEdge* undirected = rit2.next();
      if (undirected->isVisible() && undirected->getType() == currentType) {
	found = true;
	directedness = UNDIRECTED;
      }
    }
    if (found) {
      relVector.push_back(currentType);
      directednessVector.push_back(directedness);
    }
  }
  QPointer<MergeRelationshipsDialog> mergeRelationshipsDialog =
    new MergeRelationshipsDialog(this, &directedVector, &undirectedVector,
				 relVector, directednessVector);
  mergeRelationshipsDialog->exec();
  if (mergeRelationshipsDialog->getExitStatus() == 0) {
    QVector<QString> types = mergeRelationshipsDialog->getTypes();
    QString name = mergeRelationshipsDialog->getName();
    QString description = mergeRelationshipsDialog->getDescription();
    QString directedness = mergeRelationshipsDialog->getDirectedness();
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    QColor color = QColor(Qt::black);
    if (colorDialog->exec()) {
      color = colorDialog->selectedColor();
    } else {
      delete colorDialog;
      return;
    }
    delete colorDialog;
    if (directedness == DIRECTED) {
      QVectorIterator<DirectedEdge*> it(directedVector);
      while (it.hasNext()) {
	DirectedEdge* directed = it.next();
	if (types.contains(directed->getType())) {
	  DirectedEdge *newDirected = new DirectedEdge(directed->startItem(), directed->endItem(),
						       name, CREATED);
	  newDirected->setColor(color);
	  directedVector.push_back(newDirected);
	  scene->removeItem(directed);
	  scene->addItem(newDirected);
	}
      }
      QVectorIterator<DirectedEdge*> it2(directedVector);
      while (it2.hasNext()) {
	DirectedEdge *first = it2.next();
	bool found = false;
	QVectorIterator<DirectedEdge*> it3(directedVector);
	while (it3.hasNext()) {
	  DirectedEdge *second = it3.next();
	  if (first != second) {
	    if (first->startItem() == second->startItem() &&
		first->endItem() == second->endItem() &&
		first->getName() == CREATED &&
		second->getName() == CREATED) {
	      found = true;
	    }
	  }
	}
	if (found) {
	  scene->removeItem(first);
	  directedVector.removeOne(first);
	}
      }
    } else if (directedness == UNDIRECTED) {
      QVectorIterator<UndirectedEdge*> it(undirectedVector);
      while (it.hasNext()) {
	UndirectedEdge *undirected = it.next();
	if (types.contains(undirected->getType())) {
	  UndirectedEdge *newUndirected = new UndirectedEdge(undirected->startItem(),
							     undirected->endItem(),
							     name, CREATED);
	  newUndirected->setColor(color);
	  undirectedVector.push_back(newUndirected);
	  scene->removeItem(undirected);
	  scene->addItem(newUndirected);
	}
      }
      QVectorIterator<UndirectedEdge*> it2(undirectedVector);
      while (it2.hasNext()) {
	UndirectedEdge *first = it2.next();
	bool found = false;
	QVectorIterator<UndirectedEdge*> it3(undirectedVector);
	while (it3.hasNext()) {
	  UndirectedEdge *second = it3.next();
	  if (first != second) {
	    if (first->startItem() == second->startItem() &&
		first->endItem() == second->endItem() &&
		first->getName() == CREATED &&
		second->getName() == CREATED) {
	      found = true;
	    }
	  }
	}
	if (found) {
	  scene->removeItem(first);
	  undirectedVector.removeOne(first);
	}
      }
    }
    QVectorIterator<QString> rit(types);
    while (rit.hasNext()) {
      presentTypes.removeOne(rit.next());
    }
    presentTypes.push_back(name);
    QTableWidgetItem *item = new QTableWidgetItem(name);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    QString toolTip = breakString(name + " (" + directedness + ") - " + description);
    item->setToolTip(toolTip);
    item->setData(Qt::DisplayRole, name);
    edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
      setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags()
	       ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    for (int i = 0; i != edgeListWidget->rowCount();) {
      while (types.contains(edgeListWidget->item(i,0)->data(Qt::DisplayRole).toString())) {
	edgeListWidget->removeRow(i);
      }
      if (i != edgeListWidget->rowCount()) {
	i++;
      }
    }
    setVisibility();
  }
}

void NetworkGraphWidget::setFilterButtons(QTableWidgetItem *item) {
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") {
    setFilteredButton->setEnabled(true);
    unsetFilteredButton->setEnabled(true);
    hideTypeButton->setEnabled(true);
    showTypeButton->setEnabled(true);
    removeTypeButton->setEnabled(true);
  } else {
    setFilteredButton->setEnabled(false);
    unsetFilteredButton->setEnabled(false);
    hideTypeButton->setEnabled(false);
    showTypeButton->setEnabled(false);
    removeTypeButton->setEnabled(false);
    return;
  }
  QBrush brush = item->foreground();
  QVectorIterator<DirectedEdge*> it(directedVector);
  bool filtered = false;
  bool hidden = false;
  while (it.hasNext()) {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type) {
      filtered = current->isFiltered();
      hidden = current->isMassHidden();
      break;
    }
  }
  if (filtered) {
    setFilteredButton->setChecked(true);
    unsetFilteredButton->setChecked(false);
  } else {
    setFilteredButton->setChecked(false);
    unsetFilteredButton->setChecked(true);
  }
  if (hidden) {
    hideTypeButton->setChecked(true);
    showTypeButton->setChecked(false);
  } else {
    hideTypeButton->setChecked(false);
    showTypeButton->setChecked(true);
  }
  QVectorIterator<UndirectedEdge*> it2(undirectedVector);
  while (it2.hasNext()) {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type) {
      filtered = current->isFiltered();
      hidden = current->isMassHidden();
      break;
    }
  }
  if (filtered) {
    setFilteredButton->setChecked(true);
    unsetFilteredButton->setChecked(false);
  } else {
    setFilteredButton->setChecked(false);
    unsetFilteredButton->setChecked(true);
  }
  if (hidden) {
    hideTypeButton->setChecked(true);
    showTypeButton->setChecked(false);
  } else {
    hideTypeButton->setChecked(false);
    showTypeButton->setChecked(true);
  }
}

void NetworkGraphWidget::disableFilterButtons() {
  setFilteredButton->setEnabled(false);
  unsetFilteredButton->setEnabled(false);
  hideTypeButton->setEnabled(false);
  showTypeButton->setEnabled(false);
  removeTypeButton->setEnabled(false);
}

void NetworkGraphWidget::activateFilter() {
  setFilteredButton->setChecked(true);
  unsetFilteredButton->setChecked(false);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<DirectedEdge*> it(directedVector);
  while (it.hasNext()) {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type) {
      current->setFiltered(true);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(undirectedVector);
  while (it2.hasNext()) {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type) {
      current->setFiltered(true);
    }
  }
  setVisibility();
}

void NetworkGraphWidget::deactivateFilter() {
  setFilteredButton->setChecked(false);
  unsetFilteredButton->setChecked(true);  
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<DirectedEdge*> it(directedVector);
  while (it.hasNext()) {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type) {
      current->setFiltered(false);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(undirectedVector);
  while (it2.hasNext()) {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type) {
      current->setFiltered(false);
    }
  }
  setVisibility();
}

void NetworkGraphWidget::hideType() {
  hideTypeButton->setChecked(true);
  showTypeButton->setChecked(false);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  edgeListWidget->currentItem()->setBackground(Qt::gray);
  QVectorIterator<DirectedEdge*> it(directedVector);
  while (it.hasNext()) {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type) {
      current->setMassHidden(true);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(undirectedVector);
  while (it2.hasNext()) {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type) {
      current->setMassHidden(true);
    }
  }
  setVisibility();
}

void NetworkGraphWidget::showType() {
  hideTypeButton->setChecked(false);
  showTypeButton->setChecked(true);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  edgeListWidget->currentItem()->setBackground(Qt::transparent);
  QVectorIterator<DirectedEdge*> it(directedVector);
  while (it.hasNext()) {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type) {
      current->setMassHidden(false);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(undirectedVector);
  while (it2.hasNext()) {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type) {
      current->setMassHidden(false);
    }
  }
  setVisibility();
}

void NetworkGraphWidget::setNodeColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) {
    setChangeLabel();
    QColor color = colorDialog->selectedColor();
    QVectorIterator<NetworkNode*> it(nodeVector);
    while (it.hasNext()) {
      NetworkNode *currentNode = it.next();
      currentNode->setColor(color);
    }
  }
  delete colorDialog;
}

void NetworkGraphWidget::setLabelColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) {
    setChangeLabel();
    QColor color = colorDialog->selectedColor();
    QVectorIterator<NetworkNodeLabel*> it(labelVector);
    while (it.hasNext()) {
      NetworkNodeLabel *currentLabel = it.next();
      currentLabel->setDefaultTextColor(color);
    }
  }
  delete colorDialog;
}

void NetworkGraphWidget::setBackgroundColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) {
    QColor color = colorDialog->selectedColor();
    view->setBackgroundBrush(color);
  }
  delete colorDialog;
}

void NetworkGraphWidget::changeEdgeColor(QTableWidgetItem *item) {
  if (item->column() == 1) {
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    colorDialog->setCurrentColor(item->background().color());
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    if (colorDialog->exec()) {
      QColor color = colorDialog->selectedColor();
      item->setBackground(color);
      QTableWidgetItem* neighbour = edgeListWidget->item(item->row(), 0);
      QString type = neighbour->data(Qt::DisplayRole).toString();
      QVectorIterator<DirectedEdge*> it(directedVector);
      while (it.hasNext()) {
	DirectedEdge *directed = it.next();
	if (directed->getType() == type) {
	  directed->setColor(color);
	}
      }
      QVectorIterator<UndirectedEdge*> it2(undirectedVector);
      while (it2.hasNext()) {
	UndirectedEdge *undirected = it2.next();
	if (undirected->getType() == type) {
	  undirected->setColor(color);
	}
      }
    }
  }
}

void NetworkGraphWidget::changeModeColor(QTableWidgetItem *item) {
  if (item->column() == 1) {
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    colorDialog->setCurrentColor(item->background().color());
    if (colorDialog->exec()) {
      QColor color = colorDialog->selectedColor();
      item->setBackground(color);
      QTableWidgetItem* neighbour = nodeListWidget->item(item->row(), 0);
      QString mode = neighbour->data(Qt::DisplayRole).toString();
      QVectorIterator<NetworkNode*> it(nodeVector);
      while (it.hasNext()) {
	NetworkNode *current = it.next();
	if (current->getMode() == mode) {
	  current->setColor(color);
	}
      }
    }
  }
}

void NetworkGraphWidget::setPlotButton() {
  if (typeComboBox->currentText() != DEFAULT) {
    plotButton->setEnabled(true);
    if (presentTypes.size() > 0) {
      QVectorIterator<QString> it(presentTypes);
      bool found = false;
      while (it.hasNext()) {
	QString currentType = it.next();
	if (currentType == typeComboBox->currentText()) {
	  found = true;
	}
      }
      if (!found) {
	addButton->setEnabled(true);
      } else {
	addButton->setEnabled(false);
      }
    }
  } else {
    plotButton->setEnabled(false);
  }
}

void NetworkGraphWidget::plotNewGraph() {
  cleanUp();
  selectedType = typeComboBox->currentText();
  getEntities();
  plotEntities(); // Should allow for range to be set here.
  getDirectedEdges();
  getUndirectedEdges();  
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  QColor color = QColor(Qt::black);
  colorDialog->setCurrentColor(color);
  if (colorDialog->exec()) {
    color = colorDialog->selectedColor();
  } else {
    return;
  }
  delete colorDialog;
  plotDirectedEdges(selectedType, color);
  plotUndirectedEdges(selectedType, color);
  simpleLayout();
  presentTypes.push_back(selectedType);
  
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness, description FROM relationship_types "
		 "WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  QString directedness = query->value(0).toString();
  QString description = query->value(1).toString();
  QString toolTip = breakString(selectedType + " (" + directedness + ") - " + description);
  QTableWidgetItem *item = new QTableWidgetItem(selectedType);
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setToolTip(toolTip);
  item->setData(Qt::DisplayRole, selectedType);
  edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
  edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
  edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
  edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
  edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
    setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags() ^ Qt::ItemIsEditable ^
	     Qt::ItemIsSelectable);
  delete query;
  savePlotButton->setEnabled(true);
  setRangeControls();
  plotLabel->setText("Unsaved plot");
  checkCongruency();
  view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}


void NetworkGraphWidget::addRelationshipType() {
  selectedType = typeComboBox->currentText();  
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  QColor color = QColor(Qt::black);
  if (colorDialog->exec()) {
    color = colorDialog->selectedColor();
  } else {
    delete colorDialog;
    return;
  }
  delete colorDialog;
  plotDirectedEdges(selectedType, color);
  plotUndirectedEdges(selectedType, color);
  simpleLayout();
  presentTypes.push_back(selectedType);
  setRangeControls();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness, description FROM relationship_types "
		 "WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  QString directedness = query->value(0).toString();
  QString description = query->value(1).toString();
  QString toolTip = breakString(selectedType + " (" + directedness + ") - " + description);
  QTableWidgetItem *item = new QTableWidgetItem(selectedType);
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setToolTip(toolTip);
  item->setData(Qt::DisplayRole, selectedType);
  edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
  edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
  edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
  edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
  edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
    setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags() ^ Qt::ItemIsEditable ^
	     Qt::ItemIsSelectable);
  delete query;
  addButton->setEnabled(false);
  setChangeLabel();
}

void NetworkGraphWidget::removeRelationshipType() {
  setChangeLabel();
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<DirectedEdge*> it(directedVector);
  while (it.hasNext()) {
    DirectedEdge *directed = it.next();
    if (directed->getType() == text) {
      directed->hide();
      scene->removeItem(directed);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(undirectedVector);
  while (it2.hasNext()) {
    UndirectedEdge *undirected = it2.next();
    if (undirected->getType() == text) {
      undirected->hide();
      scene->removeItem(undirected);
    }
  }
  for (int i = 0; i != edgeListWidget->rowCount();) {
    if (edgeListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text) {
      edgeListWidget->removeRow(i);
    }
    if (i != edgeListWidget->rowCount()) {
      i++;
    }
  }
  presentTypes.removeOne(text);
  setVisibility();
}

void NetworkGraphWidget::toggleLabels() {
  labelsShown = !labelsShown;
  QVectorIterator<NetworkNodeLabel*> it2(labelVector);
  while (it2.hasNext()) {
    NetworkNodeLabel *currentLabel = it2.next();
    if (labelsShown) {
      if (currentLabel->getNode()->isVisible()) {
	currentLabel->show();
      } else {
	currentLabel->hide();
      }
    } else {
      currentLabel->hide();
    }
  }
}

void NetworkGraphWidget::increaseLabelSize() {
  QVectorIterator<NetworkNodeLabel*> it(labelVector);
  while (it.hasNext()) {
    NetworkNodeLabel *currentLabel = it.next();
    currentLabel->increaseFontSize();
  }
}

void NetworkGraphWidget::decreaseLabelSize() {
  QVectorIterator<NetworkNodeLabel*> it(labelVector);
  while (it.hasNext()) {
    NetworkNodeLabel *currentLabel = it.next();
    currentLabel->decreaseFontSize();
  }
}

void NetworkGraphWidget::setRangeControls() {
  lowerRangeDial->setEnabled(true);
  upperRangeDial->setEnabled(true);
  lowerRangeSpinBox->setEnabled(true);
  upperRangeSpinBox->setEnabled(true);
  lowerRangeDial->setRange(minOrder, maxOrder - 1);
  upperRangeDial->setRange(minOrder + 1, maxOrder);
  lowerRangeSpinBox->setRange(minOrder, maxOrder - 1);
  upperRangeSpinBox->setRange(minOrder + 1, maxOrder);
  lowerRangeDial->setValue(minOrder);
  lowerRangeSpinBox->setValue(minOrder);
  upperRangeDial->setValue(maxOrder);
  upperRangeSpinBox->setValue(maxOrder);
}

void NetworkGraphWidget::exportSvg() {
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

void NetworkGraphWidget::exportNodes() {
  QVector<QString> entities;
  QVector<QString> descriptions;
  QVector<QString> modes;
  
  QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode *node = it.next();
    if (node->isVisible()) {
      QString entity = node->getName();
      entities.push_back(entity);
      QString description = node->getDescription();
      descriptions.push_back(description);
      QString mode = node->getMode();
      modes.push_back(mode);
    }
  }
  
  QPointer<NodeSettingsDialog> settingsDialog = new NodeSettingsDialog(this,
								       entities,
								       descriptions,
								       modes);
  settingsDialog->exec();
  
}

void NetworkGraphWidget::exportEdges() {
 // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) {
    if(!fileName.endsWith(".csv")) {
      fileName.append(".csv");
    }
    // And we create a file outstream.  
    std::ofstream fileOut(fileName.toStdString().c_str());
    // Let us first create the file header.
    fileOut << "Source" << ","
	    << "Target" << ","
	    << "Type" <<","
	    << "Description" << ","
	    << "Comment" << "\n";
    // Then we iterate through the directed edges first.
    QVectorIterator<DirectedEdge*> it(directedVector);
    while (it.hasNext()) {
      DirectedEdge *directed = it.next();
      if (directed->isVisible()) {
	QString source = directed->startItem()->getName();
	QString target = directed->endItem()->getName();
	QString description = directed->getType();
	QString comment = directed->getComment();
	fileOut << "\"" << doubleQuote(source).toStdString() << "\"" << ","
		<< "\"" << doubleQuote(target).toStdString() << "\"" << ","
		<< "Directed" << ","
		<< "\"" << doubleQuote(description).toStdString() << "\"" << ","
		<< "\"" << doubleQuote(comment).toStdString() << "\"" << "\n";
      }
    }
    // Then we iterate through the undirected edges second.
    QVectorIterator<UndirectedEdge*> it2(undirectedVector);
    while (it2.hasNext()) {
      UndirectedEdge* undirected = it2.next();
      if (undirected->isVisible()) {
	QString source = undirected->startItem()->getName();
	QString target = undirected->endItem()->getName();
	QString description = undirected->getType();
	QString comment = undirected->getComment();
	fileOut << "\"" << doubleQuote(source).toStdString() << "\"" << ","
		<< "\"" << doubleQuote(target).toStdString() << "\"" << ","
		<< "Undirected" << ","
		<< "\"" << doubleQuote(description).toStdString() << "\"" << ","
		<< "\"" << doubleQuote(comment).toStdString() << "\"" << "\n";
      }
    }
    // And that should be it!
    fileOut.close();
  }
}

void NetworkGraphWidget::saveCurrentPlot() {
  QPointer<SimpleTextDialog> saveDialog = new SimpleTextDialog(this);
  saveDialog->setWindowTitle("Save current plot");
  saveDialog->submitText(plotLabel->text());
  saveDialog->setLabel("Plot name:");
  saveDialog->exec();
  if (saveDialog->getExitStatus() == 0) {
    QString name = saveDialog->getText();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT plot FROM saved_ng_plots WHERE plot = :name");
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
      // saved_ng_plots_event_items
      query->prepare("DELETE FROM saved_ng_plots_network_nodes "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_ng_plots_directed
      query->prepare("DELETE FROM saved_ng_plots_directed "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_ng_plots_undirected
      query->prepare("DELETE FROM saved_ng_plots_undirected "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_ng_plots_node_labels
      query->prepare("DELETE FROM saved_ng_plots_node_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_ng_plots_nodelegend
      query->prepare("DELETE FROM saved_ng_plots_nodelegend "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
      // saved_ng_plots_edgelegend
      query->prepare("DELETE FROM saved_ng_plots_edgelegend "
		     "WHERE plot = :plot");
      query->bindValue(":plot", name);
      query->exec();
    } else {
      // Insert new data into saved_eg_plots and then write data.
      query->prepare("INSERT INTO saved_ng_plots (plot) "
		     "VALUES (:name)");
      query->bindValue(":name", name);
      query->exec();
    }
    QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, nodeVector.size());
    saveProgress->setWindowTitle("Saving nodes");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    int counter = 1;
    saveProgress->show();
    QSqlDatabase::database().transaction();
    query->prepare("INSERT INTO saved_ng_plots_network_nodes "
		   "(plot, entity, description, mode, curxpos, curypos, "
		   "red, green, blue, alpha, hidden) "
		   "VALUES (:plot, :entity, :description, :mode, :curxpos, :curypos, "
		   ":red, :green, :blue, :alpha, :hidden)");
    QVectorIterator<NetworkNode*> it(nodeVector);
    while (it.hasNext()) {
      NetworkNode *current = it.next();
      QString entity = current->getName();
      QString description = current->getDescription();
      QString mode = current->getMode();
      qreal x = current->scenePos().x();
      qreal y = current->scenePos().y();
      int red = current->getColor().red();
      int green = current->getColor().green();
      int blue = current->getColor().blue();
      int alpha = current->getColor().alpha();
      int hidden = 1;
      if (current->isVisible()) {
	hidden = 0;
      }
      query->bindValue(":plot", name);
      query->bindValue(":entity", entity);
      query->bindValue(":description", description);
      query->bindValue(":mode", mode);
      query->bindValue(":curxpos", x);
      query->bindValue(":curypos", y);
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
    saveProgress = new ProgressBar(0, 1, labelVector.size());
    saveProgress->setWindowTitle("Saving node labels");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_node_labels "
		   "(plot, entity, curxpos, curypos, xoffset, yoffset, fontsize, "
		   "red, green, blue, alpha, hidden) "
		   "VALUES (:plot, :entity, :curxpos, :curypos, :xoffset, :yoffset, :fontsize, "
		   ":red, :green, :blue, :alpha, :hidden)");
    QVectorIterator<NetworkNodeLabel*> it2(labelVector);
    while (it2.hasNext()) {
      NetworkNodeLabel *current = it2.next();
      QString entity = current->getNode()->getName();
      qreal x = current->scenePos().x();
      qreal y = current->scenePos().y();
      qreal xOffset = current->getOffset().x();
      qreal yOffset = current->getOffset().y();
      int fontSize = current->getFontSize();
      int red = current->defaultTextColor().red();
      int green = current->defaultTextColor().green();
      int blue =  current->defaultTextColor().blue();
      int alpha = current->defaultTextColor().alpha();
      int hidden = 1;
      if (current->isVisible()) {
	hidden = 0;
      }
      query->bindValue(":plot", name);
      query->bindValue(":entity", entity);
      query->bindValue(":curxpos", x);
      query->bindValue(":curypos", y);
      query->bindValue(":xoffset", xOffset);
      query->bindValue(":yoffset", yOffset);
      query->bindValue(":fontsize", fontSize);
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
    saveProgress = new ProgressBar(0, 1, nodeListWidget->rowCount());
    saveProgress->setWindowTitle("Saving node legend");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_nodelegend (plot, name, tip, "
		   "red, green, blue, alpha) "
		   "VALUES (:plot, :name, :tip, :red, :green, :blue, :alpha)");
    for (int i = 0; i != nodeListWidget->rowCount(); i++) {
      QTableWidgetItem *item = nodeListWidget->item(i, 0);
      QString title = item->data(Qt::DisplayRole).toString();
      QString tip = item->data(Qt::ToolTipRole).toString();
      QColor color = nodeListWidget->item(i, 1)->background().color();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      int alpha = color.alpha();
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
    saveProgress = new ProgressBar(0, 1, edgeListWidget->rowCount());
    saveProgress->setWindowTitle("Saving edge legend");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_edgelegend (plot, name, tip, "
		   "red, green, blue, alpha, hidden) "
		   "VALUES (:plot, :name, :tip, :red, :green, :blue, :alpha, :hidden)");
    for (int i = 0; i != edgeListWidget->rowCount(); i++) {
      QTableWidgetItem *item = edgeListWidget->item(i, 0);
      QString title = item->data(Qt::DisplayRole).toString();
      QString tip = item->data(Qt::ToolTipRole).toString();
      QColor color = edgeListWidget->item(i, 1)->background().color();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      int alpha = color.alpha();
      int hidden = 0;
      if (edgeListWidget->item(i, 0)->background() == QColor(Qt::gray)) {
	hidden = 1;
      }
      query->bindValue(":plot", name);
      query->bindValue(":name", title);
      query->bindValue(":tip", tip);
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
    saveProgress = new ProgressBar(0, 1, directedVector.size());
    saveProgress->setWindowTitle("Saving directed edges");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_directed "
		   "(plot, tail, head, name, comment, type, height, filtered, masshidden, "
		   "red, green, blue, alpha, hidden) "
		   "VALUES (:plot, :tail, :head, :name, :comment, :type, :height, :filtered, "
		   ":masshidden, :red, :green, :blue, :alpha, :hidden)");
    QVectorIterator<DirectedEdge*> it3(directedVector);
    while (it3.hasNext()) {
      DirectedEdge *current = it3.next();
      QString tail = current->startItem()->getName();
      QString head = current->endItem()->getName();
      QString relationship = current->getName();
      QString comment = current->getComment();
      QString type = current->getType();
      int height = current->getHeight();
      int filtered = 0;
      if (current->isFiltered()) {
	filtered = 1;
      }
      int massHidden = 0;
      if (current->isMassHidden()) {
	massHidden = 1;
      }
      int red = current->getColor().red();
      int green = current->getColor().green();
      int blue = current->getColor().blue();
      int alpha = current->getColor().alpha();
      int hidden = 1;
      if (current->isVisible()) {
	hidden = 0;
      }
      query->bindValue(":plot", name);
      query->bindValue(":tail", tail);
      query->bindValue(":head", head);
      query->bindValue(":name", relationship);
      query->bindValue(":comment", comment);
      query->bindValue(":type", type);
      query->bindValue(":height", height);
      query->bindValue(":filtered", filtered);
      query->bindValue(":masshidden", massHidden);
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
    saveProgress = new ProgressBar(0, 1, undirectedVector.size());
    saveProgress->setWindowTitle("Saving undirected edges");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_undirected "
		   "(plot, tail, head, name, comment, type, height, filtered, masshidden, "
		   "red, green, blue, alpha, hidden) "
		   "VALUES (:plot, :tail, :head, :name, :comment, :type, :height, :filtered, "
		   ":masshidden, :red, :green, :blue, :alpha, :hidden)");
    QVectorIterator<UndirectedEdge*> it4(undirectedVector);
    while (it4.hasNext()) {
      UndirectedEdge *current = it4.next();
      QString tail = current->startItem()->getName();
      QString head = current->endItem()->getName();
      QString relationship = current->getName();
      QString comment = current->getComment();
      QString type = current->getType();
      int height = current->getHeight();
      int filtered = 0;
      if (current->isFiltered()) {
	filtered = 1;
      }
      int massHidden = 0;
      if (current->isMassHidden()) {
	massHidden = 1;
      }
      int red = current->getColor().red();
      int green = current->getColor().green();
      int blue = current->getColor().blue();
      int alpha = current->getColor().alpha();
      int hidden = 1;
      if (current->isVisible()) {
	hidden = 0;
      }
      query->bindValue(":plot", name);
      query->bindValue(":tail", tail);
      query->bindValue(":head", head);
      query->bindValue(":name", relationship);
      query->bindValue(":type", type);
      query->bindValue(":height", height);
      query->bindValue(":filtered", filtered);
      query->bindValue(":masshidden", massHidden);
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
    plotLabel->setText(name);
    changeLabel->setText("");
    delete saveProgress;
    delete query;
    QSqlDatabase::database().commit();
  }
  delete saveDialog;
}		      
		      
void NetworkGraphWidget::seePlots() {
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this, NETWORKGRAPH);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) {
    savePlotButton->setEnabled(true);
    cleanUp();
    scene->clear();
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT entity, description, mode, curxpos, curypos, "
		   "red, green, blue, alpha, hidden "
		   "FROM saved_ng_plots_network_nodes "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      QString entity = query->value(0).toString();
      QString description = query->value(1).toString();
      QString mode = query->value(2).toString();
      qreal currentX = query->value(3).toReal();
      qreal currentY = query->value(4).toReal();
      int red = query->value(5).toInt();
      int green = query->value(6).toInt();
      int blue = query->value(7).toInt();
      int alpha = query->value(8).toInt();
      int hidden = query->value(9).toInt();
      QPointF currentPos = QPointF(currentX, currentY);
      NetworkNode *node = new NetworkNode(entity, description);
      node->setColor(QColor(red, green, blue, alpha));
      node->setMode(mode);
      scene->addItem(node);
      node->setPos(currentPos);
      node->setZValue(1);
      if (hidden == 1) {
	node->hide();
      } else {
	node->show();
      }
      nodeVector.push_back(node);
    }
    query->prepare("SELECT entity, curxpos, curypos, xoffset, yoffset, fontsize, "
		   "red, green, blue, alpha, hidden "
		   "FROM saved_ng_plots_node_labels WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      QString entity = query->value(0).toString();
      qreal currentX = query->value(1).toReal();
      qreal currentY = query->value(2).toReal();
      qreal xOffset = query->value(3).toReal();
      qreal yOffset = query->value(4).toReal();
      int fontSize = query->value(5).toInt();
      int red = query->value(6).toInt();
      int green = query->value(7).toInt();
      int blue = query->value(8).toInt();
      int alpha = query->value(9).toInt();
      int hidden = query->value(10).toInt();
      QVectorIterator<NetworkNode*> it(nodeVector);
      while (it.hasNext()) {
	NetworkNode *node = it.next();
	QString nodeName = node->getName();
	if (nodeName == entity) {
	  NetworkNodeLabel *currentLabel = new NetworkNodeLabel(node);
	  currentLabel->setPlainText(entity);
	  currentLabel->setPos(QPointF(currentX, currentY));
	  currentLabel->setFontSize(fontSize);
	  currentLabel->setOffset(QPointF(xOffset, yOffset));
	  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
	  scene->addItem(currentLabel);
	  currentLabel->setZValue(2);
	  if (hidden == 1) {
	    currentLabel->hide();
	  } else {
	    currentLabel->show();
	  }
	  node->setLabel(currentLabel);
	  labelVector.push_back(currentLabel);
	  break;
	}
      }
    }
    query->prepare("SELECT name, tip, red, green, blue, alpha "
		   "FROM saved_ng_plots_nodelegend "
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
      nodeListWidget->setRowCount(nodeListWidget->rowCount() + 1);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 0, item);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setBackground(color);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->
	setFlags(nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->flags() ^
		 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    }
    query->prepare("SELECT name, tip, red, green, blue, alpha, hidden "
		   "FROM saved_ng_plots_edgelegend "
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
      int hidden = query->value(6).toInt();
      presentTypes.push_back(name);
      QTableWidgetItem *item = new QTableWidgetItem(name);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
      item->setToolTip(tip);
      item->setData(Qt::DisplayRole, name);
      edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
      edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
      edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
      edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
	setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags() ^
		 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
      if (hidden == 1) {
	edgeListWidget->item(edgeListWidget->rowCount() - 1, 0)->setBackground(Qt::gray);
      }
    }
    query->prepare("SELECT tail, head, name, comment, type, height, filtered, masshidden, "
		   "red, green, blue, alpha, hidden "
		   "FROM saved_ng_plots_directed "
		   "WHERE plot = :plot ");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      QString tail = query->value(0).toString();
      QString head = query->value(1).toString();
      QString name = query->value(2).toString();
      QString comment = query->value(3).toString();
      QString type = query->value(4).toString();
      int height = query->value(5).toInt();
      int filtered = query->value(6).toInt();
      int massHidden = query->value(7).toInt();
      int red = query->value(8).toInt();
      int green = query->value(9).toInt();
      int blue = query->value(10).toInt();
      int alpha = query->value(11).toInt();
      int hidden = query->value(12).toInt();
      QVectorIterator<NetworkNode*> it(nodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext()) {
	NetworkNode *currentItem = it.next();
	if (currentItem->getName() == tail) {
	  tempSource = currentItem;
	} else if (currentItem->getName() == head) {
	  tempTarget = currentItem;
	}
	if (tempSource != NULL && tempTarget != NULL) {
	  DirectedEdge *currentEdge = new DirectedEdge(tempSource, tempTarget,
							type, name);
	  currentEdge->setComment(comment);
	  currentEdge->setHeight(height);
	  if (filtered == 1) {
	    currentEdge->setFiltered(true);
	  }
	  if (massHidden == 1) {
	    currentEdge->setMassHidden(true);
	  }
	  currentEdge->setColor(QColor(red, green, blue, alpha));
	  if (hidden == 1) {
	    currentEdge->hide();
	  } else {
	    scene->addItem(currentEdge);
	  }
	  directedVector.push_back(currentEdge);
	  break;
	}
      }
    }
    plotLabel->setText(plot);
    changeLabel->setText("");
    setRangeControls();
    query->prepare("SELECT tail, head, name, comment, type, height, filtered, masshidden, "
		   "red, green, blue, alpha, hidden "
		   "FROM saved_ng_plots_undirected "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    while (query->next()) {
      QString tail = query->value(0).toString();
      QString head = query->value(1).toString();
      QString name = query->value(2).toString();
      QString comment = query->value(3).toString();
      QString type = query->value(4).toString();
      int height = query->value(5).toInt();
      int filtered = query->value(6).toInt();
      int massHidden = query->value(7).toInt();
      int red = query->value(8).toInt();
      int green = query->value(9).toInt();
      int blue = query->value(10).toInt();
      int alpha = query->value(11).toInt();
      int hidden = query->value(12).toInt();
      QVectorIterator<NetworkNode*> it(nodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext()) {
	NetworkNode *currentItem = it.next();
	if (currentItem->getName() == tail) {
	  tempSource = currentItem;
	} else if (currentItem->getName() == head) {
	  tempTarget = currentItem;
	}
	if (tempSource != NULL && tempTarget != NULL) {
	  UndirectedEdge *currentEdge = new UndirectedEdge(tempSource, tempTarget,
							    type, name);
	  currentEdge->setComment(comment);
	  currentEdge->setHeight(height);
	  if (filtered == 0) {
	    currentEdge->setFiltered(false);
	  }
	  if (massHidden == 1) {
	    currentEdge->setMassHidden(true);
	  }
	  currentEdge->setColor(QColor(red, green, blue, alpha));
	  if (hidden == 1) {
	    currentEdge->hide();
	  } else {
	    scene->addItem(currentEdge);
	  }
	  undirectedVector.push_back(currentEdge);
	  break;
	}
      }
    }
    query->exec("SELECT DISTINCT incident "
		"FROM relationships_to_incidents");
    query->exec();
    while (query->next()) {
      int incident = query->value(0).toInt();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
      query2->bindValue(":incident", incident);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (minOrder == 0) {
	minOrder = order;
      } else if (order < minOrder) {
	minOrder = order;
      }
      if (maxOrder == 0) {
	maxOrder = order;
      } else if (order > maxOrder) {
	maxOrder = order;
      }
      delete query2;
    }
    checkCongruency();
    setRangeControls();
    setVisibility();
    plotLabel->setText(plot);
    changeLabel->setText("");
    view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    delete query;
  } else if (savedPlotsDialog->getExitStatus() == 2) {
    // DON'T FORGET TO UPDATE THIS FUNCTION!!!!
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    // saved_ng_plots
    query->prepare("DELETE FROM saved_ng_plots "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_ng_plots_network_nodes
    query->prepare("DELETE FROM saved_ng_plots_network_nodes "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_ng_plots_node_labels
    query->prepare("DELETE FROM saved_ng_plots_node_labels "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    // saved_ng_plots_nodelegend
    query->prepare("DELETE FROM saved_ng_plots_nodelegend "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_ng_plots_edgelegend
    query->prepare("DELETE FROM saved_ng_plots_edgelegend "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_ng_plots_directed
    query->prepare("DELETE FROM saved_ng_plots_directed "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    // saved_ng_plots_undirected
    query->prepare("DELETE FROM saved_ng_plots_undirected "
		   "WHERE plot = :plot");
    query->bindValue(":plot", plot);
    query->exec();
    delete query;
    seePlots();
  }
}

void NetworkGraphWidget::setChangeLabel() {
  if (changeLabel->text() == "" && nodeVector.size() > 0) {
    changeLabel->setText("*");
  }
}

void NetworkGraphWidget::processLowerRange(int value) {
  lowerRangeDial->setValue(value);
  lowerRangeSpinBox->setValue(value);
  if (upperRangeDial->value() <= value) {
    upperRangeDial->setValue(value + 1);
    upperRangeSpinBox->setValue(value + 1);
  }
  setVisibility();
}

void NetworkGraphWidget::processUpperRange(int value) {
  upperRangeDial->setValue(value);
  upperRangeSpinBox->setValue(value);
  if (lowerRangeDial->value() >= value) {
    lowerRangeDial->setValue(value - 1);
    lowerRangeSpinBox->setValue(value - 1);
  }
  setVisibility();
}

void NetworkGraphWidget::setVisibility() {
 QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode* currentNode = it.next();
    currentNode->hide();
  }
  QVectorIterator<DirectedEdge*> it2(directedVector);
  while (it2.hasNext()) {
    bool show = false;
    DirectedEdge *currentDirected = it2.next();
    QString relationship = currentDirected->getName();
    QString type = currentDirected->getType();
    if (presentTypes.contains(type)) {
      if (relationship == CREATED && !(currentDirected->isMassHidden())) {
	show = true;
      } else if (currentDirected->isMassHidden()) {
	show = false;
      } else if (currentDirected->isFiltered()) {
	QSqlQuery *query = new QSqlQuery;
	query->prepare("SELECT incident FROM relationships_to_incidents "
		       "WHERE relationship = :relationship AND type = :type");
	query->bindValue(":relationship", relationship);
	query->bindValue(":type", type);
	query->exec();
	while (query->next()) {
	  int incident = query->value(0).toInt();
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
	  query2->bindValue(":incident", incident);
	  query2->exec();
	  query2->first();
	  int order = query2->value(0).toInt();
	  if (order >= lowerRangeDial->value() && order <= upperRangeDial->value()) {
	    show = true;
	  }
	  delete query2;
	}
	delete query;
      } else {
	show = true;
      }
    }
    if (show) {
      currentDirected->show();
      currentDirected->startItem()->show();
      currentDirected->endItem()->show();
    } else {
      currentDirected->hide();
    }
  }
  QVectorIterator<UndirectedEdge*> it3(undirectedVector);
  while (it3.hasNext()) {
    bool show = false;
    UndirectedEdge *currentUndirected = it3.next();
    QString relationship = currentUndirected->getName();
    QString type = currentUndirected->getType();
    if (presentTypes.contains(type)) {
      if (relationship == CREATED && !(currentUndirected->isMassHidden())) {
	show = true;
      } else if (currentUndirected->isMassHidden()) {
	show = false;
      } else if (currentUndirected->isFiltered()) {
	QSqlQuery *query = new QSqlQuery;
	query->prepare("SELECT incident FROM relationships_to_incidents "
		       "WHERE relationship = :relationship AND type = :type");
	query->bindValue(":relationship", relationship);
	query->bindValue(":type", type);
	query->exec();
	while (query->next()) {
	  int incident = query->value(0).toInt();
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
	  query2->bindValue(":incident", incident);
	  query2->exec();
	  query2->first();
	  int order = query2->value(0).toInt();
	  if (order >= lowerRangeDial->value() && order <= upperRangeDial->value()) {
	    show = true;
	  }
	  delete query2;
	}
	delete query;
      } else {
	show = true;
      }
    }
    if (show) {
      currentUndirected->show();
      currentUndirected->startItem()->show();
      currentUndirected->endItem()->show();
    } else {
      currentUndirected->hide();
    }
  }
  processHeights();
  QVectorIterator<NetworkNodeLabel*> it4(labelVector);
  while (it4.hasNext()) {
    NetworkNodeLabel *label = it4.next();
    if (label->getNode()->isVisible() && labelsShown) {
      label->show();
    } else {
      label->hide();
    }
  }
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
}

void NetworkGraphWidget::processHeights() {
  QList<QGraphicsItem*> edgeList;
  for (QVector<DirectedEdge*>::size_type i = 0; i != directedVector.length(); i++) {
    if (directedVector[i]->isVisible()) {
      directedVector[i]->setHeight(20);
      edgeList.push_back(directedVector[i]);
    }
  }
  for (QVector<UndirectedEdge*>::size_type i = 0; i != undirectedVector.length(); i++) {
    if (undirectedVector[i]->isVisible()) {
      undirectedVector[i]->setHeight(20);
      edgeList.push_back(undirectedVector[i]);
    }
  }
  for (QList<QGraphicsItem*>::size_type i = 0; i != edgeList.length(); i++) {
    DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(edgeList[i]);
    UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(edgeList[i]);
    if (i != edgeList.length() - 1) {
      for (QList<QGraphicsItem*>::size_type j = i + 1; j != edgeList.length(); j++) {
	DirectedEdge *secondDirected = qgraphicsitem_cast<DirectedEdge*>(edgeList[j]);
	UndirectedEdge *secondUndirected = qgraphicsitem_cast<UndirectedEdge*>(edgeList[j]);
	if (directed && secondDirected) {
	  if (secondDirected->startItem() == directed->startItem() &&
	      secondDirected->endItem() == directed->endItem()) {
	    directed->setHeight(directed->getHeight() + 60);
	  }
	} else if (directed && secondUndirected) {
	  if (secondUndirected->startItem() == directed->startItem() &&
	      secondUndirected->endItem() == directed->endItem()) {
	    directed->setHeight(directed->getHeight() + 60);
	  }
	} else if (undirected && secondDirected) {
	  if (secondDirected->startItem() == undirected->startItem() &&
	      secondDirected->endItem() == undirected->endItem()) {
	    undirected->setHeight(undirected->getHeight() + 60);
	  }
	} else if (undirected && secondUndirected) {
	  if (secondUndirected->startItem() == undirected->startItem() &&
	      secondUndirected->endItem() == undirected->endItem()) {
	    undirected->setHeight(undirected->getHeight() + 60);
	  }
	}
      }
    }
  }
}

void NetworkGraphWidget::cleanUp() {
  qDeleteAll(directedVector);
  directedVector.clear();
  qDeleteAll(undirectedVector);
  undirectedVector.clear();
  qDeleteAll(nodeVector);
  nodeVector.clear();
  qDeleteAll(labelVector);
  labelVector.clear();
  scene->clearSelection();
  scene->clear();
  nodeListWidget->setRowCount(0);
  edgeListWidget->setRowCount(0);
  presentTypes.clear();
  minOrder = 0;
  maxOrder = 0;
  selectedEntityName = "";
  nameField->clear();
  descriptionField->clear();
  resetFont(attributesTree);
}

bool NetworkGraphWidget::eventFilter(QObject *object, QEvent *event) {
  if (object == view->viewport() && event->type() == QEvent::MouseButtonRelease) {
    retrieveData();
    if (massMove) {
      massMove = false;
    }
  } else if (object == attributesTreeView && event->type() == QEvent::ChildRemoved) {
    fixTree(selectedEntityName);
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


