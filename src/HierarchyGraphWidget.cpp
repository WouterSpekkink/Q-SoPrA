/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../include/HierarchyGraphWidget.h"
                      
HierarchyGraphWidget::HierarchyGraphWidget(QWidget *parent) : QDialog(parent) 
{  
  _selectedAbstractNode = NULL;
  _selectedIncident = 0;
  _currentPenStyle = 1;
  _currentPenWidth = 1;
  _currentLineColor = QColor(Qt::black);
  _currentFillColor = QColor(Qt::black);
  _currentFillColor.setAlpha(0);
  
  scene = new Scene(this);
  view = new BandlessGraphicsView(scene);
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

  timeStampLabel = new QLabel("<b>Timing:</b>", infoWidget);
  sourceLabel = new QLabel("<b>Source:</b>", infoWidget);
  descriptionLabel = new QLabel("<b>Description:</b>", infoWidget);
  rawLabel = new QLabel("<b>Raw:</b>", infoWidget);
  commentLabel = new QLabel("<b>Comments:</b>", commentWidget);
  attributesLabel = new QLabel(tr("<b>Attributes:</b>"), attWidget);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), attWidget);
  valueLabel = new QLabel(tr("<b>Value:</b>"), attWidget);
  legendLabel = new QLabel(tr("<b>Modes:</b>"), legendWidget);
  zoomLabel = new QLabel(tr("<b>Zoom slider:</b>"), this);
  shapesLabel = new QLabel(tr("<b>Shapes:</b>"), this);
  penStyleLabel = new QLabel(tr("<b>Pen style:</b>"), this);
  penWidthLabel = new QLabel(tr("<b>Pen width:</b>"), this);
  lineColorLabel = new QLabel(tr("<b>Line / Text color:</b>"), this);
  fillColorLabel = new QLabel(tr("<b>Fill color:</b>"), this);	
  fillOpacityLabel = new QLabel(tr("<b>Opacity:</b>"), this);
  guideLinesLabel = new QLabel(tr("<b>Add guides:</b>"), this);
  labelSizeLabel = new QLabel(tr("<b>Label size:</b>"), graphicsWidget);
  
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

  linkageListWidget = new DeselectableListWidget(legendWidget);
  linkageListWidget->setColumnCount(2);
  linkageListWidget->horizontalHeader()->hide();
  linkageListWidget->verticalHeader()->hide();
  linkageListWidget->setColumnWidth(1, 20);
  linkageListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  linkageListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  linkageListWidget->setStyleSheet("QTableView {gridline-color: black}");
  linkageListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  
  zoomSlider = new QSlider(Qt::Horizontal, this);
  zoomSlider->installEventFilter(this);
  zoomSlider->setMinimum(-9);
  zoomSlider->setMaximum(9);
  zoomSlider->setValue(0);
  
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
  addModeButton = new QPushButton(tr("Create mode"), legendWidget);
  removeModeButton = new QPushButton(tr("Remove mode"), legendWidget);
  removeModeButton->setEnabled(false);
  moveModeUpButton = new QPushButton(tr("Up"), legendWidget);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton = new QPushButton(tr("Down"), legendWidget);
  moveModeDownButton->setEnabled(false);
  restoreModeColorsButton = new QPushButton(tr("Restore colors"), legendWidget);
  hideLinkageTypeButton = new QPushButton(tr("Hide"), legendWidget);
  hideLinkageTypeButton->setEnabled(false);
  showLinkageTypeButton = new QPushButton(tr("Show"), legendWidget);
  showLinkageTypeButton->setEnabled(false);
  increaseLabelSizeButton = new QPushButton(tr("+"), graphicsWidget);
  decreaseLabelSizeButton = new QPushButton(tr("-"), graphicsWidget);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);

  addLineButton = new QPushButton(QIcon("./images/line_object.png"), "", this);
  addLineButton->setIconSize(QSize(20, 20));
  addLineButton->setMinimumSize(40, 40);
  addLineButton->setMaximumSize(40, 40);
  addSingleArrowButton = new QPushButton(QIcon("./images/single_arrow_object.png"), "", this);
  addSingleArrowButton->setIconSize(QSize(20, 20));
  addSingleArrowButton->setMinimumSize(40, 40);
  addSingleArrowButton->setMaximumSize(40, 40);
  addDoubleArrowButton = new QPushButton(QIcon("./images/double_arrow_object.png"), "", this);
  addDoubleArrowButton->setIconSize(QSize(20, 20));
  addDoubleArrowButton->setMinimumSize(40, 40);
  addDoubleArrowButton->setMaximumSize(40, 40);
  addEllipseButton = new QPushButton(QIcon("./images/ellipse_object.png"), "", this);
  addEllipseButton->setIconSize(QSize(20, 20));
  addEllipseButton->setMinimumSize(40, 40);
  addEllipseButton->setMaximumSize(40, 40);
  addRectangleButton = new QPushButton(QIcon("./images/rect_object.png"), "", this);
  addRectangleButton->setIconSize(QSize(20, 20));
  addRectangleButton->setMinimumSize(40, 40);
  addRectangleButton->setMaximumSize(40, 40);
  addTextButton = new QPushButton(QIcon("./images/text_object.png"), "", this);
  addTextButton->setIconSize(QSize(20, 20));
  addTextButton->setMinimumSize(40, 40);
  addTextButton->setMaximumSize(40, 40);
  QPixmap lineColorMap(20, 20);
  lineColorMap.fill(_currentLineColor);
  QIcon lineColorIcon(lineColorMap);
  changeLineColorButton = new QPushButton(lineColorIcon, "", this);
  changeLineColorButton->setIconSize(QSize(20, 20));
  changeLineColorButton->setMinimumSize(40, 40);
  changeLineColorButton->setMaximumSize(40, 40);
  QPixmap fillColorMap(20, 20);
  QColor tempFill = _currentFillColor;
  tempFill.setAlpha(255);
  fillColorMap.fill(tempFill);
  QIcon fillColorIcon(fillColorMap);
  changeFillColorButton = new QPushButton(fillColorIcon, "", this);
  changeFillColorButton->setIconSize(QSize(20, 20));
  changeFillColorButton->setMinimumSize(40, 40);
  changeFillColorButton->setMaximumSize(40, 40);
  fillOpacitySlider = new QSlider(Qt::Horizontal, this);
  fillOpacitySlider->setMinimum(0);
  fillOpacitySlider->setMaximum(255);
  fillOpacitySlider->setValue(0);
  fillOpacitySlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
  addHorizontalGuideLineButton = new QPushButton(QIcon("./images/guide_horizontal.png"), "", this);
  addHorizontalGuideLineButton->setIconSize(QSize(20, 20));
  addHorizontalGuideLineButton->setMinimumSize(40, 40);
  addHorizontalGuideLineButton->setMaximumSize(40, 40);
  addVerticalGuideLineButton = new QPushButton(QIcon("./images/guide_vertical.png"), "", this);
  addVerticalGuideLineButton->setIconSize(QSize(20, 20));
  addVerticalGuideLineButton->setMinimumSize(40, 40);
  addVerticalGuideLineButton->setMaximumSize(40, 40);
  snapGuidesButton = new QPushButton(tr("Toggle snap guides"), this);
  snapGuidesButton->setCheckable(true);

  
  penStyleComboBox = new QComboBox(this);
  penStyleComboBox->addItem("Solid");
  penStyleComboBox->setItemIcon(0, QIcon("./images/solid_line.png"));
  penStyleComboBox->addItem("Dashed");
  penStyleComboBox->setItemIcon(1, QIcon("./images/dashed_line.png"));
  penStyleComboBox->addItem("Dotted");
  penStyleComboBox->setItemIcon(2, QIcon("./images/dotted_line.png"));
  penStyleComboBox->addItem("Dash Dot");
  penStyleComboBox->setItemIcon(3, QIcon("./images/dash_dot_line.png"));
  penStyleComboBox->addItem("Dash Dot Dot");
  penStyleComboBox->setItemIcon(4, QIcon("./images/dash_dot_dot_line.png"));
  penWidthSpinBox = new QSpinBox(this);
  penWidthSpinBox->setMinimum(1);
  penWidthSpinBox->setMaximum(20);
  
  rawField->viewport()->installEventFilter(infoWidget);
  view->viewport()->installEventFilter(this);
  attributesTreeView->installEventFilter(this);
  commentField->installEventFilter(commentWidget);
  
  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(seeAttributesButton, SIGNAL(clicked()), this, SLOT(showAttributes()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(valueField, SIGNAL(textChanged(const QString &)), this, SLOT(setValueButton()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(seeCommentsButton, SIGNAL(clicked()), this, SLOT(showComments()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(addLineButton, SIGNAL(clicked()), scene, SLOT(prepLinePoints()));
  connect(addSingleArrowButton, SIGNAL(clicked()), scene, SLOT(prepSingleArrowPoints()));
  connect(addDoubleArrowButton, SIGNAL(clicked()), scene, SLOT(prepDoubleArrowPoints()));
  connect(addEllipseButton, SIGNAL(clicked()), scene, SLOT(prepEllipseArea()));
  connect(addRectangleButton, SIGNAL(clicked()), scene, SLOT(prepRectArea()));
  connect(addTextButton, SIGNAL(clicked()), scene, SLOT(prepTextArea()));
  connect(penStyleComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPenStyle()));
  connect(penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPenWidth()));
  connect(penStyleComboBox, SIGNAL(currentIndexChanged(int)), scene, SLOT(setPenStyle(int)));
  connect(penWidthSpinBox, SIGNAL(valueChanged(int)), scene, SLOT(setPenWidth(int)));
  connect(changeLineColorButton, SIGNAL(clicked()), this, SLOT(setLineColor()));
  connect(changeFillColorButton, SIGNAL(clicked()), this, SLOT(setFillColor()));
  connect(fillOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(setFillOpacity(int)));
  connect(addHorizontalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepHorizontalGuideLine()));
  connect(addVerticalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepVerticalGuideLine()));
  connect(snapGuidesButton, SIGNAL(clicked()), this, SLOT(toggleSnapGuides()));
  connect(this, SIGNAL(sendLineColor(QColor &)), scene, SLOT(setLineColor(QColor &)));
  connect(this, SIGNAL(sendFillColor(QColor &)), scene, SLOT(setFillColor(QColor &)));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(highlightText()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(attributesTreeView, SIGNAL(noneSelected()), this, SLOT(setButtons())); 
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
  	  this, SLOT(changeFilter(const QString &)));
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(addModeButton, SIGNAL(clicked()), this, SLOT(addMode()));
  connect(removeModeButton, SIGNAL(clicked()), this, SLOT(removeMode()));
  connect(restoreModeColorsButton, SIGNAL(clicked()), this, SLOT(restoreModeColors()));
  connect(moveModeUpButton, SIGNAL(clicked()), this, SLOT(moveModeUp()));
  connect(moveModeDownButton, SIGNAL(clicked()), this, SLOT(moveModeDown()));
  connect(increaseLabelSizeButton, SIGNAL(clicked()), this, SLOT(increaseLabelSize()));
  connect(decreaseLabelSizeButton, SIGNAL(clicked()), this, SLOT(decreaseLabelSize()));
  connect(eventListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setModeButtons(QTableWidgetItem *)));
  connect(eventListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeModeColor(QTableWidgetItem *)));
  connect(eventListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableModeButtons()));
  connect(linkageListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setLinkageButtons(QTableWidgetItem *)));
  connect(linkageListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableLinkageButtons()));
  connect(linkageListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeLinkageColor(QTableWidgetItem *)));
  connect(hideLinkageTypeButton, SIGNAL(clicked()), this, SLOT(hideLinkage()));
  connect(showLinkageTypeButton, SIGNAL(clicked()), this, SLOT(showLinkage()));
  connect(scene, SIGNAL(LineContextMenuAction(const QString &)),
	  this, SLOT(processLineContextMenu(const QString &)));
  connect(scene, SIGNAL(TextContextMenuAction(const QString &)),
	  this, SLOT(processTextContextMenu(const QString &)));
  connect(scene, SIGNAL(EllipseContextMenuAction(const QString &)),
	  this, SLOT(processEllipseContextMenu(const QString &)));
  connect(scene, SIGNAL(RectContextMenuAction(const QString &)),
	  this, SLOT(processRectContextMenu(const QString &)));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(scene, SIGNAL(sendLinePoints(const QPointF&, const QPointF&)),
	  this, SLOT(addLineObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(GuideLineContextMenuAction(const QString &)),
	  this, SLOT(processGuideLineContextMenu(const QString &)));
  connect(scene, SIGNAL(sendSingleArrowPoints(const QPointF&, const QPointF&)),
	  this, SLOT(addSingleArrowObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(sendDoubleArrowPoints(const QPointF&, const QPointF&)),
	  this, SLOT(addDoubleArrowObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(sendHorizontalGuideLinePos(const QPointF&)),
	  this, SLOT(addHorizontalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(sendVerticalGuideLinePos(const QPointF&)),
	  this, SLOT(addVerticalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(sendEllipseArea(const QRectF&)), this, SLOT(addEllipseObject(const QRectF&)));
  connect(scene, SIGNAL(sendRectArea(const QRectF&)), this, SLOT(addRectObject(const QRectF&)));
  connect(scene, SIGNAL(sendTextArea(const QRectF&, const qreal&)),
	  this, SLOT(addTextObject(const QRectF&, const qreal&)));
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(processShapeSelection()));  
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(processZoomSliderChange(int)));
  connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(resetZoomSlider()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
  connect(exitButton, SIGNAL(clicked()), this, SLOT(switchBack()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> plotObjectsLayout = new QHBoxLayout;
  plotObjectsLayout->addWidget(shapesLabel);
  plotObjectsLayout->addWidget(addLineButton);
  plotObjectsLayout->addWidget(addSingleArrowButton);
  plotObjectsLayout->addWidget(addDoubleArrowButton);
  plotObjectsLayout->addWidget(addEllipseButton);
  plotObjectsLayout->addWidget(addRectangleButton);
  plotObjectsLayout->addWidget(addTextButton);
  plotObjectsLayout->addWidget(penStyleLabel);
  plotObjectsLayout->addWidget(penStyleComboBox);
  plotObjectsLayout->addWidget(penWidthLabel);
  plotObjectsLayout->addWidget(penWidthSpinBox);
  plotObjectsLayout->addWidget(lineColorLabel);
  plotObjectsLayout->addWidget(changeLineColorButton);
  plotObjectsLayout->addWidget(fillColorLabel);
  plotObjectsLayout->addWidget(changeFillColorButton);
  plotObjectsLayout->addWidget(fillOpacityLabel);
  plotObjectsLayout->addWidget(fillOpacitySlider);
  plotObjectsLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(plotObjectsLayout);
  
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
  QPointer<QHBoxLayout> modeButtonsLayout = new QHBoxLayout;
  modeButtonsLayout->addWidget(moveModeUpButton);
  modeButtonsLayout->addWidget(moveModeDownButton);
  legendLayout->addLayout(modeButtonsLayout);
  legendLayout->addWidget(addModeButton);
  legendLayout->addWidget(removeModeButton);
  legendLayout->addWidget(restoreModeColorsButton);
  legendLayout->addWidget(linkageListWidget);
  legendLayout->addWidget(hideLinkageTypeButton);
  legendLayout->addWidget(showLinkageTypeButton);
  legendWidget->setMinimumWidth(250);
  legendWidget->setMaximumWidth(250);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				   

  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(backgroundColorButton);
  graphicsControlsLayout->addWidget(labelSizeLabel);
  QPointer<QHBoxLayout> labelSizeLayout = new QHBoxLayout;
  labelSizeLayout->addWidget(increaseLabelSizeButton);
  labelSizeLayout->addWidget(decreaseLabelSizeButton);
  graphicsControlsLayout->addLayout(labelSizeLayout);
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
  drawOptionsLeftLayout->addWidget(zoomLabel);
  zoomLabel->setMaximumWidth(zoomLabel->sizeHint().width());
  drawOptionsLeftLayout->addWidget(zoomSlider);
  zoomSlider->setMaximumWidth(100);
  QPointer<QHBoxLayout> guidesLayout = new QHBoxLayout;
  guidesLayout->addWidget(guideLinesLabel);
  guidesLayout->addWidget(addHorizontalGuideLineButton);
  guidesLayout->addWidget(addVerticalGuideLineButton);
  guidesLayout->addWidget(snapGuidesButton);
  guidesLayout->setAlignment(Qt::AlignLeft);
  drawOptionsLeftLayout->addLayout(guidesLayout);
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(exportSvgButton);
  drawOptionsRightLayout->addWidget(exitButton);
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);
    
  setLayout(mainLayout);
  graphicsWidget->hide();
  attWidget->hide();
  legendWidget->hide();
}

HierarchyGraphWidget::~HierarchyGraphWidget()
{
  _abstractNodeVector.clear();
  _incidentNodeVector.clear();
  qDeleteAll(_abstractNodeLabelVector);
  _abstractNodeLabelVector.clear();
  qDeleteAll(_incidentNodeLabelVector);
  _incidentNodeLabelVector.clear();
  qDeleteAll(_tempEdges);
  _tempEdges.clear();
  _edgeVector.clear();
  qDeleteAll(_lineVector);
  _lineVector.clear();
  qDeleteAll(_textVector);
  _textVector.clear();
  qDeleteAll(_ellipseVector);
  _ellipseVector.clear();
  qDeleteAll(_rectVector);
  _rectVector.clear();
  qDeleteAll(_guidesVector);
  _guidesVector.clear();
  delete view;
  delete scene;
}

void HierarchyGraphWidget::setOpenGL(bool state)
{
  if (state == true)
    {
      QPointer<QOpenGLWidget> openGL = new QOpenGLWidget(this);
      QSurfaceFormat format;
      format.setSamples(4);
      format.setDepthBufferSize(24);
      format.setStencilBufferSize(8);
      openGL->setFormat(format);
      view->setViewport(openGL);
      view->viewport()->installEventFilter(this);
      view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    }
  else
    {
      view->setViewport(new QWidget());
      view->viewport()->installEventFilter(this);
      view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    }
}

void HierarchyGraphWidget::setAntialiasing(bool state)
{
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext())
    {
      Linkage *current = it.next();
      current->setAntialiasing(state);
    }
}

void HierarchyGraphWidget::toggleDetails() 
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

void HierarchyGraphWidget::toggleGraphicsControls() 
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

void HierarchyGraphWidget::toggleLegend() 
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

void HierarchyGraphWidget::rescale()
{
  view->scale(2.0, 2.0);
  view->scale(0.5, 0.5);
}

void HierarchyGraphWidget::showAttributes() 
{
  commentWidget->hide();
  attWidget->show();
}

void HierarchyGraphWidget::showComments() 
{
  attWidget->hide();
  commentWidget->show();
}

void HierarchyGraphWidget::processZoomSliderChange(int value)
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

void HierarchyGraphWidget::resetZoomSlider()
{
  zoomSlider->blockSignals(true);
  zoomSlider->setValue(0);
  zoomSlider->blockSignals(false);
}

void HierarchyGraphWidget::setValueButton()
{
  valueButton->setEnabled(true);
}

void HierarchyGraphWidget::setValue()
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString attribute = attributesTreeView->currentIndex().data().toString();
      if (_selectedIncident != 0)
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE attributes_to_incidents "
			 "SET value = :val "
			 "WHERE attribute = :attribute AND incident = :incident");
	  query->bindValue(":val", valueField->text());
	  query->bindValue(":attribute", attribute);
	  query->bindValue(":incident", _selectedIncident);
	  query->exec();
	  valueButton->setEnabled(false);
	  delete query;
	}
      else if (_selectedAbstractNode != NULL)
	{
	  _selectedAbstractNode->insertValue(attribute, valueField->text());
	  valueButton->setEnabled(false);
	}
    }
}

