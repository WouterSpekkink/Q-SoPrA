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

#include "../include/SystemGraphWidget.h"

SystemGraphWidget::SystemGraphWidget(QWidget *parent) : QWidget(parent)
{
  _selectedType = "";
  _selectedEntityName = "";
  _labelsShown = true;
  _massMove = false;
  _maxWeight = 0;
  _vectorPos = 0;
  _currentPenStyle = 1;
  _currentPenWidth = 1;
  _currentLineColor = QColor(Qt::black);
  _currentFillColor = QColor(Qt::black);
  _currentFillColor.setAlpha(0);
  
  scene = new Scene(this);
  view = new GraphicsView(scene);
  scene->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
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

  infoWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
  legendWidget = new QWidget(this);

  weightLabel = new QLabel(tr("<b>Minimum occurrence:</b>"), graphicsWidget);
  nodeLegendLabel = new QLabel(tr("<b>Event Types:</b>"), legendWidget);
  edgeLegendLabel = new QLabel(tr("<b>Linkage Types:</b>"), legendWidget);
  nameLabel = new QLabel(tr("<b>Event Type:</b<"), infoWidget);
  descriptionLabel = new QLabel(tr("<b>Description:</b>"), infoWidget);
  zoomLabel = new QLabel(tr("<b>Zoom slider:</b>"), this);
  shapesLabel = new QLabel(tr("<b>Shapes:</b>"), this);
  penStyleLabel = new QLabel(tr("<b>Pen style:</b>"), this);
  penWidthLabel = new QLabel(tr("<b>Pen width:</b>"), this);
  lineColorLabel = new QLabel(tr("<b>Line / Text color:</b>"), this);
  guideLinesLabel = new QLabel(tr("<b>Add guides:</b>"), this);
  fillColorLabel = new QLabel(tr("<b>Fill color:</b>"), this);
  fillOpacityLabel = new QLabel(tr("<b>Opacity:</b>"), this);
  labelSizeLabel = new QLabel(tr("<b>Label size:</b>"), this);
  layoutLabel = new QLabel(tr("<b>Layout:</b>"), this);

  layoutComboBox = new QComboBox(this);
  layoutComboBox->addItem(SPRINGLAYOUT);
  layoutComboBox->addItem(CIRCULARLAYOUT);

  nameField = new QLineEdit(infoWidget);
  nameField->setReadOnly(true);

  descriptionField = new QTextEdit(infoWidget);
  descriptionField->setReadOnly(true);

  weightCheckBox = new QCheckBox(tr("Visualise"));
  weightCheckBox->setCheckState(Qt::Unchecked);

  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  toggleDetailsButton = new QPushButton(tr("Toggle details"), this);
  toggleDetailsButton->setCheckable(true);
  expandLayoutButton = new QPushButton(QIcon(":/images/expand_network.png"), "", this);
  expandLayoutButton->setIconSize(QSize(20, 20));
  expandLayoutButton->setMinimumSize(40, 40);
  expandLayoutButton->setMaximumSize(40, 40);
  contractLayoutButton = new QPushButton(QIcon(":/images/contract_network.png"), "", this);
  contractLayoutButton->setIconSize(QSize(20, 20));
  contractLayoutButton->setMinimumSize(40, 40);
  contractLayoutButton->setMaximumSize(40, 40);
  layoutButton = new QPushButton(tr("Run layout"), this);
  previousNodeButton = new QPushButton(tr("<<"), infoWidget);
  previousNodeButton->setEnabled(false);
  nextNodeButton = new QPushButton(tr(">>"), infoWidget);
  nextNodeButton->setEnabled(false);
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  toggleLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  increaseFontSizeButton = new QPushButton(tr("+"));
  decreaseFontSizeButton = new QPushButton(tr("-"));
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  exportNodesButton = new QPushButton(tr("Export nodes"), graphicsWidget);
  exportEdgesButton = new QPushButton(tr("Export edges"), graphicsWidget);
  hideModeButton = new QPushButton(tr("Hide"), legendWidget);
  hideModeButton->setCheckable(true);
  hideModeButton->setChecked(false);
  showModeButton = new QPushButton(tr("Show"), legendWidget);
  showModeButton->setCheckable(true);
  showModeButton->setChecked(true);
  disableModeButtons();
  hideTypeButton = new QPushButton(tr("Hide"), legendWidget);
  hideTypeButton->setCheckable(true);
  hideTypeButton->setChecked(false);
  showTypeButton = new QPushButton(tr("Show"), legendWidget);
  showTypeButton->setCheckable(true);
  showTypeButton->setChecked(true);
  disableTypeButtons();
  exitButton = new QPushButton(tr("Return to event graph"), this);
  exitButton->setStyleSheet("QPushButton {color: blue; font-weight: bold}");

  addLineButton = new QPushButton(QIcon(":/images/line_object.png"), "", this);
  addLineButton->setIconSize(QSize(20, 20));
  addLineButton->setMinimumSize(40, 40);
  addLineButton->setMaximumSize(40, 40);
  addSingleArrowButton = new QPushButton(QIcon(":/images/single_arrow_object.png"), "", this);
  addSingleArrowButton->setIconSize(QSize(20, 20));
  addSingleArrowButton->setMinimumSize(40, 40);
  addSingleArrowButton->setMaximumSize(40, 40);
  addDoubleArrowButton = new QPushButton(QIcon(":/images/double_arrow_object.png"), "", this);
  addDoubleArrowButton->setIconSize(QSize(20, 20));
  addDoubleArrowButton->setMinimumSize(40, 40);
  addDoubleArrowButton->setMaximumSize(40, 40);
  addEllipseButton = new QPushButton(QIcon(":/images/ellipse_object.png"), "", this);
  addEllipseButton->setIconSize(QSize(20, 20));
  addEllipseButton->setMinimumSize(40, 40);
  addEllipseButton->setMaximumSize(40, 40);
  addRectangleButton = new QPushButton(QIcon(":/images/rect_object.png"), "", this);
  addRectangleButton->setIconSize(QSize(20, 20));
  addRectangleButton->setMinimumSize(40, 40);
  addRectangleButton->setMaximumSize(40, 40);
  addTextButton = new QPushButton(QIcon(":/images/text_object.png"), "", this);
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
  hideAnnotationsButton = new QPushButton(tr("Hide annotations"), this);
  hideAnnotationsButton->setCheckable(true);
  addHorizontalGuideLineButton = new QPushButton(QIcon(":/images/guide_horizontal.png"), "", this);
  addHorizontalGuideLineButton->setIconSize(QSize(20, 20));
  addHorizontalGuideLineButton->setMinimumSize(40, 40);
  addHorizontalGuideLineButton->setMaximumSize(40, 40);
  addVerticalGuideLineButton = new QPushButton(QIcon(":/images/guide_vertical.png"), "", this);
  addVerticalGuideLineButton->setIconSize(QSize(20, 20));
  addVerticalGuideLineButton->setMinimumSize(40, 40);
  addVerticalGuideLineButton->setMaximumSize(40, 40);
  snapGuidesButton = new QPushButton(tr("Toggle snap guides"), this);
  snapGuidesButton->setCheckable(true);

  penStyleComboBox = new QComboBox(this);
  penStyleComboBox->addItem("Solid");
  penStyleComboBox->setItemIcon(0, QIcon(":/images/solid_line.png"));
  penStyleComboBox->addItem("Dashed");
  penStyleComboBox->setItemIcon(1, QIcon(":/images/dashed_line.png"));
  penStyleComboBox->addItem("Dotted");
  penStyleComboBox->setItemIcon(2, QIcon(":/images/dotted_line.png"));
  penStyleComboBox->addItem("Dash Dot");
  penStyleComboBox->setItemIcon(3, QIcon(":/images/dash_dot_line.png"));
  penStyleComboBox->addItem("Dash Dot Dot");
  penStyleComboBox->setItemIcon(4, QIcon(":/images/dash_dot_dot_line.png"));
  penWidthSpinBox = new QSpinBox(this);
  penWidthSpinBox->setMinimum(1);
  penWidthSpinBox->setMaximum(20);

  weightSpinBox = new QSpinBox(graphicsWidget);

  zoomSlider = new QSlider(Qt::Horizontal, this);
  zoomSlider->installEventFilter(this);
  zoomSlider->setMinimum(-9);
  zoomSlider->setMaximum(9);
  zoomSlider->setValue(0);
  
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

  view->viewport()->installEventFilter(this);

  connect(toggleDetailsButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
  connect(previousNodeButton, SIGNAL(clicked()), this, SLOT(previousDataItem()));
  connect(nextNodeButton, SIGNAL(clicked()), this, SLOT(nextDataItem()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(layoutButton, SIGNAL(clicked()), this, SLOT(makeLayout()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(toggleLabelsButton, SIGNAL(clicked()), this, SLOT(toggleLabels()));
  connect(increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseLabelSize()));
  connect(decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseLabelSize()));
  connect(weightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setVisibility()));
  connect(weightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setVisibility()));
  connect(nodeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
          this, SLOT(setModeButtons(QTableWidgetItem *)));
  connect(nodeListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
          this, SLOT(changeModeColor(QTableWidgetItem *)));
  connect(nodeListWidget, SIGNAL(noneSelected()),
          this, SLOT(disableModeButtons()));
  connect(edgeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
          this, SLOT(setTypeButtons(QTableWidgetItem *)));
  connect(edgeListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
          this, SLOT(changeEdgeColor(QTableWidgetItem *)));
  connect(edgeListWidget, SIGNAL(noneSelected()),
          this, SLOT(disableTypeButtons()));
  connect(hideModeButton, SIGNAL(clicked()), this, SLOT(hideMode()));
  connect(showModeButton, SIGNAL(clicked()), this, SLOT(showMode()));
  connect(hideTypeButton, SIGNAL(clicked()), this, SLOT(hideType()));
  connect(showTypeButton, SIGNAL(clicked()), this, SLOT(showType()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(exportNodesButton, SIGNAL(clicked()), this, SLOT(exportNodes()));
  connect(exportEdgesButton, SIGNAL(clicked()), this, SLOT(exportEdges()));
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
  connect(this, SIGNAL(sendLineColor(QColor &)), scene, SLOT(setLineColor(QColor &)));
  connect(this, SIGNAL(sendFillColor(QColor &)), scene, SLOT(setFillColor(QColor &)));
  connect(hideAnnotationsButton, SIGNAL(clicked()), this, SLOT(hideAnnotations()));
  connect(addHorizontalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepHorizontalGuideLine()));
  connect(addVerticalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepVerticalGuideLine()));
  connect(snapGuidesButton, SIGNAL(clicked()), this, SLOT(toggleSnapGuides()));
  connect(scene, SIGNAL(resetItemSelection()), this, SLOT(retrieveData()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(updateEdges()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
          this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(scene, SIGNAL(LineContextMenuAction(const QString &)),
          this, SLOT(processLineContextMenu(const QString &)));
  connect(scene, SIGNAL(TextContextMenuAction(const QString &)),
          this, SLOT(processTextContextMenu(const QString &)));
  connect(scene, SIGNAL(EllipseContextMenuAction(const QString &)),
          this, SLOT(processEllipseContextMenu(const QString &)));
  connect(scene, SIGNAL(RectContextMenuAction(const QString &)),
          this, SLOT(processRectContextMenu(const QString &)));
  connect(scene, SIGNAL(sendLinePoints(const QPointF&, const QPointF&)),
          this, SLOT(addLineObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(sendSingleArrowPoints(const QPointF&, const QPointF&)),
          this, SLOT(addSingleArrowObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(sendDoubleArrowPoints(const QPointF&, const QPointF&)),
          this, SLOT(addDoubleArrowObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(sendEllipseArea(const QRectF&)), this, SLOT(addEllipseObject(const QRectF&)));
  connect(scene, SIGNAL(sendRectArea(const QRectF&)), this, SLOT(addRectObject(const QRectF&)));
  connect(scene, SIGNAL(sendTextArea(const QRectF&, const qreal&)),
          this, SLOT(addTextObject(const QRectF&, const qreal&)));
  connect(scene, SIGNAL(GuideLineContextMenuAction(const QString &)),
          this, SLOT(processGuideLineContextMenu(const QString &)));
  connect(scene, SIGNAL(sendHorizontalGuideLinePos(const QPointF&)),
          this, SLOT(addHorizontalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(sendVerticalGuideLinePos(const QPointF&)),
          this, SLOT(addVerticalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(processShapeSelection()));
  connect(expandLayoutButton, SIGNAL(clicked()), this, SLOT(expandLayout()));
  connect(contractLayoutButton, SIGNAL(clicked()), this, SLOT(contractLayout()));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(processZoomSliderChange(int)));
  connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(resetZoomSlider()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
  connect(exitButton, SIGNAL(clicked()), this, SLOT(switchBack()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> drawHelpLayout = new QHBoxLayout;
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
  plotObjectsLayout->addWidget(hideAnnotationsButton);
  plotObjectsLayout->setAlignment(Qt::AlignLeft);
  drawHelpLayout->addLayout(plotObjectsLayout);
  mainLayout->addLayout(drawHelpLayout);
  
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
  QPointer<QFrame> sepLine = new QFrame();
  sepLine->setFrameShape(QFrame::HLine);
  legendLayout->addWidget(sepLine);
  legendLayout->addWidget(hideModeButton);
  legendLayout->addWidget(showModeButton);
  legendLayout->addWidget(edgeLegendLabel);
  legendLayout->addWidget(edgeListWidget);
  QPointer<QFrame> sepLine2 = new QFrame();
  sepLine2->setFrameShape(QFrame::HLine);
  legendLayout->addWidget(sepLine2);
  legendLayout->addWidget(hideTypeButton);
  legendLayout->addWidget(showTypeButton);
  legendWidget->setMinimumWidth(250);
  legendWidget->setMaximumWidth(250);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				     
  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(backgroundColorButton);
  graphicsControlsLayout->addWidget(toggleLabelsButton);
  graphicsControlsLayout->addWidget(labelSizeLabel);
  QPointer<QHBoxLayout> fontSizeLayout = new QHBoxLayout;
  fontSizeLayout->addWidget(increaseFontSizeButton);
  fontSizeLayout->addWidget(decreaseFontSizeButton);
  graphicsControlsLayout->addLayout(fontSizeLayout);
  QPointer<QFrame> sepLine3 = new QFrame();
  sepLine3->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine3);
  graphicsControlsLayout->addWidget(weightLabel);
  QPointer<QBoxLayout> weightLayout = new QHBoxLayout;
  weightLayout->addWidget(weightSpinBox);
  weightLayout->addWidget(weightCheckBox);
  graphicsControlsLayout->addLayout(weightLayout);
  QPointer<QFrame> sepLine4 = new QFrame();
  sepLine4->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine4);
  graphicsControlsLayout->addWidget(exportSvgButton);
  graphicsControlsLayout->addWidget(exportNodesButton);
  graphicsControlsLayout->addWidget(exportEdgesButton);
  graphicsWidget->setMaximumWidth(200);
  graphicsWidget->setMinimumWidth(200);
  graphicsWidget->setLayout(graphicsControlsLayout);
  graphicsControlsLayout->setAlignment(Qt::AlignBottom);
  screenLayout->addWidget(graphicsWidget);

  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  drawOptionsLeftLayout->addWidget(toggleDetailsButton);
  toggleDetailsButton->setMaximumWidth(toggleDetailsButton->sizeHint().width());
  QPointer<QFrame> vertLineOne = new QFrame();
  vertLineOne->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineOne);
  drawOptionsLeftLayout->addWidget(layoutLabel);
  layoutLabel->setMaximumWidth(layoutLabel->sizeHint().width());
  drawOptionsLeftLayout->addWidget(layoutComboBox);
  layoutComboBox->setMaximumWidth(layoutComboBox->sizeHint().width());
  drawOptionsLeftLayout->addWidget(layoutButton);
  layoutButton->setMaximumWidth(layoutButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(expandLayoutButton);
  expandLayoutButton->setMaximumWidth(expandLayoutButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(contractLayoutButton);
  contractLayoutButton->setMaximumWidth(contractLayoutButton->sizeHint().width());
  QPointer<QFrame> vertLineTwo = new QFrame();
  vertLineTwo->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineTwo);
  drawOptionsLeftLayout->addWidget(zoomLabel);
  zoomLabel->setMaximumWidth(zoomLabel->sizeHint().width());
  drawOptionsLeftLayout->addWidget(zoomSlider);
  zoomSlider->setMaximumWidth(100);
  QPointer<QFrame> vertLineThree = new QFrame();
  vertLineThree->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineThree);
  QPointer<QHBoxLayout> guidesLayout = new QHBoxLayout;
  guidesLayout->addWidget(guideLinesLabel);
  guidesLayout->addWidget(addHorizontalGuideLineButton);
  guidesLayout->addWidget(addVerticalGuideLineButton);
  guidesLayout->addWidget(snapGuidesButton);
  guidesLayout->setAlignment(Qt::AlignLeft);
  drawOptionsLeftLayout->addLayout(guidesLayout);
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);
  drawOptionsLayout->setAlignment(drawOptionsLeftLayout, Qt::AlignLeft);
  QPointer<QFrame> vertLineFour = new QFrame();
  vertLineFour->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineFour);
  
  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(exitButton);
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);

  setLayout(mainLayout);

  infoWidget->hide();
  graphicsWidget->hide();
  legendWidget->hide();
  setGraphControls(true);
}

SystemGraphWidget::~SystemGraphWidget()
{
  qDeleteAll(_nodeVector);
  _nodeVector.clear();
  qDeleteAll(_edgeVector);
  _edgeVector.clear();
  qDeleteAll(_labelVector);
  _labelVector.clear();
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

void SystemGraphWidget::setModeButtons(QTableWidgetItem *item)
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "")
  {
    hideModeButton->setEnabled(true);
    showModeButton->setEnabled(true);
  }
  else
  {
    hideModeButton->setEnabled(false);
    showModeButton->setEnabled(false);
  }
  QBrush brush = item->foreground();
  QVectorIterator<NetworkNode*> it(_nodeVector);
  bool hidden = false;
  while (it.hasNext())
  {
    NetworkNode *current = it.next();
    QString type = current->getName();
    if (text == type)
    {
      hidden = current->isMassHidden();
      break;
    }
  }
  if (hidden)
  {
    hideModeButton->setChecked(true);
    showModeButton->setChecked(false);
  }
  else
  {
    hideModeButton->setChecked(false);
    showModeButton->setChecked(true);
  }

}

void SystemGraphWidget::disableModeButtons()
{
  hideModeButton->setEnabled(false);
  showModeButton->setEnabled(false);
}

void SystemGraphWidget::setTypeButtons(QTableWidgetItem *item)
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "")
  {
    hideTypeButton->setEnabled(true);
    showTypeButton->setEnabled(true);
  }
  else
  {
    hideTypeButton->setEnabled(false);
    showTypeButton->setEnabled(false);
  }
  QBrush brush = item->foreground();
  QVectorIterator<DirectedEdge*> it(_edgeVector);
  bool hidden = false;
  while (it.hasNext())
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      hidden = current->isMassHidden();
      break;
    }
  }
  if (hidden)
  {
    hideTypeButton->setChecked(true);
    showTypeButton->setChecked(false);
  }
  else
  {
    hideTypeButton->setChecked(false);
    showTypeButton->setChecked(true);
  }
}

