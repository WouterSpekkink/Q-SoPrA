#include "../include/EventGraphWidget.h"

EventGraphWidget::EventGraphWidget(QWidget *parent) : QWidget(parent) 
{
  selectedCoder = "";
  selectedCompare = "";
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
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
  scene->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  
  infoWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
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

  linkageListWidget = new DeselectableListWidget(legendWidget);
  linkageListWidget->setColumnCount(2);
  linkageListWidget->horizontalHeader()->hide();
  linkageListWidget->verticalHeader()->hide();
  linkageListWidget->setColumnWidth(1, 20);
  linkageListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  linkageListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  linkageListWidget->setStyleSheet("QTableView {gridline-color: black}");
  linkageListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  
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
  casesLabel = new QLabel(tr("<b>Case filtering:</b>"), graphicsWidget);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  indexLabel = new QLabel(tr("<b>(0/0)</b>"), infoWidget);
  attributesLabel = new QLabel(tr("<b>Attributes:</b>"), attWidget);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), attWidget);
  valueLabel = new QLabel(tr("<b>Value:</b>"), attWidget);
  incongruencyLabel = new QLabel(tr(""), this);
  incongruencyLabel->setStyleSheet("QLabel {color : red;}");
  eventLegendLabel = new QLabel(tr("<b>Modes:</b>"), legendWidget);
  linkageLegendLabel = new QLabel(tr("<b>Linkages:</b>"), legendWidget);
  zoomLabel = new QLabel(tr("<b>Zoom slider:</b>"), this);
  
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

  zoomSlider = new QSlider(Qt::Horizontal, this);
  zoomSlider->installEventFilter(this);
  zoomSlider->setMinimum(-9);
  zoomSlider->setMaximum(9);
  zoomSlider->setValue(0);

  timeStampField = new QLineEdit(infoWidget);
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit(infoWidget);
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit(infoWidget);
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit(infoWidget);
  rawField->setReadOnly(true);
  commentField = new QTextEdit(commentWidget);
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

  caseListWidget = new QListWidget(graphicsWidget);
  caseListWidget->setEnabled(false);
  
  plotButton = new QPushButton(tr("Plot new"), this);
  addLinkageTypeButton = new QPushButton(tr("Add linkage"), this);
  addLinkageTypeButton->setEnabled(false);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  plotButton->setEnabled(false);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  exportTableButton = new QPushButton(tr("Export table"), graphicsWidget);
  exportNodesButton = new QPushButton(tr("Export nodes"), graphicsWidget);
  exportEdgesButton = new QPushButton(tr("Export edges"), graphicsWidget);
  increaseDistanceButton = new QPushButton("< >", this);
  decreaseDistanceButton = new QPushButton("> <", this);
  expandButton = new QPushButton(tr("Expand"), this);
  contractButton = new QPushButton(tr("Contract"), this);
  compareButton = new QPushButton(tr("Compare"), this);
  toggleDetailsButton = new QPushButton(tr("Toggle details"), this);
  toggleDetailsButton->setCheckable(true);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  previousEventButton = new QPushButton("<<", infoWidget);
  previousEventButton->setEnabled(false);   
  nextEventButton = new QPushButton(">>", infoWidget);
  seeComponentsButton = new QPushButton(tr("Components"), infoWidget);
  seeComponentsButton->setEnabled(false);
  nextEventButton->setEnabled(false);
  plotLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  colorByAttributeButton = new QPushButton(tr("Create mode"), legendWidget);
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
  removeTextButton = new QPushButton("Remove text", attWidget);
  removeTextButton->setEnabled(false);
  resetTextsButton = new QPushButton("Reset texts", attWidget);
  resetTextsButton->setEnabled(false);
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  removeModeButton = new QPushButton(tr("Remove mode"), legendWidget);
  removeModeButton->setEnabled(false);
  restoreModeColorsButton = new QPushButton(tr("Restore colors"), legendWidget);
  exportTransitionMatrixButton = new QPushButton(tr("Export transitions"), legendWidget);
  moveModeUpButton = new QPushButton(tr("Up"), legendWidget);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton = new QPushButton(tr("Down"), legendWidget);
  moveModeDownButton->setEnabled(false);
  hideLinkageTypeButton = new QPushButton(tr("Hide"), legendWidget);
  hideLinkageTypeButton->setEnabled(false);
  showLinkageTypeButton = new QPushButton(tr("Show"), legendWidget);
  showLinkageTypeButton->setEnabled(false);
  removeLinkageTypeButton = new QPushButton(tr("Remove from plot"), legendWidget);
  removeLinkageTypeButton->setEnabled(false);
  
  view->viewport()->installEventFilter(this);
  attributesTreeView->installEventFilter(this);
  rawField->viewport()->installEventFilter(infoWidget);
  commentField->installEventFilter(commentWidget);
  
  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(seeAttributesButton, SIGNAL(clicked()), this, SLOT(showAttributes()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(valueField, SIGNAL(textChanged(const QString &)), this, SLOT(setValueButton()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(seeCommentsButton, SIGNAL(clicked()), this, SLOT(showComments()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(coderComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButtons()));
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButtons()));
  connect(compareComboBox, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setCompareButton()));  
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotGraph()));
  connect(addLinkageTypeButton, SIGNAL(clicked()), this, SLOT(addLinkageType()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(plotLabelsButton, SIGNAL(clicked()), this, SLOT(plotLabels()));
  connect(colorByAttributeButton, SIGNAL(clicked()), this, SLOT(colorByAttribute()));
  connect(eventColorButton, SIGNAL(clicked()), this, SLOT(setEventColor()));
  connect(labelColorButton, SIGNAL(clicked()), this, SLOT(setLabelColor()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(exportTableButton, SIGNAL(clicked()), this, SLOT(exportTable()));
  connect(exportNodesButton, SIGNAL(clicked()), this, SLOT(exportNodes()));
  connect(exportEdgesButton, SIGNAL(clicked()), this, SLOT(exportEdges()));
  connect(compareButton, SIGNAL(clicked()), this, SLOT(compare()));
  connect(scene, SIGNAL(resetItemSelection()), this, SLOT(retrieveData()));
  connect(scene, SIGNAL(posChanged(EventItem*, qreal&)),
	  this, SLOT(changePos(EventItem*, qreal&)));
  connect(scene, SIGNAL(posChanged(MacroEvent*, qreal&)),
	  this, SLOT(changePos(MacroEvent*, qreal&)));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(updateLinkages()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(scene, SIGNAL(EventItemContextMenuAction(const QString &)),
	  this, SLOT(processEventItemContextMenu(const QString &)));
  connect(scene, SIGNAL(ArrowContextMenuAction(const QString &)),
	  this, SLOT(processArrowContextMenu(const QString &)));
  connect(scene, SIGNAL(LineContextMenuAction(const QString &)),
	  this, SLOT(processLineContextMenu(const QString &)));
  connect(scene, SIGNAL(TextContextMenuAction(const QString &)),
	  this, SLOT(processTextContextMenu(const QString &)));
  connect(scene, SIGNAL(EllipseContextMenuAction(const QString &)),
	  this, SLOT(processEllipseContextMenu(const QString &)));
  connect(scene, SIGNAL(RectContextMenuAction(const QString &)),
	  this, SLOT(processRectContextMenu(const QString &)));
  connect(this, SIGNAL(changeEventWidth(QGraphicsItem*)), scene, SLOT(modEventWidth(QGraphicsItem*)));
  connect(view, SIGNAL(EventGraphContextMenuAction(const QString &, const QPoint &)),
	  this, SLOT(processEventGraphContextMenu(const QString &, const QPoint &)));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(highlightText()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(attributesTreeView, SIGNAL(noneSelected()), this, SLOT(setButtons()));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(previousEventButton, SIGNAL(clicked()), this, SLOT(previousDataItem()));
  connect(seeComponentsButton, SIGNAL(clicked()), this, SLOT(seeComponents()));
  connect(nextEventButton, SIGNAL(clicked()), this, SLOT(nextDataItem()));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(expandButton, SIGNAL(clicked()), this, SLOT(expandGraph()));
  connect(contractButton, SIGNAL(clicked()), this, SLOT(contractGraph()));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(processZoomSliderChange(int)));
  connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(resetZoomSlider()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(eventListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setModeButtons(QTableWidgetItem *)));
  connect(eventListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableModeButtons()));
  connect(eventListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeModeColor(QTableWidgetItem *)));
  connect(linkageListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setLinkageButtons(QTableWidgetItem *)));
  connect(linkageListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableLinkageButtons()));
  connect(linkageListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeLinkageColor(QTableWidgetItem *)));
  connect(caseListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(checkCases()));
  connect(hideLinkageTypeButton, SIGNAL(clicked()), this, SLOT(hideLinkageType()));
  connect(showLinkageTypeButton, SIGNAL(clicked()), this, SLOT(showLinkageType()));
  connect(removeLinkageTypeButton, SIGNAL(clicked()), this, SLOT(removeLinkageType()));
  connect(removeModeButton, SIGNAL(clicked()), this, SLOT(removeMode()));
  connect(restoreModeColorsButton, SIGNAL(clicked()), this, SLOT(restoreModeColors()));
  connect(moveModeUpButton, SIGNAL(clicked()), this, SLOT(moveModeUp()));
  connect(moveModeDownButton, SIGNAL(clicked()), this, SLOT(moveModeDown()));
  connect(exportTransitionMatrixButton, SIGNAL(clicked()), this, SLOT(exportTransitionMatrix()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
    
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(coderLabel);
  plotOptionsLayout->addWidget(coderComboBox);
  plotOptionsLayout->addWidget(typeLabel);
  plotOptionsLayout->addWidget(typeComboBox);
  plotOptionsLayout->addWidget(plotButton);
  plotOptionsLayout->addWidget(addLinkageTypeButton);
  plotOptionsLayout->addWidget(savePlotButton);
  plotOptionsLayout->addWidget(seePlotsButton);
  plotOptionsLayout->addWidget(plotLabel);
  plotOptionsLayout->addWidget(changeLabel);
  plotOptionsLayout->addWidget(incongruencyLabel);
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
  navigationLayout->addWidget(seeComponentsButton);
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
  legendLayout->addWidget(eventLegendLabel);
  legendLayout->addWidget(eventListWidget);
  QPointer<QHBoxLayout> modeButtonsLayout = new QHBoxLayout;
  modeButtonsLayout->addWidget(moveModeUpButton);
  modeButtonsLayout->addWidget(moveModeDownButton);
  legendLayout->addLayout(modeButtonsLayout);
  legendLayout->addWidget(colorByAttributeButton);
  legendLayout->addWidget(removeModeButton);
  legendLayout->addWidget(restoreModeColorsButton);
  legendLayout->addWidget(exportTransitionMatrixButton);
  legendLayout->addWidget(linkageLegendLabel);
  legendLayout->addWidget(linkageListWidget);
  legendLayout->addWidget(hideLinkageTypeButton);
  legendLayout->addWidget(showLinkageTypeButton);
  legendLayout->addWidget(removeLinkageTypeButton);
  legendWidget->setMinimumWidth(250);
  legendWidget->setMaximumWidth(250);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				   
  
  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(casesLabel);
  graphicsControlsLayout->addWidget(caseListWidget);
  graphicsControlsLayout->addWidget(eventColorButton);
  graphicsControlsLayout->addWidget(labelColorButton);
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
  graphicsControlsLayout->addWidget(exportTableButton);
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
  toggleDetailsButton->setMaximumWidth(toggleDetailsButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(increaseDistanceButton);
  drawOptionsLeftLayout->addWidget(decreaseDistanceButton);
  increaseDistanceButton->setMaximumWidth(increaseDistanceButton->sizeHint().width());
  decreaseDistanceButton->setMaximumWidth(decreaseDistanceButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(expandButton);
  drawOptionsLeftLayout->addWidget(contractButton);
  expandButton->setMaximumWidth(expandButton->sizeHint().width());
  contractButton->setMaximumWidth(contractButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(zoomLabel);
  zoomLabel->setMaximumWidth(zoomLabel->sizeHint().width());
  drawOptionsLeftLayout->addWidget(zoomSlider);
  zoomSlider->setMaximumWidth(100);
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);
    
  setLayout(mainLayout);
  infoWidget->hide();
  graphicsWidget->hide();
  attWidget->hide();
  legendWidget->hide();
  updateCases();

  setGraphControls(false);
 }

void EventGraphWidget::checkCongruency() 
{
  if (eventVector.size() > 0) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT id FROM incidents ORDER BY ch_order ASC");
      QVector<int> temp;
      while (query->next()) 
	{
	  int id = query->value(0).toInt();
	  temp.push_back(id);
	}
      std::sort(eventVector.begin(), eventVector.end(), componentsSort);
      if (temp.size() != eventVector.size()) 
	{
	  incongruencyLabel->setText("Incongruency detected");
	  return;
	}
      for (QVector<EventItem*>::size_type i = 0; i != eventVector.size(); i++) 
	{
	  EventItem *current = eventVector[i];
	  if (current->getId() != temp[i]) 
	    {
	      incongruencyLabel->setText("Incongruency detected");
	      return;
	    }
	}
      QVectorIterator<QString> it(presentTypes);
      while (it.hasNext()) 
	{
	  QString currentType = it.next();
	  query->prepare("SELECT tail, head FROM linkages "
			 "WHERE type = :type AND coder = :coder");
	  query->bindValue(":type", currentType);
	  query->bindValue(":coder", selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      int tailId = query->value(0).toInt();
	      int headId = query->value(1).toInt();
	      bool tailVisible = false;
	      bool headVisible = false;
	      QVectorIterator<EventItem*> it2(eventVector);
	      while (it2.hasNext()) 
		{
		  EventItem *currentEvent = it2.next();
		  if (currentEvent->isVisible()) 
		    {
		      if (currentEvent->getId() == tailId) 
			{
			  tailVisible = true;
			}
		      else if (currentEvent->getId() == headId) 
			{
			  headVisible = true;
			}
		    }
		}
	      if (tailVisible && headVisible) 
		{
		  QVectorIterator<Arrow*> it3(edgeVector);
		  bool found = false;
		  while (it3.hasNext()) 
		    {
		      Arrow *current = it3.next();
		      EventItem *startEvent = qgraphicsitem_cast<EventItem*>(current->startItem());
		      EventItem *endEvent = qgraphicsitem_cast<EventItem*>(current->endItem());
		      if (startEvent && endEvent) 
			{
			  if (startEvent->getId() == tailId &&
			      endEvent->getId() == headId) 
			    {
			      found = true;
			    }
			}
		    }
		  if (!found) 
		    {
		      incongruencyLabel->setText("Incongruency detected");
		      return;
		    }
		}
	    }
	}
      QVectorIterator<Arrow*> it3(edgeVector);
      while (it3.hasNext()) 
	{
	  Arrow* current = it3.next();
	  QString currentType = current->getType();
	  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(current->startItem());
	  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(current->endItem());
	  if (startEvent && endEvent) 
	    {
	      int currentTail = startEvent->getId();
	      int currentHead = endEvent->getId();
	      bool found = false;
	      query->prepare("SELECT tail, head FROM linkages "
			     "WHERE type = :type AND coder = :coder");
	      query->bindValue(":type", currentType);
	      query->bindValue(":coder", selectedCoder);
	      query->exec();
	      while (query->next()) 
		{
		  int tail = query->value(0).toInt();
		  int head = query->value(1).toInt();
		  if (currentTail == tail && currentHead == head) 
		    {
		      found = true;
		    }
		}
	      if (!found) 
		{
		  incongruencyLabel->setText("Incongruency detected");
		  return;
		}
	    }
	}
      delete query;
      incongruencyLabel->setText("");
    }
}

void EventGraphWidget::setCommentBool() 
{
  commentBool = true;
}

void EventGraphWidget::setComment() 
{
  if (commentBool && selectedIncident != 0) 
    {
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
    }
  else if (commentBool && selectedMacro != NULL) 
    {
      QString comment = commentField->toPlainText();
      selectedMacro->setComment(comment);
    }
}

void EventGraphWidget::toggleLegend() 
{
  if (legendWidget->isHidden()) 
    {
      legendWidget->show();
    }
  else 
    {
      legendWidget->hide();
    }
  rescale();
}

void EventGraphWidget::toggleDetails() 
{
  setComment();
  if (infoWidget->isHidden()) 
    {
      infoWidget->show();
    }
  else 
    {
      infoWidget->hide();
    }
  rescale();
}

void EventGraphWidget::toggleGraphicsControls() 
{
  if (graphicsWidget->isHidden()) 
    {
      graphicsWidget->show();
    }
  else 
    {
      graphicsWidget->hide();
    }
  rescale();
}

void EventGraphWidget::rescale()
{
  view->scale(2.0, 2.0);
  view->scale(0.5, 0.5);
}

void EventGraphWidget::processZoomSliderChange(int value)
{
  while (zoomSlider->sliderPosition() != 0)
    {
      view->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
      if (value < 0)
	{
	  double scale = (value * -1) / 250.0;
	  view->scale(1.0 / (1 + scale), 1.0 / (1 + scale));
	}
      else
	{
	  double scale = value / 250.0;
	  view->scale(1 + scale, 1 + scale);
	}
      qApp->processEvents();
      if (!zoomSlider->isSliderDown()) {
	break;
      }
    }
  resetZoomSlider();
}

void EventGraphWidget::resetZoomSlider()
{
  zoomSlider->setValue(0);
  zoomSlider->setSliderPosition(0);
}

void EventGraphWidget::setGraphControls(bool status)
{
  zoomSlider->setEnabled(status);
  expandButton->setEnabled(status);
  contractButton->setEnabled(status);
  increaseDistanceButton->setEnabled(status);
  decreaseDistanceButton->setEnabled(status);
}

void EventGraphWidget::updateCases() 
{
  caseListWidget->clear();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM cases");
  while (query->next()) 
    {
      QString currentCase = query->value(0).toString();
      QListWidgetItem *item = new QListWidgetItem(currentCase, caseListWidget);
      item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
      item->setCheckState(Qt::Unchecked);
    }
}

void EventGraphWidget::checkCases() 
{
  for (int i = 0; i != caseListWidget->count(); i++) 
    {
      QListWidgetItem *item = caseListWidget->item(i);
      if (item->checkState() == Qt::Checked) 
	{
	  if (!checkedCases.contains(item->data(Qt::DisplayRole).toString())) 
	    {
	      checkedCases.push_back(item->data(Qt::DisplayRole).toString());
	    }
	}
      else 
	{
	  if (checkedCases.contains(item->data(Qt::DisplayRole).toString())) 
	    {
	      checkedCases.removeOne(item->data(Qt::DisplayRole).toString());
	    }
	}
    }
  setVisibility();
}

void EventGraphWidget::showAttributes() 
{
  commentWidget->hide();
  attWidget->show();
}

void EventGraphWidget::showComments() 
{
  attWidget->hide();
  commentWidget->show();
}

void EventGraphWidget::setValueButton()
{
  valueButton->setEnabled(true);
}

void EventGraphWidget::setValue()
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      if (selectedIncident != 0)
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE attributes_to_incidents "
			 "SET value = :val "
			 "WHERE attribute = :attribute AND incident = :incident");
	  query->bindValue(":val", valueField->text());
	  query->bindValue(":attribute", attribute);
	  query->bindValue(":incident", selectedIncident);
	  query->exec();
	  valueButton->setEnabled(false);
	  delete query;
	}
      else if (selectedMacro != NULL)
	{
	  selectedMacro->insertValue(attribute, valueField->text());
	  valueButton->setEnabled(false);
	}
    }
}

void EventGraphWidget::retrieveData() 
{
  setComment();
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->setEnabled(false);
  valueField->blockSignals(false);
  valueButton->setEnabled(false);
  attributesTreeView->resetSelection();
  if (currentData.size() > 0) 
    {
      currentData.clear();
    }
  if (scene->selectedItems().size() > 0) 
    {
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	  Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
	  if (event) 
	    {
	      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
	      currentEvent->setSelectionColor(Qt::black);
	      currentEvent->update();
	      if (currentEvent->isVisible()) 
		{
		  currentData.push_back(currentEvent);
		}
	    }	else if (macro) 
	    {
	      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(it.next());
	      currentMacro->setSelectionColor(Qt::black);
	      currentMacro->update();
	      if (currentMacro->isVisible()) 
		{
		  currentData.push_back(currentMacro);
		}
	    }
	  else if (arrow && scene->selectedItems().size() > 1) 
	    {
	      scene->blockSignals(true);
	      arrow->setSelected(false);
	      scene->blockSignals(false);
	      it.next();
	    }
	  else 
	    {
	      it.next();
	    }
	}
      if (currentData.size() > 0) 
	{
	  std::sort(currentData.begin(), currentData.end(), eventLessThan);   
	  vectorPos = 0;
	  QString indexText = "(" + QString::number(vectorPos + 1) + "/"
	    + QString::number(currentData.size()) + ")";
	  indexLabel->setText(indexText);
	  EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
	  MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
	  if (currentEvent) 
	    {
	      selectedMacro = NULL;
	      seeComponentsButton->setEnabled(false);
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
	      if (query->isNull(0)) 
		{
		  timeStampField->setText("Incident was deleted");
		  descriptionField->setText("Incident was deleted");
		  rawField->setText("Incident was deleted");
		  commentField->setText("Incident was deleted");
		  sourceField->setText("Incident was deleted");
		}
	      else 
		{
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
		  QSqlQuery *query2 = new QSqlQuery;
		  query2->prepare("SELECT attribute FROM attributes_to_incidents "
				  "WHERE incident = :id");
		  query2->bindValue(":id", id);
		  query2->exec();
		  while (query2->next()) 
		    {
		      QString attribute = query2->value(0).toString();
		      boldSelected(attributesTree, attribute, id, INCIDENT);
		    }
		  delete query2;
		}
	      delete query;
	    }
	  else if (currentMacro) 
	    {
	      selectedMacro = currentMacro;
	      selectedIncident = 0;
	      currentMacro->setSelectionColor(Qt::red);
	      currentMacro->update();
	      seeComponentsButton->setEnabled(true);
	      descriptionField->setText(currentMacro->getDescription());
	      sourceLabel->setText("<b>Number of incidents:</b>");
	      int id = currentMacro->getIncidents().first()->getId();
	      rawLabel->hide();
	      rawField->hide();
	      QString timing = currentMacro->getTiming();
	      QString countText = QString::number(currentMacro->getIncidents().size());
	      timeStampField->setText(timing);
	      sourceField->setText(countText);
	      commentField->setText(currentMacro->getComment());
	      resetFont(attributesTree);
	      QSet<QString> attributes = currentMacro->getAttributes();
	      QSet<QString>::iterator it2;
	      id = currentMacro->getId();
	      for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
		{
		  QString attribute  = *it2;
		  boldSelected(attributesTree, attribute, id, MACRO);
		}
	    }
	  previousEventButton->setEnabled(true);
	  nextEventButton->setEnabled(true);
	}
    }
  else 
    {
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
      seeComponentsButton->setEnabled(false);
      resetFont(attributesTree);
    }
}

void EventGraphWidget::seeComponents() 
{
  emit seeHierarchy(selectedMacro);
}

void EventGraphWidget::previousDataItem() 
{
  setComment();
  if (vectorPos > 0) 
    {
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if(currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::black);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::black);
	  currentMacro->update();
	}
      vectorPos--;
      QString indexText = "(" + QString::number(vectorPos + 1) + "/"
	+ QString::number(currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if (currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::red);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::red);
	  currentMacro->update();
	}
      if (currentEvent) 
	{
	  selectedMacro = NULL;
	  seeComponentsButton->setEnabled(false);
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
	  if (query->isNull(0)) 
	    {
	      timeStampField->setText("Incident was deleted");
	      descriptionField->setText("Incident was deleted");
	      rawField->setText("Incident was deleted");
	      commentField->setText("Incident was deleted");
	      sourceField->setText("Incident was deleted");
	    }
	  else 
	    {
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
	  resetFont(attributesTree);
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :id");
	  query->bindValue(":id", id);
	  query->exec();
	  while (query->next()) 
	    {
	      QString attribute = query->value(0).toString();
	      boldSelected(attributesTree, attribute, id, INCIDENT);
	    }
	  delete query;
	}
      else if (currentMacro) 
	{
	  selectedMacro = currentMacro;
	  selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentMacro->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentMacro->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
	  QString timestamp = currentMacro->getTiming();
	  QVectorIterator<MacroEvent*> it(macroVector);
	  QString countText = QString::number(currentMacro->getIncidents().size());
	  timeStampField->setText(timestamp);
	  sourceField->setText(countText);
	  commentField->setText(currentMacro->getComment());
	  resetFont(attributesTree);
	  QSet<QString> attributes = currentMacro->getAttributes();
	  QSet<QString>::iterator it2;
	  id = currentMacro->getId();
	  for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
	    {
	      QString attribute  = *it2;
	      boldSelected(attributesTree, attribute, id, MACRO);
	    }
	}
    }
  else 
    {
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if(currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::black);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::black);
	  currentMacro->update();
	}
      vectorPos = currentData.size() - 1;
      QString indexText = "(" + QString::number(vectorPos + 1) + "/"
	+ QString::number(currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if (currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::red);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::red);
	  currentMacro->update();
	}
      if (currentEvent) 
	{
	  selectedMacro = 0;
	  seeComponentsButton->setEnabled(false);
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
	  if (query->isNull(0)) 
	    {
	      timeStampField->setText("Incident was deleted");
	      descriptionField->setText("Incident was deleted");
	      rawField->setText("Incident was deleted");
	      commentField->setText("Incident was deleted");
	      sourceField->setText("Incident was deleted");
	    }
	  else 
	    {
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
	  resetFont(attributesTree);
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :id");
	  query->bindValue(":id", id);
	  query->exec();
	  while (query->next()) 
	    {
	      QString attribute = query->value(0).toString();
	      boldSelected(attributesTree, attribute, id, INCIDENT);
	    }
	  delete query;
	}
      else if (currentMacro) 
	{
	  selectedMacro = currentMacro;
	  selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentMacro->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentMacro->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
	  QString timestamp = currentMacro->getTiming();
	  QString countText = QString::number(currentMacro->getIncidents().size());
	  sourceField->setText(countText);
	  commentField->setText(currentMacro->getComment());
	  resetFont(attributesTree);
	  QSet<QString> attributes = currentMacro->getAttributes();
	  QSet<QString>::iterator it2;
	  id = currentMacro->getId();
	  for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
	    {
	      QString attribute  = *it2;
	      boldSelected(attributesTree, attribute, id, MACRO);
	    }
	}
    }
}

void EventGraphWidget::nextDataItem() 
{
  setComment();
  if (vectorPos != currentData.size() - 1) 
    {
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if(currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::black);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::black);
	  currentMacro->update();
	}
      vectorPos++;
      QString indexText = "(" + QString::number(vectorPos + 1) + "/"
	+ QString::number(currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if (currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::red);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::red);
	  currentMacro->update();
	}
      if (currentEvent) 
	{
	  selectedMacro = NULL;
	  seeComponentsButton->setEnabled(false);
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
	  if (query->isNull(0)) 
	    {
	      timeStampField->setText("Incident was deleted");
	      descriptionField->setText("Incident was deleted");
	      rawField->setText("Incident was deleted");
	      commentField->setText("Incident was deleted");
	      sourceField->setText("Incident was deleted");
	    }
	  else 
	    {
	      QString timeStamp = query->value(0).toString();
	      QString description = query->value(1).toString();
	      QString raw = query->value(2).toString();
	      QString comment = query->value(3).toString();
	      QString source = query->value(4).toString();
	      descriptionField->setText(description);
	      rawField->setText(raw);
	      commentField->setText(comment);
	      sourceField->setText(source);
	    }
	  resetFont(attributesTree);
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :id");
	  query->bindValue(":id", id);
	  query->exec();
	  while (query->next()) 
	    {
	      QString attribute = query->value(0).toString();
	      boldSelected(attributesTree, attribute, id, INCIDENT);
	    }
	  delete query;
	}
      else if (currentMacro) 
	{
	  selectedMacro = currentMacro;
	  selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentMacro->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentMacro->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
	  QString timestamp =  currentMacro->getTiming();
	  QString countText = QString::number(currentMacro->getIncidents().size());
	  timeStampField->setText(timestamp);
	  sourceField->setText(countText);
	  commentField->setText(currentMacro->getComment());
	  resetFont(attributesTree);
	  QSet<QString> attributes = currentMacro->getAttributes();
	  QSet<QString>::iterator it2;
	  id = currentMacro->getId();
	  for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
	    {
	      QString attribute  = *it2;
	      boldSelected(attributesTree, attribute, id, MACRO);
	    }
	}
    }
  else 
    {
      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if(currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::black);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::black);
	  currentMacro->update();
	}
      vectorPos = 0;
      QString indexText = "(" + QString::number(vectorPos + 1) + "/"
	+ QString::number(currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentEvent = qgraphicsitem_cast<EventItem*>(currentData.at(vectorPos));
      currentMacro = qgraphicsitem_cast<MacroEvent*>(currentData.at(vectorPos));
      if (currentEvent) 
	{
	  currentEvent->setSelectionColor(Qt::red);
	  currentEvent->update();
	}
      else if (currentMacro) 
	{
	  currentMacro->setSelectionColor(Qt::red);
	  currentMacro->update();
	}
      if (currentEvent) 
	{
	  selectedMacro = NULL;
	  seeComponentsButton->setEnabled(false);
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
	  if (query->isNull(0)) 
	    {
	      timeStampField->setText("Incident was deleted");
	      descriptionField->setText("Incident was deleted");
	      rawField->setText("Incident was deleted");
	      commentField->setText("Incident was deleted");
	      sourceField->setText("Incident was deleted");
	    }
	  else 
	    {
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
	      resetFont(attributesTree);
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT attribute FROM attributes_to_incidents "
			     "WHERE incident = :id");
	      query->bindValue(":id", id);
	      query->exec();
	      while (query->next()) 
		{
		  QString attribute = query->value(0).toString();
		  boldSelected(attributesTree, attribute, id, INCIDENT);
		}
	    }
	}
      else if (currentMacro) 
	{
	  selectedMacro = currentMacro;
	  selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentMacro->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentMacro->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
 	  QString timestamp =  currentMacro->getTiming();
	  QString countText = QString::number(currentMacro->getIncidents().size());
	  timeStampField->setText(timestamp);
	  sourceField->setText(countText);
	  commentField->setText(currentMacro->getComment());
	  resetFont(attributesTree);
	  QSet<QString> attributes = currentMacro->getAttributes();
	  QSet<QString>::iterator it2;
	  id = currentMacro->getId();
	  for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
	    {
	      QString attribute  = *it2;
	      boldSelected(attributesTree, attribute, id, MACRO);
	    }
	}
    }
}

void EventGraphWidget::setTree() 
{
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  // First we will fetch the 'normal' attributes.
  query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE'");
  while (query->next()) 
    {
      QString name = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *father = new QStandardItem(name);    
      attributesTree->appendRow(father);
      QString hint = breakString(description);
      father->setToolTip(hint);
      father->setEditable(false);
      buildHierarchy(father, name);
    }
  // And then we will also fetch the entities.
  QStandardItem *entities = new QStandardItem(ENTITIES);
  QString entitiesHint = breakString("You can also assign entities that you have created "
				     "in the relationships widget as attributes.");
  entities->setToolTip(entitiesHint);
  QFont font;
  font.setItalic(true);
  attributesTree->appendRow(entities);
  entities->setFont(font);
  query->exec("SELECT name, description FROM entities WHERE father = 'NONE'");
  int children = 0;
  while (query->next()) 
    {
      QString name = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *father = new QStandardItem(name);
      entities->setChild(children, father);
      children++;
      QString hint = breakString(description);
      father->setToolTip(hint);
      father->setEditable(false);
      buildEntities(father, name);
    }    
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void EventGraphWidget::buildHierarchy(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) 
    {
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


void EventGraphWidget::buildEntities(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entities WHERE father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) 
    {
      QString childName = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *child = new QStandardItem(childName);
      top->setChild(children, child);
      QString hint = breakString(description);
      child->setToolTip(hint);
      child->setEditable(false);
      children++;
      buildEntities(child, childName);
    }
  delete query;
}

void EventGraphWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QString currentName = model->data(index).toString();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
      QFont font;
      font.setBold(false);
      font.setUnderline(false);
      QFont font2;
      font2.setItalic(true);
      font2.setBold(false);
      font2.setUnderline(false);
      if (currentName != ENTITIES) 
	{
	  currentAttribute->setFont(font);
	}
      else 
	{
	  currentAttribute->setFont(font2);
	}
      if (model->hasChildren(index)) 
	{
	  resetFont(model, index);
	}
    }
}