void HierarchyGraphWidget::setCommentBool() 
{
  _commentBool = true;
}

void HierarchyGraphWidget::setComment() 
{
  if (_commentBool && _selectedIncident != 0) 
    {
      QString comment = commentField->toPlainText();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
      query->bindValue(":incident", _selectedIncident);
      query->exec();
      query->first();
      int order = 0;
      order = query->value(0).toInt();
      query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :order");
      query->bindValue(":comment", comment);
      query->bindValue(":order", order);
      query->exec();
      _commentBool = false;
      delete query;
    }
  else if (_commentBool && _selectedAbstractNode != NULL) 
    {
      QString comment = commentField->toPlainText();
      _selectedAbstractNode->setComment(comment);
      QVectorIterator<AbstractNode*> it(_abstractNodeVector);
      while (it.hasNext()) 
	{
	  AbstractNode *abstractNode = it.next();
	  if (abstractNode->getId() == _selectedAbstractNode->getId()) 
	    {
	      abstractNode->setComment(comment);
	    }
	}
    }
}

void HierarchyGraphWidget::retrieveData() 
{
  setComment();
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->setEnabled(false);
  valueField->blockSignals(false);
  valueButton->setEnabled(false);
  attributesTreeView->clearSelection();
  if (_currentData.size() > 0) 
    {
      _currentData.clear();
    }
  if (scene->selectedItems().size() > 0) 
    {
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) 
	{
	  IncidentNode *event = qgraphicsitem_cast<IncidentNode*>(it.peekNext());
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(it.peekNext());
	  if (event) 
	    {
	      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(it.next());
	      currentIncidentNode->setSelectionColor(Qt::black);
	      currentIncidentNode->update();
	      if (currentIncidentNode->isVisible()) 
		{
		  _currentData.push_back(currentIncidentNode);
		}
	    }	else if (abstractNode) 
	    {
	      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(it.next());
	      currentAbstractNode->setSelectionColor(Qt::black);
	      currentAbstractNode->update();
	      if (currentAbstractNode->isVisible()) 
		{
		  _currentData.push_back(currentAbstractNode);
		}
	    }
	  else 
	    {
	      it.next();
	    }
	}
      if (_currentData.size() > 0) 
	{
	  std::sort(_currentData.begin(), _currentData.end(), eventLessThan);
	  QGraphicsItem *temp = _currentData[0];
	  scene->clearSelection();
	  temp->setSelected(true);
	  _currentData.clear();
	  _currentData.push_back(temp);
	  IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(_currentData[0]);
	  AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData[0]);
	  if (currentIncidentNode) 
	    {
	      _selectedAbstractNode = NULL;
	      sourceLabel->setText("<b>Source:</b>");
	      rawLabel->show();
	      rawField->show();
	      currentIncidentNode->setSelectionColor(Qt::red);
	      currentIncidentNode->update();
	      int id = currentIncidentNode->getId();
	      _selectedIncident = id;
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
	      delete query;
	    }
	  else if (currentAbstractNode) 
	    {
	      _selectedAbstractNode = currentAbstractNode;
	      _selectedIncident = 0;
	      currentAbstractNode->setSelectionColor(Qt::red);
	      currentAbstractNode->update();
	      descriptionField->setText(currentAbstractNode->getDescription());
	      sourceLabel->setText("<b>Number of components:</b>");
	      int id = currentAbstractNode->getIncidents().first()->getId();
	      rawLabel->hide();
	      rawField->hide();
	      QString timestamp = currentAbstractNode->getTiming();
	      int count = currentAbstractNode->getIncidents().size();
	      QVectorIterator<AbstractNode*> it(_abstractNodeVector);
	      while (it.hasNext()) 
		{
		  AbstractNode *temp = it.next();
		  if (temp->getAbstractNode() == currentAbstractNode) 
		    {
		      count++;
		    }
		}
	      QString countText = QString::number(count);
	      timeStampField->setText(timestamp);
	      sourceField->setText(countText);
	      commentField->setText(currentAbstractNode->getComment());
	      resetFont(attributesTree);
	      QSet<QString> attributes = currentAbstractNode->getAttributes();
	      QSet<QString>::iterator it2;
	      id = currentAbstractNode->getId();
	      for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
		{
		  QString attribute  = *it2;
		  boldSelected(attributesTree, attribute, id, ABSTRACTNODE);
		}
	    }
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
      _selectedIncident = 0;
      _selectedAbstractNode = NULL;
      resetFont(attributesTree);
    }
}