void SystemGraphWidget::disableTypeButtons()
{
  hideTypeButton->setEnabled(false);
  showTypeButton->setEnabled(false);
}

void SystemGraphWidget::setOpenGL(bool state)
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
    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
  }
  else
  {
    view->setViewport(new QWidget());
    view->viewport()->installEventFilter(this);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setDragMode(QGraphicsView::RubberBandDrag);
  }
}

void SystemGraphWidget::setAntialiasing(bool state)
{
  QVectorIterator<DirectedEdge*> it(_edgeVector);
  while (it.hasNext())
  {
    DirectedEdge *current = it.next();
    current->setAntialiasing(state);
  }
}

void SystemGraphWidget::setEvents(QMap<QVector<QString>, QColor> eventsMap)
{
  QMapIterator<QVector<QString>, QColor> eIt(eventsMap);
  while (eIt.hasNext())
  {
    QVector<QString> event = eIt.peekNext().key();
    QColor color = eIt.next().value();
    QTableWidgetItem *item = new QTableWidgetItem(event[0]);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setToolTip(event[1]);
    item->setData(Qt::DisplayRole, event[0]);
    nodeListWidget->setRowCount(nodeListWidget->rowCount() + 1);
    nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 0, item);
    nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 1,
                            new QTableWidgetItem);
    nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setBackground(color);
    nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->
      setFlags(nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->flags() ^
               Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
  }
}