void EventGraphWidget::boldSelected(QAbstractItemModel *model, QString name,
				    int event, QString type, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QString currentName = model->data(index).toString();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
      QFont font;
      font.setBold(true);
      QFont font2;
      font2.setUnderline(true);
      QFont font3;
      font3.setBold(true);
      font3.setUnderline(true);
      QFont font4;
      font4.setItalic(true);
      QFont font5;
      font5.setItalic(true);
      font5.setUnderline(true);
      if (name != ENTITIES) 
	{
	  if (name == currentName) 
	    {
	      if (currentAttribute->font().underline()) 
		{
		  currentAttribute->setFont(font3);
		}
	      else 
		{
		  currentAttribute->setFont(font);
		}
	      if (currentAttribute->parent()) 
		{
		  while (currentAttribute->parent()) 
		    {
		      currentAttribute = currentAttribute->parent();
		      QString parentName = currentAttribute->data(Qt::DisplayRole).toString();
		      if (parentName != ENTITIES) 
			{
			  if (type == INCIDENT) 
			    {
			      QSqlQuery *query = new QSqlQuery;
			      query->prepare("SELECT attribute, incident FROM attributes_to_incidents "
					     "WHERE attribute = :attribute AND incident = :incident");
			      query->bindValue(":attribute", parentName);
			      query->bindValue(":incident", event);
			      query->exec();
			      query->first();
			      if (query->isNull(0)) 
				{
				  currentAttribute->setFont(font2);      
				}
			      else 
				{
				  currentAttribute->setFont(font3);
				}
			      delete query;
			    }
			  else if (type == MACRO) 
			    {
			      QSet<QString> attributes = selectedMacro->getAttributes();
			      QSet<QString>::iterator it;
			      bool found = false;
			      for (it = attributes.begin(); it != attributes.end(); it++) 
				{
				  QString current = *it;
				  if (current == parentName) 
				    {
				      found = true;
				    }
				}
			      if (!found) 
				{
				  currentAttribute->setFont(font2);      
				}
			      else 
				{
				  currentAttribute->setFont(font3);
				}
			    }
			}
		      else 
			{
			  currentAttribute->setFont(font5);
			}
		    }
		}
	    }
	}
      else 
	{
	  currentAttribute->setFont(font4);
	}
      if (model->hasChildren(index)) 
	{
	  boldSelected(model, name, event, type, index);
	}
    }
}

void EventGraphWidget::assignAttribute() 
{
  int barPos = rawField->verticalScrollBar()->value();
  if (selectedIncident != 0) 
    {
      if (attributesTreeView->currentIndex().isValid()) 
	{
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
	  if (empty) 
	    {
	      query->prepare("INSERT INTO attributes_to_incidents (attribute, incident) "
			     "VALUES (:attribute, :incident)");
	      query->bindValue(":attribute", attribute);
	      query->bindValue(":incident", selectedIncident);
	      query->exec();
	      sourceAttributeText(attribute, selectedIncident);
	      boldSelected(attributesTree, attribute, selectedIncident, INCIDENT);
	      valueField->setEnabled(true);
	    }
	  else 
	    {
	      sourceAttributeText(attribute, selectedIncident);
	      highlightText();
	      rawField->setTextCursor(cursPos);
	    }
	  delete query;
	  setButtons();
	}
    }
  else if (selectedMacro != NULL) 
    {
      if (attributesTreeView->currentIndex().isValid()) 
	{
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  QSet<QString> attributes = selectedMacro->getAttributes();
	  if (!attributes.contains(attribute)) 
	    {
	      selectedMacro->insertAttribute(attribute);
	      boldSelected(attributesTree, attribute, selectedMacro->getId(), MACRO);
	      valueField->setEnabled(true);
	    }
	}
      setButtons();
    }
  rawField->verticalScrollBar()->setValue(barPos);
  occurrenceGraph->checkCongruency();
}

void EventGraphWidget::unassignAttribute() 
{
  if (selectedIncident != 0) 
    {
      if (attributesTreeView->currentIndex().isValid()) 
	{
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
	  if (!empty) 
	    {
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
	      while (query2->next()) 
		{
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
    }
  else if (selectedMacro != NULL) 
    {
      if (attributesTreeView->currentIndex().isValid()) 
	{
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  QSet<QString> attributes = selectedMacro->getAttributes();
	  if (attributes.contains(attribute)) 
	    {
	      selectedMacro->removeAttribute(attribute);
	      QSet<QString>::iterator it;
	      resetFont(attributesTree);
	      attributes = selectedMacro->getAttributes();
	      for (it = attributes.begin(); it != attributes.end(); it++) 
		{
		  QString current = *it;
		  boldSelected(attributesTree, current, selectedMacro->getId(), MACRO);	  
		}
	      setButtons();
	      valueField->setText("");
	      valueField->setEnabled(false);
	      valueButton->setEnabled(false);
	    }
	}
    }
  setButtons();
  occurrenceGraph->checkCongruency();
}

void EventGraphWidget::selectText() 
{
  if (rawField->textCursor().selectedText().trimmed() != "") 
    {
      int end = 0;
      int begin = 0;
      QTextCursor selectCursor = rawField->textCursor();
      if (rawField->textCursor().anchor() >= rawField->textCursor().position()) 
	{
	  begin = rawField->textCursor().position();
	  end = rawField->textCursor().anchor();
	}
      else 
	{
	  begin = rawField->textCursor().anchor();
	  end = rawField->textCursor().position();
	}
      selectCursor.setPosition(begin);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      rawField->setTextCursor(selectCursor);
      while (rawField->textCursor().selectedText()[0].isSpace()) 
	{
	  begin++;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  rawField->setTextCursor(selectCursor);
	}
      while (rawField->textCursor().selectedText()[rawField->textCursor().
						   selectedText().length() - 1].isSpace()) 
	{
	  end--;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  rawField->setTextCursor(selectCursor);
	}
      selectCursor.setPosition(begin);
      selectCursor.movePosition(QTextCursor::StartOfWord);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      if (!rawField->toPlainText()[end].isPunct()) 
	{
	  selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	}
      rawField->setTextCursor(selectCursor);
    }
}

void EventGraphWidget::sourceAttributeText(const QString &attribute, const int &incident) 
{
  if (rawField->textCursor().selectedText().trimmed() != "") 
    {
      QString sourceText = rawField->textCursor().selectedText().trimmed();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute FROM attributes_to_incidents_sources "
		     "WHERE attribute = :att AND inc = :incident AND source_text = :text");
      query->bindValue("att", attribute);
      query->bindValue("inc", incident);
      query->bindValue("text", sourceText);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO attributes_to_incidents_sources (attribute, incident, source_text)"
			 "VALUES (:att, :inc, :text)");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", incident);
	  query->bindValue(":text", sourceText);
	  query->exec();
	}
      delete query;
    }
}

void EventGraphWidget::removeText() 
{
  if (selectedIncident != 0) 
    {
      if (attributesTreeView->currentIndex().isValid()) 
	{
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  if (rawField->textCursor().selectedText().trimmed() != "") 
	    {
	      QString sourceText = rawField->textCursor().selectedText().trimmed();
	      QSqlQuery *query = new QSqlQuery;
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

void EventGraphWidget::resetTexts() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      if (selectedIncident != 0) 
	{
	  QPointer<QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->addButton(QMessageBox::Yes);
	  warningBox->addButton(QMessageBox::No);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<h2>Are you sure?</h2>");
	  warningBox->setInformativeText("Resetting source texts cannot be undone. "
					 "Are you sure you want to proceed?");
	  if (warningBox->exec() == QMessageBox::Yes) 
	    {
	      QSqlQuery *query = new QSqlQuery;
	      if (!(query->isNull(0))) 
		{
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

void EventGraphWidget::setButtons() 
{
  valueButton->setEnabled(false);
  valueField->setEnabled(false);
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->blockSignals(false);
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString currentAttribute = attributesTreeView->currentIndex().data().toString();
      if (selectedIncident != 0) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  bool empty = false;
	  query->prepare("SELECT attribute, incident, value FROM "
			 "attributes_to_incidents "
			 "WHERE attribute = :att AND incident = :inc  ");
	  query->bindValue(":att", currentAttribute);
	  query->bindValue(":inc", selectedIncident);
	  query->exec();
	  query->first();
	  empty = query->isNull(0);
	  if (!empty) 
	    {
	      unassignAttributeButton->setEnabled(true);
	      valueField->setEnabled(true);
	      if (!query->isNull(2))
		{
		  valueField->blockSignals(true);
		  valueField->setText(query->value(2).toString());
		  valueField->blockSignals(false);
		}
	    }
	  else 
	    {
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
	  if (!empty) 
	    {
	      removeTextButton->setEnabled(true);
	      resetTextsButton->setEnabled(true);
	    }
	  else 
	    {
	      removeTextButton->setEnabled(false);
	      resetTextsButton->setEnabled(false);
	    }
	  if (currentAttribute != ENTITIES) 
	    {
	      assignAttributeButton->setEnabled(true);
	    }
	  else 
	    {
	      assignAttributeButton->setEnabled(false);
	    }
	  delete query;
	}
      else if (selectedMacro != NULL) 
	{
	  QString currentAttribute = attributesTreeView->currentIndex().data().toString();
	  QSet<QString> attributes = selectedMacro->getAttributes();
	  QMap<QString, QString> values = selectedMacro->getValues();
	  if (attributes.contains(currentAttribute)) 
	    {
	      unassignAttributeButton->setEnabled(true);
	      valueField->setEnabled(true);
	      QString currentValue = values[currentAttribute];
	      if (currentValue != "")
		{
		  valueField->blockSignals(true);
		  valueField->setText(currentValue);
		  valueField->blockSignals(false);
		}
	    }
	  else 
	    {
	      unassignAttributeButton->setEnabled(false);
	    }
	  removeTextButton->setEnabled(false);
	  resetTextsButton->setEnabled(false);
	  if (currentAttribute != ENTITIES) 
	    {
	      assignAttributeButton->setEnabled(true);
	    }
	  else 
	    {
	      assignAttributeButton->setEnabled(false);
	    }
	  removeTextButton->setEnabled(false);
	  resetTextsButton->setEnabled(false);
	}
      if (currentAttribute != ENTITIES) 
	{
	  editAttributeButton->setEnabled(true);
	}
      else 
	{
	  editAttributeButton->setEnabled(false);
	}
    }
  else 
    {
      assignAttributeButton->setEnabled(false);
      editAttributeButton->setEnabled(false);
      unassignAttributeButton->setEnabled(false);
    }
}

void EventGraphWidget::removeUnusedAttributes() 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  QSet<QString> takenAttributes;
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) 
    {
      MacroEvent* current = it.next();
      QSet<QString> attributes = current->getAttributes();
      QSet<QString>::iterator it2;
      for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
	{
	  takenAttributes.insert(*it2);
	}
    }
  while (unfinished) 
    {
      query->exec("SELECT name FROM incident_attributes "
		  "EXCEPT SELECT attribute FROM attributes_to_incidents "
		  "EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_macro_events "
		  "EXCEPT SELECT father FROM incident_attributes");
      QSet<QString> temp;
      while (query->next()) 
	{
	  QString current = query->value(0).toString();			   
	  temp.insert(current);
	}
      QSet<QString>::iterator it3;
      bool found = false;
      for (it3 = temp.begin(); it3 != temp.end(); it3++) 
	{
	  if (!takenAttributes.contains(*it3)) 
	    {
	      found = true;
	      query2->prepare("DELETE FROM incident_attributes WHERE name = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	    }
	}
      if (!found) 
	{
	  unfinished = false;
	}
    }
  unfinished =  true;
  while (unfinished) 
    {
      query->exec("SELECT name FROM entities "
		  "EXCEPT SELECT source FROM entity_relationships "
		  "EXCEPT SELECT target FROM entity_relationships "
		  "EXCEPT SELECT attribute FROM attributes_to_incidents "
		  "EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_macro_events "
		  "EXCEPT SELECT attribute FROM saved_og_plots_occurrence_items "
		  "EXCEPT SELECT father FROM entities");
      QSet<QString> temp;
      while (query->next()) 
	{
	  QString current = query->value(0).toString();
	  temp.insert(current);
	}
      QSet<QString>::iterator it3;
      bool found = false;
      for (it3 = temp.begin(); it3 != temp.end(); it3++) 
	{
	  if (!takenAttributes.contains(*it3)) 
	    {
	      found = true;
	      query2->prepare("DELETE FROM entities WHERE name = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_entities WHERE entity = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents WHERE attribute = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents_sources WHERE attribute = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	    }
	}
      if (!found) 
	{
	  unfinished = false;
	}
    }
  this->setCursor(Qt::WaitCursor);
  attributesTreeView->setSortingEnabled(false);
  resetTree();
  attributesWidget->resetTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
  delete query;  
  delete query2;
  occurrenceGraph->checkCongruency();
}

void EventGraphWidget::highlightText() 
{
  int barPos = rawField->verticalScrollBar()->value();
  if (selectedIncident != 0) 
    {
      QTextCursor currentPos = rawField->textCursor();
      if (attributesTreeView->currentIndex().isValid()) 
	{
	  QStandardItem *currentAttribute = attributesTree->
	    itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
	  QString currentName = attributesTreeView->currentIndex().data().toString();
	  if (currentAttribute->font().bold()) 
	    {
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
	      while (query->next()) 
		{
		  QString currentText = query->value(0).toString();
		  QVector<QString> blocks = splitLines(currentText);
		  QVectorIterator<QString> it(blocks);
		  while (it.hasNext()) 
		    {
		      QString currentLine = it.next();
		      while (rawField->find(currentLine, QTextDocument::FindWholeWords)) 
			{
			  format.setFontWeight(QFont::Bold);
			  format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
			  format.setUnderlineColor(Qt::blue);
			  rawField->textCursor().mergeCharFormat(format);
			}
		    }
		  cursor = rawField->textCursor();
		  cursor.movePosition(QTextCursor::Start);
		  rawField->setTextCursor(cursor);
		}
	      rawField->setTextCursor(currentPos);
	      delete query;
	    }
	  else 
	    {
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
	}
      else 
	{
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
  rawField->verticalScrollBar()->setValue(barPos);
}

void EventGraphWidget::fixTree() 
{
  resetFont(attributesTree);
  if (selectedIncident != 0) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute FROM attributes_to_incidents "
		     "WHERE incident = :id");
      query->bindValue(":id", selectedIncident);
      query->exec();
      while (query->next()) 
	{
	  QString attribute = query->value(0).toString();
	  boldSelected(attributesTree, attribute);
	}
      delete query;
    }
  else if (selectedMacro != NULL) 
    {
      QSet<QString> attributes = selectedMacro->getAttributes();
      QSet<QString>::iterator it;
      for (it = attributes.begin(); it != attributes.end(); it++) 
	{
	  boldSelected(attributesTree, *it);
	}
    }
}

void EventGraphWidget::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void EventGraphWidget::newAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString currentParent = treeFilter->
	mapToSource(attributesTreeView->currentIndex()).data().toString();
      QString name = "";
      QString description = "";
      QModelIndex tempIndex = attributesTreeView->currentIndex();
      while (tempIndex.parent().isValid()) 
	{
	  tempIndex = tempIndex.parent();
	}
      QString top = tempIndex.data().toString();
      if (top == ENTITIES) 
	{
	  EntityDialog *entityDialog = new EntityDialog(this);
	  entityDialog->setRelationshipsWidget(relationshipsWidget);
	  entityDialog->setNew();
	  entityDialog->exec();
	  if (entityDialog->getExitStatus() == 0) 
	    {
	      QString name = entityDialog->getName();
	      QString description = entityDialog->getDescription();
	      QStandardItem *attribute = new QStandardItem(name);    
	      attribute->setToolTip(description);
	      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
	      QStandardItem *father = attributesTree->
		itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
	      father->appendRow(attribute);
	      attribute->setToolTip(hint);
	      attribute->setEditable(false);
	      QString fatherName = currentParent;
	      if (fatherName == ENTITIES) 
		{
		  fatherName = "NONE";
		}
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("INSERT INTO entities (name, description, father) "
			     "VALUES (:name, :description, :father)");
	      query->bindValue(":name", name);
	      query->bindValue(":description", description);
	      query->bindValue(":father", fatherName);
	      query->exec();
	      delete query;
	    }
	  delete entityDialog;
	  attributesWidget->resetTree();
	}
      else 
	{
	  QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
	  attributeDialog->exec();
	  if (attributeDialog->getExitStatus() == 0) 
	    {
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
	      query->prepare("INSERT INTO incident_attributes (name, description, father) "
			     "VALUES (:name, :description, :father)");
	      query->bindValue(":name", name);
	      query->bindValue(":description", description);
	      query->bindValue(":father", currentParent);
	      query->exec();
	      delete query;
	      attributesWidget->resetTree();
	    }
	  delete attributeDialog;
	}
    }
  else 
    {
      QString name = "";
      QString description = "";
      QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
      attributeDialog->exec();
      if (attributeDialog->getExitStatus() == 0) 
	{
	  name = attributeDialog->getName();
	  description = attributeDialog->getDescription();
	  QString currentParent = "NONE";
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("INSERT INTO incident_attributes (name, description, father) "
			 "VALUES (:name, :description, :father)");
	  query->bindValue(":name", name);
	  query->bindValue(":description", description);
	  query->bindValue(":father", currentParent);
	  query->exec();
	  QStandardItem *attribute = new QStandardItem(name);    
	  attributesTree->appendRow(attribute);
	  QString hint = breakString(description);
	  attribute->setToolTip(hint);
	  attribute->setEditable(false);
	  attributesWidget->resetTree();
	}
      delete attributeDialog;
    }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void EventGraphWidget::editAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString name = attributesTreeView->currentIndex().data().toString();
      if (name != ENTITIES) 
	{
	  QModelIndex tempIndex = attributesTreeView->currentIndex();
	  while (tempIndex.parent().isValid()) 
	    {
	      tempIndex = tempIndex.parent();
	    }
	  QString top = tempIndex.data().toString();
	  if (top == ENTITIES) 
	    {
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT description FROM entities WHERE name = :name");
	      query->bindValue(":name", name);
	      query->exec();
	      query->first();
	      QString description = query->value(0).toString();
	      EntityDialog *entityDialog = new EntityDialog(this);
	      entityDialog->setRelationshipsWidget(relationshipsWidget);
	      entityDialog->submitName(name);
	      entityDialog->submitDescription(description);
	      entityDialog->exec();
	      if (entityDialog->getExitStatus() == 0) 
		{
		  QString newName = entityDialog->getName();
		  description = entityDialog->getDescription();
		  QStandardItem *currentAttribute = attributesTree->
		    itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
		  currentAttribute->setData(newName);
		  currentAttribute->setData(newName, Qt::DisplayRole);
		  QString hint = breakString(description);
		  currentAttribute->setToolTip(hint);
		  updateEntityAfterEdit(newName, description, name);
		}
	      delete query;
	      delete entityDialog;
	    }
	  else 
	    {
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
	      if (attributeDialog->getExitStatus() == 0) 
		{
		  QString newName = attributeDialog->getName();
		  description = attributeDialog->getDescription();
		  QStandardItem *currentAttribute = attributesTree->
		    itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
		  currentAttribute->setData(newName);
		  currentAttribute->setData(newName, Qt::DisplayRole);
		  QString hint = breakString(description);
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
		  query->bindValue(":newname", newName);
		  query->bindValue(":oldname", name);
		  query->exec();
		  this->setCursor(Qt::WaitCursor);
		  retrieveData();
		  this->setCursor(Qt::ArrowCursor);
		  attributesWidget->resetTree();
		}
	      delete query;
	      delete attributeDialog;
	    }
	}
      attributesTree->sort(0, Qt::AscendingOrder);
      attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
    }
}

void EventGraphWidget::updateEntityAfterEdit(const QString name,
					     const QString description,
					     const QString former) 
{
  QSqlQuery *query = new QSqlQuery;
  // Update the entity itself.
  query->prepare("UPDATE entities "
		 "SET name = :name, description = :description "
		 "WHERE name = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();
  // Update the parent entities
  query->prepare("UPDATE entities "
		 "SET father = :name, description = :description "
		 "WHERE father = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();
  // Update attributes.
  query->prepare("UPDATE attributes_to_entities "
		 "SET entity = :new "
		 "WHERE entity = :old");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();
  // Update entities assigned as attributes
  query->prepare("UPDATE attributes_to_incidents "
		 "SET attribute = :new "
		 "WHERE attribute = :old ");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();
  /*
    Next up are the relationships in which the entity already participates.
    First, let us update all the relationships where the current entity is a source.
  */
  if (name != former) 
    {
      query->prepare("SELECT name, target, type "
		     "FROM entity_relationships WHERE source = :current");
      query->bindValue(":current", former);
      query->exec();
      while (query->next()) 
	{
	  QString oldRelationship = query->value(0).toString();
	  QString target = query->value(1).toString();
	  QString type = query->value(2).toString();
	  QSqlQuery *query2 = new QSqlQuery();
	  query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
	  query2->bindValue(":type", type);
	  query2->exec();
	  query2->first();
	  QString directedness = query2->value(0).toString();
	  QString arrow = "";
	  if (directedness == UNDIRECTED) 
	    {
	      arrow = "<-->";
	    }
	  else if (directedness == DIRECTED) 
	    {
	      arrow = "--->";
	    }
	  QString newRelationship = name + arrow + target;
	  query2->prepare("UPDATE entity_relationships "
			  "SET source = :source, name = :name "
			  "WHERE source = :oldSource AND name = :oldRelationship");
	  query2->bindValue(":source", name);
	  query2->bindValue(":name", newRelationship);
	  query2->bindValue(":oldSource", former);
	  query2->bindValue(":oldRelationship", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents_sources "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  delete query2;
	}
      // And then the relationships where the entity is a target.
      query->prepare("SELECT name, source, type "
		     "FROM entity_relationships WHERE target = :current");
      query->bindValue(":current", former);
      query->exec();
      while (query->next()) 
	{
	  QString oldRelationship = query->value(0).toString();
	  QString source = query->value(1).toString();
	  QString type = query->value(2).toString();
	  QSqlQuery *query2 = new QSqlQuery();
	  query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
	  query2->bindValue(":type", type);
	  query2->exec();
	  query2->first();
	  QString directedness = query2->value(0).toString();
	  QString arrow = "";
	  if (directedness == UNDIRECTED) 
	    {
	      arrow = "<-->";
	    }
	  else if (directedness == DIRECTED) 
	    {
	      arrow = "--->";
	    }
	  QString newRelationship = source + arrow + name;
	  query2->prepare("UPDATE entity_relationships "
			  "SET target = :target, name = :name "
			  "WHERE target = :oldTarget AND name = :oldRelationship");
	  query2->bindValue(":target", name);
	  query2->bindValue(":name", newRelationship);
	  query2->bindValue(":oldTarget", former);
	  query2->bindValue(":oldRelationship", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents_sources "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  delete query2;
	}
      delete query;
    }
  /* 
     If an entity is edited, we should communicate this change both to the
     relationships widget and the attributes widget. The tree of the Hierachy Graph Widget
     is reconstructed every time it is switched to, so we do not need to reset it
     explicitly.
  */
  relationshipsWidget->resetTree();
  attributesWidget->resetTree();
}

void EventGraphWidget::getEvents() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT ch_order, description FROM incidents ORDER BY ch_order");
  while (query->next()) 
    {
      int order = query->value(0).toInt();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT id FROM incidents WHERE ch_order = :order");
      query2->bindValue(":order", order);
      query2->exec();
      query2->first();
      int id = query2->value(0).toInt();
      QString toolTip = breakString(query->value(1).toString());
      qreal vertical = qrand() % 3000 - 1500;
      QPointF position = QPointF((order * distance), vertical);
      EventItem *currentItem = new EventItem(40, toolTip, position, id, order);
      currentItem->setPos(currentItem->getOriginalPos());
      eventVector.push_back(currentItem);
      delete query2;
    }
  delete query;
}

void EventGraphWidget::plotEvents() 
{
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
      EventItem *currentItem = it.next();
      currentItem->setZValue(3);
      scene->addItem(currentItem);
    }
}

void EventGraphWidget::getEdges(QString coder, QString type, QColor color) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", coder);
  query->bindValue(":type", type);
  query->exec();
  while (query->next()) 
    {
      int tail = query->value(0).toInt();
      int head = query->value(1).toInt();
      QSqlQuery *query2 =  new QSqlQuery;
      query2->prepare("SELECT ch_order from incidents WHERE id = :id");
      query2->bindValue(":id", tail);
      query2->exec();
      query2->first();
      int tailOrder = query2->value(0).toInt();
      query2->prepare("SELECT ch_order from incidents WHERE id = :id");
      query2->bindValue(":id", head);
      query2->exec();
      query2->first();
      int headOrder = query2->value(0).toInt();
      query2->prepare("SELECT comment, coder FROM linkage_comments "
		      "WHERE tail = :tail AND head = :head AND type = :type");
      query2->bindValue(":tail", tailOrder);
      query2->bindValue(":head", headOrder);
      query2->bindValue(":type", type);
      query2->exec();
      query2->first();
      QString comment = "";
      QString commentCoder = "";
      QString toolTip = "";
      if (!(query2->isNull(0))) 
	{
	  comment = query2->value(0).toString();
	  commentCoder = query2->value(1).toString();
	  toolTip = breakString(commentCoder + " - " + comment);
	}
      else 
	{
	  toolTip = "No comments";
	}
      QVectorIterator<EventItem*> it(eventVector);
      EventItem *tempSource = NULL;
      EventItem *tempTarget = NULL;
      while (it.hasNext()) 
	{
	  EventItem *currentItem = it.next();
	  if (currentItem->getId() == tail) 
	    {
	      tempSource = currentItem;
	    }
	  else if (currentItem->getId() == head) 
	    {
	      tempTarget = currentItem;
	    }
	  if (tempSource != NULL && tempTarget != NULL) 
	    {
	      if (tempSource->getOrder() < tempTarget->getOrder()) 
		{
		  Arrow *currentEdge = new Arrow(type, coder);
		  currentEdge->setZValue(2);
		  currentEdge->setStartItem(tempSource);
		  currentEdge->setEndItem(tempTarget);
		  currentEdge->setToolTip(toolTip);
		  currentEdge->setColor(color);
		  edgeVector.push_back(currentEdge);
		  break;
		}
	      else if (tempSource->getOrder() > tempTarget->getOrder()) 
		{
		  Arrow *currentEdge = new Arrow(type, coder);
		  currentEdge->setZValue(2);
		  currentEdge->setStartItem(tempSource);
		  currentEdge->setEndItem(tempTarget);
		  currentEdge->setToolTip(toolTip);
		  currentEdge->setColor(color);
		  edgeVector.push_back(currentEdge);
		  break;
		}
	    } 
	}
      delete query2;
    }
  delete query;
}

void EventGraphWidget::plotEdges(QString type) 
{
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) 
    {
      Arrow *currentEdge = it.next();
      if (currentEdge->getType() == type) 
	{
	  scene->addItem(currentEdge);
	}
    }
}

void EventGraphWidget::layoutGraph() 
{
  QVector<EventItem*>::iterator it4;
  for (it4 = eventVector.begin(); it4 != eventVector.end(); it4++) 
    {
      EventItem *current = *it4;
      QVector<EventItem*>::iterator it5;
      QVector<EventItem*> partners;
      for (it5 = it4 + 1; it5 != eventVector.end(); it5++) 
	{
	  EventItem *second = *it5;
	  QVectorIterator<Arrow*> it6(edgeVector);
	  while (it6.hasNext()) 
	    {
	      Arrow *edge = it6.next();
	      // Basically we just ask whether the current events are linked.
	      if ((edge->startItem() == current &&
		   edge->endItem() == second) ||
		  (edge->endItem() == current &&
		   edge->startItem() == second)) 
		{
		  partners.push_back(second);
		}
	    }
	}
      qreal originHeight = current->scenePos().y();
      std::sort(partners.begin(), partners.end(), eventLessThan);
      int partnerCount = partners.size();
      QVectorIterator<EventItem*> it7(partners);
      while (it7.hasNext()) 
	{
	  EventItem *currentPartner = it7.next();
	  qreal partnerHeight = originHeight;
	  if (partners.size() > 1) 
	    {
	      partnerHeight = originHeight + (pow(-1, partnerCount) * partnerCount * 50);
	    }
	  currentPartner->setPos(currentPartner->scenePos().x(), partnerHeight);
	  partnerCount--;
	}
    }
}

void EventGraphWidget::getLabels() 
{
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
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
      text->setZValue(4);
      text->setDefaultTextColor(Qt::black);
      nodeLabelVector.push_back(text);
      delete query;
    }
}

void EventGraphWidget::addLabels() 
{
  QVectorIterator<NodeLabel*> it2(nodeLabelVector);
  while (it2.hasNext()) 
    {
      NodeLabel *currentItem = it2.next();
      scene->addItem(currentItem);
    }
}

void EventGraphWidget::cleanUp() 
{
  scene->clearSelection();
  currentData.clear(); // Contents will be deleted below
  qDeleteAll(edgeVector);
  edgeVector.clear();
  qDeleteAll(macroVector);
  macroVector.clear();
  qDeleteAll(macroLabelVector);
  macroLabelVector.clear();
  qDeleteAll(eventVector);
  eventVector.clear();
  qDeleteAll(nodeLabelVector);
  nodeLabelVector.clear();
  qDeleteAll(compareVector);
  compareVector.clear();
  qDeleteAll(lineVector);
  lineVector.clear();
  qDeleteAll(textVector);
  textVector.clear();
  qDeleteAll(ellipseVector);
  ellipseVector.clear();
  qDeleteAll(rectVector);
  rectVector.clear();
  scene->clear();
  eventListWidget->setRowCount(0);
  linkageListWidget->setRowCount(0);
  presentTypes.clear();
  checkedCases.clear();
  selectedCoder = "";
  selectedCompare = "";
  selectedIncident = 0;
  selectedMacro = NULL;
  setGraphControls(false);
}

void EventGraphWidget::changePos(EventItem *item, qreal &dist) 
{
  QPointF original = item->scenePos();
  int order = original.x();
  NodeLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
      EventItem *currentItem = it.next();
      if (currentItem->pos().x() > order) 
	{
	  QPointF newPos = QPointF(currentItem->pos().x() + dist, currentItem->pos().y());
	  NodeLabel *currentLabel = currentItem->getLabel();
	  currentLabel->setNewPos(newPos);
	  currentItem->setPos(newPos);
	}
    }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent *currentItem = it2.next();
      if (currentItem->pos().x() > order) 
	{
	  QPointF newPos = QPointF(currentItem->pos().x() + dist, currentItem->pos().y());
	  MacroLabel *currentLabel = currentItem->getLabel();
	  currentLabel->setNewPos(newPos);
	  currentItem->setPos(newPos);
	}
    }
}

void EventGraphWidget::changePos(MacroEvent *item, qreal &dist) 
{
  QPointF original = item->scenePos();
  int order = original.x();
  MacroLabel *itemLabel = item->getLabel();
  itemLabel->setNewPos(original);
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
      EventItem *currentItem = it.next();
      if (currentItem->pos().x() > order) 
	{
	  QPointF newPos = QPointF(currentItem->pos().x() + dist, currentItem->pos().y());
	  NodeLabel *currentLabel = currentItem->getLabel();
	  currentLabel->setNewPos(newPos);
	  currentItem->setPos(newPos);
	}
    }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent *currentItem = it2.next();
      if (currentItem->pos().x() > order) 
	{
	  QPointF newPos = QPointF(currentItem->pos().x() + dist, currentItem->pos().y());
	  MacroLabel *currentLabel = currentItem->getLabel();
	  currentLabel->setNewPos(newPos);
	  currentItem->setPos(newPos);
	}
    }
}

void EventGraphWidget::increaseDistance() 
{
  QVector<QGraphicsItem*> temp;
  QVectorIterator<EventItem*> it(eventVector);
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it.hasNext()) 
    {
      temp.push_back(it.next());
    }
  while (it2.hasNext()) 
    {
      temp.push_back(it2.next());
    }
  std::sort(temp.begin(), temp.end(), eventLessThan);
  QVectorIterator<QGraphicsItem*> it3(temp);  
  int unit = 0;
  qreal last = -9999;
  if (distance < 300) 
    {
      while (it3.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(it3.peekNext());
	  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it3.peekNext());
	  if (event) 
	    {
	      event = qgraphicsitem_cast<EventItem*>(it3.next());
	      qreal oldX = event->scenePos().x();
	      if (last != oldX) 
		{
		  unit++;
		}
	      if (last == -9999) 
		{
		  last = event->scenePos().x();
		}
	      QPointF newPos = QPointF(oldX + unit, event->pos().y());
	      NodeLabel *currentLabel = event->getLabel();
	      currentLabel->setNewPos(newPos);
	      event->setOriginalPos(newPos);
	      event->setPos(newPos);
	      last = oldX;
	    }
	  else if (macro) 
	    {
	      macro = qgraphicsitem_cast<MacroEvent*>(it3.next());
	      qreal oldX = macro->scenePos().x();
	      if (last != oldX) 
		{
		  unit++;
		}
	      if (last == -9999) 
		{
		  last = macro->scenePos().x();
		}
	      QPointF newPos = QPointF(oldX + unit, macro->pos().y());
	      MacroLabel *currentLabel = macro->getLabel();
	      currentLabel->setNewPos(newPos);
	      macro->setOriginalPos(newPos);
	      macro->setPos(newPos);
	      last = oldX;
	    }
	}
      distance = distance + 1.0;
    }
}

