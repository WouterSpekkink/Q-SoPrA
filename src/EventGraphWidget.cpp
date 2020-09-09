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

#include "../include/EventGraphWidget.h"
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#include <QGL>
#endif

EventGraphWidget::EventGraphWidget(QWidget *parent) : QWidget(parent) 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT coder FROM save_data");
  query->first();
  _selectedCoder = query->value(0).toString();
  delete query;
  _selectedCompare = "";
  _selectedAbstractNode = NULL;
  _selectedIncident = 0;
  _commentBool = false;
  _vectorPos = 0;
  _labelsVisible = true;
  _labelSize = 10;
  _contracted = false;
  _currentMajorInterval = 100.0;
  _currentMinorDivision = 2.0;
  _currentMajorTickSize = 20.0;
  _currentMinorTickSize = 10.0;
  _currentTimeLineWidth = 1;
  _currentPenStyle = 1;
  _currentPenWidth = 1;
  _currentLineColor = QColor(Qt::black);
  _currentFillColor = QColor(Qt::black);
  _currentFillColor.setAlpha(0);
  _currentTimeLineColor = QColor(Qt::black);
  
  scene = new Scene(this);
  view = new GraphicsView(scene);
  view->setDragMode(QGraphicsView::RubberBandDrag);
  view->setRubberBandSelectionMode(Qt::ContainsItemShape);
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
  scene->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
  
  infoWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
  attWidget = new QWidget(this);
  commentWidget = new QWidget(this);
  legendWidget = new QWidget(this);
  timeLineWidget = new QWidget(this);

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
  incongruenceLabel = new QLabel(tr(""), this);
  incongruenceLabel->setStyleSheet("QLabel {color : red;}");
  eventLegendLabel = new QLabel(tr("<b>Modes:</b>"), legendWidget);
  linkageLegendLabel = new QLabel(tr("<b>Linkages:</b>"), legendWidget);
  zoomLabel = new QLabel(tr("<b>Zoom slider:</b>"), this);
  shapesLabel = new QLabel(tr("<b>Shapes:</b>"), this);
  penStyleLabel = new QLabel(tr("<b>Pen style:</b>"), this);
  penWidthLabel = new QLabel(tr("<b>Pen width:</b>"), this);
  lineColorLabel = new QLabel(tr("<b>Line / Text color:</b>"), this);
  fillColorLabel = new QLabel(tr("<b>Fill color:</b>"), this);
  timeLineLabel = new QLabel(tr("<b>Add timeline:</b>"), timeLineWidget);
  majorIntervalLabel = new QLabel(tr("<b>Major tick interval:</b>"), timeLineWidget);
  minorDivisionLabel = new QLabel(tr("<b>Minor tick division:</b>"), timeLineWidget);
  majorTickSizeLabel = new QLabel(tr("<b>Major tick size:</b>"), timeLineWidget);
  minorTickSizeLabel = new QLabel(tr("<b>Minor tick size:</b>"), timeLineWidget);
  timeLineWidthLabel = new QLabel(tr("<b>Pen width:</b>"), timeLineWidget);
  timeLineColorLabel = new QLabel(tr("<b>Color:</b>"), timeLineWidget);
  guideLinesLabel = new QLabel(tr("<b>Add guides:</b>"), this);
  fillOpacityLabel = new QLabel(tr("<b>Opacity:</b>"), this);
  labelSizeLabel = new QLabel(tr("<b>Label size:</b>"), graphicsWidget);
  layoutLabel = new QLabel(tr("<b>Layout:</b>"), this);
  
  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);

  compareComboBox = new QComboBox(this);
  compareComboBox->addItem(DEFAULT);

  layoutComboBox = new QComboBox(this);
  layoutComboBox->addItem(REDOLAYOUT);
  layoutComboBox->addItem(MINIMISELAYOUT);
  layoutComboBox->addItem(DATELAYOUTDAYS);
  layoutComboBox->addItem(DATELAYOUTMONTHS);
  layoutComboBox->addItem(DATELAYOUTYEARS);
  layoutComboBox->addItem(DATELAYOUTDECADES);
  layoutComboBox->addItem(NOOVERLAP);
  
  lowerRangeDial = new QDial(graphicsWidget);
  lowerRangeDial->setSingleStep(1);
  upperRangeDial = new QDial(graphicsWidget);
  upperRangeDial->setSingleStep(1);
  lowerRangeSpinBox = new QSpinBox(graphicsWidget);
  upperRangeSpinBox = new QSpinBox(graphicsWidget);

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
  addLinkageTypeButton = new QPushButton(tr("Add linkage type"), this);
  addLinkageTypeButton->setEnabled(false);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  plotButton->setEnabled(false);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  exportTableButton = new QPushButton(tr("Export table"), graphicsWidget);
  exportNodesButton = new QPushButton(tr("Export nodes"), graphicsWidget);
  exportEdgesButton = new QPushButton(tr("Export edges"), graphicsWidget);
  increaseDistanceButton = new QPushButton(QIcon(":/images/expand_horizontal.png"), "", this);
  increaseDistanceButton->setIconSize(QSize(20, 20));
  increaseDistanceButton->setMinimumSize(40, 40);
  increaseDistanceButton->setMaximumSize(40, 40);
  decreaseDistanceButton = new QPushButton(QIcon(":/images/contract_horizontal.png"), "", this);
  decreaseDistanceButton->setIconSize(QSize(20, 20));
  decreaseDistanceButton->setMinimumSize(40, 40);
  decreaseDistanceButton->setMaximumSize(40, 40);
  expandButton = new QPushButton(QIcon(":/images/expand_vertical.png"), "", this);
  expandButton->setIconSize(QSize(20, 20));
  expandButton->setMinimumSize(40, 40);
  expandButton->setMaximumSize(40, 40);
  contractButton = new QPushButton(QIcon(":/images/contract_vertical.png"), "", this);
  contractButton->setIconSize(QSize(20, 20));
  contractButton->setMinimumSize(40, 40);
  contractButton->setMaximumSize(40, 40);
  compareButton = new QPushButton(tr("Compare"), this);
  toggleDetailsButton = new QPushButton(tr("Toggle details"), this);
  toggleDetailsButton->setCheckable(true);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  toggleTimeLineButton = new QPushButton(tr("Toggle timeline controls"), this);
  toggleTimeLineButton->setCheckable(true);
  previousEventButton = new QPushButton("<<", infoWidget);
  previousEventButton->setEnabled(false);   
  nextEventButton = new QPushButton(">>", infoWidget);
  seeComponentsButton = new QPushButton(tr("Components"), infoWidget);
  seeComponentsButton->setEnabled(false);
  nextEventButton->setEnabled(false);
  toggleLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  increaseLabelSizeButton = new QPushButton("+", graphicsWidget);
  decreaseLabelSizeButton = new QPushButton("-", graphicsWidget);
  addModeButton = new QPushButton(tr("Create single mode"), legendWidget);
  addModesButton = new QPushButton(tr("Create multiple modes"), legendWidget);
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
  restoreModeColorsButton = new QPushButton(tr("Restore modes"), legendWidget);
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
  makeLayoutButton = new QPushButton(tr("Run layout"), this);
  setTimeRangeButton = new QPushButton(tr("Set time range"), graphicsWidget);
  hideModeButton = new QPushButton(tr("Hide"), legendWidget);
  hideModeButton->setCheckable(true);
  hideModeButton->setChecked(false);
  showModeButton = new QPushButton(tr("Show"), legendWidget);
  showModeButton->setCheckable(true);
  showModeButton->setChecked(true);
  disableLegendButtons();
  
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
  addTimeLineButton = new QPushButton(QIcon(":/images/timeline.png"), "", timeLineWidget);
  addTimeLineButton->setIconSize(QSize(20, 20));
  addTimeLineButton->setMinimumSize(40, 40);
  addTimeLineButton->setMaximumSize(40, 40);
  majorIntervalSlider = new QSlider(Qt::Horizontal, timeLineWidget);
  majorIntervalSlider->setMinimum(5.0);
  majorIntervalSlider->setMaximum(5000.0);
  majorIntervalSlider->setValue(100.0);
  majorIntervalSlider->setTickInterval(1.0);
  majorIntervalSpinBox = new QSpinBox(timeLineWidget);
  majorIntervalSpinBox->setMinimum(5);
  majorIntervalSpinBox->setMaximum(5000);
  majorIntervalSpinBox->setValue(100);
  minorDivisionSlider = new QSlider(Qt::Horizontal, timeLineWidget);
  minorDivisionSlider->setMinimum(1);
  minorDivisionSlider->setMaximum(20);
  minorDivisionSlider->setTickInterval(1);
  minorDivisionSlider->setValue(2);
  minorDivisionSpinBox = new QSpinBox(timeLineWidget);
  minorDivisionSpinBox->setMinimum(1);
  minorDivisionSpinBox->setMaximum(20);
  minorDivisionSpinBox->setValue(2);
  majorTickSizeSlider = new QSlider(Qt::Horizontal, timeLineWidget);
  majorTickSizeSlider->setMinimum(1.0);
  majorTickSizeSlider->setMaximum(500.0);
  majorTickSizeSlider->setValue(20.0);
  minorTickSizeSlider = new QSlider(Qt::Horizontal, timeLineWidget);
  minorTickSizeSlider->setMinimum(1.0);
  minorTickSizeSlider->setMaximum(500.0);
  minorTickSizeSlider->setValue(20.0);
  timeLineWidthSpinBox = new QSpinBox(timeLineWidget);
  timeLineWidthSpinBox->setMinimum(1);
  timeLineWidthSpinBox->setMaximum(20);
  QPixmap timeLineColorMap(20, 20);
  timeLineColorMap.fill(_currentTimeLineColor);
  QIcon timeLineColorIcon(timeLineColorMap);
  changeTimeLineColorButton = new QPushButton(timeLineColorIcon, "", timeLineWidget);
  changeTimeLineColorButton->setIconSize(QSize(20, 20));
  changeTimeLineColorButton->setMinimumSize(40, 40);
  changeTimeLineColorButton->setMaximumSize(40, 40);
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
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButtons()));
  connect(compareComboBox, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setCompareButton()));  
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotGraph()));
  connect(addLinkageTypeButton, SIGNAL(clicked()), this, SLOT(addLinkageType()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(toggleLabelsButton, SIGNAL(clicked()), this, SLOT(toggleLabels()));
  connect(increaseLabelSizeButton, SIGNAL(clicked()), this, SLOT(increaseLabelSize()));
  connect(decreaseLabelSizeButton, SIGNAL(clicked()), this, SLOT(decreaseLabelSize()));
  connect(addModeButton, SIGNAL(clicked()), this, SLOT(addMode()));
  connect(addModesButton, SIGNAL(clicked()), this, SLOT(addModes()));
  connect(eventColorButton, SIGNAL(clicked()), this, SLOT(setEventColor()));
  connect(labelColorButton, SIGNAL(clicked()), this, SLOT(setLabelColor()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(exportTableButton, SIGNAL(clicked()), this, SLOT(exportTable()));
  connect(exportNodesButton, SIGNAL(clicked()), this, SLOT(exportNodes()));
  connect(exportEdgesButton, SIGNAL(clicked()), this, SLOT(exportEdges()));
  connect(compareButton, SIGNAL(clicked()), this, SLOT(compare()));
  connect(addLineButton, SIGNAL(clicked()), scene, SLOT(prepLinePoints()));
  connect(addSingleArrowButton, SIGNAL(clicked()), scene, SLOT(prepSingleArrowPoints()));
  connect(addDoubleArrowButton, SIGNAL(clicked()), scene, SLOT(prepDoubleArrowPoints()));
  connect(addEllipseButton, SIGNAL(clicked()), scene, SLOT(prepEllipseArea()));
  connect(addRectangleButton, SIGNAL(clicked()), scene, SLOT(prepRectArea()));
  connect(addTextButton, SIGNAL(clicked()), scene, SLOT(prepTextArea()));
  connect(addTimeLineButton, SIGNAL(clicked()), scene, SLOT(prepTimeLinePoints()));
  connect(penStyleComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPenStyle()));
  connect(penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setPenWidth()));
  connect(penStyleComboBox, SIGNAL(currentIndexChanged(int)), scene, SLOT(setPenStyle(int)));
  connect(penWidthSpinBox, SIGNAL(valueChanged(int)), scene, SLOT(setPenWidth(int)));
  connect(changeLineColorButton, SIGNAL(clicked()), this, SLOT(setLineColor()));
  connect(changeFillColorButton, SIGNAL(clicked()), this, SLOT(setFillColor()));
  connect(fillOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(setFillOpacity(int)));
  connect(majorIntervalSlider, SIGNAL(valueChanged(int)), this, SLOT(setMajorIntervalBySlider()));
  connect(majorIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setMajorIntervalBySpinBox()));
  connect(minorDivisionSlider, SIGNAL(valueChanged(int)), this, SLOT(setMinorDivisionBySlider()));
  connect(minorDivisionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setMinorDivisionBySpinBox()));
  connect(majorTickSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setMajorTickSize()));
  connect(minorTickSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setMinorTickSize()));
  connect(changeTimeLineColorButton, SIGNAL(clicked()), this, SLOT(setTimeLineColor()));
  connect(timeLineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setTimeLineWidth()));
  connect(timeLineWidthSpinBox, SIGNAL(valueChanged(int)), scene, SLOT(setTimeLineWidth(int)));
  connect(hideAnnotationsButton, SIGNAL(clicked()), this, SLOT(hideAnnotations()));
  connect(addHorizontalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepHorizontalGuideLine()));
  connect(addVerticalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepVerticalGuideLine()));
  connect(snapGuidesButton, SIGNAL(clicked()), this, SLOT(toggleSnapGuides()));
  connect(this, SIGNAL(sendLineColor(QColor &)), scene, SLOT(setLineColor(QColor &)));
  connect(this, SIGNAL(sendFillColor(QColor &)), scene, SLOT(setFillColor(QColor &)));
  connect(this, SIGNAL(sendMajorInterval(qreal &)), scene, SLOT(setMajorInterval(qreal &)));
  connect(this, SIGNAL(sendMinorDivision(qreal &)), scene, SLOT(setMinorDivision(qreal &)));
  connect(this, SIGNAL(sendMajorTickSize(qreal &)), scene, SLOT(setMajorTickSize(qreal &)));
  connect(this, SIGNAL(sendMinorTickSize(qreal &)), scene, SLOT(setMinorTickSize(qreal &)));
  connect(this, SIGNAL(sendTimeLineWidth(int)), scene, SLOT(setTimeLineWidth(int)));
  connect(this, SIGNAL(sendTimeLineColor(QColor &)), scene, SLOT(setTimeLineColor(QColor &)));
  connect(scene, SIGNAL(resetItemSelection()), this, SLOT(retrieveData()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(updateLinkages()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(scene, SIGNAL(IncidentNodeContextMenuAction(const QString &)),
	  this, SLOT(processIncidentNodeContextMenu(const QString &)));
  connect(scene, SIGNAL(LinkageContextMenuAction(const QString &)),
	  this, SLOT(processLinkageContextMenu(const QString &)));
  connect(scene, SIGNAL(LineContextMenuAction(const QString &)),
	  this, SLOT(processLineContextMenu(const QString &)));
  connect(scene, SIGNAL(TextContextMenuAction(const QString &)),
	  this, SLOT(processTextContextMenu(const QString &)));
  connect(scene, SIGNAL(EllipseContextMenuAction(const QString &)),
	  this, SLOT(processEllipseContextMenu(const QString &)));
  connect(scene, SIGNAL(RectContextMenuAction(const QString &)),
	  this, SLOT(processRectContextMenu(const QString &)));
  connect(scene, SIGNAL(TimeLineContextMenuAction(const QString &)),
	  this, SLOT(processTimeLineContextMenu(const QString &)));
  connect(scene, SIGNAL(GuideLineContextMenuAction(const QString &)),
	  this, SLOT(processGuideLineContextMenu(const QString &)));
  connect(this, SIGNAL(changeEventWidth(QGraphicsItem*)), scene, SLOT(modEventWidth(QGraphicsItem*)));
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
  connect(scene, SIGNAL(sendTimeLinePoints(const qreal&, const qreal&, const qreal&)),
	  this, SLOT(addTimeLineObject(const qreal&, const qreal&, const qreal&)));
  connect(scene, SIGNAL(sendHorizontalGuideLinePos(const QPointF&)),
	  this, SLOT(addHorizontalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(sendVerticalGuideLinePos(const QPointF&)),
	  this, SLOT(addVerticalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(processShapeSelection()));
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
  connect(makeLayoutButton, SIGNAL(clicked()), this, SLOT(makeLayout()));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(processZoomSliderChange(int)));
  connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(resetZoomSlider()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(setTimeRangeButton, SIGNAL(clicked()), this, SLOT(setTimeRange()));
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(toggleTimeLineButton, SIGNAL(clicked()), this, SLOT(toggleTimeLine()));
  connect(eventListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setModeButtons(QTableWidgetItem *)));
  connect(eventListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableLegendButtons()));
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
  connect(hideModeButton, SIGNAL(clicked()), this, SLOT(hideMode()));
  connect(showModeButton, SIGNAL(clicked()), this, SLOT(showMode()));
  connect(exportTransitionMatrixButton, SIGNAL(clicked()), this, SLOT(exportTransitionMatrix()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
    
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(typeLabel);
  plotOptionsLayout->addWidget(typeComboBox);
  plotOptionsLayout->addWidget(plotButton);
  plotOptionsLayout->addWidget(addLinkageTypeButton);
  plotOptionsLayout->addWidget(savePlotButton);
  plotOptionsLayout->addWidget(seePlotsButton);
  plotOptionsLayout->addWidget(plotLabel);
  plotOptionsLayout->addWidget(changeLabel);
  plotOptionsLayout->addWidget(incongruenceLabel);
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

  QPointer<QFrame> topLine = new QFrame();
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  QPointer<QHBoxLayout> drawHelpLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotObjectsLayout = new QHBoxLayout;
  plotObjectsLayout->addWidget(toggleTimeLineButton);
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

  QPointer<QHBoxLayout> timeLineLayout = new QHBoxLayout;
  timeLineLayout->addWidget(timeLineLabel);
  timeLineLayout->addWidget(addTimeLineButton);
  timeLineLayout->addWidget(majorIntervalLabel);
  timeLineLayout->addWidget(majorIntervalSlider);
  timeLineLayout->addWidget(majorIntervalSpinBox);
  timeLineLayout->addWidget(majorTickSizeLabel);
  timeLineLayout->addWidget(majorTickSizeSlider);
  timeLineLayout->addWidget(minorDivisionLabel);
  timeLineLayout->addWidget(minorDivisionSlider);
  timeLineLayout->addWidget(minorDivisionSpinBox);
  timeLineLayout->addWidget(minorTickSizeLabel);
  timeLineLayout->addWidget(minorTickSizeSlider);
  timeLineLayout->addWidget(timeLineWidthLabel);
  timeLineLayout->addWidget(timeLineWidthSpinBox);
  timeLineLayout->addWidget(timeLineColorLabel);
  timeLineLayout->addWidget(changeTimeLineColorButton);
  timeLineLayout->setAlignment(Qt::AlignLeft);
  timeLineWidget->setLayout(timeLineLayout);
  mainLayout->addWidget(timeLineWidget);
  
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
  QPointer<QHBoxLayout> hideShowModeButtonsLayout = new QHBoxLayout;
  hideShowModeButtonsLayout->addWidget(hideModeButton);
  hideShowModeButtonsLayout->addWidget(showModeButton);
  legendLayout->addLayout(hideShowModeButtonsLayout);
  legendLayout->addWidget(addModeButton);
  legendLayout->addWidget(addModesButton);
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
  graphicsControlsLayout->addWidget(toggleLabelsButton);
  graphicsControlsLayout->addWidget(labelSizeLabel);
  QPointer<QHBoxLayout> labelSizeLayout = new QHBoxLayout;
  labelSizeLayout->addWidget(increaseLabelSizeButton);
  labelSizeLayout->addWidget(decreaseLabelSizeButton);
  graphicsControlsLayout->addLayout(labelSizeLayout);
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
  graphicsControlsLayout->addWidget(setTimeRangeButton);
  QPointer<QFrame> sepLine2 = new QFrame();
  sepLine2->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine2);
  graphicsControlsLayout->addWidget(exportSvgButton);
  graphicsControlsLayout->addWidget(exportTableButton);
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
  drawOptionsLeftLayout->addWidget(makeLayoutButton);
  makeLayoutButton->setMaximumWidth(makeLayoutButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(increaseDistanceButton);
  increaseDistanceButton->setMaximumWidth(increaseDistanceButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(decreaseDistanceButton);
  decreaseDistanceButton->setMaximumWidth(decreaseDistanceButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(expandButton);
  expandButton->setMaximumWidth(expandButton->sizeHint().width());
  drawOptionsLeftLayout->addWidget(contractButton);
  contractButton->setMaximumWidth(contractButton->sizeHint().width());
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
  QPointer<QFrame> vertLineFour = new QFrame();
  vertLineFour->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineFour);

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
  timeLineWidget->hide();
  updateCases();

  setGraphControls(false);
 }

EventGraphWidget::~EventGraphWidget()
{
  qDeleteAll(_edgeVector);
  _edgeVector.clear();
  qDeleteAll(_abstractNodeVector);
  _abstractNodeVector.clear();
  qDeleteAll(_abstractNodeLabelVector);
  _abstractNodeLabelVector.clear();
  qDeleteAll(_incidentNodeVector);
  _incidentNodeVector.clear();
  qDeleteAll(_incidentNodeLabelVector);
  _incidentNodeLabelVector.clear();
  qDeleteAll(_compareVector);
  _compareVector.clear();
  qDeleteAll(_lineVector);
  _lineVector.clear();
  qDeleteAll(_textVector);
  _textVector.clear();
  qDeleteAll(_ellipseVector);
  _ellipseVector.clear();
  qDeleteAll(_rectVector);
  _rectVector.clear();
  qDeleteAll(_timeLineVector);
  _timeLineVector.clear();
  qDeleteAll(_guidesVector);
  _guidesVector.clear();
  delete view;
  delete scene;
}

void EventGraphWidget::checkCongruence() 
{
  if (_incidentNodeVector.size() > 0) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT id FROM incidents ORDER BY ch_order ASC");
      QVector<int> temp;
      while (query->next()) 
	{
	  int id = query->value(0).toInt();
	  temp.push_back(id);
	}
      std::sort(_incidentNodeVector.begin(), _incidentNodeVector.end(), componentsSort);
      if (temp.size() != _incidentNodeVector.size()) 
	{
	  incongruenceLabel->setText("Incongruence detected");
	  delete query;
	  return;
	}
      for (QVector<IncidentNode *>::size_type i = 0; i != _incidentNodeVector.size(); i++) 
	{
	  IncidentNode *current = _incidentNodeVector[i];
	  if (current->getId() != temp[i]) 
	    {
	      incongruenceLabel->setText("Incongruence detected");
	      delete query;
	      return;
	    }
	}
      QVectorIterator<QString> it(_presentTypes);
      while (it.hasNext()) 
	{
	  QString currentType = it.next();
	  query->prepare("SELECT tail, head FROM linkages "
			 "WHERE type = :type AND coder = :coder");
	  query->bindValue(":type", currentType);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      int tailId = query->value(0).toInt();
	      int headId = query->value(1).toInt();
	      bool tailVisible = false;
	      bool headVisible = false;
	      QVectorIterator<IncidentNode *> it2(_incidentNodeVector);
	      while (it2.hasNext()) 
		{
		  IncidentNode *currentIncidentNode = it2.next();
		  if (currentIncidentNode->isVisible()) 
		    {
		      if (currentIncidentNode->getId() == tailId) 
			{
			  tailVisible = true;
			}
		      else if (currentIncidentNode->getId() == headId) 
			{
			  headVisible = true;
			}
		    }
		}
	      if (tailVisible && headVisible) 
		{
		  QVectorIterator<Linkage *> it3(_edgeVector);
		  bool found = false;
		  while (it3.hasNext()) 
		    {
		      Linkage *current = it3.next();
		      IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode *>(current->getStart());
		      IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode *>(current->getEnd());
		      if (startIncidentNode && endIncidentNode) 
			{
			  if (startIncidentNode->getId() == tailId &&
			      endIncidentNode->getId() == headId) 
			    {
			      found = true;
			    }
			}
		    }
		  if (!found) 
		    {
		      incongruenceLabel->setText("Incongruence detected");
		      delete query;
		      return;
		    }
		}
	    }
	}
      QVectorIterator<Linkage *> it3(_edgeVector);
      while (it3.hasNext()) 
	{
	  Linkage *current = it3.next();
	  QString currentType = current->getType();
	  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode*>(current->getStart());
	  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode*>(current->getEnd());
	  if (startIncidentNode && endIncidentNode) 
	    {
	      int currentTail = startIncidentNode->getId();
	      int currentHead = endIncidentNode->getId();
	      bool found = false;
	      query->prepare("SELECT tail, head FROM linkages "
			     "WHERE type = :type AND coder = :coder");
	      query->bindValue(":type", currentType);
	      query->bindValue(":coder", _selectedCoder);
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
		  incongruenceLabel->setText("Incongruence detected");
		  delete query;
		  return;
		}
	    }
	}
      // Let's check for congruence of cases
      QVector<QString> currentVector;
      for (int i = 0; i != caseListWidget->count(); i++)
	{
	  QListWidgetItem *item = caseListWidget->item(i);
	  currentVector.push_back(item->text());
	}
      QVector<QString> caseVector;
      query->exec("SELECT name FROM cases");
      while (query->next()) 
	{
	  QString currentCase = query->value(0).toString();
	  caseVector.push_back(currentCase);
	}
      QVectorIterator<QString> cit(caseVector);
      while (cit.hasNext())
	{
	  if (!currentVector.contains(cit.next()))
	    {
	      incongruenceLabel->setText("Incongruence detected");
	      delete query;
	      return;
	    }
	}
      QVectorIterator<QString> cit2(currentVector);
      while (cit2.hasNext())
	{
	  if (!caseVector.contains(cit2.next()))
	    {
	      incongruenceLabel->setText("Incongruence detected");
	      delete query;
	      return;
	    }
	}
      delete query;
      incongruenceLabel->setText("");
    }
}

void EventGraphWidget::setOpenGL(bool state)
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
      view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
      view->viewport()->installEventFilter(this);
      view->setScene(scene);
    }
  else
    {
      view->setViewport(new QWidget());
      view->viewport()->installEventFilter(this);
      view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    }
}

void EventGraphWidget::setAntialiasing(bool state)
{
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext())
    {
      Linkage *current = it.next();
      current->setAntialiasing(state);
    }
}

void EventGraphWidget::setCommentBool() 
{
  _commentBool = true;
}

void EventGraphWidget::setComment() 
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