void SystemGraphWidget::setEdges(QMap<QVector<QString>, QColor> edgesMap)
{
  QMapIterator<QVector<QString>, QColor> eIt(edgesMap);
  while (eIt.hasNext())
  {
    QVector<QString> edge = eIt.peekNext().key();
    QColor color = eIt.next().value();
    QTableWidgetItem *item = new QTableWidgetItem(edge[0]);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setToolTip(edge[1]);
    item->setData(Qt::DisplayRole, edge[0]);
    edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1,
                            new QTableWidgetItem);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
      setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags() ^
               Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
  }
}

void SystemGraphWidget::setSystem(QMap<QVector<QString>, int> system)
{
  // Let's unpack our data to create our network
  QMapIterator<QVector<QString>, int> sIt(system);
  while (sIt.hasNext())
  {
    QVector<QString> currentEdge = sIt.peekNext().key();
    int currentWeight = sIt.next().value();
    // Then we use the data to create our nodes
    // But we first need to check if they already exist
    QVectorIterator<NetworkNode *> nIt(_nodeVector);
    NetworkNode *tail = NULL;
    NetworkNode *head = NULL;
    while (nIt.hasNext())
    {
      NetworkNode *currentNode = nIt.next();
      if (currentNode->getName() == currentEdge[1])
      {
        tail = currentNode;
      }
      if (currentNode->getName() == currentEdge[2])
      {
        head = currentNode;
      }
    }
    if (tail == NULL)
    {
      NetworkNode *newTail = new NetworkNode(currentEdge[1], currentEdge[3]);
      newTail->setZValue(3);
      _nodeVector.push_back(newTail);
      scene->addItem(newTail);
      newTail->hide();
      NetworkNodeLabel *tailLabel =  new NetworkNodeLabel(newTail);
      tailLabel->setPlainText(newTail->getName());
      tailLabel->setZValue(4);
      tailLabel->setDefaultTextColor(Qt::black);
      _labelVector.push_back(tailLabel);
      newTail->setLabel(tailLabel);
      scene->addItem(tailLabel);
      tailLabel->hide();
      tail = newTail;
      // If this is a self-loop and we haven't encountered
      // this node yet, we need to do the below
      // to prevent the node from being created twice
      if (currentEdge[1] == currentEdge[2])
      {
        head = tail;
      }
    }
    if (head == NULL)
    {
      NetworkNode *newHead = new NetworkNode(currentEdge[2], currentEdge[4]);
      newHead->setZValue(3);
      _nodeVector.push_back(newHead);
      scene->addItem(newHead);
      newHead->hide();
      NetworkNodeLabel *headLabel =  new NetworkNodeLabel(newHead);
      headLabel->setPlainText(newHead->getName());
      headLabel->setZValue(4);
      headLabel->setDefaultTextColor(Qt::black);
      _labelVector.push_back(headLabel);
      newHead->setLabel(headLabel);
      scene->addItem(headLabel);
      headLabel->hide();
      head = newHead;
    }
    // And then our edge
    DirectedEdge *newEdge = new DirectedEdge(tail, head, currentEdge[0], currentEdge[0]);
    _edgeVector.push_back(newEdge);
    newEdge->hide();
    newEdge->setToolTip("Weight: " + QString::number(currentWeight));
    scene->addItem(newEdge);
    _edgeWeights.insert(newEdge, currentWeight);
  }
  // Let's set the colours of our nodes
  for (int i = 0; i < nodeListWidget->rowCount(); i++)
  {
    QString mode = nodeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    QColor color = nodeListWidget->item(i, 1)->background().color();
    QVectorIterator<NetworkNode *> it(_nodeVector);
    while (it.hasNext())
    {
      NetworkNode *node = it.next();
      if (node->getName() == mode)
      {
        node->setColor(color);
      }
    }
  }
  // And then let's set the colour of our edges
  for (int i = 0; i < edgeListWidget->rowCount(); i++)
  {
    QString type = edgeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    QColor color = edgeListWidget->item(i, 1)->background().color();
    QVectorIterator<DirectedEdge *> it(_edgeVector);
    while (it.hasNext())
    {
      DirectedEdge *edge = it.next();
      if (edge->getType() == type)
      {
        edge->setColor(color);
      }
    }
  }
  processHeights();
  setWeightControls();
  makeLayout();
}