void HierarchyGraphWidget::buildComponents(AbstractNode *origin, int layer) 
{
  AbstractNode *newOrigin = new AbstractNode(40, origin->getDescription(), QPointF(0,0),
					 origin->getId(),
					 origin->getConstraint(),
					 origin->getIncidents());
  newOrigin->setCopy(true);
  newOrigin->setMode(origin->getMode());
  newOrigin->setColor(origin->getColor());
  newOrigin->setAttributes(origin->getAttributes());
  newOrigin->setOrder(origin->getOrder());
  newOrigin->setZValue(3);
  _tempAbstractNodeVector.push_back(newOrigin);
  AbstractNodeLabel *abstractNodeLabel = new AbstractNodeLabel(newOrigin);
  _abstractNodeLabelVector.push_back(abstractNodeLabel);
  newOrigin->setLabel(abstractNodeLabel);
  QTableWidgetItem *item = new QTableWidgetItem(newOrigin->getMode());
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  if (newOrigin->getMode() != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT description FROM incident_attributes "
		     "WHERE name = :name");
      query->bindValue(":name", newOrigin->getMode());
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      delete query;
      QString toolTip = breakString(newOrigin->getMode()  + " - " + description);
      item->setToolTip(toolTip);
      item->setData(Qt::DisplayRole, newOrigin->getMode());
      eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
      eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
      eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setBackground(newOrigin->getColor());
      QVariant textColorVar = QVariant(newOrigin->getLabel()->defaultTextColor().rgb());
      eventListWidget->item(eventListWidget->rowCount() - 1, 1)
	->setData(Qt::UserRole, textColorVar);
      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    }
  else 
    {
      delete item;
    }
  if (newOrigin->getConstraint() == PATHS ||
      newOrigin->getConstraint() == PATHSATT) 
    {
      QString label = "P-" + QString::number(newOrigin->getOrder());
      abstractNodeLabel->setPlainText(label);
    }
  else if (newOrigin->getConstraint() == SEMIPATHS ||
	   newOrigin->getConstraint() == SEMIPATHSATT) 
    {
      QString label = "S-" + QString::number(newOrigin->getOrder());
      abstractNodeLabel->setPlainText(label);
    }
  else if (newOrigin->getConstraint() == NOCONSTRAINT ||
	   newOrigin->getConstraint() == NOCONSTRAINTATT) 
    {
      QString label = "N-" + QString::number(newOrigin->getOrder());
      abstractNodeLabel->setPlainText(label);
    }
  newOrigin->setPos(QPointF(newOrigin->scenePos().x(),
			    newOrigin->scenePos().y() - layer * 80));
  newOrigin->setOriginalPos(newOrigin->scenePos());
  qreal xOffset = (newOrigin->getWidth() / 2) - 20;
  abstractNodeLabel->setOffset(QPointF(xOffset,0));
  abstractNodeLabel->setNewPos(newOrigin->scenePos());
  abstractNodeLabel->setZValue(4);
  abstractNodeLabel->setDefaultTextColor(Qt::black);
  abstractNodeLabel->setFontSize(_labelSize);
  scene->addItem(newOrigin);
  scene->addItem(abstractNodeLabel);
  qreal yPos = 0 + (layer * 80);
  QVector<QGraphicsItem*> currentLayer;
  QVector<QGraphicsItem*> partners;
  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
  while (it.hasNext()) 
    {
      AbstractNode *abstractNode = it.next();
      if (abstractNode->getAbstractNode() == origin) 
	{ 
	  AbstractNode *newAbstractNode = new AbstractNode(40, abstractNode->getDescription(),
							   QPointF(0, yPos),
							   abstractNode->getId(),
							   abstractNode->getConstraint(),
							   abstractNode->getIncidents());
	  newAbstractNode->setCopy(true);
	  newAbstractNode->setMode(abstractNode->getMode());
	  newAbstractNode->setColor(abstractNode->getColor());
	  newAbstractNode->setAttributes(abstractNode->getAttributes());
	  newAbstractNode->setPos(newAbstractNode->getOriginalPos());
	  newAbstractNode->setOrder(abstractNode->getOrder());
	  newAbstractNode->setZValue(3);
	  _tempAbstractNodeVector.push_back(newAbstractNode);
	  AbstractNodeLabel *newAbstractNodeLabel = new AbstractNodeLabel(newAbstractNode);
	  _abstractNodeLabelVector.push_back(newAbstractNodeLabel);
	  newAbstractNode->setLabel(newAbstractNodeLabel);
	  if (newAbstractNode->getConstraint() == PATHS ||
	      newAbstractNode->getConstraint() == PATHSATT) 
	    {
	      QString label = "P-" + QString::number(newAbstractNode->getOrder());
	      newAbstractNodeLabel->setPlainText(label);
	    }
	  else if (newAbstractNode->getConstraint() == SEMIPATHS ||
		   newAbstractNode->getConstraint() == SEMIPATHSATT) 
	    {
	      QString label = "S-" + QString::number(newAbstractNode->getOrder());
	      newAbstractNodeLabel->setPlainText(label);
	    }
	  else if (newAbstractNode->getConstraint() == NOCONSTRAINT ||
		   newAbstractNode->getConstraint() == NOCONSTRAINTATT) 
	    {
	      QString label = "N-" + QString::number(newAbstractNode->getOrder());
	      newAbstractNodeLabel->setPlainText(label);
	    }
	  qreal xOffset = (newAbstractNode->getWidth() / 2) - 20;
	  newAbstractNodeLabel->setOffset(QPointF(xOffset,0));
	  newAbstractNodeLabel->setNewPos(newAbstractNode->scenePos());
	  newAbstractNodeLabel->setZValue(4);
	  newAbstractNodeLabel->setDefaultTextColor(Qt::black);
	  newAbstractNodeLabel->setFontSize(_labelSize);
	  currentLayer.push_back(newAbstractNode);
	  bool found = false;
	  if (newAbstractNode->getMode() != "") 
	    {
	      for (int i = 0; i < eventListWidget->rowCount(); i++) 
		{
		  if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newAbstractNode->getMode()) 
		    {
		      found = true;
		    }
		}
	      if (!found) 
		{
		  QSqlQuery *query = new QSqlQuery;
		  query->prepare("SELECT description FROM incident_attributes "
				 "WHERE name = :name");
		  query->bindValue(":name", newAbstractNode->getMode());
		  query->exec();
		  query->first();
		  QString description = query->value(0).toString();
		  delete query;
		  QTableWidgetItem *item = new QTableWidgetItem(newAbstractNode->getMode());
		  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		  QString toolTip = breakString(newAbstractNode->getMode() + " - " + description);
		  item->setToolTip(toolTip);
		  item->setData(Qt::DisplayRole, newAbstractNode->getMode());
		  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setBackground(newAbstractNode->getColor());
		  QVariant textColorVar = QVariant(newAbstractNode->getLabel()->
						   defaultTextColor().rgb());
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setData(Qt::UserRole,
										     textColorVar);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		}
	    }
	  Linkage *newLinkage = new Linkage("Hierarchy", "", 0);
	  _tempEdges.push_back(newLinkage);
	  newLinkage->setZValue(2);
	  newLinkage->setStartItem(newAbstractNode);
	  newLinkage->setEndItem(newOrigin);
	  newLinkage->setCopy(true);
	  newLinkage->setColor(QColor(Qt::gray));
	  scene->addItem(newLinkage);
	  partners.push_back(abstractNode);
	}
    }
  QVectorIterator<IncidentNode*> it2(_incidentNodeVector);
  while (it2.hasNext()) 
    {
      IncidentNode *event = it2.next();
      if (event->getAbstractNode() == origin) 
	{
	  IncidentNode *newEvent = new IncidentNode(40, event->data(Qt::ToolTipRole).toString(),
					      QPointF(0, yPos), event->getId(), event->getOrder());
	  newEvent->setCopy(true);
	  newEvent->setMode(event->getMode());
	  newEvent->setColor(event->getColor());
	  newEvent->setPos(newEvent->getOriginalPos());
	  newEvent->setZValue(3);
	  _tempIncidentNodeVector.push_back(newEvent);
	  QPointer<IncidentNodeLabel> text = new IncidentNodeLabel(newEvent);
	  _incidentNodeLabelVector.push_back(text);
	  newEvent->setLabel(text);
	  text->setPlainText(QString::number(newEvent->getOrder()));
	  QPointF currentPos = newEvent->scenePos();
	  currentPos.setX(currentPos.x() - (text->textWidth() / 2));
	  currentPos.setY(currentPos.y() -12);
	  text->setPos(currentPos);    
	  text->setZValue(4);
	  text->setDefaultTextColor(Qt::black);
	  text->setFontSize(_labelSize);
	  currentLayer.push_back(newEvent);
	  bool found = false;
	  if (newEvent->getMode() != "") 
	    {
	      for (int i = 0; i < eventListWidget->rowCount(); i++) 
		{
		  if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newEvent->getMode()) 
		    {
		      found = true;
		    }
		}
	      if (!found) 
		{
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
		  QString toolTip = breakString(newEvent->getMode() + " - " + description);
		  item->setToolTip(toolTip);
		  item->setData(Qt::DisplayRole, newEvent->getMode());
		  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setBackground(newEvent->getColor());
		  QVariant textColorVar = QVariant(newEvent->getLabel()->defaultTextColor().rgb());
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setData(Qt::UserRole,
										     textColorVar);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		}
	    }
	  Linkage *newLinkage = new Linkage("Hierarchy", "", 0);
	  _tempEdges.push_back(newLinkage);
	  newLinkage->setZValue(2);
	  newLinkage->setStartItem(newEvent);
	  newLinkage->setEndItem(newOrigin);
	  newLinkage->setCopy(true);
	  newLinkage->setColor(QColor(Qt::gray));
	  scene->addItem(newLinkage);
	  partners.push_back(event);
	}
    }
  std::sort(currentLayer.begin(), currentLayer.end(), componentsSort);
  std::sort(partners.begin(), partners.end(), componentsSort);
  int layerSize = currentLayer.size();
  qreal width = 120 * layerSize;
  qreal startX = 0 - width / 2 - 60;
  int count = 1;
  QVector<AbstractNode*> newLayer;
  QVector<AbstractNode*> partnerLayer;
  for (QVector<QGraphicsItem*>::size_type it3 = 0; it3 != currentLayer.size(); it3++) 
    {
      QGraphicsItem *current = currentLayer[it3];
      qreal xPos = startX + count * 120;
      current->setPos(xPos, current->scenePos().y());
      count++;
      IncidentNode *event = qgraphicsitem_cast<IncidentNode*>(current);
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      if (event) 
	{
	  event->setOriginalPos(event->scenePos());
	  event->getLabel()->setNewPos(event->scenePos());
	  scene->addItem(event);
	  scene->addItem(event->getLabel());
	}
      else if (abstractNode) 
	{
	  abstractNode->setOriginalPos(abstractNode->scenePos());
	  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
	  scene->addItem(abstractNode);
	  scene->addItem(abstractNode->getLabel());
	  AbstractNode *tempAbstractNode = qgraphicsitem_cast<AbstractNode*>(partners[it3]);
	  newLayer.push_back(tempAbstractNode);
	  partnerLayer.push_back(qgraphicsitem_cast<AbstractNode*>(current));
	}
    }
  if (newLayer.size() > 0) 
    {
      addLayer(newLayer, partnerLayer, layer + 1);
    }
}