void EventGraphWidget::toggleTimeLine()
{
  if (timeLineWidget->isHidden())
    {
      timeLineWidget->show();
    }
  else
    {
      timeLineWidget->hide();
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

void EventGraphWidget::setGraphControls(bool state)
{
  zoomSlider->setEnabled(state);
  expandButton->setEnabled(state);
  contractButton->setEnabled(state);
  increaseDistanceButton->setEnabled(state);
  decreaseDistanceButton->setEnabled(state);
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
  addTimeLineButton->setEnabled(state);
  majorIntervalSlider->setEnabled(state);
  majorIntervalSpinBox->setEnabled(state);
  minorDivisionSlider->setEnabled(state);
  minorDivisionSpinBox->setEnabled(state);
  majorTickSizeSlider->setEnabled(state);
  minorTickSizeSlider->setEnabled(state);
  timeLineWidthSpinBox->setEnabled(state);
  changeTimeLineColorButton->setEnabled(state);
  addHorizontalGuideLineButton->setEnabled(state);
  addVerticalGuideLineButton->setEnabled(state);
  fillOpacitySlider->setEnabled(state);
  snapGuidesButton->setEnabled(state);
  increaseLabelSizeButton->setEnabled(state);
  decreaseLabelSizeButton->setEnabled(state);
  layoutComboBox->setEnabled(state);
  makeLayoutButton->setEnabled(state);
  hideAnnotationsButton->setEnabled(state);
  setTimeRangeButton->setEnabled(state);
  exportTransitionMatrixButton->setEnabled(state);
  restoreModeColorsButton->setEnabled(state);
  addModeButton->setEnabled(state);
  addModesButton->setEnabled(state);
  toggleLabelsButton->setEnabled(state);
  eventColorButton->setEnabled(state);
  labelColorButton->setEnabled(state);
  backgroundColorButton->setEnabled(state);
  increaseLabelSizeButton->setEnabled(state);
  decreaseLabelSizeButton->setEnabled(state);
  exportSvgButton->setEnabled(state);
  exportNodesButton->setEnabled(state);
  exportEdgesButton->setEnabled(state);
  exportTableButton->setEnabled(state);
  lowerRangeDial->setEnabled(state);
  upperRangeDial->setEnabled(state);
  lowerRangeSpinBox->setEnabled(state);
  upperRangeSpinBox->setEnabled(state);

}

void EventGraphWidget::updateCases() 
{
  restorePositions();
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
  if (_contracted)
    {
      restorePositions();
    }
  for (int i = 0; i != caseListWidget->count(); i++) 
    {
      QListWidgetItem *item = caseListWidget->item(i);
      if (item->checkState() == Qt::Checked) 
	{
	  if (!_checkedCases.contains(item->data(Qt::DisplayRole).toString())) 
	    {
	      _checkedCases.push_back(item->data(Qt::DisplayRole).toString());
	    }
	}
      else 
	{
	  if (_checkedCases.contains(item->data(Qt::DisplayRole).toString())) 
	    {
	      _checkedCases.removeOne(item->data(Qt::DisplayRole).toString());
	    }
	}
    }
  setVisibility();
}

QVector<QString> EventGraphWidget::getCheckedCases()
{
  return _checkedCases;
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
      if (_selectedIncident != 0)
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE attributes_to_incidents "
			 "SET value = :val "
			 "WHERE attribute = :attribute AND incident = :incident AND coder = :coder");
	  query->bindValue(":val", valueField->text());
	  query->bindValue(":attribute", attribute);
	  query->bindValue(":incident", _selectedIncident);
	  query->bindValue(":coder", _selectedCoder);
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

void EventGraphWidget::retrieveData() 
{
  setComment();
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->setEnabled(false);
  valueField->blockSignals(false);
  valueButton->setEnabled(false);
  attributesTreeView->resetSelection();
  if (_currentData.size() > 0) 
    {
      _currentData.clear();
    }
  if (scene->selectedItems().size() > 0) 
    {
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext()) 
	{
	  IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(it.peekNext());
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(it.peekNext());
	  Linkage *linkage = qgraphicsitem_cast<Linkage*>(it.peekNext());
	  if (incidentNode) 
	    {
	      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(it.next());
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
	  else if (linkage && scene->selectedItems().size() > 1) 
	    {
	      scene->blockSignals(true);
	      linkage->setSelected(false);
	      scene->blockSignals(false);
	      it.next();
	    }
	  else 
	    {
	      it.next();
	    }
	}
      if (_currentData.size() > 0) 
	{
	  std::sort(_currentData.begin(), _currentData.end(), eventLessThan);   
	  _vectorPos = 0;
	  QString indexText = "(" + QString::number(_vectorPos + 1) + "/"
	    + QString::number(_currentData.size()) + ")";
	  indexLabel->setText(indexText);
	  IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
	  AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
	  if (currentIncidentNode) 
	    {
	      _selectedAbstractNode = NULL;
	      seeComponentsButton->setEnabled(false);
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
		  QSqlQuery *query2 = new QSqlQuery;
		  query2->prepare("SELECT attribute FROM attributes_to_incidents "
				  "WHERE incident = :id AND coder = :coder");
		  query2->bindValue(":id", id);
		  query2->bindValue(":coder", _selectedCoder);
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
	  else if (currentAbstractNode) 
	    {
	      _selectedAbstractNode = currentAbstractNode;
	      _selectedIncident = 0;
	      currentAbstractNode->setSelectionColor(Qt::red);
	      currentAbstractNode->update();
	      seeComponentsButton->setEnabled(true);
	      descriptionField->setText(currentAbstractNode->getDescription());
	      sourceLabel->setText("<b>Number of incidents:</b>");
	      int id = currentAbstractNode->getIncidents().first()->getId();
	      rawLabel->hide();
	      rawField->hide();
	      QString timing = currentAbstractNode->getTiming();
	      QString countText = QString::number(currentAbstractNode->getIncidents().size());
	      timeStampField->setText(timing);
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
      _selectedIncident = 0;
      _selectedAbstractNode = NULL;
      seeComponentsButton->setEnabled(false);
      resetFont(attributesTree);
    }
}

void EventGraphWidget::seeComponents() 
{
  emit seeHierarchy(_selectedAbstractNode);
}

void EventGraphWidget::previousDataItem() 
{
  setComment();
  if (_vectorPos > 0) 
    {
      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if(currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::black);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::black);
	  currentAbstractNode->update();
	}
      _vectorPos--;
      QString indexText = "(" + QString::number(_vectorPos + 1) + "/"
	+ QString::number(_currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if (currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::red);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::red);
	  currentAbstractNode->update();
	}
      if (currentIncidentNode) 
	{
	  _selectedAbstractNode = NULL;
	  seeComponentsButton->setEnabled(false);
	  sourceLabel->setText("<b>Source:</b>");
	  rawLabel->show();
	  rawField->show();

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
	    }
	  resetFont(attributesTree);
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :id AND coder = :coder");
	  query->bindValue(":id", id);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      QString attribute = query->value(0).toString();
	      boldSelected(attributesTree, attribute, id, INCIDENT);
	    }
	  delete query;
	}
      else if (currentAbstractNode) 
	{
	  _selectedAbstractNode = currentAbstractNode;
	  _selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentAbstractNode->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentAbstractNode->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
	  QString timestamp = currentAbstractNode->getTiming();
	  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
	  QString countText = QString::number(currentAbstractNode->getIncidents().size());
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
  else 
    {
      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if(currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::black);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::black);
	  currentAbstractNode->update();
	}
      _vectorPos = _currentData.size() - 1;
      QString indexText = "(" + QString::number(_vectorPos + 1) + "/"
	+ QString::number(_currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if (currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::red);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::red);
	  currentAbstractNode->update();
	}
      if (currentIncidentNode) 
	{
	  _selectedAbstractNode = 0;
	  seeComponentsButton->setEnabled(false);
	  sourceLabel->setText("<b>Source:</b>");
	  rawLabel->show();
	  rawField->show();
      
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
	    }
	  resetFont(attributesTree);
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :id AND coder = :coder");
	  query->bindValue(":id", id);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      QString attribute = query->value(0).toString();
	      boldSelected(attributesTree, attribute, id, INCIDENT);
	    }
	  delete query;
	}
      else if (currentAbstractNode) 
	{
	  _selectedAbstractNode = currentAbstractNode;
	  _selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentAbstractNode->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentAbstractNode->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
	  QString timestamp = currentAbstractNode->getTiming();
	  QString countText = QString::number(currentAbstractNode->getIncidents().size());
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

void EventGraphWidget::nextDataItem() 
{
  setComment();
  if (_vectorPos != _currentData.size() - 1) 
    {
      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if(currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::black);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::black);
	  currentAbstractNode->update();
	}
      _vectorPos++;
      QString indexText = "(" + QString::number(_vectorPos + 1) + "/"
	+ QString::number(_currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if (currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::red);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::red);
	  currentAbstractNode->update();
	}
      if (currentIncidentNode) 
	{
	  _selectedAbstractNode = NULL;
	  seeComponentsButton->setEnabled(false);
	  sourceLabel->setText("<b>Source:</b>");
	  rawLabel->show();
	  rawField->show();
      
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
	    }
	  resetFont(attributesTree);
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :id AND coder = :coder");
	  query->bindValue(":id", id);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      QString attribute = query->value(0).toString();
	      boldSelected(attributesTree, attribute, id, INCIDENT);
	    }
	  delete query;
	}
      else if (currentAbstractNode) 
	{
	  _selectedAbstractNode = currentAbstractNode;
	  _selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentAbstractNode->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentAbstractNode->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
	  QString timestamp =  currentAbstractNode->getTiming();
	  QString countText = QString::number(currentAbstractNode->getIncidents().size());
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
  else 
    {
      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if(currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::black);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::black);
	  currentAbstractNode->update();
	}
      _vectorPos = 0;
      QString indexText = "(" + QString::number(_vectorPos + 1) + "/"
	+ QString::number(_currentData.size()) + ")";
      indexLabel->setText(indexText);
      currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(_currentData.at(_vectorPos));
      currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(_currentData.at(_vectorPos));
      if (currentIncidentNode) 
	{
	  currentIncidentNode->setSelectionColor(Qt::red);
	  currentIncidentNode->update();
	}
      else if (currentAbstractNode) 
	{
	  currentAbstractNode->setSelectionColor(Qt::red);
	  currentAbstractNode->update();
	}
      if (currentIncidentNode) 
	{
	  _selectedAbstractNode = NULL;
	  seeComponentsButton->setEnabled(false);
	  timeStampLabel->setText("<b>Timing:</b>");
	  sourceLabel->setText("<b>Source:</b>");
	  rawLabel->show();
	  rawField->show();

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
	      delete query;
	      resetFont(attributesTree);
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT attribute FROM attributes_to_incidents "
			     "WHERE incident = :id AND coder = :coder");
	      query->bindValue(":id", id);
	      query->bindValue(":coder", _selectedCoder);
	      query->exec();
	      while (query->next()) 
		{
		  QString attribute = query->value(0).toString();
		  boldSelected(attributesTree, attribute, id, INCIDENT);
		}
	    }
	}
      else if (currentAbstractNode) 
	{
	  _selectedAbstractNode = currentAbstractNode;
	  _selectedIncident = 0;
	  seeComponentsButton->setEnabled(true);
	  descriptionField->setText(currentAbstractNode->getDescription());
	  sourceLabel->setText("<b>Number of incidents:</b>");
	  int id = currentAbstractNode->getIncidents().first()->getId();
	  rawLabel->hide();
	  rawField->hide();
 	  QString timestamp =  currentAbstractNode->getTiming();
	  QString countText = QString::number(currentAbstractNode->getIncidents().size());
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
      int fontSize = currentAttribute->font().pointSize();
      QFont font;
      font.setBold(false);
      font.setUnderline(false);
      font.setPointSize(fontSize);
      QFont font2;
      font2.setItalic(true);
      font2.setBold(false);
      font2.setUnderline(false);
      font2.setPointSize(fontSize);
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

void EventGraphWidget::changeTreeFontSize(QAbstractItemModel *model, QModelIndex parent, int size)
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
      QFont font = currentAttribute->font();
      int fontSize = font.pointSize();
      if ((size == -1 && fontSize >= 10) ||
	  (size == 1 && fontSize <= 50))
	{
	  font.setPointSize(fontSize + size);
	  currentAttribute->setFont(font);
	  if (model->hasChildren(index)) 
	    {
	      changeTreeFontSize(model, index, size);
	    }
	}
    }
}

void EventGraphWidget::boldSelected(QAbstractItemModel *model, QString name,
				    int incidentNode, QString type, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QString currentName = model->data(index).toString();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
      int fontSize = currentAttribute->font().pointSize();
      QFont font;
      font.setBold(true);
      font.setPointSize(fontSize);
      QFont font2;
      font2.setUnderline(true);
      font2.setPointSize(fontSize);
      QFont font3;
      font3.setBold(true);
      font3.setUnderline(true);
      font3.setPointSize(fontSize);
      QFont font4;
      font4.setItalic(true);
      font4.setPointSize(fontSize);
      QFont font5;
      font5.setItalic(true);
      font5.setUnderline(true);
      font5.setPointSize(fontSize);
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
			      query->prepare("SELECT attribute, incident "
					     "FROM attributes_to_incidents "
					     "WHERE attribute = :attribute "
					     "AND incident = :incident "
					     "AND coder = :coder");
			      query->bindValue(":attribute", parentName);
			      query->bindValue(":incident", incidentNode);
			      query->bindValue(":coder", _selectedCoder);
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
	  boldSelected(model, name, incidentNode, type, index);
	}
    }
}

void EventGraphWidget::assignAttribute() 
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
			 "attribute = :att AND incident = :incident AND coder = :coder");
	  query->bindValue(":att", attribute);
	  query->bindValue(":incident", _selectedIncident);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  query->first();
	  empty = query->isNull(0);
	  QTextCursor cursPos = rawField->textCursor();
	  if (empty) 
	    {
	      query->prepare("INSERT INTO attributes_to_incidents (attribute, incident, coder) "
			     "VALUES (:attribute, :incident, :coder)");
	      query->bindValue(":attribute", attribute);
	      query->bindValue(":incident", _selectedIncident);
	      query->bindValue(":coder", _selectedCoder);
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
	      boldSelected(attributesTree, attribute, _selectedAbstractNode->getId(), ABSTRACTNODE);
	      valueField->setEnabled(true);
	    }
	}
      setButtons();
    }
  rawField->verticalScrollBar()->setValue(barPos);
}

void EventGraphWidget::unassignAttribute() 
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
			 "WHERE attribute = :att AND incident = :incident AND coder = :coder");
	  query->bindValue(":att", attribute);
	  query->bindValue(":incident", _selectedIncident);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  query->first();
	  empty = query->isNull(0);
	  if (!empty) 
	    {
	      query->prepare("DELETE FROM attributes_to_incidents "
			     "WHERE attribute = :att AND incident = :incident AND coder = :coder");
	      query->bindValue(":att", attribute);
	      query->bindValue(":incident", _selectedIncident);
	      query->bindValue(":coder", _selectedCoder);
	      query->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents_sources "
			      "WHERE attribute = :att AND incident = :incident AND coder = :coder");
	      query2->bindValue(":att", attribute);
	      query2->bindValue(":inc", _selectedIncident);
	      query2->bindValue(":coder", _selectedCoder);
	      query2->exec();
	      resetFont(attributesTree);
	      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			      "WHERE incident = :incident AND coder = :coder");
	      query2->bindValue(":incident", _selectedIncident);
	      query2->bindValue(":coder", _selectedCoder);
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
	      QSet<QString>::iterator it;
	      resetFont(attributesTree);
	      attributes = _selectedAbstractNode->getAttributes();
	      for (it = attributes.begin(); it != attributes.end(); it++) 
		{
		  QString current = *it;
		  boldSelected(attributesTree, current, _selectedAbstractNode->getId(), ABSTRACTNODE);	  
		}
	      setButtons();
	      valueField->setText("");
	      valueField->setEnabled(false);
	      valueButton->setEnabled(false);
	    }
	}
    }
  setButtons();
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
		     "WHERE attribute = :att AND inc = :incident AND coder = :coder "
		     "AND source_text = :text");
      query->bindValue(":att", attribute);
      query->bindValue(":inc", incident);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":text", sourceText);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO attributes_to_incidents_sources "
			 "(attribute, incident, coder, source_text)"
			 "VALUES (:att, :inc, :coder, :text)");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", incident);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":text", sourceText);
	  query->exec();
	}
      delete query;
    }
}

void EventGraphWidget::removeText() 
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
			     "WHERE attribute = :att AND incident = :inc AND coder = :coder "
			     "AND source_text = :text");
	      query->bindValue(":att", attribute);
	      query->bindValue(":inc", _selectedIncident);
	      query->bindValue(":coder", _selectedCoder);
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
      if (_selectedIncident != 0) 
	{
	  QPointer<QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->setWindowTitle("Resetting texts");
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
				 "WHERE attribute = :att AND incident = :inc AND coder = :coder");
		  query->bindValue(":att", attribute);
		  query->bindValue(":inc", _selectedIncident);
		  query->bindValue(":coder", _selectedCoder);
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
      if (_selectedIncident != 0) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  bool empty = false;
	  query->prepare("SELECT attribute, incident, coder, value FROM "
			 "attributes_to_incidents "
			 "WHERE attribute = :att AND incident = :inc AND coder = :coder ");
	  query->bindValue(":att", currentAttribute);
	  query->bindValue(":inc", _selectedIncident);
	  query->bindValue(":coder", _selectedCoder);
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
			 "WHERE attribute = :att AND incident = :inc AND coder = :coder");
	  query->bindValue(":att", currentAttribute);
	  query->bindValue(":inc", _selectedIncident);
	  query->bindValue(":coder", _selectedCoder);
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
  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
  while (it.hasNext()) 
    {
      AbstractNode *current = it.next();
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

void EventGraphWidget::highlightText() 
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
			     "WHERE attribute = :attribute AND incident = :id AND coder = :coder");
	      query->bindValue(":attribute", currentName);
	      query->bindValue(":id", _selectedIncident);
	      query->bindValue(":coder", _selectedCoder);
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
  if (_selectedIncident != 0) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute FROM attributes_to_incidents "
		     "WHERE incident = :id AND coder = :coder");
      query->bindValue(":id", _selectedIncident);
      query->bindValue(":coder", _selectedCoder);
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
  _relationshipsWidgetPtr->resetTree();
  _attributesWidgetPtr->resetTree();
}

void EventGraphWidget::getIncidents() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT ch_order, description FROM incidents ORDER BY ch_order");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT id FROM incidents WHERE ch_order = :order");
  while (query->next()) 
    {
      int order = query->value(0).toInt();
      query2->bindValue(":order", order);
      query2->exec();
      query2->first();
      int id = query2->value(0).toInt();
      QString toolTip = breakString(query->value(1).toString());
      qreal vertical = qrand() % 500 - 250;
      QPointF position = QPointF((order * 70), vertical);
      IncidentNode *currentItem = new IncidentNode(40, toolTip, position, id, order);
      currentItem->setPos(currentItem->getOriginalPos());
      _incidentNodeVector.push_back(currentItem);
    }
  delete query;
  delete query2;
}

void EventGraphWidget::plotIncidents() 
{
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *currentItem = it.next();
      currentItem->setZValue(3);
      scene->addItem(currentItem);
    }
}

void EventGraphWidget::getEdges(QString coder, QString type, QColor color) 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE coder = :coder AND type = :type");
  query2->prepare("SELECT comment, coder FROM linkage_comments "
		  "WHERE tail = :tail AND head = :head AND type = :type");
  query->bindValue(":coder", coder);
  query->bindValue(":type", type);
  query->exec();
  while (query->next()) 
    {
      int tail = query->value(0).toInt();
      int head = query->value(1).toInt();
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
      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
      IncidentNode *tempSource = NULL;
      IncidentNode *tempTarget = NULL;
      while (it.hasNext()) 
	{
	  IncidentNode *currentItem = it.next();
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
		  Linkage *currentEdge = new Linkage(type, coder);
		  currentEdge->setZValue(_incidentNodeVector[0]->zValue() - 1);
		  currentEdge->setStartItem(tempSource);
		  currentEdge->setEndItem(tempTarget);
		  currentEdge->setToolTip(toolTip);
		  currentEdge->setColor(color);
		  _edgeVector.push_back(currentEdge);
		  break;
		}
	      else if (tempSource->getOrder() > tempTarget->getOrder()) 
		{
		  Linkage *currentEdge = new Linkage(type, coder);
		  currentEdge->setZValue(_incidentNodeVector[0]->zValue() - 1);
		  currentEdge->setStartItem(tempSource);
		  currentEdge->setEndItem(tempTarget);
		  currentEdge->setToolTip(toolTip);
		  currentEdge->setColor(color);
		  _edgeVector.push_back(currentEdge);
		  break;
		}
	    } 
	}
    }
  delete query;
  delete query2;
}

void EventGraphWidget::plotEdges(QString type) 
{
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *currentEdge = it.next();
      if (currentEdge->getType() == type) 
	{
	  scene->addItem(currentEdge);
	}
    }
}

void EventGraphWidget::makeLayout()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (layoutComboBox->currentText() == REDOLAYOUT)
    {
      redoLayout();
    }
  else if (layoutComboBox->currentText() == MINIMISELAYOUT)
    {
      minimiseCurrentGraph();
    }
  else if (layoutComboBox->currentText() == DATELAYOUTDAYS)
    {
      memorizeLayout();
      dateLayout(35);
      correctLayout();
    }
  else if (layoutComboBox->currentText() == DATELAYOUTMONTHS)
    {
      memorizeLayout();
      dateLayout(20);
      correctLayout();
    }
  else if (layoutComboBox->currentText() == DATELAYOUTYEARS)
    {
      memorizeLayout();
      dateLayout(5);
      correctLayout();
    }
  else if (layoutComboBox->currentText() == DATELAYOUTDECADES)
    {
      memorizeLayout();
      dateLayout(1);
      correctLayout();
    }
  else if (layoutComboBox->currentText() == NOOVERLAP)
    {
      noOverlap();
    }
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void EventGraphWidget::layoutGraph() 
{
  QVector<IncidentNode*>::iterator it;
  for (it = _incidentNodeVector.begin(); it != _incidentNodeVector.end(); it++) 
    {
      IncidentNode *current = *it;
      QVector<IncidentNode*>::iterator it2;
      QVector<IncidentNode*> partners;
      for (it2 = it + 1; it2 != _incidentNodeVector.end(); it2++) 
	{
	  IncidentNode *second = *it2;
	  QVectorIterator<Linkage *> it3(_edgeVector);
	  while (it3.hasNext()) 
	    {
	      Linkage *edge = it3.next();
	      // Basically we just ask whether the current events are linked.
	      if ((edge->getStart() == current &&
		   edge->getEnd() == second) ||
		  (edge->getEnd() == current &&
		   edge->getStart() == second)) 
		{
		  partners.push_back(second);
		}
	    }
	}
      qreal originHeight = current->scenePos().y();
      std::sort(partners.begin(), partners.end(), eventLessThan);
      int partnerCount = partners.size();
      QVectorIterator<IncidentNode*> it4(partners);
      while (it4.hasNext()) 
	{
	  IncidentNode *currentPartner = it4.next();
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

void EventGraphWidget::redoLayout()
{
  QVector<QGraphicsItem*> allEvents;
  QVector<IncidentNode*> allIncidents;
  QVectorIterator<IncidentNode*> it(_incidentNodeVector);
  while (it.hasNext())
    {
      IncidentNode *incident = it.next();
      allEvents.push_back(incident);
      allIncidents.push_back(incident);
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext())
    {
      AbstractNode *abstract = it2.next();
      allEvents.push_back(abstract);
      QVector<IncidentNode*> incidents = abstract->getIncidents();
      QVectorIterator<IncidentNode*> it3(incidents);
      while (it3.hasNext())
	{
	  allIncidents.push_back(it3.next());
	}
    }
  QVectorIterator<IncidentNode*> it4(allIncidents);
  while (it4.hasNext())
    {
      IncidentNode *incident = it4.next();
      int order = incident->getOrder();
      qreal vertical = qrand() % 3000 - 1500;
      QPointF position = QPointF((order * 70), vertical);
      incident->setPos(position);
      incident->setOriginalPos(position);
    }
  std::sort(allEvents.begin(), allEvents.end(), componentsSort);
  it2.toFront();
  while (it2.hasNext())
    {
      AbstractNode *abstract = it2.next();
      QVector<IncidentNode*> incidents = abstract->getIncidents();
      std::sort(incidents.begin(), incidents.end(), componentsSort);
      IncidentNode *first = incidents.first();
      IncidentNode *last = incidents.last();
      int oldWidth = abstract->getWidth();
      int newWidth = last->scenePos().x() - first->scenePos().x() + last->getWidth();
      abstract->setPos(first->scenePos().x(), first->scenePos().y());
      int diff = (newWidth - oldWidth) / 2;
      abstract->setWidth(newWidth);
      abstract->getLabel()->setNewPos(abstract->scenePos(), diff);
    }
  QVector<QGraphicsItem*>::iterator it5;
  for (it5 = allEvents.begin(); it5 != allEvents.end(); it5++) 
    {
      QGraphicsItem *current = *it5;
      QVector<QGraphicsItem*>::iterator it6;
      QVector<QGraphicsItem*> partners;
      for (it6 = it5 + 1; it6 != allEvents.end(); it6++) 
	{
	  QGraphicsItem *second = *it6;
	  QVectorIterator<Linkage *> it7(_edgeVector);
	  while (it7.hasNext()) 
	    {
	      Linkage *edge = it7.next();
	      // Basically we just ask whether the current events are linked.
	      if ((edge->getStart() == current &&
		   edge->getEnd() == second) ||
		  (edge->getEnd() == current &&
		   edge->getStart() == second)) 
		{
		  partners.push_back(second);
		}
	    }
	}
      qreal originHeight = current->scenePos().y();
      std::sort(partners.begin(), partners.end(), eventLessThan);
      int partnerCount = partners.size();
      QVectorIterator<QGraphicsItem*> it8(partners);
      while (it8.hasNext()) 
	{
	  QGraphicsItem *currentPartner = it8.next();
	  qreal partnerHeight = originHeight;
	  if (partners.size() > 1) 
	    {
	      partnerHeight = originHeight + (pow(-1, partnerCount) * partnerCount * 50);
	    }
	  currentPartner->setPos(currentPartner->scenePos().x(), partnerHeight);
	  partnerCount--;
	}
    }
  QVectorIterator<IncidentNodeLabel*> it9(_incidentNodeLabelVector);
  while (it9.hasNext())
    {
      IncidentNodeLabel *currentLabel = it9.next();
      currentLabel->setNewPos(currentLabel->getNode()->scenePos());
    }
  QVectorIterator<AbstractNodeLabel*> it10(_abstractNodeLabelVector);
  while (it10.hasNext())
    {
      AbstractNodeLabel *currentLabel = it10.next();
      currentLabel->setNewPos(currentLabel->getAbstractNode()->scenePos());
    }
  updateLinkages();
}

void EventGraphWidget::dateLayout(int scale) 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->prepare("SELECT timestamp FROM incidents "
		 "WHERE id = :id");
  query2->prepare("UPDATE incidents SET mark = 1 "
		  "WHERE id = :id");
  QVector<IncidentNode*> visible;
  QVector<QGraphicsItem*> allEvents;
  QVectorIterator<IncidentNode*> it(_incidentNodeVector);
  while (it.hasNext())
    {
      IncidentNode *incident = it.next();
      if (incident->isVisible())
	{
	  visible.push_back(incident);
	}
      allEvents.push_back(incident);
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext())
    {
      AbstractNode *abstract = it2.next();
      if (abstract->isVisible())
	{
	  QVector<IncidentNode*> incidents = abstract->getIncidents();
	  QVectorIterator<IncidentNode*> it3(incidents);
	  while (it3.hasNext())
	    {
	      visible.push_back(it3.next());
	    }
	}
      allEvents.push_back(abstract);
    }
  std::sort(visible.begin(), visible.end(), componentsSort);
  IncidentNode *first = visible.first();
  query->bindValue(":id", first->getId());
  query->exec();
  query->first();
  QString firstDateString = query->value(0).toString();
  QDate firstDate;
  if (firstDateString.length() == 4) // We are dealing with a year only.
    {
      firstDate = QDate::fromString(firstDateString, "yyyy");
    }
  if (firstDateString.length() == 7) // We are dealing with a month and year.
    {
      if (firstDateString[2] == '/')
	{
	  firstDate = QDate::fromString(firstDateString, "MM/yyyy");
	}
      else if (firstDateString[2] == '-')
	{
	  firstDate = QDate::fromString(firstDateString, "MM-yyyy");
	}
      else if (firstDateString[4] == '\\') 
	{
	  firstDate = QDate::fromString(firstDateString, "yyyy\\MM");
	}
      else if (firstDateString[4] == '-')
	{
	  firstDate = QDate::fromString(firstDateString, "yyyy-MM");
	}
    }
  if (firstDateString.length() == 10) // We are dealing with a day, month and year.
    {
      if (firstDateString[2] == '/')
	{
	  firstDate = QDate::fromString(firstDateString, "dd/MM/yyyy");
	}
      else if (firstDateString[2] == '-')
	{
	  firstDate = QDate::fromString(firstDateString, "dd-MM-yyyy");
	}
      else if (firstDateString[4] == '\\') 
	{
	  firstDate = QDate::fromString(firstDateString, "yyyy\\MM\\dd");
	}
      else if (firstDateString[4] == '-')
	{
	  firstDate = QDate::fromString(firstDateString, "yyyy-MM-dd");
	}
    }
  if (!firstDate.isValid())
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Checking dates");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>No valid starting date</b>");
      warningBox->setInformativeText("The date of the first visible incident must be a valid date "
				     "for this function to work.");
      warningBox->exec();
      delete warningBox;
      delete query;
      delete query2;
      return;
    }
  else
    {
      int total = visible.size();
      int validTotal = 0;
      QMap<IncidentNode*, qint64> days;
      QMap<IncidentNode*, int> precision;
      QMap<IncidentNode*, QDate> dates;
      QVectorIterator<IncidentNode*> it4(visible);
      while (it4.hasNext())
	{
	  IncidentNode *incident = it4.next();
	  int id = incident->getId();
	  query->bindValue(":id", id);
	  query->exec();
	  query->first();
	  QString dateString = query->value(0).toString();
	  QDate date;
	  if (dateString.length() == 4) // We are dealing with a year only.
	    {
	      date = QDate::fromString(dateString, "yyyy");
	    }
	  if (dateString.length() == 7) // We are dealing with a month and year.
	    {
	      if (dateString[2] == '/')
		{
		  date = QDate::fromString(dateString, "MM/yyyy");
		}
	      else if (dateString[2] == '-')
		{
		  date = QDate::fromString(dateString, "MM-yyyy");
		}
	      else if (dateString[4] == '\\') 
		{
		  date = QDate::fromString(dateString, "yyyy\\MM");
		}
	      else if (dateString[4] == '-')
		{
		  date = QDate::fromString(dateString, "yyyy-MM");
		}
	    }
	  if (dateString.length() == 10) // We are dealing with a day, month and year.
	    {
	      if (dateString[2] == '/')
		{
		  date = QDate::fromString(dateString, "dd/MM/yyyy");
		}
	      else if (dateString[2] == '-')
		{
		  date = QDate::fromString(dateString, "dd-MM-yyyy");
		}
	      else if (dateString[4] == '\\') 
		{
		  date = QDate::fromString(dateString, "yyyy\\MM\\dd");
		}
	      else if (dateString[4] == '-')
		{
		  date = QDate::fromString(dateString, "yyyy-MM-dd");
		}
	    }
	  if (date.isValid())
	    {
	      days.insert(incident, firstDate.daysTo(date));
	      validTotal++;
	      dates.insert(incident, date);
	      if (dateString.length() == 4)
		{
		  precision.insert(incident, 1);
		}
	      else if (dateString.length() == 7)
		{
		  precision.insert(incident, 2);
		}
	      else if (dateString.length() == 10)
		{
		  precision.insert(incident, 3);
		}
	    }
	}
      qreal lastValid = first->scenePos().x();
      int lastPrecision = precision.value(first);
      QDate lastDate = dates.value(first);
      qreal validPerc = (qreal) validTotal / (qreal) total * 100;
      validPerc = std::roundf(validPerc * 100) / 100;
      QApplication::restoreOverrideCursor();
      qApp->processEvents();
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Checking dates");
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Dates found:</h2>");
      warningBox->setInformativeText(QString::number(validPerc) + "% of the currently visible "
				     "nodes have a valid date set in the 'timing' field. "
				     "Do you wish to continue?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
	  bool warn = true;
	  QApplication::setOverrideCursor(Qt::WaitCursor);
	  delete warningBox;
	  it4.toFront();
	  it4.next(); // skip the first one
	  while (it4.hasNext())
	    {
	      IncidentNode *incident = it4.next();
	      if (days.contains(incident))
		{
		  qint64 daysTo = days.value(incident);
		  qreal x = scale * daysTo + first->scenePos().x();
		  QDate currentDate = dates.value(incident);
		  bool precisionDifference = false;
		  if (x >= lastValid)
		    {
		      incident->setPos(x, incident->scenePos().y());
		      incident->getLabel()->setNewPos(incident->scenePos());
		      lastValid = x;
		      lastPrecision = precision.value(incident);
		      lastDate = dates.value(incident);
		    }
		  else if (precision.value(incident) <= lastPrecision)
		    {
		      if (precision.value(incident) == 2 &&
			  currentDate.month() == lastDate.month())
			{
			  precisionDifference = true;
			}
		      else if (precision.value(incident) == 1 &&
			       currentDate.year() == lastDate.year())
			{
			  precisionDifference = true;
			}
		      if (!precisionDifference)
			{
			  if (warn)
			    {
			      QApplication::restoreOverrideCursor();
			      qApp->processEvents();
			      QPointer <QMessageBox> warningBox = new QMessageBox(this);
			      warningBox->setWindowTitle("Checking dates");
			      warningBox->addButton(QMessageBox::Ok);
			      QPointer<QAbstractButton> markButton = warningBox->
				addButton(tr("Mark"), QMessageBox::NoRole);
			      QPointer<QAbstractButton> skipButton = warningBox->
				addButton(tr("Skip remaining warnings"), QMessageBox::NoRole);
			      warningBox->setIcon(QMessageBox::Warning);
			      warningBox->setText("<b>Possible problem detected</b>");
			      warningBox->setInformativeText("Incident " +
							     QString::number(incident->getOrder()) +
							     " is incorrectly positioned in the "
							     "chronological order and may cause "
							     "problems for the layout.");
			      warningBox->exec();
			      if (warningBox->clickedButton() == markButton)
				{
				  query2->bindValue(":id", incident->getId());
				  query2->exec();
				}
			      else if (warningBox->clickedButton() == skipButton)
				{
				  warn = false;
				}
			      delete warningBox;
			      QApplication::setOverrideCursor(Qt::WaitCursor);
			    }
			}
		      else
			{
			  QVectorIterator<IncidentNode*> it5 = it4;
			  bool resolved = false;
			  bool foundValid = false;
			  while (!foundValid)
			    {
			      while (it5.hasNext())
				{
				  IncidentNode *next = it5.next();
				  if (days.contains(next))
				    {
				      qint64 daysToNext = days.value(next);
				      qreal xNext = scale * daysToNext + first->scenePos().x();
				      if (xNext >= lastValid)
					{
					  qreal tempX = (lastValid + xNext) / 2;
					  incident->setPos(tempX, incident->scenePos().y());
					  incident->getLabel()->setNewPos(incident->scenePos());
					  foundValid = true;
					  resolved = true;
					  break;
					}
				    }
				}
			      foundValid = true;
			    }
			  if (!resolved)
			    {
			      incident->setPos(lastValid + 70, incident->scenePos().y());
			      incident->getLabel()->setNewPos(incident->scenePos());
			    }
			}
		    }
		}
	      else
		{
		  QVectorIterator<IncidentNode*> it5 = it4;
		  bool foundValid = false;
		  bool resolved = false;
		  while (!foundValid)
		    {
		      while (it5.hasNext())
			{
			  IncidentNode *next = it5.next();
			  if (days.contains(next))
			    {
			      qint64 daysToNext = days.value(next);
			      qreal xNext = scale * daysToNext + first->scenePos().x();
			      if (xNext >= lastValid)
				{
				  qreal tempX = (lastValid + xNext) / 2;
				  incident->setPos(tempX, incident->scenePos().y());
				  incident->getLabel()->setNewPos(incident->scenePos());
				  foundValid = true;
				  resolved = true;
				  break;
				}
			    }
			}
		      foundValid = true;
		    }
		  if (!resolved)
		    {
		      incident->setPos(lastValid + 70, incident->scenePos().y());
		      incident->getLabel()->setNewPos(incident->scenePos());
		    }
		}
	    }
	  it2.toFront();
	  while (it2.hasNext())
	    {
	      AbstractNode *abstract = it2.next();
	      QVector<IncidentNode*> incidents = abstract->getIncidents();
	      std::sort(incidents.begin(), incidents.end(), componentsSort);
	      IncidentNode *first = incidents.first();
	      IncidentNode *last = incidents.last();
	      int oldWidth = abstract->getWidth();
	      int newWidth = last->scenePos().x() - first->scenePos().x() + last->getWidth();
	      abstract->setPos(first->scenePos().x(), first->scenePos().y());
	      int diff = (newWidth - oldWidth) / 2;
	      abstract->setWidth(newWidth);
	      abstract->getLabel()->setNewPos(abstract->scenePos(), diff);
	    }
	  // Let's make sure that we have no overlap.
	  noOverlap();
	}
      else
	{
	  delete warningBox;
	  delete query;
	  delete query2;
	  return;
	}
    }
  updateLinkages();
  delete query;
  delete query2;
}