void EventGraphWidget::decreaseDistance() 
{
  QVector<QGraphicsItem*> temp;
  QVectorIterator<EventItem*> it(eventVector);
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it.hasNext()) 
    {
      temp.push_back(it.next());
    }
  while (it2.hasNext()) 
    {
      temp.push_back(it2.next());
    }
  std::sort(temp.begin(), temp.end(), eventLessThan);
  QVectorIterator<QGraphicsItem*> it3(temp);  
  int unit = 0;
  qreal last = -9999;
  if (distance > 35) 
    {
      while (it3.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(it3.peekNext());
	  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it3.peekNext());
	  if (event) 
	    {
	      event = qgraphicsitem_cast<EventItem*>(it3.next());
	      qreal oldX = event->scenePos().x();
	      if (last != oldX) 
		{
		  unit++;
		}      
	      if (last == -9999) 
		{
		  last = event->scenePos().x();
		}      
	      QPointF newPos = QPointF(oldX - unit, event->pos().y());
	      NodeLabel *currentLabel = event->getLabel();
	      currentLabel->setNewPos(newPos);
	      event->setOriginalPos(newPos);
	      event->setPos(newPos);
	      last = oldX;
	    }
	  else if (macro) 
	    {
	      macro = qgraphicsitem_cast<MacroEvent*>(it3.next());
	      qreal oldX = macro->scenePos().x();
	      if (last != oldX) 
		{
		  unit++;
		}      
	      if (last == -9999) 
		{
		  last = macro->scenePos().x();
		}      
	      QPointF newPos = QPointF(oldX, macro->pos().y());
	      MacroLabel *currentLabel = macro->getLabel();
	      currentLabel->setNewPos(newPos);
	      macro->setOriginalPos(newPos);
	      macro->setPos(newPos);
	      last = oldX;
	    }
	}
      distance = distance - 1.0;
    }
}

void EventGraphWidget::expandGraph() 
{
  qreal virtualCenter = 0.0;
  int total = 0;
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
      EventItem *current = it.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent *current = it2.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  virtualCenter /= total;
  it.toFront();
  while (it.hasNext()) 
    {
      EventItem *current = it.next();
      qreal currentY = current->scenePos().y();
      qreal diffY  = (currentY - virtualCenter) * 1.1;
      current->setPos(current->scenePos().x(), virtualCenter + diffY);
      current->getLabel()->setNewPos(current->scenePos());
    }
  it2.toFront();
  while (it2.hasNext()) 
    {
      MacroEvent *current = it2.next();
      qreal currentY = current->scenePos().y();
      qreal diffY  = (currentY - virtualCenter) * 1.1;
      current->setPos(current->scenePos().x(), virtualCenter + diffY);
      current->getLabel()->setNewPos(current->scenePos());
    }
}

void EventGraphWidget::contractGraph() 
{
  qreal virtualCenter = 0.0;
  int total = 0;
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
      EventItem *current = it.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent *current = it2.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  virtualCenter /= total;
  it.toFront();
  while (it.hasNext()) 
    {
      EventItem *current = it.next();
      qreal currentY = current->scenePos().y();
      qreal diffY  = (currentY - virtualCenter) * 0.9;
      current->setPos(current->scenePos().x(), virtualCenter + diffY);
      current->getLabel()->setNewPos(current->scenePos());
    }
  it2.toFront();
  while (it2.hasNext()) 
    {
      MacroEvent *current = it2.next();
      qreal currentY = current->scenePos().y();
      qreal diffY  = (currentY - virtualCenter) * 0.9;
      current->setPos(current->scenePos().x(), virtualCenter + diffY);
      current->getLabel()->setNewPos(current->scenePos());
    }
}

void EventGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) 
{
  if (currentData.size() > 0) 
    {
      QGraphicsItem *source = NULL;
      QVectorIterator<QGraphicsItem*> it(currentData);
      while (it.hasNext()) 
	{
	  QGraphicsItem *temp = it.next();
	  if (temp == item) 
	    {
	      source = temp;
	    }
	}
      if (source != NULL) 
	{
	  qreal currentY = source->scenePos().y();
	  qreal newY = pos.y();
	  qreal yDiff = newY - currentY;
	  QVectorIterator<QGraphicsItem*> it2(currentData);
	  while (it2.hasNext()) 
	    {
	      QGraphicsItem *current = it2.next();
	      current->setPos(current->scenePos().x(), current->scenePos().y() + yDiff);
	      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(current);
	      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(current);
	      if (currentEvent) 
		{
		  currentEvent->getLabel()->setNewPos(currentEvent->scenePos());
		}
	      else if (currentMacro) 
		{
		  currentMacro->getLabel()->setNewPos(currentMacro->scenePos());
		}
	    }
	}
    }
}

void EventGraphWidget::setPlotButtons() 
{
  if (typeComboBox->currentText() != DEFAULT && coderComboBox->currentText() != DEFAULT) 
    {
      plotButton->setEnabled(true);
    }
  else 
    {
      plotButton->setEnabled(false);
    }
  if (coderComboBox->currentText() == selectedCoder &&
      !presentTypes.contains(typeComboBox->currentText())) 
    {
      if (macroVector.size() > 0) 
	{
	  addLinkageTypeButton->setEnabled(false);
	}
      else 
	{
	  addLinkageTypeButton->setEnabled(true);
	}
    }
  else 
    {
      addLinkageTypeButton->setEnabled(false);
    }
  if (coderComboBox->currentText() != DEFAULT &&
      presentTypes.contains(typeComboBox->currentText())) 
    {
      compareComboBox->clear();
      compareComboBox->addItem(DEFAULT);
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT name FROM coders");
      while (query->next()) 
	{
	  QString currentCoder = query->value(0).toString();
	  if (currentCoder != coderComboBox->currentText()) 
	    {
	      compareComboBox->addItem(currentCoder);
	    }
	}
      delete query;
    }
}

void EventGraphWidget::getLinkageDetails() 
{
  coderComboBox->clear();
  coderComboBox->addItem(DEFAULT);
  typeComboBox->clear();
  typeComboBox->addItem(DEFAULT);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM coders");
  while (query->next()) 
    {
      QString currentCoder = query->value(0).toString();
      coderComboBox->blockSignals(true);
      coderComboBox->addItem(currentCoder);
      coderComboBox->blockSignals(false);
    }
  query->exec("SELECT name FROM linkage_types");
  while (query->next()) 
    {
      QString currentType = query->value(0).toString();
      typeComboBox->addItem(currentType);
    }
  delete query;
}