void HierarchyGraphWidget::addLayer(QVector<AbstractNode*> presentLayer,
				    QVector<AbstractNode*> partLayer,
				    int layer) 
{
  qreal yPos = 0 + (layer * 80);
  QVector<QGraphicsItem*> currentLayer;
  QVector<QGraphicsItem*> partners;
  for (QVector<AbstractNode*>::size_type it = 0; it != presentLayer.size(); it++) 
    {
      AbstractNode *currentFather = presentLayer[it];
      QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
      while (it2.hasNext()) 
	{
	  AbstractNode *abstractNode = it2.next();
	  if (abstractNode->getAbstractNode() == currentFather) 
	    { 
	      AbstractNode *newAbstractNode = new AbstractNode(40,
							       abstractNode->getDescription(),
							       QPointF(0,yPos),
							       abstractNode->getId(),
							       abstractNode->getConstraint(),
							       abstractNode->getIncidents());
	      newAbstractNode->setCopy(true);
	      newAbstractNode->setMode(abstractNode->getMode());
	      newAbstractNode->setColor(abstractNode->getColor());
	      newAbstractNode->setAttributes(abstractNode->getAttributes());
	      newAbstractNode->setPos(newAbstractNode->getOriginalPos());
	      newAbstractNode->setOrder(abstractNode->getOrder());
	      newAbstractNode->setZValue(3);
	      _tempAbstractNodeVector.push_back(newAbstractNode);
	      bool found = false;
	      if (newAbstractNode->getMode() != "") 
		{
		  QSqlQuery *query = new QSqlQuery;
		  query->prepare("SELECT description FROM incident_attributes "
				 "WHERE name = :name");
		  query->bindValue(":name", newAbstractNode->getMode());
		  query->exec();
		  query->first();
		  QString description = query->value(0).toString();
		  delete query;
		  for (int i = 0; i < eventListWidget->rowCount(); i++) 
		    {
		      if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newAbstractNode->getMode()) 
			{
			  found = true;
			}
		    }
		  if (!found) 
		    {
		      QTableWidgetItem *item = new QTableWidgetItem(newAbstractNode->getMode());
		      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		      QString toolTip = breakString(newAbstractNode->getMode() + " - " + description);
		      item->setToolTip(toolTip);
		      item->setData(Qt::DisplayRole, newAbstractNode->getMode());
		      eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
		      eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
		      eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
		      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
			setBackground(newAbstractNode->getColor());
		      QVariant textColorVar = QVariant(newAbstractNode->getLabel()
						       ->defaultTextColor().rgb());
		      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
			setData(Qt::UserRole, textColorVar);
		      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
			setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
				 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		    }
		}
	      AbstractNodeLabel *newAbstractNodeLabel = new AbstractNodeLabel(newAbstractNode);
	      _abstractNodeLabelVector.push_back(newAbstractNodeLabel);
	      newAbstractNode->setLabel(newAbstractNodeLabel);
	      if (newAbstractNode->getConstraint() == PATHS ||
		  newAbstractNode->getConstraint() == PATHSATT) 
		{
		  QString label = "P-" + QString::number(newAbstractNode->getOrder());
		  newAbstractNodeLabel->setPlainText(label);
		}
	      else if (newAbstractNode->getConstraint() == SEMIPATHS ||
		       newAbstractNode->getConstraint() == SEMIPATHSATT) 
		{
		  QString label = "S-" + QString::number(newAbstractNode->getOrder());
		  newAbstractNodeLabel->setPlainText(label);
		}
	      else if (newAbstractNode->getConstraint() == NOCONSTRAINT ||
		       newAbstractNode->getConstraint() == NOCONSTRAINTATT) 
		{
		  QString label = "N-" + QString::number(newAbstractNode->getOrder());
		  newAbstractNodeLabel->setPlainText(label);
		}
	      qreal xOffset = (newAbstractNode->getWidth() / 2) - 20;
	      newAbstractNodeLabel->setOffset(QPointF(xOffset, 0));
	      newAbstractNodeLabel->setNewPos(newAbstractNode->scenePos());
	      newAbstractNodeLabel->setZValue(4);
	      newAbstractNodeLabel->setDefaultTextColor(Qt::black);
	      currentLayer.push_back(newAbstractNode);
	      partners.push_back(abstractNode);
	      Linkage *newLinkage = new Linkage("Hierarchy", "", 0);
	      _tempEdges.push_back(newLinkage);
	      newLinkage->setZValue(2);
	      newLinkage->setStartItem(newAbstractNode);
	      newLinkage->setEndItem(partLayer[it]);
	      newLinkage->setCopy(true);
	      newLinkage->setColor(QColor(Qt::gray));
	      scene->addItem(newLinkage);
	    }
	}
      QVectorIterator<IncidentNode*> it3(_incidentNodeVector);
      while (it3.hasNext()) 
	{
	  IncidentNode *event = it3.next();
	  if (event->getAbstractNode() == currentFather) 
	    {
	      QVectorIterator<QGraphicsItem*> it4(partners);
	      bool found = false;
	      while (it4.hasNext()) 
		{
		  AbstractNode *tempAbstractNode = qgraphicsitem_cast<AbstractNode*>(it4.next());
		  if (tempAbstractNode) 
		    {
		      QVector<IncidentNode*> tempIncidents = tempAbstractNode->getIncidents();
		      if (tempIncidents.contains(event)) 
			{
			  found = true;
			}
		    }
		}
	      if (!found) 
		{
		  IncidentNode *newEvent = new IncidentNode(40,
							    event->data(Qt::ToolTipRole).toString(),
							    QPointF(0, yPos), event->getId(),
							    event->getOrder());
		  newEvent->setCopy(true);
		  newEvent->setMode(event->getMode());
		  newEvent->setColor(event->getColor());
		  newEvent->setPos(newEvent->getOriginalPos());
		  newEvent->setZValue(3);
		  _tempIncidentNodeVector.push_back(newEvent);
		  bool found = false;
		  if (newEvent->getMode() != "") 
		    {
		      QSqlQuery *query = new QSqlQuery;
		      query->prepare("SELECT description FROM incident_attributes "
				     "WHERE name = :name");
		      query->bindValue(":name", newEvent->getMode());
		      query->exec();
		      query->first();
		      QString description = query->value(0).toString();
		      delete query;
		      for (int i = 0; i < eventListWidget->rowCount(); i++) 
			{
			  if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == newEvent->getMode()) 
			    {
			      found = true;
			    }
			}
		      if (!found) 
			{
			  QTableWidgetItem *item = new QTableWidgetItem(newEvent->getMode());
			  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
			  QString toolTip = breakString(newEvent->getMode() + " - " + description);
			  item->setToolTip(toolTip);
			  item->setData(Qt::DisplayRole, newEvent->getMode());
			  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
			  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
			  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
			  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
			    setBackground(newEvent->getColor());
			  QVariant textColorVar = QVariant(newEvent->
							   getLabel()->defaultTextColor().rgb());
			  eventListWidget->item(eventListWidget->
						rowCount() - 1, 1)->setData(Qt::UserRole,
									    textColorVar);
			  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
			    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
				     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
			}
		    }
		  QPointer<IncidentNodeLabel> text = new IncidentNodeLabel(newEvent);
		  _incidentNodeLabelVector.push_back(text);
		  newEvent->setLabel(text);
		  text->setPlainText(QString::number(newEvent->getOrder()));
		  QPointF currentPos = newEvent->scenePos();
		  currentPos.setX(currentPos.x() - (text->textWidth() / 2));
		  currentPos.setY(currentPos.y() -12);
		  text->setPos(currentPos);    
		  text->setZValue(4);
		  text->setDefaultTextColor(Qt::black);
		  text->setFontSize(_labelSize);
		  currentLayer.push_back(newEvent);
		  partners.push_back(event);
		  Linkage *newLinkage = new Linkage("Hierarchy", "", 0);
		  _tempEdges.push_back(newLinkage);
		  newLinkage->setZValue(2);
		  newLinkage->setStartItem(newEvent);
		  newLinkage->setEndItem(partLayer[it]);
		  newLinkage->setCopy(true);
		  newLinkage->setColor(QColor(Qt::gray));
		  scene->addItem(newLinkage);
		}
	    }
	}
    }
  std::sort(currentLayer.begin(), currentLayer.end(), componentsSort);
  std::sort(partners.begin(), partners.end(), componentsSort);
  QVector<AbstractNode*> newLayer;
  QVector<AbstractNode*> partnerLayer;
  int layerSize = currentLayer.size();
  qreal width = 120 * layerSize;
  qreal startX = 0 - width / 2 - 60;
  int count = 1;
  for (QVector<QGraphicsItem*>::size_type it5 = 0; it5 != currentLayer.size(); it5++) 
    {
      QGraphicsItem *current = currentLayer[it5];
      qreal xPos = startX + count * 120;
      current->setPos(xPos, current->scenePos().y());
      count++;
      IncidentNode *event = qgraphicsitem_cast<IncidentNode*>(current);
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      if (event) 
	{
	  event->setOriginalPos(event->scenePos());
	  event->getLabel()->setNewPos(event->scenePos());
	  scene->addItem(event);
	  scene->addItem(event->getLabel());
	}
      else if (abstractNode) 
	{
	  abstractNode->setOriginalPos(abstractNode->scenePos());
	  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
	  scene->addItem(abstractNode);
	  scene->addItem(abstractNode->getLabel());
	  AbstractNode *tempAbstractNode = qgraphicsitem_cast<AbstractNode*>(partners[it5]);
	  newLayer.push_back(tempAbstractNode);
	  partnerLayer.push_back(qgraphicsitem_cast<AbstractNode*>(current));
	}
    }
  if (newLayer.size() > 0) 
    {
      addLayer(newLayer, partnerLayer, layer + 1);
    }
}

void HierarchyGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) 
{
  QGraphicsItem *source = NULL;
  QListIterator<QGraphicsItem*> it(scene->items());
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
      qreal currentX = source->scenePos().x();
      qreal currentY = source->scenePos().y();
      qreal newX = pos.x();
      qreal newY = pos.y();
      qreal xDiff = newX - currentX;
      qreal yDiff = newY - currentY;
      AbstractNode *sourceAbstractNode = qgraphicsitem_cast<AbstractNode*>(source);
      sourceAbstractNode->setPos(source->scenePos().x() + xDiff, source->scenePos().y() + yDiff);
      sourceAbstractNode->getLabel()->setNewPos(sourceAbstractNode->scenePos());
      QListIterator<QGraphicsItem*> it2(scene->items());
      while (it2.hasNext()) 
	{
	  QGraphicsItem *current = it2.next();
	  Linkage *linkage = qgraphicsitem_cast<Linkage*>(current);
	  if (linkage && linkage->getType() == "Hierarchy") 
	    {
	      AbstractNode *endAbstractNode = qgraphicsitem_cast<AbstractNode*>(linkage->getEnd());
	      if (endAbstractNode) 
		{
		  if (endAbstractNode == qgraphicsitem_cast<AbstractNode*>(source)) 
		    {
		      QGraphicsItem* partner = linkage->getStart();
		      partner->setPos(partner->scenePos().x() + xDiff, partner->scenePos().y() + yDiff);
		      IncidentNode *partnerEvent = qgraphicsitem_cast<IncidentNode*>(partner);
		      AbstractNode *partnerAbstractNode = qgraphicsitem_cast<AbstractNode*>(partner);
		      if (partnerEvent) 
			{
			  partnerEvent->getLabel()->setNewPos(partnerEvent->scenePos());
			}
		      else if (partnerAbstractNode) 
			{
			  partnerAbstractNode->getLabel()->setNewPos(partnerAbstractNode->scenePos());
			}
		    }
		}
	    }
	}
    }
}

  
void HierarchyGraphWidget::getEdges() 
{
  QVector<QColor> typeColors;
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *linkage = it.next();
      QString originalType = linkage->getType();
      QColor originalColor = linkage->getColor();
      if (!_presentTypes.contains(originalType)) 
	{
	  _presentTypes.push_back(originalType);
	  typeColors.push_back(originalColor);
	}
      QGraphicsItem *source = NULL;
      QGraphicsItem *target = NULL;
      IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode*>(linkage->getStart());
      IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode*>(linkage->getEnd());
      AbstractNode *startAbstractNode = qgraphicsitem_cast<AbstractNode*>(linkage->getStart());
      AbstractNode *endAbstractNode = qgraphicsitem_cast<AbstractNode*>(linkage->getEnd());
      QListIterator<QGraphicsItem*> it2(scene->items());
      while (it2.hasNext()) 
	{
	  QGraphicsItem *item = it2.next();
	  IncidentNode *event = qgraphicsitem_cast<IncidentNode*>(item);
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(item);
	  if (startIncidentNode && event) 
	    {
	      if (startIncidentNode->getId() == event->getId()) 
		{
		  source = event;
		}
	    }
	  if (endIncidentNode && event) 
	    {
	      if (endIncidentNode->getId() == event->getId()) 
		{
		  target = event;
		}
	    }
	  if (startAbstractNode && abstractNode) 
	    {
	      if (startAbstractNode->getId() == abstractNode->getId()) 
		{
		  source = abstractNode;
		}
	    }
	  if (endAbstractNode && abstractNode) 
	    {
	      if (endAbstractNode->getId() == abstractNode->getId()) 
		{
		  target = abstractNode;
		}
	    }
	}
      if (source != NULL && target != NULL) 
	{
	  bool valid = false;
	  IncidentNode *eventSource = qgraphicsitem_cast<IncidentNode*>(source);
	  IncidentNode *eventTarget = qgraphicsitem_cast<IncidentNode*>(target);
	  AbstractNode *abstractNodeSource = qgraphicsitem_cast<AbstractNode*>(source);
	  AbstractNode *abstractNodeTarget = qgraphicsitem_cast<AbstractNode*>(target);
	  if (eventSource && eventTarget) 
	    {
	      if (eventSource->getOriginalPos().y() == eventTarget->getOriginalPos().y()) 
		{
		  valid = true;
		}
	    }
	  else if (eventSource && abstractNodeTarget) 
	    {
	      if (eventSource->getOriginalPos().y() == abstractNodeTarget->getOriginalPos().y()) 
		{
		  valid = true;
		}
	    }
	  else if (abstractNodeSource && eventTarget) 
	    {
	      if (abstractNodeSource->getOriginalPos().y() == eventTarget->getOriginalPos().y()) 
		{
		  valid = true;
		}
	    }
	  else if (abstractNodeSource && abstractNodeTarget) 
	    {
	      if (abstractNodeSource->getOriginalPos().y() == abstractNodeTarget->getOriginalPos().y()) 
		{
		  valid = true;
		}
	    }
	  if (valid) 
	    {
	      Linkage *newLinkage = new Linkage(originalType, "", 0);
	      _tempEdges.push_back(newLinkage);
	      newLinkage->setZValue(2);
	      newLinkage->setStartItem(source);
	      newLinkage->setEndItem(target);
	      newLinkage->setColor(originalColor);
	      newLinkage->setCopy(true);
	      scene->addItem(newLinkage);
	      newLinkage->hide();
	      newLinkage->setMassHidden(true);
	    }
	}
    }
  QTableWidgetItem *item = new QTableWidgetItem("Hierarchy");
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  QString toolTip = breakString("Hierarchical - indicates inclusion of events "
				"in more abstract events.");
  item->setToolTip(toolTip);
  item->setData(Qt::DisplayRole, "Hierarchy");
  linkageListWidget->setRowCount(linkageListWidget->rowCount() + 1);
  linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 0, item);
  linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 1, new QTableWidgetItem);
  linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->setBackground(QColor(Qt::gray));
  linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->
    setFlags(linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->flags() ^
	     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
  for (QVector<QString>::size_type it2 = 0; it2 != _presentTypes.size(); it2++) 
    {
      QString currentType = _presentTypes[it2];
      QColor currentColor = typeColors[it2];
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT description, direction FROM linkage_types "
		     "WHERE name = :name");
      query->bindValue(":name", currentType);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QString direction = query->value(1).toString();
      delete query;
      QTableWidgetItem *item = new QTableWidgetItem(currentType);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
      QString toolTip = breakString(currentType + " (" + direction + ") - " + description);
      item->setToolTip(toolTip);
      item->setData(Qt::DisplayRole, currentType);
      linkageListWidget->setRowCount(linkageListWidget->rowCount() + 1);
      linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 0, item);
      linkageListWidget->setItem(linkageListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->setBackground(currentColor);
      linkageListWidget->item(linkageListWidget->rowCount() - 1, 0)->setBackground(QColor(Qt::gray));
      linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->
	setFlags(linkageListWidget->item(linkageListWidget->rowCount() - 1, 1)->flags() ^
		 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    }
  updateLinkages();
}

void HierarchyGraphWidget::setBackgroundColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      QColor color = colorDialog->selectedColor();
      scene->setBackgroundBrush(QBrush(color));
    }
  delete colorDialog;
}

void HierarchyGraphWidget::changeModeColor(QTableWidgetItem *item) 
{
   if (item->column() == 1) 
    {
      QColor currentFill = item->background().color();
      QVariant textColorVar = item->data(Qt::UserRole);
      QColor currentText = QColor::fromRgb(textColorVar.toUInt());
      QPointer<ModeColorDialog> colorDialog = new ModeColorDialog(this, currentFill, currentText);
      colorDialog->exec();
      if (colorDialog->getExitStatus() == 0)
	{
	  QColor fillColor = colorDialog->getFillColor();
	  QColor textColor = colorDialog->getTextColor();
	  item->setBackground(fillColor);
	  item->setData(Qt::UserRole, textColorVar);
	  QTableWidgetItem* neighbour = eventListWidget->item(item->row(), 0);
	  QString mode = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<IncidentNode*> it(_tempIncidentNodeVector);
	  while (it.hasNext()) 
	    {
	      IncidentNode *current = it.next();
	      if (current->getMode() == mode) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	  QVectorIterator<AbstractNode*> it2(_tempAbstractNodeVector);
	  while (it2.hasNext()) 
	    {
	      AbstractNode *current = it2.next();
	      if (current->getMode() == mode) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	}
    }
}

void HierarchyGraphWidget::addMode() 
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
	      QListIterator<QGraphicsItem*> it2(scene->items());
	      while (it2.hasNext()) 
		{
		  QGraphicsItem *current = it2.next();
		  IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(current);
		  if (currentIncidentNode && currentIncidentNode->getId() == currentIncident) 
		    {
		      currentIncidentNode->setColor(color);
		      currentIncidentNode->setMode(attribute);
		      currentIncidentNode->getLabel()->setDefaultTextColor(textColor);
		    }
		}
	    }
	  QListIterator<QGraphicsItem*> it2(scene->items());
	  while (it2.hasNext()) 
	    {
	      QGraphicsItem *current = it2.next();
	      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	      if (currentAbstractNode) 
		{
		  QSet<QString> attributes = currentAbstractNode->getAttributes();
		  if (attributes.contains(currentAttribute)) 
		    {
		      currentAbstractNode->setColor(color);
		      currentAbstractNode->setMode(attribute);
		      currentAbstractNode->getLabel()->setDefaultTextColor(textColor);
		    }
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
	  QVariant textColorVar = QVariant(textColor.rgb());
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
	    ->setData(Qt::UserRole, textColorVar);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	}
      delete query;
    }
  delete attributeColorDialog;
}


void HierarchyGraphWidget::removeMode() 
{
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *current = it.next();
      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(current);
      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      if (currentIncidentNode && currentIncidentNode->getMode() == text) 
	{
	  currentIncidentNode->setColor(Qt::white);
	  currentIncidentNode->getLabel()->setDefaultTextColor(Qt::black);
	  currentIncidentNode->setMode("");
	}
      else if (currentAbstractNode && currentAbstractNode->getMode() == text) 
	{
	  currentAbstractNode->setColor(Qt::white);
	  currentAbstractNode->getLabel()->setDefaultTextColor(Qt::black);
	  currentAbstractNode->setMode("");
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
      QVectorIterator<QString> it2(attributeVector);
      while (it2.hasNext()) 
	{
	  QString currentAttribute = it2.next();
	  query->prepare("SELECT incident FROM attributes_to_incidents "
			 "WHERE attribute = :currentAttribute");
	  query->bindValue(":currentAttribute", currentAttribute);
	  query->exec();
	  while (query->next()) 
	    {
	      int currentIncident = query->value(0).toInt();
	      QListIterator<QGraphicsItem*> it3(scene->items());
	      while (it3.hasNext()) 
		{
		  QGraphicsItem *current = it3.next();
		  IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(current);
		  if (currentIncidentNode && currentIncidentNode->getId() == currentIncident) 
		    {
		      currentIncidentNode->setColor(color);
		      currentIncidentNode->setMode(currentMode);
		    }
		}
	    }
	  QListIterator<QGraphicsItem*> it3(scene->items());
	  while (it3.hasNext()) 
	    {
	      QGraphicsItem *current = it3.next();
	      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	      if (currentAbstractNode) 
		{
		  QSet<QString> attributes = currentAbstractNode->getAttributes();
		  if (attributes.contains(currentAttribute)) 
		    {
		      currentAbstractNode->setColor(color);
		      currentAbstractNode->setMode(currentMode);
		    }
		}
	    }
	}
      delete query;
    }
}

void HierarchyGraphWidget::setModeButtons(QTableWidgetItem *item) 
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

void HierarchyGraphWidget::disableModeButtons() 
{
  removeModeButton->setEnabled(false);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton->setEnabled(false);
}

void HierarchyGraphWidget::restoreModeColors() 
{
  QVectorIterator<IncidentNode *> it(_tempIncidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *incidentNode = it.next();
      incidentNode->setColor(Qt::white);
      incidentNode->getLabel()->setDefaultTextColor(Qt::black);
      incidentNode->setMode("");
    }
  QVectorIterator<AbstractNode*> it2(_tempAbstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *abstractNode = it2.next();
      abstractNode->setColor(Qt::white);
      abstractNode->getLabel()->setDefaultTextColor(Qt::black);
      abstractNode->setMode("");
    }
  for (int i = 0; i != eventListWidget->rowCount(); i++) 
    {
      QString currentMode = eventListWidget->item(i,0)->data(Qt::DisplayRole).toString();
      QColor color = eventListWidget->item(i, 1)->background().color();
      QVariant textColorVar = eventListWidget->item(i, 1)->data(Qt::UserRole);
      QColor textColor = QColor::fromRgb(textColorVar.toUInt());
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
	      QListIterator<QGraphicsItem*> it4(scene->items());
	      while (it4.hasNext()) 
		{
		  QGraphicsItem *current = it4.next();
		  IncidentNode* currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(current);
		  if (currentIncidentNode && currentIncidentNode->getId() == currentIncident) 
		    {
		      currentIncidentNode->setColor(color);
		      currentIncidentNode->getLabel()->setDefaultTextColor(textColor);
		      currentIncidentNode->setMode(currentMode);
		    }
		}
	    }
	  QListIterator<QGraphicsItem*> it4(scene->items());
	  while (it4.hasNext()) 
	    {
	      QGraphicsItem *current = it4.next();
	      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	      if (currentAbstractNode) 
		{
		  QSet<QString> attributes = currentAbstractNode->getAttributes();
		  if (attributes.contains(currentAttribute)) 
		    {
		      currentAbstractNode->setColor(color);
		      currentAbstractNode->getLabel()->setDefaultTextColor(textColor);
		      currentAbstractNode->setMode(currentMode);
		    }
		}
	    }
	}
      delete query;
    }
  for (int i = 0; i < eventListWidget->rowCount(); i++) 
    {
      QString mode = eventListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
      QColor color = eventListWidget->item(i, 1)->background().color();
      QVariant textColorVar = eventListWidget->item(i, 1)->data(Qt::UserRole);
      QColor textColor = QColor::fromRgb(textColorVar.toUInt());
      QListIterator<QGraphicsItem*> it(scene->items());
      while (it.hasNext()) 
	{
	  QGraphicsItem *current = it.next();
	  IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(current);
	  AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	  if (currentIncidentNode && currentIncidentNode->getMode() == mode) 
	    {
	      currentIncidentNode->setColor(color);
	      currentIncidentNode->getLabel()->setDefaultTextColor(textColor);
	    }
	  else if (currentAbstractNode && currentAbstractNode->getMode() == mode) 
	    {
	      currentAbstractNode->setColor(color);
	      currentAbstractNode->getLabel()->setDefaultTextColor(textColor);
	    }
	}
    }
}