void SystemGraphWidget::switchBack()
{
  hideAnnotationsButton->setChecked(false);
  emit goToEventGraph();
}

void SystemGraphWidget::toggleDetails()
{
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

void SystemGraphWidget::toggleGraphicsControls()
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

void SystemGraphWidget::toggleLegend()
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

void SystemGraphWidget::rescale()
{
  view->scale(2.0, 2.0);
  view->scale(0.5, 0.5);
}

void SystemGraphWidget::processZoomSliderChange(int value)
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

void SystemGraphWidget::resetZoomSlider()
{
  zoomSlider->blockSignals(true);
  zoomSlider->setValue(0);
  zoomSlider->blockSignals(false);
}


void SystemGraphWidget::setGraphControls(bool state)
{
  zoomSlider->setEnabled(state);
  expandLayoutButton->setEnabled(state);
  contractLayoutButton->setEnabled(state);
  weightSpinBox->setEnabled(state);
  addLineButton->setEnabled(state);
  addSingleArrowButton->setEnabled(state);
  addDoubleArrowButton->setEnabled(state);
  addEllipseButton->setEnabled(state);
  addRectangleButton->setEnabled(state);
  addTextButton->setEnabled(state);
  penStyleComboBox->setEnabled(state);
  penWidthSpinBox->setEnabled(state);
  changeLineColorButton->setEnabled(state);
  changeFillColorButton->setEnabled(state);
  fillOpacitySlider->setEnabled(state);
  addHorizontalGuideLineButton->setEnabled(state);
  addVerticalGuideLineButton->setEnabled(state);
  snapGuidesButton->setEnabled(state);
  layoutButton->setEnabled(state);
  hideAnnotationsButton->setEnabled(state);
  toggleLabelsButton->setEnabled(state);
  backgroundColorButton->setEnabled(state);
  increaseFontSizeButton->setEnabled(state);
  decreaseFontSizeButton->setEnabled(state);
  exportSvgButton->setEnabled(state);
  exportNodesButton->setEnabled(state);
  exportEdgesButton->setEnabled(state);
  weightCheckBox->setEnabled(state);
}

void SystemGraphWidget::retrieveData()
{
  if (_currentData.size() > 0)
  {
    _currentData.clear();
  }
  if (scene->selectedItems().size() > 0)
  {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext())
    {
      NetworkNode *node = qgraphicsitem_cast<NetworkNode*>(it.peekNext());
      DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(it.peekNext());
      if (node && !directed)
      {
        NetworkNode *currentNode = qgraphicsitem_cast<NetworkNode*>(it.next());
        currentNode->setSelectionColor(Qt::black);
        currentNode->update();
        _currentData.push_back(currentNode);
      }
      else
      {
        it.next();
      }
    }
    if (_currentData.size() > 0)
    {
      std::sort(_currentData.begin(), _currentData.end(), nodeLessThan);
      _vectorPos = 0;
      NetworkNode *currentNode = _currentData.at(_vectorPos);
      currentNode->setSelectionColor(Qt::red);
      currentNode->update();
      QString name = currentNode->getName();
      _selectedEntityName = name;
      QString description = currentNode->getDescription();
      nameField->setText(name);
      descriptionField->setText(description);
      previousNodeButton->setEnabled(true);
      nextNodeButton->setEnabled(true);
    }
  }
  else
  {
    nameField->clear();
    _selectedEntityName = "";
    descriptionField->clear();
    previousNodeButton->setEnabled(false);
    nextNodeButton->setEnabled(false);
  }
}