void EventGraphWidget::plotGraph() 
{
  cleanUp();
  selectedCoder = coderComboBox->currentText();
  QString currentType = typeComboBox->currentText();
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  QColor color = QColor(Qt::black);
  colorDialog->setCurrentColor(color);
  if (colorDialog->exec()) 
    {
      color = colorDialog->selectedColor();
    }
  else 
    {
      return;
    }
  QApplication::setOverrideCursor(Qt::WaitCursor);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT description, direction FROM linkage_types "
		 "WHERE name = :name");
  query->bindValue(":name", currentType);
  query->exec();
  query->first();
  QString description = query->value(0).toString();
  QString direction = query->value(1).toString();
  presentTypes.push_back(currentType);
  QTableWidgetItem *item = new QTableWidgetItem(currentType);
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  QString toolTip = breakString(currentType + " (" + direction + ") - " + description);
  item->setToolTip(toolTip);
  item->setData(Qt::DisplayRole, currentType);
  linkageListWidget->setRowCount(linkageListWidget->rowCount() + 1);
  linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 0, item);
  linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 1, new QTableWidgetItem);
  linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->setBackground(color);
  linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->
    setFlags(linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->flags() ^
	     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
  if (coderComboBox->currentText() != DEFAULT &&
      presentTypes.contains(typeComboBox->currentText())) 
    {
      compareComboBox->clear();
      compareComboBox->addItem(DEFAULT);
      query->exec("SELECT name FROM coders");
      while (query->next()) 
	{
	  QString currentCoder = query->value(0).toString();
	  if (currentCoder != coderComboBox->currentText()) 
	    {
	      compareComboBox->addItem(currentCoder);
	    }
	}
    }
  delete query;
  distance = 70;
  getEvents();
  plotEvents(); // Should allow for range to be set here.
  getEdges(selectedCoder, currentType, color);
  plotEdges(currentType);
  layoutGraph();
  getLabels();
  addLabels();
  compareComboBox->setCurrentIndex(0);
  savePlotButton->setEnabled(true);
  updateCases();
  caseListWidget->setEnabled(true);
  setRangeControls();
  plotLabel->setText("Unsaved plot");
  checkCongruency();
  updateLinkages();
  setGraphControls(true);
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void EventGraphWidget::addLinkageType() 
{
  QString currentType = typeComboBox->currentText();
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  QColor color = QColor(Qt::black);
  colorDialog->setCurrentColor(color);
  if (colorDialog->exec()) 
    {
      color = colorDialog->selectedColor();
    }
  else 
    {
      return;
    }
  QApplication::setOverrideCursor(Qt::WaitCursor);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT description, direction FROM linkage_types "
		 "WHERE name = :name");
  query->bindValue(":name", currentType);
  query->exec();
  query->first();
  QString description = query->value(0).toString();
  QString direction = query->value(1).toString();
  presentTypes.push_back(currentType);
  QTableWidgetItem *item = new QTableWidgetItem(currentType);
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  QString toolTip = breakString(currentType + " (" + direction + ") - " + description);
  item->setToolTip(toolTip);
  item->setData(Qt::DisplayRole, currentType);
  linkageListWidget->setRowCount(linkageListWidget->rowCount() + 1);
  linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 0, item);
  linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 1, new QTableWidgetItem);
  linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->setBackground(color);
  linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->
    setFlags(linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->flags() ^
	     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
  if (coderComboBox->currentText() != DEFAULT &&
      presentTypes.contains(typeComboBox->currentText())) 
    {
      compareComboBox->clear();
      compareComboBox->addItem(DEFAULT);
      query->exec("SELECT name FROM coders");
      while (query->next()) 
	{
	  QString currentCoder = query->value(0).toString();
	  if (currentCoder != coderComboBox->currentText()) 
	    {
	      compareComboBox->addItem(currentCoder);
	    }
	}
    }
  delete query;
  getEdges(selectedCoder, currentType, color);
  plotEdges(currentType);
  setHeights();
  checkCongruency();
  updateLinkages();
  addLinkageTypeButton->setEnabled(false);
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void EventGraphWidget::setCompareButton() 
{
  if (presentTypes.size() > 0) 
    {
      if (compareComboBox->currentText() != DEFAULT && selectedCoder != "") 
	{
	  if (macroVector.size() > 0) 
	    {
	      compareButton->setEnabled(false);
	    }
	  else 
	    {
	      compareButton->setEnabled(true);
	    }
	}
      else 
	{
	  compareButton->setEnabled(false);
	}
    }
  else 
    {
      compareButton->setEnabled(false);
    }
}


void EventGraphWidget::compare() 
{
  selectedCompare = compareComboBox->currentText();
  getCompareEdges(selectedCompare, presentTypes[0]);
  plotCompareEdges();
  updateLinkages();
  compareButton->setEnabled(false);
}

void EventGraphWidget::getCompareEdges(QString coder, QString type) 
{
  qDeleteAll(compareVector);
  compareVector.clear();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", coder);
  query->bindValue(":type", type);
  query->exec();
  while (query->next()) 
    {
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
      if (!(query2->isNull(0))) 
	{
	  comment = query2->value(0).toString();
	  commentCoder = query2->value(1).toString();
	  toolTip = breakString(commentCoder + " - " + comment);
	}
      else 
	{
	  toolTip = "No comments";
	}
      QVectorIterator<EventItem*> it(eventVector);
      EventItem *tempSource = NULL;
      EventItem *tempTarget = NULL;
      while (it.hasNext()) 
	{
	  EventItem *currentItem = it.next();
	  if (currentItem->getId() == tail) 
	    {
	      tempSource = currentItem;
	    }
	  else if (currentItem->getId() == head) 
	    {
	      tempTarget = currentItem;
	    }
	  if (tempSource != NULL && tempTarget != NULL) 
	    {
	      bool sameFound = false;
	      QVectorIterator<Arrow*> it2(edgeVector);
	      QColor edgeColor = QColor(Qt::gray);
	      while (it2.hasNext()) 
		{
		  Arrow *currentEdge = it2.next();
		  edgeColor = currentEdge->getColor();
		  EventItem *tempS = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
		  EventItem *tempT = qgraphicsitem_cast<EventItem*>(currentEdge->endItem());
		  if (tempSource == tempS && tempTarget == tempT) 
		    {
		      currentEdge->setPenStyle(2);
		      sameFound = true;
		    }
		}
	      if (sameFound == false) 
		{
		  Arrow *compareEdge = new Arrow(type, coder);
		  compareEdge->setZValue(2);
		  compareEdge->setStartItem(tempSource);
		  compareEdge->setEndItem(tempTarget);
		  compareEdge->setPenStyle(4);
		  compareEdge->setColor(edgeColor);
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

void EventGraphWidget::plotCompareEdges() 
{
  QVectorIterator<Arrow*> it(compareVector);
  while (it.hasNext()) 
    {
      Arrow *currentEdge = it.next();
      scene->addItem(currentEdge);
    }
  QVectorIterator<Arrow*> it2(edgeVector);
  while (it2.hasNext()) 
    {
      Arrow *currentEdge = it2.next();
      if (currentEdge->getPenStyle() == 1) 
	{
	  currentEdge->setPenStyle(3);
	}
      else 
	{
	  currentEdge->setPenStyle(1);
	}
    }
}

void EventGraphWidget::saveCurrentPlot() 
{
  QPointer<SimpleTextDialog> saveDialog = new SimpleTextDialog(this);
  saveDialog->setWindowTitle("Save current plot");
  saveDialog->submitText(plotLabel->text());
  saveDialog->setLabel("Plot name:");
  saveDialog->exec();
  if (saveDialog->getExitStatus() == 0) 
    {
      QString name = saveDialog->getText();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT plot FROM saved_eg_plots WHERE plot = :name");
      query->bindValue(":name", name);
      query->exec();
      query->first();
      bool empty = false;
      if (query->isNull(0)) 
	{
	  empty = true;
	}
      else 
	{
	  QPointer<QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->addButton(QMessageBox::Yes);
	  warningBox->addButton(QMessageBox::No);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<h2>Plot exists</h2>");
	  warningBox->setInformativeText("A plot with this name already exists "
					 "Do you want to overwrite this plot?");
	  if (warningBox->exec() == QMessageBox::Yes) 
	    {
	      delete warningBox;
	    }
	  else 
	    {
	      delete warningBox;
	      return;
	    }
	}
      if (!empty) 
	{
	  // Reset plot name and details, and clear out all data before writing.
	  QColor color = scene->backgroundBrush().color();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  query->prepare("UPDATE saved_eg_plots SET coder = :coder, "
			 "red = :red, green = :green, blue = :blue "
			 "WHERE plot = :name");
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
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
	  // saved_eg_plots_legend
	  query->prepare("DELETE FROM saved_eg_plots_legend "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_eg_plots_embedded_incidents
	  query->prepare("DELETE FROM saved_eg_plots_embedded_incidents "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_eg_plots_macros_to_macros 
	  query->prepare("DELETE FROM saved_eg_plots_macros_to_macros "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_eg_plots_lines
	  query->prepare("DELETE FROM saved_eg_plots_lines "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_eg_plots_texts
	  query->prepare("DELETE FROM saved_eg_plots_texts "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_eg_plots_ellipses
	  query->prepare("DELETE FROM saved_eg_plots_ellipses "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_eg_plots_rects
	  query->prepare("DELETE FROM saved_eg_plots_rects "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	}
      else 
	{
	  // Insert new data into saved_eg_plots and then write data.
	  QColor color = scene->backgroundBrush().color();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  query->prepare("INSERT INTO saved_eg_plots (plot, coder, red, green, blue) "
			 "VALUES (:name, :coder, :red, :green, :blue)");
	  query->bindValue(":name", name);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->exec();
	}
      QSqlDatabase::database().transaction();
      query->prepare("INSERT INTO saved_eg_plots_event_items "
		     "(plot, incident, ch_order, width, curxpos, curypos, orixpos, oriypos, "
		     "dislodged, mode, red, green, blue, alpha, hidden) "
		     "VALUES (:plot, :incident, :order, :width, :curxpos, :curypos, :orixpos, "
		     ":oriypos, :dislodged, :mode, :red, :green, :blue, :alpha, :hidden)");
      QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, eventVector.size());
      saveProgress->setWindowTitle("Saving event items");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      int counter = 1;
      saveProgress->show();
      QVectorIterator<EventItem*> it(eventVector);
      while (it.hasNext()) 
	{
	  EventItem *currentItem = it.next();
	  qreal currentX = currentItem->pos().x();
	  qreal currentY = currentItem->pos().y();
	  qreal originalX = currentItem->getOriginalPos().x();
	  qreal originalY = currentItem->getOriginalPos().y();
	  int incident = currentItem->getId();
	  int order = currentItem->getOrder();
	  int width = currentItem->getWidth();
	  int dislodged = 0;
	  QString mode = currentItem->getMode();
	  QColor color = currentItem->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  int hidden = 1;
	  if (currentItem->isDislodged()) 
	    {
	      dislodged = 1;
	    }
	  if (currentItem->isVisible()) 
	    {
	      hidden = 0;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":incident", incident);
	  query->bindValue(":order", order);
	  query->bindValue(":width", width);
	  query->bindValue(":curxpos", currentX);
	  query->bindValue(":curypos", currentY);
	  query->bindValue(":orixpos", originalX);
	  query->bindValue(":oriypos", originalY);
	  query->bindValue(":dislodged", dislodged);
	  query->bindValue(":mode", mode);
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
      query->prepare("INSERT INTO saved_eg_plots_event_labels "
		     "(plot, incident, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden) "
		     "VALUES (:plot, :incident, :label, :curxpos, :curypos, "
		     ":xoffset, :yoffset, :red, :green, :blue, :alpha, :hidden)");
      QVectorIterator<NodeLabel*> it2(nodeLabelVector);
      while (it2.hasNext()) 
	{
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
	  if (currentLabel->isVisible()) 
	    {
	      hidden = 0;
	    }
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
      query->prepare("INSERT INTO saved_eg_plots_edges "
		     "(plot, tail, head, tailmacro, headmacro, linkage, "
		     "red, green, blue, alpha, hidden, masshidden) "
		     "VALUES (:plot, :tail, :head, :tmacro, :hmacro, :linkage, "
		     ":red, :green, :blue, :alpha, :hidden, :masshidden)");
      QVectorIterator<Arrow*> it3(edgeVector);
      while (it3.hasNext()) 
	{
	  Arrow *currentEdge = it3.next();
	  QColor color = currentEdge->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  EventItem *eventTail = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	  EventItem *eventHead = qgraphicsitem_cast<EventItem*>(currentEdge->endItem());
	  MacroEvent *macroTail = qgraphicsitem_cast<MacroEvent*>(currentEdge->startItem());
	  MacroEvent *macroHead = qgraphicsitem_cast<MacroEvent*>(currentEdge->endItem());
	  QString linkage = currentEdge->getType();
	  int tail = 0;
	  int head = 0;
	  int mTail = 0;
	  int mHead = 0;
	  int hidden = 1;
	  int massHidden = 0;
	  if (eventTail) 
	    {
	      tail = eventTail->getId();
	    }
	  else if (macroTail) 
	    {
	      tail = macroTail->getId();
	      mTail = 1;
	    }
	  if (eventHead) 
	    {
	      head = eventHead->getId();
	    }
	  else if (macroHead) 
	    {
	      head = macroHead->getId();
	      mHead = 1;
	    }
	  if (currentEdge->isVisible()) 
	    {
	      hidden = 0;
	    }
	  if (currentEdge->isMassHidden()) 
	    {
	      massHidden = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", head);
	  query->bindValue(":tmacro", mTail);
	  query->bindValue(":hmacro", mHead);
	  query->bindValue(":linkage", linkage);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":alpha", alpha);
	  query->bindValue(":hidden", hidden);
	  query->bindValue(":masshidden", massHidden);
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
      query->prepare("INSERT INTO saved_eg_plots_incidents_to_macro_events "
		     "(plot, incident, macro) "
		     "VALUES (:plot, :incident, :macro)");
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("INSERT INTO saved_eg_plots_attributes_to_macro_events "
		      "(plot, attribute, macro, value) "
		      "VALUES(:plot, :attribute, :macro, :value)");
      QSqlQuery *query3 = new QSqlQuery;
      query3->prepare("INSERT INTO saved_eg_plots_macro_events "
		      "(plot, eventid, ch_order, colligation, timing, description, comment, width, "
		      "mode, curxpos, curypos, orixpos, oriypos, dislodged, "
		      "red, green, blue, alpha, hidden) "
		      "VALUES (:plot, :eventid, :ch_order, :colligation, :timing, :description, "
		      ":comment, :width, :mode, :curxpos, :curypos, :orixpos, :oriypos, :dislodged, "
		      ":red, :green, :blue, :alpha, :hidden)");;
      QVectorIterator<MacroEvent*> it4(macroVector);
      while (it4.hasNext()) 
	{
	  MacroEvent *currentMacro = it4.next();
	  QVector<EventItem*> incidents = currentMacro->getIncidents();
	  QVectorIterator<EventItem*> tit(incidents);
	  while (tit.hasNext()) 
	    {
	      EventItem* currentIncident = tit.next();
	      query->bindValue(":plot", name);
	      query->bindValue(":incident", currentIncident->getId());
	      query->bindValue(":macro", currentMacro->getId());
	      query->exec();
	    }
	  QSet<QString> attributes = currentMacro->getAttributes();
	  QMap<QString, QString> values = currentMacro->getValues();
	  QSet<QString>::iterator tit2;
	  for (tit2 = attributes.begin(); tit2 != attributes.end(); tit2++) 
	    {
	      QString attribute = *tit2;
	      QString value = values.value(attribute);
	      query2->bindValue(":plot", name);
	      query2->bindValue(":attribute", attribute);
	      query2->bindValue(":macro", currentMacro->getId());
	      query2->bindValue(":value", value);
	      query2->exec();
	    }
	  QString timing = currentMacro->getTiming();
	  QString description = currentMacro->getDescription();
	  QString comment = currentMacro->getComment();
	  int width = currentMacro->getWidth();
	  QString mode = currentMacro->getMode();
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
	  if (currentMacro->isDislodged()) 
	    {
	      dislodged = 1;
	    }
	  if (currentMacro->isVisible()) 
	    {
	      hidden = 0;
	    }
	  query3->bindValue(":plot", name);
	  query3->bindValue(":eventid", currentMacro->getId());
	  query3->bindValue(":ch_order", currentMacro->getOrder());
	  query3->bindValue(":colligation", currentMacro->getConstraint());
	  query3->bindValue(":timing", timing);
	  query3->bindValue(":description", description);
	  query3->bindValue(":comment", comment);
	  query3->bindValue(":width", width);
	  query3->bindValue(":mode", mode);
	  query3->bindValue(":curxpos", currentX);
	  query3->bindValue(":curypos", currentY);
	  query3->bindValue(":orixpos", originalX);
	  query3->bindValue(":oriypos", originalY);
	  query3->bindValue(":dislodged", dislodged);
	  query3->bindValue(":red", red);
	  query3->bindValue(":green", green);
	  query3->bindValue(":blue", blue);
	  query3->bindValue(":alpha", alpha);
	  query3->bindValue(":hidden", hidden);
	  query3->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      delete query2;
      delete query3;
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, eventVector.size());
      saveProgress->setWindowTitle("Saving embedded incidents");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_embedded_incidents "
		     "(plot, incident, macro) "
		     "VALUES (:plot, :incident, :macro)");
      QVectorIterator<EventItem*> it5(eventVector);
      while (it5.hasNext()) 
	{
	  EventItem *currentEvent = it5.next();
	  if (currentEvent->getMacroEvent() != NULL) 
	    {
	      query->bindValue(":plot", name);
	      query->bindValue(":incident", currentEvent->getId());
	      query->bindValue(":macro", currentEvent->getMacroEvent()->getId());
	      query->exec();
	      counter++;
	      saveProgress->setProgress(counter);
	      qApp->processEvents();
	    }
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, macroVector.size());
      saveProgress->setWindowTitle("Saving embedded macros");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_macros_to_macros "
		     "(plot, son, father) "
		     "VALUES (:plot, :son, :father)");
      QVectorIterator<MacroEvent*> it6(macroVector);
      while (it6.hasNext()) 
	{
	  MacroEvent *currentMacro = it6.next();
	  MacroEvent *currentFather = currentMacro->getMacroEvent(); 
	  if (currentMacro->getMacroEvent() != NULL) 
	    {
	      query->bindValue(":plot", name);
	      query->bindValue(":son", currentMacro->getId());
	      query->bindValue(":father", currentFather->getId());
	      query->exec();
	    }
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
      query->prepare("INSERT INTO saved_eg_plots_macro_labels "
		     "(plot, eventid, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden) "
		     "VALUES (:plot, :eventid, :label, :curxpos, :curypos, :xoffset, :yoffset, "
		     ":red, :green, :blue, :alpha, :hidden)");
      QVectorIterator<MacroLabel*> it7(macroLabelVector);
      while (it7.hasNext()) 
	{
	  MacroLabel *currentLabel = it7.next();
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
	  if (currentLabel->isVisible()) 
	    {
	      hidden = 0;
	    }
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
      saveProgress = new ProgressBar(0, 1, eventListWidget->rowCount());
      saveProgress->setWindowTitle("Saving legend");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_legend (plot, name, tip, "
		     "red, green, blue, alpha) "
		     "VALUES (:plot, :name, :tip, :red, :green, :blue, :alpha)");
      for (int i = 0; i != eventListWidget->rowCount(); i++) 
	{
	  QTableWidgetItem *item = eventListWidget->item(i, 0);
	  QString title = item->data(Qt::DisplayRole).toString();
	  QString tip = item->data(Qt::ToolTipRole).toString();
	  QColor color = eventListWidget->item(i, 1)->background().color();
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
      saveProgress = new ProgressBar(0, 1, lineVector.size());
      saveProgress->setWindowTitle("Saving line objects");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_lines "
		     "(plot, startx, starty, endx, endy, arone, artwo, penwidth, penstyle, "
		     "zvalue, red, green, blue, alpha) "
		     "VALUES (:plot, :startx, :starty, :endx, :endy, :arone, :artwo, "
		     ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha)");
      QVectorIterator<LineObject*> it8(lineVector);
      while (it8.hasNext()) 
	{
	  LineObject *currentLine = it8.next();
	  qreal startx = currentLine->getStartPos().x();
	  qreal starty = currentLine->getStartPos().y();
	  qreal endx = currentLine->getEndPos().x();
	  qreal endy = currentLine->getEndPos().y();
	  int arone = 0;
	  int artwo = 0;
	  if (currentLine->arrow1()) 
	    {
	      arone = 1;
	    }
	  if (currentLine->arrow2()) 
	    {
	      artwo = 1;
	    }
	  int penwidth = currentLine->getPenWidth();
	  int penstyle = currentLine->getPenStyle();
	  int zValue = currentLine->zValue();
	  QColor color = currentLine->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  query->bindValue(":plot", name);
	  query->bindValue(":startx", startx);
	  query->bindValue(":starty", starty);
	  query->bindValue(":endx", endx);
	  query->bindValue(":endy", endy);
	  query->bindValue(":arone", arone);
	  query->bindValue(":artwo", artwo);
	  query->bindValue(":penwidth", penwidth);
	  query->bindValue(":penstyle", penstyle);
	  query->bindValue(":zvalue", zValue);
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
      saveProgress = new ProgressBar(0, 1, textVector.size());
      saveProgress->setWindowTitle("Saving text items");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_texts "
		     "(plot, desc, xpos, ypos, width, size, rotation, zvalue, "
		     "red, green, blue, alpha) "
		     "VALUES (:plot, :desc, :xpos, :ypos, :width, :size, :rotation, "
		     ":zvalue, :red, :green, :blue, :alpha)");
      QVectorIterator<TextObject*> it9(textVector);
      while (it9.hasNext()) 
	{
	  TextObject *currentText = it9.next();
	  QString desc = currentText->toPlainText();
	  qreal xpos = currentText->scenePos().x();
	  qreal ypos = currentText->scenePos().y();
	  int width = currentText->textWidth();
	  int size = currentText->font().pointSize();
	  qreal rotation = currentText->getRotationValue();
	  int zValue = currentText->zValue();
	  QColor color = currentText->defaultTextColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  query->bindValue(":plot", name);
	  query->bindValue(":desc", desc);
	  query->bindValue(":xpos", xpos);
	  query->bindValue(":ypos", ypos);
	  query->bindValue(":width", width);
	  query->bindValue(":size", size);
	  query->bindValue(":rotation", rotation);
	  query->bindValue(":zvalue", zValue);
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
      saveProgress = new ProgressBar(0, 1, ellipseVector.size());
      saveProgress->setWindowTitle("Saving ellipses");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_ellipses "
		     "(plot, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha) "
		     "VALUES (:plot, :xpos, :ypos, :topleftx, :toplefty, :toprightx, :toprighty, "
		     ":bottomleftx, :bottomlefty, :bottomrightx, :bottomrighty, :rotation, "
		     ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha, "
		     ":fillred, :fillgreen, :fillblue, :fillalpha)");
      QVectorIterator<EllipseObject*> it10(ellipseVector);
      while (it10.hasNext()) 
	{
	  EllipseObject *ellipse = it10.next();
	  qreal xpos = ellipse->mapToScene(ellipse->getCenter()).x();
	  qreal ypos = ellipse->mapToScene(ellipse->getCenter()).y();
	  qreal topleftx = ellipse->topLeft().x();
	  qreal toplefty = ellipse->topLeft().y();
	  qreal toprightx = ellipse->topRight().x();
	  qreal toprighty = ellipse->topRight().y();
	  qreal bottomleftx = ellipse->bottomLeft().x();
	  qreal bottomlefty = ellipse->bottomLeft().y();
	  qreal bottomrightx = ellipse->bottomRight().x();
	  qreal bottomrighty = ellipse->bottomRight().y();
	  qreal rotation = ellipse->getRotationValue();
	  int penwidth = ellipse->getPenWidth();
	  int penstyle = ellipse->getPenStyle();
	  int zValue = ellipse->zValue();
	  QColor color = ellipse->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  QColor fillColor = ellipse->getFillColor();
	  int fillred = fillColor.red();
	  int fillgreen = fillColor.green();
	  int fillblue = fillColor.blue();
	  int fillalpha = fillColor.alpha();
	  query->bindValue(":plot", name);
	  query->bindValue(":xpos", xpos);
	  query->bindValue(":ypos", ypos);
	  query->bindValue(":topleftx", topleftx);
	  query->bindValue(":toplefty", toplefty);
	  query->bindValue(":toprightx", toprightx);
	  query->bindValue(":toprighty", toprighty);
	  query->bindValue(":bottomleftx", bottomleftx);
	  query->bindValue(":bottomlefty", bottomlefty);
	  query->bindValue(":bottomrightx", bottomrightx);
	  query->bindValue(":bottomrighty", bottomrighty);
	  query->bindValue(":rotation", rotation);
	  query->bindValue(":penwidth", penwidth);
	  query->bindValue(":penstyle", penstyle);
	  query->bindValue(":zvalue", zValue);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":alpha", alpha);
	  query->bindValue(":fillred", fillred);
	  query->bindValue(":fillgreen", fillgreen);
	  query->bindValue(":fillblue", fillblue);
	  query->bindValue(":fillalpha", fillalpha);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, rectVector.size());
      saveProgress->setWindowTitle("Saving rectangles");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_rects "
		     "(plot, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha) "
		     "VALUES (:plot, :xpos, :ypos, :topleftx, :toplefty, :toprightx, :toprighty, "
		     ":bottomleftx, :bottomlefty, :bottomrightx, :bottomrighty, :rotation, "
		     ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha, "
		     ":fillred, :fillgreen, :fillblue, :fillalpha)");
      QVectorIterator<RectObject*> it11(rectVector);
      while (it11.hasNext()) 
	{
	  RectObject *rect = it11.next();
	  qreal xpos = rect->mapToScene(rect->getCenter()).x();
	  qreal ypos = rect->mapToScene(rect->getCenter()).y();
	  qreal topleftx = rect->topLeft().x();
	  qreal toplefty = rect->topLeft().y();
	  qreal toprightx = rect->topRight().x();
	  qreal toprighty = rect->topRight().y();
	  qreal bottomleftx = rect->bottomLeft().x();
	  qreal bottomlefty = rect->bottomLeft().y();
	  qreal bottomrightx = rect->bottomRight().x();
	  qreal bottomrighty = rect->bottomRight().y();
	  qreal rotation = rect->getRotationValue();
	  int penwidth = rect->getPenWidth();
	  int penstyle = rect->getPenStyle();
	  int zValue = rect->zValue();
	  QColor color = rect->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  QColor fillColor = rect->getFillColor();
	  int fillred = fillColor.red();
	  int fillgreen = fillColor.green();
	  int fillblue = fillColor.blue();
	  int fillalpha = fillColor.alpha();
	  query->bindValue(":plot", name);
	  query->bindValue(":xpos", xpos);
	  query->bindValue(":ypos", ypos);
	  query->bindValue(":topleftx", topleftx);
	  query->bindValue(":toplefty", toplefty);
	  query->bindValue(":toprightx", toprightx);
	  query->bindValue(":toprighty", toprighty);
	  query->bindValue(":bottomleftx", bottomleftx);
	  query->bindValue(":bottomlefty", bottomlefty);
	  query->bindValue(":bottomrightx", bottomrightx);
	  query->bindValue(":bottomrighty", bottomrighty);
	  query->bindValue(":rotation", rotation);
	  query->bindValue(":penwidth", penwidth);
	  query->bindValue(":penstyle", penstyle);
	  query->bindValue(":zvalue", zValue);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":alpha", alpha);
	  query->bindValue(":fillred", fillred);
	  query->bindValue(":fillgreen", fillgreen);
	  query->bindValue(":fillblue", fillblue);
	  query->bindValue(":fillalpha", fillalpha);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      delete query;
      plotLabel->setText(name);
      changeLabel->setText("");
      QSqlDatabase::database().commit();
    }
  delete saveDialog;
}

void EventGraphWidget::seePlots() 
{
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this, EVENTGRAPH);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) 
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);
      savePlotButton->setEnabled(true);
      cleanUp();
      scene->clear();
      QString plot = savedPlotsDialog->getSelectedPlot();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT coder, red, green, blue "
		     "FROM saved_eg_plots "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      query->first();
      QString coder = query->value(0).toString();
      int red = query->value(1).toInt();
      int green = query->value(2).toInt();
      int blue = query->value(3).toInt();
      selectedCoder = coder;
      scene->setBackgroundBrush(QBrush(QColor(red, green, blue)));
      int index = coderComboBox->findText(coder);
      coderComboBox->setCurrentIndex(index);
      query->prepare("SELECT incident, ch_order, width, curxpos, curypos, orixpos, oriypos, "
		     "dislodged, mode, red, green, blue, alpha, hidden "
		     "FROM saved_eg_plots_event_items "
		     "WHERE plot = :plot ");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  int id = query->value(0).toInt();
	  int order = query->value(1).toInt();
	  int width = query->value(2).toInt();
	  qreal currentX = query->value(3).toReal();
	  qreal currentY = query->value(4).toReal();
	  qreal originalX = query->value(5).toReal();
	  qreal originalY = query->value(6).toReal();
	  int dislodged = query->value(7).toInt();
	  QString mode = query->value(8).toString();
	  int red = query->value(9).toInt();
	  int green = query->value(10).toInt();
	  int blue = query->value(11).toInt();
	  int alpha = query->value(12).toInt();
	  int hidden = query->value(13).toInt();
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT description FROM incidents WHERE id = :id");
	  query2->bindValue(":id", id);
	  query2->exec();
	  query2->first();
	  QString toolTip = "";
	  if (query2->isNull(0)) 
	    {
	      toolTip = "Incident was deleted";
	    }
	  else 
	    {
	      toolTip = breakString(query2->value(0).toString());
	    }
	  delete query2;
	  QPointF currentPos = QPointF(currentX, currentY);
	  QPointF originalPos = QPointF(originalX, originalY);
	  EventItem *currentItem = new EventItem(width, toolTip, originalPos, id, order);
	  currentItem->setPos(currentPos);
	  currentItem->setColor(QColor(red, green, blue, alpha));
	  currentItem->setZValue(3);
	  currentItem->setMode(mode);
	  eventVector.push_back(currentItem);
	  scene->addItem(currentItem);
	  if (dislodged == 1) 
	    {
	      currentItem->setDislodged(true);
	    }
	  if (hidden == 1) 
	    {
	      currentItem->hide();
	    }
	  else 
	    {
	      currentItem->show();
	    }
	}
      query->prepare("SELECT incident, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden "
		     "FROM saved_eg_plots_event_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
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
	  while (it.hasNext()) 
	    {
	      EventItem *currentItem = it.next();
	      int nodeId = currentItem->getId();
	      if (nodeId == id) 
		{
		  NodeLabel *currentLabel = new NodeLabel(currentItem);
		  currentLabel->setPlainText(text);
		  currentLabel->setTextWidth(currentLabel->boundingRect().width());
		  currentLabel->setPos(QPointF(currentX, currentY));
		  currentLabel->setOffset(QPointF(xOffset, yOffset));
		  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
		  currentLabel->setZValue(4);
		  currentItem->setLabel(currentLabel);
		  nodeLabelVector.push_back(currentLabel);
		  scene->addItem(currentLabel);
		  if (hidden == 1) 
		    {
		      currentLabel->hide();
		    }
		  else 
		    {
		      currentLabel->show();
		    }
		  break;
		}
	    }
	}
      query->prepare("SELECT eventid, ch_order, colligation, timing, description, comment, width, "
		     "mode, curxpos, curypos, orixpos, oriypos, dislodged, red, green, blue, alpha, "
		     "hidden "
		     "FROM saved_eg_plots_macro_events "
		     "WHERE plot = :plot ");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  int id = query->value(0).toInt();
	  int order = query->value(1).toInt();
	  QString constraint = query->value(2).toString();
	  QString timing = query->value(3).toString();
	  QString description = query->value(4).toString();
	  QString comment = query->value(5).toString();
	  int width = query->value(6).toInt();
	  QString mode = query->value(7).toString();
	  qreal currentX = query->value(8).toReal();
	  qreal currentY = query->value(9).toReal();
	  qreal originalX = query->value(10).toReal();
	  qreal originalY = query->value(11).toReal();
	  int dislodged = query->value(12).toInt();
	  int red = query->value(13).toInt();
	  int green = query->value(14).toInt();
	  int blue = query->value(15).toInt();
	  int alpha = query->value(16).toInt();
	  int hidden = query->value(17).toInt();
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
	  while (query2->next()) 
	    {
	      int incidentId = query2->value(0).toInt();
	      QVectorIterator<EventItem*> it(eventVector);
	      while (it.hasNext()) 
		{
		  EventItem *currentEvent = it.next();
		  if (incidentId == currentEvent->getId()) 
		    {
		      incidents.push_back(currentEvent);
		      break;
		    }
		}
	    }
	  std::sort(incidents.begin(), incidents.end(), eventLessThan);
	  query2->prepare("SELECT attribute, value FROM saved_eg_plots_attributes_to_macro_events "
			  "WHERE plot = :plot AND macro = :id");
	  query2->bindValue(":plot", plot);
	  query2->bindValue(":id", id);
	  query2->exec();
	  QSet<QString> attributes;
	  QMap<QString, QString> values;
	  while (query2->next()) 
	    {
	      QString attribute = query2->value(0).toString();
	      QString value = query2->value(1).toString();
	      attributes.insert(attribute);
	      values.insert(attribute, value);
	    }
	  delete query2;
	  MacroEvent* newMacro = new MacroEvent(width, description, originalPos, id,
						constraint, incidents);
	  QVectorIterator<EventItem*> it(incidents);
	  while (it.hasNext()) 
	    {
	      EventItem *currentEvent = it.next();
	      currentEvent->hide();
	    }
	  newMacro->setTiming(timing);
	  newMacro->setOriginalPos(originalPos);
	  newMacro->setPos(currentPos);
	  newMacro->setZValue(3);
	  newMacro->setColor(color);
	  newMacro->setAttributes(attributes);
	  newMacro->setValues(values);
	  newMacro->setOrder(order);
	  if (hidden == 1) 
	    {
	      newMacro->hide();
	    }
	  if (dislodged == 1) 
	    {
	      newMacro->setDislodged(true);
	    }
	  scene->addItem(newMacro);
	  macroVector.push_back(newMacro);
	}
      query->prepare("SELECT incident, macro "
		     "FROM saved_eg_plots_embedded_incidents "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  int incident = query->value(0).toInt();
	  int macro = query->value(1).toInt();
	  QVectorIterator<EventItem*> it(eventVector);
	  while (it.hasNext()) 
	    {
	      EventItem *incidentEvent = it.next();
	      if (incidentEvent->getId() == incident) 
		{
		  QVectorIterator<MacroEvent*> it2(macroVector);
		  while (it2.hasNext()) 
		    {
		      MacroEvent *macroEvent = it2.next();
		      if (macroEvent->getId() == macro) 
			{
			  incidentEvent->hide();
			  incidentEvent->setMacroEvent(macroEvent);
			}
		    }
		}
	    }
	}
      query->prepare("SELECT son, father "
		     "FROM saved_eg_plots_macros_to_macros "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  int son = query->value(0).toInt();
	  int father = query->value(1).toInt();
	  QVectorIterator<MacroEvent*> it(macroVector);
	  while (it.hasNext()) 
	    {
	      MacroEvent *sonEvent = it.next();
	      if (sonEvent->getId() == son) 
		{
		  QVectorIterator<MacroEvent*> it2(macroVector);
		  while (it2.hasNext()) 
		    {
		      MacroEvent *fatherEvent = it2.next();
		      if (fatherEvent->getId() == father) 
			{
			  sonEvent->hide();
			  sonEvent->setMacroEvent(fatherEvent);
			}
		    }
		}
	    }
	}
      query->prepare("SELECT eventid, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden "
		     "FROM saved_eg_plots_macro_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
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
	  while (it.hasNext()) 
	    {
	      MacroEvent *currentMacro = it.next();
	      int eventId = currentMacro->getId();
	      if (eventId == event) 
		{
		  MacroLabel *currentLabel = new MacroLabel(currentMacro);
		  currentLabel->setPlainText(text);
		  currentLabel->setTextWidth(currentLabel->boundingRect().width());
		  currentLabel->setPos(QPointF(currentX, currentY));
		  currentLabel->setOffset(QPointF(xOffset, yOffset));
		  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
		  currentLabel->setZValue(4);
		  currentMacro->setLabel(currentLabel);
		  macroLabelVector.push_back(currentLabel);
		  scene->addItem(currentLabel);
		  if (hidden == 1) 
		    {
		      currentLabel->hide();
		    }
		  else 
		    {
		      currentLabel->show();
		    }
		  break;
		}
	    }
	}
      std::sort(macroVector.begin(), macroVector.end(), eventLessThan);
      QSet<QString> types;
      query->prepare("SELECT tail, head, tailmacro, headmacro, linkage, red, green, blue, alpha, "
		     "hidden, masshidden "
		     "FROM saved_eg_plots_edges "
		     "WHERE plot = :plot ");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  int tail = query->value(0).toInt();
	  int head = query->value(1).toInt();
	  int tM = query->value(2).toInt();
	  int hM = query->value(3).toInt();
	  QString linkage = query->value(4).toString();
	  int red = query->value(5).toInt();
	  int green = query->value(6).toInt();
	  int blue = query->value(7).toInt();
	  int alpha = query->value(8).toInt();
	  int hidden = query->value(9).toInt();
	  int massHidden = query->value(10).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  bool tailMacro = false;
	  bool headMacro = false;
	  if (tM == 1) 
	    {
	      tailMacro = true;
	    }
	  if (hM == 1) 
	    {
	      headMacro = true;
	    }
	  if (!types.contains(linkage)) 
	    {
	      presentTypes.push_back(linkage);
	      QTableWidgetItem *item = new QTableWidgetItem(linkage);
	      QSqlQuery *query3 = new QSqlQuery;
	      query3->prepare("SELECT description, direction FROM linkage_types WHERE name = :name");
	      query3->bindValue(":name", linkage);
	      query3->exec();
	      query3->first();
	      QString description = query3->value(0).toString();
	      QString direction = query3->value(1).toString();
	      QString tip = linkage + " (" + direction + ") - " + description;
	      delete query3;
	      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	      item->setToolTip(tip);
	      item->setData(Qt::DisplayRole, linkage);
	      linkageListWidget->setRowCount(linkageListWidget->rowCount() + 1);
	      linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 0, item);
	      linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	      linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->setBackground(color);
	      linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->
		setFlags(linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	    }      
	  types.insert(linkage);
	  QString toolTip = "";
	  if (!(tailMacro) && !(headMacro)) 
	    {
	      QSqlQuery *query2 =  new QSqlQuery;
	      query2->prepare("SELECT comment, coder FROM linkage_comments "
			      "WHERE tail = :tail AND head = :head AND type = :type");
	      query2->bindValue(":tail", tail);
	      query2->bindValue(":head", head);
	      query2->bindValue(":type", linkage);
	      query2->exec();
	      query2->first();
	      QString comment = "";
	      QString commentCoder = "";
	      if (!(query2->isNull(0))) 
		{
		  comment = query2->value(0).toString();
		  commentCoder = query2->value(1).toString();
		  toolTip = breakString(commentCoder + " - " + comment);
		}
	      else 
		{
		  toolTip = "No comments";
		}
	      delete query2;
	    }
	  QVectorIterator<EventItem*> it(eventVector);
	  QVectorIterator<MacroEvent*> it2(macroVector);
	  QGraphicsItem *tempSource = NULL;
	  QGraphicsItem *tempTarget = NULL;
	  if (tailMacro && !(headMacro)) 
	    {
	      while (it2.hasNext()) 
		{
		  MacroEvent *currentSource = it2.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      while (it.hasNext()) 
		{
		  EventItem *currentTarget = it.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  else if (!(tailMacro) && headMacro) 
	    {
	      while (it.hasNext()) 
		{
		  EventItem *currentSource = it.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      while (it2.hasNext()) 
		{
		  MacroEvent *currentTarget = it2.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  else if (tailMacro && headMacro) 
	    {
	      while (it2.hasNext()) 
		{
		  MacroEvent *currentSource = it2.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      it2.toFront();
	      while (it2.hasNext()) 
		{
		  MacroEvent *currentTarget = it2.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  else if (!(tailMacro) && !(headMacro)) 
	    {
	      while (it.hasNext()) 
		{
		  EventItem *currentSource = it.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      it.toFront();
	      while (it.hasNext()) 
		{
		  EventItem *currentTarget = it.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  if (tempSource != NULL && tempTarget != NULL) 
	    {
	      Arrow *currentEdge = new Arrow(linkage, selectedCoder);
	      currentEdge->setZValue(2);
	      currentEdge->setStartItem(tempSource);
	      currentEdge->setEndItem(tempTarget);
	      currentEdge->setToolTip(toolTip);
	      currentEdge->setColor(color);
	      edgeVector.push_back(currentEdge);
	      scene->addItem(currentEdge);
	      if (hidden == 1) 
		{
		  currentEdge->hide();
		}
	      else 
		{
		  currentEdge->show();
		}
	      if (massHidden) 
		{
		  currentEdge->setMassHidden(true);
		}
	    }
	}
      query->prepare("SELECT name, tip, red, green, blue, alpha "
		     "FROM saved_eg_plots_legend "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
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
	  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setBackground(color);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	}
      query->prepare("SELECT startx, starty, endx, endy, arone, artwo, penwidth, penstyle, "
		     "zvalue, red, green, blue, alpha "
		     "FROM saved_eg_plots_lines "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  qreal startx = query->value(0).toReal();
	  qreal starty = query->value(1).toReal();
	  qreal endx = query->value(2).toReal();
	  qreal endy = query->value(3).toReal();
	  int arone = query->value(4).toInt();
	  int artwo = query->value(5).toInt();
	  int penwidth = query->value(6).toInt();
	  int penstyle = query->value(7).toInt();
	  int zValue = query->value(8).toInt();
	  int red = query->value(9).toInt();
	  int green = query->value(10).toInt();
	  int blue = query->value(11).toInt();
	  int alpha = query->value(12).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  LineObject *newLine = new LineObject(QPointF(startx, starty), QPointF(endx, endy));
	  lineVector.push_back(newLine);
	  newLine->setZValue(zValue);
	  newLine->setColor(color);
	  if (arone == 1) 
	    {
	      newLine->setArrow1(true);
	    }
	  if (artwo == 1) 
	    {
	      newLine->setArrow2(true);
	    }
	  newLine->setPenWidth(penwidth);
	  newLine->setPenStyle(penstyle);
	  scene->addItem(newLine);
	}
      query->prepare("SELECT desc, xpos, ypos, width, size, rotation, zValue, red, green, blue, alpha "
		     "FROM saved_eg_plots_texts "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  QString desc = query->value(0).toString();
	  qreal xpos = query->value(1).toReal();
	  qreal ypos = query->value(2).toReal();
	  int width = query->value(3).toInt();
	  int size = query->value(4).toInt();
	  qreal rotation = query->value(5).toReal();
	  int zValue = query->value(6).toInt();
	  int red = query->value(7).toInt();
	  int green = query->value(8).toInt();
	  int blue = query->value(9).toInt();
	  int alpha = query->value(10).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  TextObject *newText = new TextObject(desc);
	  textVector.push_back(newText);
	  newText->setZValue(zValue);
	  newText->setDefaultTextColor(color);
	  QFont font = newText->font();
	  font.setPointSize(size);
	  newText->setFont(font);
	  newText->setTextWidth(width);
	  newText->setPos(xpos, ypos);
	  newText->setRotationValue(rotation);
	  scene->addItem(newText);
	}
      query->prepare("SELECT xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha "
		     "FROM saved_eg_plots_ellipses "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  qreal xpos = query->value(0).toReal();
	  qreal ypos = query->value(1).toReal();
	  qreal topleftx = query->value(2).toReal();
	  qreal toplefty = query->value(3).toReal();
	  qreal toprightx = query->value(4).toReal();
	  qreal toprighty = query->value(5).toReal();
	  qreal bottomleftx = query->value(6).toReal();
	  qreal bottomlefty = query->value(7).toReal();
	  qreal bottomrightx = query->value(8).toReal();
	  qreal bottomrighty = query->value(9).toReal();
	  qreal rotation = query->value(10).toReal();
	  int penwidth = query->value(11).toInt();
	  int penstyle = query->value(12).toInt();
	  int zValue = query->value(13).toInt();
	  int red = query->value(14).toInt();
	  int green = query->value(15).toInt();
	  int blue = query->value(16).toInt();
	  int alpha = query->value(17).toInt();
	  int fillred = query->value(18).toInt();
	  int fillgreen = query->value(19).toInt();
	  int fillblue = query->value(20).toInt();
	  int fillalpha = query->value(21).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  QColor fillColor = QColor(fillred, fillgreen, fillblue, fillalpha);
	  EllipseObject *newEllipse = new EllipseObject();
	  ellipseVector.push_back(newEllipse);
	  scene->addItem(newEllipse);
	  newEllipse->setTopLeft(QPointF(topleftx, toplefty));
	  newEllipse->setTopRight(QPointF(toprightx, toprighty));
	  newEllipse->setBottomLeft(QPointF(bottomleftx, bottomlefty));
	  newEllipse->setBottomRight(QPointF(bottomrightx, bottomrighty));
	  newEllipse->moveCenter(newEllipse->mapToScene(QPointF(xpos, ypos)));
	  newEllipse->setRotationValue(rotation);
	  newEllipse->setColor(color);
	  newEllipse->setFillColor(fillColor);
	  newEllipse->setPenWidth(penwidth);
	  newEllipse->setPenStyle(penstyle);
	  newEllipse->setZValue(zValue);
	}
      query->prepare("SELECT xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha "
		     "FROM saved_eg_plots_rects "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  qreal xpos = query->value(0).toReal();
	  qreal ypos = query->value(1).toReal();
	  qreal topleftx = query->value(2).toReal();
	  qreal toplefty = query->value(3).toReal();
	  qreal toprightx = query->value(4).toReal();
	  qreal toprighty = query->value(5).toReal();
	  qreal bottomleftx = query->value(6).toReal();
	  qreal bottomlefty = query->value(7).toReal();
	  qreal bottomrightx = query->value(8).toReal();
	  qreal bottomrighty = query->value(9).toReal();
	  qreal rotation = query->value(10).toReal();
	  int penwidth = query->value(11).toInt();
	  int penstyle = query->value(12).toInt();
	  int zValue = query->value(13).toInt();
	  int red = query->value(14).toInt();
	  int green = query->value(15).toInt();
	  int blue = query->value(16).toInt();
	  int alpha = query->value(17).toInt();
	  int fillred = query->value(18).toInt();
	  int fillgreen = query->value(19).toInt();
	  int fillblue = query->value(20).toInt();
	  int fillalpha = query->value(21).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  QColor fillColor = QColor(fillred, fillgreen, fillblue, fillalpha);
	  RectObject *newRect = new RectObject();
	  rectVector.push_back(newRect);
	  scene->addItem(newRect);
	  newRect->setTopLeft(QPointF(topleftx, toplefty));
	  newRect->setTopRight(QPointF(toprightx, toprighty));
	  newRect->setBottomLeft(QPointF(bottomleftx, bottomlefty));
	  newRect->setBottomRight(QPointF(bottomrightx, bottomrighty));
	  newRect->moveCenter(newRect->mapToScene(QPointF(xpos, ypos)));
	  newRect->setRotationValue(rotation);
	  newRect->setColor(color);
	  newRect->setFillColor(fillColor);
	  newRect->setPenWidth(penwidth);
	  newRect->setPenStyle(penstyle);
	  newRect->setZValue(zValue);
	}
      distance = 70;
      plotLabel->setText(plot);
      changeLabel->setText("");
      scene->update();
      setRangeControls();
      updateLinkages();
      checkCongruency();
      setVisibility();
      delete query;
      QApplication::restoreOverrideCursor();
      qApp->processEvents();
    }
  else if (savedPlotsDialog->getExitStatus() == 2) 
    {
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
      query->exec();
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
      // saved_eg_plots_legend
      query->prepare("DELETE FROM saved_eg_plots_legend "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_eg_plots_embedded_incidents
      query->prepare("DELETE FROM saved_eg_plots_embedded_incidents "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_eg_plots_macros_to_macros 
      query->prepare("DELETE FROM saved_eg_plots_macros_to_macros "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_eg_plots_lines
      query->prepare("DELETE FROM saved_eg_plots_lines "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_eg_plots_texts
      query->prepare("DELETE FROM saved_eg_plots_texts "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_eg_plots_eipses
      query->prepare("DELETE FROM saved_eg_plots_ellipses "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_eg_plots_rects
      query->prepare("DELETE FROM saved_eg_plots_rects "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      delete query;
      seePlots();
    }
  delete savedPlotsDialog;
}

void EventGraphWidget::setChangeLabel() 
{
  if (changeLabel->text() == "" && eventVector.size() > 0) 
    {
      changeLabel->setText("*");
    }
}

void EventGraphWidget::updateLinkages() 
{
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) 
    {
      Arrow *current = it.next();
      current->updatePosition();
    }
  if (compareVector.size() > 0) 
    {
      QVectorIterator<Arrow*> it2(compareVector);
      while (it2.hasNext()) 
	{
	  Arrow *current = it2.next();
	  current->updatePosition();
	}
    }
}

void EventGraphWidget::colorByAttribute() 
{
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, INCIDENT);
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) 
    {
      QColor color = attributeColorDialog->getColor();
      QColor textColor = attributeColorDialog->getTextColor();
      QString attribute = attributeColorDialog->getAttribute();
      QString description = "";
      QSqlQuery *query = new QSqlQuery;
      bool entity = attributeColorDialog->isEntity();
      if (entity) 
	{
	  query->prepare("SELECT description FROM entities "
			 "WHERE name = :name");
	  query->bindValue(":name", attribute);
	  query->exec();
	  query->first();
	  description = query->value(0).toString();
	}
      else 
	{
	  query->prepare("SELECT description FROM incident_attributes "
			 "WHERE name = :name");
	  query->bindValue(":name", attribute);
	  query->exec();
	  query->first();
	  description = query->value(0).toString();
	}
      QVector<QString> attributeVector;
      attributeVector.push_back(attribute);
      findChildren(attribute, &attributeVector, entity);
      QVectorIterator<QString> it(attributeVector);
      while (it.hasNext()) 
	{
	  QString currentAttribute = it.next();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT incident FROM attributes_to_incidents "
			 "WHERE attribute = :currentAttribute");
	  query->bindValue(":currentAttribute", currentAttribute);
	  query->exec();
	  while (query->next()) 
	    {
	      int currentIncident = query->value(0).toInt();
	      QVectorIterator<EventItem*> it2(eventVector);
	      while (it2.hasNext()) 
		{
		  EventItem* currentEvent = it2.next();
		  if (currentEvent->getId() == currentIncident) 
		    {
		      currentEvent->setColor(color);
		      currentEvent->setMode(attribute);
		      currentEvent->getLabel()->setDefaultTextColor(textColor);
		    }
		}
	    }
	  QVectorIterator<MacroEvent*> it3(macroVector);
	  while (it3.hasNext()) 
	    {
	      MacroEvent *currentMacro = it3.next();
	      QSet<QString> attributes = currentMacro->getAttributes();
	      if (attributes.contains(currentAttribute)) 
		{
		  currentMacro->setColor(color);
		  currentMacro->setMode(attribute);
		  currentMacro->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	}
      bool found = false;
      for (int i = 0; i < eventListWidget->rowCount(); i++) 
	{
	  if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == attribute) 
	    {
	      found = true;
	      QTableWidgetItem *item = eventListWidget->item(i,0);
	      QString toolTip = breakString(attribute + " - " + description);
	      item->setToolTip(toolTip);
	      eventListWidget->item(i, 1)->setBackground(color);
	      break;
	    }
	}
      if (!found) 
	{
	  QTableWidgetItem *item = new QTableWidgetItem(attribute);
	  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	  QString toolTip = breakString(attribute + " - " + description);
	  item->setToolTip(toolTip);
	  item->setData(Qt::DisplayRole, attribute);
	  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setBackground(color);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	}
      delete query;
    }
  delete attributeColorDialog;
}


void EventGraphWidget::removeMode() 
{
  setChangeLabel();
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<EventItem*> it (eventVector);
  while (it.hasNext()) 
    {
      EventItem *current = it.next();
      if (current->getMode() == text) 
	{
	  current->setColor(Qt::white);
	  current->getLabel()->setDefaultTextColor(Qt::black);
	  current->setMode("");
	}
    }
  QVectorIterator<MacroEvent*> it2 (macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent *current = it2.next();
      if (current->getMode() == text) 
	{
	  current->setColor(Qt::white);
	  current->getLabel()->setDefaultTextColor(Qt::black);
	  current->setMode("");
	}
    }
  for (int i = 0; i != eventListWidget->rowCount();) 
    {
      if (eventListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text) 
	{
	  eventListWidget->removeRow(i);
	}
      if (i != eventListWidget->rowCount()) 
	{
	  i++;
	}
    }
  // We also want to restore any other modes that were overruled by the one we just removed.
  for (int i = 0; i != eventListWidget->rowCount(); i++) 
    {
      QString currentMode = eventListWidget->item(i,0)->data(Qt::DisplayRole).toString();
      QColor color = eventListWidget->item(i, 1)->background().color();
      QVector<QString> attributeVector;
      attributeVector.push_back(currentMode);
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT name FROM entities WHERE name = :name");
      query->bindValue(":name", currentMode);
      query->exec();
      query->first();
      bool entity = false;
      if (!query->isNull(0)) 
	{
	  entity = true;
	}
      findChildren(currentMode, &attributeVector, entity);
      QVectorIterator<QString> it3(attributeVector);
      while (it3.hasNext()) 
	{
	  QString currentAttribute = it3.next();
	  query->prepare("SELECT incident FROM attributes_to_incidents "
			 "WHERE attribute = :currentAttribute");
	  query->bindValue(":currentAttribute", currentAttribute);
	  query->exec();
	  while (query->next()) 
	    {
	      int currentIncident = query->value(0).toInt();
	      QVectorIterator<EventItem*> it4(eventVector);
	      while (it4.hasNext()) 
		{
		  EventItem* currentEvent = it4.next();
		  if (currentEvent->getId() == currentIncident) 
		    {
		      currentEvent->setColor(color);
		      currentEvent->setMode(currentMode);
		    }
		}
	    }
	  QVectorIterator<MacroEvent*> it5(macroVector);
	  while (it5.hasNext()) 
	    {
	      MacroEvent *currentMacro = it5.next();
	      QSet<QString> attributes = currentMacro->getAttributes();
	      if (attributes.contains(currentAttribute)) 
		{
		  currentMacro->setColor(color);
		  currentMacro->setMode(currentMode);
		}
	    }
	}
      delete query;
    }
}

void EventGraphWidget::setModeButtons(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
    {
      removeModeButton->setEnabled(true);
    }
  else 
    {
      removeModeButton->setEnabled(false);
    }
  if (text != eventListWidget->item(0, 0)->data(Qt::DisplayRole).toString()) 
    {
      moveModeUpButton->setEnabled(true);
    }
  else 
    {
      moveModeUpButton->setEnabled(false);
    }
  if (text != eventListWidget->item(eventListWidget->rowCount() - 1, 0)
      ->data(Qt::DisplayRole).toString()) 
    {
      moveModeDownButton->setEnabled(true);
    }
  else 
    {
      moveModeDownButton->setEnabled(false);
    }
}

void EventGraphWidget::disableModeButtons() 
{
  removeModeButton->setEnabled(false);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton->setEnabled(false);
}


void EventGraphWidget::setLinkageButtons(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
    {
      QVectorIterator<Arrow*> it(edgeVector);
      while (it.hasNext()) 
	{
	  Arrow *current = it.next();
	  if (current->getType() == text) 
	    {
	      if (current->isMassHidden()) 
		{
		  hideLinkageTypeButton->setEnabled(false);
		  showLinkageTypeButton->setEnabled(true);
		}
	      else 
		{
		  hideLinkageTypeButton->setEnabled(true);
		  showLinkageTypeButton->setEnabled(false);
		}
	      break;
	    }
	}
      if (presentTypes.size() > 1) 
	{
	  removeLinkageTypeButton->setEnabled(true);
	}
      else 
	{
	  removeLinkageTypeButton->setEnabled(false);
	}
    }
  else 
    {
      hideLinkageTypeButton->setEnabled(false);
      showLinkageTypeButton->setEnabled(false);
      removeLinkageTypeButton->setEnabled(false);
    }
}

void EventGraphWidget::disableLinkageButtons() 
{
  hideLinkageTypeButton->setEnabled(false);
  showLinkageTypeButton->setEnabled(false);
  removeLinkageTypeButton->setEnabled(false);
}

void EventGraphWidget::restoreModeColors() 
{
  for (int i = 0; i != eventListWidget->rowCount(); i++) 
    {
      QString currentMode = eventListWidget->item(i,0)->data(Qt::DisplayRole).toString();
      QColor color = eventListWidget->item(i, 1)->background().color();
      QVector<QString> attributeVector;
      attributeVector.push_back(currentMode);
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT name FROM entities WHERE name = :name");
      query->bindValue(":name", currentMode);
      query->exec();
      query->first();
      bool entity = false;
      if (!query->isNull(0)) 
	{
	  entity = true;
	}
      findChildren(currentMode, &attributeVector, entity);    
      QVectorIterator<QString> it3(attributeVector);
      while (it3.hasNext()) 
	{
	  QString currentAttribute = it3.next();
	  query->prepare("SELECT incident FROM attributes_to_incidents "
			 "WHERE attribute = :currentAttribute");
	  query->bindValue(":currentAttribute", currentAttribute);
	  query->exec();
	  while (query->next()) 
	    {
	      int currentIncident = query->value(0).toInt();
	      QVectorIterator<EventItem*> it4(eventVector);
	      while (it4.hasNext()) 
		{
		  EventItem* currentEvent = it4.next();
		  if (currentEvent->getId() == currentIncident) 
		    {
		      currentEvent->setColor(color);
		      currentEvent->setMode(currentMode);
		    }
		}
	    }
	  QVectorIterator<MacroEvent*> it5(macroVector);
	  while (it5.hasNext()) 
	    {
	      MacroEvent *currentMacro = it5.next();
	      QSet<QString> attributes = currentMacro->getAttributes();
	      if (attributes.contains(currentAttribute)) 
		{
		  currentMacro->setColor(color);
		  currentMacro->setMode(currentMode);
		}
	    }
	}
      delete query;
    }
  for (int i = 0; i < eventListWidget->rowCount(); i++) 
    {
      QString mode = eventListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
      QColor color = eventListWidget->item(i, 1)->background().color();
      QVectorIterator<EventItem*> it(eventVector);
      while (it.hasNext()) 
	{
	  EventItem *event = it.next();
	  if (event->getMode() == mode) 
	    {
	      event->setColor(color);
	    }
	}
      QVectorIterator<MacroEvent*> it2(macroVector);
      while (it2.hasNext()) 
	{
	  MacroEvent* macro = it2.next();
	  if (macro->getMode() == mode) 
	    {
	      macro->setColor(color);
	    }
	}
    }
}

void EventGraphWidget::moveModeUp() 
{
  setChangeLabel();
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != eventListWidget->item(0,0)->data(Qt::DisplayRole).toString()) 
    {
      int currentRow = eventListWidget->row(eventListWidget->currentItem());
      QTableWidgetItem *currentItem = eventListWidget->takeItem(currentRow,0);
      QColor currentColor = eventListWidget->item(currentRow, 1)->background().color();
      int newRow = currentRow - 1;
      QTableWidgetItem *otherItem = eventListWidget->takeItem(newRow, 0);
      QColor otherColor = eventListWidget->item(newRow, 1)->background().color();
      eventListWidget->setItem(newRow, 0, currentItem);
      eventListWidget->item(newRow, 1)->setBackground(currentColor);
      eventListWidget->setItem(currentRow, 0, otherItem);
      eventListWidget->item(currentRow, 1)->setBackground(otherColor);
      restoreModeColors();
      QModelIndex newIndex = eventListWidget->model()->index(newRow, 0);
      eventListWidget->setCurrentIndex(newIndex);
      setModeButtons(eventListWidget->currentItem());
    }
}

void EventGraphWidget::moveModeDown() 
{
  setChangeLabel();
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != eventListWidget->item(eventListWidget->rowCount() - 1, 0)->
      data(Qt::DisplayRole).toString()) 
    {
      int currentRow = eventListWidget->row(eventListWidget->currentItem());
      QTableWidgetItem *currentItem = eventListWidget->takeItem(currentRow, 0);
      QColor currentColor = eventListWidget->item(currentRow, 1)->background().color();
      int newRow = currentRow + 1;
      QTableWidgetItem *otherItem = eventListWidget->takeItem(newRow, 0);
      QColor otherColor = eventListWidget->item(newRow, 1)->background().color();;
      eventListWidget->setItem(newRow, 0, currentItem);
      eventListWidget->item(newRow, 1)->setBackground(currentColor);
      eventListWidget->setItem(currentRow, 0, otherItem);
      eventListWidget->item(currentRow, 1)->setBackground(otherColor);
      restoreModeColors();
      QModelIndex newIndex = eventListWidget->model()->index(newRow, 0);
      eventListWidget->setCurrentIndex(newIndex);
      setModeButtons(eventListWidget->currentItem());
    }
}

void EventGraphWidget::findChildren(QString father, QVector<QString> *children, bool entity) 
{
  QSqlQuery *query = new QSqlQuery;
  if (entity) 
    {
      query->prepare("SELECT name FROM entities WHERE father = :father");
    }
  else 
    {
      query->prepare("SELECT name FROM incident_attributes WHERE father = :father");
    }
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) 
    {
      QString currentChild = query->value(0).toString();
      children->push_back(currentChild);
      findChildren(currentChild, children, entity);
    }
  delete query;
}

void EventGraphWidget::setEventColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      QColor color = colorDialog->selectedColor();
      QVectorIterator<EventItem*> it(eventVector);
      while (it.hasNext()) 
	{
	  EventItem *currentEvent = it.next();
	  currentEvent->setColor(color);
	}
    }
  delete colorDialog;
}

void EventGraphWidget::setEdgeColor() 
{
  if (edgeVector.size() > 0) 
    {
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      if (colorDialog->exec()) 
	{
	  QColor color = colorDialog->selectedColor();
	  QVectorIterator<Arrow*> it(edgeVector);
	  while (it.hasNext()) 
	    {
	      Arrow *currentArrow = it.next();
	      currentArrow->setColor(color);
	    }
	}
      delete colorDialog;
    }
}

void EventGraphWidget::setLabelColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      QColor color = colorDialog->selectedColor();
      QVectorIterator<NodeLabel*> it(nodeLabelVector);
      while (it.hasNext()) 
	{
	  NodeLabel *currentItem = it.next();
	  currentItem->setDefaultTextColor(color);
	}
    }
  delete colorDialog;
}
  
void EventGraphWidget::setBackgroundColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      setChangeLabel();
      QColor color = colorDialog->selectedColor();
      scene->setBackgroundBrush(QBrush(color));
    }
  delete colorDialog;
}

void EventGraphWidget::changeModeColor(QTableWidgetItem *item) 
{
  if (item->column() == 1) 
    {
      QColor currentFill = item->background().color();
      QColor currentText = QColor("black");
      QPointer<ModeColorDialog> colorDialog = new ModeColorDialog(this, currentFill, currentText);
      colorDialog->exec();
      if (colorDialog->getExitStatus() == 0)
	{
	  QColor fillColor = colorDialog->getFillColor();
	  QColor textColor = colorDialog->getTextColor();
	  item->setBackground(fillColor);
	  QTableWidgetItem* neighbour = eventListWidget->item(item->row(), 0);
	  QString mode = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<EventItem*> it(eventVector);
	  while (it.hasNext()) 
	    {
	      EventItem *current = it.next();
	      if (current->getMode() == mode) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	  QVectorIterator<MacroEvent*> it2(macroVector);
	  while (it2.hasNext()) 
	    {
	      MacroEvent *current = it2.next();
	      if (current->getMode() == mode) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	}
    }
}

void EventGraphWidget::changeLinkageColor(QTableWidgetItem *item) 
{
  if (item->column() == 1) 
    {
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      colorDialog->setCurrentColor(item->background().color());
      if (colorDialog->exec()) 
	{
	  QColor color = colorDialog->selectedColor();
	  item->setBackground(color);
	  QTableWidgetItem* neighbour = linkageListWidget->item(item->row(), 0);
	  QString type = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<Arrow*> it(edgeVector);
	  while (it.hasNext()) 
	    {
	      Arrow *current = it.next();
	      if (current->getType() == type) 
		{
		  current->setColor(color);
		}
	    }
	}
    }
}

void EventGraphWidget::hideLinkageType() 
{
  setChangeLabel();
  showLinkageTypeButton->setEnabled(true);
  hideLinkageTypeButton->setEnabled(false);
  QString text = linkageListWidget->currentItem()->data(Qt::DisplayRole).toString();
  linkageListWidget->currentItem()->setBackground(Qt::gray);
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) 
    {
      Arrow *current = it.next();
      if (current->getType() == text) 
	{
	  current->setMassHidden(true);
	}
    }
  setVisibility();
  setHeights();
}

void EventGraphWidget::showLinkageType() 
{
  setChangeLabel();
  showLinkageTypeButton->setEnabled(false);
  hideLinkageTypeButton->setEnabled(true);
  QString text = linkageListWidget->currentItem()->data(Qt::DisplayRole).toString();
  linkageListWidget->currentItem()->setBackground(Qt::transparent);
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) 
    {
      Arrow *current = it.next();
      if (current->getType() == text) 
	{
	  current->setMassHidden(false);
	}
    }
  setVisibility();
  setHeights();
}

void EventGraphWidget::removeLinkageType() 
{
  setChangeLabel();
  QString text = linkageListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<Arrow*> it (edgeVector);
  while (it.hasNext()) 
    {
      Arrow *current = it.next();
      if (current->getType() == text) 
	{
	  delete current;
	  edgeVector.removeOne(current);
	}
    }
  for (int i = 0; i != linkageListWidget->rowCount();) 
    {
      if (linkageListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text) 
	{
	  linkageListWidget->removeRow(i);
	}
      if (i != linkageListWidget->rowCount()) 
	{
	  i++;
	}
    }
  QVectorIterator<QString> it2(presentTypes);
  while (it2.hasNext()) 
    {
      QString type = it2.next();
      if (type == text) 
	{
	  presentTypes.removeOne(type);
	}
    }
  setHeights();
  setPlotButtons();
  disableLinkageButtons();
}

void EventGraphWidget::plotLabels() 
{
  QVectorIterator<NodeLabel*> it(nodeLabelVector);
  while (it.hasNext()) 
    {
      NodeLabel *currentItem = it.next();
      EventItem *currentEvent = currentItem->getNode();
      if (currentEvent->isVisible()) 
	{
	  if (labelsVisible) 
	    {
	      currentItem->hide();
	    }
	  else 
	    {
	      currentItem->show();
	    }
	}
    }
  QVectorIterator<MacroLabel*> it2(macroLabelVector);
  while (it2.hasNext()) 
    {
      MacroLabel *currentItem = it2.next();
      MacroEvent *currentEvent = currentItem->getMacroEvent();
      if (currentEvent->isVisible()) 
	{
	  if (labelsVisible) 
	    {
	      currentItem->hide();
	    }
	  else 
	    {
	      currentItem->show();
	    }
	}
    }
  labelsVisible = !(labelsVisible);
}

void EventGraphWidget::processLowerRange(int value) 
{
  lowerRangeDial->setValue(value);
  lowerRangeSpinBox->setValue(value);
  if (upperRangeDial->value() <= value) 
    {
      upperRangeDial->setValue(value + 1);
      upperRangeSpinBox->setValue(value + 1);
    }
  setVisibility();
}
 
void EventGraphWidget::processUpperRange(int value) 
{
  upperRangeDial->setValue(value);
  upperRangeSpinBox->setValue(value);
  if (lowerRangeDial->value() >= value) 
    {
      lowerRangeDial->setValue(value - 1);
      lowerRangeSpinBox->setValue(value - 1);
    }
  setVisibility();
} 

void EventGraphWidget::setVisibility() 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT incident FROM incidents_to_cases "
		 "WHERE incident = :incident AND casename = :casename");
  QVectorIterator<EventItem*> it(eventVector);
  int counter = 0;
  while (it.hasNext()) 
    {
      counter++;
      EventItem* currentItem = it.next();
      if (currentItem->getMacroEvent() == NULL) 
	{
	  if (counter >= lowerRangeDial->value() && counter <= upperRangeDial->value()) 
	    {
	      currentItem->show();
	    }  else 
	    {
	      currentItem->hide();
	    }
	}
      else 
	{
	  currentItem->hide();
	  if (counter >= lowerRangeDial->value() && counter <= upperRangeDial->value()) 
	    {
	      currentItem->getMacroEvent()->show();
	    }
	  else 
	    {
	      currentItem->getMacroEvent()->hide();
	    }
	}
    }
  if (checkedCases.size() > 0) 
    {
      it.toFront();
      while (it.hasNext()) 
	{
	  EventItem *currentItem = it.next();
	  QVectorIterator<QString> it2(checkedCases);
	  bool found = false;
	  while (it2.hasNext()) 
	    {
	      QString currentCase = it2.next();
	      query->bindValue(":incident", currentItem->getId());
	      query->bindValue(":casename", currentCase);
	      query->exec();
	      query->first();
	      if (!query->isNull(0)) 
		{
		  found = true;
		}
	    }
	  if (!found) 
	    {
	      if (currentItem->getMacroEvent() == NULL) 
		{
		  currentItem->hide();
		}
	      else 
		{
		  bool keep = false;
		  QVector<EventItem*> contents = currentItem->getMacroEvent()->getIncidents();
		  QVectorIterator<EventItem*> it2(contents);
		  while (it2.hasNext()) 
		    {
		      EventItem *currentIncident = it2.next();
		      QVectorIterator<QString> it3(checkedCases);
		      while (it3.hasNext()) 
			{
			  QString currentCase = it3.next();
			  query->bindValue(":incident", currentIncident->getId());
			  query->bindValue(":casename", currentCase);
			  query->exec();
			  query->first();
			  if (!query->isNull(0)) 
			    {
			      keep = true;
			    }
			}
		    }
		  if (!keep) 
		    {
		      currentItem->getMacroEvent()->hide();
		    }
		}
	    }
	}
    }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent* current = it2.next();
      if (current->getMacroEvent() != NULL) 
	{
	  current->hide();
	}
    }
  QVectorIterator<Arrow*> it3(edgeVector);
  while (it3.hasNext()) 
    {
      Arrow *currentEdge = it3.next();
      if (currentEdge->isMassHidden()) 
	{
	  currentEdge->hide();
	}
      else 
	{
	  QGraphicsItem *tail = currentEdge->startItem();
	  QGraphicsItem *head = currentEdge->endItem();
	  if (tail->isVisible() && head->isVisible()) 
	    {
	      currentEdge->setVisible(true);
	    }
	  else 
	    {
	      currentEdge->setVisible(false);
	    }
	}
    }
  QVectorIterator<NodeLabel*> it4(nodeLabelVector);
  while (it4.hasNext()) 
    {
      NodeLabel *currentText = it4.next();
      EventItem *currentParent = currentText->getNode();
      if (!(currentParent->isVisible())) 
	{
	  currentText->hide();
	}
      else 
	{
	  if (labelsVisible) 
	    {
	      currentText->show();
	    }
	}
    }
  QVectorIterator<MacroLabel*> it5(macroLabelVector);
  while (it5.hasNext()) 
    {
      MacroLabel *currentText = it5.next();
      MacroEvent *currentParent = currentText->getMacroEvent();
      if (!(currentParent->isVisible())) 
	{
	  currentText->hide();
	}
      else 
	{
	  if (labelsVisible) 
	    {
	      currentText->show();
	    }
	}
    }
  setHeights();
  QRectF currentRect = scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
  delete query;
}

void EventGraphWidget::setHeights() 
{
  QVector<QString> finished;
  QVectorIterator<QString> it(presentTypes);
  it.toBack();
  while (it.hasPrevious()) 
    {
      QString currentType = it.previous();
      QVectorIterator<Arrow*> it2(edgeVector);
      while (it2.hasNext()) 
	{
	  Arrow *currentArrow = it2.next();
	  if (currentArrow->isVisible()) 
	    {
	      if (currentArrow->getType() == currentType) 
		{
		  QGraphicsItem *start = currentArrow->startItem();
		  QGraphicsItem *end = currentArrow->endItem();
		  int countFound = 0;
		  bool found = false;
		  QVectorIterator<Arrow*> it3(edgeVector);
		  while (it3.hasNext()) 
		    {
		      Arrow *otherArrow = it3.next();
		      if (otherArrow->isVisible()) 
			{
			  if (otherArrow != currentArrow) 
			    {
			      if (otherArrow->startItem() == start &&
				  otherArrow->endItem() == end) 
				{
				  found = true;
				  if (!finished.contains(otherArrow->getType())) 
				    {
				      countFound++;
				    }
				}
			      else if (otherArrow->startItem() == end && otherArrow->endItem() == start) 
				{
				  found = true;
				}
			    }
			}
		    }
		  if (countFound > 0) 
		    {
		      currentArrow->setHeight((countFound + 1) * 40);
		    }
		  else if (found) 
		    {
		      currentArrow->setHeight(1 * 40);
		    }
		  else 
		    {
		      currentArrow->setHeight(0);
		    }
		}
	    }
	  finished.push_back(currentType);
	}
    }
}

void EventGraphWidget::setRangeControls() 
{
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

void EventGraphWidget::exportSvg() 
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("New svg file"),"", tr("svg files (*.)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if (!fileName.endsWith(".svg")) 
	{
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

void EventGraphWidget::exportTable() 
{
  // We create a vector of all types of events and fill it.
  QVector<QGraphicsItem*> events;
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
      EventItem* current = it.next();
      if (current->isVisible()) 
	{
	  events.push_back(current);
	}
    }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent *current = it2.next();
      if (current->isVisible()) 
	{
	  events.push_back(current);
	}
    }
  // We finish this vector by sorting it.
  std::sort(events.begin(), events.end(), componentsSort);
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // First we should write the headers of the columns.
      fileOut << "row_number" << ","
	      << "Id" << ","
	      << "Timing" << ","
	      << "Description" << ","
	      << "Raw" << ","
	      << "Comments" << ","
	      << "Type" << ","
	      << "Mode" << ","
	      << "Incidents" << ","
	      << "Source" << "\n";
      // We set up a variable to keep track of row numbers.
      int row = 0;
      // Now we can iterate the vector and write the results.
      QVectorIterator<QGraphicsItem*> it3(events);
      while (it3.hasNext()) 
	{
	  row++;
	  QGraphicsItem *current = it3.next();
	  EventItem *event = qgraphicsitem_cast<EventItem*>(current);
	  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
	  if (event) 
	    {
	      QString timing = "";
	      QString description = "";
	      QString raw = "";
	      QString comment = "";
	      QString source = "";
	      QString mode = event->getMode();
	      QString type = "Incident";
	      int id = event->getId();
	      QString order = event->getLabel()->toPlainText();
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT timestamp, description, raw, comment, source FROM incidents "
			     "WHERE id = :id");
	      query->bindValue(":id", id);
	      query->exec();
	      query->first();
	      if (!(query->isNull(0))) 
		{
		  timing = query->value(0).toString();
		  description = query->value(1).toString();
		  raw = query->value(2).toString();
		  comment = query->value(3).toString();
		  source = query->value(4).toString();
		}
	      delete query;
	      fileOut << row << ","
		      << order.toStdString() << ","
		      << "\"" << doubleQuote(timing).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(description).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(raw).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(comment).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(type).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(mode).toStdString() << "\"" << ","
		      << "NA" << ","
		      << "\"" << doubleQuote(source).toStdString() << "\""<< "\n";
	    }
	  else if (macro) 
	    {
	      QString timing = macro->getTiming();
	      QString description = macro->getDescription();
	      QString raw = "";
	      QString comment = macro->getComment();
	      QString source = "";
	      QString mode = macro->getMode();
	      QString type = "";
	      QString id = macro->getLabel()->toPlainText();
	      if (macro->getConstraint() == PATHS ||
		  macro->getConstraint() == PATHSATT) 
		{
		  type = "Paths based";
		}
	      else if (macro->getConstraint() == SEMIPATHS ||
		       macro->getConstraint() == SEMIPATHSATT) 
		{
		  type = "Semi-paths based";
		}
	      else if (macro->getConstraint() == NOCONSTRAINT ||
		       macro->getConstraint() == NOCONSTRAINTATT) 
		{
		  type = "No constraint";
		}
	      QString components = "";
	      QVector<EventItem*> incidents = macro->getIncidents();
	      int incidentId = macro->getIncidents().first()->getId();
	      QSqlQuery *query = new QSqlQuery;
	      delete query;
	      QVectorIterator<EventItem*> it4(incidents);
	      while (it4.hasNext()) 
		{
		  EventItem* incident = it4.next();
		  incidentId = incident->getId();
		  QSqlQuery *query = new QSqlQuery;
		  query->prepare("SELECT ch_order, source FROM incidents "
				 "WHERE id = :id");
		  query->bindValue(":id", incidentId);
		  query->exec();
		  query->first();
		  if (!query->isNull(0)) 
		    {
		      if (components == "") 
			{
			  components = query->value(0).toString();
			}
		      else 
			{
			  components.append(";");
			  components.append(query->value(0).toString());
			}
		      if (source == "") 
			{
			  source = query->value(1).toString();
			}
		      else 
			{
			  source.append(";");
			  source.append(query->value(1).toString());
			}
		    }
		  delete query;
		}
	      fileOut << row << ","
		      << "\"" << id.toStdString() << "\"" << ","
		      << "\"" << doubleQuote(timing).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(description).toStdString() << "\"" ","
		      << "" << ","
		      << "\"" << doubleQuote(comment).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(type).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(mode).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(components).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(source).toStdString() << "\"" << "\n";
	    }
	}
      // And that should be it.
      fileOut.close();
    }
}

void EventGraphWidget::exportNodes() 
{
  // We create a vector of all types of events and fill it.
  QVector<QGraphicsItem*> events;
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) 
    {
      EventItem* current = it.next();
      if (current->isVisible()) 
	{
	  events.push_back(current);
	}
    }
  QVectorIterator<MacroEvent*> it2(macroVector);
  while (it2.hasNext()) 
    {
      MacroEvent *current = it2.next();
      if (current->isVisible()) 
	{
	  events.push_back(current);
	}
    }
  // We finish this vector by sorting it.
  std::sort(events.begin(), events.end(), componentsSort);
  QVectorIterator<QGraphicsItem*> it3(events);
  QVector<QString> ids;
  QVector<QString> labels;
  QVector<QString> descriptions;
  QVector<QString> timings;
  QVector<QString> comments;
  QVector<QString> types;
  QVector<QString> modes;
  QVector<QString> xCoords;
  QVector<QString> yCoords;
  while (it3.hasNext()) 
    {
      QGraphicsItem *current = it3.next();
      EventItem *event = qgraphicsitem_cast<EventItem*>(current);
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
      if (event) 
	{
	  int id = event->getId();
	  QString label = event->getLabel()->toPlainText();
	  QString timing = "";
	  QString description = "";
	  QString comment = "";
	  QString mode = event->getMode();
	  QString xCoord = QString::number(event->scenePos().x());
	  QString yCoord = QString::number(event->scenePos().y());
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT description, timestamp, comment FROM incidents "
			 "WHERE id = :id");
	  query->bindValue(":id", id);
	  query->exec();
	  query->first();
	  if (!(query->isNull(0))) 
	    {
	      description = query->value(0).toString();
	      timing = query->value(1).toString();
	      comment = query->value(2).toString();
	    }
	  delete query;
	  ids.push_back("i" + label);
	  labels.push_back(label);
	  timings.push_back(timing);
	  descriptions.push_back(description);
	  comments.push_back(comment);
	  types.push_back(INCIDENT);
	  modes.push_back(mode);
	  xCoords.push_back(xCoord);
	  yCoords.push_back(yCoord);
	}
      else if (macro) 
	{
	  QString label = macro->getLabel()->toPlainText();
	  QString timing = macro->getTiming();
	  QString description = macro->getDescription();
	  QString comment = macro->getComment();
	  QString mode = macro->getMode();
	  QString xCoord = QString::number(macro->scenePos().x());
	  QString yCoord = QString::number(macro->scenePos().y());
	  QString identifier = "";
	  QString type = "";
	  if (macro->getConstraint() == PATHS ||
	      macro->getConstraint() == PATHSATT) 
	    {
	      identifier = "p";
	      type = "Paths based";
	    }
	  else if (macro->getConstraint() == SEMIPATHS ||
		   macro->getConstraint() == PATHSATT) 
	    {
	      identifier = "s";
	      type = "Semi-paths based";
	    }
	  else if (macro->getConstraint() == NOCONSTRAINT ||
		   macro->getConstraint() == NOCONSTRAINTATT) 
	    {
	      identifier = "n";
	      type = "No constraint";

	    }
	  QString id = identifier + QString::number(macro->getOrder());
	  ids.push_back(id);
	  labels.push_back(label);
	  timings.push_back(timing);
	  descriptions.push_back(description);
	  comments.push_back(comment);
	  types.push_back(type);
	  modes.push_back(mode);
	  xCoords.push_back(xCoord);
	  yCoords.push_back(yCoord);
	}
    }
  QPointer<EventNodeSettingsDialog> settingsDialog = new EventNodeSettingsDialog(this,
										 ids,
										 labels,
										 timings,
										 descriptions,
										 comments,
										 types,
										 modes,
										 xCoords,
										 yCoords,
										 macroVector);
  settingsDialog->exec();
  delete settingsDialog;
}

void EventGraphWidget::exportEdges() 
{
  // We let the user pick a file name.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // First we create the file headers
      fileOut << "Source" << ","
	      << "Target" << ","
	      << "Type" << ","
	      << "Description" << ","
	      << "Coder" << ","
	      << "Comments" << "\n";
      // Then we iterate through our edges.
      QVectorIterator<Arrow*> it(edgeVector);
      while (it.hasNext()) 
	{
	  Arrow *edge = it.next();
	  if (edge->isVisible()) 
	    {
	      EventItem *eventStart = qgraphicsitem_cast<EventItem*>(edge->startItem());
	      EventItem *eventEnd = qgraphicsitem_cast<EventItem*>(edge->endItem());
	      MacroEvent *macroStart = qgraphicsitem_cast<MacroEvent*>(edge->startItem());
	      MacroEvent *macroEnd = qgraphicsitem_cast<MacroEvent*>(edge->endItem());
	      QString description = edge->getType();
	      QString coder = selectedCoder;
	      QString source = "";
	      QString target = "";
	      QString comment = "";	      
	      if (eventStart && eventEnd)
		{
		  QSqlQuery *query = new QSqlQuery;
		  query->prepare("SELECT comment, coder FROM linkage_comments "
				 "WHERE type = :type AND tail = :tail AND head = :head");
		  query->bindValue(":type", description);
		  query->bindValue(":tail", eventStart->getId());
		  query->bindValue(":head", eventEnd->getId());
		  query->exec();
		  query->first();
		  if (!query->isNull(0))
		    {
		      comment = query->value(1).toString() + ": " + query->value(0).toString();
		    }
		  delete query;
		}
	      if (eventStart && eventEnd) 
		{
		  source = "i" + QString::number(eventStart->getOrder());
		  target = "i" + QString::number(eventEnd->getOrder());
		}
	      else if (eventStart && macroEnd) 
		{
		  source = "i" + QString::number(eventStart->getOrder());
		  if (macroEnd->getConstraint() == PATHS ||
		      macroEnd->getConstraint() == PATHSATT) 
		    {
		      target = "p" + QString::number(macroEnd->getOrder());
		    }
		  else if (macroEnd->getConstraint() == SEMIPATHS ||
			   macroEnd->getConstraint() == SEMIPATHSATT) 
		    {
		      target = "s" + QString::number(macroEnd->getOrder());
		    }
		  else if (macroEnd->getConstraint() == NOCONSTRAINT ||
			   macroEnd->getConstraint() == NOCONSTRAINTATT) 
		    {
		      target = "n" + QString::number(macroEnd->getOrder());
		    }
		}
	      else if (macroStart && macroEnd) 
		{
		  if (macroStart->getConstraint() == PATHS ||
		      macroStart->getConstraint() == PATHSATT) 
		    {
		      source = "p" + QString::number(macroStart->getOrder());
		    }
		  else if (macroStart->getConstraint() == SEMIPATHS ||
			   macroStart->getConstraint() == SEMIPATHSATT) 
		    {
		      source = "s" + QString::number(macroStart->getOrder());
		    }
		  else if (macroStart->getConstraint() == NOCONSTRAINT ||
			   macroStart->getConstraint() == NOCONSTRAINTATT) 
		    {
		      source = "n" + QString::number(macroStart->getOrder());
		    }
		  if (macroEnd->getConstraint() == PATHS ||
		      macroEnd->getConstraint() == PATHSATT) 
		    {
		      target = "p" + QString::number(macroEnd->getOrder());
		    }
		  else if (macroEnd->getConstraint() == SEMIPATHS ||
			   macroEnd->getConstraint() == SEMIPATHSATT) 
		    {
		      target = "s" + QString::number(macroEnd->getOrder());
		    }
		  else if (macroEnd->getConstraint() == NOCONSTRAINT ||
			   macroEnd->getConstraint() == NOCONSTRAINTATT) 
		    {
		      target = "n" + QString::number(macroEnd->getOrder());
		    }
		}
	      else if (macroStart && eventEnd) 
		{
		  if (macroStart->getConstraint() == PATHS) 
		    {
		      source = "p" + QString::number(macroStart->getOrder());
		    }
		  else if (macroStart->getConstraint() == SEMIPATHS ||
			   macroStart->getConstraint() == SEMIPATHSATT) 
		    {
		      source = "s" + QString::number(macroStart->getOrder());
		    }
		  else if (macroStart->getConstraint() == NOCONSTRAINT ||
			   macroStart->getConstraint() == NOCONSTRAINTATT) 
		    {
		      source = "n" + QString::number(macroStart->getOrder());
		    }
		  target = "i" + QString::number(eventEnd->getId());
		}
	      fileOut << doubleQuote(source).toStdString() << ","
		      << doubleQuote(target).toStdString() << ","
		      << "Directed" << ","
		      << "\"" << doubleQuote(description).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(coder).toStdString() << "\"" << ","
		      << "\"" << doubleQuote(comment).toStdString() << "\"" << "\n";
	    }
	}
      // And that should be it!
      fileOut.close();
    }
}

void EventGraphWidget::processEventItemContextMenu(const QString &action) 
{
  retrieveData();
  if (action == COLLIGATEACTION) 
    {
      colligateEvents();
    }
  else if (action == DISAGGREGATEACTION) 
    {
      disaggregateEvent();
    }
  else if (action == MAKEMACROACTION) 
    {
      colligateEvents();
    }
  else if (action == RECOLOREVENTSACTION) 
    {
      recolorEvents();
    }
  else if (action == RECOLORLABELSACTION) 
    {
      recolorLabels();
    }
  else if (action == COLORLINEAGEACTION) 
    {
      colorLineage();
    }
  else if (action == SETTLEACTION) 
    {
      settleEvent();
    }
  else if (action == PARALLELACTION) 
    {
      makeParallel();
    }
  else if (action == NORMALIZEACTION) 
    {
      normalizeDistance();
    }
  else if (action == CLOSEGAPACTION) 
    {
      closeGap();
    }
  else if (action == CHANGEDESCRIPTIONACTION) 
    {
      changeEventDescription();
    }
  else if (action == ADDLINKAGEACTION) 
    {
      addLinkage();
    }
  else if (action == SELECTFOLLOWERSACTION) 
    {
      selectFollowers();
    }
  else if (action == SELECTPREDECESSORSACTION) 
    {
      selectPredecessors();
    }
  else if (action == COPYDESCRIPTIONTOTEXTACTION) 
    {
      copyDescriptionToText();
    }
  else if (action == SETWIDTHACTION) 
    {
      setEventWidth();
    }
}

void EventGraphWidget::colligateEvents() 
{
  if (currentData.size() > 0) 
    {
      QPointer<AbstractionDialog> abstractionDialog = new AbstractionDialog(this,
									    eventVector,
									    macroVector,
									    currentData,
									    presentTypes,
									    selectedCoder);
      abstractionDialog->exec();
      if (abstractionDialog->getExitStatus() == 0)
	{
	  QVector<EventItem*> tempIncidents = abstractionDialog->getCollectedIncidents();
	  QString chosenAttribute = abstractionDialog->getAttribute();
	  if (tempIncidents.size() > 0) 
	    {
	      std::sort(tempIncidents.begin(), tempIncidents.end(), componentsSort);
	      qreal lowestX = 0.0;
	      qreal highestX = 0.0;
	      qreal lowestY = 0.0;
	      qreal highestY = 0.0;
	      QVectorIterator<EventItem*> it2(tempIncidents);
	      while (it2.hasNext()) 
		{
		  EventItem *current = it2.next();
		  if (lowestX == 0.0) 
		    {
		      lowestX = current->scenePos().x();
		    }
		  if (highestX == 0.0) 
		    {
		      highestX = current->scenePos().x();
		    }
		  if (lowestY == 0.0) 
		    {
		      lowestY = current->scenePos().y();
		    }
		  if (highestY == 0.0) 
		    {
		      highestY = current->scenePos().y();
		    }
		  if (current->scenePos().x() < lowestX) 
		    {
		      lowestX = current->scenePos().x();
		    }
		  if (current->scenePos().x() > highestX) 
		    {
		      highestX = current->scenePos().x();
		    }
		  if (current->scenePos().y() < lowestY) 
		    {
		      lowestY = current->scenePos().y();
		    }
		  if (current->scenePos().y() > highestY) 
		    {
		      highestY = current->scenePos().y();
		    }
		}
	      int width = highestX - lowestX + tempIncidents.last()->getWidth();
	      qreal xPos = lowestX;
	      qreal yPos = lowestY + ((highestY - lowestY) / 2);
	      QPointF originalPos = QPointF(xPos, yPos);
	      QString description = abstractionDialog->getDescription();
	      MacroEvent* current = new MacroEvent(width, description, originalPos,
						   macroVector.size() + 1,
						   abstractionDialog->getConstraint(),
						   tempIncidents);
	      current->setPos(originalPos);
	      current->setZValue(3);
	      QVectorIterator<QGraphicsItem*> it3(currentData);
	      while (it3.hasNext()) 
		{
		  EventItem *event = qgraphicsitem_cast<EventItem*>(it3.peekNext());
		  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it3.peekNext());
		  if (event) 
		    {
		      EventItem *item = qgraphicsitem_cast<EventItem*>(it3.next());
		      if (chosenAttribute != DEFAULT2) 
			{
			  bool hasAttribute = false;
			  QVector<QString> attributes;
			  attributes.push_back(chosenAttribute);
			  findChildren(chosenAttribute, &attributes,
				       abstractionDialog->isEntity());
			  QVectorIterator<QString> it4(attributes);
			  while (it4.hasNext()) 
			    {
			      QString currentAttribute = it4.next();
			      int id = item->getId();
			      QSqlQuery *query = new QSqlQuery;
			      query->prepare("SELECT incident FROM attributes_to_incidents "
					     "WHERE attribute = :attribute AND incident = :id");
			      query->bindValue(":attribute", currentAttribute);
			      query->bindValue(":id", id);
			      query->exec();
			      query->first();
			      if (!query->isNull(0)) 
				{
				  hasAttribute = true;
				  break;
				}
			      delete query;
			    }
			  if (hasAttribute) 
			    {
			      item->setMacroEvent(current);
			      item->hide();
			    }
			}
		      else 
			{
			  item->setMacroEvent(current);
			  item->hide();
			}
		    }
		  else if (macro) 
		    {
		      MacroEvent *item = qgraphicsitem_cast<MacroEvent*>(it3.next());
		      if (chosenAttribute != DEFAULT2)
			{
			  bool hasAttribute = false;
			  QVector<QString> attributes;
			  attributes.push_back(chosenAttribute);
			  findChildren(chosenAttribute, &attributes,
				       abstractionDialog->isEntity());
			  QVectorIterator<QString> it5(attributes);
			  while (it5.hasNext())
			    {
			      QString currentAttribute = it5.next();
			      QSet<QString> macroAttributes = macro->getAttributes();
			      QSetIterator<QString> it6(macroAttributes);
			      while (it6.hasNext())
				{
				  QString macroAttribute = it6.next();
				  if (macroAttribute == currentAttribute)
				    {
				      hasAttribute = true;
				    }
				}
			    }
			  if (hasAttribute)
			    {
			      item->setMacroEvent(current);
			      item->hide();
			    }
			}
		      else
			{
			  item->setMacroEvent(current);
			  item->hide();
			}
		    }
		}
	      macroVector.push_back(current);
	      current->setTiming(abstractionDialog->getTiming());
	      scene->addItem(current);
	      if (abstractionDialog->isInheriting())
		{
		  QVector<QString> inheritance = abstractionDialog->getInheritance();
		  QVectorIterator<QString> it7(inheritance);
		  while (it7.hasNext())
		    {
		      current->insertAttribute(it7.next());
		    }
		}
	      MacroLabel *macroLabel = new MacroLabel(current);
	      current->setLabel(macroLabel);
	      qreal xOffset = (current->getWidth() / 2) - 20;
	      macroLabel->setOffset(QPointF(xOffset,0));
	      macroLabel->setNewPos(current->scenePos());
	      macroLabel->setZValue(4);
	      macroLabel->setDefaultTextColor(Qt::black);
	      macroLabelVector.push_back(macroLabel);
	      scene->addItem(macroLabel);
	      rewireLinkages(current, tempIncidents);
	      updateMacroOrder();
	      updateLinkages();
	      setVisibility();
	      currentData.clear();
	      current->setSelected(true);
	      retrieveData();
	      setChangeLabel();
	      addLinkageTypeButton->setEnabled(false);
	      compareButton->setEnabled(false);
	    }
	  delete abstractionDialog;
	}
      else 
	{
	  delete abstractionDialog;
	  return;
	}
    }
  else
    {
      return;
    }
 }

void EventGraphWidget::rewireLinkages(MacroEvent *macro, QVector<EventItem*> incidents) 
{
  QVectorIterator<QString> lit(presentTypes);
  while (lit.hasNext()) 
    {
      QString currentType = lit.next();
      QColor edgeColor = QColor(Qt::gray);
      for (int i = 0; i < linkageListWidget->rowCount(); i++) 
	{
	  if (linkageListWidget->item(i, 0)->data(Qt::DisplayRole) == currentType) 
	    {
	      edgeColor = linkageListWidget->item(i, 1)->background().color();
	    }
	}    
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
      query->bindValue(":type", currentType);
      query->exec();
      query->first();
      QString direction = query->value(0).toString();
      QVector<int> incidentId;
      QVectorIterator<EventItem*> tit(incidents);
      while (tit.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(tit.next());
	  if (event) 
	    {
	      incidentId.push_back(event->getId());
	    }
	}
      QVectorIterator<EventItem*> it(incidents);
      while (it.hasNext()) 
	{
	  EventItem *current = it.next();
	  int incident = current->getId();
	  query->prepare("SELECT tail FROM linkages "
			 "WHERE head = :head AND coder = :coder AND type = :type");
	  query->bindValue(":head", incident);
	  query->bindValue(":type", currentType);
	  query->bindValue(":coder", selectedCoder);
	  query->exec();
	  QSet<int> tailSet;
	  while (query->next()) 
	    {
	      int tail = query->value(0).toInt();
	      if (!(incidentId.contains(tail))) 
		{
		  tailSet.insert(tail);
		}
	    }
	  QList<int> tailList = tailSet.toList();
	  QListIterator<int> it2(tailList);
	  while (it2.hasNext()) 
	    {
	      QGraphicsItem *tempTarget = macro;
	      QGraphicsItem *tempSource = NULL;
	      int currentTail = it2.next();
	      QVectorIterator<EventItem*> et(eventVector);
	      while (et.hasNext()) 
		{
		  EventItem *currentEvent = et.next();
		  if (currentEvent->getId() == currentTail) 
		    {
		      if (currentEvent->getMacroEvent() == NULL) 
			{
			  tempSource = currentEvent;
			}
		      else 
			{
			  tempSource = currentEvent->getMacroEvent();
			  MacroEvent *tempMacro = currentEvent->getMacroEvent();
			  while (tempMacro->getMacroEvent() != NULL) 
			    {
			      tempMacro = tempMacro->getMacroEvent();
			    }
			  tempSource = tempMacro;
			}
		      if (!tempSource->isVisible()) 
			{
			  tempSource = NULL;
			}
		      if (!tempTarget->isVisible()) 
			{
			  tempTarget = NULL;
			}
		      if (tempSource && tempTarget) 
			{
			  bool found = false;
			  QVectorIterator<Arrow*> it3(edgeVector);
			  while (it3.hasNext()) 
			    {
			      Arrow* temp = it3.next();
			      if (temp->getType() == currentType) 
				{
				  if (temp->startItem() == tempSource && temp->endItem() == tempTarget) 
				    {
				      found = true;
				    }
				}
			    }
			  if (!found) 
			    {
			      Arrow *newEdge = new Arrow(currentType, selectedCoder);
			      newEdge->setZValue(2);
			      newEdge->setStartItem(tempSource);
			      newEdge->setEndItem(tempTarget);
			      newEdge->setColor(edgeColor);
			      edgeVector.push_back(newEdge);
			      scene->addItem(newEdge);
			    }
			}
		    }
		}
	    }
	  query->prepare("SELECT head FROM linkages "
			 "WHERE tail = :tail AND coder = :coder AND type = :type");
	  query->bindValue(":tail", incident);
	  query->bindValue(":type", currentType);
	  query->bindValue(":coder", selectedCoder);
	  query->exec();
	  QSet<int> headSet;
	  while (query->next()) 
	    {
	      int head = query->value(0).toInt();
	      if (!(incidentId.contains(head))) 
		{
		  headSet.insert(head);
		}
	    }
	  QList<int> headList = headSet.toList();
	  QListIterator<int> it4(headList);
	  while (it4.hasNext()) 
	    {
	      QGraphicsItem *tempTarget = NULL;
	      QGraphicsItem *tempSource = macro;
	      int currentHead = it4.next();
	      QVectorIterator<EventItem*> et(eventVector);
	      while (et.hasNext()) 
		{
		  EventItem *currentEvent = et.next();
		  if (currentEvent->getId() == currentHead) 
		    {
		      if (currentEvent->getMacroEvent() == NULL) 
			{
			  tempTarget = currentEvent;
			}
		      else 
			{
			  tempTarget = currentEvent->getMacroEvent();
			  MacroEvent *tempMacro = currentEvent->getMacroEvent();
			  while (tempMacro->getMacroEvent() != NULL) 
			    {
			      tempMacro = tempMacro->getMacroEvent();
			    }
			  tempTarget = tempMacro;
			}
		      if (!tempTarget->isVisible()) 
			{
			  tempTarget = NULL;
			}
		      if (!tempSource->isVisible()) 
			{
			  tempSource = NULL;
			}
		      if (tempSource && tempTarget) 
			{
			  bool found = false;
			  QVectorIterator<Arrow*> it5(edgeVector);
			  while (it5.hasNext()) 
			    {
			      Arrow *temp = it5.next();
			      if (temp->getType() == currentType) 
				{
				  if (temp->startItem() == tempSource && temp->endItem() == tempTarget) 
				    {
				      found = true;
				    }
				}
			    }
			  if (!found) 
			    {
			      Arrow *newEdge = new Arrow(currentType, selectedCoder);
			      newEdge->setZValue(2);
			      newEdge->setStartItem(tempSource);
			      newEdge->setEndItem(tempTarget);
			      newEdge->setColor(edgeColor);
			      edgeVector.push_back(newEdge);
			      scene->addItem(newEdge);
			    }
			}
		    }
		}
	    }
	}
      delete query; 
    }
}

void EventGraphWidget::disaggregateEvent() 
{
  QVector<QGraphicsItem*> components;
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) 
    {
      MacroEvent *macro = it.next();
      if (macro->getMacroEvent() == selectedMacro) 
	{
	  components.push_back(macro);
	  macro->setMacroEvent(NULL);
	  macro->show();
	  // And we want to reassign their child incidents.
	  QVector<EventItem*> tempIncidents = macro->getIncidents();
	  QVectorIterator<EventItem*> it2(tempIncidents);
	  while (it2.hasNext()) 
	    {
	      EventItem *tempIncident = it2.next();
	      tempIncident->setMacroEvent(macro);
	    }
	}
    }
  QVectorIterator<EventItem*> it2(eventVector);
  while (it2.hasNext()) 
    {
      EventItem *event = it2.next();
      if (event->getMacroEvent() == selectedMacro) 
	{
	  components.push_back(event);
	  event->setMacroEvent(NULL);
	  event->show();
	}
    }
  std::sort(components.begin(), components.end(), componentsSort);
  QVectorIterator<QGraphicsItem*> it3(components);
  qreal xPos = selectedMacro->scenePos().x();
  qreal firstPos = components.first()->scenePos().x();
  qreal diff = 50;
  while (it3.hasNext()) 
    {
      QGraphicsItem *current = it3.next();
      EventItem *event = qgraphicsitem_cast<EventItem*>(current);
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
      diff = diff * -1;
      qreal yPos = selectedMacro->scenePos().y() + diff;
      if (event) 
	{
	  event->setPos(xPos + event->scenePos().x() - firstPos, yPos);
	  event->getLabel()->setNewPos(event->scenePos());
	}
      else if (macro) 
	{
	  macro->setPos(xPos + macro->scenePos().x() - firstPos, yPos);
	  macro->getLabel()->setNewPos(macro->scenePos());
	}
    }
  QVector<QGraphicsItem*> allEvents;
  QVectorIterator<EventItem*> it4(eventVector);
  while (it4.hasNext()) 
    {
      EventItem *item = it4.next();
      allEvents.push_back(item);
    }
  QVectorIterator<MacroEvent*> it5(macroVector);
  while (it5.hasNext()) 
    {
      MacroEvent *item = it5.next();
      allEvents.push_back(item);
    }
  std::sort(allEvents.begin(), allEvents.end(), eventLessThanWidth);
  QGraphicsItem *nextUp = NULL;
  QVectorIterator<QGraphicsItem*> it6(allEvents);
  while (it6.hasNext()) 
    {
      QGraphicsItem *current = it6.next();
      if (!components.contains(current) &&
	  current->scenePos().x() > selectedMacro->scenePos().x() + selectedMacro->getWidth()) 
	{
	  nextUp = current;
	  break;
	}
    }
  if (nextUp != NULL) 
    {
      EventItem* event = qgraphicsitem_cast<EventItem*>(components.last());
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(components.last());
      qreal dist = 0;
      if (event) 
	{
	  dist = nextUp->scenePos().x() - event->scenePos().x() - event->getWidth() + 40;
	}
      else if (macro) 
	{
	  dist = nextUp->scenePos().x() - macro->scenePos().x() - macro->getWidth() + 40;
	}
      QPointF currentPos = nextUp->scenePos();
      if (dist < distance) 
	{
	  QVectorIterator<QGraphicsItem*> it7(allEvents);
	  while (it7.hasNext()) 
	    {
	      QGraphicsItem *item = it7.next();
	      EventItem *event = qgraphicsitem_cast<EventItem*>(item);
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(item);
	      if (event) 
		{
		  if (event->scenePos().x() >= currentPos.x()) 
		    {
		      if (!components.contains(event)) 
			{
			  event->setPos(event->scenePos().x() + distance - dist, event->scenePos().y());
			  event->setOriginalPos(event->scenePos());
			  event->getLabel()->setNewPos(event->scenePos());
			}
		    }
		}
	      else if (macro) 
		{
		  if (macro->scenePos().x() >= currentPos.x()) 
		    {
		      if (!components.contains(macro)) 
			{
			  macro->setPos(macro->scenePos().x() + distance - dist, macro->scenePos().y());
			  macro->setOriginalPos(macro->scenePos());
			  macro->getLabel()->setNewPos(macro->scenePos());
			}
		    }
		}
	    }
	}
    }
  /* 
     Then we adapt to the new situation by cleaning up the macro that
     we have disaggregated, by rewiring linkages, and so on.
  */
  QVector<Arrow*>::iterator it8;
  for (it8 = edgeVector.begin(); it8 != edgeVector.end();) 
    {
      Arrow *current = *it8;
      if(current->startItem() == selectedMacro ||
	 current->endItem() == selectedMacro) 
	{
	  delete current;
	  edgeVector.removeOne(current);
	}
      else 
	{
	  it8++;
	}
    }
  QVectorIterator<MacroEvent*> it9(macroVector);
  while (it9.hasNext()) 
    {
      MacroEvent *macro = it9.next();
      if (macro != selectedMacro) 
	{
	  QVector<EventItem*> currentIncidents = macro->getIncidents();
	  rewireLinkages(macro, currentIncidents);
	}
    }
  scene->removeItem(selectedMacro->getLabel());
  scene->removeItem(selectedMacro);
  delete selectedMacro->getLabel();
  macroLabelVector.removeOne(selectedMacro->getLabel());
  updateMacroIds(selectedMacro);
  delete selectedMacro;
  macroVector.removeOne(selectedMacro);
  selectedMacro = NULL;
  updateMacroOrder();
  setVisibility();
  if (macroVector.size() > 0) 
    {
      addLinkageTypeButton->setEnabled(false);
    }
  else 
    {
      addLinkageTypeButton->setEnabled(true);
    }
  setCompareButton();
  updateLinkages();
}

void EventGraphWidget::updateMacroIds(MacroEvent *macro) 
{
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) 
    {
      MacroEvent *current = it.next();
      if (current != macro && current->getId() > macro->getId()) 
	{
	  current->setNewId(current->getId() - 1);
	}
    }
}

void EventGraphWidget::updateMacroOrder() 
{
  int order = 1;
  std::sort(macroVector.begin(), macroVector.end(), componentsSort);
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) 
    {
      MacroEvent *current = it.next();
      current->setOrder(order);
      MacroLabel *label = current->getLabel();
      QColor labelColor = label->defaultTextColor();
      delete label;
      macroLabelVector.removeOne(label);
      MacroLabel *newLabel = new MacroLabel(current);
      order++;
      if (current->getConstraint() == PATHS ||
	  current->getConstraint() == PATHSATT) 
	{
	  QString label = "P-" + QString::number(current->getOrder());
	  newLabel->setPlainText(label);
	  newLabel->setTextWidth(newLabel->boundingRect().width());
      
	}
      else if (current->getConstraint() == SEMIPATHS ||
	       current->getConstraint() == SEMIPATHSATT) 
	{
	  QString label = "S-" + QString::number(current->getOrder());
	  newLabel->setPlainText(label);
	  newLabel->setTextWidth(newLabel->boundingRect().width());
	}
      else if (current->getConstraint() == NOCONSTRAINT ||
	       current->getConstraint() == NOCONSTRAINTATT) 
	{
	  QString label = "N-" + QString::number(current->getOrder());
	  newLabel->setPlainText(label);
	  newLabel->setTextWidth(newLabel->boundingRect().width());
	}
      current->setLabel(newLabel);
      macroLabelVector.push_back(newLabel);
      qreal xOffset = (current->getWidth() / 2) - 20;
      newLabel->setOffset(QPointF(xOffset,0));
      newLabel->setNewPos(current->scenePos());
      newLabel->setZValue(4);
      newLabel->setDefaultTextColor(labelColor);
      scene->addItem(newLabel);
    }
}

void EventGraphWidget::recolorEvents() 
{
  if (scene->selectedItems().size() > 0) 
    {
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      if (colorDialog->exec()) 
	{
	  setChangeLabel();
	  QColor color = colorDialog->selectedColor();
	  delete colorDialog;
	  QListIterator<QGraphicsItem*> it(scene->selectedItems());
	  while (it.hasNext()) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	      MacroEvent *macro =  qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	      if (event) 
		{
		  EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
		  currentEvent->setColor(color);
		}
	      else if (macro) 
		{
		  MacroEvent *currentEvent = qgraphicsitem_cast<MacroEvent*>(it.next());
		  currentEvent->setColor(color);
		}
	      else 
		{
		  it.next();
		}
	    }
	}
    }
}

void EventGraphWidget::recolorLabels() 
{
  if (scene->selectedItems().size() > 0) 
    {
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      if (colorDialog->exec()) 
	{
	  setChangeLabel();
	  QColor color = colorDialog->selectedColor();
	  delete colorDialog;
	  QListIterator<QGraphicsItem*> it(scene->selectedItems());
	  while (it.hasNext()) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	      if (event) 
		{
		  EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
		  NodeLabel *currentLabel = currentEvent->getLabel();
		  currentLabel->setDefaultTextColor(color);
		}
	      else if (macro) 
		{
		  MacroEvent *currentEvent = qgraphicsitem_cast<MacroEvent*>(it.next());
		  MacroLabel *currentLabel = currentEvent->getLabel();
		  currentLabel->setDefaultTextColor(color);
		}
	      else 
		{
		  it.next();
		}
	    }
	}
    }
}