void HierarchyGraphWidget::moveModeUp() 
{
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != eventListWidget->item(0,0)->data(Qt::DisplayRole).toString()) 
    {
      int currentRow = eventListWidget->row(eventListWidget->currentItem());
      QTableWidgetItem *currentItem = eventListWidget->takeItem(currentRow,0);
      QColor currentColor = eventListWidget->item(currentRow, 1)->background().color();
      QVariant currentTextColorVar = eventListWidget->item(currentRow, 1)->data(Qt::UserRole);
      int newRow = currentRow - 1;
      QTableWidgetItem *otherItem = eventListWidget->takeItem(newRow, 0);
      QColor otherColor = eventListWidget->item(newRow, 1)->background().color();
      QVariant otherTextColorVar = eventListWidget->item(newRow, 1)->data(Qt::UserRole);
      eventListWidget->setItem(newRow, 0, currentItem);
      eventListWidget->item(newRow, 1)->setBackground(currentColor);
      eventListWidget->item(newRow, 1)->setData(Qt::UserRole, currentTextColorVar);
      eventListWidget->setItem(currentRow, 0, otherItem);
      eventListWidget->item(currentRow, 1)->setBackground(otherColor);
      eventListWidget->item(currentRow, 1)->setData(Qt::UserRole, otherTextColorVar);
      restoreModeColors();
      QModelIndex newIndex = eventListWidget->model()->index(newRow, 0);
      eventListWidget->setCurrentIndex(newIndex);
      setModeButtons(eventListWidget->currentItem());
    }
}

void HierarchyGraphWidget::moveModeDown() 
{
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != eventListWidget->item(eventListWidget->rowCount() - 1, 0)->
      data(Qt::DisplayRole).toString()) 
    {
      int currentRow = eventListWidget->row(eventListWidget->currentItem());
      QTableWidgetItem *currentItem = eventListWidget->takeItem(currentRow, 0);
      QColor currentColor = eventListWidget->item(currentRow, 1)->background().color();
      QVariant currentTextColorVar = eventListWidget->item(currentRow, 1)->data(Qt::UserRole);
      int newRow = currentRow + 1;
      QTableWidgetItem *otherItem = eventListWidget->takeItem(newRow, 0);
      QColor otherColor = eventListWidget->item(newRow, 1)->background().color();
      QVariant otherTextColorVar = eventListWidget->item(newRow, 1)->data(Qt::UserRole);
      eventListWidget->setItem(newRow, 0, currentItem);
      eventListWidget->item(newRow, 1)->setBackground(currentColor);
      eventListWidget->item(newRow, 1)->setData(Qt::UserRole, currentTextColorVar);
      eventListWidget->setItem(currentRow, 0, otherItem);
      eventListWidget->item(currentRow, 1)->setBackground(otherColor);
      eventListWidget->item(currentRow, 1)->setData(Qt::UserRole, otherTextColorVar);
      restoreModeColors();
      QModelIndex newIndex = eventListWidget->model()->index(newRow, 0);
      eventListWidget->setCurrentIndex(newIndex);
      setModeButtons(eventListWidget->currentItem());
    }
}

void HierarchyGraphWidget::increaseLabelSize()
{
  if (_labelSize < 100)
    {
      _labelSize++;
      QVectorIterator<IncidentNodeLabel*> it(_incidentNodeLabelVector);
      while (it.hasNext())
	{
	  IncidentNodeLabel *currentLabel = it.next();
	  currentLabel->setFontSize(_labelSize);
	}
      QVectorIterator<AbstractNodeLabel*> it2(_abstractNodeLabelVector);
      while (it2.hasNext())
	{
	  AbstractNodeLabel *currentLabel = it2.next();
	  currentLabel->setFontSize(_labelSize);
	}
    }
}

void HierarchyGraphWidget::decreaseLabelSize()
{
  if (_labelSize > 1)
    {
      _labelSize--;
      QVectorIterator<IncidentNodeLabel*> it(_incidentNodeLabelVector);
      while (it.hasNext())
	{
	  IncidentNodeLabel *currentLabel = it.next();
	  currentLabel->setFontSize(_labelSize);
	}
      QVectorIterator<AbstractNodeLabel*> it2(_abstractNodeLabelVector);
      while (it2.hasNext())
	{
	  AbstractNodeLabel *currentLabel = it2.next();
	  currentLabel->setFontSize(_labelSize);
	}
    }
}

void HierarchyGraphWidget::findChildren(QString father, QVector<QString> *children, bool entity) 
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

void HierarchyGraphWidget::setLinkageButtons(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
    {
      QListIterator<QGraphicsItem*> it(scene->items());
      while (it.hasNext()) 
	{
	  QGraphicsItem *currentItem = it.next();
	  Linkage *current = qgraphicsitem_cast<Linkage*>(currentItem);
	  if (current) 
	    {
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
	}
    }
  else 
    {
      hideLinkageTypeButton->setEnabled(false);
      showLinkageTypeButton->setEnabled(false);
    }
}

void HierarchyGraphWidget::changeLinkageColor(QTableWidgetItem *item) 
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
	  QListIterator<QGraphicsItem*> it(scene->items());
	  while (it.hasNext()) 
	    {
	      QGraphicsItem *currentItem = it.next();
	      Linkage *current = qgraphicsitem_cast<Linkage*>(currentItem);
	      if (current) 
		{
		  if (current->getType() == type) 
		    {
		      current->setColor(color);
		    }
		}
	    }
	}
    }
}

void HierarchyGraphWidget::disableLinkageButtons() 
{
  hideLinkageTypeButton->setEnabled(false);
  showLinkageTypeButton->setEnabled(false);
}

void HierarchyGraphWidget::hideLinkage() 
{
  showLinkageTypeButton->setEnabled(true);
  hideLinkageTypeButton->setEnabled(false);
  QString text = linkageListWidget->currentItem()->data(Qt::DisplayRole).toString();
  linkageListWidget->currentItem()->setBackground(Qt::gray);
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *currentItem = it.next();
      Linkage *current = qgraphicsitem_cast<Linkage*>(currentItem);
      if (current) 
	{
	  if (current->getType() == text) 
	    {
	      current->setMassHidden(true);
	      current->hide();
	    }
	}
    }
  setHeights();
}

void HierarchyGraphWidget::showLinkage() 
{
  showLinkageTypeButton->setEnabled(false);
  hideLinkageTypeButton->setEnabled(true);
  QString text = linkageListWidget->currentItem()->data(Qt::DisplayRole).toString();
  linkageListWidget->currentItem()->setBackground(Qt::transparent);
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *currentItem = it.next();
      Linkage *current = qgraphicsitem_cast<Linkage*>(currentItem);
      if (current) 
	{
	  if (current->getType() == text) 
	    {
	      current->setMassHidden(false);
	      current->show();
	    }
	}
    }
  setHeights();
}

void HierarchyGraphWidget::updateLinkages() 
{
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext()) 
    {
      QGraphicsItem *current = it.next();
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(current);
      if (linkage) 
	{
	  linkage->updatePosition();
	}
    }
}

void HierarchyGraphWidget::setHeights() 
{
  QVector<QString> finished;
  QVectorIterator<QString> it(_presentTypes);
  it.toBack();
  while (it.hasPrevious()) 
    {
      QString currentType = it.previous();
      QVectorIterator<Linkage*> it2(_edgeVector);
      while (it2.hasNext()) 
	{
	  Linkage *currentLinkage = it2.next();
	  if (currentLinkage->isVisible()) 
	    {
	      if (currentLinkage->getType() == currentType) 
		{
		  QGraphicsItem *start = currentLinkage->getStart();
		  QGraphicsItem *end = currentLinkage->getEnd();
		  int countFound = 0;
		  bool found = false;
		  QVectorIterator<Linkage*> it3(_edgeVector);
		  while (it3.hasNext()) 
		    {
		      Linkage *otherLinkage = it3.next();
		      if (otherLinkage->isVisible()) 
			{
			  if (otherLinkage != currentLinkage) 
			    {
			      if (otherLinkage->getStart() == start &&
				  otherLinkage->getEnd() == end) 
				{
				  found = true;
				  if (!finished.contains(otherLinkage->getType())) 
				    {
				      countFound++;
				    }
				}
			      else if (otherLinkage->getStart() == end && otherLinkage->getEnd() == start) 
				{
				  found = true;
				}
			    }
			}
		    }
		  if (countFound > 0) 
		    {
		      currentLinkage->setHeight((countFound + 1) * 40);
		    }
		  else if (found) 
		    {
		      currentLinkage->setHeight(1 * 40);
		    }
		  else 
		    {
		      currentLinkage->setHeight(0);
		    }
		}
	    }
	  finished.push_back(currentType);
	}
    }
}

void HierarchyGraphWidget::setPenStyle()
{
  _currentPenStyle = penStyleComboBox->currentIndex() + 1;
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      LineObject *line = qgraphicsitem_cast<LineObject*>(selectedItem);
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(selectedItem);
      RectObject *rect = qgraphicsitem_cast<RectObject*>(selectedItem);
      if (line)
	{
	  line->setPenStyle(_currentPenStyle);
	}
      else if (ellipse)
	{
	  ellipse->setPenStyle(_currentPenStyle);
	}
      else if (rect)
	{
	  rect->setPenStyle(_currentPenStyle);
	}
    }
}

void HierarchyGraphWidget::setPenWidth()
{
  _currentPenWidth = penWidthSpinBox->value();
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      LineObject *line = qgraphicsitem_cast<LineObject*>(selectedItem);
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(selectedItem);
      RectObject *rect = qgraphicsitem_cast<RectObject*>(selectedItem);
      if (line)
	{
	  line->setPenWidth(_currentPenWidth);
	}
      else if (ellipse)
	{
	  ellipse->setPenWidth(_currentPenWidth);
	}
      else if (rect)
	{
	  rect->setPenWidth(_currentPenWidth);
	}
    }
}

void HierarchyGraphWidget::addLineObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  newLineObject->setZValue(5);
}

void HierarchyGraphWidget::addSingleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  newLineObject->setZValue(5);
}

void HierarchyGraphWidget::addDoubleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setArrow2(true);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  newLineObject->setZValue(5);
}

void HierarchyGraphWidget::addEllipseObject(const QRectF &area)
{
  EllipseObject *newEllipse = new EllipseObject();
  _ellipseVector.push_back(newEllipse);
  scene->addItem(newEllipse);
  newEllipse->setPenStyle(_currentPenStyle);
  newEllipse->setPenWidth(_currentPenWidth);
  newEllipse->setColor(_currentLineColor);
  newEllipse->setFillColor(_currentFillColor);
  newEllipse->moveCenter(newEllipse->mapToScene(area.center()));
  newEllipse->setBottomRight(newEllipse->mapToScene(area.bottomRight()));
  newEllipse->setTopLeft(newEllipse->mapToScene(area.topLeft()));
  newEllipse->setZValue(5);
}

void HierarchyGraphWidget::addRectObject(const QRectF &area) 
{
  RectObject *newRect = new RectObject();
  _rectVector.push_back(newRect);
  scene->addItem(newRect);
  newRect->setPenStyle(_currentPenStyle);
  newRect->setPenWidth(_currentPenWidth);
  newRect->setColor(_currentLineColor);
  newRect->setFillColor(_currentFillColor);
  newRect->moveCenter(newRect->mapToScene(area.center()));
  newRect->setBottomRight(newRect->mapToScene(area.bottomRight()));
  newRect->setTopLeft(newRect->mapToScene(area.topLeft()));
  newRect->setZValue(5);
}