void SystemGraphWidget::previousDataItem()
{
  if (_vectorPos > 0)
  {
    NetworkNode *currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    _vectorPos--;
    currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    _selectedEntityName = name;
    descriptionField->setText(description);
  }
  else
  {
    NetworkNode *currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    _vectorPos = _currentData.size() - 1;
    currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    scene->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    _selectedEntityName = name;
    descriptionField->setText(description);
  }
}

void SystemGraphWidget::nextDataItem()
{
  if (_vectorPos != _currentData.size() - 1)
  {
    NetworkNode *currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    _vectorPos++;
    currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    _selectedEntityName = name;
    descriptionField->setText(description);
  }
  else
  {
    NetworkNode *currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::black);
    currentNode->update();
    _vectorPos = 0;
    currentNode = _currentData.at(_vectorPos);
    currentNode->setSelectionColor(Qt::red);
    currentNode->update();
    QString name = currentNode->getName();
    QString description = currentNode->getDescription();
    nameField->setText(name);
    _selectedEntityName = name;
    descriptionField->setText(description);
  }
}

void SystemGraphWidget::makeLayout()
{
  if (layoutComboBox->currentText() == SPRINGLAYOUT)
  {
    springLayout();
  }
  else if (layoutComboBox->currentText() == CIRCULARLAYOUT)
  {
    circularLayout();
  }
  setVisibility();
}

void SystemGraphWidget::springLayout()
{
  qApp->setOverrideCursor(Qt::WaitCursor);
  QVector<QGraphicsItem*> edges;
  QVector<NetworkNode*> nodes;
  QMap<NetworkNode*, QPointF> newPos;
  QListIterator<QGraphicsItem*> it(scene->items());
  while (it.hasNext())
  {
    QGraphicsItem *current = it.next();
    DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(current);
    NetworkNode *node = qgraphicsitem_cast<NetworkNode*>(current);
    if (directed)
    {
      edges.push_back(directed);
    }
    else if (node)
    {
      qreal x = qrand() % 5000;
      qreal y = qrand() % 5000;
      node->setPos(x, y);
      if (node->isVisible())
      {
        newPos.insert(node, node->scenePos());
        nodes.push_back(node);
      }
    }
  }
  for(int i = 0; i != 100; i++) 
  {
    QVectorIterator<QGraphicsItem*> it2(edges);
    while (it2.hasNext())
    {
      QGraphicsItem *current = it2.next();
      DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(current);
      if (directed)
      {
        if (directed->isVisible())
        {
          NetworkNode *tail = directed->getStart();
          NetworkNode *head = directed->getEnd();
          QPointF tailPos = newPos.value(tail);
          QPointF headPos = newPos.value(head);
          qreal dist = qSqrt(qPow(tailPos.x() - headPos.x(), 2) +
                             qPow(tailPos.y() - headPos.y(), 2));
          if (dist > 200)
          {
            QPointF diff = 0.9 * (tailPos - headPos);
            tailPos = headPos + diff;
            diff = 0.9 * (headPos - tailPos);
            headPos = tailPos + diff;
            newPos.insert(tail, tailPos);
            newPos.insert(head, headPos);
          }
        }
      }
    }
    QVectorIterator<NetworkNode*> it3(nodes);
    while (it3.hasNext())
    {
      NetworkNode *first = it3.next();
      QVectorIterator<NetworkNode*> it4(nodes);
      while (it4.hasNext())
      {
        NetworkNode *second = it4.next();
        QPointF firstPos = newPos.value(first);
        QPointF secondPos = newPos.value(second);
        qreal dist = qSqrt(qPow(firstPos.x() - secondPos.x(), 2) +
                           qPow(firstPos.y() - secondPos.y(), 2));
        if (dist < 200)
        {
          QPointF diff = 1.5 * (firstPos - secondPos);
          firstPos = secondPos + diff;
          diff = 0.9 * (secondPos - firstPos);
          secondPos = firstPos + diff;
          newPos.insert(first, firstPos);
          newPos.insert(second, secondPos);
        }
      }
    }
  }
  QVectorIterator<NetworkNode*> it5(_nodeVector);
  while (it5.hasNext())
  {
    NetworkNode *node = it5.next();
    QPointF newPosition = newPos.value(node);
    node->setPos(newPosition);
    node->getLabel()->setNewPos(node->scenePos());
  }
  bool overlapping = true;
  while (overlapping)
  {
    overlapping = false;
    it5.toFront();
    while (it5.hasNext())
    {
      NetworkNode *first = it5.next();
      QVectorIterator<NetworkNode*> it6(_nodeVector);
      while (it6.hasNext())
      {
        NetworkNode* second = it6.next();
        QPointF firstPos = first->scenePos();
        QPointF secondPos = second->scenePos();
        qreal dist = qSqrt(qPow(firstPos.x() - secondPos.x(), 2) +
                           qPow(firstPos.y() - secondPos.y(), 2));
        if (first != second)
        {
          if (dist < 50)
          {
            overlapping = true;
            QPointF displacement = QPointF((qrand() % 100) - 50, (qrand() % 100) - 50);
            first->setPos(firstPos + displacement);
            first->getLabel()->setNewPos(first->scenePos());
            displacement = QPointF((qrand() % 100) - 50, (qrand() % 100) - 50);
            second->setPos(secondPos + displacement);
            second->getLabel()->setNewPos(second->scenePos());
          }
        }
      }
    }
  }
  view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
  qApp->restoreOverrideCursor();
  qApp->processEvents();
}