void EventGraphWidget::colorLineage() 
{
  if (currentData.size() == 1) 
    {
      QGraphicsItem *current = currentData[0];
      QPointer<LineageColorDialog> lineage = new LineageColorDialog(this);
      lineage->exec();
      if (lineage->getExitStatus() == 0) 
	{
	  QColor originFill = lineage->getOriginFillColor();
	  QColor ancestorsFill = lineage->getAncestorFillColor();
	  QColor descendantsFill = lineage->getDescendantFillColor();
	  QColor unrelatedFill = lineage->getUnrelatedFillColor();
	  QColor originText = lineage->getOriginTextColor();
	  QColor ancestorsText = lineage->getAncestorTextColor();
	  QColor descendantsText = lineage->getDescendantTextColor();
	  QColor unrelatedText = lineage->getUnrelatedTextColor();
	  QVectorIterator<EventItem*> it(eventVector);
	  while (it.hasNext()) 
	    {
	      EventItem *current = it.next();
	      current->setColor(unrelatedFill);
	      current->getLabel()->setDefaultTextColor(unrelatedText);
	    }
	  QVectorIterator<MacroEvent*> it2(macroVector);
	  while (it2.hasNext()) 
	    {
	      MacroEvent *current = it2.next();
	      current->setColor(unrelatedFill);
	      current->getLabel()->setDefaultTextColor(unrelatedText);
	    }
	  EventItem *event = qgraphicsitem_cast<EventItem*>(current);
	  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
	  if (event) 
	    {
	      event->setColor(originFill);
	      event->getLabel()->setDefaultTextColor(originText);
	    }
	  else if (macro) 
	    {
	      macro->setColor(originFill);
	      macro->getLabel()->setDefaultTextColor(originText);
	    }
	  if (presentTypes.size() > 1) 
	    {
	      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, presentTypes);
	      relationshipChooser->exec();
	      if (relationshipChooser->getExitStatus() == 0) 
		{
		  QString selection = relationshipChooser->getSelection();
		  QSet<QGraphicsItem*> finished;
		  finished.insert(current);
		  findAncestors(ancestorsFill, ancestorsText, current, &finished, selection);
		  finished.clear();
		  finished.insert(current);
		  findDescendants(descendantsFill, descendantsText, current, &finished, selection);
		}
	    }
	  else 
	    {
	      QString selection = presentTypes[0];
	      QSet<QGraphicsItem*> finished;
	      finished.insert(current);
	      findAncestors(ancestorsFill, ancestorsText, current, &finished, selection);
	      finished.clear();
	      finished.insert(current);
	      findDescendants(descendantsFill, descendantsText, current, &finished, selection);
	    }
	}
    }
}