void HierarchyGraphWidget::addTextObject(const QRectF &area, const qreal &size)
{
  QPointer<LargeTextDialog> textDialog = new LargeTextDialog(this);
  textDialog->setWindowTitle("Set text");
  textDialog->setLabel("Free text:");
  textDialog->exec();
  if (textDialog->getExitStatus() == 0) 
    {
      QString text = textDialog->getText();
      TextObject *newText = new TextObject(text);
      QFont font = newText->font();
      font.setPointSize(size);
      newText->setFont(font);
      _textVector.push_back(newText);
      scene->addItem(newText);
      newText->setPos(newText->mapFromScene(area.topLeft()));
      newText->setDefaultTextColor(_currentLineColor);
      newText->setZValue(6);
      newText->adjustSize();
    }
  delete textDialog;
}

void HierarchyGraphWidget::setLineColor()
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setCurrentColor(_currentLineColor);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _currentLineColor = colorDialog->selectedColor();
      emit sendLineColor(_currentLineColor);
      QPixmap lineColorMap(20, 20);
      lineColorMap.fill(_currentLineColor);
      QIcon lineColorIcon(lineColorMap);
      changeLineColorButton->setIcon(lineColorIcon);
    }
  delete colorDialog;
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      LineObject *line = qgraphicsitem_cast<LineObject*>(selectedItem);
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(selectedItem);
      RectObject *rect = qgraphicsitem_cast<RectObject*>(selectedItem);
      TextObject *text = qgraphicsitem_cast<TextObject*>(selectedItem);
      if (line)
	{
	  line->setColor(_currentLineColor);
	}
      else if (ellipse)
	{
	  ellipse->setColor(_currentLineColor);
	}
      else if (rect)
	{
	  rect->setColor(_currentLineColor);
	}
      else if (text)
	{
	  text->setDefaultTextColor(_currentLineColor);
	}
    }
}

void HierarchyGraphWidget::setFillColor()
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setCurrentColor(_currentFillColor);
  _currentFillColor.setAlpha(fillOpacitySlider->value());
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _currentFillColor = colorDialog->selectedColor();
      emit sendFillColor(_currentFillColor);
      QPixmap fillColorMap(20, 20);
      QColor tempFill = _currentFillColor;
      tempFill.setAlpha(255);
      fillColorMap.fill(tempFill);
      QIcon fillColorIcon(fillColorMap);
      changeFillColorButton->setIcon(fillColorIcon);
    }
  delete colorDialog;
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(selectedItem);
      RectObject *rect = qgraphicsitem_cast<RectObject*>(selectedItem);
      if (ellipse)
	{
	  ellipse->setFillColor(_currentFillColor);
	}
      else if (rect)
	{
	  rect->setFillColor(_currentFillColor);
	}
    }
}

void HierarchyGraphWidget::setFillOpacity(int value)
{
  _currentFillColor.setAlpha(value);
  emit sendFillColor(_currentFillColor);
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(selectedItem);
      RectObject *rect = qgraphicsitem_cast<RectObject*>(selectedItem);
      if (ellipse)
	{
	  ellipse->setFillColor(_currentFillColor);
	}
      else if (rect)
	{
	  rect->setFillColor(_currentFillColor);
	}
    }
}

void HierarchyGraphWidget::addHorizontalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(true);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void HierarchyGraphWidget::addVerticalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(false);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void HierarchyGraphWidget::toggleSnapGuides()
{
  if (snapGuidesButton->isChecked())
    {
      scene->setSnapGuides(true);
    }
  else
    {
      scene->setSnapGuides(false);
    }
}

void HierarchyGraphWidget::processShapeSelection()
{
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      LineObject *line = qgraphicsitem_cast<LineObject*>(selectedItem);
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(selectedItem);
      RectObject *rect = qgraphicsitem_cast<RectObject*>(selectedItem);
      TextObject *text = qgraphicsitem_cast<TextObject*>(selectedItem);
      if (line)
	{
	  int penStyle = line->getPenStyle();
	  int penWidth = line->getPenWidth();
	  penStyleComboBox->setCurrentIndex(penStyle - 1);
	  penWidthSpinBox->setValue(penWidth);
	  _currentLineColor = line->getColor();
	  emit sendLineColor(_currentLineColor);
	  QPixmap lineColorMap(20, 20);
	  lineColorMap.fill(_currentLineColor);
	  QIcon lineColorIcon(lineColorMap);
	  changeLineColorButton->setIcon(lineColorIcon);
	}
      else if (ellipse)
	{
	  int penStyle = ellipse->getPenStyle();
	  int penWidth = ellipse->getPenWidth();
	  penStyleComboBox->setCurrentIndex(penStyle - 1);
	  penWidthSpinBox->setValue(penWidth);
	  _currentLineColor = ellipse->getColor();
	  _currentFillColor = ellipse->getFillColor();
	  emit sendLineColor(_currentLineColor);
	  emit sendFillColor(_currentFillColor);
	  QPixmap lineColorMap(20, 20);
	  lineColorMap.fill(_currentLineColor);
	  QIcon lineColorIcon(lineColorMap);
	  changeLineColorButton->setIcon(lineColorIcon);
	  QPixmap fillColorMap(20, 20);
	  QColor tempFill = _currentFillColor;
	  tempFill.setAlpha(255);
	  fillColorMap.fill(tempFill);
	  QIcon fillColorIcon(fillColorMap);
	  changeFillColorButton->setIcon(fillColorIcon);
	  fillOpacitySlider->blockSignals(true);
	  fillOpacitySlider->setValue(_currentFillColor.alpha());
	  fillOpacitySlider->blockSignals(false);
	}
      else if (rect)
	{
	  int penStyle = rect->getPenStyle();
	  int penWidth = rect->getPenWidth();
	  penStyleComboBox->setCurrentIndex(penStyle - 1);
	  penWidthSpinBox->setValue(penWidth);
	  _currentLineColor = rect->getColor();
	  _currentFillColor = rect->getFillColor();
	  emit sendLineColor(_currentLineColor);
	  emit sendFillColor(_currentFillColor);
	  QPixmap lineColorMap(20, 20);
	  lineColorMap.fill(_currentLineColor);
	  QIcon lineColorIcon(lineColorMap);
	  changeLineColorButton->setIcon(lineColorIcon);
	  QPixmap fillColorMap(20, 20);
	  QColor tempFill = _currentFillColor;
	  tempFill.setAlpha(255);
	  fillColorMap.fill(tempFill);
	  QIcon fillColorIcon(fillColorMap);
	  changeFillColorButton->setIcon(fillColorIcon);
	  fillOpacitySlider->blockSignals(true);
	  fillOpacitySlider->setValue(_currentFillColor.alpha());
	  fillOpacitySlider->blockSignals(false);
	}
      else if (text)
	{
	  _currentLineColor = text->defaultTextColor();
	  emit sendLineColor(_currentLineColor);
	  QPixmap lineColorMap(20, 20);
	  lineColorMap.fill(_currentLineColor);
	  QIcon lineColorIcon(lineColorMap);
	  changeLineColorButton->setIcon(lineColorIcon);
	}
    }
}

void HierarchyGraphWidget::processLineContextMenu(const QString &action) 
{
  if (action == TOGGLEARROW1) 
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

void HierarchyGraphWidget::toggleArrow1() 
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

void HierarchyGraphWidget::toggleArrow2() 
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

void HierarchyGraphWidget::deleteLine() 
{
  if (scene->selectedItems().size() == 1) 
    {
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      if (line) 
	{
	  delete line;
	  _lineVector.removeOne(line);
	}
    }
}

void HierarchyGraphWidget::duplicateLine() 
{
  if (scene->selectedItems().size() == 1) 
    {
      LineObject *line = qgraphicsitem_cast<LineObject*>(scene->selectedItems().first());
      if (line) 
	{
	  QPointF newStartPos = line->getStartPos();
	  QPointF newEndPos = line->getEndPos();
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
	  _lineVector.push_back(newLineObject);
	  scene->addItem(newLineObject);
	  newLineObject->setZValue(5);
	}
    }
}

void HierarchyGraphWidget::processTextContextMenu(const QString &action) 
{
  if (action == CHANGETEXT) 
    {
      changeText();
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

void HierarchyGraphWidget::changeText() 
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

void HierarchyGraphWidget::deleteText() 
{
  if (scene->selectedItems().size() == 1) 
    {
      TextObject *text = qgraphicsitem_cast<TextObject*>(scene->selectedItems().first());
      if (text) 
	{
	  delete text;
	  _textVector.removeOne(text);
	}
    }
}

void HierarchyGraphWidget::duplicateText() 
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
	      _textVector.push_back(newText);
	      scene->addItem(newText);
	      QPointF pos = text->scenePos();
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

void HierarchyGraphWidget::processEllipseContextMenu(const QString &action) 
{
  if (action == DELETEELLIPSE) 
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

void HierarchyGraphWidget::deleteEllipse() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  delete ellipse;
	  _ellipseVector.removeOne(ellipse);
	}
    }  
}

void HierarchyGraphWidget::duplicateEllipse() 
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
	  _ellipseVector.push_back(newEllipse);
	  newEllipse->setZValue(5);
	  scene->addItem(newEllipse);
	  QPointF pos = ellipse->mapToScene(ellipse->getCenter());
	  newEllipse->moveCenter(newEllipse->mapFromScene(pos));
	}
    }
}

void HierarchyGraphWidget::processRectContextMenu(const QString &action) 
{
  if (action == DELETERECT) 
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

void HierarchyGraphWidget::deleteRect() 
{
  if (scene->selectedItems().size() == 1) 
    {
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      if (rect) 
	{
	  delete rect;
	  _rectVector.removeOne(rect);
	}
    }  
}

void HierarchyGraphWidget::duplicateRect() 
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
	  _rectVector.push_back(newRect);
	  newRect->setZValue(5);
	  scene->addItem(newRect);
	  QPointF pos = rect->mapToScene(rect->getCenter());
	  newRect->moveCenter(newRect->mapFromScene(pos));
	}
    }
}

void HierarchyGraphWidget::processGuideLineContextMenu(const QString &action) 
{
  if (action == DELETEGUIDEACTION) 
    {
      deleteGuideLine();
    }
}

void HierarchyGraphWidget::deleteGuideLine()
{
 if (scene->selectedItems().size() == 1) 
    {
      GuideLine *guide = qgraphicsitem_cast<GuideLine*>(scene->selectedItems().first());
      if (guide) 
	{
	  delete guide;
	  _guidesVector.removeOne(guide);
	}
    }  
}


void HierarchyGraphWidget::objectOneForward() 
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

void HierarchyGraphWidget::objectOneBackward() 
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

void HierarchyGraphWidget::objectToFront() 
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

void HierarchyGraphWidget::objectToBack() 
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

void HierarchyGraphWidget::fixZValues() 
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
  if (maxZ > 3)
    {
      for (int i = 4; i != maxZ; i++) 
	{
	  bool currentZFound = false;
	  QListIterator<QGraphicsItem*> it2(scene->items());
	  while (it2.hasNext()) 
	    {
	      QGraphicsItem *current = it2.next();
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
		  QGraphicsItem *current = it3.next();
		  if (current->zValue() > i) 
		    {
		      current->setZValue(current->zValue() - 1);
		    }
		}
	    }
	}
    }
  QVectorIterator<GuideLine*> it4(_guidesVector);
  while (it4.hasNext())
    {
      GuideLine *guide = it4.next();
      guide->setZValue(maxZ + 1);
    }
}

void HierarchyGraphWidget::exportSvg() 
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("New svg file"),"", tr("svg files (*.svg)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if (!fileName.endsWith(".svg")) 
	{
	  fileName.append(".svg");
	}
      scene->clearSelection();
      QSvgGenerator gen;
      gen.setFileName(fileName);
      QRectF currentRect = this->scene->itemsBoundingRect();
      currentRect.setX(currentRect.x());
      currentRect.setY(currentRect.y());
      currentRect.setWidth(currentRect.width());
      currentRect.setHeight(currentRect.height());
      gen.setSize(QSize(currentRect.width(), currentRect.height()));
      gen.setViewBox(QRect(0, 0, currentRect.width(), currentRect.height()));
      int dpiX = qApp->desktop()->logicalDpiX();
      gen.setResolution(dpiX);
      QPainter painter;
      painter.begin(&gen);
      scene->render(&painter);
      painter.end();
    }
}