void SystemGraphWidget::circularLayout()
{
  const qreal Pi = 3.14;
  QVector<NetworkNode*> nodes;
  QVectorIterator<NetworkNode*> it(_nodeVector);
  while(it.hasNext()) 
  {
    NetworkNode *current = it.next();
    if (current->isVisible())
    {
      nodes.push_back(current);
    }
  }
  std::sort(nodes.begin(), nodes.end(), modeSort);
  QVectorIterator<NetworkNode*> it2(nodes);  
  int count = 1;
  while (it2.hasNext()) 
  {
    NetworkNode *current = it2.next();
    if (current->isVisible())
    {
      qreal x = 100 * cos(count * 2 * Pi / nodes.size());
      qreal y = 100 * sin(count * 2 * Pi / nodes.size());
      current->setPos(QPointF(x, y));
      current->getLabel()->setNewPos(current->scenePos());
      count++;
    }
  }
  NetworkNode *first = nodes[0];
  NetworkNode *second = nodes[1];
  qreal dist = qSqrt(qPow(first->scenePos().x() -
                          second->scenePos().x(), 2) +
                     qPow(first->scenePos().y() -
                          second->scenePos().y(), 2));
  while (dist < 50) 
  {
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
void SystemGraphWidget::expandLayout()
{
  QPointF virtualCenter = QPointF(0,0);
  qreal total = 0.0;
  QVectorIterator<NetworkNode*> it(_nodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    virtualCenter += current->scenePos();
    total = total + 1.0;
  }
  virtualCenter /= total;
  QVectorIterator<NetworkNode*> it2(_nodeVector);
  while (it2.hasNext()) 
  {
    NetworkNode *current = it2.next();
    QPointF currentPoint = current->scenePos();
    qreal diffX = (currentPoint.x() - virtualCenter.x()) * 1.1;
    qreal diffY = (currentPoint.y() - virtualCenter.y()) * 1.1;
    current->setPos(virtualCenter.x() + diffX, virtualCenter.y() + diffY);
    current->getLabel()->setNewPos(current->scenePos());
  }
  updateEdges();
}

/*
  The idea behind this function below was inspired on the code for contracting
  layouts in Gephi.
*/
void SystemGraphWidget::contractLayout()
{
  QPointF virtualCenter = QPointF(0,0);
  int total = 0;
  QVectorIterator<NetworkNode*> it(_nodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    virtualCenter += current->scenePos();
    total++;
  }
  virtualCenter /= total;
  QVectorIterator<NetworkNode*> it2(_nodeVector);
  while (it2.hasNext()) 
  {
    NetworkNode *current = it2.next();
    QPointF currentPoint = current->scenePos();
    qreal diffX = (currentPoint.x() - virtualCenter.x()) * 0.9;
    qreal diffY = (currentPoint.y() - virtualCenter.y()) * 0.9;
    current->setPos(virtualCenter.x() + diffX, virtualCenter.y() + diffY);
    current->getLabel()->setNewPos(current->scenePos());
  }
  updateEdges();
}

void SystemGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos)
{
  _massMove = true;
  if (_currentData.size() > 0) 
  {
    QGraphicsItem *source = NULL;
    QVectorIterator<NetworkNode*> it(_currentData);
    while (it.hasNext())
    {
      NetworkNode *temp = it.next();
      if (temp == item)
      {
        source = temp;
      }
    }
    if (source != NULL)
    {
      qreal currentY = source->scenePos().y();
      qreal currentX = source->scenePos().x();
      qreal newY = pos.y();
      qreal newX = pos.x();
      qreal yDiff = newY - currentY;
      qreal xDiff = newX - currentX;
      QVectorIterator<NetworkNode*> it2(_currentData);
      while (it2.hasNext())
      {
        NetworkNode *current = it2.next();
        current->setPos(current->scenePos().x() + xDiff, current->scenePos().y() + yDiff);
        current->getLabel()->setNewPos(current->scenePos());
      }
    }
    QVectorIterator<NetworkNode*> it3(_nodeVector);
    while (it3.hasNext())
    {
      NetworkNode *first = it3.next();
      if (first->isSelected())
      {
        QVectorIterator<NetworkNode*> it4(_nodeVector);
        while (it4.hasNext())
        {
          NetworkNode *second = it4.next();
          if (!second->isSelected())
          {
            qreal dist = qSqrt(qPow(first->scenePos().x() -
                                    second->scenePos().x(), 2) +
                               qPow(first->scenePos().y() -
                                    second->scenePos().y(), 2));
            if (dist <= 40)
            {
              QPointF firstPoint = first->scenePos();
              QPointF secondPoint = second->scenePos();
              qreal mX = (firstPoint.x() + secondPoint.x()) / 2;
              qreal mY = (firstPoint.y() + secondPoint.y()) / 2;
              QPointF midPoint = QPointF(mX, mY);
              qreal secondXDiff = secondPoint.x() - midPoint.x();
              qreal secondYDiff = secondPoint.y() - midPoint.y();
              qreal xDiff = 0;
              qreal yDiff = 0;
              if (secondXDiff > 0)
              {
                xDiff = 5;
              }
              else if (secondXDiff < 0)
              {
                xDiff = -5;
              }
              if (secondYDiff > 0)
              {
                yDiff = 5;
              }
              else if (secondYDiff < 0)
              {
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

void SystemGraphWidget::setPenStyle()
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

void SystemGraphWidget::setPenWidth()
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

void SystemGraphWidget::setLineColor()
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

void SystemGraphWidget::setFillColor()
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setCurrentColor(_currentFillColor);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
  {
    _currentFillColor = colorDialog->selectedColor();
    _currentFillColor.setAlpha(fillOpacitySlider->value());
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

void SystemGraphWidget::setFillOpacity(int value)
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

void SystemGraphWidget::addLineObject(const QPointF &start, const QPointF &end)
{
  LineObject *newLineObject = new LineObject(start, end);
  _lineVector.push_back(newLineObject);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_nodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void SystemGraphWidget::addSingleArrowObject(const QPointF &start, const QPointF &end)
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_nodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void SystemGraphWidget::addDoubleArrowObject(const QPointF &start, const QPointF &end)
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setArrow2(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_nodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void SystemGraphWidget::addEllipseObject(const QRectF &area)
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
  newEllipse->setZValue(_nodeVector[0]->zValue() + 2);
  newEllipse->setSelected(true);
}

void SystemGraphWidget::addRectObject(const QRectF &area)
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
  newRect->setZValue(_nodeVector[0]->zValue() + 2);
  newRect->setSelected(true);
}

void SystemGraphWidget::addTextObject(const QRectF &area, const qreal &size)
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
    newText->setZValue(_nodeVector[0]->zValue() + 3);
    newText->adjustSize();
    newText->setSelected(true);
  }
  delete textDialog;
}

void SystemGraphWidget::addHorizontalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(true);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void SystemGraphWidget::addVerticalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(false);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void SystemGraphWidget::toggleSnapGuides()
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

void SystemGraphWidget::processShapeSelection()
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

void SystemGraphWidget::processLineContextMenu(const QString &action)
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

void SystemGraphWidget::toggleArrow1()
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

void SystemGraphWidget::toggleArrow2()
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

void SystemGraphWidget::deleteLine()
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

void SystemGraphWidget::duplicateLine()
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
      newLineObject->setZValue(line->zValue());
    }
  }
}

void SystemGraphWidget::processTextContextMenu(const QString &action)
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

void SystemGraphWidget::changeText()
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

void SystemGraphWidget::deleteText()
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

void SystemGraphWidget::duplicateText()
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
        newText->setZValue(text->zValue());
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

void SystemGraphWidget::processEllipseContextMenu(const QString &action)
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

void SystemGraphWidget::deleteEllipse()
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

void SystemGraphWidget::duplicateEllipse()
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
      newEllipse->setZValue(ellipse->zValue());
      scene->addItem(newEllipse);
      QPointF pos = ellipse->mapToScene(ellipse->getCenter());
      newEllipse->moveCenter(newEllipse->mapFromScene(pos));
    }
  }
}

void SystemGraphWidget::processRectContextMenu(const QString &action)
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

void SystemGraphWidget::deleteRect()
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

void SystemGraphWidget::duplicateRect()
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
      newRect->setZValue(rect->zValue());
      scene->addItem(newRect);
      QPointF pos = rect->mapToScene(rect->getCenter());
      newRect->moveCenter(newRect->mapFromScene(pos));
    }
  }
}