void EventGraphWidget::findAncestors(QColor ancestorFill,
				     QColor ancestorText,
				     QGraphicsItem *origin,
				     QSet<QGraphicsItem*> *pFinished,
				     QString type) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query->bindValue(":type", type);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  if (direction == PAST) 
    {
      QVectorIterator<Arrow*> it(edgeVector);
      while (it.hasNext()) 
	{
	  Arrow *edge = it.next();
	  if (edge->startItem() == origin) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(edge->endItem());
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(edge->endItem());
	      if (event) 
		{
		  event->setColor(ancestorFill);
		  event->getLabel()->setDefaultTextColor(ancestorText);
		}
	      else if (macro) 
		{
		  macro->setColor(ancestorFill);
		  macro->getLabel()->setDefaultTextColor(ancestorText);
		}
	      if (!pFinished->contains(edge->endItem())) 
		{
		  pFinished->insert(edge->endItem());
		  findAncestors(ancestorFill, ancestorText, edge->endItem(), pFinished, type);
		}
	    }
	}
    }
  else if (direction == FUTURE) 
    {
      QVectorIterator<Arrow*> it(edgeVector);
      while (it.hasNext()) 
	{
	  Arrow *edge = it.next();
	  if (edge->endItem() == origin) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(edge->startItem());
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(edge->startItem());
	      if (event) 
		{
		  event->setColor(ancestorFill);
		  event->getLabel()->setDefaultTextColor(ancestorText);
		}
	      else if (macro) 
		{
		  macro->setColor(ancestorFill);
		  macro->getLabel()->setDefaultTextColor(ancestorText);
		}
	      if (!pFinished->contains(edge->startItem())) 
		{
		  pFinished->insert(edge->startItem());
		  findAncestors(ancestorFill, ancestorText, edge->startItem(), pFinished, type);
		}
	    }
	}
    }
  delete query;
}


void EventGraphWidget::findDescendants(QColor descendantFill,
				       QColor descendantText,
				       QGraphicsItem *origin,
				       QSet<QGraphicsItem*> *pFinished,
				       QString type) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query->bindValue(":type", type);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  if (direction == PAST) 
    {
      QVectorIterator<Arrow*> it(edgeVector);
      while (it.hasNext()) 
	{
	  Arrow *edge = it.next();
	  if (edge->endItem() == origin) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(edge->startItem());
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(edge->startItem());
	      if (event) 
		{
		  event->setColor(descendantFill);
		  event->getLabel()->setDefaultTextColor(descendantText);
		}
	      else if (macro) 
		{
		  macro->setColor(descendantFill);
		  macro->getLabel()->setDefaultTextColor(descendantText);
		}
	      if (!pFinished->contains(edge->startItem())) 
		{
		  pFinished->insert(edge->startItem());
		  findDescendants(descendantFill, descendantText, edge->startItem(), pFinished, type);
		}
	    }
	}
    }
  else if (direction == FUTURE) 
    {
      QVectorIterator<Arrow*> it(edgeVector);
      while (it.hasNext()) 
	{
	  Arrow *edge = it.next();
	  if (edge->startItem() == origin) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(edge->endItem());
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(edge->endItem());
	      if (event) 
		{
		  event->setColor(descendantFill);
		  event->getLabel()->setDefaultTextColor(descendantText);
		}
	      else if (macro) 
		{
		  macro->setColor(descendantFill);
		  macro->getLabel()->setDefaultTextColor(descendantText);
		}
	      if (!pFinished->contains(edge->endItem())) 
		{
		  pFinished->insert(edge->endItem());
		  findDescendants(descendantFill, descendantText, edge->endItem(), pFinished, type);
		}
	    }
	}
    }
  delete query;
}

void EventGraphWidget::setEventWidth() 
{
  emit changeEventWidth(scene->selectedItems()[0]);
}