void HierarchyGraphWidget::assignAttribute() 
{
  int barPos = rawField->verticalScrollBar()->value();
  if (_selectedIncident != 0) 
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
	  query->bindValue(":incident", _selectedIncident);
	  query->exec();
	  query->first();
	  empty = query->isNull(0);
	  QTextCursor cursPos = rawField->textCursor();
	  if (empty) 
	    {
	      query->prepare("INSERT INTO attributes_to_incidents (attribute, incident) "
			     "VALUES (:attribute, :incident)");
	      query->bindValue(":attribute", attribute);
	      query->bindValue(":incident", _selectedIncident);
	      query->exec();
	      sourceAttributeText(attribute, _selectedIncident);
	      boldSelected(attributesTree, attribute, _selectedIncident, INCIDENT);
	      valueField->setEnabled(true);
	    }
	  else 
	    {
	      sourceAttributeText(attribute, _selectedIncident);
	      highlightText();
	      rawField->setTextCursor(cursPos);
	    }
	  delete query;
	  setButtons();
	}
    }
  else if (_selectedAbstractNode != NULL) 
    {
      if (attributesTreeView->currentIndex().isValid()) 
	{
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  QSet<QString> attributes = _selectedAbstractNode->getAttributes();
	  if (!attributes.contains(attribute)) 
	    {
	      _selectedAbstractNode->insertAttribute(attribute);
	      QVectorIterator<AbstractNode*> it(_abstractNodeVector);
	      while (it.hasNext()) 
		{
		  AbstractNode *abstractNode = it.next();
		  if (abstractNode->getId() == _selectedAbstractNode->getId()) 
		    {
		      abstractNode->insertAttribute(attribute);							 
		    }
		} 
	      boldSelected(attributesTree, attribute, _selectedAbstractNode->getId(), ABSTRACTNODE);
	      valueField->setEnabled(true);
	    }
	}
      setButtons();
    }
  rawField->verticalScrollBar()->setValue(barPos);
}

void HierarchyGraphWidget::unassignAttribute() 
{
  if (_selectedIncident != 0) 
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
	  query->bindValue(":incident", _selectedIncident);
	  query->exec();
	  query->first();
	  empty = query->isNull(0);
	  if (!empty) 
	    {
	      query->prepare("DELETE FROM attributes_to_incidents "
			     "WHERE attribute = :att AND incident = :incident");
	      query->bindValue(":att", attribute);
	      query->bindValue(":incident", _selectedIncident);
	      query->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents_sources "
			      "WHERE attribute = :att AND incident = :incident");
	      query2->bindValue(":att", attribute);
	      query2->bindValue(":inc", _selectedIncident);
	      query2->exec();
	      resetFont(attributesTree);
	      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			      "WHERE incident = :incident");
	      query2->bindValue(":incident", _selectedIncident);
	      query2->exec();
	      while (query2->next()) 
		{
		  QString attribute = query2->value(0).toString();
		  boldSelected(attributesTree, attribute, _selectedIncident, INCIDENT);
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
  else if (_selectedAbstractNode != NULL) 
    {
      if (attributesTreeView->currentIndex().isValid()) 
	{
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  QSet<QString> attributes = _selectedAbstractNode->getAttributes();
	  if (attributes.contains(attribute)) 
	    {
	      _selectedAbstractNode->removeAttribute(attribute);
	      QVectorIterator<AbstractNode*> it(_abstractNodeVector);
	      while (it.hasNext()) 
		{
		  AbstractNode *abstractNode = it.next();
		  if (abstractNode->getId() == _selectedAbstractNode->getId()) 
		    {
		      abstractNode->removeAttribute(attribute);							 
		    }
		} 
	      QSet<QString>::iterator it2;
	      resetFont(attributesTree);
	      attributes = _selectedAbstractNode->getAttributes();
	      for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
		{
		  QString current = *it2;
		  boldSelected(attributesTree, current, _selectedAbstractNode->getId(), ABSTRACTNODE);	  
		}
	      setButtons();
	      valueField->setText("");
	      valueField->setEnabled(false);
	      valueButton->setEnabled(false);
	    }
	}
      setButtons();
    }
}

void HierarchyGraphWidget::newAttribute() 
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
	  entityDialog->setRelationshipsWidget(_relationshipsWidgetPtr);
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
	  _attributesWidgetPtr->resetTree();
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
	      _attributesWidgetPtr->resetTree();
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
	  _attributesWidgetPtr->resetTree();
	}
      delete attributeDialog;
    }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void HierarchyGraphWidget::editAttribute() 
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
	      entityDialog->setRelationshipsWidget(_relationshipsWidgetPtr);
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
	      attributeDialog->submitDescription(description);
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
		  query->prepare("UPDATE saved_eg_plots_attributes_to_abstract_nodes "
				 "SET attribute = :newname "
				 "WHERE attribute = :oldname");
		  query->bindValue(":newname", newName);
		  query->bindValue(":oldname", name);
		  query->exec();
		  this->setCursor(Qt::WaitCursor);
		  retrieveData();
		  this->setCursor(Qt::ArrowCursor);
		  _attributesWidgetPtr->resetTree();
		}
	      delete query;
	      delete attributeDialog;
	    }
	}
      attributesTree->sort(0, Qt::AscendingOrder);
      attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
    }
}

void HierarchyGraphWidget::updateEntityAfterEdit(const QString name,
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
  _relationshipsWidgetPtr->resetTree();
}

void HierarchyGraphWidget::removeUnusedAttributes() 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  QSet<QString> takenAttributes;
  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
  while (it.hasNext()) 
    {
      AbstractNode* current = it.next();
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
		  "EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_abstract_nodes "
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
		  "EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_abstract_nodes "
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
  _attributesWidgetPtr->resetTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
  delete query;  
  delete query2;
}

void HierarchyGraphWidget::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void HierarchyGraphWidget::highlightText() 
{
  int barPos = rawField->verticalScrollBar()->value();
  if (_selectedIncident != 0) 
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
	      query->bindValue(":id", _selectedIncident);
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

void HierarchyGraphWidget::removeText() 
{
  if (_selectedIncident != 0) 
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
	      query->bindValue(":inc", _selectedIncident);
	      query->bindValue(":text", sourceText);
	      query->exec();
	      delete query;
	    }
	  setButtons();
	  highlightText();
	}
    }
}

void HierarchyGraphWidget::selectText() 
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

void HierarchyGraphWidget::sourceAttributeText(const QString &attribute, const int &incident) 
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
	  query->prepare("INSERT INTO attributes_to_incidents_sources "
			 "(attribute, incident, source_text) "
			 "VALUES (:att, :inc, :text)");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", incident);
	  query->bindValue(":text", sourceText);
	  query->exec();
	}
      delete query;
    }
}

void HierarchyGraphWidget::resetTexts() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      if (_selectedIncident != 0) 
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
		  query->bindValue(":inc", _selectedIncident);
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

void HierarchyGraphWidget::setLabelSize(int size)
{
  _labelSize = size;
}

void HierarchyGraphWidget::setOrigin(AbstractNode *origin) 
{
  _origin = origin;
  buildComponents(_origin, 1);
}

void HierarchyGraphWidget::setEvents(QVector<IncidentNode*> eventVector) 
{
  _incidentNodeVector = eventVector;
}

void HierarchyGraphWidget::setAbstractNodes(QVector<AbstractNode*> abstractNodeVector) 
{
  _abstractNodeVector = abstractNodeVector;
}

void HierarchyGraphWidget::setEdges(QVector<Linkage*> edgeVector) 
{
  _edgeVector = edgeVector;
}

void HierarchyGraphWidget::setTree() 
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

void HierarchyGraphWidget::resetTree() 
{
  scene->clearSelection();
  retrieveData();
  delete attributesTree;
  setTree();
}

void HierarchyGraphWidget::buildHierarchy(QStandardItem *top, QString name) 
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

void HierarchyGraphWidget::buildEntities(QStandardItem *top, QString name) 
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

void HierarchyGraphWidget::fixTree() 
{
  resetFont(attributesTree);
  if (_selectedIncident != 0) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute FROM attributes_to_incidents "
		     "WHERE incident = :id");
      query->bindValue(":id", _selectedIncident);
      query->exec();
      while (query->next()) 
	{
	  QString attribute = query->value(0).toString();
	  boldSelected(attributesTree, attribute);
	}
      delete query;
    }
  else if (_selectedAbstractNode != NULL) 
    {
      QSet<QString> attributes = _selectedAbstractNode->getAttributes();
      QSet<QString>::iterator it;
      for (it = attributes.begin(); it != attributes.end(); it++) 
	{
	  boldSelected(attributesTree, *it);
	}
    }
}

void HierarchyGraphWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) 
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

void HierarchyGraphWidget::boldSelected(QAbstractItemModel *model, QString name,
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
			  else if (type == ABSTRACTNODE) 
			    {
			      QSet<QString> attributes = _selectedAbstractNode->getAttributes();
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

void HierarchyGraphWidget::setButtons() 
{
  valueButton->setEnabled(false);
  valueField->setEnabled(false);
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->blockSignals(false);
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString currentAttribute = attributesTreeView->currentIndex().data().toString();
      if (_selectedIncident != 0) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  bool empty = false;
	  query->prepare("SELECT attribute, incident, value FROM "
			 "attributes_to_incidents "
			 "WHERE attribute = :att AND incident = :inc  ");
	  query->bindValue(":att", currentAttribute);
	  query->bindValue(":inc", _selectedIncident);
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
	  query->bindValue(":inc", _selectedIncident);
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
      else if (_selectedAbstractNode != NULL) 
	{
	  QString currentAttribute = attributesTreeView->currentIndex().data().toString();
	  QSet<QString> attributes = _selectedAbstractNode->getAttributes();
	  QMap<QString, QString> values = _selectedAbstractNode->getValues();
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
      removeTextButton->setEnabled(false);
      resetTextsButton->setEnabled(false);
    }
}

void HierarchyGraphWidget::cleanUp() 
{
  setComment();
  _incidentNodeVector.clear();
  _abstractNodeVector.clear();
  qDeleteAll(_abstractNodeLabelVector);
  _abstractNodeLabelVector.clear();
  qDeleteAll(_incidentNodeLabelVector);
  _incidentNodeLabelVector.clear();
  _edgeVector.clear();
  qDeleteAll(_tempEdges);
  _tempEdges.clear();
  _origin = NULL;
  scene->clear();
  _currentData.clear();
  _selectedAbstractNode = NULL;
  _selectedIncident = 0;
  qDeleteAll(_lineVector);
  _lineVector.clear();
  qDeleteAll(_textVector);
  _textVector.clear();
  qDeleteAll(_ellipseVector);
  _ellipseVector.clear();
  qDeleteAll(_rectVector);
  _rectVector.clear();
  qDeleteAll(_guidesVector);
  _guidesVector.clear();
  snapGuidesButton->setChecked(false);
  toggleSnapGuides();
  retrieveData();
  eventListWidget->setRowCount(0);
  linkageListWidget->setRowCount(0);
  _presentTypes.clear();
}

void HierarchyGraphWidget::switchBack() 
{
  setComment();
  emit goToEventGraph();
}

void HierarchyGraphWidget::setAttributesWidget(AttributesWidget *attributesWidgetPtr) 
{
  _attributesWidgetPtr = attributesWidgetPtr;
}

void HierarchyGraphWidget::setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr) 
{
  _relationshipsWidgetPtr = relationshipsWidgetPtr;
}

void HierarchyGraphWidget::finalBusiness() 
{
  cleanUp();
}

bool HierarchyGraphWidget::eventFilter(QObject *object, QEvent *event) 
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