void EventGraphWidget::memorizeLayout() 
{
  std::sort(_incidentNodeVector.begin(), _incidentNodeVector.end(), eventLessThan);
  QVectorIterator<IncidentNode*> it(_incidentNodeVector);
  while (it.hasNext())
    {
      IncidentNode *incident = it.next();
      int order = incident->getOrder();
      if (order > upperRangeDial->value())
	{
	  if (it.findPrevious(incident))
	    {
	      IncidentNode *previous = it.previous();
	      if (previous->getAbstractNode() == NULL)
		{
		  QPair<QGraphicsItem*, QPointF> currentPair(previous, previous->scenePos());
		  _layoutMemory = currentPair;
		  break;
		}
	      else
		{
		  AbstractNode *abstract = previous->getAbstractNode();
		  QPair<QGraphicsItem*, QPointF> currentPair(abstract, abstract->scenePos());
		  _layoutMemory = currentPair;
		  break;
		}
	    }
	}
    }
}

void EventGraphWidget::correctLayout() 
{
  std::sort(_incidentNodeVector.begin(), _incidentNodeVector.end(), eventLessThan);
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *incident = it.next();
      int order = incident->getOrder();
      if (order > upperRangeDial->value())
	{
	  if(incident->getAbstractNode() == NULL) 
	    {
	      QGraphicsItem* preceding = _layoutMemory.first;
	      QPointF oldPos = _layoutMemory.second;
	      qreal distance = preceding->scenePos().x() - oldPos.x();
	      incident->setPos(incident->scenePos().x() + distance, incident->scenePos().y());
	      incident->getLabel()->setNewPos(incident->scenePos());
	    }
	  else
	    {
	      AbstractNode *abstract = incident->getAbstractNode();
	      QGraphicsItem* preceding = _layoutMemory.first;
	      QPointF oldPos = _layoutMemory.second;
	      qreal distance = preceding->scenePos().x() - oldPos.x();
	      abstract->setPos(abstract->scenePos().x() + distance, abstract->scenePos().y());
	      abstract->getLabel()->setNewPos(abstract->scenePos());
	    }
	}
    }
}

void EventGraphWidget::noOverlap()
{
  QVector<QGraphicsItem*> allEvents;
  QVectorIterator<IncidentNode*> it(_incidentNodeVector);
  while (it.hasNext())
    {
      allEvents.push_back(it.next());
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext())
    {
      allEvents.push_back(it2.next());
    }
  std::sort(allEvents.begin(), allEvents.end(), componentsSort);
  bool overlap = true;
  while (overlap)
    {
      overlap = false;
      QVectorIterator<QGraphicsItem*> it3(allEvents);
      while (it3.hasNext())
	{
	  QGraphicsItem *first = it3.next();
	  QVectorIterator<QGraphicsItem*> it4(allEvents);
	  while (it4.hasNext())
	    {
	      QGraphicsItem *second = it4.next();
	      if (first != second)
		{
		  qreal dist = qSqrt(qPow(first->scenePos().x() - second->scenePos().x(), 2) + 
				     qPow(first->scenePos().y() - second->scenePos().y(), 2));
		  if (dist < 70)
		    {
		      overlap = true;
		      if (first->scenePos().y() > second->scenePos().y())
			{
			  first->setPos(first->scenePos().x(), first->scenePos().y() + 30);
			  second->setPos(second->scenePos().x(), second->scenePos().y() - 30);
			  IncidentNode *incidentFirst = qgraphicsitem_cast<IncidentNode*>(first);
			  AbstractNode *abstractFirst = qgraphicsitem_cast<AbstractNode*>(first);
			  IncidentNode *incidentSecond = qgraphicsitem_cast<IncidentNode*>(second);
			  AbstractNode *abstractSecond = qgraphicsitem_cast<AbstractNode*>(second);
			  if (incidentFirst)
			    {
			      incidentFirst->getLabel()->setNewPos(incidentFirst->scenePos());
			    }
			  else if (abstractFirst)
			    {
			      abstractFirst->getLabel()->setNewPos(abstractFirst->scenePos());
			    }
			  if (incidentSecond)
			    {
			      incidentSecond->getLabel()->setNewPos(incidentSecond->scenePos());
			    }
			  else if (abstractSecond)
			    {
			      abstractSecond->getLabel()->setNewPos(abstractSecond->scenePos());
			    }
			}
		      else
			{
			  first->setPos(first->scenePos().x(), first->scenePos().y() - 30);
			  second->setPos(second->scenePos().x(), second->scenePos().y() + 30);
			  IncidentNode *incidentFirst = qgraphicsitem_cast<IncidentNode*>(first);
			  AbstractNode *abstractFirst = qgraphicsitem_cast<AbstractNode*>(first);
			  IncidentNode *incidentSecond = qgraphicsitem_cast<IncidentNode*>(second);
			  AbstractNode *abstractSecond = qgraphicsitem_cast<AbstractNode*>(second);
			  if (incidentFirst)
			    {
			      incidentFirst->getLabel()->setNewPos(incidentFirst->scenePos());
			    }
			  else if (abstractFirst)
			    {
			      abstractFirst->getLabel()->setNewPos(abstractFirst->scenePos());
			    }
			  if (incidentSecond)
			    {
			      incidentSecond->getLabel()->setNewPos(incidentSecond->scenePos());
			    }
			  else if (abstractSecond)
			    {
			      abstractSecond->getLabel()->setNewPos(abstractSecond->scenePos());
			    }
			}
		    }
		}
	    }
	}
    }
}

void EventGraphWidget::getLabels() 
{
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *currentItem = it.next();
      int id = currentItem->getId();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT ch_order, description FROM incidents "
		     "WHERE id = :id");
      query->bindValue(":id", id);
      query->exec();
      query->first();
      QString order = query->value(0).toString();
      QPointer<IncidentNodeLabel> text = new IncidentNodeLabel(currentItem);
      currentItem->setLabel(text);
      text->setPlainText(order);
      text->setNewPos(currentItem->scenePos());    
      text->setZValue(_incidentNodeVector[0]->zValue() + 1);
      text->setDefaultTextColor(Qt::black);
      _incidentNodeLabelVector.push_back(text);
      delete query;
    }
}

void EventGraphWidget::addLabels() 
{
  QVectorIterator<IncidentNodeLabel*> it2(_incidentNodeLabelVector);
  while (it2.hasNext()) 
    {
      IncidentNodeLabel *currentItem = it2.next();
      scene->addItem(currentItem);
    }
}

void EventGraphWidget::cleanUp() 
{
  scene->clearSelection();
  _currentData.clear(); // Contents will be deleted below
  qDeleteAll(_edgeVector);
  _edgeVector.clear();
  qDeleteAll(_abstractNodeVector);
  _abstractNodeVector.clear();
  qDeleteAll(_abstractNodeLabelVector);
  _abstractNodeLabelVector.clear();
  qDeleteAll(_incidentNodeVector);
  _incidentNodeVector.clear();
  qDeleteAll(_incidentNodeLabelVector);
  _incidentNodeLabelVector.clear();
  qDeleteAll(_compareVector);
  _compareVector.clear();
  qDeleteAll(_lineVector);
  _lineVector.clear();
  qDeleteAll(_textVector);
  _textVector.clear();
  qDeleteAll(_ellipseVector);
  _ellipseVector.clear();
  qDeleteAll(_rectVector);
  _rectVector.clear();
  qDeleteAll(_timeLineVector);
  _timeLineVector.clear();
  qDeleteAll(_guidesVector);
  _guidesVector.clear();
  _contractedMap.clear();
  scene->clear();
  eventListWidget->setRowCount(0);
  linkageListWidget->setRowCount(0);
  caseListWidget->clear();
  _presentTypes.clear();
  _checkedCases.clear();
  _selectedCompare = "";
  _selectedIncident = 0;
  _selectedAbstractNode = NULL;
  _contracted = false;
  snapGuidesButton->setChecked(false);
  toggleSnapGuides();
  _labelsVisible = true;
  _labelSize = 10;
  setGraphControls(false);
  disableLegendButtons();
  plotLabel->setText("");
}

void EventGraphWidget::increaseDistance() 
{
  memorizeLayout();
  QVector<QGraphicsItem*> temp;
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *incident = it.next();
      int order = incident->getOrder();
      if (order >= lowerRangeDial->value() && order <= upperRangeDial->value())
	{
	  if (incident->getAbstractNode() == NULL) 
	    {
	      temp.push_back(incident);
	    }
	  else
	    {
	      temp.push_back(incident->getAbstractNode());
	    }
	}
    }
  std::sort(temp.begin(), temp.end(), eventLessThan);
  QGraphicsItem *first = temp.first();
  QVectorIterator<QGraphicsItem*> it3(temp);  
  while (it3.hasNext()) 
    {
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(it3.peekNext());
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(it3.peekNext());
      if (incidentNode) 
	{
	  incidentNode = qgraphicsitem_cast<IncidentNode *>(it3.next());
	  if (incidentNode != first)
	    {
	      qreal distance = incidentNode->scenePos().x() - first->scenePos().x();
	      qreal newDistance = distance * 1.1;
	      incidentNode->setPos(QPointF(first->scenePos().x() +
					   newDistance, incidentNode->scenePos().y()));
	      incidentNode->setPos(QPointF(first->scenePos().x() +
					   newDistance, incidentNode->scenePos().y()));
	      incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
	    }
	}
      else if (abstractNode) 
	{
	  abstractNode = qgraphicsitem_cast<AbstractNode*>(it3.next());
	  if (abstractNode != first)
	    {
	      qreal distance = abstractNode->scenePos().x() - first->scenePos().x();
	      qreal newDistance = distance * 1.1;
	      abstractNode->setPos(QPointF(first->scenePos().x() +
					   newDistance, abstractNode->scenePos().y()));
	      int oldWidth = abstractNode->getWidth();
	      int newWidth = oldWidth * 0.9;
	      int diff = (newWidth - oldWidth) / 2;
	      abstractNode->setWidth(newWidth);
	      abstractNode->getLabel()->setNewPos(abstractNode->scenePos(), diff);
	    }
	}
    }
  correctLayout();
  updateLinkages();
}
  
void EventGraphWidget::decreaseDistance() 
{
  memorizeLayout();
  setChangeLabel();
  QVector<QGraphicsItem*> temp;
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *incident = it.next();
      int order = incident->getOrder();
      if (order >= lowerRangeDial->value() && order <= upperRangeDial->value())
	{
	  if (incident->getAbstractNode() == NULL) 
	    {
	      temp.push_back(incident);
	    }
	  else
	    {
	      temp.push_back(incident->getAbstractNode());
	    }
	}
    }
  std::sort(temp.begin(), temp.end(), eventLessThan);
  QGraphicsItem *first = temp.first();
  QVectorIterator<QGraphicsItem*> it3(temp);  
  while (it3.hasNext()) 
    {
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(it3.peekNext());
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(it3.peekNext());
      if (incidentNode) 
	{
	  incidentNode = qgraphicsitem_cast<IncidentNode *>(it3.next());
	  if (incidentNode != first)
	    {
	      qreal distance = incidentNode->scenePos().x() - first->scenePos().x();
	      qreal newDistance = distance * 0.9;
	      incidentNode->setPos(QPointF(first->scenePos().x() +
					   newDistance, incidentNode->scenePos().y()));
	      incidentNode->setPos(QPointF(first->scenePos().x() +
					   newDistance, incidentNode->scenePos().y()));
	      incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
	    }
	}
      else if (abstractNode) 
	{
	  abstractNode = qgraphicsitem_cast<AbstractNode*>(it3.next());
	  if (abstractNode != first)
	    {
	      qreal distance = abstractNode->scenePos().x() - first->scenePos().x();
	      qreal newDistance = distance * 0.9;
	      abstractNode->setPos(QPointF(first->scenePos().x() +
					   newDistance, abstractNode->scenePos().y()));
	      int oldWidth = abstractNode->getWidth();
	      int newWidth = oldWidth * 0.9;
	      int diff = (newWidth - oldWidth) / 2;
	      abstractNode->setWidth(newWidth);
	      abstractNode->getLabel()->setNewPos(abstractNode->scenePos(), diff);
	    }
	}
    }
  correctLayout();
  updateLinkages();
}
	
void EventGraphWidget::expandGraph() 
{
  setChangeLabel();
  qreal virtualCenter = 0.0;
  int total = 0;
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  virtualCenter /= total;
  it.toFront();
  int counter = 0;
  while (it.hasNext()) 
    {
      counter++;
      IncidentNode *current = it.next();
      if (current->getAbstractNode() == NULL) 
	{
	  if (counter >= lowerRangeDial->value() && counter <= upperRangeDial->value()) 
	    {
	      qreal currentY = current->scenePos().y();
	      qreal diffY  = (currentY - virtualCenter) * 1.1;
	      current->setPos(current->scenePos().x(), virtualCenter + diffY);
	      current->getLabel()->setNewPos(current->scenePos());
	    }
	}
    }
  it2.toFront();
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      QVector<IncidentNode*> incidents = current->getIncidents();
      QVectorIterator<IncidentNode*> it3(incidents);
      bool valid = false;
      while (it3.hasNext())
	{
	  IncidentNode *incident = it3.next();
	  int order = incident->getOrder();
	  if (order >= lowerRangeDial->value() && order <= upperRangeDial->value())
	    {
	      valid = true;
	    }
	}
      if (valid)
	{
	  qreal currentY = current->scenePos().y();
	  qreal diffY  = (currentY - virtualCenter) * 1.1;
	  current->setPos(current->scenePos().x(), virtualCenter + diffY);
	  current->getLabel()->setNewPos(current->scenePos());
	}
    }
  updateLinkages();
}

void EventGraphWidget::contractGraph() 
{
  setChangeLabel();
  qreal virtualCenter = 0.0;
  int total = 0;
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      virtualCenter += current->scenePos().y();
      total = total + 1.0;
    }
  virtualCenter /= total;
  it.toFront();
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      qreal currentY = current->scenePos().y();
      qreal diffY  = (currentY - virtualCenter) * 0.9;
      current->setPos(current->scenePos().x(), virtualCenter + diffY);
      current->getLabel()->setNewPos(current->scenePos());
    }
  it2.toFront();
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      qreal currentY = current->scenePos().y();
      qreal diffY  = (currentY - virtualCenter) * 0.9;
      current->setPos(current->scenePos().x(), virtualCenter + diffY);
      current->getLabel()->setNewPos(current->scenePos());
    }
  updateLinkages();
}

void EventGraphWidget::minimiseCurrentGraph()
{
  _contracted = true;
  QVector<QGraphicsItem*> allEvents;
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *incident = it.next();
      if (incident->isVisible())
	{
	  allEvents.push_back(incident);
	}
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *abstract = it2.next();
      if (abstract->isVisible())
	{
	  allEvents.push_back(abstract);
	}
    }
  std::sort(allEvents.begin(), allEvents.end(), eventLessThanWidth);
  QVectorIterator<QGraphicsItem*> it3(allEvents);
  // Let us store the current positions
  while (it3.hasNext())
    {
      QGraphicsItem *current = it3.next();
      _contractedMap.insert(current, current->scenePos());
    }
  it3.toFront();
  // We want to skip the first one
  it3.next();
  while (it3.hasNext())
    {
      QGraphicsItem *current = it3.next();
      QGraphicsItem *target = NULL;
      QVectorIterator<QGraphicsItem*> it4(allEvents);
      while (it4.hasNext()) 
	{
	  QGraphicsItem *item = it4.next();
	  IncidentNode *itemIncidentNode = qgraphicsitem_cast<IncidentNode *>(item);
	  AbstractNode *itemAbstractNode = qgraphicsitem_cast<AbstractNode*>(item);
	  if (itemIncidentNode) 
	    {
	      if (itemIncidentNode->isVisible() &&
		  itemIncidentNode->scenePos().x() +
		  itemIncidentNode->getWidth() < current->scenePos().x()) 
		{
		  target = item;
		}
	    }
	  else if (itemAbstractNode) 
	    {
	      if (itemAbstractNode->isVisible() &&
		  itemAbstractNode->scenePos().x() +
		  itemAbstractNode->getWidth() < current->scenePos().x()) 
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
	  IncidentNode *targetIncidentNode = qgraphicsitem_cast<IncidentNode *>(target);
	  AbstractNode *targetAbstractNode = qgraphicsitem_cast<AbstractNode*>(target);
	  int width = 0;
	  if (targetIncidentNode) 
	    {
	      width = targetIncidentNode->getWidth();
	    }
	  else if (targetAbstractNode) 
	    {
	      width = targetAbstractNode->getWidth();
	    }
	  if (current->scenePos().x() - target->scenePos().x() - width + 40 > 70) 
	    {
	      qreal oldX = current->scenePos().x();
	      current->setPos(target->scenePos().x() +
			      70 + width - 40, current->scenePos().y());
	      qreal newX = current->scenePos().x();
	      qreal dist = oldX - newX;
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	      if (incidentNode) 
		{
		  incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
		}
	      else if (abstractNode) 
		{
		  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
		}
	      QVectorIterator<QGraphicsItem*> it5(allEvents);
	      while (it5.hasNext()) 
		{
		  QGraphicsItem *follow = it5.next();
		  if (follow->scenePos().x() > current->scenePos().x()) 
		    {
		      IncidentNode *followIncidentNode = qgraphicsitem_cast<IncidentNode *>(follow);
		      AbstractNode *followAbstractNode = qgraphicsitem_cast<AbstractNode*>(follow);
		      if (followIncidentNode) 
			{
			  followIncidentNode->setPos(followIncidentNode->scenePos().x() -
						     dist, followIncidentNode->scenePos().y());
			  followIncidentNode->getLabel()->setNewPos(followIncidentNode->scenePos());
			}
		      else if (followAbstractNode) 
			{
			  followAbstractNode->setPos(followAbstractNode->scenePos().x() -
						     dist, followAbstractNode->scenePos().y());
			  followAbstractNode->getLabel()->setNewPos(followAbstractNode->scenePos());
			}
		    }
		}
	    }
	}
    }
  updateLinkages();
}

void EventGraphWidget::restorePositions()
{
  QMapIterator<QGraphicsItem*, QPointF> it(_contractedMap);
  while (it.hasNext())
    {
      it.next();
      QGraphicsItem *current = it.key();
      QPointF oldPos = it.value();
      IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(current);
      AbstractNode *abstract = qgraphicsitem_cast<AbstractNode*>(current);
      if (incident)
	{
	  incident->setPos(oldPos);
	  incident->getLabel()->setNewPos(incident->scenePos());
	}
      else if (abstract)
	{
	  abstract->setPos(oldPos);
	  abstract->getLabel()->setNewPos(abstract->scenePos());
	}
    }
  _contractedMap.clear();
  _contracted = false;
}

void EventGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) 
{
  if (_currentData.size() > 0) 
    {
      QGraphicsItem *source = NULL;
      QVectorIterator<QGraphicsItem*> it(_currentData);
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
	  QVectorIterator<QGraphicsItem*> it2(_currentData);
	  while (it2.hasNext()) 
	    {
	      QGraphicsItem *current = it2.next();
	      current->setPos(current->scenePos().x(), current->scenePos().y() + yDiff);
	      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(current);
	      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	      if (currentIncidentNode) 
		{
		  currentIncidentNode->getLabel()->setNewPos(currentIncidentNode->scenePos());
		}
	      else if (currentAbstractNode) 
		{
		  currentAbstractNode->getLabel()->setNewPos(currentAbstractNode->scenePos());
		}
	    }
	}
    }
}