void EventGraphWidget::exportTransitionMatrix() 
{
  // First we ask the user which relationship to consider (if there are more than one present).
  QString type = presentTypes[0];
  if (presentTypes.size() > 1) 
    {
      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, presentTypes);
      relationshipChooser->exec();
      if (relationshipChooser->getExitStatus() == 0) 
	{
	  type = relationshipChooser->getSelection();
	}
    }
  // Then we ask the user what kind of matrix (s)he wants to export
  QPointer<ExportTransitionMatrixDialog> exportDialog = new ExportTransitionMatrixDialog(this);
  exportDialog->exec();
  if (exportDialog->getExitStatus() == 0) 
    {
      bool isMode = exportDialog->isMode();
      bool isProb = exportDialog->isProbability();
      // We make a vector of vectors for our matrix.
      QVector<QVector<int>> matrix;
      // We make a vector to hold the row and column names of our matrix.
      QVector<QString> names;
      // We make vector for our row marginals.
      QVector<int> rowMarginals;
      // And we want to know how often an event of a given mode appears (as the source) in a transition.
      QVector<int> transitionsRow;
      // We need to iterate through the list widget twice.
      qApp->setOverrideCursor(Qt::WaitCursor);
      if (eventListWidget->rowCount() == 0) 
	{
	  QPointer <QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->addButton(QMessageBox::Ok);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<b>No modes assigned</b>");
	  warningBox->setInformativeText("This function only works after you assigned modes.");
	  warningBox->exec();
	  delete warningBox;
	}
      for (int i = 0; i != eventListWidget->rowCount(); i++) 
	{
	  // We initialise the transitions count here.
	  int transitions = 0;
	  QTableWidgetItem *rowItem = eventListWidget->item(i, 0);
	  QString rowMode = rowItem->data(Qt::DisplayRole).toString();
	  QVector<int> currentRow; // Current row of our new matrix.
	  names.push_back(rowMode); // fill the vector of names.
	  // Let's first count the number of times an event with this mode occurs.
	  QVector<QGraphicsItem*> allEvents;
	  QVectorIterator<EventItem*> eventIt(eventVector);
	  QVectorIterator<MacroEvent*> macroIt(macroVector);
	  int occurrence = 0;
	  while (eventIt.hasNext())
	    {
	      EventItem* currentEvent = eventIt.next();
	      if (currentEvent->isVisible())
		{
		  allEvents.push_back(currentEvent);
		}
	    }
	  while (macroIt.hasNext())
	    {
	      MacroEvent* currentMacro = macroIt.next();
	      if (currentMacro->isVisible())
		{
		  allEvents.push_back(currentMacro);
		}
	    }
	  // If we want to look at attributes instead of mode, it is better
	  // to identify the relevant attributes before we iterate through
	  // all events.
	  QVector<int> incidents;
	  QVector<QString> attributeVector; 
	  if (!isMode)
	    {
	      attributeVector.push_back(rowMode);
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT name FROM entities WHERE name = :name");
	      query->bindValue(":name", rowMode);
	      query->exec();
	      query->first();
	      bool entity = false;
	      if (!query->isNull(0)) 
		{
		  entity = true;
		}
	      findChildren(rowMode, &attributeVector, entity);

	      QVectorIterator<QString> attIt(attributeVector);
	      while (attIt.hasNext()) 
		{
		  QString attribute = attIt.next();
		  query->prepare("SELECT incident FROM attributes_to_incidents "
				 "WHERE attribute = :attribute");
		  query->bindValue(":attribute", attribute);
		  query->exec();
		  while (query->next())
		    {
		      incidents.push_back(query->value(0).toInt());
		    }
		}
	      delete query;
	    }
	  QVectorIterator<QGraphicsItem*> allIt(allEvents);
	  while (allIt.hasNext())
	    {
	      QGraphicsItem* currentEvent = allIt.next();
	      EventItem *eventItem = qgraphicsitem_cast<EventItem*>(currentEvent);
	      MacroEvent *macroItem = qgraphicsitem_cast<MacroEvent*>(currentEvent);
	      if (isMode) 
		{
		  if (eventItem) 
		    {
		      if (eventItem->getMode() == rowMode) 
			{
			  occurrence++;
			}
		    }
		  else if (macroItem) 
		    {
		      if (macroItem->getMode() == rowMode) 
			{
			  occurrence++;
			}
		    }
		}
	      else if (!isMode) 
		{
		  if (eventItem) 
		    {
		      if (incidents.contains(eventItem->getId())) 
			{
			  occurrence++;
			}
		    }
		  else if (macroItem) 
		    {
		      bool found = false;
		      QVectorIterator<QString> attIt(attributeVector);
		      while (attIt.hasNext()) 
			{
			  QString attribute = attIt.next();
			  if (macroItem->getAttributes().contains(attribute)) 
			    {
			      found = true;
			    }
			}
		      if (found) 
			{
			  occurrence++;
			}
		    }
		}
	    }
	  rowMarginals.push_back(occurrence);
	  // And we add the results to the appropriate columns.
	  for (int j = 0; j != eventListWidget->rowCount(); j++)
	    {
	      QTableWidgetItem *colItem = eventListWidget->item(j, 0);
	      QString colMode = colItem->data(Qt::DisplayRole).toString();
	      // Now we need to iterate through our edges.
	      QVectorIterator<Arrow*> it(edgeVector);
	      int count = 0;
	      QVector<QString> rowVector;
	      rowVector.push_back(rowMode);
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT name FROM entities WHERE name = :name");
	      query->bindValue(":name", rowMode);
	      query->exec();
	      query->first();
	      bool entity = false;
	      if (!query->isNull(0)) 
		{
		  entity = true;
		}
	      findChildren(rowMode, &rowVector, entity);
	      QVector<QString> colVector;
	      colVector.push_back(colMode);
	      query->prepare("SELECT name FROM entities WHERE name = :name");
	      query->bindValue(":name", colMode);
	      query->exec();
	      query->first();
	      entity = false;
	      if (!query->isNull(0)) 
		{
		  entity = true;
		}
	      findChildren(colMode, &colVector, entity);
	      QSet<int> rowIncidents;
	      QSet<int> colIncidents;
	      QVectorIterator<QString> rit(rowVector);
	      QVectorIterator<QString> cit(colVector);
	      query->prepare("SELECT incident FROM attributes_to_incidents "
			     "WHERE attribute = :attribute");
	      while (rit.hasNext())
		{
		  QString currentAttribute = rit.next();
		  query->bindValue(":attribute", currentAttribute);
		  query->exec();
		  while (query->next())
		    {
		      rowIncidents.insert(query->value(0).toInt());
		    }
		}
	      while (cit.hasNext())
		{
		  QString currentAttribute = cit.next();
		  query->bindValue(":attribute", currentAttribute);
		  query->exec();
		  while (query->next())
		    {
		      colIncidents.insert(query->value(0).toInt());
		    }
		}
	      while (it.hasNext()) 
		{
		  Arrow *edge = it.next();
		  if (edge->getType() == type) 
		    {
		      if (edge->isVisible()) 
			{
			  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(edge->startItem());
			  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(edge->endItem());
			  MacroEvent *startMacro = qgraphicsitem_cast<MacroEvent*>(edge->startItem());
			  MacroEvent *endMacro = qgraphicsitem_cast<MacroEvent*>(edge->endItem());
			  // If we are only counting modes.
			  if (isMode) 
			    {
			      QString startMode = "";
			      QString endMode = "";
			      if (startEvent) 
				{
				  startMode = startEvent->getMode();
				}
			      else if (startMacro) 
				{
				  startMode = startMacro->getMode();
				}
			      if (endEvent) 
				{
				  endMode = endEvent->getMode(); 
				}
			      else if (endMacro) 
				{
				  endMode = endMacro->getMode();
				}
			      if (startMode == rowMode && endMode == colMode) 
				{
				  transitions++;
				  count++;
				}
			    }
			  // If we are counting all occurrences of variables.
			  else if (!isMode) 
			    {
			      bool rowFound = false;
			      bool colFound = false;
			      if (startEvent) 
				{
				  if (rowIncidents.contains(startEvent->getId()))
				    {
				      rowFound = true;
				    }
				}
			      else if (startMacro) 
				{
				  QSetIterator<QString> attIt(startMacro->getAttributes());
				  while(attIt.hasNext()) 
				    {
				      QString currentAttribute = attIt.next();
				      if (rowVector.contains(currentAttribute)) 
					{
					  rowFound = true;
					  break;
					}
				    }
				}
			      if (endEvent) 
				{
				  if (colIncidents.contains(endEvent->getId()))
				    {
				      colFound = true;
				    }
				}
			      else if (endMacro) 
				{
				  QSetIterator<QString> attIt(endMacro->getAttributes());
				  while(attIt.hasNext()) 
				    {
				      QString currentAttribute = attIt.next();
				      if (colVector.contains(currentAttribute)) 
					{
					  colFound = true;
					  break;
					}
				    }
				}
			      if (rowFound && colFound) 
				{
				  transitions++;
				  count++;
				}
			    }
			}	      
		    }
		}
	      // We push the current cell value to our row vector.
	      currentRow.push_back(count);
	      delete query;
	    }
	  // We push the number of transitions observed into the appropriate row.
	  transitionsRow.push_back(transitions);
	  // And then we add our row to the matrix.
	  matrix.push_back(currentRow);
	}
      qApp->restoreOverrideCursor();
      qApp->processEvents();
      // If we have a filled matrix, let's write it to a file.
      if (matrix.size() > 0) 
	{
	  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"",
							  tr("csv files (*.csv)"));
	  if (!fileName.trimmed().isEmpty()) 
	    {
	      if(!fileName.endsWith(".csv")) 
		{
		  fileName.append(".csv");
		}
	      // And we create a file outstream.  
	      std::ofstream fileOut(fileName.toStdString().c_str());
	      // We first write the header.
	      QVectorIterator<QString> it2(names);
	      while (it2.hasNext()) 
		{
		  QString currentName = it2.next();
		  fileOut << "," << "\"" << doubleQuote(currentName).toStdString() << "\"";
		}
	      // Add occurrence label and end the header with a newline symbol.
	      fileOut << "," << "Occurrence" << "\n"; 
	      int counter = 0;
	      QVectorIterator<QVector<int>> it3(matrix);
	      while (it3.hasNext()) 
		{
		  QVector<int> currentRow = it3.next();
		  fileOut << doubleQuote(names[counter]).toStdString(); // First row contains row names.
		  QVectorIterator<int> it4(currentRow);
		  while (it4.hasNext()) 
		    {
		      // If we are working with probabilities
		      double currentCell = -1.0;
		      if (isProb) 
			{
			  currentCell = it4.next() / (double) rowMarginals[counter];
			  // If we are working with raw values.
			}
		      else if (!isProb) 
			{
			  currentCell = it4.next();
			}
		      fileOut << "," << currentCell;
		    }
		  // We add the row marginals to the end and close with a newline symbol.
		  fileOut << "," << rowMarginals[counter] << "\n";
		  counter++;
		}
	      // Our final row is the number of times our items were observed in transitions.
	      fileOut << "Transitions observed";
	      QVectorIterator<int> it5(transitionsRow);
	      while (it5.hasNext()) 
		{
		  int currentCol = it5.next();
		  fileOut << "," << currentCol;
		}
	      // And that is it.
	      fileOut.close();
	    }
	}
    }
}

void EventGraphWidget::settleEvent() 
{
  if (currentData.size() == 1) 
    {
      QGraphicsItem *current = currentData[0];
      EventItem *event = qgraphicsitem_cast<EventItem*>(current);
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
      if (event) 
	{
	  event->setOriginalPos(current->scenePos());
	}
      else if (macro) 
	{
	  macro->setOriginalPos(current->scenePos());
	}
    }
  else 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>Multiple events selected</b>");
      warningBox->setInformativeText("You can only settle one event at a time.");
      warningBox->exec();
      delete warningBox;
    }
}

void EventGraphWidget::normalizeDistance() 
{
  if (currentData.size() == 1) 
    {
      QVector<QGraphicsItem*> allEvents;
      QVectorIterator<EventItem*> it(eventVector);
      while (it.hasNext()) 
	{
	  allEvents.push_back(it.next());
	}
      QVectorIterator<MacroEvent*> it2(macroVector);
      while (it2.hasNext()) 
	{
	  allEvents.push_back(it2.next());
	}
      std::sort(allEvents.begin(), allEvents.end(), eventLessThanWidth);
      QGraphicsItem *current = currentData[0];
      QGraphicsItem *target = NULL;
      QVectorIterator<QGraphicsItem*> it3(allEvents);
      while (it3.hasNext()) 
	{
	  QGraphicsItem *item = it3.next();
	  EventItem *itemEvent = qgraphicsitem_cast<EventItem*>(item);
	  MacroEvent *itemMacro = qgraphicsitem_cast<MacroEvent*>(item);
	  if (itemEvent && itemEvent->scenePos().x() +
	      itemEvent->getWidth() < current->scenePos().x()) 
	    {
	      target = item;
	    }
	  else if (itemMacro && itemMacro->scenePos().x() +
		   itemMacro->getWidth() < current->scenePos().x()) 
	    {
	      target = item;
	    }
	  else 
	    {
	      break;
	    }
	}
      if (target != NULL) 
	{
	  EventItem *targetEvent = qgraphicsitem_cast<EventItem*>(target);
	  MacroEvent *targetMacro = qgraphicsitem_cast<MacroEvent*>(target);
	  int width = 0;
	  if (targetEvent) 
	    {
	      width = targetEvent->getWidth();
	    }
	  else if (targetMacro) 
	    {
	      width = targetMacro->getWidth();
	    }
	  current->setPos(target->scenePos().x() + distance + width - 40, current->scenePos().y());
	  EventItem *event = qgraphicsitem_cast<EventItem*>(current);
	  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
	  if (event) 
	    {
	      event->getLabel()->setNewPos(event->scenePos());
	    }
	  else if (macro) 
	    {
	      macro->getLabel()->setNewPos(macro->scenePos());
	    }
	}
    }
  else 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>Multiple events selected</b>");
      warningBox->setInformativeText("You can only normalize one event at a time.");
      warningBox->exec();
      delete warningBox;
    }
}

void EventGraphWidget::makeParallel() 
{
  if (currentData.size() > 1) 
    {
      QGraphicsItem *lowest = NULL;
      QVectorIterator<QGraphicsItem*> it(currentData);
      while (it.hasNext()) 
	{
	  QGraphicsItem *current = it.next();
	  if (lowest == NULL) 
	    {
	      lowest = current;
	    }
	  else 
	    {
	      if (current->scenePos().x() < lowest->scenePos().x()) 
		{
		  lowest = current;
		}
	    }
	}
      QVectorIterator<QGraphicsItem*> it2(currentData);
      while (it2.hasNext()) 
	{
	  QGraphicsItem *current = it2.next();
	  if (current != lowest) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(current);
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
	      if (event) 
		{
		  event->setPos(lowest->scenePos().x(), event->scenePos().y());
		  event->getLabel()->setNewPos(event->scenePos());
		}
	      else if (macro) 
		{
		  macro->setPos(lowest->scenePos().x(), macro->scenePos().y());
		  macro->getLabel()->setNewPos(macro->scenePos());
		}
	    } 
	}
    }
  else 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>Multiple events required </b>");
      warningBox->setInformativeText("You cannot perform this action on a single event.");
      warningBox->exec();
      delete warningBox;
    }
}

void EventGraphWidget::closeGap() 
{
  if (currentData.size() == 1) 
    {
      QVector<QGraphicsItem*> allEvents;
      QVectorIterator<EventItem*> it(eventVector);
      while (it.hasNext()) 
	{
	  allEvents.push_back(it.next());
	}
      QVectorIterator<MacroEvent*> it2(macroVector);
      while (it2.hasNext()) 
	{
	  allEvents.push_back(it2.next());
	}
      std::sort(allEvents.begin(), allEvents.end(), eventLessThanWidth);
      QGraphicsItem *current = currentData[0];
      QGraphicsItem *target = NULL;
      QVectorIterator<QGraphicsItem*> it3(allEvents);
      while (it3.hasNext()) 
	{
	  QGraphicsItem *item = it3.next();
	  EventItem *itemEvent = qgraphicsitem_cast<EventItem*>(item);
	  MacroEvent *itemMacro = qgraphicsitem_cast<MacroEvent*>(item);
	  if (itemEvent) 
	    {
	      if (itemEvent->isVisible() &&
		  itemEvent->scenePos().x() + itemEvent->getWidth() < current->scenePos().x()) 
		{
		  target = item;
		}
	    }
	  else if (itemMacro) 
	    {
	      if (itemMacro->isVisible() &&
		  itemMacro->scenePos().x() + itemMacro->getWidth() < current->scenePos().x()) 
		{
		  target = item;
		}
	    }
	  else 
	    {
	      break;
	    }
	}
      if (target != NULL) 
	{
	  EventItem *targetEvent = qgraphicsitem_cast<EventItem*>(target);
	  MacroEvent *targetMacro = qgraphicsitem_cast<MacroEvent*>(target);
	  int width = 0;
	  if (targetEvent) 
	    {
	      width = targetEvent->getWidth();
	    }
	  else if (targetMacro) 
	    {
	      width = targetMacro->getWidth();
	    }
	  if (current->scenePos().x() - target->scenePos().x() - width + 40 > distance) 
	    {
	      qreal oldX = current->scenePos().x();
	      current->setPos(target->scenePos().x() + distance + width - 40, current->scenePos().y());
	      qreal newX = current->scenePos().x();
	      qreal dist = oldX - newX;
	      EventItem *event = qgraphicsitem_cast<EventItem*>(current);
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(current);
	      if (event) 
		{
		  event->setOriginalPos(event->scenePos());
		  event->getLabel()->setNewPos(event->scenePos());
		}
	      else if (macro) 
		{
		  macro->setOriginalPos(macro->scenePos());
		  macro->getLabel()->setNewPos(macro->scenePos());
		}
	      QVectorIterator<QGraphicsItem*> it5(allEvents);
	      while (it5.hasNext()) 
		{
		  QGraphicsItem *follow = it5.next();
		  if (follow->scenePos().x() > current->scenePos().x()) 
		    {
		      EventItem *followEvent = qgraphicsitem_cast<EventItem*>(follow);
		      MacroEvent *followMacro = qgraphicsitem_cast<MacroEvent*>(follow);
		      if (followEvent) 
			{
			  followEvent->setPos(followEvent->scenePos().x() - dist, followEvent->scenePos().y());
			  followEvent->setOriginalPos(followEvent->scenePos());
			  followEvent->getLabel()->setNewPos(followEvent->scenePos());
			}
		      else if (followMacro) 
			{
			  followMacro->setPos(followMacro->scenePos().x() - dist, followMacro->scenePos().y());
			  followMacro->setOriginalPos(followMacro->scenePos());
			  followMacro->getLabel()->setNewPos(followMacro->scenePos());
			}
		    }
		}
	    }
	}
    }
  else 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>Multiple events selected</b>");
      warningBox->setInformativeText("You can only normalize one event at a time.");
      warningBox->exec();
      delete warningBox;
    }
}

void EventGraphWidget::changeEventDescription() 
{
  QPointer<EventTextDialog> textDialog = new EventTextDialog(this);
  textDialog->submitText(selectedMacro->getDescription());
  textDialog->submitTiming(selectedMacro->getTiming());
  textDialog->exec();
  if (textDialog->getExitStatus() == 0) 
    {
      QString description = textDialog->getText();
      QString timing = textDialog->getTiming();
      selectedMacro->setDescription(description);
      selectedMacro->setTiming(timing);
      retrieveData();
    }
  delete textDialog;
}

void EventGraphWidget::addLinkage() 
{
  EventItem *eventOne = qgraphicsitem_cast<EventItem*>(scene->selectedItems()[0]);
  EventItem *eventTwo = qgraphicsitem_cast<EventItem*>(scene->selectedItems()[1]);
  if (eventOne && eventTwo) 
    {
      QString type = presentTypes[0];
      if (presentTypes.size() > 1) 
	{
	  QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, presentTypes);
	  relationshipChooser->exec();
	  if (relationshipChooser->getExitStatus() == 0) 
	    {
	      type = relationshipChooser->getSelection();
	    }
	  else 
	    {
	      delete relationshipChooser;
	      return;
	    }
	  delete relationshipChooser;
	}
      QColor edgeColor = QColor(Qt::gray);
      for (int i = 0; i < linkageListWidget->rowCount(); i++) 
	{
	  if (linkageListWidget->item(i, 0)->data(Qt::DisplayRole) == type) 
	    {
	      edgeColor = linkageListWidget->item(i, 1)->background().color();
	    }
	}    
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
      query->bindValue(":type", type);
      query->exec();
      query->first();
      QString direction = query->value(0).toString();
      int idOne = eventOne->getId();
      int idTwo = eventTwo->getId();
      bool found = false;
      query->prepare("SELECT tail, head FROM linkages "
		     "WHERE type = :type AND coder = :coder AND tail = :tail AND head = :head");
      query->bindValue(":type", type);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":tail", idOne);
      query->bindValue(":head", idTwo);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  found = true;
	}
      query->prepare("SELECT tail, head FROM linkages "
		     "WHERE type = :type AND coder = :coder AND tail = :tail AND head = :head");
      query->bindValue(":type", type);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":tail", idTwo);
      query->bindValue(":head", idOne);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  found = true;
	}
      if (!found) 
	{
	  query->prepare("INSERT INTO linkages (tail, head, type, coder) "
			 "VALUES (:tail, :head, :type, :coder)");
	  if (eventOne->getOrder() < eventTwo->getOrder()) 
	    {
	      if (direction == PAST) 
		{
		  query->bindValue(":tail", idTwo);
		  query->bindValue(":head", idOne);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", selectedCoder);
		  query->exec();      
		  Arrow *newArrow = new Arrow(type, selectedCoder);
		  newArrow->setZValue(2);
		  newArrow->setStartItem(eventTwo);
		  newArrow->setEndItem(eventOne);
		  QString toolTip = "no comment";
		  newArrow->setColor(edgeColor);
		  edgeVector.push_back(newArrow);
		  scene->addItem(newArrow);
		}
	      else if (direction == FUTURE) 
		{
		  query->bindValue(":tail", idOne);
		  query->bindValue(":head", idTwo);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", selectedCoder);
		  query->exec();      
		  Arrow *newArrow = new Arrow(type, selectedCoder);
		  newArrow->setZValue(2);
		  newArrow->setStartItem(eventOne);
		  newArrow->setEndItem(eventTwo);
		  QString toolTip = "no comment";
		  newArrow->setToolTip(toolTip);
		  newArrow->setColor(edgeColor);
		  edgeVector.push_back(newArrow);
		  scene->addItem(newArrow);
		}
	    }
	  else 
	    {
	      if (direction == PAST) 
		{
		  query->bindValue(":tail", idOne);
		  query->bindValue(":head", idTwo);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", selectedCoder);
		  query->exec();      
		  Arrow *newArrow = new Arrow(type, selectedCoder);
		  newArrow->setZValue(2);
		  newArrow->setStartItem(eventOne);
		  newArrow->setEndItem(eventTwo);
		  QString toolTip = "no comment";
		  newArrow->setColor(edgeColor);
		  edgeVector.push_back(newArrow);
		  scene->addItem(newArrow);
		}
	      else if (direction == FUTURE) 
		{
		  query->bindValue(":tail", idTwo);
		  query->bindValue(":head", idOne);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", selectedCoder);
		  query->exec();      
		  Arrow *newArrow = new Arrow(type, selectedCoder);
		  newArrow->setZValue(2);
		  newArrow->setStartItem(eventTwo);
		  newArrow->setEndItem(eventOne);
		  QString toolTip = "no comment";
		  newArrow->setColor(edgeColor);
		  newArrow->setToolTip(toolTip);
		  edgeVector.push_back(newArrow);
		  scene->addItem(newArrow);
		}
	    }
	}
      delete query;
    }
  setHeights();
  updateLinkages();
}

void EventGraphWidget::selectFollowers() 
{
  QString type = presentTypes[0];
  if (presentTypes.size() > 1) 
    {
      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, presentTypes);
      relationshipChooser->exec();
      if (relationshipChooser->getExitStatus() == 0) 
	{
	  type = relationshipChooser->getSelection();
	}
    }
  QGraphicsItem *origin = scene->selectedItems()[0];
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query->bindValue(":type", type);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  QSet<QGraphicsItem*> finished;
  finished.insert(origin);
  if (direction == PAST) 
    {
      selectAncestors(origin, &finished, type);
    }
  else if (direction == FUTURE) 
    {
      selectDescendants(origin, &finished, type);
    }
  delete query;
}

void EventGraphWidget::selectPredecessors() 
{
  QString type = presentTypes[0];
  if (presentTypes.size() > 1) 
    {
      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, presentTypes);
      relationshipChooser->exec();
      if (relationshipChooser->getExitStatus() == 0) 
	{
	  type = relationshipChooser->getSelection();
	}
    }
  QGraphicsItem *origin = scene->selectedItems()[0];
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query->bindValue(":type", type);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  QSet<QGraphicsItem*> finished;
  finished.insert(origin);
  if (direction == PAST) 
    {
      selectDescendants(origin, &finished, type);
    }
  else if (direction == FUTURE) 
    {
      selectAncestors(origin, &finished, type);
    }
  delete query;
}

void EventGraphWidget::selectAncestors(QGraphicsItem *origin,
				       QSet<QGraphicsItem*> *pFinished,
				       QString type) 
{
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) 
    {
      Arrow *edge = it.next();
      if (edge->getType() == type) 
	{
	  if (edge->endItem() == origin) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(edge->startItem());
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(edge->startItem());
	      if (event) 
		{
		  event->setSelected(true);
		}
	      else if (macro) 
		{
		  macro->setSelected(true);
		}
	      if (!pFinished->contains(edge->startItem())) 
		{
		  pFinished->insert(edge->startItem());
		  selectAncestors(edge->startItem(), pFinished, type);
		}
	    }
	}
    }
}

void EventGraphWidget::selectDescendants(QGraphicsItem *origin,
					 QSet<QGraphicsItem*> *pFinished,
					 QString type) 
{
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) 
    {
      Arrow *edge = it.next();
      if (edge->getType() == type) 
	{
	  if (edge->startItem() == origin) 
	    {
	      EventItem *event = qgraphicsitem_cast<EventItem*>(edge->endItem());
	      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(edge->endItem());
	      if (event) 
		{
		  event->setSelected(true);
		}
	      else if (macro) 
		{
		  macro->setSelected(true);
		}
	      if (!pFinished->contains(edge->endItem())) 
		{
		  pFinished->insert(edge->endItem());
		  selectDescendants(edge->endItem(), pFinished, type);
		}
	    }
	}
    }
}

void EventGraphWidget::processArrowContextMenu(const QString &action) 
{
  if (action == REMOVELINKAGEACTION) 
    {
      removeLinkage();
    }
  else if (action == KEEPLINKAGEACTION) 
    {
      keepLinkage();
    }
  else if (action == ACCEPTLINKAGEACTION) 
    {
      acceptLinkage();
    }
  else if (action == REJECTLINKAGEACTION) 
    {
      rejectLinkage();
    }
  else if (action == IGNOREMEACTION) 
    {
      ignoreLinkage();
    }
  else if (action == REMOVENORMALLINKAGEACTION) 
    {
      removeNormalLinkage();
    }
  else if (action == CHANGECOMMENTACTION) 
    {
      changeLinkageComment();
    }
}

void EventGraphWidget::removeLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(scene->selectedItems().first());
      if (arrow) 
	{
	  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(arrow);
	  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(arrow);
	  int tail = startEvent->getId();
	  int head = endEvent->getId();
	  QString type = arrow->getType();
	  QSqlQuery *query =  new QSqlQuery;
	  query->prepare("DELETE FROM linkages "
			 "WHERE tail = :tail AND head = :head AND coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", type);
	  query->exec();
	  delete query;
	  delete arrow;
	  edgeVector.removeOne(arrow);
	}
    }
}

void EventGraphWidget::removeNormalLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(scene->selectedItems().first());
      if (arrow) 
	{
	  QPointer<QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->addButton(QMessageBox::Yes);
	  warningBox->addButton(QMessageBox::No);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<h2>Are you sure?</h2>");
	  warningBox->setInformativeText("Are you sure you want to remove this linkage?");
	  if (warningBox->exec() == QMessageBox::Yes) 
	    {
	      EventItem *startEvent = qgraphicsitem_cast<EventItem*>(arrow->startItem());
	      EventItem *endEvent = qgraphicsitem_cast<EventItem*>(arrow->endItem());
	      if (startEvent && endEvent) 
		{
		  int tail = startEvent->getId();
		  int head = endEvent->getId();
		  QString type = arrow->getType();
		  QSqlQuery *query =  new QSqlQuery;
		  query->prepare("DELETE FROM linkages "
				 "WHERE tail = :tail AND head = :head "
				 "AND coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":head", head);
		  query->bindValue(":coder", selectedCoder);
		  query->bindValue(":type", type);
		  query->exec();
		  delete query;
		  delete arrow;
		  edgeVector.removeOne(arrow);
		  return;
		}
	      else 
		{
		  QPointer<QMessageBox> warningBox2 = new QMessageBox(this);
		  warningBox2->addButton(QMessageBox::Ok);
		  warningBox2->setIcon(QMessageBox::Warning);
		  warningBox2->setText("<h2>Cannot remove this linkage</h2>");
		  warningBox2->setInformativeText("You cannot remove linkages that have abstracted events "
						  "as tail and/or head.");
		  warningBox2->exec();
		}
	    }
	  else 
	    {
	      delete warningBox;
	      return;
	    }
	}
    }
  setHeights();
}

void EventGraphWidget::changeLinkageComment() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Arrow *arrow = qgraphicsitem_cast<Arrow*>(scene->selectedItems().first());
      int tail = 0;
      int head = 0;
      QString type = arrow->getType();
      EventItem *startEvent = qgraphicsitem_cast<EventItem*>(arrow->startItem());
      EventItem *endEvent = qgraphicsitem_cast<EventItem*>(arrow->endItem());
      MacroEvent *startMacro = qgraphicsitem_cast<MacroEvent*>(arrow->startItem());
      MacroEvent *endMacro = qgraphicsitem_cast<MacroEvent*>(arrow->endItem());
      if (startEvent) 
	{
	  tail = startEvent->getId();
	}
      else if (startMacro) 
	{
	  tail = startMacro->getId();
	}
      if (endEvent) 
	{
	  head = endEvent->getId();
	}
      else if (endMacro) 
	{
	  head = endMacro->getId();
	}
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT ch_order FROM incidents WHERE id = :id");
      query->bindValue(":id", tail);
      query->exec();
      query->first();
      tail = query->value(0).toInt();
      query->bindValue(":id", head);
      query->exec();
      query->first();
      head = query->value(0).toInt();
      query->prepare("SELECT comment FROM linkage_comments "
		     "WHERE type = :type AND tail = :tail AND head = :head");
      query->bindValue(":type", type);
      query->bindValue(":tail", tail);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      QString comment = "";
      bool empty = true;
      if (!query->isNull(0)) 
	{
	  comment = query->value(0).toString();
	  empty = false;
	}
      if (arrow) 
	{
	  QPointer<LargeTextDialog> textDialog = new LargeTextDialog(this);
	  textDialog->submitText(comment);
	  textDialog->setWindowTitle("Set comment");
	  textDialog->setLabel("Comment:");
	  textDialog->setEmptyAllowed(true);
	  textDialog->exec();
	  if (textDialog->getExitStatus() == 0) 
	    {
	      QString newComment = textDialog->getText();
	      if (newComment != "") 
		{
		  if (empty) 
		    {
		      query->prepare("INSERT INTO linkage_comments (comment, coder, type, tail, head) "
				     "VALUES (:comment, :coder, :type, :tail, :head)");
		    }
		  else 
		    {
		      query->prepare("UPDATE linkage_comments "
				     "SET comment = :comment, coder = :coder "
				     "WHERE type = :type AND tail = :tail AND head = :head");
		    }
		  query->bindValue(":comment", newComment);
		  query->bindValue(":coder", selectedCoder);
		  query->bindValue(":type", type);
		  query->bindValue(":tail", tail);
		  query->bindValue(":head", head);
		  query->exec();
		  QString toolTip = breakString(selectedCoder + " - " + newComment);
		  arrow->setToolTip(toolTip);
		}
	      else 
		{
		  if (empty) 
		    {
		      query->prepare("DELETE FROM linkage_comments "
				     "WHERE type = :type AND tail = :tail AND head = :head");
		      query->bindValue(":type", type);
		      query->bindValue(":tail", tail);
		      query->bindValue(":head", head);
		      query->exec();
		      QString toolTip = "no comment";
		      arrow->setToolTip(toolTip);
		    }
		}
	    }
	}
      delete query;
    }
}

void EventGraphWidget::ignoreLinkage() 
{
  if (scene->selectedItems().size() > 0) 
    {
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	  Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
	  NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(it.peekNext());
	  if (arrow && !(event) && !(text)) 
	    {
	      Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	      currentEdge->setPenStyle(1);
	    }
	  else 
	    {
	      it.next();
	    }
	}
    }
}