void SystemGraphWidget::processGuideLineContextMenu(const QString &action)
{
  if (action == DELETEGUIDEACTION) 
  {
    deleteGuideLine();
  }
}

void SystemGraphWidget::deleteGuideLine()
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

void SystemGraphWidget::objectOneForward()
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

void SystemGraphWidget::objectOneBackward()
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

void SystemGraphWidget::objectToFront()
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

void SystemGraphWidget::objectToBack()
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

void SystemGraphWidget::fixZValues()
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
  QVectorIterator<GuideLine*> it4(_guidesVector);
  while (it4.hasNext())
  {
    GuideLine *guide = it4.next();
    guide->setZValue(maxZ + 1);
  }
}

void SystemGraphWidget::hideMode()
{
  hideModeButton->setChecked(true);
  showModeButton->setChecked(false);
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  nodeListWidget->currentItem()->setBackground(Qt::gray);
  QVectorIterator<NetworkNode*> it(_nodeVector);
  while (it.hasNext())
  {
    NetworkNode *current = it.next();
    QString type = current->getName();
    if (text == type)
    {
      current->setMassHidden(true);
    }
  }
  updateWeightControls();
  setVisibility();
}

void SystemGraphWidget::showMode()
{
  hideModeButton->setChecked(false);
  showModeButton->setChecked(true);
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  nodeListWidget->currentItem()->setBackground(Qt::transparent);
  QVectorIterator<NetworkNode*> it(_nodeVector);
  while (it.hasNext())
  {
    NetworkNode *current = it.next();
    QString type = current->getName();
    if (text == type)
    {
      current->setMassHidden(false);
    }
  }
  updateWeightControls();
  setVisibility();
}

void SystemGraphWidget::hideType()
{
  hideTypeButton->setChecked(true);
  showTypeButton->setChecked(false);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  edgeListWidget->currentItem()->setBackground(Qt::gray);
  QVectorIterator<DirectedEdge*> it(_edgeVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setMassHidden(true);
    }
  }
  updateWeightControls();
  setVisibility();
}

void SystemGraphWidget::showType()
{
  hideTypeButton->setChecked(false);
  showTypeButton->setChecked(true);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  edgeListWidget->currentItem()->setBackground(Qt::transparent);
  QVectorIterator<DirectedEdge*> it(_edgeVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setMassHidden(false);
    }
  }
  updateWeightControls();
  setVisibility();
}

void SystemGraphWidget::setBackgroundColor()
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
  {
    QColor color = colorDialog->selectedColor();
    scene->setBackgroundBrush(color);
  }
  delete colorDialog;
}

void SystemGraphWidget::changeEdgeColor(QTableWidgetItem *item)
{
  if (item->column() == 1) 
  {
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    colorDialog->setCurrentColor(item->background().color());
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    if (colorDialog->exec())
    {
      QColor color = colorDialog->selectedColor();
      item->setBackground(color);
      QTableWidgetItem* neighbour = edgeListWidget->item(item->row(), 0);
      QString type = neighbour->data(Qt::DisplayRole).toString();
      QVectorIterator<DirectedEdge*> it(_edgeVector);
      while (it.hasNext())
      {
        DirectedEdge *directed = it.next();
        if (directed->getType() == type)
        {
          directed->setColor(color);
        }
      }
    }
  }
}

void SystemGraphWidget::changeModeColor(QTableWidgetItem *item)
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
      textColorVar = QVariant(textColor.rgb());
      item->setData(Qt::UserRole, textColorVar);
      QTableWidgetItem* neighbour = nodeListWidget->item(item->row(), 0);
      QString mode = neighbour->data(Qt::DisplayRole).toString();
      QVectorIterator<NetworkNode*> it(_nodeVector);
      while (it.hasNext())
      {
        NetworkNode *current = it.next();
        if (current->getName() == mode)
        {
          current->setColor(fillColor);
          current->getLabel()->setDefaultTextColor(textColor);
        }
      }
    }
  }
}

void SystemGraphWidget::hideAnnotations()
{
  QVectorIterator<EllipseObject*> it(_ellipseVector);
  QVectorIterator<RectObject*> it2(_rectVector);
  QVectorIterator<LineObject*> it3(_lineVector);
  QVectorIterator<TextObject*> it4(_textVector);
  if (hideAnnotationsButton->isChecked())
  {
    while (it.hasNext())
    {
      it.next()->hide();
    }
    while (it2.hasNext())
    {
      it2.next()->hide();
    }
    while (it3.hasNext())
    {
      it3.next()->hide();
    }
    while (it4.hasNext())
    {
      it4.next()->hide();
    }
  }
  else
  {
    while (it.hasNext())
    {
      it.next()->show();
    }
    while (it2.hasNext())
    {
      it2.next()->show();
    }
    while (it3.hasNext())
    {
      it3.next()->show();
    }
    while (it4.hasNext())
    {
      it4.next()->show();
    }
  }
}

void SystemGraphWidget::toggleLabels()
{
  _labelsShown = !_labelsShown;
  QVectorIterator<NetworkNodeLabel*> it2(_labelVector);
  while (it2.hasNext()) 
  {
    NetworkNodeLabel *currentLabel = it2.next();
    if (_labelsShown)
    {
      if (currentLabel->getNode()->isVisible())
      {
        currentLabel->show();
      }
      else
      {
        currentLabel->hide();
      }
    }
    else
    {
      currentLabel->hide();
    }
  }
}

void SystemGraphWidget::increaseLabelSize()
{
  QVectorIterator<NetworkNodeLabel*> it(_labelVector);
  while (it.hasNext()) 
  {
    NetworkNodeLabel *currentLabel = it.next();
    currentLabel->increaseFontSize();
  }
}

void SystemGraphWidget::decreaseLabelSize()
{
  QVectorIterator<NetworkNodeLabel*> it(_labelVector);
  while (it.hasNext()) 
  {
    NetworkNodeLabel *currentLabel = it.next();
    currentLabel->decreaseFontSize();
  }
}


void SystemGraphWidget::setWeightControls()
{
  _maxWeight = 0;
  QVectorIterator<DirectedEdge*> it(_edgeVector);
  while (it.hasNext())
  {
    DirectedEdge *edge = it.next();
    if (edge->isVisible())
    {
      int weight = _edgeWeights.value(edge);
      if (weight > _maxWeight)
      {
        _maxWeight = weight;
      }
    }
  }
  weightSpinBox->setEnabled(true);
  weightSpinBox->setRange(1, _maxWeight);
}

void SystemGraphWidget::updateWeightControls()
{
  int currentWeight = weightSpinBox->value();
  _maxWeight = 0;
  QVectorIterator<DirectedEdge*> it(_edgeVector);
  while (it.hasNext())
  {
    DirectedEdge *edge = it.next();
    if (edge->isVisible())
    {
      int weight = _edgeWeights.value(edge);
      if (weight > _maxWeight)
      {
        _maxWeight = weight;
      }
    }
  }
  if (weightSpinBox->value() > _maxWeight)
  {
    weightSpinBox->setValue(_maxWeight);
  }
  weightSpinBox->setRange(1, _maxWeight);
  if (_maxWeight < currentWeight)
  {
    currentWeight = _maxWeight;
  }
  weightSpinBox->setRange(1, _maxWeight);
  weightSpinBox->setValue(currentWeight);
}