void EventGraphWidget::setPlotButtons() 
{
  if (typeComboBox->currentText() != DEFAULT) 
    {
      plotButton->setEnabled(true);
    }
  else 
    {
      plotButton->setEnabled(false);
    }
  if (!_presentTypes.contains(typeComboBox->currentText())) 
    {
      if (_abstractNodeVector.size() > 0) 
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
  if (_presentTypes.contains(typeComboBox->currentText())) 
    {
      compareComboBox->clear();
      compareComboBox->addItem(DEFAULT);
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT name FROM coders");
      while (query->next()) 
	{
	  QString currentCoder = query->value(0).toString();
	  if (currentCoder != _selectedCoder)
	    {
	      compareComboBox->addItem(currentCoder);
	    }
	}
      delete query;
    }
}

void EventGraphWidget::getLinkageDetails() 
{
  typeComboBox->clear();
  typeComboBox->addItem(DEFAULT);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM linkage_types ORDER BY name ASC");
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
  _presentTypes.push_back(currentType);
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
  if (_presentTypes.contains(typeComboBox->currentText())) 
    {
      compareComboBox->clear();
      compareComboBox->addItem(DEFAULT);
      query->exec("SELECT name FROM coders");
      while (query->next()) 
	{
	  QString currentCoder = query->value(0).toString();
	  if (currentCoder != _selectedCoder) 
	    {
	      compareComboBox->addItem(currentCoder);
	    }
	}
    }
  delete query;
  getIncidents();
  plotIncidents(); // Should allow for range to be set here.
  getEdges(_selectedCoder, currentType, color);
  plotEdges(currentType);
  layoutGraph();
  getLabels();
  addLabels();
  compareComboBox->setCurrentIndex(0);
  savePlotButton->setEnabled(true);
  updateCases();
  caseListWidget->setEnabled(true);
  addLinkageTypeButton->setEnabled(false);
  setRangeControls();
  resetRange();
  plotLabel->setText("Unsaved plot");
  checkCongruence();
  updateLinkages();
  setGraphControls(true);
  hideAnnotationsButton->setChecked(false);
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
  _presentTypes.push_back(currentType);
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
  if (_presentTypes.contains(typeComboBox->currentText())) 
    {
      compareComboBox->clear();
      compareComboBox->addItem(DEFAULT);
      query->exec("SELECT name FROM coders");
      while (query->next()) 
	{
	  QString currentCoder = query->value(0).toString();
	  if (currentCoder != _selectedCoder)
	    {
	      compareComboBox->addItem(currentCoder);
	    }
	}
    }
  delete query;
  getEdges(_selectedCoder, currentType, color);
  plotEdges(currentType);
  setHeights();
  checkCongruence();
  redoLayout();
  setVisibility();
  addLinkageTypeButton->setEnabled(false);
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void EventGraphWidget::setCompareButton() 
{
  if (_presentTypes.size() > 0) 
    {
      if (compareComboBox->currentText() != DEFAULT &&
	  compareComboBox->currentText() != _selectedCoder) 
	{
	  if (_abstractNodeVector.size() > 0) 
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
  _selectedCompare = compareComboBox->currentText();
  getCompareEdges(_selectedCompare, _presentTypes[0]);
  plotCompareEdges();
  updateLinkages();
  compareButton->setEnabled(false);
}

void EventGraphWidget::getCompareEdges(QString coder, QString type) 
{
  qDeleteAll(_compareVector);
  _compareVector.clear();
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
      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
      IncidentNode *tempSource = NULL;
      IncidentNode *tempTarget = NULL;
      while (it.hasNext()) 
	{
	  IncidentNode *currentItem = it.next();
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
	      QVectorIterator<Linkage*> it2(_edgeVector);
	      QColor edgeColor = QColor(Qt::gray);
	      while (it2.hasNext()) 
		{
		  Linkage *currentEdge = it2.next();
		  edgeColor = currentEdge->getColor();
		  IncidentNode *tempS = qgraphicsitem_cast<IncidentNode *>(currentEdge->getStart());
		  IncidentNode *tempT = qgraphicsitem_cast<IncidentNode *>(currentEdge->getEnd());
		  if (tempSource == tempS && tempTarget == tempT) 
		    {
		      currentEdge->setPenStyle(2);
		      sameFound = true;
		    }
		}
	      if (sameFound == false) 
		{
		  Linkage *compareEdge = new Linkage(type, coder);
		  compareEdge->setZValue(_incidentNodeVector[0]->zValue() - 1);
		  compareEdge->setStartItem(tempSource);
		  compareEdge->setEndItem(tempTarget);
		  compareEdge->setPenStyle(4);
		  compareEdge->setColor(edgeColor);
		  _compareVector.push_back(compareEdge);
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
  QVectorIterator<Linkage*> it(_compareVector);
  while (it.hasNext()) 
    {
      Linkage *currentEdge = it.next();
      scene->addItem(currentEdge);
    }
  QVectorIterator<Linkage*> it2(_edgeVector);
  while (it2.hasNext()) 
    {
      Linkage *currentEdge = it2.next();
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
      query->prepare("SELECT plot FROM saved_eg_plots "
		     "WHERE plot = :name AND coder = :coder");
      query->bindValue(":name", name);
      query->bindValue(":coder", _selectedCoder);
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
	  warningBox->setWindowTitle("Saving plot");
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
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":name", name);
	  query->exec();
	  // saved_eg_plots_settings
	  query->prepare("DELETE FROM saved_eg_plots_settings "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_incident_nodes
	  query->prepare("DELETE FROM saved_eg_plots_incident_nodes "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_edges
	  query->prepare("DELETE FROM saved_eg_plots_edges "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_incident_node_labels
	  query->prepare("DELETE FROM saved_eg_plots_incident_node_labels "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_abstract_nodes
	  query->prepare("DELETE FROM saved_eg_plots_abstract_nodes "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_incidents_to_abstract_nodes
	  query->prepare("DELETE FROM saved_eg_plots_incidents_to_abstract_nodes "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_attributes_to_abstract_nodes
	  query->prepare("DELETE FROM saved_eg_plots_attributes_to_abstract_nodes "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_abstract_node_labels
	  query->prepare("DELETE FROM saved_eg_plots_abstract_node_labels "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_legend
	  query->prepare("DELETE FROM saved_eg_plots_legend "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_embedded_incidents
	  query->prepare("DELETE FROM saved_eg_plots_embedded_incidents "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_abstract_nodes_to_abstract_nodes 
	  query->prepare("DELETE FROM saved_eg_plots_abstract_nodes_to_abstract_nodes "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_lines
	  query->prepare("DELETE FROM saved_eg_plots_lines "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_timelines
	  query->prepare("DELETE FROM saved_eg_plots_timelines "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_texts
	  query->prepare("DELETE FROM saved_eg_plots_texts "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_ellipses
	  query->prepare("DELETE FROM saved_eg_plots_ellipses "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_rects
	  query->prepare("DELETE FROM saved_eg_plots_rects "
			 "WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_contraction
	  query->prepare("DELETE FROM saved_eg_plots_contraction "
			"WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_cases
	  query->prepare("DELETE FROM saved_eg_plots_cases "
			"WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  // saved_eg_plots_guides
	  query->prepare("DELETE FROM saved_eg_plots_guides "
			"WHERE plot = :plot AND coder = :coder");
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
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
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->exec();
	}
      QSqlDatabase::database().transaction();
      query->prepare("INSERT INTO saved_eg_plots_settings "
		     "(plot, coder, lowerbound, upperbound, labelson, labelsize) "
		     "VALUES (:plot, :coder, :lowerbound, :upperbound, :labelson, :labelsize)");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":lowerbound", lowerRangeDial->value());
      query->bindValue(":upperbound", upperRangeDial->value());
      int labelson = 0;
      if (_labelsVisible)
	{
	  labelson = 1;
	}
      query->bindValue(":labelson", labelson);
      query->bindValue(":labelsize", _labelSize);
      query->exec();      
      query->prepare("INSERT INTO saved_eg_plots_incident_nodes "
		     "(plot, coder, incident, ch_order, width, curxpos, curypos, orixpos, oriypos, "
		     "dislodged, mode, red, green, blue, alpha, zvalue, hidden, masshidden) "
		     "VALUES (:plot, :coder, :incident, :order, :width, :curxpos, :curypos, :orixpos, "
		     ":oriypos, :dislodged, :mode, :red, :green, :blue, :alpha, :zvalue, :hidden, "
		     ":masshidden)");
      QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, _incidentNodeVector.size());
      saveProgress->setWindowTitle("Saving incident nodes");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      int counter = 1;
      saveProgress->show();
      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
      while (it.hasNext()) 
	{
	  IncidentNode *currentItem = it.next();
	  qreal currentX = currentItem->scenePos().x();
	  qreal currentY = currentItem->scenePos().y();
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
	  int zvalue = currentItem->zValue();
	  int hidden = 1;
	  int masshidden = 0;
	  if (currentItem->isDislodged()) 
	    {
	      dislodged = 1;
	    }
	  if (currentItem->isVisible()) 
	    {
	      hidden = 0;
	    }
	  if (currentItem->isMassHidden())
	    {
	      masshidden = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
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
	  query->bindValue(":zvalue", zvalue);
	  query->bindValue(":hidden", hidden);
	  query->bindValue(":masshidden", masshidden);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _incidentNodeLabelVector.size());
      saveProgress->setWindowTitle("Saving node labels");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_incident_node_labels "
		     "(plot, coder, incident, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, zvalue, hidden) "
		     "VALUES (:plot, :coder, :incident, :label, :curxpos, :curypos, "
		     ":xoffset, :yoffset, :red, :green, :blue, :alpha, :zvalue, :hidden)");
      QVectorIterator<IncidentNodeLabel*> it2(_incidentNodeLabelVector);
      while (it2.hasNext()) 
	{
	  IncidentNodeLabel *currentLabel = it2.next();
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
	  int zvalue = currentLabel->zValue();
	  int hidden = 1;
	  if (currentLabel->isVisible()) 
	    {
	      hidden = 0;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
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
	  query->bindValue(":zvalue", zvalue);
	  query->bindValue(":hidden", hidden);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _edgeVector.size());
      saveProgress->setWindowTitle("Saving edge items");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_edges "
		     "(plot, coder, tail, head, tailabstractnode, headabstractnode, linkage, "
		     "red, green, blue, alpha, zvalue, hidden, masshidden) "
		     "VALUES (:plot, :coder, :tail, :head, :tabstractnode, :habstractnode, :linkage, "
		     ":red, :green, :blue, :alpha, :zvalue, :hidden, :masshidden)");
      QVectorIterator<Linkage*> it3(_edgeVector);
      while (it3.hasNext()) 
	{
	  Linkage *currentEdge = it3.next();
	  QColor color = currentEdge->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  IncidentNode *incidentNodeTail = qgraphicsitem_cast<IncidentNode *>(currentEdge->getStart());
	  IncidentNode *incidentNodeHead = qgraphicsitem_cast<IncidentNode *>(currentEdge->getEnd());
	  AbstractNode *abstractNodeTail = qgraphicsitem_cast<AbstractNode*>(currentEdge->getStart());
	  AbstractNode *abstractNodeHead = qgraphicsitem_cast<AbstractNode*>(currentEdge->getEnd());
	  QString linkage = currentEdge->getType();
	  int tail = 0;
	  int head = 0;
	  int mTail = 0;
	  int mHead = 0;
	  int zvalue = currentEdge->zValue();
	  int hidden = 1;
	  int massHidden = 0;
	  if (incidentNodeTail) 
	    {
	      tail = incidentNodeTail->getId();
	    }
	  else if (abstractNodeTail) 
	    {
	      tail = abstractNodeTail->getId();
	      mTail = 1;
	    }
	  if (incidentNodeHead) 
	    {
	      head = incidentNodeHead->getId();
	    }
	  else if (abstractNodeHead) 
	    {
	      head = abstractNodeHead->getId();
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
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", head);
	  query->bindValue(":tabstractnode", mTail);
	  query->bindValue(":habstractnode", mHead);
	  query->bindValue(":linkage", linkage);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":alpha", alpha);
	  query->bindValue(":zvalue", zvalue);
	  query->bindValue(":hidden", hidden);
	  query->bindValue(":masshidden", massHidden);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _abstractNodeVector.size());
      saveProgress->setWindowTitle("Saving Abstract Nodes");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_incidents_to_abstract_nodes "
		     "(plot, coder, incident, abstractnode) "
		     "VALUES (:plot, :coder, :incident, :abstractnode)");
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("INSERT INTO saved_eg_plots_attributes_to_abstract_nodes "
		      "(plot, coder, attribute, abstractnode, value) "
		      "VALUES(:plot, :coder, :attribute, :abstractnode, :value)");
      QSqlQuery *query3 = new QSqlQuery;
      query3->prepare("INSERT INTO saved_eg_plots_abstract_nodes "
		      "(plot, coder, eventid, ch_order, abstraction, timing, description, "
		      "comment, width, "
		      "mode, curxpos, curypos, orixpos, oriypos, dislodged, "
		      "red, green, blue, alpha, zvalue, hidden, masshidden) "
		      "VALUES (:plot, :coder, :eventid, :ch_order, :abstraction, :timing, "
		      ":description, "
		      ":comment, :width, :mode, :curxpos, :curypos, :orixpos, :oriypos, :dislodged, "
		      ":red, :green, :blue, :alpha, :zvalue, :hidden, :masshidden)");;
      QVectorIterator<AbstractNode*> it4(_abstractNodeVector);
      while (it4.hasNext()) 
	{
	  AbstractNode *currentAbstractNode = it4.next();
	  QVector<IncidentNode *> incidents = currentAbstractNode->getIncidents();
	  QVectorIterator<IncidentNode *> tit(incidents);
	  while (tit.hasNext()) 
	    {
	      IncidentNode *currentIncident = tit.next();
	      query->bindValue(":plot", name);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":incident", currentIncident->getId());
	      query->bindValue(":abstractnode", currentAbstractNode->getId());
	      query->exec();
	    }
	  QSet<QString> attributes = currentAbstractNode->getAttributes();
	  QMap<QString, QString> values = currentAbstractNode->getValues();
	  QSet<QString>::iterator tit2;
	  for (tit2 = attributes.begin(); tit2 != attributes.end(); tit2++) 
	    {
	      QString attribute = *tit2;
	      QString value = values.value(attribute);
	      query2->bindValue(":plot", name);
	      query2->bindValue(":coder", _selectedCoder);
	      query2->bindValue(":attribute", attribute);
	      query2->bindValue(":abstractnode", currentAbstractNode->getId());
	      query2->bindValue(":value", value);
	      query2->exec();
	    }
	  QString timing = currentAbstractNode->getTiming();
	  QString description = currentAbstractNode->getDescription();
	  QString comment = currentAbstractNode->getComment();
	  int width = currentAbstractNode->getWidth();
	  QString mode = currentAbstractNode->getMode();
	  qreal currentX = currentAbstractNode->pos().x();
	  qreal currentY = currentAbstractNode->pos().y();
	  qreal originalX = currentAbstractNode->getOriginalPos().x();
	  qreal originalY = currentAbstractNode->getOriginalPos().y();
	  int dislodged = 0;
	  QColor color = currentAbstractNode->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  int zvalue = currentAbstractNode->zValue();
	  int hidden = 1;
	  int masshidden = 0;
	  if (currentAbstractNode->isDislodged()) 
	    {
	      dislodged = 1;
	    }
	  if (currentAbstractNode->isVisible()) 
	    {
	      hidden = 0;
	    }
	  if (currentAbstractNode->isMassHidden())
	    {
	      masshidden = 1;
	    }
	  query3->bindValue(":plot", name);
	  query3->bindValue(":coder", _selectedCoder);
	  query3->bindValue(":eventid", currentAbstractNode->getId());
	  query3->bindValue(":ch_order", currentAbstractNode->getOrder());
	  query3->bindValue(":abstraction", currentAbstractNode->getConstraint());
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
	  query3->bindValue(":zvalue", zvalue);
	  query3->bindValue(":hidden", hidden);
	  query3->bindValue(":masshidden", masshidden);
	  query3->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      delete query2;
      delete query3;
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _incidentNodeVector.size());
      saveProgress->setWindowTitle("Saving embedded incidents");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_embedded_incidents "
		     "(plot, coder, incident, abstractnode) "
		     "VALUES (:plot, :coder, :incident, :abstractnode)");
      QVectorIterator<IncidentNode *> it5(_incidentNodeVector);
      while (it5.hasNext()) 
	{
	  IncidentNode *currentIncidentNode = it5.next();
	  if (currentIncidentNode->getAbstractNode() != NULL) 
	    {
	      query->bindValue(":plot", name);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":incident", currentIncidentNode->getId());
	      query->bindValue(":abstractnode", currentIncidentNode->getAbstractNode()->getId());
	      query->exec();
	      counter++;
	      saveProgress->setProgress(counter);
	      qApp->processEvents();
	    }
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _abstractNodeVector.size());
      saveProgress->setWindowTitle("Saving embedded Abstract Nodes");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_abstract_nodes_to_abstract_nodes "
		     "(plot, coder, son, father) "
		     "VALUES (:plot, :coder, :son, :father)");
      QVectorIterator<AbstractNode*> it6(_abstractNodeVector);
      while (it6.hasNext()) 
	{
	  AbstractNode *currentAbstractNode = it6.next();
	  AbstractNode *currentFather = currentAbstractNode->getAbstractNode(); 
	  if (currentAbstractNode->getAbstractNode() != NULL) 
	    {
	      query->bindValue(":plot", name);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":son", currentAbstractNode->getId());
	      query->bindValue(":father", currentFather->getId());
	      query->exec();
	    }
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;    
      saveProgress = new ProgressBar(0, 1, _abstractNodeLabelVector.size());
      saveProgress->setWindowTitle("Saving Abstract Node Labels");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_abstract_node_labels "
		     "(plot, coder, eventid, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, zvalue, hidden) "
		     "VALUES (:plot, :coder, :eventid, :label, :curxpos, :curypos, "
		     ":xoffset, :yoffset, "
		     ":red, :green, :blue, :alpha, :zvalue, :hidden)");
      QVectorIterator<AbstractNodeLabel*> it7(_abstractNodeLabelVector);
      while (it7.hasNext()) 
	{
	  AbstractNodeLabel *currentLabel = it7.next();
	  int id = currentLabel->getAbstractNode()->getId();
	  QString text = currentLabel->toPlainText();
	  qreal currentX = currentLabel->scenePos().x();
	  qreal currentY = currentLabel->scenePos().y();
	  qreal xOffset = currentLabel->getOffset().x();
	  qreal yOffset = currentLabel->getOffset().y();
	  int red = currentLabel->defaultTextColor().red();
	  int green = currentLabel->defaultTextColor().green();
	  int blue = currentLabel->defaultTextColor().blue();
	  int alpha = currentLabel->defaultTextColor().alpha();
	  int zvalue = currentLabel->zValue();
	  int hidden = 1;
	  if (currentLabel->isVisible()) 
	    {
	      hidden = 0;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
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
	  query->bindValue(":zvalue", zvalue);
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
      query->prepare("INSERT INTO saved_eg_plots_legend (plot, coder, name, tip, "
		     "red, green, blue, alpha, textred, textgreen, textblue, textalpha, hidden) "
		     "VALUES (:plot, :coder, :name, :tip, :red, :green, :blue, :alpha, "
		     ":textred, :textgreen, :textblue, :textalpha, :hidden)");
      for (int i = 0; i != eventListWidget->rowCount(); i++) 
	{
	  QTableWidgetItem *item = eventListWidget->item(i, 0);
	  QString title = item->data(Qt::DisplayRole).toString();
	  QString tip = item->data(Qt::ToolTipRole).toString();
	  QColor color = eventListWidget->item(i, 1)->background().color();
	  QVariant textColorVar = eventListWidget->item(i, 1)->data(Qt::UserRole);
	  QColor textColor = QColor::fromRgb(textColorVar.toUInt());
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  int textred = textColor.red();
	  int textgreen = textColor.green();
	  int textblue = textColor.blue();
	  int textalpha = textColor.alpha();
	  int hidden = 0;
	  if (eventListWidget->item(i,0)->background() == QColor(Qt::gray))
	    {
	      hidden = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":name", title);
	  query->bindValue(":tip", tip);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":alpha", alpha);
	  query->bindValue(":textred", textred);
	  query->bindValue(":textgreen", textgreen);
	  query->bindValue(":textblue", textblue);
	  query->bindValue(":textalpha", textalpha);
	  query->bindValue(":hidden", hidden);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _lineVector.size());
      saveProgress->setWindowTitle("Saving line objects");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_lines "
		     "(plot, coder, startx, starty, endx, endy, arone, artwo, penwidth, penstyle, "
		     "zvalue, red, green, blue, alpha) "
		     "VALUES (:plot, :coder, :startx, :starty, :endx, :endy, :arone, :artwo, "
		     ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha)");
      QVectorIterator<LineObject*> it8(_lineVector);
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
	  query->bindValue(":coder", _selectedCoder);
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
      saveProgress = new ProgressBar(0, 1, _timeLineVector.size());
      saveProgress->setWindowTitle("Saving timeline objects");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_timelines "
		     "(plot, coder, startx, endx, y, penwidth, majorinterval, minordivision, "
		     "majorsize, minorsize, firsttick, lasttick, zvalue, red, green, blue, alpha) "
		     "VALUES (:plot, :coder, :startx, :endx, :y, :penwidth, :majorinterval, "
		     ":minordivision, "
		     ":majorsize, :minorsize, :firsttick, :lasttick, :zvalue, "
		     ":red, :green, :blue, :alpha)");
      QVectorIterator<TimeLineObject*> it9(_timeLineVector);
      while (it9.hasNext()) 
	{
	  TimeLineObject *currentTimeLine = it9.next();
	  qreal startx = currentTimeLine->getStartX();
	  qreal endx = currentTimeLine->getEndX();
	  qreal y = currentTimeLine->getY();
	  int penwidth = currentTimeLine->getPenWidth();
	  qreal majorinterval = currentTimeLine->getMajorTickInterval();
	  qreal minordivision = currentTimeLine->getMinorTickDivision();
	  qreal majorsize = currentTimeLine->getMajorTickSize();
	  qreal minorsize = currentTimeLine->getMinorTickSize();
	  int firsttick = 0;
	  if (currentTimeLine->getFirstTick())
	    {
	      firsttick = 1;
	    }
	  int lasttick = 0;
	  if (currentTimeLine->getForceLastTick())
	    {
	      lasttick = 1;
	    }
	  int zValue = currentTimeLine->zValue();
	  QColor color = currentTimeLine->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":startx", startx);
	  query->bindValue(":endx", endx);
	  query->bindValue(":y", y);
	  query->bindValue(":penwidth", penwidth);
	  query->bindValue(":majorinterval", majorinterval);
	  query->bindValue(":minordivision", minordivision);
	  query->bindValue(":majorsize", majorsize);
	  query->bindValue(":minorsize", minorsize);
	  query->bindValue(":firsttick", firsttick);
	  query->bindValue(":lasttick", lasttick);
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
      saveProgress = new ProgressBar(0, 1, _textVector.size());
      saveProgress->setWindowTitle("Saving text items");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_texts "
		     "(plot, coder, desc, xpos, ypos, width, size, rotation, zvalue, "
		     "red, green, blue, alpha) "
		     "VALUES (:plot, :coder, :desc, :xpos, :ypos, :width, :size, :rotation, "
		     ":zvalue, :red, :green, :blue, :alpha)");
      QVectorIterator<TextObject*> it10(_textVector);
      while (it10.hasNext()) 
	{
	  TextObject *currentText = it10.next();
	  QString desc = currentText->toPlainText();
	  qreal xpos = currentText->scenePos().x();
	  qreal ypos = currentText->scenePos().y();
	  qreal width = currentText->textWidth();
	  int size = currentText->font().pointSize();
	  qreal rotation = currentText->getRotationValue();
	  int zValue = currentText->zValue();
	  QColor color = currentText->defaultTextColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
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
      saveProgress = new ProgressBar(0, 1, _ellipseVector.size());
      saveProgress->setWindowTitle("Saving ellipses");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_ellipses "
		     "(plot, coder, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha) "
		     "VALUES (:plot, :coder, :xpos, :ypos, :topleftx, :toplefty, :toprightx, "
		     ":toprighty, "
		     ":bottomleftx, :bottomlefty, :bottomrightx, :bottomrighty, :rotation, "
		     ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha, "
		     ":fillred, :fillgreen, :fillblue, :fillalpha)");
      QVectorIterator<EllipseObject*> it11(_ellipseVector);
      while (it11.hasNext()) 
	{
	  EllipseObject *ellipse = it11.next();
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
	  query->bindValue(":coder", _selectedCoder);
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
      saveProgress = new ProgressBar(0, 1, _rectVector.size());
      saveProgress->setWindowTitle("Saving rectangles");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_rects "
		     "(plot, coder, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha) "
		     "VALUES (:plot, :coder, :xpos, :ypos, :topleftx, :toplefty, :toprightx, "
		     ":toprighty, "
		     ":bottomleftx, :bottomlefty, :bottomrightx, :bottomrighty, :rotation, "
		     ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha, "
		     ":fillred, :fillgreen, :fillblue, :fillalpha)");
      QVectorIterator<RectObject*> it12(_rectVector);
      while (it12.hasNext()) 
	{
	  RectObject *rect = it12.next();
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
	  query->bindValue(":coder", _selectedCoder);
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
      saveProgress = new ProgressBar(0, 1, _contractedMap.size());
      saveProgress->setWindowTitle("Saving contracted item positions");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_contraction "
		     "(plot, coder, nodeid, abstract, xpos, ypos) "
		     "VALUES (:plot, :coder, :nodeid, :abstract, :xpos, :ypos)");
      QMapIterator<QGraphicsItem*, QPointF> it13(_contractedMap);
      while (it13.hasNext())
	{
	  it13.next();
	  QGraphicsItem *current = it13.key();
	  QPointF position = it13.value();
	  int id = -1;
	  int abstract = 0;
	  IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode*>(current);
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	  if (incidentNode)
	    {
	      id  = incidentNode->getId();
	    }
	  else if (abstractNode)
	    {
	      id = abstractNode->getId();
	      abstract = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":nodeid", id);
	  query->bindValue(":abstract", abstract);
	  query->bindValue(":xpos", position.x());
	  query->bindValue(":ypos", position.y());
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, caseListWidget->count());
      saveProgress->setWindowTitle("Saving cases");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_cases "
		     "(plot, coder, casename, checked) "
		     "VALUES (:plot, :coder, :casename, :checked)");
      for (int i = 0; i != caseListWidget->count(); i++) 
	{
	  QListWidgetItem *item = caseListWidget->item(i);
	  QString casename = item->text();
	  int checked = 0;
	  if (item->checkState() == Qt::Checked)
	    {
	      checked = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":casename", casename);
	  query->bindValue(":checked", checked);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _guidesVector.size());
      saveProgress->setWindowTitle("Saving guides");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_eg_plots_guides "
		     "(plot, coder, xpos, ypos, horizontal) "
		     "VALUES (:plot, :coder, :xpos, :ypos, :horizontal)");
      QVectorIterator<GuideLine*> it14(_guidesVector);
      while (it14.hasNext())
	{
	  GuideLine *guide = it14.next();
	  qreal xpos = guide->getOrientationPoint().x();
	  qreal ypos = guide->getOrientationPoint().y();
	  int horizontal = 0;
	  if (guide->isHorizontal())
	    {
	      horizontal = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":xpos", xpos);
	  query->bindValue(":ypos", ypos);
	  query->bindValue(":horizontal", horizontal);
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
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this, EVENTGRAPH, _selectedCoder);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) 
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);
      savePlotButton->setEnabled(true);
      hideAnnotationsButton->setChecked(false);
      cleanUp();
      scene->clear();
      QString plot = savedPlotsDialog->getSelectedPlot();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT coder, red, green, blue "
		     "FROM saved_eg_plots "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      query->first();
      QString coder = query->value(0).toString();
      int red = query->value(1).toInt();
      int green = query->value(2).toInt();
      int blue = query->value(3).toInt();
      _selectedCoder = coder;
      scene->setBackgroundBrush(QBrush(QColor(red, green, blue)));
      query->prepare("SELECT lowerbound, upperbound, labelson, labelsize "
		     "FROM saved_eg_plots_settings "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      query->first();
      int lowerbound = query->value(0).toInt();
      int upperbound = query->value(1).toInt();
      int labelson = query->value(2).toInt();
      int labelsize = query->value(3).toInt();
      if (labelson == 1)
	{
	  _labelsVisible = true;
	}
      else
	{
	  _labelsVisible = false;
	}
      _labelSize = labelsize;
      query->prepare("SELECT incident, ch_order, width, curxpos, curypos, orixpos, oriypos, "
		     "dislodged, mode, red, green, blue, alpha, zvalue, hidden, masshidden "
		     "FROM saved_eg_plots_incident_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
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
	  int zvalue = query->value(13).toInt();
	  int hidden = query->value(14).toInt();
	  int masshidden = query->value(15).toInt();
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
	  IncidentNode *currentItem = new IncidentNode(width, toolTip, originalPos, id, order);
	  currentItem->setPos(currentPos);
	  currentItem->setColor(QColor(red, green, blue, alpha));
	  currentItem->setZValue(zvalue);
	  currentItem->setMode(mode);
	  _incidentNodeVector.push_back(currentItem);
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
	  if (masshidden == 1)
	    {
	      currentItem->setMassHidden(true);
	    }
	}
      query->prepare("SELECT incident, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, zvalue, hidden "
		     "FROM saved_eg_plots_incident_node_labels "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
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
	  int zvalue = query->value(10).toInt();
	  int hidden = query->value(11).toInt();
	  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
	  while (it.hasNext()) 
	    {
	      IncidentNode *currentItem = it.next();
	      int nodeId = currentItem->getId();
	      if (nodeId == id) 
		{
		  IncidentNodeLabel *currentLabel = new IncidentNodeLabel(currentItem);
		  currentLabel->setPlainText(text);
		  currentLabel->setPos(QPointF(currentX, currentY));
		  currentLabel->setOffset(QPointF(xOffset, yOffset));
		  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
		  currentLabel->setZValue(zvalue);
		  currentLabel->setFontSize(_labelSize);
		  currentItem->setLabel(currentLabel);
		  _incidentNodeLabelVector.push_back(currentLabel);
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
      query->prepare("SELECT eventid, ch_order, abstraction, timing, description, comment, width, "
		     "mode, curxpos, curypos, orixpos, oriypos, dislodged, red, green, blue, alpha, "
		     "zvalue, hidden, masshidden "
		     "FROM saved_eg_plots_abstract_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
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
	  int zvalue = query->value(17).toInt();
	  int hidden = query->value(18).toInt();
	  int masshidden = query->value(19).toInt();
	  QPointF currentPos = QPointF(currentX, currentY);
	  QPointF originalPos = QPointF(originalX, originalY);
	  QColor color = QColor(red, green, blue, alpha);
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT incident FROM saved_eg_plots_incidents_to_abstract_nodes "
			  "WHERE plot = :plot AND coder = :coder AND abstractnode = :id");
	  query2->bindValue(":plot", plot);
	  query2->bindValue(":coder", _selectedCoder);
	  query2->bindValue(":id", id);
	  query2->exec();
	  QVector<IncidentNode *> incidents;
	  while (query2->next()) 
	    {
	      int incidentId = query2->value(0).toInt();
	      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
	      while (it.hasNext()) 
		{
		  IncidentNode *currentIncidentNode = it.next();
		  if (incidentId == currentIncidentNode->getId()) 
		    {
		      incidents.push_back(currentIncidentNode);
		      break;
		    }
		}
	    }
	  std::sort(incidents.begin(), incidents.end(), eventLessThan);
	  query2->prepare("SELECT attribute, value FROM saved_eg_plots_attributes_to_abstract_nodes "
			  "WHERE plot = :plot AND coder = :coder AND abstractnode = :id");
	  query2->bindValue(":plot", plot);
	  query->bindValue(":coder", _selectedCoder);
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
	  AbstractNode *newAbstractNode = new AbstractNode(width, description, originalPos, id,
							   constraint, incidents);
	  QVectorIterator<IncidentNode *> it(incidents);
	  while (it.hasNext()) 
	    {
	      IncidentNode *currentIncidentNode = it.next();
	      currentIncidentNode->hide();
	    }
	  newAbstractNode->setTiming(timing);
	  newAbstractNode->setOriginalPos(originalPos);
	  newAbstractNode->setPos(currentPos);
	  newAbstractNode->setZValue(zvalue);
	  newAbstractNode->setColor(color);
	  newAbstractNode->setAttributes(attributes);
	  newAbstractNode->setValues(values);
	  newAbstractNode->setOrder(order);
	  newAbstractNode->setMode(mode);
	  if (hidden == 1) 
	    {
	      newAbstractNode->hide();
	    }
	  if (dislodged == 1) 
	    {
	      newAbstractNode->setDislodged(true);
	    }
	  if (masshidden == 1)
	    {
	      newAbstractNode->setMassHidden(true);
	    }
	  scene->addItem(newAbstractNode);
	  _abstractNodeVector.push_back(newAbstractNode);
	}
      query->prepare("SELECT incident, abstractnode "
		     "FROM saved_eg_plots_embedded_incidents "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  int incidentId = query->value(0).toInt();
	  int abstractNodeId = query->value(1).toInt();
	  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
	  while (it.hasNext()) 
	    {
	      IncidentNode *incidentNode = it.next();
	      if (incidentNode->getId() == incidentId) 
		{
		  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
		  while (it2.hasNext()) 
		    {
		      AbstractNode *abstractNode = it2.next();
		      if (abstractNode->getId() == abstractNodeId) 
			{
			  incidentNode->hide();
			  incidentNode->setAbstractNode(abstractNode);
			}
		    }
		}
	    }
	}
      query->prepare("SELECT son, father "
		     "FROM saved_eg_plots_abstract_nodes_to_abstract_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  int son = query->value(0).toInt();
	  int father = query->value(1).toInt();
	  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
	  while (it.hasNext()) 
	    {
	      AbstractNode *sonIncidentNode = it.next();
	      if (sonIncidentNode->getId() == son) 
		{
		  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
		  while (it2.hasNext()) 
		    {
		      AbstractNode *fatherIncidentNode = it2.next();
		      if (fatherIncidentNode->getId() == father) 
			{
			  sonIncidentNode->hide();
			  sonIncidentNode->setAbstractNode(fatherIncidentNode);
			}
		    }
		}
	    }
	}
      query->prepare("SELECT eventid, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, zvalue, hidden "
		     "FROM saved_eg_plots_abstract_node_labels "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  int abstractNode = query->value(0).toInt();
	  QString text = query->value(1).toString();
	  qreal currentX = query->value(2).toReal();
	  qreal currentY = query->value(3).toReal();
	  qreal xOffset = query->value(4).toReal();
	  qreal yOffset = query->value(5).toReal();
	  int red = query->value(6).toInt();
	  int green = query->value(7).toInt();
	  int blue = query->value(8).toInt();
	  int alpha = query->value(9).toInt();
	  int zvalue = query->value(10).toInt();
	  int hidden = query->value(11).toInt();
	  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
	  while (it.hasNext()) 
	    {
	      AbstractNode *currentAbstractNode = it.next();
	      int abstractNodeId = currentAbstractNode->getId();
	      if (abstractNodeId == abstractNode) 
		{
		  AbstractNodeLabel *currentLabel = new AbstractNodeLabel(currentAbstractNode);
		  currentLabel->setPlainText(text);
		  currentLabel->setPos(QPointF(currentX, currentY));
		  currentLabel->setOffset(QPointF(xOffset, yOffset));
		  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
		  currentLabel->setZValue(zvalue);
		  currentLabel->setFontSize(_labelSize);
		  currentAbstractNode->setLabel(currentLabel);
		  _abstractNodeLabelVector.push_back(currentLabel);
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
      std::sort(_abstractNodeVector.begin(), _abstractNodeVector.end(), eventLessThan);
      QSet<QString> types;
      query->prepare("SELECT tail, head, tailabstractnode, headabstractnode, "
		     "linkage, red, green, blue, alpha, zvalue, hidden, masshidden "
		     "FROM saved_eg_plots_edges "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
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
	  int zvalue = query->value(9).toInt();
	  int hidden = query->value(10).toInt();
	  int massHidden = query->value(11).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  bool tailAbstractNode = false;
	  bool headAbstractNode = false;
	  if (tM == 1) 
	    {
	      tailAbstractNode = true;
	    }
	  if (hM == 1) 
	    {
	      headAbstractNode = true;
	    }
	  if (!types.contains(linkage)) 
	    {
	      _presentTypes.push_back(linkage);
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
	      if (massHidden)
		{
		  linkageListWidget->item(linkageListWidget->rowCount() - 1, 0)->
		    setBackground(Qt::gray);
		}
	    }      
	  types.insert(linkage);
	  QString toolTip = "";
	  if (!(tailAbstractNode) && !(headAbstractNode)) 
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
	  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
	  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
	  QGraphicsItem *tempSource = NULL;
	  QGraphicsItem *tempTarget = NULL;
	  if (tailAbstractNode && !(headAbstractNode)) 
	    {
	      while (it2.hasNext()) 
		{
		  AbstractNode *currentSource = it2.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      while (it.hasNext()) 
		{
		  IncidentNode *currentTarget = it.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  else if (!(tailAbstractNode) && headAbstractNode) 
	    {
	      while (it.hasNext()) 
		{
		  IncidentNode *currentSource = it.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      while (it2.hasNext()) 
		{
		  AbstractNode *currentTarget = it2.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  else if (tailAbstractNode && headAbstractNode) 
	    {
	      while (it2.hasNext()) 
		{
		  AbstractNode *currentSource = it2.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      it2.toFront();
	      while (it2.hasNext()) 
		{
		  AbstractNode *currentTarget = it2.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  else if (!(tailAbstractNode) && !(headAbstractNode)) 
	    {
	      while (it.hasNext()) 
		{
		  IncidentNode *currentSource = it.next();
		  if (currentSource->getId() == tail) 
		    {
		      tempSource = currentSource;
		    }
		}
	      it.toFront();
	      while (it.hasNext()) 
		{
		  IncidentNode *currentTarget = it.next();
		  if (currentTarget->getId() == head) 
		    {
		      tempTarget = currentTarget;
		    }
		}
	    }
	  if (tempSource != NULL && tempTarget != NULL) 
	    {
	      Linkage *currentEdge = new Linkage(linkage, _selectedCoder);
	      currentEdge->setZValue(zvalue);
	      currentEdge->setStartItem(tempSource);
	      currentEdge->setEndItem(tempTarget);
	      currentEdge->setToolTip(toolTip);
	      currentEdge->setColor(color);
	      _edgeVector.push_back(currentEdge);
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
      query->prepare("SELECT name, tip, red, green, blue, alpha, "
		     "textred, textgreen, textblue, textalpha, hidden "
		     "FROM saved_eg_plots_legend "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString tip = query->value(1).toString();
	  int red = query->value(2).toInt();
	  int green = query->value(3).toInt();
	  int blue = query->value(4).toInt();
	  int alpha = query->value(5).toInt();
	  int textred = query->value(6).toInt();
	  int textgreen = query->value(7).toInt();
	  int textblue = query->value(8).toInt();
	  int textalpha = query->value(9).toInt();
	  int hidden = query->value(10).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  QColor textColor = QColor(textred, textgreen, textblue, textalpha);
	  QTableWidgetItem *item = new QTableWidgetItem(name);
	  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	  item->setToolTip(tip);
	  item->setData(Qt::DisplayRole, name);
	  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setBackground(color);
	  QVariant textColorVar = QVariant(textColor.rgb());
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setData(Qt::UserRole,
									     textColorVar);
	  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
	    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
		     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	  if (hidden)
	    {
	      eventListWidget->item(eventListWidget->rowCount() - 1, 0)->setBackground(Qt::gray);
	    }
	}
      query->prepare("SELECT startx, starty, endx, endy, arone, artwo, penwidth, penstyle, "
		     "zvalue, red, green, blue, alpha "
		     "FROM saved_eg_plots_lines "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
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
	  _lineVector.push_back(newLine);
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
      query->prepare("SELECT startx, endx, y, penwidth, majorinterval, minordivision, "
		     "majorsize, minorsize, firsttick, lasttick, zvalue, red, green, blue, alpha "
		     "FROM saved_eg_plots_timelines "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  qreal startx = query->value(0).toReal();
	  qreal endx = query->value(1).toReal();
	  qreal y = query->value(2).toReal();
	  int penwidth = query->value(3).toInt();
	  qreal majorinterval = query->value(4).toReal();
	  qreal minordivision = query->value(5).toReal();
	  qreal majorsize = query->value(6).toReal();
	  qreal minorsize = query->value(7).toReal();
	  int firsttick = query->value(8).toInt();
	  int lasttick = query->value(9).toInt();
	  int zValue = query->value(10).toInt();
	  int red = query->value(11).toInt();
	  int green = query->value(12).toInt();
	  int blue = query->value(13).toInt();
	  int alpha = query->value(14).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  TimeLineObject *newTimeLine = new TimeLineObject(startx, endx, y,
							   majorinterval, minordivision,
							   majorsize, minorsize);
	  if (firsttick == 0)
	    {
	      newTimeLine->setFirstTick(false);
	    }
	  if (lasttick == 1)
	    {
	      newTimeLine->setForceLastTick(true);
	    }
	  _timeLineVector.push_back(newTimeLine);
	  newTimeLine->setZValue(zValue);
	  newTimeLine->setColor(color);
	  newTimeLine->setPenWidth(penwidth);
	  scene->addItem(newTimeLine);
	}
      query->prepare("SELECT desc, xpos, ypos, width, size, rotation, zValue, "
		     "red, green, blue, alpha "
		     "FROM saved_eg_plots_texts "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  QString desc = query->value(0).toString();
	  qreal xpos = query->value(1).toReal();
	  qreal ypos = query->value(2).toReal();
	  qreal width = query->value(3).toReal();
	  int size = query->value(4).toInt();
	  qreal rotation = query->value(5).toReal();
	  int zValue = query->value(6).toInt();
	  int red = query->value(7).toInt();
	  int green = query->value(8).toInt();
	  int blue = query->value(9).toInt();
	  int alpha = query->value(10).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  TextObject *newText = new TextObject(desc);
	  _textVector.push_back(newText);
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
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
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
	  _ellipseVector.push_back(newEllipse);
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
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
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
	  _rectVector.push_back(newRect);
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
      query->prepare("SELECT nodeid, abstract, xpos, ypos "
		     "FROM saved_eg_plots_contraction "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  int id = query->value(0).toInt();
	  int abstract = query->value(1).toInt();
	  qreal xpos = query->value(2).toReal();
	  qreal ypos = query->value(3).toReal();
	  if (abstract == 1)
	    {
	      QVectorIterator<AbstractNode*> it(_abstractNodeVector);
	      while (it.hasNext())
		{
		  AbstractNode *current = it.next();
		  if (current->getId() == id)
		    {
		      _contractedMap.insert(current, QPointF(xpos, ypos));
		      break;
		    }
		}
	    }
	  else
	    {
	      QVectorIterator<IncidentNode*> it(_incidentNodeVector);
	      while (it.hasNext())
		{
		  IncidentNode *current = it.next();
		  if (current->getId() == id)
		    {
		      _contractedMap.insert(current, QPointF(xpos, ypos));
		      break;
		    }
		}
	    }
	}
      _contracted = false;
      query->prepare("SELECT casename, checked "
		     "FROM saved_eg_plots_cases "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  QString casename = query->value(0).toString();
	  int checked = query->value(1).toInt();
	  QListWidgetItem *item = new QListWidgetItem(casename, caseListWidget);
	  item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
	  if (checked == 1)
	    {
	      item->setCheckState(Qt::Checked);
	    }
	  else
	    {
	      item->setCheckState(Qt::Unchecked);
	    }
	}      
      query->prepare("SELECT xpos, ypos, horizontal "
		     "FROM saved_eg_plots_guides "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      while (query->next())
	{
	  qreal xpos = query->value(0).toReal();
	  qreal ypos = query->value(1).toReal();
	  int horizontal = query->value(2).toInt();
	  bool isHorizontal = false;
	  if (horizontal == 1)
	    {
	      isHorizontal = true;
	    }
	  GuideLine* guide = new GuideLine(isHorizontal);
	  _guidesVector.push_back(guide);
	  scene->addItem(guide);
	  guide->setOrientationPoint(QPointF(xpos, ypos));
	}
      caseListWidget->setEnabled(true);
      if (!_contractedMap.empty())
	{
	  _contracted = true;
	}
      fixZValues();
      plotLabel->setText(plot);
      changeLabel->setText("");
      scene->update();
      setRangeControls();
      lowerRangeDial->setValue(lowerbound);
      upperRangeDial->setValue(upperbound);
      setGraphControls(true);
      updateLinkages();
      setVisibility();
      checkCongruence();
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
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_settings
      query->prepare("DELETE FROM saved_eg_plots_settings "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_incident_nodes
      query->prepare("DELETE FROM saved_eg_plots_incident_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_edges
      query->prepare("DELETE FROM saved_eg_plots_edges "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_incident_node_labels
      query->prepare("DELETE FROM saved_eg_plots_incident_node_labels "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_abstract_nodes
      query->prepare("DELETE FROM saved_eg_plots_abstract_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_incidents_to_abstract_nodes
      query->prepare("DELETE FROM saved_eg_plots_incidents_to_abstract_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_attributes_to_abstract_nodes
      query->prepare("DELETE FROM saved_eg_plots_attributes_to_abstract_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_abstract_node_labels
      query->prepare("DELETE FROM saved_eg_plots_abstract_node_labels "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_legend
      query->prepare("DELETE FROM saved_eg_plots_legend "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_embedded_incidents
      query->prepare("DELETE FROM saved_eg_plots_embedded_incidents "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_abstract_nodes_to_abstract_nodes 
      query->prepare("DELETE FROM saved_eg_plots_abstract_nodes_to_abstract_nodes "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_lines
      query->prepare("DELETE FROM saved_eg_plots_lines "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_timelines
      query->prepare("DELETE FROM saved_eg_plots_timelines "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_texts
      query->prepare("DELETE FROM saved_eg_plots_texts "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_eipses
      query->prepare("DELETE FROM saved_eg_plots_ellipses "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_rects
      query->prepare("DELETE FROM saved_eg_plots_rects "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_contraction
      query->prepare("DELETE FROM saved_eg_plots_contraction "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_cases
      query->prepare("DELETE FROM saved_eg_plots_cases "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_eg_plots_guides
      query->prepare("DELETE FROM saved_eg_plots_guides "
		     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", plot);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      delete query;
      seePlots();
    }
  delete savedPlotsDialog;
}

void EventGraphWidget::setChangeLabel() 
{
  if (changeLabel->text() == "" && _incidentNodeVector.size() > 0) 
    {
      changeLabel->setText("*");
    }
}

void EventGraphWidget::updateLinkages() 
{
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *current = it.next();
      current->updatePosition();
    }
  if (_compareVector.size() > 0) 
    {
      QVectorIterator<Linkage*> it2(_compareVector);
      while (it2.hasNext()) 
	{
	  Linkage *current = it2.next();
	  current->updatePosition();
	}
    }
}

void EventGraphWidget::addMode() 
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
			 "WHERE attribute = :currentAttribute AND coder = :coder");
	  query->bindValue(":currentAttribute", currentAttribute);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      int currentIncident = query->value(0).toInt();
	      QVectorIterator<IncidentNode *> it2(_incidentNodeVector);
	      while (it2.hasNext()) 
		{
		  IncidentNode *currentIncidentNode = it2.next();
		  if (currentIncidentNode->getId() == currentIncident) 
		    {
		      currentIncidentNode->setColor(color);
		      currentIncidentNode->setMode(attribute);
		      currentIncidentNode->getLabel()->setDefaultTextColor(textColor);
		      currentIncidentNode->setMassHidden(false);
		    }
		}
	    }
	  QVectorIterator<AbstractNode*> it3(_abstractNodeVector);
	  while (it3.hasNext()) 
	    {
	      AbstractNode *currentAbstractNode = it3.next();
	      QSet<QString> attributes = currentAbstractNode->getAttributes();
	      if (attributes.contains(currentAttribute)) 
		{
		  currentAbstractNode->setColor(color);
		  currentAbstractNode->setMode(attribute);
		  currentAbstractNode->getLabel()->setDefaultTextColor(textColor);
		  currentAbstractNode->setMassHidden(false);
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
	      QVariant textColorVar = QVariant(textColor.rgb()); 
	      eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		->setData(Qt::UserRole, textColorVar);
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
  setVisibility();
}

void EventGraphWidget::addModes() 
{
  QPointer<AttributeCheckBoxDialog> attributeDialog = new AttributeCheckBoxDialog(this, INCIDENT);
  attributeDialog->exec();
  if (attributeDialog->getExitStatus() == 0) 
    {
      // Let's first create a few queries
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2= new QSqlQuery;
      QSqlQuery *query3 = new QSqlQuery;
      query->prepare("SELECT description FROM incident_attributes "
		     "WHERE name = :name");
      query2->prepare("SELECT description FROM entities "
		      "WHERE name = :name");
      query3->prepare("SELECT incident FROM attributes_to_incidents "
		      "WHERE attribute  = :currentAttribute AND coder = :coder");
      // Then we get the selected attributes
      QVector<QPair<QString, bool> > chosenAttributes = attributeDialog->getAttributes();
      // Let's create a color palette;
      QList<QColor> palette;
      double hue = double(rand()) / (double(RAND_MAX) + 1.0);
      int colorCount = chosenAttributes.size();
      for (int i = 0; i != colorCount; i++)
	{
	  palette.append(QColor::fromHslF(hue, 1.0, 0.5));
	  hue += 0.61803398874895f;
	  hue = std::fmod(hue, 1.0f);
	}
      int colorNumber = 0;
      // And then we iterate through our attributes
      QVectorIterator<QPair<QString, bool> > it(chosenAttributes);
       while (it.hasNext())
	{
	  // Set our current color and then increment the color number
	  QColor currentColor = palette[colorNumber];
	  colorNumber++;
	  // Let's retrieve the data we need.
	  QPair<QString, bool> currentPair = it.next();
	  QString currentAttribute = currentPair.first;
	  bool isEntity = currentPair.second;
	  QVector<QString> childrenVector;
	  childrenVector.push_back(currentAttribute);
	  findChildren(currentAttribute, &childrenVector, isEntity);
	  QString description = QString();
	  if (isEntity)
	    {
	      query2->bindValue(":name", currentAttribute);
	      query2->exec();
	      query2->first();
	      description = query2->value(0).toString();
	    }
	  else
	    {
	      query->bindValue(":name", currentAttribute);
	      query->exec();
	      query->first();
	      description = query->value(0).toString();
	    }
	  QVectorIterator<QString> it2(childrenVector);
	  while (it2.hasNext())
	    {
	      QString currentChild = it2.next();
	      query3->bindValue(":currentAttribute", currentChild);
	      query3->bindValue(":coder", _selectedCoder);
	      query3->exec();
	      while (query3->next())
		{
		  int currentIncident = query3->value(0).toInt();
		  QVectorIterator<IncidentNode *> it3(_incidentNodeVector);
		  while (it3.hasNext()) 
		    {
		      IncidentNode *currentIncidentNode = it3.next();
		      if (currentIncidentNode->getId() == currentIncident) 
			{
			  currentIncidentNode->setColor(currentColor);
			  currentIncidentNode->setMode(currentAttribute);
			  currentIncidentNode->getLabel()->setDefaultTextColor(Qt::black);
			  currentIncidentNode->setMassHidden(false);
			}
		    }
		}
	      QVectorIterator<AbstractNode*> it4(_abstractNodeVector);
	      while (it4.hasNext()) 
		{
		  AbstractNode *currentAbstractNode = it4.next();
		  QSet<QString> attributes = currentAbstractNode->getAttributes();
		  if (attributes.contains(currentChild)) 
		    {
		      currentAbstractNode->setColor(currentColor);
		      currentAbstractNode->setMode(currentAttribute);
		      currentAbstractNode->getLabel()->setDefaultTextColor(Qt::black);
		      currentAbstractNode->setMassHidden(false);
		    }
		}
	    }
	  bool found = false;
	  for (int i = 0; i < eventListWidget->rowCount(); i++) 
	    {
	      if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == currentAttribute) 
		{
		  found = true;
		  QTableWidgetItem *item = eventListWidget->item(i,0);
		  QString toolTip = breakString(currentAttribute + " - " + description);
		  item->setToolTip(toolTip);
		  eventListWidget->item(i, 1)->setBackground(currentColor);
		  break;
		}
	    }
	  if (!found) 
	    {
	      QTableWidgetItem *item = new QTableWidgetItem(currentAttribute);
	      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	      QString toolTip = breakString(currentAttribute + " - " + description);
	      item->setToolTip(toolTip);
	      item->setData(Qt::DisplayRole, currentAttribute);
	      eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
	      eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
	      eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setBackground(currentColor);
	      QVariant textColorVar = QVariant(QColor(Qt::black).rgb());
	      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->setData(Qt::UserRole,
										 textColorVar);
	      eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	    }
	}
      delete query;
      delete query2;
      delete query3;
    }
  delete attributeDialog;
  setVisibility();
}

void EventGraphWidget::removeMode() 
{
  setChangeLabel();
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<IncidentNode *> it (_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      if (current->getMode() == text) 
	{
	  current->setColor(Qt::white);
	  current->getLabel()->setDefaultTextColor(Qt::black);
	  current->setMode("");
	  current->setMassHidden(false);
	}
    }
  QVectorIterator<AbstractNode*> it2 (_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      if (current->getMode() == text) 
	{
	  current->setColor(Qt::white);
	  current->getLabel()->setDefaultTextColor(Qt::black);
	  current->setMode("");
	  current->setMassHidden(false);
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
      bool hidden = false;
      if (eventListWidget->item(i, 0)->background().color() == Qt::gray)
	{
	  hidden = true;
	}
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
			 "WHERE attribute = :currentAttribute AND coder = :coder");
	  query->bindValue(":currentAttribute", currentAttribute);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      int currentIncident = query->value(0).toInt();
	      QVectorIterator<IncidentNode *> it4(_incidentNodeVector);
	      while (it4.hasNext()) 
		{
		  IncidentNode *currentIncidentNode = it4.next();
		  if (currentIncidentNode->getId() == currentIncident) 
		    {
		      currentIncidentNode->setColor(color);
		      currentIncidentNode->setMode(currentMode);
		      if (hidden)
			{
			  currentIncidentNode->setMassHidden(true);
			}
		      else
			{
			  currentIncidentNode->setMassHidden(false);
			}
		    }
		}
	    }
	  QVectorIterator<AbstractNode*> it5(_abstractNodeVector);
	  while (it5.hasNext()) 
	    {
	      AbstractNode *currentAbstractNode = it5.next();
	      QSet<QString> attributes = currentAbstractNode->getAttributes();
	      if (attributes.contains(currentAttribute)) 
		{
		  currentAbstractNode->setColor(color);
		  currentAbstractNode->setMode(currentMode);
		  if (hidden)
		    {
		      currentAbstractNode->setMassHidden(true);
		    }
		  else
		    {
		      currentAbstractNode->setMassHidden(false);
		    }
		}
	    }
	}
      delete query;
    }
  setVisibility();
}

void EventGraphWidget::setModeButtons(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
    {
      removeModeButton->setEnabled(true);
      hideModeButton->setEnabled(true);
      showModeButton->setEnabled(true);
    }
  else 
    {
      removeModeButton->setEnabled(false);
      hideModeButton->setEnabled(false);
      showModeButton->setEnabled(false);
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
  bool hidden = false;
  QVectorIterator<IncidentNode*> it(_incidentNodeVector);
  while (it.hasNext())
    {
      IncidentNode *current = it.next();
      QString mode = current->getMode();
      if (text == mode)
	{
	  hidden = current->isMassHidden();
	  break;
	}
    }
  if (!hidden)
    {
      QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
      while (it2.hasNext())
	{
	  AbstractNode *current = it2.next();
	  QString mode = current->getMode();
	  if (text == mode)
	    {
	      hidden = current->isMassHidden();
	      break;
	    }
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

void EventGraphWidget::disableLegendButtons() 
{
  removeModeButton->setEnabled(false);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton->setEnabled(false);
  hideModeButton->setEnabled(false);
  showModeButton->setEnabled(false);
  hideLinkageTypeButton->setEnabled(false);
  showLinkageTypeButton->setEnabled(false);
  removeLinkageTypeButton->setEnabled(false);
}

void EventGraphWidget::hideMode() 
{
  hideModeButton->setChecked(true);
  showModeButton->setChecked(false);
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  eventListWidget->currentItem()->setBackground(Qt::gray);
  QVectorIterator<IncidentNode*> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      QString mode = current->getMode();
      if (text == mode) 
	{
	  current->setMassHidden(true);
	}
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      QString mode = current->getMode();
      if (text == mode) 
	{
	  current->setMassHidden(true);
	}
    }
  updateLinkages();
  setVisibility();
}

void EventGraphWidget::showMode() 
{
  hideModeButton->setChecked(false);
  showModeButton->setChecked(true);
  QString text = eventListWidget->currentItem()->data(Qt::DisplayRole).toString();
  eventListWidget->currentItem()->setBackground(Qt::transparent);
  QVectorIterator<IncidentNode*> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      QString type = current->getMode();
      if (text == type) 
	{
	  current->setMassHidden(false);
	}
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      QString type = current->getMode();
      if (text == type) 
	{
	  current->setMassHidden(false);
	}
    }
  updateLinkages();
  setVisibility();
}

void EventGraphWidget::setLinkageButtons(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
    {
      QVectorIterator<Linkage*> it(_edgeVector);
      while (it.hasNext()) 
	{
	  Linkage *current = it.next();
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
      if (_presentTypes.size() > 0) 
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
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *incidentNode = it.next();
      incidentNode->setColor(Qt::white);
      incidentNode->getLabel()->setDefaultTextColor(Qt::black);
      incidentNode->setMode("");
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
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
      bool hidden = false;
      if (eventListWidget->item(i, 0)->background().color() == Qt::gray)
	{
	  hidden = true;
	}
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
			 "WHERE attribute = :currentAttribute AND coder = :coder");
	  query->bindValue(":currentAttribute", currentAttribute);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next()) 
	    {
	      int currentIncident = query->value(0).toInt();
	      QVectorIterator<IncidentNode *> it4(_incidentNodeVector);
	      while (it4.hasNext()) 
		{
		  IncidentNode *currentIncidentNode = it4.next();
		  if (currentIncidentNode->getId() == currentIncident) 
		    {
		      currentIncidentNode->setColor(color);
		      currentIncidentNode->getLabel()->setDefaultTextColor(textColor);
		      currentIncidentNode->setMode(currentMode);
		      if (hidden)
			{
			  currentIncidentNode->setMassHidden(true);
			}
		      else
			{
			  currentIncidentNode->setMassHidden(false);
			}
		    }
		}
	    }
	  QVectorIterator<AbstractNode*> it5(_abstractNodeVector);
	  while (it5.hasNext()) 
	    {
	      AbstractNode *currentAbstractNode = it5.next();
	      QSet<QString> attributes = currentAbstractNode->getAttributes();
	      if (attributes.contains(currentAttribute)) 
		{
		  currentAbstractNode->setColor(color);
		  currentAbstractNode->getLabel()->setDefaultTextColor(textColor);
		  currentAbstractNode->setMode(currentMode);
		  if (hidden)
		    {
		      currentAbstractNode->setMassHidden(true);
		    }
		  else
		    {
		      currentAbstractNode->setMassHidden(false);
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
      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
      while (it.hasNext()) 
	{
	  IncidentNode *incidentNode = it.next();
	  if (incidentNode->getMode() == mode) 
	    {
	      incidentNode->setColor(color);
	      incidentNode->getLabel()->setDefaultTextColor(textColor);
	    }
	}
      QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
      while (it2.hasNext()) 
	{
	  AbstractNode *abstractNode = it2.next();
	  if (abstractNode->getMode() == mode) 
	    {
	      abstractNode->setColor(color);
	      abstractNode->getLabel()->setDefaultTextColor(textColor);
	    }
	}
    }
  setChangeLabel();
  setVisibility();
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
      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
      while (it.hasNext()) 
	{
	  IncidentNode *currentIncidentNode = it.next();
	  currentIncidentNode->setColor(color);
	}
    }
  delete colorDialog;
}

void EventGraphWidget::setEdgeColor() 
{
  if (_edgeVector.size() > 0) 
    {
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      if (colorDialog->exec()) 
	{
	  QColor color = colorDialog->selectedColor();
	  QVectorIterator<Linkage*> it(_edgeVector);
	  while (it.hasNext()) 
	    {
	      Linkage *currentLinkage = it.next();
	      currentLinkage->setColor(color);
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
      QVectorIterator<IncidentNodeLabel*> it(_incidentNodeLabelVector);
      while (it.hasNext()) 
	{
	  IncidentNodeLabel *currentItem = it.next();
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
      QVariant textColorVar = item->data(Qt::UserRole);
      QColor currentText = QColor::fromRgb(textColorVar.toUInt());
      QPointer<ModeColorDialog> colorDialog = new ModeColorDialog(this, currentFill, currentText);
      colorDialog->exec();
      if (colorDialog->getExitStatus() == 0)
	{
	  QColor fillColor = colorDialog->getFillColor();
	  QColor textColor = colorDialog->getTextColor();
	  textColorVar = QVariant(textColor.rgb());
	  item->setBackground(fillColor);
	  item->setData(Qt::UserRole, textColorVar);
	  QTableWidgetItem*neighbour = eventListWidget->item(item->row(), 0);
	  QString mode = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
	  while (it.hasNext()) 
	    {
	      IncidentNode *current = it.next();
	      if (current->getMode() == mode) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
	  while (it2.hasNext()) 
	    {
	      AbstractNode *current = it2.next();
	      if (current->getMode() == mode) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	  setChangeLabel();
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
	  QTableWidgetItem*neighbour = linkageListWidget->item(item->row(), 0);
	  QString type = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<Linkage*> it(_edgeVector);
	  while (it.hasNext()) 
	    {
	      Linkage *current = it.next();
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
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *current = it.next();
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
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *current = it.next();
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
  QString text = linkageListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<Linkage*> it (_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *current = it.next();
      if (current->getType() == text) 
	{
	  delete current;
	  _edgeVector.removeOne(current);
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
  QVectorIterator<QString> it2(_presentTypes);
  while (it2.hasNext()) 
    {
      QString type = it2.next();
      if (type == text) 
	{
	  _presentTypes.removeOne(type);
	}
    }
  if (_presentTypes.size() > 0)
    {
      setChangeLabel();
      setHeights();
      setPlotButtons();
      disableLinkageButtons();
    }
  else
    {
      cleanUp();
      changeLabel->setText("");
      removeLinkageTypeButton->setEnabled(false);
    }
}

void EventGraphWidget::toggleLabels() 
{
  QVectorIterator<IncidentNodeLabel*> it(_incidentNodeLabelVector);
  while (it.hasNext()) 
    {
      IncidentNodeLabel *currentItem = it.next();
      IncidentNode *currentIncidentNode = currentItem->getNode();
      if (currentIncidentNode->isVisible()) 
	{
	  if (_labelsVisible) 
	    {
	      currentItem->hide();
	    }
	  else 
	    {
	      currentItem->show();
	    }
	}
    }
  QVectorIterator<AbstractNodeLabel*> it2(_abstractNodeLabelVector);
  while (it2.hasNext()) 
    {
      AbstractNodeLabel *currentItem = it2.next();
      AbstractNode *currentIncidentNode = currentItem->getAbstractNode();
      if (currentIncidentNode->isVisible()) 
	{
	  if (_labelsVisible) 
	    {
	      currentItem->hide();
	    }
	  else 
	    {
	      currentItem->show();
	    }
	}
    }
  _labelsVisible = !(_labelsVisible);
}

void EventGraphWidget::increaseLabelSize()
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
      setChangeLabel();
    }
}

void EventGraphWidget::decreaseLabelSize()
{
  if (_labelSize > 5)
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
      setChangeLabel();
    }
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
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *currentItem = it.next();
      int order = currentItem->getOrder();
      if (currentItem->isMassHidden())
	{
	  currentItem->hide();
	}
      else
	{
	  if (currentItem->getAbstractNode() == NULL) 
	    {
	      if (order >= lowerRangeDial->value() && order <= upperRangeDial->value()) 
		{
		  currentItem->show();
		}
	      else 
		{
		  currentItem->hide();
		}
	    }
	  else 
	    {
	      currentItem->hide();
	      if (order >= lowerRangeDial->value() && order <= upperRangeDial->value()) 
		{
		  currentItem->getAbstractNode()->show();
		}
	      else 
		{
		  currentItem->getAbstractNode()->hide();
		}
	    }
	}
    }
  if (_checkedCases.size() > 0) 
    {
      it.toFront();
      while (it.hasNext()) 
	{
	  IncidentNode *currentItem = it.next();
	  QVectorIterator<QString> it2(_checkedCases);
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
	      if (currentItem->getAbstractNode() == NULL) 
		{
		  currentItem->hide();
		}
	      else 
		{
		  bool keep = false;
		  QVector<IncidentNode *> contents =
		    currentItem->getAbstractNode()->getIncidents();
		  QVectorIterator<IncidentNode *> it2(contents);
		  while (it2.hasNext()) 
		    {
		      IncidentNode *currentIncident = it2.next();
		      QVectorIterator<QString> it3(_checkedCases);
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
		      currentItem->getAbstractNode()->hide();
		    }
		}
	    }
	}
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      if (current->isMassHidden() || current->getAbstractNode() != NULL)
	{
	  current->hide();
	}
    }
  QVector<Linkage*> tempEdges;
  QVectorIterator<Linkage*> it3(_edgeVector);
  while (it3.hasNext()) 
    {
      tempEdges.push_back(it3.next());
    }
  QVectorIterator<Linkage*> it4(_compareVector);
  while (it4.hasNext()) 
    {
      tempEdges.push_back(it4.next());
    }
  QVectorIterator<Linkage*> it5(tempEdges);
  while (it5.hasNext())
    {
      Linkage *currentEdge = it5.next();
      if (currentEdge->isMassHidden()) 
	{
	  currentEdge->hide();
	}
      else 
	{
	  QGraphicsItem *tail = currentEdge->getStart();
	  QGraphicsItem *head = currentEdge->getEnd();
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
  QVectorIterator<IncidentNodeLabel*> it6(_incidentNodeLabelVector);
  while (it6.hasNext()) 
    {
      IncidentNodeLabel *currentText = it6.next();
      IncidentNode *currentParent = currentText->getNode();
      if (!(currentParent->isVisible())) 
	{
	  currentText->hide();
	}
      else 
	{
	  if (_labelsVisible) 
	    {
	      currentText->show();
	    }
	}
    }
  QVectorIterator<AbstractNodeLabel*> it7(_abstractNodeLabelVector);
  while (it7.hasNext()) 
    {
      AbstractNodeLabel *currentText = it7.next();
      AbstractNode *currentParent = currentText->getAbstractNode();
      if (!(currentParent->isVisible())) 
	{
	  currentText->hide();
	}
      else 
	{
	  if (_labelsVisible) 
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

void EventGraphWidget::setRangeControls() 
{
  lowerRangeDial->setRange(1, _incidentNodeVector.size() - 1);
  upperRangeDial->setRange(2, _incidentNodeVector.size());
  lowerRangeSpinBox->setRange(1, _incidentNodeVector.size() - 1);
  upperRangeSpinBox->setRange(2, _incidentNodeVector.size());
  upperRangeDial->setValue(_incidentNodeVector.size());
  upperRangeSpinBox->setValue(_incidentNodeVector.size());
}

void EventGraphWidget::resetRange()
{
  lowerRangeDial->setValue(1);
  lowerRangeSpinBox->setValue(1);
}

void EventGraphWidget::setTimeRange()
{
  QPointer<TimeRangeDialog> timeRangeDialog = new TimeRangeDialog(this);
  timeRangeDialog->exec();
  if (timeRangeDialog->getExitStatus() == 0)
    {
      QDate startDate = timeRangeDialog->getStartDate();
      QDate endDate = timeRangeDialog->getEndDate();
      QDate currentStart;
      int lowerBound = 1;
      bool finished = false;
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT COUNT(*) FROM incidents");
      query->first();
      int upperBound = query->value(0).toInt();
      query->exec("SELECT timestamp, ch_order FROM incidents "
		  "ORDER BY ch_order ASC");
      while (query->next())
	{
	  QString dateString = query->value(0).toString();
	  int order = query->value(1).toInt();
	  QDate date;
	  if (dateString.length() == 4) // We are dealing with a year only.
	    {
	      date = QDate::fromString(dateString, "yyyy");
	    }
	  if (dateString.length() == 7) // We are dealing with a month and year.
	    {
	      if (dateString[2] == '/')
		{
		  date = QDate::fromString(dateString, "MM/yyyy");
		}
	      else if (dateString[2] == '-')
		{
		  date = QDate::fromString(dateString, "MM-yyyy");
		}
	      else if (dateString[4] == '\\') 
		{
		  date = QDate::fromString(dateString, "yyyy\\MM");
		}
	      else if (dateString[4] == '-')
		{
		  date = QDate::fromString(dateString, "yyyy-MM");
		}
	    }
	  if (dateString.length() == 10) // We are dealing with a day, month and year.
	    {
	      if (dateString[2] == '/')
		{
		  date = QDate::fromString(dateString, "dd/MM/yyyy");
		}
	      else if (dateString[2] == '-')
		{
		  date = QDate::fromString(dateString, "dd-MM-yyyy");
		}
	      else if (dateString[4] == '\\') 
		{
		  date = QDate::fromString(dateString, "yyyy\\MM\\dd");
		}
	      else if (dateString[4] == '-')
		{
		  date = QDate::fromString(dateString, "yyyy-MM-dd");
		}
	    }
	  if (date.isValid())
	    {
	      if (date.daysTo(startDate) <= 0)
		{
		  if (currentStart.isNull())
		    {
		      currentStart = date;
		      lowerBound = order;
		    }
		}
	      if (date.daysTo(endDate) >= 0 && !finished)
		{
		  upperBound = order;
		}
	      else if (date.daysTo(endDate) < 0)
		{
		  finished = true;
		}
	    }
	}
      if (currentStart.isNull())
	{
	  QPointer <QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->setWindowTitle("Setting time range");
	  warningBox->addButton(QMessageBox::Ok);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("Range invalid.");
	  warningBox->setInformativeText("The selected range does not match any dates in the "
					 "data set.");
	  warningBox->exec();
	  delete warningBox;
	  delete timeRangeDialog;
	  delete query;
	  return;
	}
      delete query;
      lowerRangeDial->setValue(lowerBound);
      lowerRangeSpinBox->setValue(lowerBound);
      upperRangeDial->setValue(upperBound);
      upperRangeSpinBox->setValue(upperBound);
      setVisibility();
    }
  delete timeRangeDialog;
}

void EventGraphWidget::exportSvg() 
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
      it.toFront();
      while (it.hasNext())
	{
	  it.next()->show();
	}
    }
}

void EventGraphWidget::exportTable() 
{
  // We create a vector of all types of events and fill it.
  QVector<QGraphicsItem*> incidentNodes;
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      if (current->isVisible()) 
	{
	  incidentNodes.push_back(current);
	}
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      if (current->isVisible()) 
	{
	  incidentNodes.push_back(current);
	}
    }
  // We finish this vector by sorting it.
  std::sort(incidentNodes.begin(), incidentNodes.end(), componentsSort);
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
      QVectorIterator<QGraphicsItem*> it3(incidentNodes);
      while (it3.hasNext()) 
	{
	  row++;
	  QGraphicsItem *current = it3.next();
	  IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	  if (incidentNode) 
	    {
	      QString timing = "";
	      QString description = "";
	      QString raw = "";
	      QString comment = "";
	      QString source = "";
	      QString mode = incidentNode->getMode();
	      QString type = "Incident";
	      int id = incidentNode->getId();
	      QString order = incidentNode->getLabel()->toPlainText();
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
	  else if (abstractNode) 
	    {
	      QString timing = abstractNode->getTiming();
	      QString description = abstractNode->getDescription();
	      QString raw = "";
	      QString comment = abstractNode->getComment();
	      QString source = "";
	      QString mode = abstractNode->getMode();
	      QString type = "";
	      QString id = abstractNode->getLabel()->toPlainText();
	      if (abstractNode->getConstraint() == PATHS)
		{
		  type = "Paths based";
		}
	      else if (abstractNode->getConstraint() == SEMIPATHS)
		{
		  type = "Semi-paths based";
		}
	      else if (abstractNode->getConstraint() == COMPOSITEEVENT)
		{
		  type = "Composite event";
		}
	      else if (abstractNode->getConstraint() == NOCONSTRAINT)
		{
		  type = "No constraint";
		}
	      QString components = "";
	      QVector<IncidentNode *> incidents = abstractNode->getIncidents();
	      int incidentId = abstractNode->getIncidents().first()->getId();
	      QSqlQuery *query = new QSqlQuery;
	      delete query;
	      QVectorIterator<IncidentNode *> it4(incidents);
	      while (it4.hasNext()) 
		{
		  IncidentNode *incident = it4.next();
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
  QVector<QGraphicsItem*> incidentNodes;
  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
  while (it.hasNext()) 
    {
      IncidentNode *current = it.next();
      if (current->isVisible()) 
	{
	  incidentNodes.push_back(current);
	}
    }
  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
  while (it2.hasNext()) 
    {
      AbstractNode *current = it2.next();
      if (current->isVisible()) 
	{
	  incidentNodes.push_back(current);
	}
    }
  // We finish this vector by sorting it.
  std::sort(incidentNodes.begin(), incidentNodes.end(), componentsSort);
  QVectorIterator<QGraphicsItem*> it3(incidentNodes);
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
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      if (incidentNode) 
	{
	  int id = incidentNode->getId();
	  QString label = incidentNode->getLabel()->toPlainText();
	  QString timing = "";
	  QString description = "";
	  QString comment = "";
	  QString mode = incidentNode->getMode();
	  QString xCoord = QString::number(incidentNode->scenePos().x());
	  QString yCoord = QString::number(incidentNode->scenePos().y());
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
      else if (abstractNode) 
	{
	  QString label = abstractNode->getLabel()->toPlainText();
	  QString timing = abstractNode->getTiming();
	  QString description = abstractNode->getDescription();
	  QString comment = abstractNode->getComment();
	  QString mode = abstractNode->getMode();
	  QString xCoord = QString::number(abstractNode->scenePos().x());
	  QString yCoord = QString::number(abstractNode->scenePos().y());
	  QString identifier = "";
	  QString type = "";
	  if (abstractNode->getConstraint() == PATHS)
	    {
	      identifier = "p";
	      type = "Paths based";
	    }
	  else if (abstractNode->getConstraint() == SEMIPATHS)
	    {
	      identifier = "s";
	      type = "Semi-paths based";
	    }
	  else if (abstractNode->getConstraint() == COMPOSITEEVENT)
	    {
	      identifier = "c";
	      type = "Composite event";
	    }
	  else if (abstractNode->getConstraint() == NOCONSTRAINT)
	    {
	      identifier = "n";
	      type = "No constraint";

	    }
	  QString id = identifier + QString::number(abstractNode->getOrder());
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
										 _abstractNodeVector);
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
      QVectorIterator<Linkage*> it(_edgeVector);
      while (it.hasNext()) 
	{
	  Linkage *edge = it.next();
	  if (edge->isVisible()) 
	    {
	      IncidentNode *incidentNodeStart = qgraphicsitem_cast<IncidentNode *>(edge->getStart());
	      IncidentNode *incidentNodeEnd = qgraphicsitem_cast<IncidentNode *>(edge->getEnd());
	      AbstractNode *abstractNodeStart = qgraphicsitem_cast<AbstractNode*>(edge->getStart());
	      AbstractNode *abstractNodeEnd = qgraphicsitem_cast<AbstractNode*>(edge->getEnd());
	      QString description = edge->getType();
	      QString coder = _selectedCoder;
	      QString source = "";
	      QString target = "";
	      QString comment = "";	      
	      if (incidentNodeStart && incidentNodeEnd)
		{
		  QSqlQuery *query = new QSqlQuery;
		  query->prepare("SELECT comment, coder FROM linkage_comments "
				 "WHERE type = :type AND tail = :tail AND head = :head");
		  query->bindValue(":type", description);
		  query->bindValue(":tail", incidentNodeStart->getId());
		  query->bindValue(":head", incidentNodeEnd->getId());
		  query->exec();
		  query->first();
		  if (!query->isNull(0))
		    {
		      comment = query->value(1).toString() + ": " + query->value(0).toString();
		    }
		  delete query;
		}
	      if (incidentNodeStart && incidentNodeEnd) 
		{
		  source = "i" + QString::number(incidentNodeStart->getOrder());
		  target = "i" + QString::number(incidentNodeEnd->getOrder());
		}
	      else if (incidentNodeStart && abstractNodeEnd) 
		{
		  source = "i" + QString::number(incidentNodeStart->getOrder());
		  if (abstractNodeEnd->getConstraint() == PATHS)
		    {
		      target = "p" + QString::number(abstractNodeEnd->getOrder());
		    }
		  else if (abstractNodeEnd->getConstraint() == SEMIPATHS)
		    {
		      target = "s" + QString::number(abstractNodeEnd->getOrder());
		    }
		  else if (abstractNodeEnd->getConstraint() == COMPOSITEEVENT)
		    {
		      target = "c" + QString::number(abstractNodeEnd->getOrder());
		    }
		  else if (abstractNodeEnd->getConstraint() == NOCONSTRAINT)
		    {
		      target = "n" + QString::number(abstractNodeEnd->getOrder());
		    }
		}
	      else if (abstractNodeStart && abstractNodeEnd) 
		{
		  if (abstractNodeStart->getConstraint() == PATHS)
		    {
		      source = "p" + QString::number(abstractNodeStart->getOrder());
		    }
		  else if (abstractNodeStart->getConstraint() == SEMIPATHS)
		    {
		      source = "s" + QString::number(abstractNodeStart->getOrder());
		    }
		  else if (abstractNodeStart->getConstraint() == COMPOSITEEVENT)
		    {
		      source = "c" + QString::number(abstractNodeStart->getOrder());
		    }
		  else if (abstractNodeStart->getConstraint() == NOCONSTRAINT)
		    {
		      source = "n" + QString::number(abstractNodeStart->getOrder());
		    }
		  if (abstractNodeEnd->getConstraint() == PATHS)
		    {
		      target = "p" + QString::number(abstractNodeEnd->getOrder());
		    }
		  else if (abstractNodeEnd->getConstraint() == SEMIPATHS)
		    {
		      target = "s" + QString::number(abstractNodeEnd->getOrder());
		    }
		  else if (abstractNodeEnd->getConstraint() == COMPOSITEEVENT)
		    {
		      target = "c" + QString::number(abstractNodeEnd->getOrder());
		    }
		  else if (abstractNodeEnd->getConstraint() == NOCONSTRAINT)
		    {
		      target = "n" + QString::number(abstractNodeEnd->getOrder());
		    }
		}
	      else if (abstractNodeStart && incidentNodeEnd) 
		{
		  if (abstractNodeStart->getConstraint() == PATHS) 
		    {
		      source = "p" + QString::number(abstractNodeStart->getOrder());
		    }
		  else if (abstractNodeStart->getConstraint() == SEMIPATHS)
		    {
		      source = "s" + QString::number(abstractNodeStart->getOrder());
		    }
		  else if (abstractNodeStart->getConstraint() == COMPOSITEEVENT)
		    {
		      source = "c" + QString::number(abstractNodeStart->getOrder());
		    }
		  else if (abstractNodeStart->getConstraint() == NOCONSTRAINT)
		    {
		      source = "n" + QString::number(abstractNodeStart->getOrder());
		    }
		  target = "i" + QString::number(incidentNodeEnd->getId());
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

void EventGraphWidget::processIncidentNodeContextMenu(const QString &action) 
{
  retrieveData();
  if (action == ABSTRACTACTION) 
    {
      abstractEvents();
    }
  else if (action == DISAGGREGATEACTION) 
    {
      disaggregateEvent();
    }
  else if (action == MAKEABSTRACTNODEACTION) 
    {
      abstractEvents();
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
  else if (action == ORIGINALPOSACTION)
    {
      setEventOriginalPosition();
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
  else if (action == ADDLINKAGEEVIDENCEACTION)
    {
      addLinkageEvidence();
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
  else if (action == ADDTOCASEACTION)
    {
      addToCase();
    }
  else if (action == CREATENEWCASEACTION)
    {
      createNewCase();
    }
  else if (action == REMOVEFROMCASEACTION)
    {
      removeFromCase();
    }
}

void EventGraphWidget::abstractEvents() 
{
  if (_currentData.size() > 0) 
    {
      QPointer<AbstractionDialog> abstractionDialog = new AbstractionDialog(this,
									    _incidentNodeVector,
									    _abstractNodeVector,
									    _currentData,
									    _presentTypes,
									    _selectedCoder);
      abstractionDialog->exec();
      if (abstractionDialog->getExitStatus() == 0)
	{
	  QVector<IncidentNode *> tempIncidents = abstractionDialog->getCollectedIncidents();
	  QString chosenAttribute = abstractionDialog->getAttribute();
	  if (tempIncidents.size() > 0) 
	    {
	      std::sort(tempIncidents.begin(), tempIncidents.end(), componentsSort);
	      qreal lowestX = 0.0;
	      qreal highestX = 0.0;
	      qreal lowestY = 0.0;
	      qreal highestY = 0.0;
	      QVectorIterator<IncidentNode *> it2(tempIncidents);
	      while (it2.hasNext()) 
		{
		  IncidentNode *current = it2.next();
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
	      AbstractNode *current = new AbstractNode(width, description, originalPos,
						       (_abstractNodeVector.size() + 1) * -1,
						       abstractionDialog->getConstraint(),
						       tempIncidents);
	      current->setPos(originalPos);
	      current->setZValue(_incidentNodeVector[0]->zValue());
	      QVectorIterator<QGraphicsItem*> it3(_currentData);
	      while (it3.hasNext()) 
		{
		  IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(it3.peekNext());
		  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(it3.peekNext());
		  if (incidentNode) 
		    {
		      IncidentNode *item = qgraphicsitem_cast<IncidentNode *>(it3.next());
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
					     "WHERE attribute = :attribute AND incident = :id "
					     "AND coder = :coder");
			      query->bindValue(":attribute", currentAttribute);
			      query->bindValue(":id", id);
			      query->bindValue(":coder", _selectedCoder);
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
			      item->setAbstractNode(current);
			      item->hide();
			    }
			}
		      else 
			{
			  item->setAbstractNode(current);
			  item->hide();
			}
		    }
		  else if (abstractNode) 
		    {
		      AbstractNode *item = qgraphicsitem_cast<AbstractNode*>(it3.next());
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
			      QSet<QString> abstractNodeAttributes = abstractNode->getAttributes();
			      QSetIterator<QString> it6(abstractNodeAttributes);
			      while (it6.hasNext())
				{
				  QString abstractNodeAttribute = it6.next();
				  if (abstractNodeAttribute == currentAttribute)
				    {
				      hasAttribute = true;
				    }
				}
			    }
			  if (hasAttribute)
			    {
			      item->setAbstractNode(current);
			      item->hide();
			    }
			}
		      else
			{
			  item->setAbstractNode(current);
			  item->hide();
			}
		    }
		}
	      _abstractNodeVector.push_back(current);
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
	      AbstractNodeLabel *abstractNodeLabel = new AbstractNodeLabel(current);
	      current->setLabel(abstractNodeLabel);
	      qreal xOffset = (current->getWidth() / 2) - 20;
	      abstractNodeLabel->setOffset(QPointF(xOffset,0));
	      abstractNodeLabel->setNewPos(current->scenePos());
	      abstractNodeLabel->setZValue(_incidentNodeVector[0]->zValue() + 1);
	      abstractNodeLabel->setDefaultTextColor(Qt::black);
	      abstractNodeLabel->setFontSize(_labelSize);
	      _abstractNodeLabelVector.push_back(abstractNodeLabel);
	      scene->addItem(abstractNodeLabel);
	      rewireLinkages(current, tempIncidents);
	      updateAbstractNodeOrder();
	      updateLinkages();
	      setVisibility();
	      _currentData.clear();
	      current->setSelected(true);
	      retrieveData();
	      setChangeLabel();
	      addLinkageTypeButton->setEnabled(false);
	      compareButton->setEnabled(false);
	      restoreModeColors();
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

void EventGraphWidget::rewireLinkages(AbstractNode *abstractNode, QVector<IncidentNode *> incidents) 
{
  QVectorIterator<QString> lit(_presentTypes);
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
      QVectorIterator<IncidentNode *> tit(incidents);
      while (tit.hasNext()) 
	{
	  IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(tit.next());
	  if (incidentNode) 
	    {
	      incidentId.push_back(incidentNode->getId());
	    }
	}
      QVectorIterator<IncidentNode *> it(incidents);
      while (it.hasNext()) 
	{
	  IncidentNode *current = it.next();
	  int incident = current->getId();
	  query->prepare("SELECT tail FROM linkages "
			 "WHERE head = :head AND coder = :coder AND type = :type");
	  query->bindValue(":head", incident);
	  query->bindValue(":type", currentType);
	  query->bindValue(":coder", _selectedCoder);
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
	      QGraphicsItem *tempTarget = abstractNode;
	      QGraphicsItem *tempSource = NULL;
	      int currentTail = it2.next();
	      QVectorIterator<IncidentNode *> et(_incidentNodeVector);
	      while (et.hasNext()) 
		{
		  IncidentNode *currentIncidentNode = et.next();
		  if (currentIncidentNode->getId() == currentTail) 
		    {
		      if (currentIncidentNode->getAbstractNode() == NULL) 
			{
			  tempSource = currentIncidentNode;
			}
		      else 
			{
			  tempSource = currentIncidentNode->getAbstractNode();
			  AbstractNode *tempAbstractNode = currentIncidentNode->getAbstractNode();
			  while (tempAbstractNode->getAbstractNode() != NULL) 
			    {
			      tempAbstractNode = tempAbstractNode->getAbstractNode();
			    }
			  tempSource = tempAbstractNode;
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
			  QVectorIterator<Linkage*> it3(_edgeVector);
			  while (it3.hasNext()) 
			    {
			      Linkage*temp = it3.next();
			      if (temp->getType() == currentType) 
				{
				  if (temp->getStart() == tempSource && temp->getEnd() == tempTarget) 
				    {
				      found = true;
				    }
				}
			    }
			  if (!found) 
			    {
			      Linkage *newEdge = new Linkage(currentType, _selectedCoder);
			      newEdge->setZValue(_incidentNodeVector[0]->zValue() - 1);
			      newEdge->setStartItem(tempSource);
			      newEdge->setEndItem(tempTarget);
			      newEdge->setColor(edgeColor);
			      _edgeVector.push_back(newEdge);
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
	  query->bindValue(":coder", _selectedCoder);
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
	      QGraphicsItem *tempSource = abstractNode;
	      int currentHead = it4.next();
	      QVectorIterator<IncidentNode *> et(_incidentNodeVector);
	      while (et.hasNext()) 
		{
		  IncidentNode *currentIncidentNode = et.next();
		  if (currentIncidentNode->getId() == currentHead) 
		    {
		      if (currentIncidentNode->getAbstractNode() == NULL) 
			{
			  tempTarget = currentIncidentNode;
			}
		      else 
			{
			  tempTarget = currentIncidentNode->getAbstractNode();
			  AbstractNode *tempAbstractNode = currentIncidentNode->getAbstractNode();
			  while (tempAbstractNode->getAbstractNode() != NULL) 
			    {
			      tempAbstractNode = tempAbstractNode->getAbstractNode();
			    }
			  tempTarget = tempAbstractNode;
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
			  QVectorIterator<Linkage*> it5(_edgeVector);
			  while (it5.hasNext()) 
			    {
			      Linkage *temp = it5.next();
			      if (temp->getType() == currentType) 
				{
				  if (temp->getStart() == tempSource && temp->getEnd() == tempTarget) 
				    {
				      found = true;
				    }
				}
			    }
			  if (!found) 
			    {
			      Linkage *newEdge = new Linkage(currentType, _selectedCoder);
			      newEdge->setZValue(_incidentNodeVector[0]->zValue() - 1);
			      newEdge->setStartItem(tempSource);
			      newEdge->setEndItem(tempTarget);
			      newEdge->setColor(edgeColor);
			      _edgeVector.push_back(newEdge);
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
  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
  while (it.hasNext()) 
    {
      AbstractNode *abstractNode = it.next();
      if (abstractNode->getAbstractNode() == _selectedAbstractNode) 
	{
	  components.push_back(abstractNode);
	  abstractNode->setAbstractNode(NULL);
	  abstractNode->show();
	  // And we want to reassign their child incidents.
	  QVector<IncidentNode *> tempIncidents = abstractNode->getIncidents();
	  QVectorIterator<IncidentNode *> it2(tempIncidents);
	  while (it2.hasNext()) 
	    {
	      IncidentNode *tempIncident = it2.next();
	      tempIncident->setAbstractNode(abstractNode);
	    }
	}
    }
  QVectorIterator<IncidentNode *> it2(_incidentNodeVector);
  while (it2.hasNext()) 
    {
      IncidentNode *incidentNode = it2.next();
      if (incidentNode->getAbstractNode() == _selectedAbstractNode) 
	{
	  components.push_back(incidentNode);
	  incidentNode->setAbstractNode(NULL);
	  incidentNode->show();
	}
    }
  std::sort(components.begin(), components.end(), componentsSort);
  QVectorIterator<QGraphicsItem*> it3(components);
  qreal xPos = _selectedAbstractNode->scenePos().x();
  qreal firstPos = components.first()->scenePos().x();
  qreal diff = 50;
  while (it3.hasNext()) 
    {
      QGraphicsItem *current = it3.next();
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      diff = diff * -1;
      qreal yPos = _selectedAbstractNode->scenePos().y() + diff;
      if (incidentNode) 
	{
	  incidentNode->setPos(xPos + incidentNode->scenePos().x() - firstPos, yPos);
	  incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
	}
      else if (abstractNode) 
	{
	  abstractNode->setPos(xPos + abstractNode->scenePos().x() - firstPos, yPos);
	  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
	}
    }
  QVector<QGraphicsItem*> allEvents;
  QVectorIterator<IncidentNode *> it4(_incidentNodeVector);
  while (it4.hasNext()) 
    {
      IncidentNode *item = it4.next();
      allEvents.push_back(item);
    }
  QVectorIterator<AbstractNode*> it5(_abstractNodeVector);
  while (it5.hasNext()) 
    {
      AbstractNode *item = it5.next();
      allEvents.push_back(item);
    }
  std::sort(allEvents.begin(), allEvents.end(), eventLessThanWidth);
  QGraphicsItem *nextUp = NULL;
  QVectorIterator<QGraphicsItem*> it6(allEvents);
  while (it6.hasNext()) 
    {
      QGraphicsItem *current = it6.next();
      if (!components.contains(current) &&
	  current->scenePos().x() > _selectedAbstractNode->scenePos().x() +
	  _selectedAbstractNode->getWidth()) 
	{
	  nextUp = current;
	  break;
	}
    }
  if (nextUp != NULL) 
    {
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(components.last());
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(components.last());
      qreal dist = 0;
      if (incidentNode) 
	{
	  dist = nextUp->scenePos().x() - incidentNode->scenePos().x() -
	    incidentNode->getWidth() + 40;
	}
      else if (abstractNode) 
	{
	  dist = nextUp->scenePos().x() - abstractNode->scenePos().x() -
	    abstractNode->getWidth() + 40;
	}
      QPointF currentPos = nextUp->scenePos();
      if (dist < 70) 
	{
	  QVectorIterator<QGraphicsItem*> it7(allEvents);
	  while (it7.hasNext()) 
	    {
	      QGraphicsItem *item = it7.next();
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(item);
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(item);
	      if (incidentNode) 
		{
		  if (incidentNode->scenePos().x() >= currentPos.x()) 
		    {
		      if (!components.contains(incidentNode)) 
			{
			  incidentNode->setPos(incidentNode->scenePos().x() + 70 - dist,
					       incidentNode->scenePos().y());
			  incidentNode->setOriginalPos(incidentNode->scenePos());
			  incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
			}
		    }
		}
	      else if (abstractNode) 
		{
		  if (abstractNode->scenePos().x() >= currentPos.x()) 
		    {
		      if (!components.contains(abstractNode)) 
			{
			  abstractNode->setPos(abstractNode->scenePos().x() +
					       70 - dist, abstractNode->scenePos().y());
			  abstractNode->setOriginalPos(abstractNode->scenePos());
			  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
			}
		    }
		}
	    }
	}
    }
  /* 
     Then we adapt to the new situation by cleaning up the abstractNode that
     we have disaggregated, by rewiring linkages, and so on.
  */
  QVector<Linkage*>::iterator it8;
  for (it8 = _edgeVector.begin(); it8 != _edgeVector.end();) 
    {
      Linkage *current = *it8;
      if(current->getStart() == _selectedAbstractNode ||
	 current->getEnd() == _selectedAbstractNode) 
	{
	  delete current;
	  _edgeVector.removeOne(current);
	}
      else 
	{
	  it8++;
	}
    }
  QVectorIterator<AbstractNode*> it9(_abstractNodeVector);
  while (it9.hasNext()) 
    {
      AbstractNode *abstractNode = it9.next();
      if (abstractNode != _selectedAbstractNode) 
	{
	  QVector<IncidentNode *> currentIncidents = abstractNode->getIncidents();
	  rewireLinkages(abstractNode, currentIncidents);
	}
    }
  scene->removeItem(_selectedAbstractNode->getLabel());
  scene->removeItem(_selectedAbstractNode);
  delete _selectedAbstractNode->getLabel();
  _abstractNodeLabelVector.removeOne(_selectedAbstractNode->getLabel());
  updateAbstractNodeIds(_selectedAbstractNode);
  delete _selectedAbstractNode;
  _abstractNodeVector.removeOne(_selectedAbstractNode);
  _selectedAbstractNode = NULL;
  updateAbstractNodeOrder();
  setVisibility();
  if (_abstractNodeVector.size() > 0) 
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

void EventGraphWidget::updateAbstractNodeIds(AbstractNode *abstractNode) 
{
  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
  while (it.hasNext()) 
    {
      AbstractNode *current = it.next();
      if (current != abstractNode && current->getId() < abstractNode->getId()) 
	{
	  current->setNewId(current->getId() + 1);
	}
    }
}

void EventGraphWidget::updateAbstractNodeOrder() 
{
  int order = 1;
  std::sort(_abstractNodeVector.begin(), _abstractNodeVector.end(), componentsSort);
  QVectorIterator<AbstractNode*> it(_abstractNodeVector);
  while (it.hasNext()) 
    {
      AbstractNode *current = it.next();
      current->setOrder(order);
      AbstractNodeLabel *label = current->getLabel();
      QColor labelColor = label->defaultTextColor();
      delete label;
      _abstractNodeLabelVector.removeOne(label);
      AbstractNodeLabel *newLabel = new AbstractNodeLabel(current);
      order++;
      if (current->getConstraint() == PATHS)
	{
	  QString label = "P-" + QString::number(current->getOrder());
	  newLabel->setPlainText(label);      
	}
      else if (current->getConstraint() == SEMIPATHS)
	{
	  QString label = "S-" + QString::number(current->getOrder());
	  newLabel->setPlainText(label);
	}
      else if (current->getConstraint() == COMPOSITEEVENT)
	{
	  QString label = "C-" + QString::number(current->getOrder());
	  newLabel->setPlainText(label);
	}
      else if (current->getConstraint() == NOCONSTRAINT)
	{
	  QString label = "N-" + QString::number(current->getOrder());
	  newLabel->setPlainText(label);
	}
      current->setLabel(newLabel);
      _abstractNodeLabelVector.push_back(newLabel);
      qreal xOffset = (current->getWidth() / 2) - 20;
      newLabel->setOffset(QPointF(xOffset,0));
      newLabel->setNewPos(current->scenePos());
      newLabel->setZValue(_incidentNodeVector[0]->zValue() + 1);
      newLabel->setDefaultTextColor(labelColor);
      newLabel->setFontSize(_labelSize);
      scene->addItem(newLabel);
      if (_labelsVisible == false || !current->isVisible())
	{
	  newLabel->hide();
	}
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
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(it.peekNext());
	      AbstractNode *abstractNode =  qgraphicsitem_cast<AbstractNode*>(it.peekNext());
	      if (incidentNode) 
		{
		  IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(it.next());
		  currentIncidentNode->setColor(color);
		}
	      else if (abstractNode) 
		{
		  AbstractNode *currentIncidentNode = qgraphicsitem_cast<AbstractNode*>(it.next());
		  currentIncidentNode->setColor(color);
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
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(it.peekNext());
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(it.peekNext());
	      if (incidentNode) 
		{
		  IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode *>(it.next());
		  IncidentNodeLabel *currentLabel = currentIncidentNode->getLabel();
		  currentLabel->setDefaultTextColor(color);
		}
	      else if (abstractNode) 
		{
		  AbstractNode *currentIncidentNode = qgraphicsitem_cast<AbstractNode*>(it.next());
		  AbstractNodeLabel *currentLabel = currentIncidentNode->getLabel();
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
  if (_currentData.size() == 1) 
    {
      QGraphicsItem *current = _currentData[0];
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
	  bool modes = lineage->modesOn();
	  QVectorIterator<IncidentNode *> it(_incidentNodeVector);
	  while (it.hasNext()) 
	    {
	      IncidentNode *current = it.next();
	      current->setColor(unrelatedFill);
	      if (modes)
		{
		  current->setMode(UNRELATEDMODE);
		}
	      current->getLabel()->setDefaultTextColor(unrelatedText);
	    }
	  QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
	  while (it2.hasNext()) 
	    {
	      AbstractNode *current = it2.next();
	      current->setColor(unrelatedFill);
	      if (modes)
		{
		  current->setMode(UNRELATEDMODE);
		}
	      current->getLabel()->setDefaultTextColor(unrelatedText);
	    }
	  IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	  if (incidentNode) 
	    {
	      incidentNode->setColor(originFill);
	      if (modes)
		{
		  incidentNode->setMode(ORIGINMODE);
		}
	      incidentNode->getLabel()->setDefaultTextColor(originText);
	    }
	  else if (abstractNode) 
	    {
	      abstractNode->setColor(originFill);
	      if (modes)
		{
		  abstractNode->setMode(ORIGINMODE);
		}
	      abstractNode->getLabel()->setDefaultTextColor(originText);
	    }
	  if (_presentTypes.size() > 1) 
	    {
	      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, _presentTypes);
	      relationshipChooser->exec();
	      if (relationshipChooser->getExitStatus() == 0) 
		{
		  QString selection = relationshipChooser->getSelection();
		  QSet<QGraphicsItem*> finished;
		  finished.insert(current);
		  findAncestors(ancestorsFill, ancestorsText, current,
				&finished, selection, modes);
		  finished.clear();
		  finished.insert(current);
		  findDescendants(descendantsFill, descendantsText, current,
				  &finished, selection, modes);
		}
	    }
	  else 
	    {
	      QString selection = _presentTypes[0];
	      QSet<QGraphicsItem*> finished;
	      finished.insert(current);
	      findAncestors(ancestorsFill, ancestorsText, current,
			    &finished, selection, modes);
	      finished.clear();
	      finished.insert(current);
	      findDescendants(descendantsFill, descendantsText, current,
			      &finished, selection, modes);
	    }
	  if (modes)
	    {
	      bool ancestorFound = false;
	      bool originFound = false;
	      bool descendantFound = false;
	      bool unrelatedFound = false;
	      for (int i = 0; i < eventListWidget->rowCount(); i++) 
		{
		  if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == ANCESTORMODE) 
		    {
		      ancestorFound = true;
		      QTableWidgetItem *item = eventListWidget->item(i,0);
		      QString toolTip = breakString(ANCESTORMODE + " - "
						    + "Ancestors in lineage");
		      item->setToolTip(toolTip);
		      eventListWidget->item(i, 1)->setBackground(ancestorsFill);
		      QVariant textColorVar = QVariant(ancestorsText.rgb()); 
		      eventListWidget->item(eventListWidget->rowCount() - 1, 1)
			->setData(Qt::UserRole, textColorVar);
		    }
		  else if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == DESCENDANTMODE) 
		    {
		      descendantFound = true;
		      QTableWidgetItem *item = eventListWidget->item(i,0);
		      QString toolTip = breakString(DESCENDANTMODE + " - "
						    + "Descendants in lineage");
		      item->setToolTip(toolTip);
		      eventListWidget->item(i, 1)->setBackground(descendantsFill);
		      QVariant textColorVar = QVariant(descendantsText.rgb()); 
		      eventListWidget->item(eventListWidget->rowCount() - 1, 1)
			->setData(Qt::UserRole, textColorVar);
		    } 
		  else if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == ORIGINMODE) 
		    {
		      originFound = true;
		      QTableWidgetItem *item = eventListWidget->item(i,0);
		      QString toolTip = breakString(ORIGINMODE + " - "
						    + "Origin in lineage");
		      item->setToolTip(toolTip);
		      eventListWidget->item(i, 1)->setBackground(originFill);
		      QVariant textColorVar = QVariant(originText.rgb()); 
		      eventListWidget->item(eventListWidget->rowCount() - 1, 1)
			->setData(Qt::UserRole, textColorVar);
		    }
		  else if (eventListWidget->item(i, 0)->data(Qt::DisplayRole) == UNRELATEDMODE) 
		    {
		      unrelatedFound = true;
		      QTableWidgetItem *item = eventListWidget->item(i,0);
		      QString toolTip = breakString(UNRELATEDMODE + " - "
						    + "Unrelated to lineage");
		      item->setToolTip(toolTip);
		      eventListWidget->item(i, 1)->setBackground(unrelatedFill);
		      QVariant textColorVar = QVariant(unrelatedText.rgb()); 
		      eventListWidget->item(eventListWidget->rowCount() - 1, 1)
			->setData(Qt::UserRole, textColorVar);
		    }
		}
	      if (!ancestorFound) 
		{
		  QTableWidgetItem *item = new QTableWidgetItem(ANCESTORMODE);
		  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		  QString toolTip = breakString(ANCESTORMODE + " - " + "Ancestors in lineage");
		  item->setToolTip(toolTip);
		  item->setData(Qt::DisplayRole, ANCESTORMODE);
		  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setBackground(ancestorsFill);
		  QVariant textColorVar = QVariant(ancestorsText.rgb()); 
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setData(Qt::UserRole, textColorVar);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		}
	      if (!descendantFound)
		{
		  QTableWidgetItem *item = new QTableWidgetItem(DESCENDANTMODE);
		  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		  QString toolTip = breakString(DESCENDANTMODE + " - " + "Descendants in lineage");
		  item->setToolTip(toolTip);
		  item->setData(Qt::DisplayRole, DESCENDANTMODE);
		  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setBackground(descendantsFill);
		  QVariant textColorVar = QVariant(descendantsText.rgb()); 
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setData(Qt::UserRole, textColorVar);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		}
	      if (!originFound)
		{
		  QTableWidgetItem *item = new QTableWidgetItem(ORIGINMODE);
		  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		  QString toolTip = breakString(ORIGINMODE + " - " + "Origin in lineage");
		  item->setToolTip(toolTip);
		  item->setData(Qt::DisplayRole, ORIGINMODE);
		  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setBackground(originFill);
		  QVariant textColorVar = QVariant(originText.rgb()); 
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setData(Qt::UserRole, textColorVar);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		}
	      if (!unrelatedFound)
		{
		  QTableWidgetItem *item = new QTableWidgetItem(UNRELATEDMODE);
		  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		  QString toolTip = breakString(UNRELATEDMODE + " - " + "Unrelated to lineage");
		  item->setToolTip(toolTip);
		  item->setData(Qt::DisplayRole, UNRELATEDMODE);
		  eventListWidget->setRowCount(eventListWidget->rowCount() + 1);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 0, item);
		  eventListWidget->setItem(eventListWidget->rowCount() - 1, 1, new QTableWidgetItem);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setBackground(unrelatedFill);
		  QVariant textColorVar = QVariant(unrelatedText.rgb()); 
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)
		    ->setData(Qt::UserRole, textColorVar);
		  eventListWidget->item(eventListWidget->rowCount() - 1, 1)->
		    setFlags(eventListWidget->item(eventListWidget->rowCount() - 1, 1)->flags() ^
			     Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		}
	    }
	}
    }
}

void EventGraphWidget::findAncestors(QColor ancestorFill,
				     QColor ancestorText,
				     QGraphicsItem *origin,
				     QSet<QGraphicsItem*> *pFinished,
				     QString type,
				     bool modes) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query->bindValue(":type", type);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  if (direction == PAST) 
    {
      QVectorIterator<Linkage*> it(_edgeVector);
      while (it.hasNext()) 
	{
	  Linkage *edge = it.next();
	  if (edge->getStart() == origin) 
	    {
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getEnd());
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getEnd());
	      if (incidentNode) 
		{
		  incidentNode->setColor(ancestorFill);
		  if (modes)
		    {
		      incidentNode->setMode(ANCESTORMODE);
		    }
		  incidentNode->getLabel()->setDefaultTextColor(ancestorText);
		}
	      else if (abstractNode) 
		{
		  abstractNode->setColor(ancestorFill);
		  if (modes)
		    {
		      abstractNode->setMode(ANCESTORMODE);
		    }
		  abstractNode->getLabel()->setDefaultTextColor(ancestorText);
		}
	      if (!pFinished->contains(edge->getEnd())) 
		{
		  pFinished->insert(edge->getEnd());
		  findAncestors(ancestorFill, ancestorText, edge->getEnd(), pFinished, type, modes);
		}
	    }
	}
    }
  else if (direction == FUTURE) 
    {
      QVectorIterator<Linkage*> it(_edgeVector);
      while (it.hasNext()) 
	{
	  Linkage *edge = it.next();
	  if (edge->getEnd() == origin) 
	    {
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getStart());
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getStart());
	      if (incidentNode) 
		{
		  incidentNode->setColor(ancestorFill);
		  if (modes)
		    {
		      incidentNode->setMode(ANCESTORMODE);
		    }
		  incidentNode->getLabel()->setDefaultTextColor(ancestorText);
		}
	      else if (abstractNode) 
		{
		  abstractNode->setColor(ancestorFill);
		  if (modes)
		    {
		      abstractNode->setMode(ANCESTORMODE);
		    }
		  abstractNode->getLabel()->setDefaultTextColor(ancestorText);
		}
	      if (!pFinished->contains(edge->getStart())) 
		{
		  pFinished->insert(edge->getStart());
		  findAncestors(ancestorFill, ancestorText, edge->getStart(), pFinished, type, modes);
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
				       QString type,
				       bool modes) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query->bindValue(":type", type);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  if (direction == PAST) 
    {
      QVectorIterator<Linkage*> it(_edgeVector);
      while (it.hasNext()) 
	{
	  Linkage *edge = it.next();
	  if (edge->getEnd() == origin) 
	    {
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getStart());
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getStart());
	      if (incidentNode) 
		{
		  incidentNode->setColor(descendantFill);
		  if (modes)
		    {
		      incidentNode->setMode(DESCENDANTMODE);
		    }
		  incidentNode->getLabel()->setDefaultTextColor(descendantText);
		}
	      else if (abstractNode) 
		{
		  abstractNode->setColor(descendantFill);
		  if (modes)
		    {
		      abstractNode->setMode(DESCENDANTMODE);
		    }
		  abstractNode->getLabel()->setDefaultTextColor(descendantText);
		}
	      if (!pFinished->contains(edge->getStart())) 
		{
		  pFinished->insert(edge->getStart());
		  findDescendants(descendantFill, descendantText, edge->getStart(),
				  pFinished, type, modes);
		}
	    }
	}
    }
  else if (direction == FUTURE) 
    {
      QVectorIterator<Linkage*> it(_edgeVector);
      while (it.hasNext()) 
	{
	  Linkage *edge = it.next();
	  if (edge->getStart() == origin) 
	    {
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getEnd());
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getEnd());
	      if (incidentNode) 
		{
		  incidentNode->setColor(descendantFill);
		  if (modes)
		    {
		      incidentNode->setMode(DESCENDANTMODE);
		    }
		  incidentNode->getLabel()->setDefaultTextColor(descendantText);
		}
	      else if (abstractNode) 
		{
		  abstractNode->setColor(descendantFill);
		  if (modes)
		    {
		      abstractNode->setMode(DESCENDANTMODE);
		    }
		  abstractNode->getLabel()->setDefaultTextColor(descendantText);
		}
	      if (!pFinished->contains(edge->getEnd())) 
		{
		  pFinished->insert(edge->getEnd());
		  findDescendants(descendantFill, descendantText, edge->getEnd(),
				  pFinished, type, modes);
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

void EventGraphWidget::addToCase()
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->exec("SELECT name FROM cases");
  QVector<QString> cases;
  while (query->next())
    {
      cases.push_back(query->value(0).toString());
    }
  query->prepare("SELECT incident FROM incidents_to_cases "
		 "WHERE casename = :casename AND "
		 "incident = :incident");
  query2->prepare("INSERT INTO incidents_to_cases "
		  "(incident, casename) "
		  "VALUES (:incident, :casename)");
  QPointer<ComboBoxDialog> comboDialog = new ComboBoxDialog(this, cases);
  comboDialog->setWindowTitle("Select case to assign to");
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) 
    {
      QString selection = comboDialog->getSelection();
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext())
	{
	  QGraphicsItem *current = it.next();
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(current);
	  AbstractNode *abstract = qgraphicsitem_cast<AbstractNode*>(current);
	  if (incident)
	    {
	      int id = incident->getId();
	      query->bindValue(":casename", selection);
	      query->bindValue(":incident", id);
	      query->exec();
	      query->first();
	      if (query->isNull(0))
		{
		  query2->bindValue(":incident", id);
		  query2->bindValue(":casename", selection);
		  query2->exec();
		}
	    }
	  else if (abstract)
	    {
	      QVector<IncidentNode*> incidents = abstract->getIncidents();
	      QVectorIterator<IncidentNode*> it2(incidents);
	      while (it2.hasNext())
		{
		  IncidentNode *incident = it2.next();
		  int id = incident->getId();
		  query->bindValue(":casename", selection);
		  query->bindValue(":incident", id);
		  query->exec();
		  query->first();
		  if (query->isNull(0))
		    {
		      query2->bindValue(":incident", id);
		      query2->bindValue(":casename", selection);
		      query2->exec();
		    }
		}
	    }
	}
    }
  delete query;
  delete query2;
}

void EventGraphWidget::removeFromCase()
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM cases");
  QVector<QString> cases;
  while (query->next())
    {
      cases.push_back(query->value(0).toString());
    }
  query->prepare("DELETE FROM incidents_to_cases "
		 "WHERE casename = :casename AND "
		 "incident = :incident");
  QPointer<ComboBoxDialog> comboDialog = new ComboBoxDialog(this, cases);
  comboDialog->setWindowTitle("Select case to assign to");
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) 
    {
      QString selection = comboDialog->getSelection();
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext())
	{
	  QGraphicsItem *current = it.next();
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(current);
	  AbstractNode *abstract = qgraphicsitem_cast<AbstractNode*>(current);
	  if (incident)
	    {
	      int id = incident->getId();
	      query->bindValue(":casename", selection);
	      query->bindValue(":incident", id);
	      query->exec();
	    }
	  else if (abstract)
	    {
	      QVector<IncidentNode*> incidents = abstract->getIncidents();
	      QVectorIterator<IncidentNode*> it2(incidents);
	      while (it2.hasNext())
		{
		  IncidentNode *incident = it2.next();
		  int id = incident->getId();
		  query->bindValue(":casename", selection);
		  query->bindValue(":incident", id);
		  query->exec();
		}
	    }
	}
    }
  delete query;
}

void EventGraphWidget::createNewCase()
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->prepare("INSERT INTO cases (name, description) "
		 "VALUES (:name, :description)");
  query2->prepare("INSERT INTO incidents_to_cases "
		  "(incident, casename) "
		  "VALUES (:incident, :casename)");
  QPointer<CaseDialog> caseDialog = new CaseDialog(this);
  caseDialog->exec();
  if (caseDialog->getExitStatus() == 0) 
    {
      QString casename = caseDialog->getName();
      QString description = caseDialog->getDescription();
      query->bindValue(":name", casename);
      query->bindValue(":description", description);
      query->exec();
      QListIterator<QGraphicsItem*> it(scene->selectedItems());
      while (it.hasNext())
	{
	  QGraphicsItem *current = it.next();
	  IncidentNode *incident = qgraphicsitem_cast<IncidentNode*>(current);
	  AbstractNode *abstract = qgraphicsitem_cast<AbstractNode*>(current);
	  if (incident)
	    {
	      int id = incident->getId();
	      query2->bindValue(":casename", casename);
	      query2->bindValue(":incident", id);
	      query2->exec();
	    }
	  else if (abstract)
	    {
	      QVector<IncidentNode*> incidents = abstract->getIncidents();
	      QVectorIterator<IncidentNode*> it2(incidents);
	      while (it2.hasNext())
		{
		  IncidentNode *incident = it2.next();
		  int id = incident->getId();
		  query2->bindValue(":casename", casename);
		  query2->bindValue(":incident", id);
		  query2->exec();
		}
	    }
	}
    }
  delete query;
  delete query2;
  updateCases();
}

void EventGraphWidget::exportTransitionMatrix() 
{
  // First we ask the user which relationship to consider (if there are more than one present).
  QString type = _presentTypes[0];
  if (_presentTypes.size() > 1) 
    {
      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, _presentTypes);
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
      bool isProb = exportDialog->isProbability();
      bool ignoreDuplicates = exportDialog->ignoreDuplicates();
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
	  warningBox->setWindowTitle("Exporting transition matrix");
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
	  QVectorIterator<IncidentNode *> incidentNodeIt(_incidentNodeVector);
	  QVectorIterator<AbstractNode*> abstractNodeIt(_abstractNodeVector);
	  int occurrence = 0;
	  while (incidentNodeIt.hasNext())
	    {
	      IncidentNode *currentIncidentNode = incidentNodeIt.next();
	      if (currentIncidentNode->isVisible())
		{
		  allEvents.push_back(currentIncidentNode);
		}
	    }
	  while (abstractNodeIt.hasNext())
	    {
	      AbstractNode *currentAbstractNode = abstractNodeIt.next();
	      if (currentAbstractNode->isVisible())
		{
		  allEvents.push_back(currentAbstractNode);
		}
	    }
	  // If we want to look at attributes instead of mode, it is better
	  // to identify the relevant attributes before we iterate through
	  // all events.
	  QVector<int> incidents;
	  QVector<QString> attributeVector; 
	  QVectorIterator<QGraphicsItem*> allIt(allEvents);
	  while (allIt.hasNext())
	    {
	      QGraphicsItem* currentIncidentNode = allIt.next();
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(currentIncidentNode);
	      AbstractNode *abstractNodeItem = qgraphicsitem_cast<AbstractNode*>(currentIncidentNode);
	      if (incidentNode) 
		{
		  if (incidentNode->getMode() == rowMode) 
		    {
		      occurrence++;
		    }
		}
	      else if (abstractNodeItem) 
		{
		  if (abstractNodeItem->getMode() == rowMode) 
		    {
		      occurrence++;
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
	      QVectorIterator<Linkage*> it(_edgeVector);
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
			     "WHERE attribute = :attribute AND coder = :coder");
	      while (rit.hasNext())
		{
		  QString currentAttribute = rit.next();
		  query->bindValue(":attribute", currentAttribute);
		  query->bindValue(":coder", _selectedCoder);
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
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();
		  while (query->next())
		    {
		      colIncidents.insert(query->value(0).toInt());
		    }
		}
	      QMap<QGraphicsItem*, QSet<QString>> observedEnds;
	      while (it.hasNext()) 
		{
		  Linkage *edge = it.next();
		  if (edge->getType() == type) 
		    {
		      if (edge->isVisible()) 
			{
			  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getStart());
			  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getEnd());
			  AbstractNode *startAbstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getStart());
			  AbstractNode *endAbstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getEnd());
			  // If we are only counting modes.
			  QString startMode = "";
			  QString endMode = "";
			  if (startIncidentNode) 
			    {
			      startMode = startIncidentNode->getMode();
			    }
			  else if (startAbstractNode) 
			    {
			      startMode = startAbstractNode->getMode();
			    }
			  if (endIncidentNode) 
			    {
			      endMode = endIncidentNode->getMode();
			    }
			  else if (endAbstractNode) 
			    {
			      endMode = endAbstractNode->getMode();
			    }
			  if (startMode == rowMode && endMode == colMode) 
			    {
			      if (ignoreDuplicates)
				{
				  QSet<QString> currentModes = observedEnds.value(edge->getStart());
				  if (!currentModes.contains(endMode))
				    {
				      transitions++;
				      count++;
				      currentModes.insert(endMode);
				      observedEnds.insert(edge->getStart(), currentModes);
				    }
				}
			      else
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
      delete exportDialog;
    }
  else
    {
      delete exportDialog;
    }
}

void EventGraphWidget::setEventOriginalPosition()
{
 if (_currentData.size() == 1) 
    {
      QGraphicsItem *current = _currentData[0];
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      if (incidentNode) 
	{
	  incidentNode->setPos(incidentNode->getOriginalPos().x(), incidentNode->scenePos().y());
	  incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
	}
      else if (abstractNode) 
	{
	  abstractNode->setPos(abstractNode->getOriginalPos().x(), abstractNode->scenePos().y());
	  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
	}
    }
}

void EventGraphWidget::settleEvent() 
{
  if (_currentData.size() == 1) 
    {
      QGraphicsItem *current = _currentData[0];
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
      if (incidentNode) 
	{
	  incidentNode->setOriginalPos(current->scenePos());
	}
      else if (abstractNode) 
	{
	  abstractNode->setOriginalPos(current->scenePos());
	}
    }
}

void EventGraphWidget::normalizeDistance() 
{
  if (_currentData.size() == 1) 
    {
      QVector<QGraphicsItem*> allEvents;
      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
      while (it.hasNext()) 
	{
	  allEvents.push_back(it.next());
	}
      QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
      while (it2.hasNext()) 
	{
	  allEvents.push_back(it2.next());
	}
      std::sort(allEvents.begin(), allEvents.end(), eventLessThanWidth);
      QGraphicsItem *current = _currentData[0];
      QGraphicsItem *target = NULL;
      QVectorIterator<QGraphicsItem*> it3(allEvents);
      while (it3.hasNext()) 
	{
	  QGraphicsItem *item = it3.next();
	  IncidentNode *itemIncidentNode = qgraphicsitem_cast<IncidentNode *>(item);
	  AbstractNode *itemAbstractNode = qgraphicsitem_cast<AbstractNode*>(item);
	  if (itemIncidentNode && itemIncidentNode->scenePos().x() +
	      itemIncidentNode->getWidth() < current->scenePos().x()) 
	    {
	      target = item;
	    }
	  else if (itemAbstractNode && itemAbstractNode->scenePos().x() +
		   itemAbstractNode->getWidth() < current->scenePos().x()) 
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
	  IncidentNode *targetIncidentNode = qgraphicsitem_cast<IncidentNode *>(target);
	  AbstractNode *targetAbstractNode = qgraphicsitem_cast<AbstractNode*>(target);
	  int width = 0;
	  if (targetIncidentNode) 
	    {
	      width = targetIncidentNode->getWidth();
	    }
	  else if (targetAbstractNode) 
	    {
	      width = targetAbstractNode->getWidth();
	    }
	  current->setPos(target->scenePos().x() + 70 + width - 40, current->scenePos().y());
	  IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	  if (incidentNode) 
	    {
	      incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
	    }
	  else if (abstractNode) 
	    {
	      abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
	    }
	}
    }
}

void EventGraphWidget::makeParallel() 
{
  if (_currentData.size() > 1) 
    {
      QGraphicsItem *lowest = NULL;
      QVectorIterator<QGraphicsItem*> it(_currentData);
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
      QVectorIterator<QGraphicsItem*> it2(_currentData);
      while (it2.hasNext()) 
	{
	  QGraphicsItem *current = it2.next();
	  if (current != lowest) 
	    {
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	      if (incidentNode) 
		{
		  incidentNode->setPos(lowest->scenePos().x(), incidentNode->scenePos().y());
		  incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
		}
	      else if (abstractNode) 
		{
		  abstractNode->setPos(lowest->scenePos().x(), abstractNode->scenePos().y());
		  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
		}
	    } 
	}
    }
}

void EventGraphWidget::closeGap() 
{
  if (_currentData.size() == 1) 
    {
      QVector<QGraphicsItem*> allEvents;
      QVectorIterator<IncidentNode *> it(_incidentNodeVector);
      while (it.hasNext()) 
	{
	  allEvents.push_back(it.next());
	}
      QVectorIterator<AbstractNode*> it2(_abstractNodeVector);
      while (it2.hasNext()) 
	{
	  allEvents.push_back(it2.next());
	}
      std::sort(allEvents.begin(), allEvents.end(), eventLessThanWidth);
      QGraphicsItem *current = _currentData[0];
      QGraphicsItem *target = NULL;
      QVectorIterator<QGraphicsItem*> it3(allEvents);
      while (it3.hasNext()) 
	{
	  QGraphicsItem *item = it3.next();
	  IncidentNode *itemIncidentNode = qgraphicsitem_cast<IncidentNode *>(item);
	  AbstractNode *itemAbstractNode = qgraphicsitem_cast<AbstractNode*>(item);
	  if (itemIncidentNode) 
	    {
	      if (itemIncidentNode->isVisible() &&
		  itemIncidentNode->scenePos().x() + itemIncidentNode->getWidth() < current->scenePos().x()) 
		{
		  target = item;
		}
	    }
	  else if (itemAbstractNode) 
	    {
	      if (itemAbstractNode->isVisible() &&
		  itemAbstractNode->scenePos().x() + itemAbstractNode->getWidth() < current->scenePos().x()) 
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
	  IncidentNode *targetIncidentNode = qgraphicsitem_cast<IncidentNode *>(target);
	  AbstractNode *targetAbstractNode = qgraphicsitem_cast<AbstractNode*>(target);
	  int width = 0;
	  if (targetIncidentNode) 
	    {
	      width = targetIncidentNode->getWidth();
	    }
	  else if (targetAbstractNode) 
	    {
	      width = targetAbstractNode->getWidth();
	    }
	  if (current->scenePos().x() - target->scenePos().x() - width + 40 > 70) 
	    {
	      qreal oldX = current->scenePos().x();
	      current->setPos(target->scenePos().x() + 70 + width - 40, current->scenePos().y());
	      qreal newX = current->scenePos().x();
	      qreal dist = oldX - newX;
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(current);
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(current);
	      if (incidentNode) 
		{
		  incidentNode->setOriginalPos(incidentNode->scenePos());
		  incidentNode->getLabel()->setNewPos(incidentNode->scenePos());
		}
	      else if (abstractNode) 
		{
		  abstractNode->setOriginalPos(abstractNode->scenePos());
		  abstractNode->getLabel()->setNewPos(abstractNode->scenePos());
		}
	      QVectorIterator<QGraphicsItem*> it5(allEvents);
	      while (it5.hasNext()) 
		{
		  QGraphicsItem *follow = it5.next();
		  if (follow->scenePos().x() > current->scenePos().x()) 
		    {
		      IncidentNode *followIncidentNode = qgraphicsitem_cast<IncidentNode *>(follow);
		      AbstractNode *followAbstractNode = qgraphicsitem_cast<AbstractNode*>(follow);
		      if (followIncidentNode) 
			{
			  followIncidentNode->setPos(followIncidentNode->scenePos().x() - dist, followIncidentNode->scenePos().y());
			  followIncidentNode->setOriginalPos(followIncidentNode->scenePos());
			  followIncidentNode->getLabel()->setNewPos(followIncidentNode->scenePos());
			}
		      else if (followAbstractNode) 
			{
			  followAbstractNode->setPos(followAbstractNode->scenePos().x() - dist, followAbstractNode->scenePos().y());
			  followAbstractNode->setOriginalPos(followAbstractNode->scenePos());
			  followAbstractNode->getLabel()->setNewPos(followAbstractNode->scenePos());
			}
		    }
		}
	    }
	}
    }
}

void EventGraphWidget::changeEventDescription() 
{
  QPointer<EventTextDialog> textDialog = new EventTextDialog(this);
  textDialog->submitText(_selectedAbstractNode->getDescription());
  textDialog->submitTiming(_selectedAbstractNode->getTiming());
  textDialog->exec();
  if (textDialog->getExitStatus() == 0) 
    {
      QString description = textDialog->getText();
      QString timing = textDialog->getTiming();
      _selectedAbstractNode->setDescription(description);
      _selectedAbstractNode->setTiming(timing);
      retrieveData();
    }
  delete textDialog;
}

void EventGraphWidget::addLinkage() 
{
  IncidentNode *incidentNodeOne = qgraphicsitem_cast<IncidentNode *>(scene->selectedItems()[0]);
  IncidentNode *incidentNodeTwo = qgraphicsitem_cast<IncidentNode *>(scene->selectedItems()[1]);
  if (incidentNodeOne && incidentNodeTwo) 
    {
      QString type = _presentTypes[0];
      if (_presentTypes.size() > 1) 
	{
	  QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, _presentTypes);
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
      int idOne = incidentNodeOne->getId();
      int idTwo = incidentNodeTwo->getId();
      bool found = false;
      query->prepare("SELECT tail, head FROM linkages "
		     "WHERE type = :type AND coder = :coder AND tail = :tail AND head = :head");
      query->bindValue(":type", type);
      query->bindValue(":coder", _selectedCoder);
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
      query->bindValue(":coder", _selectedCoder);
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
	  if (incidentNodeOne->getOrder() < incidentNodeTwo->getOrder()) 
	    {
	      if (direction == PAST) 
		{
		  query->bindValue(":tail", idTwo);
		  query->bindValue(":head", idOne);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();      
		  Linkage *newLinkage = new Linkage(type, _selectedCoder);
		  newLinkage->setZValue(_incidentNodeVector[0]->zValue() - 1);
		  newLinkage->setStartItem(incidentNodeTwo);
		  newLinkage->setEndItem(incidentNodeOne);
		  QString toolTip = "no comment";
		  newLinkage->setColor(edgeColor);
		  _edgeVector.push_back(newLinkage);
		  scene->addItem(newLinkage);
		}
	      else if (direction == FUTURE) 
		{
		  query->bindValue(":tail", idOne);
		  query->bindValue(":head", idTwo);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();      
		  Linkage *newLinkage = new Linkage(type, _selectedCoder);
		  newLinkage->setZValue(_incidentNodeVector[0]->zValue() - 1);
		  newLinkage->setStartItem(incidentNodeOne);
		  newLinkage->setEndItem(incidentNodeTwo);
		  QString toolTip = "no comment";
		  newLinkage->setToolTip(toolTip);
		  newLinkage->setColor(edgeColor);
		  _edgeVector.push_back(newLinkage);
		  scene->addItem(newLinkage);
		}
	    }
	  else 
	    {
	      if (direction == PAST) 
		{
		  query->bindValue(":tail", idOne);
		  query->bindValue(":head", idTwo);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();      
		  Linkage *newLinkage = new Linkage(type, _selectedCoder);
		  newLinkage->setZValue(_incidentNodeVector[0]->zValue()-1);
		  newLinkage->setStartItem(incidentNodeOne);
		  newLinkage->setEndItem(incidentNodeTwo);
		  QString toolTip = "no comment";
		  newLinkage->setColor(edgeColor);
		  _edgeVector.push_back(newLinkage);
		  scene->addItem(newLinkage);
		}
	      else if (direction == FUTURE) 
		{
		  query->bindValue(":tail", idTwo);
		  query->bindValue(":head", idOne);
		  query->bindValue(":type", type);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();      
		  Linkage *newLinkage = new Linkage(type, _selectedCoder);
		  newLinkage->setZValue(_incidentNodeVector[0]->zValue()-1);
		  newLinkage->setStartItem(incidentNodeTwo);
		  newLinkage->setEndItem(incidentNodeOne);
		  QString toolTip = "no comment";
		  newLinkage->setColor(edgeColor);
		  newLinkage->setToolTip(toolTip);
		  _edgeVector.push_back(newLinkage);
		  scene->addItem(newLinkage);
		}
	    }
	}
      delete query;
    }
  setHeights();
  updateLinkages();
}

void EventGraphWidget::addLinkageEvidence() 
{
  IncidentNode *incidentNodeOne = qgraphicsitem_cast<IncidentNode *>(scene->selectedItems()[0]);
  IncidentNode *incidentNodeTwo = qgraphicsitem_cast<IncidentNode *>(scene->selectedItems()[1]);
  if (incidentNodeOne && incidentNodeTwo) 
    {
      QString type = _presentTypes[0];
      if (_presentTypes.size() > 1) 
	{
	  QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, _presentTypes);
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
      int idOne = incidentNodeOne->getId();
      int idTwo = incidentNodeTwo->getId();
      bool found = false;
      query->prepare("SELECT tail, head FROM linkages "
		     "WHERE type = :type AND coder = :coder AND tail = :tail AND head = :head");
      query->bindValue(":type", type);
      query->bindValue(":coder", _selectedCoder);
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
      query->bindValue(":coder", _selectedCoder);
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
	  if (incidentNodeOne->getOrder() < incidentNodeTwo->getOrder()) 
	    {
	      if (direction == PAST) 
		{
		  QPointer<AddEvidenceDialog> evidenceDialog = new AddEvidenceDialog(idTwo,
										     idOne,
										     type,
										     _selectedCoder,
										     this);
		  evidenceDialog->exec();
		  if (evidenceDialog->getExitStatus() == 0)
		    {
		      query->bindValue(":tail", idTwo);
		      query->bindValue(":head", idOne);
		      query->bindValue(":type", type);
		      query->bindValue(":coder", _selectedCoder);
		      query->exec();      
		      Linkage *newLinkage = new Linkage(type, _selectedCoder);
		      newLinkage->setZValue(_incidentNodeVector[0]->zValue() - 1);
		      newLinkage->setStartItem(incidentNodeTwo);
		      newLinkage->setEndItem(incidentNodeOne);
		      QString toolTip = "no comment";
		      newLinkage->setColor(edgeColor);
		      _edgeVector.push_back(newLinkage);
		      scene->addItem(newLinkage);
		    }
		}
	      else if (direction == FUTURE) 
		{
		  QPointer<AddEvidenceDialog> evidenceDialog = new AddEvidenceDialog(idOne,
										     idTwo,
										     type,
										     _selectedCoder,
										     this);
		  evidenceDialog->exec();
		  if (evidenceDialog->getExitStatus() == 0)
		    {
		      query->bindValue(":tail", idOne);
		      query->bindValue(":head", idTwo);
		      query->bindValue(":type", type);
		      query->bindValue(":coder", _selectedCoder);
		      query->exec();      
		      Linkage *newLinkage = new Linkage(type, _selectedCoder);
		      newLinkage->setZValue(_incidentNodeVector[0]->zValue() - 1);
		      newLinkage->setStartItem(incidentNodeOne);
		      newLinkage->setEndItem(incidentNodeTwo);
		      QString toolTip = "no comment";
		      newLinkage->setToolTip(toolTip);
		      newLinkage->setColor(edgeColor);
		      _edgeVector.push_back(newLinkage);
		      scene->addItem(newLinkage);
		    }
		}
	    }
	  else 
	    {
	      if (direction == PAST) 
		{
		  QPointer<AddEvidenceDialog> evidenceDialog = new AddEvidenceDialog(idOne,
										     idTwo,
										     type,
										     _selectedCoder,
										     this);
		  evidenceDialog->exec();
		  if (evidenceDialog->getExitStatus() == 0)
		    {
		      query->bindValue(":tail", idOne);
		      query->bindValue(":head", idTwo);
		      query->bindValue(":type", type);
		      query->bindValue(":coder", _selectedCoder);
		      query->exec();      
		      Linkage *newLinkage = new Linkage(type, _selectedCoder);
		      newLinkage->setZValue(_incidentNodeVector[0]->zValue() - 1);
		      newLinkage->setStartItem(incidentNodeOne);
		      newLinkage->setEndItem(incidentNodeTwo);
		      QString toolTip = "no comment";
		      newLinkage->setColor(edgeColor);
		      _edgeVector.push_back(newLinkage);
		      scene->addItem(newLinkage);
		    }
		}
	      else if (direction == FUTURE) 
		{
		  QPointer<AddEvidenceDialog> evidenceDialog = new AddEvidenceDialog(idTwo,
										     idOne,
										     type,
										     _selectedCoder,
										     this);
		  evidenceDialog->exec();
		  if (evidenceDialog->getExitStatus() == 0)
		    {
		      query->bindValue(":tail", idTwo);
		      query->bindValue(":head", idOne);
		      query->bindValue(":type", type);
		      query->bindValue(":coder", _selectedCoder);
		      query->exec();      
		      Linkage *newLinkage = new Linkage(type, _selectedCoder);
		      newLinkage->setZValue(_incidentNodeVector[0]->zValue() - 1);
		      newLinkage->setStartItem(incidentNodeTwo);
		      newLinkage->setEndItem(incidentNodeOne);
		      QString toolTip = "no comment";
		      newLinkage->setColor(edgeColor);
		      newLinkage->setToolTip(toolTip);
		      _edgeVector.push_back(newLinkage);
		      scene->addItem(newLinkage);
		    }
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
  QString type = _presentTypes[0];
  if (_presentTypes.size() > 1) 
    {
      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, _presentTypes);
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
  QString type = _presentTypes[0];
  if (_presentTypes.size() > 1) 
    {
      QPointer<ComboBoxDialog> relationshipChooser = new ComboBoxDialog(this, _presentTypes);
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
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *edge = it.next();
      if (edge->getType() == type) 
	{
	  if (edge->getEnd() == origin) 
	    {
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getStart());
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getStart());
	      if (incidentNode) 
		{
		  incidentNode->setSelected(true);
		}
	      else if (abstractNode) 
		{
		  abstractNode->setSelected(true);
		}
	      if (!pFinished->contains(edge->getStart())) 
		{
		  pFinished->insert(edge->getStart());
		  selectAncestors(edge->getStart(), pFinished, type);
		}
	    }
	}
    }
}

void EventGraphWidget::selectDescendants(QGraphicsItem *origin,
					 QSet<QGraphicsItem*> *pFinished,
					 QString type) 
{
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *edge = it.next();
      if (edge->getType() == type) 
	{
	  if (edge->getStart() == origin) 
	    {
	      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(edge->getEnd());
	      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(edge->getEnd());
	      if (incidentNode) 
		{
		  incidentNode->setSelected(true);
		}
	      else if (abstractNode) 
		{
		  abstractNode->setSelected(true);
		}
	      if (!pFinished->contains(edge->getEnd())) 
		{
		  pFinished->insert(edge->getEnd());
		  selectDescendants(edge->getEnd(), pFinished, type);
		}
	    }
	}
    }
}

void EventGraphWidget::processLinkageContextMenu(const QString &action) 
{
  if (action == SEELINKAGEEVIDENCEACTION)
    {
      seeLinkageEvidence();
    }
  else if (action == REMOVELINKAGEACTION) 
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
}

void EventGraphWidget::seeLinkageEvidence()
{
 if (scene->selectedItems().size() == 1) 
    {
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(scene->selectedItems().first());
      if (linkage)
	{
	  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getStart());
	  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getEnd());
	  int tail = startIncidentNode->getId();
	  int head = endIncidentNode->getId();
	  QString type = linkage->getType();
	  QString coder = _selectedCoder;
	  if (linkage->getPenStyle() == 4)
	    {
	      coder = _selectedCompare;
	    }
	  QPointer<EvidenceDialog> evidenceDialog = new EvidenceDialog(tail, head, type, coder, this);
	  evidenceDialog->exec();
	  QString comment = evidenceDialog->getComment();
	  linkage->setToolTip(comment);
	}
    }
}

void EventGraphWidget::removeLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(scene->selectedItems().first());
      if (linkage) 
	{
	  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getStart());
	  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getEnd());
	  int tail = startIncidentNode->getId();
	  int head = endIncidentNode->getId();
	  QString type = linkage->getType();
	  QSqlQuery *query =  new QSqlQuery;
	  query->prepare("DELETE FROM linkages "
			 "WHERE tail = :tail AND head = :head AND coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", head);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", type);
	  query->exec();
	  query->prepare("DELETE FROM linkages_sources "
			 "WHERE tail = :tail AND head = :head AND coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", head);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", type);
	  query->exec();
	  delete query;
	  delete linkage;
	  _edgeVector.removeOne(linkage);
	}
    }
  setChangeLabel();
  setHeights();
  setVisibility();
}

void EventGraphWidget::removeNormalLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(scene->selectedItems().first());
      if (linkage) 
	{
	  QPointer<QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->setWindowTitle("Removing linkage");
	  warningBox->addButton(QMessageBox::Yes);
	  warningBox->addButton(QMessageBox::No);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<h2>Are you sure?</h2>");
	  warningBox->setInformativeText("Are you sure you want to remove this linkage?");
	  if (warningBox->exec() == QMessageBox::Yes) 
	    {
	      IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getStart());
	      IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getEnd());
	      if (startIncidentNode && endIncidentNode) 
		{
		  int tail = startIncidentNode->getId();
		  int head = endIncidentNode->getId();
		  QString type = linkage->getType();
		  QSqlQuery *query =  new QSqlQuery;
		  query->prepare("DELETE FROM linkages "
				 "WHERE tail = :tail AND head = :head "
				 "AND coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", type);
		  query->exec();
		  query->prepare("DELETE FROM linkages_sources "
				 "WHERE tail = :tail AND head = :head "
				 "AND coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", type);
		  query->exec();
		  delete query;
		  delete linkage;
		  _edgeVector.removeOne(linkage);
		  setChangeLabel();
		  setHeights();
		  return;
		}
	      else 
		{
		  QPointer<QMessageBox> warningBox2 = new QMessageBox(this);
		  warningBox->setWindowTitle("Removing linkage");
		  warningBox2->addButton(QMessageBox::Ok);
		  warningBox2->setIcon(QMessageBox::Warning);
		  warningBox2->setText("<h2>Cannot remove this linkage</h2>");
		  warningBox2->setInformativeText("You cannot remove linkages that have abstracted "
						  "events as tail and/or head.");
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
}

void EventGraphWidget::ignoreLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(scene->selectedItems().first());
      if (linkage) 
	{
	  linkage->setPenStyle(1);
	}
    }
}


void EventGraphWidget::keepLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(scene->selectedItems().first());
      if (linkage) 
	{
	  QMap<int, QSet<int>> tailsMap;
	  QMap<int, QSet<int>> headsMap;
	  QString type = linkage->getType();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT tail, head FROM linkages "
			 "WHERE type = :type AND coder = :coder");
	  query->bindValue(":type", type);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next())
	    {
	      int tail = query->value(0).toInt();
	      int head = query->value(1).toInt();
	      QSet<int> currentTails = tailsMap.value(head);
	      QSet<int> currentHeads = headsMap.value(tail);
	      currentTails.insert(tail);
	      currentHeads.insert(head);
	      headsMap.insert(tail, currentHeads);
	      tailsMap.insert(head, currentTails);
	    }
	  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getStart());
	  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getEnd());
	  int tail = startIncidentNode->getId();
	  int head = endIncidentNode->getId();
	  linkage->setPenStyle(1);
	  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	  query->bindValue(":type", type);
	  query->exec();
	  query->first();
	  QString direction = query->value(0).toString();
	  delete query;
	  QSet<int> markOne;
	  QSet<int> markTwo;
	  findHeads(&markOne, &headsMap, tail);
	  findTails(&markTwo, &tailsMap, tail);
	  QSetIterator<int> it2(markOne);
	  while (it2.hasNext()) 
	    {
	      int currentPathMate = it2.next();
	      QSet<int> currentHeads = headsMap.value(currentPathMate);
	      if (currentHeads.contains(head))
		{
		  linkage->setPenStyle(2);
		}
	    }
	  QSetIterator<int> it3(markTwo);
	  while (it3.hasNext()) 
	    {
	      int currentPathMate = it3.next();
	      QSet<int> currentHeads = headsMap.value(currentPathMate);
	      if (currentHeads.contains(head))
		{
		  linkage->setPenStyle(2);
		}
	    }
	}
    }
}

void EventGraphWidget::acceptLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(scene->selectedItems().first());
      if (linkage) 
	{
	  QMap<int, QSet<int>> tailsMap;
	  QMap<int, QSet<int>> headsMap;
	  QString type = linkage->getType();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT tail, head FROM linkages "
			 "WHERE type = :type AND coder = :coder");
	  query->bindValue(":type", type);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  while (query->next())
	    {
	      int tail = query->value(0).toInt();
	      int head = query->value(1).toInt();
	      QSet<int> currentTails = tailsMap.value(head);
	      QSet<int> currentHeads = headsMap.value(tail);
	      currentTails.insert(tail);
	      currentHeads.insert(head);
	      headsMap.insert(tail, currentHeads);
	      tailsMap.insert(head, currentTails);
	    }
	  IncidentNode *startIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getStart());
	  IncidentNode *endIncidentNode = qgraphicsitem_cast<IncidentNode *>(linkage->getEnd());
	  int tail = startIncidentNode->getId();
	  int head = endIncidentNode->getId();
	  linkage->setPenStyle(1);
	  _edgeVector.push_back(linkage);
	  _compareVector.removeOne(linkage);
	  query->prepare("INSERT INTO linkages (tail, head, coder, type) "
			 "VALUES (:tail, :head, :coder, :type)");
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", head);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", type);
	  query->exec();
	  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	  query->bindValue(":type", type);
	  query->exec();
	  query->first();
	  QString direction = query->value(0).toString();
	  delete query;
	  QSet<int> markOne;
	  QSet<int> markTwo;
	  findHeads(&markOne, &headsMap, tail);
	  findTails(&markTwo, &tailsMap, tail);
	  QSetIterator<int> it2(markOne);
	  while (it2.hasNext()) 
	    {
	      int currentPathMate = it2.next();
	      QSet<int> currentHeads = headsMap.value(currentPathMate);
	      if (currentHeads.contains(head))
		{
		  linkage->setPenStyle(2);
		}
	    }
	  QSetIterator<int> it3(markTwo);
	  while (it3.hasNext()) 
	    {
	      int currentPathMate = it3.next();
	      QSet<int> currentHeads = headsMap.value(currentPathMate);
	      if (currentHeads.contains(head))
		{
		  linkage->setPenStyle(2);
		}
	    }
	}
    }
}

void EventGraphWidget::rejectLinkage() 
{
  if (scene->selectedItems().size() == 1) 
    {
      Linkage *linkage = qgraphicsitem_cast<Linkage*>(scene->selectedItems().first());
      if (linkage) 
	{
	  delete linkage;
	  _compareVector.removeOne(linkage);
	}
    }
}

void EventGraphWidget::addLineObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  _lineVector.push_back(newLineObject);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_incidentNodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void EventGraphWidget::addSingleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_incidentNodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void EventGraphWidget::addDoubleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setArrow2(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_incidentNodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void EventGraphWidget::addEllipseObject(const QRectF &area)
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
  newEllipse->setZValue(_incidentNodeVector[0]->zValue() + 2);
  newEllipse->setSelected(true);
}

void EventGraphWidget::addRectObject(const QRectF &area) 
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
  newRect->setZValue(_incidentNodeVector[0]->zValue() + 2);
  newRect->setSelected(true);
}

void EventGraphWidget::addTextObject(const QRectF &area, const qreal &size)
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
      newText->setZValue(_incidentNodeVector[0]->zValue() + 3);
      newText->adjustSize();
      newText->setSelected(true);
    }
  delete textDialog;
}

void EventGraphWidget::addTimeLineObject(const qreal &startX, const qreal &endX, const qreal &y)
{
  TimeLineObject *newTimeLine = new TimeLineObject(startX, endX, y,
						   _currentMajorInterval,
						   _currentMinorDivision,
						   _currentMajorTickSize,
						   _currentMinorTickSize);
  _timeLineVector.push_back(newTimeLine);
  newTimeLine->setPenWidth(_currentTimeLineWidth);
  newTimeLine->setColor(_currentTimeLineColor);
  scene->addItem(newTimeLine);
  newTimeLine->setZValue(_incidentNodeVector[0]->zValue() + 2);
  newTimeLine->setSelected(true);
}

void EventGraphWidget::addHorizontalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(true);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void EventGraphWidget::addVerticalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(false);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void EventGraphWidget::toggleSnapGuides()
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

void EventGraphWidget::setPenStyle()
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

void EventGraphWidget::setPenWidth()
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

void EventGraphWidget::setLineColor()
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

void EventGraphWidget::setFillColor()
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

void EventGraphWidget::setFillOpacity(int value)
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

void EventGraphWidget::setMajorIntervalBySlider()
{
  _currentMajorInterval = majorIntervalSlider->value();
  majorIntervalSpinBox->blockSignals(true);
  majorIntervalSpinBox->setValue(_currentMajorInterval);
  majorIntervalSpinBox->blockSignals(false);
  emit sendMajorInterval(_currentMajorInterval);
  if (scene->selectedItems().size() == 1)
     {
       QGraphicsItem *selectedItem = scene->selectedItems().first();
       TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
       if (timeline)
	 {
	   timeline->setMajorTickInterval(_currentMajorInterval);
	 }
     }
}

void EventGraphWidget::setMajorIntervalBySpinBox()
{
  _currentMajorInterval = majorIntervalSpinBox->value();
  majorIntervalSlider->blockSignals(true);
  majorIntervalSlider->setValue(_currentMajorInterval);
  majorIntervalSlider->blockSignals(false);
  emit sendMajorInterval(_currentMajorInterval);
  if (scene->selectedItems().size() == 1)
     {
       QGraphicsItem *selectedItem = scene->selectedItems().first();
       TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
       if (timeline)
	 {
	   timeline->setMajorTickInterval(_currentMajorInterval);
	 }
     }
}

void EventGraphWidget::setMinorDivisionBySlider()
{
  _currentMinorDivision = minorDivisionSlider->value();
  minorDivisionSpinBox->blockSignals(true);
  minorDivisionSpinBox->setValue(_currentMinorDivision);
  minorDivisionSpinBox->blockSignals(false);
  emit sendMinorDivision(_currentMinorDivision);
   if (scene->selectedItems().size() == 1)
     {
       QGraphicsItem *selectedItem = scene->selectedItems().first();
       TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
       if (timeline)
	 {
	   timeline->setMinorTickDivision(_currentMinorDivision);
	 }
     }
}

void EventGraphWidget::setMinorDivisionBySpinBox()
{
  _currentMinorDivision = minorDivisionSpinBox->value();
  minorDivisionSlider->blockSignals(true);
  minorDivisionSlider->setValue(_currentMinorDivision);
  minorDivisionSlider->blockSignals(false);
  emit sendMinorDivision(_currentMinorDivision);
   if (scene->selectedItems().size() == 1)
     {
       QGraphicsItem *selectedItem = scene->selectedItems().first();
       TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
       if (timeline)
	 {
	   timeline->setMinorTickDivision(_currentMinorDivision);
	 }
     }
}

void EventGraphWidget::setMajorTickSize()
{
  _currentMajorTickSize = majorTickSizeSlider->value();
  emit sendMajorTickSize(_currentMajorTickSize);
  if (scene->selectedItems().size() == 1)
     {
       QGraphicsItem *selectedItem = scene->selectedItems().first();
       TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
       if (timeline)
	 {
	   timeline->setMajorTickSize(_currentMajorTickSize);
	 }
     }
}

void EventGraphWidget::setMinorTickSize()
{
  _currentMinorTickSize = minorTickSizeSlider->value();
  emit sendMinorTickSize(_currentMinorTickSize);
   if (scene->selectedItems().size() == 1)
     {
       QGraphicsItem *selectedItem = scene->selectedItems().first();
       TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
       if (timeline)
	 {
	   timeline->setMinorTickSize(_currentMinorTickSize);
	 }
     }
}

void EventGraphWidget::setTimeLineWidth()
{
  _currentTimeLineWidth = timeLineWidthSpinBox->value();
 emit sendTimeLineWidth(_currentTimeLineWidth);
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
      if (timeline)
	{
	  timeline->setPenWidth(_currentTimeLineWidth);
	}
    }
}

void EventGraphWidget::setTimeLineColor()
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setCurrentColor(_currentTimeLineColor);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _currentTimeLineColor = colorDialog->selectedColor();
      emit sendTimeLineColor(_currentTimeLineColor);
      QPixmap timeLineColorMap(20, 20);
      timeLineColorMap.fill(_currentTimeLineColor);
      QIcon timeLineColorIcon(timeLineColorMap);
      changeTimeLineColorButton->setIcon(timeLineColorIcon);
    }
  delete colorDialog;
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
      if (timeline)
	{
	  timeline->setColor(_currentTimeLineColor);
	}
    }
}

void EventGraphWidget::hideAnnotations()
{
  QVectorIterator<EllipseObject*> it(_ellipseVector);
  QVectorIterator<RectObject*> it2(_rectVector);
  QVectorIterator<LineObject*> it3(_lineVector);
  QVectorIterator<TextObject*> it4(_textVector);
  QVectorIterator<TimeLineObject*> it5(_timeLineVector);
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
      while (it5.hasNext())
	{
	  it5.next()->hide();
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
      while (it5.hasNext())
	{
	  it5.next()->show();
	}
    }
}

void EventGraphWidget::processShapeSelection()
{
  if (scene->selectedItems().size() == 1)
    {
      QGraphicsItem *selectedItem = scene->selectedItems().first();
      LineObject *line = qgraphicsitem_cast<LineObject*>(selectedItem);
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(selectedItem);
      RectObject *rect = qgraphicsitem_cast<RectObject*>(selectedItem);
      TextObject *text = qgraphicsitem_cast<TextObject*>(selectedItem);
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(selectedItem);
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
      else if (timeline)
	{
	  _currentMajorInterval = timeline->getMajorTickInterval();
	  emit sendMajorInterval(_currentMajorInterval);
	  _currentMinorDivision = timeline->getMinorTickDivision();
	  emit sendMinorDivision(_currentMinorDivision);
	  _currentMajorTickSize = timeline->getMajorTickSize();
	  emit sendMajorTickSize(_currentMajorTickSize);
	  _currentMinorTickSize = timeline->getMinorTickSize();
	  emit sendMinorTickSize(_currentMinorTickSize);
	  _currentTimeLineWidth = timeline->getPenWidth();
	  emit sendTimeLineWidth(_currentTimeLineWidth);
	  timeLineWidthSpinBox->setValue(_currentTimeLineWidth);
	  majorIntervalSlider->blockSignals(true);
	  majorIntervalSlider->setValue(_currentMajorInterval);
	  majorIntervalSlider->blockSignals(false);
	  majorIntervalSpinBox->blockSignals(true);
	  majorIntervalSpinBox->setValue(_currentMajorInterval);
	  majorIntervalSpinBox->blockSignals(false);
	  minorDivisionSlider->blockSignals(true);
	  minorDivisionSlider->setValue(_currentMinorDivision);
	  minorDivisionSlider->blockSignals(false);
	  minorDivisionSpinBox->blockSignals(true);
	  minorDivisionSpinBox->setValue(_currentMinorDivision);
	  minorDivisionSpinBox->blockSignals(false);
	  majorTickSizeSlider->blockSignals(true);
	  majorTickSizeSlider->setValue(_currentMajorTickSize);
	  majorTickSizeSlider->blockSignals(false);
	  minorTickSizeSlider->blockSignals(true);
	  minorTickSizeSlider->setValue(_currentMinorTickSize);
	  minorTickSizeSlider->blockSignals(false);
	  _currentTimeLineWidth = timeline->getPenWidth();
	  emit sendTimeLineWidth(_currentTimeLineWidth);
	  _currentTimeLineColor = timeline->getColor();
	  emit sendTimeLineColor(_currentTimeLineColor);
	  QPixmap timeLineColorMap(20, 20);
	  timeLineColorMap.fill(_currentTimeLineColor);
	  QIcon timeLineColorIcon(timeLineColorMap);
	  changeTimeLineColorButton->setIcon(timeLineColorIcon);
	}
    }
}

void EventGraphWidget::processLineContextMenu(const QString &action) 
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
	  _lineVector.removeOne(line);
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

void EventGraphWidget::processTextContextMenu(const QString &action) 
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
	  setChangeLabel();
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
	  _textVector.removeOne(text);
	  setChangeLabel();
	}
    }
}

void EventGraphWidget::copyDescriptionToText() 
{
  if (scene->selectedItems().size() == 1) 
    {
      IncidentNode *incidentNode = qgraphicsitem_cast<IncidentNode *>(scene->selectedItems().first());
      AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(scene->selectedItems().first());
      if (incidentNode) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT description FROM incidents WHERE id = :id");
	  query->bindValue(":id", incidentNode->getId());
	  query->exec();
	  query->first();
	  QString text = query->value(0).toString();
	  delete query;
	  TextObject *newText = new TextObject(text);
	  _textVector.push_back(newText);
	  scene->addItem(newText);
	  QPointF pos = incidentNode->scenePos();
	  pos.setY(pos.y() - 80);
	  newText->setPos(pos);
	  newText->setZValue(_incidentNodeVector[0]->zValue() + 3);
	  newText->adjustSize();
	  setChangeLabel();
	}
      else if (abstractNode) 
	{
	  QString text = abstractNode->getDescription();
	  TextObject *newText = new TextObject(text);
	  _textVector.push_back(newText);
	  scene->addItem(newText);
	  QPointF pos = abstractNode->scenePos();
	  pos.setY(pos.y() - 80);
	  newText->setPos(pos);
	  newText->setZValue(_incidentNodeVector[0]->zValue() + 3);
	  newText->adjustSize();
	  setChangeLabel();
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
	      setChangeLabel();
	    }
	  delete textDialog;
	}
    }
}

void EventGraphWidget::processEllipseContextMenu(const QString &action) 
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

void EventGraphWidget::deleteEllipse() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  delete ellipse;
	  _ellipseVector.removeOne(ellipse);
	  setChangeLabel();
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
	  _ellipseVector.push_back(newEllipse);
	  newEllipse->setZValue(ellipse->zValue());
	  scene->addItem(newEllipse);
	  QPointF pos = ellipse->mapToScene(ellipse->getCenter());
	  newEllipse->moveCenter(newEllipse->mapFromScene(pos));
	  setChangeLabel();
	}
    }
}

void EventGraphWidget::processRectContextMenu(const QString &action) 
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

void EventGraphWidget::deleteRect() 
{
  if (scene->selectedItems().size() == 1) 
    {
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      if (rect) 
	{
	  delete rect;
	  _rectVector.removeOne(rect);
	  setChangeLabel();
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
	  _rectVector.push_back(newRect);
	  newRect->setZValue(rect->zValue());
	  scene->addItem(newRect);
	  QPointF pos = rect->mapToScene(rect->getCenter());
	  newRect->moveCenter(newRect->mapFromScene(pos));
	  setChangeLabel();
	}
    }
}

void EventGraphWidget::processTimeLineContextMenu(const QString &action) 
{
  if (action == TOGGLEFIRSTTICK)
    {
      timeLineToggleFirstTick();
    }
  else if (action == FORCELASTTICK)
    {
      timeLineToggleForceLastTick();
    }
  else if (action == DELETETIMELINE) 
    {
      deleteTimeLine();
    }
  else if (action == COPYOBJECT) 
    {
      duplicateTimeLine();
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

void EventGraphWidget::timeLineToggleFirstTick()
{
 if (scene->selectedItems().size() == 1) 
    {
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
      if (timeline) 
	{
	  bool state = timeline->getFirstTick();
	  state = !state;
	  timeline->setFirstTick(state);
	  setChangeLabel();
	}
    }  
}

void EventGraphWidget::timeLineToggleForceLastTick()
{
 if (scene->selectedItems().size() == 1) 
    {
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
      if (timeline) 
	{
	  bool state = timeline->getForceLastTick();
	  state = !state;
	  timeline->setForceLastTick(state);
	  setChangeLabel();
	}
    }  
}

void EventGraphWidget::deleteTimeLine() 
{
  if (scene->selectedItems().size() == 1) 
    {
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
      if (timeline) 
	{
	  delete timeline;
	  _timeLineVector.removeOne(timeline);
	  setChangeLabel();
	}
    }  
}

void EventGraphWidget::duplicateTimeLine() 
{
  if (scene->selectedItems().size() == 1) 
    {
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
      if (timeline) 
	{
	  TimeLineObject *newTimeLine = new TimeLineObject();
	  newTimeLine->setStartX(timeline->getStartX());
	  newTimeLine->setEndX(timeline->getEndX());
	  newTimeLine->setY(timeline->getY());
	  newTimeLine->setMajorTickInterval(timeline->getMajorTickInterval());
	  newTimeLine->setMinorTickDivision(timeline->getMinorTickDivision());
	  newTimeLine->setMajorTickSize(timeline->getMajorTickSize());
	  newTimeLine->setMinorTickSize(timeline->getMinorTickSize());
	  newTimeLine->setPenWidth(timeline->getPenWidth());
	  newTimeLine->setColor(timeline->getColor());
	  _timeLineVector.push_back(newTimeLine);
	  newTimeLine->setZValue(timeline->zValue());
	  scene->addItem(newTimeLine);
	  setChangeLabel();
	}
    }
}

void EventGraphWidget::processGuideLineContextMenu(const QString &action) 
{
  if (action == DELETEGUIDEACTION) 
    {
      deleteGuideLine();
      setChangeLabel();
    }
}

void EventGraphWidget::deleteGuideLine()
{
 if (scene->selectedItems().size() == 1) 
    {
      GuideLine *guide = qgraphicsitem_cast<GuideLine*>(scene->selectedItems().first());
      if (guide) 
	{
	  delete guide;
	  _guidesVector.removeOne(guide);
	  setChangeLabel();
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
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
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
      else if (timeline)
	{
	  int currentZValue = timeline->zValue();
	  if (currentZValue < maxZ + 1) 
	    {
	      timeline->setZValue(currentZValue + 1);
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
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
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
      else if (timeline) 
	{
	  int currentZValue = timeline->zValue();
	  if (currentZValue > 1) 
	    {
	      timeline->setZValue(currentZValue - 1);
	      if (timeline->zValue() == 1) 
		{
		  QListIterator<QGraphicsItem*> it(scene->items());
		  while (it.hasNext()) 
		    {
		      QGraphicsItem *current = it.next();
		      if (current !=  timeline) 
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
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
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
      else if (timeline) 
	{
	  timeline->setZValue(maxZ + 1);
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
      TimeLineObject *timeline = qgraphicsitem_cast<TimeLineObject*>(scene->selectedItems().first());
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
      else if (timeline) 
	{
	  timeline->setZValue(1);
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
  if (maxZ > _incidentNodeVector[0]->zValue())
    {
      for (int i = _incidentNodeVector[0]->zValue(); i != maxZ; i++) 
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
  setChangeLabel();  
}

QVector<AbstractNode*> EventGraphWidget::getAbstractNodes() 
{
  return _abstractNodeVector;
}

QVector<IncidentNode *> EventGraphWidget::getIncidentNodes() 
{
  return _incidentNodeVector;
}

QVector<Linkage*> EventGraphWidget::getEdgeVector() 
{
  return _edgeVector;
}

void EventGraphWidget::setAttributesWidget(AttributesWidget *attributesWidgetPtr) 
{
  _attributesWidgetPtr = attributesWidgetPtr;
}


void EventGraphWidget::setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr) 
{
  _relationshipsWidgetPtr = relationshipsWidgetPtr;
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

int EventGraphWidget::getLabelSize()
{
  return _labelSize;
}

void EventGraphWidget::setCurrentCoder(QString coder)
{
  _selectedCoder = coder;
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
  else if (object == attributesTreeView)
    {
      if (event->type() == QEvent::ChildRemoved) 
	{
	  fixTree();
	}
      else if (event->type() == QEvent::Wheel)
	{
	  QWheelEvent *wheelEvent = (QWheelEvent*) event;
	  if(wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->angleDelta().y() > 0) 
		{
		  changeTreeFontSize(attributesTree, QModelIndex(), 1);
		}
	      else if (wheelEvent->angleDelta().y() < 0) 
		{
		  changeTreeFontSize(attributesTree, QModelIndex(), -1);
		}
	    }
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