void EventGraphWidget::keepLinkage() 
{
  if (scene->selectedItems().size() > 0) 
    {
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) 
	{
	  Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
	  if (arrow) 
	    {
	      Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());
	      EventItem *startEvent = qgraphicsitem_cast<EventItem*>(arrow->startItem());
	      int tail = startEvent->getId();
	      currentEdge->setPenStyle(1);
	      QString type = currentEdge->getType();
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	      query->bindValue(":type", type);
	      query->exec();
	      query->first();
	      QString direction = query->value(0).toString();
	      QSet<int> markOne;
	      QSet<int> markTwo;
	      if (direction == PAST) 
		{
		  findHeadsLowerBound(&markOne, tail, 1, type);
		  findTailsUpperBound(&markTwo, tail, eventVector.size(), type);
		}
	      else if (direction == FUTURE) 
		{
		  findTailsLowerBound(&markOne, tail, 1, type);
		  findHeadsUpperBound(&markTwo, tail, eventVector.size(), type);
		}
	      QSetIterator<int> it2(markOne);
	      bool found = false;
	      QSqlDatabase::database().transaction();
	      query->prepare("SELECT head FROM linkages "
			     "WHERE coder = :coder AND type = :type "
			     "AND tail = :tail");
	      QSqlQuery *query2 = new QSqlQuery;
	      query2->prepare("SELECT tail, head FROM linkages "
			      "WHERE coder = :coder AND type = :type "
			      "AND tail = :tail AND head = :head");
	      while (it2.hasNext()) 
		{
		  int currentPathMate = it2.next();
		  query->bindValue(":coder", selectedCoder);
		  query->bindValue(":type", type);
		  query->bindValue(":tail", currentPathMate);
		  query->exec();
		  while (query->next()) 
		    {
		      int mateHead = query->value(0).toInt();
		      query2->bindValue(":coder", selectedCoder);
		      query2->bindValue(":type", type);
		      query2->bindValue(":tail", tail);
		      query2->bindValue(":head", mateHead);
		      query2->exec();
		      query2->first();
		      if (!(query2->isNull(0))) 
			{
			  found = true;
			}
		    }
		  if (found) 
		    {
		      currentEdge->setPenStyle(2);
		    }
		}
	      QSqlDatabase::database().commit();
	      QSetIterator<int> it3(markTwo);
	      found = false;
	      QSqlDatabase::database().transaction();
	      query->prepare("SELECT head FROM linkages "
			     "WHERE coder = :coder AND type = :type "
			     "AND tail = :tail");
	      query2->prepare("SELECT tail, head FROM linkages "
			      "WHERE coder = :coder AND type = :type "
			      "AND tail = :tail AND head = :head");
	      while (it3.hasNext()) 
		{
		  int currentPathMate = it3.next();
		  query->bindValue(":coder", selectedCoder);
		  query->bindValue(":type", type);
		  query->bindValue(":tail", currentPathMate);
		  query->exec();
		  while (query->next()) 
		    {
		      int mateHead = query->value(0).toInt();
		      query2->bindValue(":coder", selectedCoder);
		      query2->bindValue(":type", type);
		      query2->bindValue(":tail", tail);
		      query2->bindValue(":head", mateHead);
		      query2->exec();
		      query2->first();
		      if (!(query2->isNull(0))) 
			{
			  found = true;
			}
		    }
		  if (found) 
		    {
		      currentEdge->setPenStyle(2);
		    }
		}
	      QSqlDatabase::database().commit();
	      delete query2;
	      delete query;
	    }
	  else 
	    {
	      it.next();
	    }
	}
    }
}

void EventGraphWidget::acceptLinkage() 
{
  if (scene->selectedItems().size() > 0) 
    {
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	  Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
	  NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(it.peekNext());
	  if (arrow && !(event) && !(text)) 
	    {
	      Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	      QString type = currentEdge->getType();
	      EventItem *startEvent = qgraphicsitem_cast<EventItem*>(currentEdge->startItem());
	      EventItem *endEvent = qgraphicsitem_cast<EventItem*>(currentEdge->endItem());
	      int tail = startEvent->getId();
	      int head = endEvent->getId();
	      currentEdge->setPenStyle(0);
	      edgeVector.push_back(currentEdge);
	      compareVector.removeOne(currentEdge);
	      QSqlQuery *query =  new QSqlQuery;
	      query->prepare("INSERT INTO linkages (tail, head, coder, type) "
			     "VALUES (:tail, :head, :coder, :type)");
	      query->bindValue(":tail", tail);
	      query->bindValue(":head", head);
	      query->bindValue(":coder", selectedCoder);
	      query->bindValue(":type", type);
	      query->exec();
	      query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	      query->bindValue(":type", type);
	      query->exec();
	      query->first();
	      QString direction = query->value(0).toString();
	      QSet<int> markOne;
	      QSet<int> markTwo;
	      if (direction == PAST) 
		{
		  findHeadsLowerBound(&markOne, tail, 1, type);
		  findTailsUpperBound(&markTwo, tail, eventVector.size(), type);
		}
	      else if (direction == FUTURE) 
		{
		  findTailsLowerBound(&markOne, tail, 1, type);
		  findHeadsUpperBound(&markTwo, tail, eventVector.size(), type);
		}
	      QSetIterator<int> it2(markOne);
	      bool found = false;
	      QSqlDatabase::database().transaction();
	      query->prepare("SELECT head FROM linkages "
			     "WHERE coder = :coder AND type = :type "
			     "AND tail = :tail");
	      QSqlQuery *query2 = new QSqlQuery;
	      query2->prepare("SELECT tail, head FROM linkages "
			      "WHERE coder = :coder AND type = :type "
			      "AND tail = :tail AND head = :head");
	      while (it2.hasNext()) 
		{
		  int currentPathMate = it2.next();
		  query->bindValue(":coder", selectedCoder);
		  query->bindValue(":type", type);
		  query->bindValue(":tail", currentPathMate);
		  query->exec();
		  while (query->next()) 
		    {
		      int mateHead = query->value(0).toInt();
		      query2->bindValue(":coder", selectedCoder);
		      query2->bindValue(":type", type);
		      query2->bindValue(":tail", tail);
		      query2->bindValue(":head", mateHead);
		      query2->exec();
		      query2->first();
		      if (!(query2->isNull(0))) 
			{
			  found = true;
			}
		    }
		  if (found) 
		    {
		      currentEdge->setPenStyle(2);
		    }
		}
	      QSqlDatabase::database().commit();
	      QSetIterator<int> it3(markTwo);
	      found = false;
	      QSqlDatabase::database().transaction();
	      query->prepare("SELECT head FROM linkages "
			     "WHERE coder = :coder AND type = :type "
			     "AND tail = :tail");
	      query2->prepare("SELECT tail, head FROM linkages "
			      "WHERE coder = :coder AND type = :type "
			      "AND tail = :tail AND head = :head");
	      while (it3.hasNext()) 
		{
		  int currentPathMate = it3.next();
		  query->bindValue(":coder", selectedCoder);
		  query->bindValue(":type", type);
		  query->bindValue(":tail", currentPathMate);
		  query->exec();
		  while (query->next()) 
		    {
		      int mateHead = query->value(0).toInt();
	
		      query2->bindValue(":coder", selectedCoder);
		      query2->bindValue(":type", type);
		      query2->bindValue(":tail", tail);
		      query2->bindValue(":head", mateHead);
		      query2->exec();
		      query2->first();
		      if (!(query2->isNull(0))) 
			{
			  found = true;
			}
		    }
		  if (found) 
		    {
		      currentEdge->setPenStyle(2);
		    }
		}
	      delete query;
	      delete query2;
	      QSqlDatabase::database().commit();
	    }
	  else 
	    {
	      it.next();
	    }
	}
    }
}

void EventGraphWidget::rejectLinkage() 
{
  if (scene->selectedItems().size() > 0) 
    {
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	  Arrow *arrow = qgraphicsitem_cast<Arrow*>(it.peekNext());
	  NodeLabel *text = qgraphicsitem_cast<NodeLabel*>(it.peekNext());
	  if (arrow && !(event) && !(text)) 
	    {
	      Arrow *currentEdge = qgraphicsitem_cast<Arrow*>(it.next());;
	      delete currentEdge;
	      compareVector.removeOne(currentEdge);
	    }
	  else 
	    {
	      it.next();
	    }
	}
    }
}

void EventGraphWidget::processEventGraphContextMenu(const QString &action, const QPoint &pos) 
{
  if (action == ADDLINE) 
    {
      addLineObject(false, false, view->mapToScene(pos));
    }
  else if (action == ADDSINGLEARROW) 
    {
      addLineObject(true, false, view->mapToScene(pos));
    }
  else if (action == ADDDOUBLEARROW) 
    {
      addLineObject(true, true, view->mapToScene(pos));
    }
  else if (action == ADDTEXT) 
    {
      addTextObject(view->mapToScene(pos));
    }
  else if (action == ADDELLIPSE) 
    {
      addEllipseObject(view->mapToScene(pos));
    }
  else if (action == ADDRECT) 
    {
      addRectObject(view->mapToScene(pos));
    }
}

void EventGraphWidget::addLineObject(bool arrow1, bool arrow2, const QPointF &pos) 
{
  LineObject *newLineObject = new LineObject(QPointF(pos.x() - 100, pos.y()),
					     QPointF(pos.x() + 100, pos.y()));
  if (arrow1) 
    {
      newLineObject->setArrow1(true);
    }
  if (arrow2) 
    {
      newLineObject->setArrow2(true);
    }
  lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setZValue(5);
}

void EventGraphWidget::addTextObject(const QPointF &pos) 
{
  QPointer<LargeTextDialog> textDialog = new LargeTextDialog(this);
  textDialog->setWindowTitle("Set text");
  textDialog->setLabel("Free text:");
  textDialog->exec();
  if (textDialog->getExitStatus() == 0) 
    {
      QString text = textDialog->getText();
      TextObject *newText = new TextObject(text);
      textVector.push_back(newText);
      scene->addItem(newText);
      newText->setPos(pos);
      newText->setZValue(6);
      newText->adjustSize();
    }
  delete textDialog;
}

void EventGraphWidget::addEllipseObject(const QPointF &pos) 
{
  EllipseObject *newEllipse = new EllipseObject();
  ellipseVector.push_back(newEllipse);
  scene->addItem(newEllipse);
  newEllipse->setZValue(5);
  newEllipse->setPos(pos);
  newEllipse->moveCenter(newEllipse->mapFromScene(pos));
}

void EventGraphWidget::addRectObject(const QPointF &pos) 
{
  RectObject *newRect = new RectObject();
  rectVector.push_back(newRect);
  scene->addItem(newRect);
  newRect->setZValue(5);
  newRect->setPos(pos);
  newRect->moveCenter(newRect->mapFromScene(pos));
}

void EventGraphWidget::processLineContextMenu(const QString &action) 
{
  if (action == CHANGELINECOLOR) 
    {
      changeLineColor();
    }
  else if (action == TOGGLEARROW1) 
    {
      toggleArrow1();
    }
  else if (action == TOGGLEARROW2) 
    {
      toggleArrow2();
    }
  else if (action == DELETELINE) 
    {
      deleteLine();
    }
  else if (action == COPYOBJECT) 
    {
      duplicateLine();
    }
  else if (action == ONEFORWARD) 
    {
      objectOneForward();
    }
  else if (action == ONEBACKWARD) 
    {
      objectOneBackward();
    }
  else if (action == BRINGFORWARD) 
    {
      objectToFront();
    }
  else if (action ==  BRINGBACKWARD) 
    {
      objectToBack();
    }
}

void EventGraphWidget::changeLineColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      if (line) 
	{
	  QColor currentColor = line->getColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setCurrentColor(currentColor);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      line->setColor(color);
	    }
	  delete colorDialog;
	}
    }
}

void EventGraphWidget::toggleArrow1() 
{
  if (scene->selectedItems().size() == 1) 
    {
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      if (line) 
	{
	  line->setArrow1(!line->arrow1());
	}
    }
}

void EventGraphWidget::toggleArrow2() 
{
  if (scene->selectedItems().size() == 1) 
    {
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      if (line) 
	{
	  line->setArrow2(!line->arrow2());
	}
    }
}

void EventGraphWidget::deleteLine() 
{
  if (scene->selectedItems().size() == 1) 
    {
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      if (line) 
	{
	  delete line;
	  lineVector.removeOne(line);
	}
    }
}

void EventGraphWidget::duplicateLine() 
{
  if (scene->selectedItems().size() == 1) 
    {
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      if (line) 
	{
	  QPointF newStartPos = line->getStartPos();
	  QPointF newEndPos = line->getEndPos();
	  newStartPos.setY(newStartPos.y() - 100);
	  newEndPos.setY(newEndPos.y() - 100);
	  LineObject *newLineObject = new LineObject(newStartPos, newEndPos);
	  if (line->arrow1()) 
	    {
	      newLineObject->setArrow1(true);
	    }
	  if (line->arrow2()) 
	    {
	      newLineObject->setArrow2(true);
	    }
	  newLineObject->setPenWidth(line->getPenWidth());
	  newLineObject->setPenStyle(line->getPenStyle());
	  newLineObject->setColor(line->getColor());
	  lineVector.push_back(newLineObject);
	  scene->addItem(newLineObject);
	  newLineObject->setZValue(5);
	}
    }
}

void EventGraphWidget::processTextContextMenu(const QString &action) 
{
  if (action == CHANGETEXT) 
    {
      changeText();
    }
  else if (action == CHANGETEXTCOLOR) 
    {
      changeTextColor();
    }
  else if (action == DELETETEXT) 
    {
      deleteText();
    }
  else if (action == COPYOBJECT) 
    {
      duplicateText();
    }
  else if (action == ONEFORWARD) 
    {
      objectOneForward();
    }
  else if (action == ONEBACKWARD) 
    {
      objectOneBackward();
    }
  else if (action == BRINGFORWARD) 
    {
      objectToFront();
    }
  else if (action ==  BRINGBACKWARD) 
    {
      objectToBack();
    }
}

void EventGraphWidget::changeText() 
{
  if (scene->selectedItems().size() == 1) 
    {
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (text) 
	{
	  QString oldText = text->toPlainText();
	  QPointer<LargeTextDialog> textDialog= new LargeTextDialog(this);
	  textDialog->setWindowTitle("Edit text");
	  textDialog->setLabel("Edit free text:");
	  textDialog->submitText(oldText);
	  textDialog->exec();
	  if (textDialog->getExitStatus() == 0) 
	    {
	      QString newText = textDialog->getText();
	      text->setPlainText(newText);
	    }
	  delete textDialog;
	}
    }
}

void EventGraphWidget::changeTextColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (text) 
	{
	  QColor currentColor = text->defaultTextColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setCurrentColor(currentColor);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      text->setDefaultTextColor(color);
	    }
	  delete colorDialog;
	}
    }
}

void EventGraphWidget::deleteText() 
{
  if (scene->selectedItems().size() == 1) 
    {
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (text) 
	{
	  delete text;
	  textVector.removeOne(text);
	}
    }
}

void EventGraphWidget::copyDescriptionToText() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EventItem *event = qgraphicsitem_cast<EventItem*>(scene->selectedItems().first());
      MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(scene->selectedItems().first());
      if (event) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT description FROM incidents WHERE id = :id");
	  query->bindValue(":id", event->getId());
	  query->exec();
	  query->first();
	  QString text = query->value(0).toString();
	  delete query;
	  TextObject *newText = new TextObject(text);
	  textVector.push_back(newText);
	  scene->addItem(newText);
	  QPointF pos = event->scenePos();
	  pos.setY(pos.y() - 80);
	  newText->setPos(pos);
	  newText->setZValue(6);
	  newText->adjustSize();
	}
      else if (macro) 
	{
	  QString text = macro->getDescription();
	  TextObject *newText = new TextObject(text);
	  textVector.push_back(newText);
	  scene->addItem(newText);
	  QPointF pos = macro->scenePos();
	  pos.setY(pos.y() - 80);
	  newText->setPos(pos);
	  newText->setZValue(6);
	  newText->adjustSize();
	}
    }
}

void EventGraphWidget::duplicateText() 
{
  if (scene->selectedItems().size() == 1) 
    {
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (text) 
	{
	  QString oldText = text->toPlainText();
	  QPointer<LargeTextDialog> textDialog = new LargeTextDialog(this);
	  textDialog->setWindowTitle("Set text");
	  textDialog->setLabel("Free text:");
	  textDialog->submitText(oldText);
	  textDialog->exec();
	  if (textDialog->getExitStatus() == 0) 
	    {
	      QString alteredText = textDialog->getText();
	      TextObject *newText = new TextObject(alteredText);
	      textVector.push_back(newText);
	      scene->addItem(newText);
	      QPointF pos = text->scenePos();
	      pos.setY(pos.y() - 300);
	      newText->setPos(pos);
	      newText->setZValue(6);
	      newText->setDefaultTextColor(text->defaultTextColor());
	      newText->setRotationValue(text->getRotationValue());
	      newText->setFont(text->font());
	      newText->adjustSize();
	      newText->setTextWidth(newText->textWidth() + 50);
	    }
	  delete textDialog;
	}
    }
}

void EventGraphWidget::processEllipseContextMenu(const QString &action) 
{
  if (action == CHANGEELLIPSECOLOR) 
    {
      changeEllipseColor();
    }
  else if (action == CHANGEELLIPSEFILLCOLOR) 
    {
      changeEllipseFillColor();
    }
  else if (action == DELETEELLIPSE) 
    {
      deleteEllipse();
    }
  else if (action == COPYOBJECT) 
    {
      duplicateEllipse();
    }
  else if (action == ONEFORWARD) 
    {
      objectOneForward();
    }
  else if (action == ONEBACKWARD) 
    {
      objectOneBackward();
    }
  else if (action == BRINGFORWARD) 
    {
      objectToFront();
    }
  else if (action ==  BRINGBACKWARD) 
    {
      objectToBack();
    }
}

void EventGraphWidget::changeEllipseColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  QColor currentColor = ellipse->getFillColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setCurrentColor(currentColor);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      ellipse->setColor(color);
	    }
	  delete colorDialog;
	}
    }
}

void EventGraphWidget::changeEllipseFillColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  QColor currentColor = ellipse->getFillColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);
	  colorDialog->setCurrentColor(currentColor);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      ellipse->setFillColor(color);
	    }
	  delete colorDialog;
	}
    }
}

void EventGraphWidget::deleteEllipse() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  delete ellipse;
	  ellipseVector.removeOne(ellipse);
	}
    }  
}

void EventGraphWidget::duplicateEllipse() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  EllipseObject *newEllipse = new EllipseObject();
	  newEllipse->setRotationValue(ellipse->getRotationValue());
	  newEllipse->setTopLeft(ellipse->topLeft());
	  newEllipse->setBottomLeft(ellipse->bottomLeft());
	  newEllipse->setTopRight(ellipse->topRight());
	  newEllipse->setBottomRight(ellipse->bottomRight());
	  newEllipse->setColor(ellipse->getColor());
	  newEllipse->setFillColor(ellipse->getFillColor());
	  newEllipse->setPenWidth(ellipse->getPenWidth());
	  newEllipse->setPenStyle(ellipse->getPenStyle());
	  ellipseVector.push_back(newEllipse);
	  newEllipse->setZValue(5);
	  scene->addItem(newEllipse);
	  QPointF pos = ellipse->mapToScene(ellipse->getCenter());
	  pos.setY(pos.y() - 100);
	  pos.setX(pos.x() - 100);
	  newEllipse->moveCenter(newEllipse->mapFromScene(pos));
	}
    }
}

void EventGraphWidget::processRectContextMenu(const QString &action) 
{
  if (action == CHANGERECTCOLOR) 
    {
      changeRectColor();
    }
  else if (action == CHANGERECTFILLCOLOR) 
    {
      changeRectFillColor(); 
    }
  else if (action == DELETERECT) 
    {
      deleteRect();
    }
  else if (action == COPYOBJECT) 
    {
      duplicateRect();
    }
  else if (action == ONEFORWARD) 
    {
      objectOneForward();
    }
  else if (action == ONEBACKWARD) 
    {
      objectOneBackward();
    }
  else if (action == BRINGFORWARD) 
    {
      objectToFront();
    }
  else if (action ==  BRINGBACKWARD) 
    {
      objectToBack();
    }
}

void EventGraphWidget::changeRectColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      if (rect) 
	{
	  QColor currentColor = rect->getColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setCurrentColor(currentColor);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      rect->setColor(color);
	    }
	  delete colorDialog;
	}
    }
}

void EventGraphWidget::changeRectFillColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      if (rect) 
	{
	  QColor currentColor = rect->getFillColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);
	  colorDialog->setCurrentColor(currentColor);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      rect->setFillColor(color);
	    }
	  delete colorDialog;
	}
    }
}

void EventGraphWidget::deleteRect() 
{
  if (scene->selectedItems().size() == 1) 
    {
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      if (rect) 
	{
	  delete rect;
	  rectVector.removeOne(rect);
	}
    }  
}

void EventGraphWidget::duplicateRect() 
{
  if (scene->selectedItems().size() == 1) 
    {
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      if (rect) 
	{
	  RectObject *newRect = new RectObject();
	  newRect->setRotationValue(rect->getRotationValue());
	  newRect->setTopLeft(rect->topLeft());
	  newRect->setBottomLeft(rect->bottomLeft());
	  newRect->setTopRight(rect->topRight());
	  newRect->setBottomRight(rect->bottomRight());
	  newRect->setColor(rect->getColor());
	  newRect->setFillColor(rect->getFillColor());
	  newRect->setPenWidth(rect->getPenWidth());
	  newRect->setPenStyle(rect->getPenStyle());
	  rectVector.push_back(newRect);
	  newRect->setZValue(5);
	  scene->addItem(newRect);
	  QPointF pos = rect->mapToScene(rect->getCenter());
	  pos.setY(pos.y() - 100);
	  pos.setX(pos.x() - 100);
	  newRect->moveCenter(newRect->mapFromScene(pos));
	}
    }
}

void EventGraphWidget::objectOneForward() 
{
  int maxZ = -1;
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *current = it.next();
      if (maxZ == -1) 
	{
	  maxZ = current->zValue();
	}
      else if (maxZ < current->zValue()) 
	{
	  maxZ = current->zValue();
	}
    } 
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  int currentZValue = ellipse->zValue();
	  if (currentZValue < maxZ + 1) 
	    {
	      ellipse->setZValue(currentZValue + 1);
	    }
	}
      else if (rect) 
	{
	  int currentZValue = rect->zValue();
	  if (currentZValue < maxZ + 1) 
	    {
	      rect->setZValue(currentZValue + 1);
	    }
	}
      else if (line) 
	{
	  int currentZValue = line->zValue();
	  if (currentZValue <  maxZ + 1) 
	    {
	      line->setZValue(currentZValue + 1);
	    }
	}
      else if (text) 
	{
	  int currentZValue = text->zValue();
	  if (currentZValue < maxZ + 1) 
	    {
	      text->setZValue(currentZValue + 1);
	    }
	}
    }
  fixZValues();
}

void EventGraphWidget::objectOneBackward() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  int currentZValue = ellipse->zValue();
	  if (currentZValue > 1) 
	    {
	      ellipse->setZValue(currentZValue - 1);
	      if (ellipse->zValue() == 1) 
		{
		  QListIterator<QGraphicsItem*> it(scene->items());
		  while (it.hasNext()) 
		    {
		      QGraphicsItem *current = it.next();
		      if (current !=  ellipse) 
			{
			  current->setZValue(current->zValue() + 1);
			}
		    }
		}
	    }
	}
      else if (rect) 
	{
	  int currentZValue = rect->zValue();
	  if (currentZValue > 1) 
	    {
	      rect->setZValue(currentZValue - 1);
	      if (rect->zValue() == 1) 
		{
		  QListIterator<QGraphicsItem*> it(scene->items());
		  while (it.hasNext()) 
		    {
		      QGraphicsItem *current = it.next();
		      if (current !=  rect) 
			{
			  current->setZValue(current->zValue() + 1);
			}
		    }
		}
	    }
	}
      else if (line) 
	{
	  int currentZValue = line->zValue();
	  if (currentZValue > 1) 
	    {
	      line->setZValue(currentZValue - 1);
	      if (line->zValue() == 1) 
		{
		  QListIterator<QGraphicsItem*> it(scene->items());
		  while (it.hasNext()) 
		    {
		      QGraphicsItem *current = it.next();
		      if (current !=  line) 
			{
			  current->setZValue(current->zValue() + 1);
			}
		    }
		}
	    }
	}
      else if (text) 
	{
	  int currentZValue = text->zValue();
	  if (currentZValue > 1) 
	    {
	      text->setZValue(currentZValue - 1);
	      if (text->zValue() == 1) 
		{
		  QListIterator<QGraphicsItem*> it(scene->items());
		  while (it.hasNext()) 
		    {
		      QGraphicsItem *current = it.next();
		      if (current !=  text) 
			{
			  current->setZValue(current->zValue() + 1);
			}
		    }
		}
	    }
	}
    }
  fixZValues();
}

void EventGraphWidget::objectToFront() 
{
  int maxZ = -1;
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *current = it.next();
      if (maxZ == -1) 
	{
	  maxZ = current->zValue();
	}
      else if (maxZ < current->zValue()) 
	{
	  maxZ = current->zValue();
	}
    }
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  ellipse->setZValue(maxZ + 1);
	}
      else if (rect) 
	{
	  rect->setZValue(maxZ + 1);
	}
      else if (line) 
	{
	  line->setZValue(maxZ + 1);
	}
      else if (text) 
	{
	  text->setZValue(maxZ + 1);
	}
    }
  fixZValues();
}

void EventGraphWidget::objectToBack() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  ellipse->setZValue(1);
	  QListIterator<QGraphicsItem*> it(scene->items());
	  while (it.hasNext()) 
	    {
	      QGraphicsItem *current = it.next();
	      if (current != ellipse) 
		{
		  current->setZValue(current->zValue() + 1);
		}
	    }
	}
      else if (rect) 
	{
	  rect->setZValue(1);
	  QListIterator<QGraphicsItem*> it(scene->items());
	  while (it.hasNext()) 
	    {
	      QGraphicsItem *current = it.next();
	      if (current != rect) 
		{
		  current->setZValue(current->zValue() + 1);
		}
	    }
	}
      else if (line) 
	{
	  line->setZValue(1);
	  QListIterator<QGraphicsItem*> it(scene->items());
	  while (it.hasNext()) 
	    {
	      QGraphicsItem *current = it.next();
	      if (current != line) 
		{
		  current->setZValue(current->zValue() + 1);
		}
	    }
	}
      else if (text) 
	{
	  text->setZValue(1);
	  QListIterator<QGraphicsItem*> it(scene->items());
	  while (it.hasNext()) 
	    {
	      QGraphicsItem *current = it.next();
	      if (current != text) 
		{
		  current->setZValue(current->zValue() + 1);
		}
	    }
	}
    }
  fixZValues();
}

void EventGraphWidget::fixZValues() 
{
  int maxZ = -1;
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *current = it.next();
      if (maxZ == -1) 
	{
	  maxZ = current->zValue();
	}
      else if (maxZ < current->zValue()) 
	{
	  maxZ = current->zValue();
	}
    }
  for (int i = 4; i != maxZ; i++) 
    {
      bool currentZFound = false;
      QListIterator<QGraphicsItem*> it2(scene->items());
      while (it2.hasNext()) 
	{
	  QGraphicsItem* current = it2.next();
	  if (current->zValue() == i) 
	    {
	      currentZFound = true;
	      break;
	    }
	}
      if (!currentZFound) 
	{
	  QListIterator<QGraphicsItem*> it3(scene->items());
	  while (it3.hasNext()) 
	    {
	      QGraphicsItem* current = it3.next();
	      if (current->zValue() > i) 
		{
		  current->setZValue(current->zValue() - 1);
		}
	    }
	}
    }
  setChangeLabel();  
}

void EventGraphWidget::findHeadsLowerBound(QSet<int> *pMark, int currentIncident,
					   int lowerLimit, QString type) 
{
  int currentTail = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM linkages "
		 "WHERE tail = :tail AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :head");
  query->bindValue(":tail", currentTail);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentHead = query->value(0).toInt();
      query2->bindValue(":head", currentHead);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order >= lowerLimit && !pMark->contains(currentHead)) 
	{
	  results.push_back(currentHead);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findHeadsLowerBound(pMark, *it, lowerLimit, type);
    }
}

void EventGraphWidget::findHeadsUpperBound(QSet<int> *pMark, int currentIncident,
					   int upperLimit, QString type) 
{
  int currentTail = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM linkages "
		 "WHERE tail = :tail AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :head");
  query->bindValue(":tail", currentTail);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentHead = query->value(0).toInt();
      query2->bindValue(":head", currentHead);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order <= upperLimit && !pMark->contains(currentHead)) 
	{
	  results.push_back(currentHead);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findHeadsUpperBound(pMark, *it, upperLimit, type);
    }
}

void EventGraphWidget::findTailsUpperBound(QSet<int> *pMark, int currentIncident,
					   int upperLimit, QString type) 
{
  int currentHead = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM linkages "
		 "WHERE head = :head AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :tail");  
  query->bindValue(":head", currentHead);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentTail = query->value(0).toInt();
      query2->bindValue(":tail", currentTail);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order <=  upperLimit && !pMark->contains(currentHead)) 
	{
	  results.push_back(currentTail);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findTailsUpperBound(pMark, *it, upperLimit, type);
    }
}

void EventGraphWidget::findTailsLowerBound(QSet<int> *pMark, int currentIncident,
					   int lowerLimit, QString type) 
{
  int currentHead = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM linkages "
		 "WHERE head = :head AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :tail");  
  query->bindValue(":head", currentHead);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentTail = query->value(0).toInt();
      query2->bindValue(":tail", currentTail);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order >=  lowerLimit && !pMark->contains(currentHead)) 
	{
	  results.push_back(currentTail);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findTailsLowerBound(pMark, *it, lowerLimit, type);
    }
}

QVector<MacroEvent*> EventGraphWidget::getMacros() 
{
  return macroVector;
}

QVector<EventItem*> EventGraphWidget::getEventItems() 
{
  return eventVector;
}

QVector<Arrow*> EventGraphWidget::getEdgeVector() 
{
  return edgeVector;
}

void EventGraphWidget::setAttributesWidget(AttributesWidget *aw) 
{
  attributesWidget = aw;
}

void EventGraphWidget::setOccurrenceGraph(OccurrenceGraphWidget *ogw) 
{
  occurrenceGraph = ogw;
}

void EventGraphWidget::setRelationshipsWidget(RelationshipsWidget *rw) 
{
  relationshipsWidget = rw;
}

void EventGraphWidget::resetTree() 
{
  delete attributesTree;
  setTree();
}

void EventGraphWidget::finalBusiness() 
{
  setComment();
  cleanUp();
}

bool EventGraphWidget::eventFilter(QObject *object, QEvent *event) 
{
  if (event->type() == QEvent::MouseButtonRelease)
    {
      resetZoomSlider();
    }
  if (object == rawField->viewport() && event->type() == QEvent::MouseButtonRelease) 
    {
      selectText();
    }
  else if (object == view->viewport() && event->type() == QEvent::MouseButtonRelease) 
    {
      QMouseEvent *mouseEvent = (QMouseEvent*) event;
      if (mouseEvent->button() == Qt::LeftButton) 
	{
	  retrieveData();
	  setButtons();
	}
    }
  else if (object == attributesTreeView && event->type() == QEvent::ChildRemoved) 
    {
      fixTree();
    }
  else if (object == zoomSlider)
    {
      if (event->type() == QEvent::Wheel ||
	  event->type() == QEvent::MouseButtonDblClick)
	{
	  event->ignore();
	  return true;
	}
      else if (event->type() == QEvent::MouseButtonPress)
	{
	  if (zoomSlider->sliderPosition() == 0)
	    {
	      return false;
	    }
	  else
	    {
	      event->ignore();
	      return true;
	    }
	}
    }
  else if (event->type() == QEvent::Wheel) 
    {
      QWheelEvent *wheelEvent = (QWheelEvent*) event;
      QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
      if (textEdit) 
	{
	  if(wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->angleDelta().y() > 0) 
		{
		  textEdit->zoomIn(1);
		}
	      else if (wheelEvent->angleDelta().y() < 0) 
		{
		  textEdit->zoomOut(1);
		}
	    }
	}
    }
  return false;
}