void SystemGraphWidget::exportSvg()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("New svg file"),"", tr("svg files (*.svg)"));
  if (!fileName.trimmed().isEmpty()) 
  {
    if (!fileName.endsWith(".svg"))
    {
      fileName.append(".svg");
    }
    scene->clearSelection();
    QVectorIterator<GuideLine*> it(_guidesVector);
    while (it.hasNext())
    {
      it.next()->hide();
    }
    QSvgGenerator gen;
    gen.setFileName(fileName);
    QRectF currentRect = this->scene->itemsBoundingRect();
    currentRect.setX(currentRect.x());
    currentRect.setY(currentRect.y());
    currentRect.setWidth(currentRect.width());
    currentRect.setHeight(currentRect.height());
    gen.setSize(QSize(currentRect.width(), currentRect.height()));
    gen.setViewBox(QRect(0, 0, currentRect.width(), currentRect.height()));
    int dpiX = view->logicalDpiX();
    gen.setResolution(dpiX);
    QPainter painter;
    painter.begin(&gen);
    scene->render(&painter);
    painter.end();
    while (it.hasNext())
    {
      it.next()->show();
    }
  }
}

void SystemGraphWidget::exportNodes()
{
  // TODO
}

void SystemGraphWidget::exportEdges()
{
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
    // Let us first create the file header.
    fileOut << "Source" << ","
            << "Target" << ","
            << "Type" << ","
            << "Weight" << ","
            << "Description" << "\n";
    // Then we iterate through the directed edges first.
    QVectorIterator<DirectedEdge*> it(_edgeVector);
    while (it.hasNext())
    {
      DirectedEdge *directed = it.next();
      if (directed->isVisible())
      {
        QString source = directed->getStart()->getName();
        QString target = directed->getEnd()->getName();
        QString description = directed->getType();
        // TODO: update weight
        int weight = 1;
        fileOut << "\"" << doubleQuote(source).toStdString() << "\"" << ","
                << "\"" << doubleQuote(target).toStdString() << "\"" << ","
                << "Directed" << ","
                << weight << ","
                << "\"" << doubleQuote(description).toStdString() << "\"" << "\n";
      }
    }
    // And that should be it!
    fileOut.close();
  }
}

void SystemGraphWidget::updateEdges()
{
  QVectorIterator<DirectedEdge*> it(_edgeVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    current->updatePosition();
  }
}

void SystemGraphWidget::setVisibility()
{
  updateWeightControls();
  QVectorIterator<NetworkNode*> it(_nodeVector);
  while (it.hasNext())
  {
    NetworkNode *currentNode = it.next();
    if (currentNode->isMassHidden())
    {
      currentNode->hide();
    }
    else
    {
      currentNode->show();
    }
  }
  QVectorIterator<DirectedEdge*> it2(_edgeVector);
  while (it2.hasNext())
  {
    bool show = false;
    DirectedEdge *currentDirected = it2.next();
    QString relationship = currentDirected->getName();
    QString type = currentDirected->getType();
    if (currentDirected->isMassHidden())
    {
      show = false;
    }
    else if (_edgeWeights.value(currentDirected) < weightSpinBox->value())
    {
      show = false;
    }
    else
    {
      show = true;
    }
    if (show)
    {
      if (!currentDirected->getStart()->isMassHidden() && !currentDirected->getEnd()->isMassHidden())
      {
        currentDirected->show();
        currentDirected->getStart()->show();
        currentDirected->getEnd()->show();
        if (weightCheckBox->checkState() == Qt::Checked)
        {
          qreal originalWeight = (qreal) _edgeWeights.value(currentDirected);
          if (originalWeight == 1.0f || _maxWeight == weightSpinBox->value())
          {
            currentDirected->setPenWidth(1.0f);
          }
          else
          {
            qreal normalizedWeight =
              ((originalWeight - weightSpinBox->value()) /
               (_maxWeight - weightSpinBox->value())) * (5.0f *1.0f) + 1.0f;
            currentDirected->setPenWidth(normalizedWeight);

          }
        }
        else
        {
          currentDirected->setPenWidth(1.0f);
        }
      }
      else
      {
        currentDirected->hide();
      }
    }
    else
    {
      currentDirected->hide();
    }
  }
  processHeights();
  QVectorIterator<NetworkNodeLabel*> it3(_labelVector);
  while (it3.hasNext())
  {
    NetworkNodeLabel *label = it3.next();
    if (label->getNode()->isVisible() && _labelsShown)
    {
      label->show();
    }
    else
    {
      label->hide();
    }
  }
  updateEdges();
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
}

void SystemGraphWidget::processHeights()
{
  QList<QGraphicsItem*> edgeList;
  for (QVector<DirectedEdge*>::size_type i = 0; i != _edgeVector.length(); i++)
  {
    if (_edgeVector[i]->isVisible())
    {
      _edgeVector[i]->setHeight(20);
      edgeList.push_back(_edgeVector[i]);
    }
  }
  for (QList<QGraphicsItem*>::size_type i = 0; i != edgeList.length(); i++)
  {
    DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(edgeList[i]);
    if (i != edgeList.length() - 1)
    {
      for (QList<QGraphicsItem*>::size_type j = i + 1; j != edgeList.length(); j++)
      {
        DirectedEdge *secondDirected = qgraphicsitem_cast<DirectedEdge*>(edgeList[j]);
        if (directed && secondDirected)
        {
          if (secondDirected->getStart() == directed->getStart() &&
              secondDirected->getEnd() == directed->getEnd())
          {
            directed->setHeight(directed->getHeight() + 60);
          }
        }
      }
    }
  }
}

void SystemGraphWidget::cleanUp()
{
  _currentData.clear();
  scene->clearSelection();
  qDeleteAll(_edgeVector);
  _edgeVector.clear();
  qDeleteAll(_nodeVector);
  _nodeVector.clear();
  qDeleteAll(_labelVector);
  _labelVector.clear();
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
  scene->clear();
  nodeListWidget->setRowCount(0);
  edgeListWidget->setRowCount(0);
  _maxWeight = 0;
  _selectedEntityName = "";
  _selectedType = "";
  nameField->clear();
  descriptionField->clear();
  snapGuidesButton->setChecked(false);
  toggleSnapGuides();
  setGraphControls(true);
  weightCheckBox->setCheckState(Qt::Unchecked);
  disableModeButtons();
  disableTypeButtons();
}

void SystemGraphWidget::finalBusiness()
{
  cleanUp();
}

bool SystemGraphWidget::eventFilter(QObject *object, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonRelease)
  {
    resetZoomSlider();
  }
  if (object == view->viewport() && event->type() == QEvent::MouseButtonRelease) 
  {
    retrieveData();
    if (_massMove)
    {
      _massMove = false;
    }
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


