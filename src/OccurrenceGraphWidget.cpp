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

#include "../include/OccurrenceGraphWidget.h"

/*
  Notes:
  I want to fetch the abstractNode events from saved event graph plots later,
  and then group occurrence in the events that are visible.
*/

OccurrenceGraphWidget::OccurrenceGraphWidget(QWidget *parent) : QWidget(parent) 
{
  _distance = 70.0;
  _labelsVisible = true;
  _incidentLabelsOnly = false;
  _attributeLabelsOnly = false;
  _matched = false;
  _labelSize = 10;
  _currentPenStyle = 1;
  _currentPenWidth = 1;
  _currentLineColor = QColor(Qt::black);
  _currentFillColor = QColor(Qt::transparent);
  _currentMajorInterval = 100.0;
  _currentMinorDivision = 2.0;
  _currentMajorTickSize = 20.0;
  _currentMinorTickSize = 10.0;
  _currentTimeLineWidth = 1;
  _currentLineColor = QColor(Qt::black);  _currentFillColor = QColor(Qt::black);
  _currentFillColor.setAlpha(0);
  _currentTimeLineColor = QColor(Qt::black);
  
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

  legendWidget = new QWidget(this);
  graphicsWidget = new QWidget(this);
  timeLineWidget = new QWidget(this);
  
  attributeListWidget = new DeselectableListWidget(legendWidget);
  attributeListWidget->setColumnCount(2);
  attributeListWidget->horizontalHeader()->hide();
  attributeListWidget->verticalHeader()->hide();
  attributeListWidget->setColumnWidth(1, 20);
  attributeListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  attributeListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  attributeListWidget->setStyleSheet("QTableView {gridline-color: black}");
  attributeListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

  relationshipListWidget = new DeselectableListWidget(legendWidget);
  relationshipListWidget->setColumnCount(2);
  relationshipListWidget->horizontalHeader()->hide();
  relationshipListWidget->verticalHeader()->hide();
  relationshipListWidget->setColumnWidth(1, 20);
  relationshipListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  relationshipListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  relationshipListWidget->setStyleSheet("QTableView {gridline-color: black}");
  relationshipListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

  caseListWidget = new QListWidget(graphicsWidget);
  caseListWidget->setEnabled(false);
  
  attributeLegendLabel = new QLabel(tr("<b>Attributes:</b>"), this);
  relationshipLegendLabel = new QLabel(tr("<b>Relationships:</b>"), this);
  plotLabel = new QLabel(tr("Unsaved plot"), this);
  changeLabel = new QLabel(tr("*"), this);
  incongruenceLabel = new QLabel(tr(""), this);
  incongruenceLabel->setStyleSheet("QLabel {color : red;}");
  casesLabel = new QLabel(tr("<b>Case filtering:</b>"), graphicsWidget);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
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
  fillOpacityLabel = new QLabel(tr("<b>Opacity:</b>"), this);
  guideLinesLabel = new QLabel(tr("<b>Add guides:</b>"), this);
  labelSizeLabel = new QLabel(tr("<b>Label size:</b>"), graphicsWidget);
  layoutLabel = new QLabel(tr("<b>Layout:</b>"), this);
  
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
  
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  toggleLegendButton->setChecked(true);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  toggleTimeLineButton = new QPushButton(tr("Toggle timeline controls"), this);
  toggleTimeLineButton->setCheckable(true);
  addAttributeButton = new QPushButton(tr("Add single attribute"), legendWidget);
  addAttributesButton = new QPushButton(tr("Add multiple attributes"), legendWidget);
  removeAttributeModeButton = new QPushButton(tr("Remove attribute"), legendWidget);
  removeAttributeModeButton->setEnabled(false);
  addRelationshipButton = new QPushButton(tr("Add single relationship"), legendWidget);
  addRelationshipsButton = new QPushButton(tr("Add multiple relationships"), legendWidget);
  removeRelationshipModeButton = new QPushButton(tr("Remove relationship"), legendWidget);
  removeRelationshipModeButton->setEnabled(false);
  makeLayoutButton = new QPushButton(tr("Run layout"), this);
  plotLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  incidentLabelsOnlyButton = new QPushButton(tr("Incident labels only"), graphicsWidget);
  incidentLabelsOnlyButton->setCheckable(true);
  attributeLabelsOnlyButton = new QPushButton(tr("Attribute labels only"), graphicsWidget);
  attributeLabelsOnlyButton->setCheckable(true);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  increaseDistanceButton = new QPushButton(QIcon(":/images/expand_horizontal.png"), "", this);
  increaseDistanceButton->setIconSize(QSize(20, 20));
  increaseDistanceButton->setMinimumSize(40, 40);
  increaseDistanceButton->setMaximumSize(40, 40);
  decreaseDistanceButton = new QPushButton(QIcon(":/images/contract_horizontal.png"), "", this);
  decreaseDistanceButton->setIconSize(QSize(20, 20));
  decreaseDistanceButton->setMinimumSize(40, 40);
  decreaseDistanceButton->setMaximumSize(40, 40);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  viewConcordanceButton = new QPushButton(tr("View line plot"), graphicsWidget); 
  exportMatrixButton = new QPushButton(tr("Export matrix"), graphicsWidget);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  clearPlotButton = new QPushButton(tr("Clear plot"), this);
  clearPlotButton->setEnabled(false);
  increaseLabelSizeButton = new QPushButton("+", this);
  decreaseLabelSizeButton = new QPushButton("-", this);
  setTimeRangeButton = new QPushButton(tr("Set time range"), graphicsWidget);
  
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

  layoutComboBox = new QComboBox(this);
  layoutComboBox->addItem(REDOLAYOUT);
  layoutComboBox->addItem(MATCHEVENTGRAPH);
  layoutComboBox->addItem(DATELAYOUTDAYS);
  layoutComboBox->addItem(DATELAYOUTMONTHS);
  layoutComboBox->addItem(DATELAYOUTYEARS);
  layoutComboBox->addItem(DATELAYOUTDECADES);
  layoutComboBox->addItem(REDUCEOVERLAP);
  
  view->viewport()->installEventFilter(this);
  
  // connections
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(toggleTimeLineButton, SIGNAL(clicked()), this, SLOT(toggleTimeLine()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(addAttributesButton, SIGNAL(clicked()), this, SLOT(addAttributes()));
  connect(addRelationshipButton, SIGNAL(clicked()), this, SLOT(addRelationship()));
  connect(addRelationshipsButton, SIGNAL(clicked()), this, SLOT(addRelationships()));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(processZoomSliderChange(int)));
  connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(resetZoomSlider()));
  connect(attributeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setAttributeModeButton(QTableWidgetItem *)));
  connect(attributeListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableAttributeModeButton()));
  connect(attributeListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeAttributeModeColor(QTableWidgetItem *)));
  connect(relationshipListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setRelationshipModeButton(QTableWidgetItem *)));
  connect(relationshipListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableRelationshipModeButton()));
  connect(relationshipListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
	  this, SLOT(changeRelationshipModeColor(QTableWidgetItem *)));
  connect(removeAttributeModeButton, SIGNAL(clicked()), this, SLOT(removeAttributeMode()));
  connect(removeRelationshipModeButton, SIGNAL(clicked()), this, SLOT(removeRelationshipMode()));
  connect(makeLayoutButton, SIGNAL(clicked()), this, SLOT(makeLayout()));
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
  connect(addTimeLineButton, SIGNAL(clicked()), scene, SLOT(prepTimeLinePoints()));
  connect(hideAnnotationsButton, SIGNAL(clicked()), this, SLOT(hideAnnotations()));
  connect(this, SIGNAL(sendLineColor(QColor &)), scene, SLOT(setLineColor(QColor &)));
  connect(this, SIGNAL(sendFillColor(QColor &)), scene, SLOT(setFillColor(QColor &)));
  connect(majorIntervalSlider, SIGNAL(valueChanged(int)), this, SLOT(setMajorIntervalBySlider()));
  connect(majorIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setMajorIntervalBySpinBox()));
  connect(minorDivisionSlider, SIGNAL(valueChanged(int)), this, SLOT(setMinorDivisionBySlider()));
  connect(minorDivisionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setMinorDivisionBySpinBox()));
  connect(majorTickSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setMajorTickSize()));
  connect(minorTickSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setMinorTickSize()));
  connect(changeTimeLineColorButton, SIGNAL(clicked()), this, SLOT(setTimeLineColor()));
  connect(timeLineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setTimeLineWidth()));
  connect(timeLineWidthSpinBox, SIGNAL(valueChanged(int)), scene, SLOT(setTimeLineWidth(int)));
  connect(addHorizontalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepHorizontalGuideLine()));
  connect(addVerticalGuideLineButton, SIGNAL(clicked()), scene, SLOT(prepVerticalGuideLine()));
  connect(snapGuidesButton, SIGNAL(clicked()), this, SLOT(toggleSnapGuides()));
  connect(this, SIGNAL(sendMajorInterval(qreal &)), scene, SLOT(setMajorInterval(qreal &)));
  connect(this, SIGNAL(sendMinorDivision(qreal &)), scene, SLOT(setMinorDivision(qreal &)));
  connect(this, SIGNAL(sendMajorTickSize(qreal &)), scene, SLOT(setMajorTickSize(qreal &)));
  connect(this, SIGNAL(sendMinorTickSize(qreal &)), scene, SLOT(setMinorTickSize(qreal &)));
  connect(this, SIGNAL(sendTimeLineWidth(int)), scene, SLOT(setTimeLineWidth(int)));
  connect(this, SIGNAL(sendTimeLineColor(QColor &)), scene, SLOT(setTimeLineColor(QColor &)));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(updateLinkages()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(scene, SIGNAL(moveLine(QGraphicsItem *, QPointF)),
	  this, SLOT(processMoveLine(QGraphicsItem *, QPointF)));
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
  connect(scene, SIGNAL(sendLinePoints(const QPointF&, const QPointF&)),
	  this, SLOT(addLineObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(sendSingleArrowPoints(const QPointF&, const QPointF&)),
	  this, SLOT(addSingleArrowObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(sendDoubleArrowPoints(const QPointF&, const QPointF&)),
	  this, SLOT(addDoubleArrowObject(const QPointF&, const QPointF&)));
  connect(scene, SIGNAL(GuideLineContextMenuAction(const QString &)),
	  this, SLOT(processGuideLineContextMenu(const QString &)));
  connect(scene, SIGNAL(sendEllipseArea(const QRectF&)), this, SLOT(addEllipseObject(const QRectF&)));
  connect(scene, SIGNAL(sendRectArea(const QRectF&)), this, SLOT(addRectObject(const QRectF&)));
  connect(scene, SIGNAL(sendTextArea(const QRectF&, const qreal&)),
	  this, SLOT(addTextObject(const QRectF&, const qreal&)));
  connect(scene, SIGNAL(sendTimeLinePoints(const qreal&, const qreal&, const qreal&)),
	  this, SLOT(addTimeLineObject(const qreal&, const qreal&, const qreal&)));
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(processShapeSelection()));
  connect(scene, SIGNAL(sendHorizontalGuideLinePos(const QPointF&)),
	  this, SLOT(addHorizontalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(sendVerticalGuideLinePos(const QPointF&)),
	  this, SLOT(addVerticalGuideLine(const QPointF&)));
  connect(scene, SIGNAL(OccurrenceItemContextMenuAction(QGraphicsItem*, const QString &)),
	  this, SLOT(processOccurrenceItemContextMenu(QGraphicsItem*, const QString &)));
  connect(caseListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(checkCases()));
  connect(plotLabelsButton, SIGNAL(clicked()), this, SLOT(plotLabels()));
  connect(incidentLabelsOnlyButton, SIGNAL(clicked()), this, SLOT(toggleIncidentLabelsOnly()));
  connect(attributeLabelsOnlyButton, SIGNAL(clicked()), this, SLOT(toggleAttributeLabelsOnly()));
  connect(increaseLabelSizeButton, SIGNAL(clicked()), this, SLOT(increaseLabelSize()));
  connect(decreaseLabelSizeButton, SIGNAL(clicked()), this, SLOT(decreaseLabelSize()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(setTimeRangeButton, SIGNAL(clicked()), this, SLOT(setTimeRange()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(viewConcordanceButton, SIGNAL(clicked()), this, SLOT(viewConcordancePlot()));  
  connect(exportMatrixButton, SIGNAL(clicked()), this, SLOT(exportMatrix()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(clearPlotButton, SIGNAL(clicked()), this, SLOT(clearPlot()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
    
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(savePlotButton);
  plotOptionsLayout->addWidget(seePlotsButton);
  plotOptionsLayout->addWidget(plotLabel);
  plotOptionsLayout->addWidget(changeLabel);
  plotOptionsLayout->addWidget(incongruenceLabel);
  plotOptionsLayout->addSpacerItem(new QSpacerItem(100,0));
  plotOptionsLayout->setAlignment(Qt::AlignLeft);
  topLayout->addLayout(plotOptionsLayout);

  QPointer<QHBoxLayout> clearLayout = new QHBoxLayout;
  clearLayout->addWidget(clearPlotButton);
  clearLayout->setAlignment(Qt::AlignRight);
  topLayout->addLayout(clearLayout);
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

  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  leftLayout->addWidget(view);
  view->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
  screenLayout->addLayout(leftLayout);

  QPointer<QVBoxLayout> legendLayout = new QVBoxLayout;
  legendLayout->addWidget(attributeLegendLabel);
  legendLayout->addWidget(attributeListWidget);
  legendLayout->addWidget(addAttributeButton);
  legendLayout->addWidget(addAttributesButton);
  legendLayout->addWidget(removeAttributeModeButton);
  legendLayout->addWidget(relationshipLegendLabel);
  legendLayout->addWidget(relationshipListWidget);
  legendLayout->addWidget(addRelationshipButton);
  legendLayout->addWidget(addRelationshipsButton);
  legendLayout->addWidget(removeRelationshipModeButton);
  legendWidget->setMinimumWidth(250);
  legendWidget->setMaximumWidth(250);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				   
  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(casesLabel);
  graphicsControlsLayout->addWidget(caseListWidget);
  graphicsControlsLayout->addWidget(backgroundColorButton);
  graphicsControlsLayout->addWidget(plotLabelsButton);
  graphicsControlsLayout->addWidget(incidentLabelsOnlyButton);
  graphicsControlsLayout->addWidget(attributeLabelsOnlyButton);
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
  graphicsControlsLayout->addWidget(viewConcordanceButton);
  graphicsControlsLayout->addWidget(exportMatrixButton);
  graphicsWidget->setMaximumWidth(200);
  graphicsWidget->setMinimumWidth(200);
  graphicsWidget->setLayout(graphicsControlsLayout);
  graphicsControlsLayout->setAlignment(Qt::AlignBottom);
  screenLayout->addWidget(graphicsWidget);

  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
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
  QPointer<QFrame> vertLineOne = new QFrame();
  vertLineOne->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineOne);
  drawOptionsLeftLayout->addWidget(zoomLabel);
  zoomLabel->setMaximumWidth(zoomLabel->sizeHint().width());
  drawOptionsLeftLayout->addWidget(zoomSlider);
  zoomSlider->setMaximumWidth(100);
  QPointer<QFrame> vertLineTwo = new QFrame();
  vertLineTwo->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineTwo);
  QPointer<QHBoxLayout> guidesLayout = new QHBoxLayout;
  guidesLayout->addWidget(guideLinesLabel);
  guidesLayout->addWidget(addHorizontalGuideLineButton);
  guidesLayout->addWidget(addVerticalGuideLineButton);
  guidesLayout->addWidget(snapGuidesButton);
  guidesLayout->setAlignment(Qt::AlignLeft);
  drawOptionsLeftLayout->addLayout(guidesLayout);
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);
  QPointer<QFrame> vertLineThree = new QFrame();
  vertLineThree->setFrameShape(QFrame::VLine);
  drawOptionsLeftLayout->addWidget(vertLineThree);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);

  setLayout(mainLayout);
  graphicsWidget->hide();
  timeLineWidget->hide();
  updateCases();
  setGraphControls(false);
}

OccurrenceGraphWidget::~OccurrenceGraphWidget()
{
  qDeleteAll(_attributeOccurrenceVector);
  _attributeOccurrenceVector.clear();
  qDeleteAll(_relationshipOccurrenceVector);
  _relationshipOccurrenceVector.clear();
  qDeleteAll(_attributeLabelVector);
  _attributeLabelVector.clear();
  qDeleteAll(_relationshipLabelVector);
  _relationshipLabelVector.clear();
  qDeleteAll(_edgeVector);
  _edgeVector.clear();
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
  delete scene;
  delete view;
}

void OccurrenceGraphWidget::setOpenGL(bool state)
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

void OccurrenceGraphWidget::setAntialiasing(bool state)
{
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext())
    {
      Linkage *current = it.next();
      current->setAntialiasing(state);
    }
}

void OccurrenceGraphWidget::checkCongruence() 
{
  qApp->setOverrideCursor(Qt::WaitCursor);
  if (_attributeOccurrenceVector.size() > 0) 
    {
      QSqlDatabase::database().transaction();
      QMap<QString, QVector<int>> attributeMap;
      QVector<QPair<int, int>> orderPairs;
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT name FROM entities "
		     "WHERE name = :name");
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT incident FROM attributes_to_incidents "
		      "WHERE attribute = :attribute");
      for (int i = 0; i != attributeListWidget->rowCount(); i++) 
	{
	  QString attribute = attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
	  QVector<int> attributeIncidents;
	  bool entity = false;
	  query->bindValue(":name", attribute);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      entity = true;
	    }
	  QVector<QString> attributeVector;
	  attributeVector.push_back(attribute);
	  findChildren(attribute, &attributeVector, entity);
	  QVectorIterator<QString> it(attributeVector);
	  while (it.hasNext()) 
	    {
	      QString currentAttribute = it.next();
	      query2->bindValue(":attribute", currentAttribute);
	      query2->exec();
	      while (query2->next())
		{
		  attributeIncidents.push_back(query2->value(0).toInt());
		}
	    }
	  attributeMap.insert(attribute, attributeIncidents);
	}
      query->exec("SELECT id, ch_order FROM incidents");
      while (query->next())
	{
	  int incident = query->value(0).toInt();
	  int order = query->value(1).toInt();
	  orderPairs.push_back(QPair<int, int>(incident, order));
	}
      QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem* current = it.next();
	  int id = current->getId();
	  if (id >= 0) 
	    {
	      int order = current->getOrder();
	      QString attribute = current->getAttribute();
	      QVector<int> attributeIncidents = attributeMap.value(attribute);
	      if (!attributeIncidents.contains(id))
		{
		  incongruenceLabel->setText("Incongruence detected");
		  qDebug() << "1";
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	      QPair<int, int> currentPair = QPair<int, int>(id, order);
	      if (!orderPairs.contains(currentPair))
		{
		  incongruenceLabel->setText("Incongruence detected");
		  qDebug() << "2";
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	    }
	}
      query->prepare("SELECT name FROM entities "
		     "WHERE name = :name");
      QSqlQuery *query3 = new QSqlQuery;
      query3->prepare("SELECT incident FROM attributes_to_incidents "
		      "WHERE attribute = :attribute");
      for (int i = 0; i != attributeListWidget->rowCount(); i++) 
	{
	  QString attribute = attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
	  QSet<int> expected;
	  bool entity = false;
	  query->bindValue(":name", attribute);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      entity = true;
	    }
	  QVector<QString> attributeVector;
	  attributeVector.push_back(attribute);
	  findChildren(attribute, &attributeVector, entity);
	  QVectorIterator<QString> it2(attributeVector);
	  while (it2.hasNext())
	    {
	      QString currentAttribute = it2.next();
	      query3->bindValue(":attribute", currentAttribute);
	      query3->exec();
	      while (query3->next()) 
		{
		  int current = query3->value(0).toInt();
		  expected.insert(current);
		}
	    }
	  QSet<int> observed;
	  QVectorIterator<OccurrenceItem*> it3(_attributeOccurrenceVector);
	  while (it3.hasNext()) 
	    {
	      OccurrenceItem *item = it3.next();
	      if (item->getAttribute() == attribute) 
		{
		  observed.insert(item->getId());
		}
	    }
	  QList<int> expectedList = expected.toList();
	  QListIterator<int> it4(expectedList);
	  while (it4.hasNext())
	    {
	      int currentExpected = it4.next();
	      if (!observed.contains(currentExpected))
		{
		  incongruenceLabel->setText("Incongruence detected");
		  qDebug() << "3";
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  delete query3;
		  return;
		}
	    }
	}
      delete query;
      delete query2;
      delete query3;
      QSqlDatabase::database().commit();
    } 
  if (_relationshipOccurrenceVector.size() > 0) 
    {
      QSqlDatabase::database().transaction();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT relationship, type, incident FROM relationships_to_incidents "
		     "WHERE relationship = :relationship AND type = :type AND incident = :incident");
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents "
		      "WHERE id = :incident");
      QVectorIterator<OccurrenceItem*> it(_relationshipOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem* current = it.next();
	  int id = current->getId();
	  if (id >= 0) 
	    {
	      int order = current->getOrder();
	      QString combi = current->getAttribute();
	      QString relationship = "";
	      QString type = "";
	      bool typePart = false;
	      bool undirected = false;
	      for (int i = 0; i != combi.length(); i++) 
		{
		  if (combi[i] == '<')
		    {
		      undirected = true;
		    }
		  if (combi[i] != '[' && combi[i] != ']' && !typePart) 
		    {
		      relationship.append(combi[i]);
		    }
		  else if (combi[i] == '[') 
		    {
		      typePart = true;
		      if (!undirected)
			{
			  relationship.append('-');
			}
		    }
		  if (typePart && combi[i] != ']' && combi[i] != '[') 
		    {
		      type.append(combi[i]);
		    }
		  if (typePart && combi[i] == ']')
		    {
		      typePart = false;
		    }
		}
	      query->bindValue(":relationship", relationship);
	      query->bindValue(":type", type);
	      query->bindValue(":incident", id);
	      query->exec();
	      query->first();
	      if (query->isNull(0)) 
		{
		  incongruenceLabel->setText("Incongruence detected");
		  qDebug() << "4";
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	      query2->bindValue(":incident", id);
	      query2->exec();
	      query2->first();
	      if (query2->isNull(0)) 
		{
		  incongruenceLabel->setText("Incongruence detected");
		  qDebug() << "5";
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	      else if (query2->value(0).toInt() != order) 
		{
		  incongruenceLabel->setText("Incongruence detected");
		  qDebug() << "6";
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	    }
	}
      query->prepare("SELECT incident FROM relationships_to_incident "
		     "WHERE relationship = :relationship AND type = :type");
      for (int i = 0; i != relationshipListWidget->rowCount(); i++) 
	{
	  QString combi = relationshipListWidget->item(i,0)->data(Qt::DisplayRole).toString();
	  QString relationship = "";
	  QString type = "";
	  bool typePart = false;
	  bool undirected = false;
	  for (int i = 0; i != combi.length(); i++) 
	    {
	      if (combi[i] == '<')
		{
		  undirected = true;
		}
	      if (combi[i] != '[' && combi[i] != ']' && !typePart) 
		{
		  relationship.append(combi[i]);
		}
	      else if (combi[i] == '[') 
		{
		  typePart = true;
		  if (!undirected)
		    {
		      relationship.append('-');
		    }
		}
	      if (typePart && combi[i] != ']' && combi[i] != '[') 
		{
		  type.append(combi[i]);
		}
	      if (typePart && combi[i] == ']')
		{
		  typePart = false;
		}
	    }
	  query->bindValue(":relationship", relationship);
	  query->bindValue(":type", type);
	  query->exec();
	  while (query->next()) 
	    {
	      int current = query->value(0).toInt();
	      bool found = false;
	      QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
	      while (it2.hasNext()) 
		{
		  OccurrenceItem *item = it2.next();
		  if (item->getId() == current &&
		      item->getAttribute() == combi) 
		    {
		      found = true;
		      break;
		    }
		}
	      if (!found) 
		{
		  incongruenceLabel->setText("Incongruence detected");
		  qDebug() << "7";
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	    }
	}
      delete query;
      delete query2;
      QSqlDatabase::database().commit();
    }
  QSqlQuery *query = new QSqlQuery;      
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
	  qDebug() << "8";
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
	  qDebug() << "9";
	  delete query;
	  return;
	}
    }
  incongruenceLabel->setText("");
  qApp->restoreOverrideCursor();
  qApp->processEvents();
}
  
void OccurrenceGraphWidget::toggleLegend() 
{
  if (legendWidget->isVisible()) 
    {
      legendWidget->hide();
    }
  else 
    {
      legendWidget->show();
    }
  rescale();
}

void OccurrenceGraphWidget::toggleGraphicsControls() 
{
  if (graphicsWidget->isVisible()) 
    {
      graphicsWidget->hide();
    }
  else 
    {
      graphicsWidget->show();
    }
  rescale();
}

void OccurrenceGraphWidget::toggleTimeLine()
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

void OccurrenceGraphWidget::rescale()
{
  view->scale(2.0, 2.0);
  view->scale(0.5, 0.5);
} 

void OccurrenceGraphWidget::processZoomSliderChange(int value)
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

void OccurrenceGraphWidget::resetZoomSlider()
{
  zoomSlider->setValue(0);
  zoomSlider->setSliderPosition(0);
}

void OccurrenceGraphWidget::setGraphControls(bool state)
{
  zoomSlider->setEnabled(state);
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
  fillOpacitySlider->setEnabled(state);
  addHorizontalGuideLineButton->setEnabled(state);
  addVerticalGuideLineButton->setEnabled(state);
  snapGuidesButton->setEnabled(state);
  increaseLabelSizeButton->setEnabled(state);
  decreaseLabelSizeButton->setEnabled(state);
  hideAnnotationsButton->setEnabled(state);
  setTimeRangeButton->setEnabled(state);
  plotLabelsButton->setEnabled(state);
  incidentLabelsOnlyButton->setEnabled(state);
  attributeLabelsOnlyButton->setEnabled(state);
  backgroundColorButton->setEnabled(state);
  increaseLabelSizeButton->setEnabled(state);
  decreaseLabelSizeButton->setEnabled(state);
  exportSvgButton->setEnabled(state);
  viewConcordanceButton->setEnabled(state);
  exportMatrixButton->setEnabled(state);
  lowerRangeDial->setEnabled(state);
  upperRangeDial->setEnabled(state);
  lowerRangeSpinBox->setEnabled(state);
  upperRangeSpinBox->setEnabled(state);
}

void OccurrenceGraphWidget::updateCases() 
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

void OccurrenceGraphWidget::checkCases() 
{
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

bool OccurrenceGraphWidget::attributesPresent() 
{
  return _presentAttributes.size() > 0;
}

bool OccurrenceGraphWidget::relationshipsPresent() 
{
  return _presentRelationships.size() > 0;
}

void OccurrenceGraphWidget::addAttribute() 
{
  setChangeLabel();
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, INCIDENT);
  attributeColorDialog->setWindowTitle("Add attribute to plot");
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) 
    {
      reset();
      QColor color = attributeColorDialog->getColor();
      QColor textColor = attributeColorDialog->getTextColor();
      bool entity = attributeColorDialog->isEntity();
      QString attribute = attributeColorDialog->getAttribute();
      if (!_presentAttributes.contains(attribute)) 
	{
	  _presentAttributes.push_back(attribute);
	  savePlotButton->setEnabled(true);
	  clearPlotButton->setEnabled(true);
	  QSqlQuery *query = new QSqlQuery;
	  if (entity) 
	    {
	      query->prepare("SELECT description FROM entities "
			     "WHERE name = :name");
	    }
	  else 
	    {
	      query->prepare("SELECT description FROM incident_attributes "
			     "WHERE name = :name");
	    }
	  query->bindValue(":name", attribute);
	  query->exec();
	  query->first();
	  QString description = query->value(0).toString();
	  QVector<QString> attributeVector;
	  attributeVector.push_back(attribute);
	  findChildren(attribute, &attributeVector, entity);
	  QVector<int> orders;
	  QSqlDatabase::database().transaction();
	  query->prepare("SELECT incident FROM attributes_to_incidents "
			 "WHERE attribute = :currentAttribute");
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT ch_order, description FROM incidents WHERE id = :id");
	  QVectorIterator<QString> it(attributeVector);
	  while (it.hasNext()) 
	    {
	      QString currentAttribute = it.next();
	      query->bindValue(":currentAttribute", currentAttribute);
	      query->exec();
	      while (query->next()) 
		{
		  int currentIncident = query->value(0).toInt();
		  query2->bindValue(":id", currentIncident);
		  query2->exec();
		  query2->first();
		  int order = query2->value(0).toInt();
		  QString incidentDescription = query2->value(1).toString();
		  if (!orders.contains(order)) 
		    {
		      orders.push_back(order);
		      QPointF position = QPointF((order * _distance), 0);
		      OccurrenceItem *newOccurrence = new OccurrenceItem(40, incidentDescription, position,
									 currentIncident, order, attribute);
		      newOccurrence->setPos(newOccurrence->getOriginalPos());
		      newOccurrence->setColor(color);
		      if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
			{
			  newOccurrence->setZValue(3);
			}
		      else if (_attributeOccurrenceVector.empty())
			{
			  newOccurrence->setZValue(_relationshipOccurrenceVector[0]->zValue());
			}
		      else
			{
			  newOccurrence->setZValue(_attributeOccurrenceVector[0]->zValue());
			}
		      _attributeOccurrenceVector.push_back(newOccurrence);
		      scene->addItem(newOccurrence);
		      OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
		      QString text = QString::number(order) + " - " + attribute;
		      label->setPlainText(text);
		      label->setDefaultTextColor(textColor);
		      label->setNewPos(newOccurrence->scenePos());
		      _attributeLabelVector.push_back(label);
		      newOccurrence->setLabel(label);
		      label->setZValue(newOccurrence->zValue() + 1);
		      scene->addItem(label);
		    }
		}
	    }
	  bool found = false;
	  for (int i = 0; i < attributeListWidget->rowCount(); i++) 
	    {
	      if (attributeListWidget->item(i, 0)->data(Qt::DisplayRole) == attribute) 
		{
		  found = true;
		  QTableWidgetItem *item = attributeListWidget->item(i,0);
		  QString toolTip = breakString(attribute + " - " + description);
		  item->setToolTip(toolTip);
		  attributeListWidget->item(i, 1)->setBackground(color);
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
	      attributeListWidget->setRowCount(attributeListWidget->rowCount() + 1);
	      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 0, item);
	      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->setBackground(color);
	      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->
		setFlags(attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	    }
	  delete query;
	  delete query2;
	  QSqlDatabase::database().commit();
	  groupOccurrences();
	  wireLinkages();
	}
      setRangeControls();
      scene->update();
      view->update();
      updateLinkages();
      checkCongruence();
      if (!caseListWidget->isEnabled()) 
	{
	  caseListWidget->setEnabled(true);
	}
      setGraphControls(true);      
    }
  delete attributeColorDialog;
}

void OccurrenceGraphWidget::addAttributes() 
{
  setChangeLabel();
  QPointer<AttributeCheckBoxDialog> attributeDialog = new AttributeCheckBoxDialog(this, INCIDENT);
  attributeDialog->exec();
  if (attributeDialog->getExitStatus() == 0)
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);
      // Let us first prepare a few queries
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2= new QSqlQuery;
      QSqlQuery *query3 = new QSqlQuery;
      QSqlQuery *query4 = new QSqlQuery;
      query->prepare("SELECT description FROM incident_attributes "
		     "WHERE name = :name");
      query2->prepare("SELECT description FROM entities "
		      "WHERE name = :name");
      query3->prepare("SELECT incident FROM attributes_to_incidents "
		      "WHERE attribute  = :currentAttribute");
      query4->prepare("SELECT ch_order, description FROM incidents "
		      "WHERE id = :id");
      // Then we get the selected attributes; 
      QVector<QPair<QString, bool> > chosenAttributes = attributeDialog->getAttributes();
      // Let's create a palette;
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
	  if (!_presentAttributes.contains(currentAttribute)) 
	    {
	      _presentAttributes.push_back(currentAttribute);
	    }
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
	  QVector<int> orders;
	  QVectorIterator<QString> it2(childrenVector);
	  while (it2.hasNext())
	    {
	      QString currentChild = it2.next();
	      query3->bindValue(":currentAttribute", currentChild);
	      query3->exec();
	      while (query3->next())
		{
		  int currentIncident = query3->value(0).toInt();
		  query4->bindValue(":id", currentIncident);
		  query4->exec();
		  query4->first();
		  int order = query4->value(0).toInt();
		  QString incidentDescription = query4->value(1).toString();
		  if (!orders.contains(order))
		    {
		      orders.push_back(order);
		      QPointF position = QPointF((order * _distance), 0);
		      OccurrenceItem *newOccurrence = new OccurrenceItem(40,
									 incidentDescription,
									 position,
									 currentIncident,
									 order,
									 currentAttribute);
		      newOccurrence->setPos(newOccurrence->getOriginalPos());
		      newOccurrence->setColor(currentColor);
		      if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
			{
			  newOccurrence->setZValue(3);
			}
		      else if (_attributeOccurrenceVector.empty())
			{
			  newOccurrence->setZValue(_relationshipOccurrenceVector[0]->zValue());
			}
		      else
			{
			  newOccurrence->setZValue(_attributeOccurrenceVector[0]->zValue());
			}
		      _attributeOccurrenceVector.push_back(newOccurrence);
		      scene->addItem(newOccurrence);
		      OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
		      QString text = QString::number(order) + " - " + currentAttribute;
		      label->setPlainText(text);
		      label->setDefaultTextColor(Qt::black);
		      label->setNewPos(newOccurrence->scenePos());
		      _attributeLabelVector.push_back(label);
		      newOccurrence->setLabel(label);
		      label->setZValue(newOccurrence->zValue() + 1);
		      scene->addItem(label);
		    }
		}
	    }
	  bool found = false;
	  for (int i = 0; i < attributeListWidget->rowCount(); i++) 
	    {
	      if (attributeListWidget->item(i, 0)->data(Qt::DisplayRole) == currentAttribute) 
		{
		  found = true;
		  QTableWidgetItem *item = attributeListWidget->item(i,0);
		  QString toolTip = breakString(currentAttribute + " - " + description);
		  item->setToolTip(toolTip);
		  attributeListWidget->item(i, 1)->setBackground(currentColor);
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
	      attributeListWidget->setRowCount(attributeListWidget->rowCount() + 1);
	      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 0, item);
	      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->setBackground(currentColor);
	      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->
		setFlags(attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	    }
	  groupOccurrences();
	  wireLinkages();
	}
      // Clean up our queries.
      delete query;
      delete query2;
      delete query3;
      delete query4;      
      // Set some settings
      savePlotButton->setEnabled(true);
      clearPlotButton->setEnabled(true);
      delete attributeDialog;
      setRangeControls();
      scene->update();
      view->update();
      updateLinkages();
      checkCongruence();
      if (!caseListWidget->isEnabled()) 
	{
	  caseListWidget->setEnabled(true);
	}
      setGraphControls(true);
      QApplication::restoreOverrideCursor();
      qApp->processEvents();
    }
  delete attributeDialog;
}
	 
void OccurrenceGraphWidget::addRelationship() 
{
  setChangeLabel();
  QPointer<RelationshipColorDialog> relationshipColorDialog = new RelationshipColorDialog(this);
  relationshipColorDialog->setWindowTitle("Add relationship to plot");
  relationshipColorDialog->exec();
  if (relationshipColorDialog->getExitStatus() == 0) 
    {
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2 = new QSqlQuery;
      QSqlQuery *query3 = new QSqlQuery;
      query->prepare("SELECT description, directedness FROM relationship_types "
		     "WHERE name = :type");
      query2->prepare("SELECT incident FROM relationships_to_incidents "
		     "WHERE relationship = :relationship AND type = :type");
      query3->prepare("SELECT ch_order, description FROM incidents WHERE id = :id");
      reset();
      QColor color = relationshipColorDialog->getColor();
      QColor textColor = relationshipColorDialog->getTextColor();
      QString relationship = relationshipColorDialog->getRelationship();
      QString type = relationshipColorDialog->getType();
      QStringList relationshipParts = QStringList();
      QString combi = QString();
      query->bindValue(":type", type);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QString directedness = query->value(1).toString();
      if (directedness == DIRECTED)
	{
	  relationshipParts = relationship.split("--->");
	  combi = relationshipParts[0] + "-[" + type
	    + "]->" + relationshipParts[1]; 
	}
      else if (directedness == UNDIRECTED)
	{
	  relationshipParts = relationship.split("<-->");
	  combi = relationshipParts[0] + "<-[" + type
	    + "]->" + relationshipParts[1]; 
	}
      if (!_presentRelationships.contains(combi)) 
	{
	  _presentRelationships.push_back(combi);
	  savePlotButton->setEnabled(true);
	  clearPlotButton->setEnabled(true);
	  QString description = query->value(0).toString();
	  QSqlDatabase::database().transaction();
	  query2->bindValue(":relationship", relationship);
	  query2->bindValue(":type", type);
	  query2->exec();
	  while (query2->next()) 
	    {
	      int currentIncident = query2->value(0).toInt();
	      query3->bindValue(":id", currentIncident);
	      query3->exec();
	      query3->first();
	      int order = query3->value(0).toInt();
	      QString incidentDescription = query3->value(1).toString();
	      QPointF position = QPointF((order * _distance), 0);
	      OccurrenceItem *newOccurrence = new OccurrenceItem(40, incidentDescription, position,
								 currentIncident, order, combi);
	      newOccurrence->setPos(newOccurrence->getOriginalPos());
	      newOccurrence->setColor(color);
	      if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
		{
		  newOccurrence->setZValue(3);
		}
	      else if (_attributeOccurrenceVector.empty())
		{
		  newOccurrence->setZValue(_relationshipOccurrenceVector[0]->zValue());
		}
	      else
		{
		  newOccurrence->setZValue(_attributeOccurrenceVector[0]->zValue());
		}
	      _relationshipOccurrenceVector.push_back(newOccurrence);
	      scene->addItem(newOccurrence);
	      OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
	      QString text = QString::number(order) + " - " + combi;
	      label->setPlainText(text);
	      label->setDefaultTextColor(textColor);
	      label->setNewPos(newOccurrence->scenePos());
	      _relationshipLabelVector.push_back(label);
	      newOccurrence->setLabel(label);
	      label->setZValue(newOccurrence->zValue() + 1);
	      scene->addItem(label);
	    }
	  bool found = false;
	  for (int i = 0; i < relationshipListWidget->rowCount(); i++) 
	    {
	      if (relationshipListWidget->item(i, 0)->data(Qt::DisplayRole) == combi) 
		{
		  found = true;
		  QTableWidgetItem *item = relationshipListWidget->item(i,0);
		  QString toolTip = breakString(combi + " - " + description);
		  item->setToolTip(toolTip);
		  relationshipListWidget->item(i, 1)->setBackground(color);
		  break;
		}
	    }
	  if (!found) 
	    {
	      QTableWidgetItem *item = new QTableWidgetItem(combi);
	      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	      QString toolTip = breakString(combi + " - " + 
					    description);
	      item->setToolTip(toolTip);
	      item->setData(Qt::DisplayRole, combi);
	      relationshipListWidget->setRowCount(relationshipListWidget->rowCount() + 1);
	      relationshipListWidget->setItem(relationshipListWidget->rowCount() - 1, 0, item);
	      relationshipListWidget->setItem(relationshipListWidget->rowCount() - 1, 1,
					      new QTableWidgetItem);
	      relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)
		->setBackground(color);
	      relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->
		setFlags(relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)
			 ->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	    }
	  QSqlDatabase::database().commit();
	  groupOccurrences();
	  wireLinkages();
	}
      delete query;
      delete query2;
      delete query3;
      setRangeControls();
      scene->update();
      view->update();
      updateLinkages();
      checkCongruence();
      if (!caseListWidget->isEnabled()) 
	{
	  caseListWidget->setEnabled(true);
	}
      setGraphControls(true);
    }
  delete relationshipColorDialog;
}

void OccurrenceGraphWidget::addRelationships() 
{
  setChangeLabel();
  QPointer<RelationshipCheckBoxDialog> relationshipDialog = new RelationshipCheckBoxDialog(this);
  relationshipDialog->exec();
  if (relationshipDialog->getExitStatus() == 0)
    {
      // Let us first prepare a few queries.
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2 = new QSqlQuery;
      QSqlQuery *query3 = new QSqlQuery;
      query->prepare("SELECT description, directedness FROM relationship_types "
		     "WHERE name = :type");
      query2->prepare("SELECT incident FROM relationships_to_incidents "
		      "WHERE relationship = :relationship AND type = :type");
      query3->prepare("SELECT ch_order, description FROM incidents "
		      "WHERE id = :id");
      // Let us then retrieve our data.
      QVector<QPair<QString, QString> > chosenRelationships = relationshipDialog->getRelationships();
      // Now let's create our color palette.
      QList<QColor> palette;
      double hue = double(rand()) / (double(RAND_MAX) + 1.0);
      int colorCount = chosenRelationships.size();
      for (int i = 0; i != colorCount; i++)
	{
	  palette.append(QColor::fromHslF(hue, 1.0, 0.5));
	  hue += 0.61803398874895f;
	  hue = std::fmod(hue, 1.0f);
	}
      int colorNumber = 0;
      QVectorIterator<QPair<QString, QString> > it(chosenRelationships);
      while (it.hasNext())
	{
	  // Let us set the color for this relationship and iteratre colorNumber
	  QColor currentColor = palette[colorNumber];
	  colorNumber++;
	  // Then we create our occurrences.
	  QPair<QString, QString> currentPair = it.next();	    
	  QString type = currentPair.first;
	  QString relationship = currentPair.second;
	  query->bindValue(":type", type);
	  query->exec();
	  query->first();
	  QString description = query->value(0).toString();
	  QString directedness = query->value(1).toString();
	  QStringList relationshipParts = QStringList();
	  QString combi = QString();
	  if (directedness == DIRECTED)
	    {
	      relationshipParts = relationship.split("--->");
	      combi = relationshipParts[0] + "-[" + type
		+ "]->" + relationshipParts[1]; 
	    }
	  else if (directedness == UNDIRECTED)
	    {
	      relationshipParts = relationship.split("<-->");
	      combi = relationshipParts[0] + "<-[" + type
		+ "]->" + relationshipParts[1]; 
	    }
	  if (!_presentRelationships.contains(combi))
	    {
	      _presentRelationships.push_back(combi);
	      savePlotButton->setEnabled(true);
	      clearPlotButton->setEnabled(true);
	      query2->bindValue(":relationship", relationship);
	      query2->bindValue(":type", type);
	      query2->exec();
	      while (query2->next())
		{
		  int currentIncident = query2->value(0).toInt();
		  query3->bindValue(":id", currentIncident);
		  query3->exec();
		  query3->first();
		  int order = query3->value(0).toInt();
		  QString incidentDescription = query3->value(1).toString();
		  QPointF position = QPointF((order * _distance), 0);
		  OccurrenceItem *newOccurrence = new OccurrenceItem(40,
								     incidentDescription,
								     position,
								     currentIncident,
								     order, combi);
		  newOccurrence->setPos(newOccurrence->getOriginalPos());
		  newOccurrence->setColor(currentColor);
		  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
		    {
		      newOccurrence->setZValue(3);
		    }
		  else if (_attributeOccurrenceVector.empty())
		    {
		      newOccurrence->setZValue(_relationshipOccurrenceVector[0]->zValue());
		    }
		  else
		    {
		      newOccurrence->setZValue(_attributeOccurrenceVector[0]->zValue());
		    }
		  _relationshipOccurrenceVector.push_back(newOccurrence);
		  scene->addItem(newOccurrence);
		  OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
		  QString text = QString::number(order) + " - " + combi;
		  label->setPlainText(text);
		  label->setDefaultTextColor(Qt::black);
		  label->setNewPos(newOccurrence->scenePos());
		  _relationshipLabelVector.push_back(label);
		  newOccurrence->setLabel(label);
		  label->setZValue(newOccurrence->zValue() + 1);
		  scene->addItem(label);
		}
	      bool found = false;
	      for (int i = 0; i < relationshipListWidget->rowCount(); i++) 
		{
		  if (relationshipListWidget->item(i, 0)->data(Qt::DisplayRole) == combi) 
		    {
		      found = true;
		      QTableWidgetItem *item = relationshipListWidget->item(i,0);
		      QString toolTip = breakString(combi + " - " + description);
		      item->setToolTip(toolTip);
		      relationshipListWidget->item(i, 1)->setBackground(currentColor);
		      break;
		    }
		}
	      if (!found) 
		{
		  QTableWidgetItem *item = new QTableWidgetItem(combi);
		  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		  QString toolTip = breakString(combi + " - " + 
						description);
		  item->setToolTip(toolTip);
		  item->setData(Qt::DisplayRole, combi);
		  relationshipListWidget->setRowCount(relationshipListWidget->rowCount() + 1);
		  relationshipListWidget->setItem(relationshipListWidget->rowCount() - 1, 0, item);
		  relationshipListWidget->setItem(relationshipListWidget->rowCount() - 1, 1,
						  new QTableWidgetItem);
		  relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)
		    ->setBackground(currentColor);
		  relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->
		    setFlags(relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)
			     ->flags() ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
		}
	      groupOccurrences();
	      wireLinkages();
	    }
	}
      // Cleaning up our queries
      delete query;
      delete query2;
      delete query3;
      // Setting some settings
      setRangeControls();
      scene->update();
      view->update();
      updateLinkages();
      checkCongruence();
      if (!caseListWidget->isEnabled()) 
	{
	  caseListWidget->setEnabled(true);
	}
      setGraphControls(true);
    }
  delete relationshipDialog;
}


void OccurrenceGraphWidget::setChangeLabel() 
{
  if (changeLabel->text() == "" && (_attributeOccurrenceVector.size() > 0 ||
				    _relationshipOccurrenceVector.size() > 0)) 
    {
      changeLabel->setText("*");
    }
}

void OccurrenceGraphWidget::updateLinkages() 
{
  QVectorIterator<Linkage*> it(_edgeVector);
  while (it.hasNext()) 
    {
      Linkage *current = it.next();
      current->updatePosition();
    }
}

void OccurrenceGraphWidget::findChildren(QString father, QVector<QString> *children, bool entity) 
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

void OccurrenceGraphWidget::removeAttributeMode() 
{
  setChangeLabel();
  QString text = attributeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  for (QVector<OccurrenceItem*>::iterator it = _attributeOccurrenceVector.begin();
       it != _attributeOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getAttribute() == text) 
	{
	  delete current->getLabel();
	  _attributeLabelVector.removeOne(current->getLabel());
	  delete current;
	  _attributeOccurrenceVector.removeOne(current);
	  _presentAttributes.removeOne(text);
	}
      else 
	{
	  it++;
	}
    }
  for (int i = 0; i != attributeListWidget->rowCount();) 
    {
      if (attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text) 
	{
	  attributeListWidget->removeRow(i);
	}
      if (i != attributeListWidget->rowCount()) 
	{
	  i++;
	}
    }
  if (_presentAttributes.size() > 0 || _presentRelationships.size() > 0) 
    {
      groupOccurrences();
    }
  else if (_presentAttributes.size() == 0 && _presentRelationships.size() == 0) 
    {
      incidentLabelsOnlyButton->setChecked(false);
      attributeLabelsOnlyButton->setChecked(false);
      savePlotButton->setEnabled(false);
      clearPlotButton->setEnabled(false);
      caseListWidget->setEnabled(false);
      setGraphControls(false);
    }
  wireLinkages();
  removeAttributeModeButton->setEnabled(false);
  checkCongruence();
}

void OccurrenceGraphWidget::removeRelationshipMode() 
{
  setChangeLabel();
  QString text = relationshipListWidget->currentItem()->data(Qt::DisplayRole).toString();
  for (QVector<OccurrenceItem*>::iterator it = _relationshipOccurrenceVector.begin();
       it != _relationshipOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getAttribute() == text) 
	{
	  delete current->getLabel();
	  _relationshipLabelVector.removeOne(current->getLabel());
	  delete current;
	  _relationshipOccurrenceVector.removeOne(current);
	  _presentRelationships.removeOne(text);
	}
      else 
	{
	  it++;
	}
    }
  for (int i = 0; i != relationshipListWidget->rowCount();) 
    {
      if (relationshipListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text) 
	{
	  relationshipListWidget->removeRow(i);
	}
      if (i != relationshipListWidget->rowCount()) 
	{
	  i++;
	}
    }
  if (_presentAttributes.size() > 0 || _presentRelationships.size() > 0) 
    {
      groupOccurrences();
    }
  else if (_presentAttributes.size() == 0 && _presentRelationships.size() == 0) 
    {
      incidentLabelsOnlyButton->setChecked(false);
      attributeLabelsOnlyButton->setChecked(false);
      caseListWidget->setEnabled(false);
      savePlotButton->setEnabled(false);
      clearPlotButton->setEnabled(false);
      setGraphControls(false);
    }
  wireLinkages();
  removeRelationshipModeButton->setEnabled(false);
  checkCongruence();
}

void OccurrenceGraphWidget::disableAttributeModeButton() 
{
  removeAttributeModeButton->setEnabled(false);
}

void OccurrenceGraphWidget::disableRelationshipModeButton() 
{
  removeRelationshipModeButton->setEnabled(false);
}

void OccurrenceGraphWidget::setAttributeModeButton(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
    {
      removeAttributeModeButton->setEnabled(true);
    }
  else 
    {
      removeAttributeModeButton->setEnabled(false);
    }
}

void OccurrenceGraphWidget::setRelationshipModeButton(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
    {
      removeRelationshipModeButton->setEnabled(true);
    }
  else 
    {
      removeRelationshipModeButton->setEnabled(false);
    }
}

void OccurrenceGraphWidget::changeAttributeModeColor(QTableWidgetItem *item) 
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
	  QTableWidgetItem* neighbour = attributeListWidget->item(item->row(), 0);
	  QString attribute = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
	  while (it.hasNext()) 
	    {
	      OccurrenceItem *current = it.next();
	      if (current->getAttribute() == attribute) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	}
    }
}

void OccurrenceGraphWidget::changeRelationshipModeColor(QTableWidgetItem *item) 
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
	  QTableWidgetItem* neighbour = relationshipListWidget->item(item->row(), 0);
	  QString relationship = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<OccurrenceItem*> it(_relationshipOccurrenceVector);
	  while (it.hasNext()) 
	    {
	      OccurrenceItem *current = it.next();
	      if (current->getAttribute() == relationship) 
		{
		  current->setColor(fillColor);
		  current->getLabel()->setDefaultTextColor(textColor);
		}
	    }
	}
    }
}

void OccurrenceGraphWidget::groupOccurrences() 
{
  QVector<OccurrenceItem*> allOccurrences;
  std::sort(_presentAttributes.begin(), _presentAttributes.end(), stringSort);
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  int persistentDist = 0;
  while (it.hasNext()) 
    {
      OccurrenceItem *current = it.next();
      allOccurrences.push_back(current);
      current->setGrouped(false);
      int dist = 0;
      QVectorIterator<QString> it2(_presentAttributes);
      while (it2.hasNext()) 
	{
	  QString text = it2.next();
	  if (text == current->getAttribute()) 
	    {
	      current->setPos(current->scenePos().x(), dist);
	      current->getLabel()->setNewPos(current->scenePos());
	      break;
	    }
	  else 
	    {
	      dist -= 150;
	      persistentDist -= 150;
	    }
	}
    }
  persistentDist -= 150;
  std::sort(_presentRelationships.begin(), _presentRelationships.end(), stringSort);
  QVectorIterator<OccurrenceItem*> it3(_relationshipOccurrenceVector);
  while (it3.hasNext()) 
    {
      OccurrenceItem *current = it3.next();
      allOccurrences.push_back(current);
      current->setGrouped(false);
      int dist = persistentDist;
      QVectorIterator<QString> it4(_presentRelationships);
      while (it4.hasNext()) 
	{
	  QString text = it4.next();
	  if (text == current->getAttribute()) 
	    {
	      current->setPos(current->scenePos().x(), dist);
	      current->getLabel()->setNewPos(current->scenePos());
	      break;
	    }
	  else 
	    {
	      dist -= 150;
	    }
	}
    }
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  QSet<OccurrenceItem*> finished;
  QSet<OccurrenceItem*> ignored;
  QVector<OccurrenceItem*>::iterator it5;
  for (it5 = allOccurrences.begin(); it5 != allOccurrences.end(); it5++) 
    {
      OccurrenceItem *first = *it5;
      QVector<OccurrenceItem*> group;
      if (!finished.contains(first) && !ignored.contains(first)) 
	{
	  group.push_back(first);
	  finished.insert(first);
	  QVector<OccurrenceItem*>::iterator it6;
	  for (it6 = it5 + 1; it6 != allOccurrences.end(); it6++) 
	    {
	      OccurrenceItem *second = *it6;
	      if (!finished.contains(second) && !ignored.contains(second)) 
		{
		  // First we need to check if they are in the same x coordinate
		  if (second->scenePos().x() == first->scenePos().x()) 
		    {
		      // Now we are sure that this entity belongs in this group.
		      group.push_back(second);
		      finished.insert(second);
		    }
		}
	    }
	  // Now we should have a finished group.
	  std::sort(group.begin(), group.end(), occurrencesSortTwo);
	  std::sort(group.begin(), group.end(), attributeLessThan);
	  qreal x = group.first()->scenePos().x();
	  qreal startY = group.first()->scenePos().y();
	  int dist = 80;
	  QVector<OccurrenceItem*>::iterator it7;
	  QMultiMap<int, QString> finishedAttributes;
	  for (it7 = group.begin(); it7 != group.end(); it7++) 
	    {
	      OccurrenceItem *current = *it7;
	      QList<QString> attributes = finishedAttributes.values(current->getId());
	      if (attributes.contains(current->getAttribute()))
		{
		  finished.remove(current);
		  ignored.insert(current);
		}
	      else
		{
		  if (group.size() > 1) 
		    {
		      current->setGrouped(true);
		    }
		  current->setPos(x, startY - dist);
		  current->getLabel()->setNewPos(current->scenePos());
		  finishedAttributes.insert(current->getId(), current->getAttribute());
		  dist += 80;
		}
	    }
	}
    }
  QVectorIterator<OccurrenceItem*> it8(allOccurrences);
  while (it8.hasNext()) 
    {
      OccurrenceItem *candidate = it8.next();
      if (!finished.contains(candidate)) 
	{
	  candidate->hide();
	  candidate->setPermHidden(true);
	  candidate->getLabel()->hide();
	}
    }
}

void OccurrenceGraphWidget::wireLinkages() 
{
  qDeleteAll(_edgeVector);
  _edgeVector.clear();
  std::sort(_attributeOccurrenceVector.begin(), _attributeOccurrenceVector.end(), eventLessThan);
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      OccurrenceItem *tempSource = it.next();
      OccurrenceItem *tempTarget = NULL;
      if (tempSource->isVisible()) 
	{
	  QVectorIterator<OccurrenceItem*> it2(_attributeOccurrenceVector);
	  while (it2.hasNext()) 
	    {
	      OccurrenceItem *temp = it2.next();
	      if (temp->isVisible()) 
		{
		  if (tempSource->getAttribute() == temp->getAttribute()) 
		    {
		      if (tempTarget == NULL && temp->scenePos().x() > tempSource->scenePos().x()) 
			{
			  tempTarget = temp;
			}
		      else 
			{
			  if (temp->scenePos().x() > tempSource->scenePos().x() &&
			      temp->scenePos().x() < tempTarget->scenePos().x() &&
			      tempSource->getAttribute() == temp->getAttribute()) 
			    {
			      tempTarget = temp;
			    }
			}
		    }
		}
	    }
	  if (tempTarget != NULL) 
	    {
	      Linkage *newLinkage = new Linkage(tempSource->getAttribute(), "");
	      if (_attributeOccurrenceVector.empty())
		{
		  newLinkage->setZValue(_relationshipOccurrenceVector[0]->zValue() - 1);
		}
	      else
		{
		  newLinkage->setZValue(_attributeOccurrenceVector[0]->zValue() - 1);
		}
	      newLinkage->setStartItem(tempSource);
	      newLinkage->setEndItem(tempTarget);
	      newLinkage->setCopy(true);
	      if (tempSource->isHighlighted())
		{
		  newLinkage->setColor(tempSource->getHighlightColor());
		  newLinkage->setPenWidth(5);
		}
	      _edgeVector.push_back(newLinkage);
	      scene->addItem(newLinkage);
	      QVectorIterator<OccurrenceItem*> it3(_attributeOccurrenceVector);
	      while (it3.hasNext())
		{
		  OccurrenceItem *current = it3.next();
		  if (current->isVisible() && current->getAttribute() == tempTarget->getAttribute() &&
		      current->scenePos().x() == tempTarget->scenePos().x() &&
		      current != tempTarget)
		    {
		      Linkage *newLinkageTwo = new Linkage(tempSource->getAttribute(), "");
		      if (_attributeOccurrenceVector.empty())
			{
			  newLinkage->setZValue(_relationshipOccurrenceVector[0]->zValue() - 1);
			}
		      else
			{
			  newLinkage->setZValue(_attributeOccurrenceVector[0]->zValue() - 1);
			}		     
		      newLinkageTwo->setStartItem(tempSource);
		      newLinkageTwo->setEndItem(current);
		      newLinkageTwo->setCopy(true);
		      if (tempSource->isHighlighted())
			{
			  newLinkageTwo->setColor(tempSource->getHighlightColor());
			  newLinkageTwo->setPenWidth(5);
			}
		      _edgeVector.push_back(newLinkageTwo);
		      scene->addItem(newLinkageTwo);
		    }
		}
	    }
	}
    }
  // And we do the same for the relationship->oriented occurrences
  std::sort(_relationshipOccurrenceVector.begin(), _relationshipOccurrenceVector.end(), eventLessThan);
  QVectorIterator<OccurrenceItem*> it4(_relationshipOccurrenceVector);
  while (it4.hasNext()) 
    {
      OccurrenceItem *tempSource = it4.next();
      OccurrenceItem *tempTarget = NULL;
      if (tempSource->isVisible()) 
	{
	  QVectorIterator<OccurrenceItem*> it5(_relationshipOccurrenceVector);
	  while (it5.hasNext()) 
	    {
	      OccurrenceItem *temp = it5.next();
	      if (temp->isVisible()) 
		{
		  if (tempSource->getAttribute() == temp->getAttribute()) 
		    {
		      if (tempTarget == NULL && temp->scenePos().x() > tempSource->scenePos().x()) 
			{
			  tempTarget = temp;
			}
		      else 
			{
			  if (temp->scenePos().x() > tempSource->scenePos().x() &&
			      temp->scenePos().x() < tempTarget->scenePos().x() &&
			      tempSource->getAttribute() == temp->getAttribute()) 
			    {
			      tempTarget = temp;
			    }
			}
		    }
		}
	    }
	  if (tempTarget != NULL) 
	    {
	      Linkage *newLinkage = new Linkage(tempSource->getAttribute(), "");
	      if (_attributeOccurrenceVector.empty())
		{
		  newLinkage->setZValue(_relationshipOccurrenceVector[0]->zValue() - 1);
		}
	      else
		{
		  newLinkage->setZValue(_attributeOccurrenceVector[0]->zValue() - 1);
		}
	      newLinkage->setStartItem(tempSource);
	      newLinkage->setEndItem(tempTarget);
	      newLinkage->setCopy(true);
	      if (tempSource->isHighlighted())
		{
		  newLinkage->setColor(tempSource->getHighlightColor());
		  newLinkage->setPenWidth(5);
		}
	      _edgeVector.push_back(newLinkage);
	      scene->addItem(newLinkage);
	      QVectorIterator<OccurrenceItem*> it6(_relationshipOccurrenceVector);
	      while (it6.hasNext())
		{
		  OccurrenceItem *current = it6.next();
		  if (current->isVisible() && current->getAttribute() == tempTarget->getAttribute() &&
		      current->scenePos().x() == tempTarget->scenePos().x() &&
		      current != tempTarget)
		    {
		      Linkage *newLinkageTwo = new Linkage(tempSource->getAttribute(), "");
		      if (_attributeOccurrenceVector.empty())
			{
			  newLinkage->setZValue(_relationshipOccurrenceVector[0]->zValue() - 1);
			}
		      else
			{
			  newLinkage->setZValue(_attributeOccurrenceVector[0]->zValue() - 1);
			}
		      newLinkageTwo->setStartItem(tempSource);
		      newLinkageTwo->setEndItem(current);
		      newLinkageTwo->setCopy(true);
		      if (tempSource->isHighlighted())
			{
			  newLinkageTwo->setColor(tempSource->getHighlightColor());
			  newLinkageTwo->setPenWidth(5);
			}
		      _edgeVector.push_back(newLinkageTwo);
		      scene->addItem(newLinkageTwo);
		    }
		}
	    }
	}
    }
  updateLinkages();
}

void OccurrenceGraphWidget::makeLayout()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (layoutComboBox->currentText() == REDOLAYOUT)
    {
      restore();
    }
  else if (layoutComboBox->currentText() == MATCHEVENTGRAPH)
    {
      restore();
      matchEventGraph();
    }
  else if (layoutComboBox->currentText() == DATELAYOUTDAYS)
    {
      dateLayout(35);
    }
  else if (layoutComboBox->currentText() == DATELAYOUTMONTHS)
    {
      dateLayout(20);
    }
  else if (layoutComboBox->currentText() == DATELAYOUTYEARS)
    {
      dateLayout(5);
    }
  else if (layoutComboBox->currentText() == DATELAYOUTDECADES)
    {
      dateLayout(1);
    }  else if (layoutComboBox->currentText() == REDUCEOVERLAP)
    {
      reduceOverlap();
    }
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void OccurrenceGraphWidget::restore() 
{
  reset();
  for (int i = 0; i != caseListWidget->count(); i++)
    {
      QListWidgetItem *item = caseListWidget->item(i);
      item->setCheckState(Qt::Unchecked);
    }
  _checkedCases.clear();
  caseListWidget->setEnabled(true); 
  groupOccurrences();
  wireLinkages();
  setVisibility();
}

void OccurrenceGraphWidget::reset() 
{
  _matched = false;
  QVector<OccurrenceItem*>::iterator it;
  for (it = _attributeOccurrenceVector.begin(); it != _attributeOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getId() < 0) 
	{
	  delete current->getLabel();
	  _attributeLabelVector.removeOne(current->getLabel());
	  current->setLabel(NULL);
	  delete current;
	  _attributeOccurrenceVector.removeOne(current);
	}
      else 
	{
	  current->setPos(QPointF((current->getOrder() * _distance), 0));
	  current->setPermHidden(false); // We reset this here.2
	  QString text = QString::number(current->getOrder()) + " - " + current->getAttribute();
	  QColor textColor = current->getLabel()->defaultTextColor();
	  delete current->getLabel();
	  _attributeLabelVector.removeOne(current->getLabel());
	  OccurrenceLabel *newLabel = new OccurrenceLabel(current);
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(textColor);
	  current->show();
	  current->setLabel(newLabel);
	  current->getLabel()->show();
	  current->getLabel()->setNewPos(current->scenePos());
	  scene->addItem(current->getLabel());
	  _attributeLabelVector.push_back(current->getLabel());
	  it++;
	}
    }
  // And now we do the same for the relationship-oriented occurrences
  for (it = _relationshipOccurrenceVector.begin(); it != _relationshipOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getId() < 0) 
	{
	  delete current->getLabel();
	  _relationshipLabelVector.removeOne(current->getLabel());
	  current->setLabel(NULL);
	  delete current;
	  _relationshipOccurrenceVector.removeOne(current);
	}
      else 
	{
	  current->setPos(QPointF((current->getOrder() * _distance), 0));
	  current->setPermHidden(false); // We reset this here.2
	  QString text = QString::number(current->getOrder()) + " - " + current->getAttribute();
	  QColor textColor = current->getLabel()->defaultTextColor();
	  delete current->getLabel();
	  _relationshipLabelVector.removeOne(current->getLabel());
	  OccurrenceLabel *newLabel = new OccurrenceLabel(current);
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(textColor);
	  current->show();
	  current->setLabel(newLabel);
	  current->getLabel()->show();
	  current->getLabel()->setNewPos(current->scenePos());
	  scene->addItem(current->getLabel());
	  _relationshipLabelVector.push_back(current->getLabel());
	  it++;
	}
    }
  setVisibility();
}

void OccurrenceGraphWidget::reduceOverlap()
{
  // Let's first group all occurrences together
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext())
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext())
    {
      allOccurrences.push_back(it2.next());
    }
  // We need to make a while loop that continues to run as long as there
  // is still something that overlaps
  bool overlapping = true; // We assume that something overlaps initially.
  while (overlapping)
    {
      // We reset this boolean on every run.
      overlapping = false;
      // Then we iterate through everything to remove the overlap.
      QVectorIterator<OccurrenceItem*> it3(allOccurrences);
      while (it3.hasNext())
	{
	  OccurrenceItem *first = it3.next();
	  QVectorIterator<OccurrenceItem*> it4(allOccurrences);
	  while (it4.hasNext())
	    {
	      OccurrenceItem *second = it4.next();
	      // Make sure that we are not looking at the same occurrence twice.
	      if (first != second)
		{
		  // Let's calculate the distance between these two items.
		  qreal dist = qSqrt(qPow(first->scenePos().x() - second->scenePos().x(), 2) +
				     qPow(first->scenePos().y() - second->scenePos().y(), 2));
		  // If they overlap
		  if (dist < 40)
		    {
		      overlapping = true; // We need another run
		      // Let's see which node needs to move in which direction.
		      if (first->scenePos().y() > second->scenePos().y())
			{
			  first->setPos(first->scenePos().x(), first->scenePos().y() + 30);
			  second->setPos(second->scenePos().x(), second->scenePos().y() - 30);
			  first->getLabel()->setNewPos(first->scenePos());
			  second->getLabel()->setNewPos(second->scenePos());
			}
		      else // Same stuff for moving in the other direction
			{
			  first->setPos(first->scenePos().x(), first->scenePos().y() - 30);
			  second->setPos(second->scenePos().x(), second->scenePos().y() + 30);
			  first->getLabel()->setNewPos(first->scenePos());
			  second->getLabel()->setNewPos(second->scenePos());
			}
		    }
		}
	    }
	}
    }
  softGrouping();
}

void OccurrenceGraphWidget::softGrouping()
{
  // First we gather all occurrences
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext())
    {
      OccurrenceItem *occurrence = it.next();
      if (occurrence->isVisible())
	{
	  allOccurrences.push_back(occurrence);
	}
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext())
    {
      OccurrenceItem *occurrence = it2.next();
      if (occurrence->isVisible())
	{
	  allOccurrences.push_back(occurrence);
	}
    }
  // Let's sort the occurrences.
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  // And let's now regroup the grouped items.
  QVector<OccurrenceItem*> finished;
  QVector<OccurrenceItem*>::iterator it3;
  for (it3 = allOccurrences.begin(); it3 != allOccurrences.end(); it3++) 
    {
      OccurrenceItem *first = *it3;
      if (first->isGrouped())
	{
	  QVector<OccurrenceItem*> group;
	  if (!finished.contains(first)) 
	    {
	      group.push_back(first);
	      finished.push_back(first);
	      QVector<OccurrenceItem*>::iterator it4;
	      for (it4 = it3 + 1; it4 != allOccurrences.end(); it4++) 
		{
		  OccurrenceItem *second = *it4;
		  if (second->isGrouped())
		    {
		      if (!finished.contains(second)) 
			{
			  // First we need to check if they are in the same x coordinate
			  if (second->scenePos().x() == first->scenePos().x()) 
			    {
			      group.push_back(second);
			      finished.push_back(second);
			    }
			}
		    }
		}
	    }
	  // Now we should have a finished group.
	  std::sort(group.begin(), group.end(), occurrencesSortTwo);
	  std::sort(group.begin(), group.end(), attributeLessThan);
	  if (!group.empty())
	    {
	      qreal x = group.first()->scenePos().x();
	      qreal startY = group.first()->scenePos().y();
	      int dist = 80;
	      QVector<OccurrenceItem*>::iterator it5;
	      for (it5 = group.begin(); it5 != group.end(); it5++) 
		{
		  OccurrenceItem *current = *it5;
		  current->setPos(x, startY - dist);
		  current->getLabel()->setNewPos(current->scenePos());
		  dist += 80;
		}
	    }
	}
    }
}

void OccurrenceGraphWidget::dateLayout(int scale) 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->prepare("SELECT timestamp FROM incidents "
		 "WHERE id = :id");
  query2->prepare("UPDATE incidents SET mark = 1 "
		  "WHERE id = :id");
  QVector<OccurrenceItem*> visible;
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext())
    {
      OccurrenceItem *occurrence = it.next();
      if (occurrence->isVisible())
	{
	  visible.push_back(occurrence);
	}
      allOccurrences.push_back(occurrence);
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext())
    {
      OccurrenceItem *occurrence = it2.next();
      if (occurrence->isVisible())
	{
	  visible.push_back(occurrence);
	}
      allOccurrences.push_back(occurrence);
    }
  std::sort(visible.begin(), visible.end(), occurrencesSort);
  OccurrenceItem *first = visible.first();
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
      QMap<OccurrenceItem*, qint64> days;
      QMap<OccurrenceItem*, int> precision;
      QMap<OccurrenceItem*, QDate> dates;
      QVectorIterator<OccurrenceItem*> it4(visible);
      while (it4.hasNext())
	{
	  OccurrenceItem *occurrence = it4.next();
	  int id = occurrence->getId();
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
	      days.insert(occurrence, firstDate.daysTo(date));
	      validTotal++;
	      dates.insert(occurrence, date);
	      if (dateString.length() == 4)
		{
		  precision.insert(occurrence, 1);
		}
	      else if (dateString.length() == 7)
		{
		  precision.insert(occurrence, 2);
		}
	      else if (dateString.length() == 10)
		{
		  precision.insert(occurrence, 3);
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
	  it4.next(); // skip the first one.
	  while (it4.hasNext())
	    {
	      OccurrenceItem *occurrence = it4.next();
	      if (days.contains(occurrence))
		{
		  qint64 daysTo = days.value(occurrence);
		  qreal x = scale * daysTo + first->scenePos().x();
		  QDate currentDate = dates.value(occurrence);
		  bool precisionDifference = false;
		  if (x >= lastValid)
		    {
		      occurrence->setPos(x, occurrence->scenePos().y());
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		      lastValid = x;
		      lastPrecision = precision.value(occurrence);
		      lastDate = dates.value(occurrence);
		    }
		  else if (precision.value(occurrence) <= lastPrecision)
		    {
		      if (precision.value(occurrence) == 2 &&
			  currentDate.month() == lastDate.month())
			{
			  precisionDifference = true;
			}
		      else if (precision.value(occurrence) == 1 &&
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
			      warningBox->setWindowTitle("Checking dtates");
			      warningBox->addButton(QMessageBox::Ok);
			      QPointer<QAbstractButton> markButton = warningBox->
				addButton(tr("Mark"), QMessageBox::NoRole);
			      QPointer<QAbstractButton> skipButton = warningBox->
				addButton(tr("Skip remaining warnings"), QMessageBox::NoRole);
			      warningBox->setIcon(QMessageBox::Warning);
			      warningBox->setText("<b>Possible problem detected</b>");
			      warningBox->setInformativeText("Incident " +
							     QString::number(occurrence->getOrder()) +
							     " is incorrectly positioned in the "
							     "chronological order and may cause "
							     "problems for the layout.");
			      warningBox->exec();
			      if (warningBox->clickedButton() == markButton)
				{
				  query2->bindValue(":id", occurrence->getId());
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
			  QVectorIterator<OccurrenceItem*> it5 = it4;
			  bool resolved = false;
			  bool foundValid = false;
			  while (!foundValid)
			    {
			      while (it5.hasNext())
				{
				  OccurrenceItem *next = it5.next();
				  if (days.contains(next))
				    {
				      qint64 daysToNext = days.value(next);
				      qreal xNext = scale * daysToNext + first->scenePos().x();
				      if (xNext >= lastValid)
					{
					  qreal tempX = (lastValid + xNext) / 2;
					  occurrence->setPos(tempX, occurrence->scenePos().y());
					  occurrence->getLabel()->setNewPos(occurrence->scenePos());
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
			      occurrence->setPos(lastValid + _distance, occurrence->scenePos().y());
			      occurrence->getLabel()->setNewPos(occurrence->scenePos());
			    }
			}
		    }
		}
	      else
		{
		  QVectorIterator<OccurrenceItem*> it5 = it4;
		  bool foundValid = false;
		  bool resolved = false;
		  while (!foundValid)
		    {
		      while (it5.hasNext())
			{
			  OccurrenceItem *next = it5.next();
			  if (days.contains(next))
			    {
			      qint64 daysToNext = days.value(next);
			      qreal xNext = scale * daysToNext + first->scenePos().x();
			      if (xNext >= lastValid)
				{
				  qreal tempX = (lastValid + xNext) / 2;
				  occurrence->setPos(tempX, occurrence->scenePos().y());
				  occurrence->getLabel()->setNewPos(occurrence->scenePos());
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
		      occurrence->setPos(lastValid + _distance, occurrence->scenePos().y());
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		    }
		}
	    }
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
  groupOccurrences();
  wireLinkages();
  setVisibility();
  delete query;
}

void OccurrenceGraphWidget::matchEventGraph() 
{
  reset();
  _matched = true;
  QVector<IncidentNode*> incidents = _eventGraphWidgetPtr->getIncidentNodes();
  if (incidents.size() > 0) 
    {
      QVectorIterator<IncidentNode*> it(incidents);
      while (it.hasNext()) 
	{
	  IncidentNode *incident = it.next();
	  // We first do this for the attributes-oriented occurrences.
	  QVectorIterator<OccurrenceItem*> it2(_attributeOccurrenceVector);
	  while (it2.hasNext()) 
	    {
	      OccurrenceItem *occurrence = it2.next();
	      if (incident->getId() == occurrence->getId()) 
		{
		  if (incident->getAbstractNode() != NULL) 
		    {
		      AbstractNode *abstractNode = incident->getAbstractNode();
		      while (abstractNode->getAbstractNode() != NULL) 
			{
			  abstractNode = abstractNode->getAbstractNode();
			}
		      QString type = "";
		      if (abstractNode->getConstraint() == PATHS ||
			  abstractNode->getConstraint() == PATHSATT) 
			{
			  type = "P";
			}
		      else if (abstractNode->getConstraint() == SEMIPATHS ||
			       abstractNode->getConstraint() == SEMIPATHSATT) 
			{
			  type = "S";
			}
		      else if (abstractNode->getConstraint() == NOCONSTRAINT ||
			       abstractNode->getConstraint() == NOCONSTRAINTATT) 
			{
			  type = "N";
			}
		      QString text = type + QString::number(abstractNode->getOrder()) + " - " +
			occurrence->getAttribute();
		      QColor textColor = occurrence->getLabel()->defaultTextColor();
		      delete occurrence->getLabel();
		      _attributeLabelVector.removeOne(occurrence->getLabel());
		      OccurrenceLabel *newLabel = new OccurrenceLabel(occurrence);
		      newLabel->setPlainText(text);
		      newLabel->setDefaultTextColor(textColor);
		      occurrence->setLabel(newLabel);
		      occurrence->show();
		      occurrence->setId(abstractNode->getId());
		      occurrence->getLabel()->show();
		      occurrence->setToolTip(breakString(abstractNode->getDescription()));
		      occurrence->setPos(abstractNode->scenePos().x(), 0);
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		      scene->addItem(occurrence->getLabel());
		      _attributeLabelVector.push_back(occurrence->getLabel());
		    }
		  else 
		    {
		      occurrence->setPos(incident->scenePos().x(), 0);
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		    }
		}
	    }
	  // And then we do the relationships-oriented occurrences.
	  QVectorIterator<OccurrenceItem*> it3(_relationshipOccurrenceVector);
	  while (it3.hasNext()) 
	    {
	      OccurrenceItem *occurrence = it3.next();
	      if (incident->getId() == occurrence->getId()) 
		{
		  if (incident->getAbstractNode() != NULL) 
		    {
		      AbstractNode *abstractNode = incident->getAbstractNode();
		      while (abstractNode->getAbstractNode() != NULL) 
			{
			  abstractNode = abstractNode->getAbstractNode();
			}
		      QString type = "";
		      if (abstractNode->getConstraint() == PATHS ||
			  abstractNode->getConstraint() == PATHSATT) 
			{
			  type = "P";
			}
		      else if (abstractNode->getConstraint() == SEMIPATHS ||
			       abstractNode->getConstraint() == SEMIPATHSATT) 
			{
			  type = "S";
			}
		      else if (abstractNode->getConstraint() == NOCONSTRAINT ||
			       abstractNode->getConstraint() == NOCONSTRAINTATT) 
			{
			  type = "N";
			}
		      QString text = type + QString::number(abstractNode->getOrder()) + " - " +
			occurrence->getAttribute();
		      QColor textColor = occurrence->getLabel()->defaultTextColor();
		      delete occurrence->getLabel();
		      _relationshipLabelVector.removeOne(occurrence->getLabel());
		      OccurrenceLabel *newLabel = new OccurrenceLabel(occurrence);
		      newLabel->setPlainText(text);
		      newLabel->setDefaultTextColor(textColor);
		      occurrence->setLabel(newLabel);
		      occurrence->show();
		      occurrence->setId(abstractNode->getId());
		      occurrence->getLabel()->show();
		      occurrence->setPos(abstractNode->scenePos().x(), 0);
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		      scene->addItem(occurrence->getLabel());
		      _attributeLabelVector.push_back(occurrence->getLabel());
		    }
		  else 
		    {
		      occurrence->setPos(incident->scenePos().x(), 0);
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		    }
		}
	    }
	}
    }
  /* 
     Then we do the abstractNodes for the attribute-oriented occurrences. 
     We don't need to do this for the relationship-oriented occurrences.
  */
  QVector<AbstractNode*> abstractNodes = _eventGraphWidgetPtr->getAbstractNodes();
  if (abstractNodes.size() > 0) 
    {
      QVectorIterator<AbstractNode*> it(abstractNodes);
      while (it.hasNext()) 
	{
	  AbstractNode *abstractNode = it.next();
	  QSet<QString> attributes = abstractNode->getAttributes();
	  QSetIterator<QString> it2(attributes);
	  while (it2.hasNext()) 
	    {
	      QString currentAttribute = it2.next();
	      QColor color = QColor();
	      bool found = false;
	      for (int i = 0; i != attributeListWidget->rowCount(); i++) 
		{
		  QTableWidgetItem *item = attributeListWidget->item(i, 0);
		  QString title = item->data(Qt::DisplayRole).toString();
		  if (title == currentAttribute) 
		    {
		      color = attributeListWidget->item(i, 1)->background().color();
		      found = true;
		    }
		}
	      if (found) 
		{
		  while (abstractNode->getAbstractNode() != NULL) 
		    {
		      abstractNode = abstractNode->getAbstractNode();
		    }
		  QPointF position = QPointF(abstractNode->scenePos().x(), 0);
		  OccurrenceItem *newOccurrence = new OccurrenceItem(40,
								     abstractNode->getDescription(),
								     position, abstractNode->getId(),
								     abstractNode->getOrder(),
								     currentAttribute);
		  newOccurrence->setPos(newOccurrence->getOriginalPos());
		  newOccurrence->setColor(color);
		  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
		    {
		      newOccurrence->setZValue(3);
		    }
		  else if (_attributeOccurrenceVector.empty())
		    {
		      newOccurrence->setZValue(_relationshipOccurrenceVector[0]->zValue());
		    }
		  else
		    {
		      newOccurrence->setZValue(_attributeOccurrenceVector[0]->zValue());
		    }
		  _attributeOccurrenceVector.push_back(newOccurrence);
		  scene->addItem(newOccurrence);
		  OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
		  QString type = "";
		  if (abstractNode->getConstraint() == PATHS ||
		      abstractNode->getConstraint() == PATHSATT) 
		    {
		      type = "P";
		    }
		  else if (abstractNode->getConstraint() == SEMIPATHS ||
			   abstractNode->getConstraint() == SEMIPATHSATT) 
		    {
		      type = "S"; 
		    }
		  else if (abstractNode->getConstraint() == NOCONSTRAINT ||
			   abstractNode->getConstraint() == NOCONSTRAINTATT) 
		    {
		      type = "N";
		    }
		  QString text = type + QString::number(abstractNode->getOrder()) + " - " + currentAttribute;
		  label->setPlainText(text);
		  label->setDefaultTextColor(Qt::black);
		  label->setNewPos(newOccurrence->scenePos());
		  _attributeLabelVector.push_back(label);
		  newOccurrence->setLabel(label);
		  label->setZValue(newOccurrence->zValue() + 1);
		  scene->addItem(label);
		}
	    }
	}
    }
  // Now we also need to check the same cases as those checked in the event graph widget
  QVector<QString> eventGraphCases = _eventGraphWidgetPtr->getCheckedCases();
  _checkedCases.clear();
  for (int i = 0; i != caseListWidget->count(); i++)
    {
      QListWidgetItem *item = caseListWidget->item(i);
      if (eventGraphCases.contains(item->data(Qt::DisplayRole).toString()))
	{
	  item->setCheckState(Qt::Checked);
	  _checkedCases.push_back(item->data(Qt::DisplayRole).toString());
	}
      else
	{
	  item->setCheckState(Qt::Unchecked);
	}
    }
  caseListWidget->setEnabled(false);
  setVisibility();
  groupOccurrences();
  wireLinkages();
}

void OccurrenceGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) 
{
  OccurrenceItem *source = NULL;
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  QVectorIterator<OccurrenceItem*> it3(allOccurrences);
  while (it3.hasNext()) 
    {
      OccurrenceItem *temp = it3.next();
      if (temp == item) 
	{
	  source = temp;
	}
    }
  if (source != NULL) 
    {
      qreal currentY = source->getLastPos().y();
      qreal newY = pos.y();
      qreal yDiff = newY - currentY;
      it3.toFront();
      while (it3.hasNext()) 
	{
	  OccurrenceItem *current = it3.next();
	  if (current->scenePos().x() == source->scenePos().x() && current != source) 
	    {
	      current->setPos(current->scenePos().x(), current->scenePos().y() + yDiff);
	      current->getLabel()->setNewPos(current->scenePos());
	    }
	}
    }
}

void OccurrenceGraphWidget::processMoveLine(QGraphicsItem *item, QPointF pos) 
{
  OccurrenceItem *source = NULL;
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  QVectorIterator<OccurrenceItem*> it3(allOccurrences);
  while (it3.hasNext()) 
    {
      OccurrenceItem *temp = it3.next();
      if (temp == item) 
	{
	  source = temp;
	}
    }
  if (source != NULL) 
    {
      qreal currentY = source->getLastPos().y();
      qreal newY = pos.y();
      qreal yDiff = newY - currentY;
      if (!source->isGrouped()) 
	{
	  it3.toFront();
	  while (it3.hasNext()) 
	    {
	      OccurrenceItem *current = it3.next();
	      if (current->getAttribute() == source->getAttribute() &&
		  !current->isGrouped() && current != source) 
		{
		  current->setPos(current->scenePos().x(), current->scenePos().y() + yDiff);
		  current->getLabel()->setNewPos(current->scenePos());
		}
	    }
	}
    }
}

void OccurrenceGraphWidget::setPenStyle()
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

void OccurrenceGraphWidget::setPenWidth()
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

void OccurrenceGraphWidget::addLineObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
    {
      newLineObject->setZValue(5);
    }
  else if (_attributeOccurrenceVector.empty())
    {
      newLineObject->setZValue(_relationshipOccurrenceVector[0]->zValue() + 2);
    }
  else
    {
      newLineObject->setZValue(_attributeOccurrenceVector[0]->zValue() + 2);
    }
  newLineObject->setSelected(true);
}

void OccurrenceGraphWidget::addSingleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
    {
      newLineObject->setZValue(5);
    }
  else if (_attributeOccurrenceVector.empty())
    {
      newLineObject->setZValue(_relationshipOccurrenceVector[0]->zValue() + 2);
    }
  else
    {
      newLineObject->setZValue(_attributeOccurrenceVector[0]->zValue() + 2);
    }
  newLineObject->setSelected(true);
}

void OccurrenceGraphWidget::addDoubleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setArrow2(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
    {
      newLineObject->setZValue(5);
    }
  else if (_attributeOccurrenceVector.empty())
    {
      newLineObject->setZValue(_relationshipOccurrenceVector[0]->zValue() + 2);
    }
  else
    {
      newLineObject->setZValue(_attributeOccurrenceVector[0]->zValue() + 2);
    }
  newLineObject->setSelected(true);
}

void OccurrenceGraphWidget::addEllipseObject(const QRectF &area)
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
  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
    {
      newEllipse->setZValue(5);
    }
  else if (_attributeOccurrenceVector.empty())
    {
      newEllipse->setZValue(_relationshipOccurrenceVector[0]->zValue() + 2);
    }
  else
    {
      newEllipse->setZValue(_attributeOccurrenceVector[0]->zValue() + 2);
    }
  newEllipse->setSelected(true);
}

void OccurrenceGraphWidget::addRectObject(const QRectF &area) 
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
  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
    {
      newRect->setZValue(5);
    }
  else if (_attributeOccurrenceVector.empty())
    {
      newRect->setZValue(_relationshipOccurrenceVector[0]->zValue() + 2);
    }
  else
    {
      newRect->setZValue(_attributeOccurrenceVector[0]->zValue() + 2);
    }
  newRect->setSelected(true);
}

void OccurrenceGraphWidget::addTextObject(const QRectF &area, const qreal &size)
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
      if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
	{
	  newText->setZValue(6);
	}
      else if (_attributeOccurrenceVector.empty())
	{
	  newText->setZValue(_relationshipOccurrenceVector[0]->zValue() + 3);
	}
      else
	{
	  newText->setZValue(_attributeOccurrenceVector[0]->zValue() + 3);
	}
      newText->adjustSize();
      newText->setSelected(true);
    }
  delete textDialog;
}

void OccurrenceGraphWidget::addTimeLineObject(const qreal &startX, const qreal &endX, const qreal &y)
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
  if (_attributeOccurrenceVector.empty() && _relationshipOccurrenceVector.empty())
    {
      newTimeLine->setZValue(5);
    }
  else if (_attributeOccurrenceVector.empty())
    {
      newTimeLine->setZValue(_relationshipOccurrenceVector[0]->zValue() + 2);
    }
  else
    {
      newTimeLine->setZValue(_attributeOccurrenceVector[0]->zValue() + 2);
    }
  newTimeLine->setSelected(true);
}

void OccurrenceGraphWidget::addHorizontalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(true);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void OccurrenceGraphWidget::addVerticalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(false);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void OccurrenceGraphWidget::toggleSnapGuides()
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

void OccurrenceGraphWidget::setLineColor()
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

void OccurrenceGraphWidget::setFillColor()
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

void OccurrenceGraphWidget::setFillOpacity(int value)
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

void OccurrenceGraphWidget::setMajorIntervalBySlider()
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

void OccurrenceGraphWidget::setMajorIntervalBySpinBox()
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

void OccurrenceGraphWidget::setMinorDivisionBySlider()
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

void OccurrenceGraphWidget::setMinorDivisionBySpinBox()
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

void OccurrenceGraphWidget::setMajorTickSize()
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

void OccurrenceGraphWidget::setMinorTickSize()
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

void OccurrenceGraphWidget::setTimeLineWidth()
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

void OccurrenceGraphWidget::setTimeLineColor()
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

void OccurrenceGraphWidget::processShapeSelection()
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

void OccurrenceGraphWidget::processLineContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::toggleArrow1() 
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

void OccurrenceGraphWidget::toggleArrow2() 
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

void OccurrenceGraphWidget::deleteLine() 
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

void OccurrenceGraphWidget::duplicateLine() 
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

void OccurrenceGraphWidget::processTextContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::changeText() 
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

void OccurrenceGraphWidget::deleteText() 
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

void OccurrenceGraphWidget::duplicateText() 
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
	  setChangeLabel();
	}
    }
}

void OccurrenceGraphWidget::processEllipseContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::deleteEllipse() 
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

void OccurrenceGraphWidget::duplicateEllipse() 
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

void OccurrenceGraphWidget::processRectContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::deleteRect() 
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

void OccurrenceGraphWidget::duplicateRect() 
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

void OccurrenceGraphWidget::processTimeLineContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::timeLineToggleFirstTick()
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

void OccurrenceGraphWidget::timeLineToggleForceLastTick()
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

void OccurrenceGraphWidget::deleteTimeLine() 
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

void OccurrenceGraphWidget::duplicateTimeLine() 
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

void OccurrenceGraphWidget::objectOneForward() 
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

void OccurrenceGraphWidget::processGuideLineContextMenu(const QString &action) 
{
  if (action == DELETEGUIDEACTION) 
    {
      deleteGuideLine();
    }
}

void OccurrenceGraphWidget::deleteGuideLine()
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

void OccurrenceGraphWidget::processOccurrenceItemContextMenu(QGraphicsItem* item,
							     const QString &action)
{
  OccurrenceItem *original = qgraphicsitem_cast<OccurrenceItem*>(item);
  QString attribute = original->getAttribute();
  if (action == SETHIGHLIGHTEDACTION)
    {
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      QColor highlightColor = QColor(Qt::black);
      colorDialog->setCurrentColor(highlightColor);
      if (colorDialog->exec()) 
	{
	  highlightColor = colorDialog->selectedColor();
	}
      else 
	{
	  return;
	}
      QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
      while (it.hasNext())
	{
	  OccurrenceItem *current = it.next();
	  if (current->getAttribute() == attribute)
	    {
	      current->setHighlight(highlightColor);
	    }
	}
      QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
      while (it2.hasNext())
	{
	  OccurrenceItem *current = it2.next();
	  if (current->getAttribute() == attribute)
	    {
	      current->setHighlight(highlightColor);
	    }
	}
      QVectorIterator<Linkage*> it3(_edgeVector);
      while (it3.hasNext())
	{
	  Linkage *edge = it3.next();
	  QGraphicsItem *start = edge->getStart();
	  OccurrenceItem *current = qgraphicsitem_cast<OccurrenceItem*>(start);
	  if (current->getAttribute() == attribute)
	    {
	      edge->setPenWidth(5);
	      edge->setColor(highlightColor);
	    }
	}
    }
  else if (action == UNSETHIGHLIGHTEDACTION)
    {
      QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
      while (it.hasNext())
	{
	  OccurrenceItem *current = it.next();
	  if (current->getAttribute() == attribute)
	    {
	      current->unsetHighlight();
	    }
	}
      QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
      while (it2.hasNext())
	{
	  OccurrenceItem *current = it2.next();
	  if (current->getAttribute() == attribute)
	    {
	      current->unsetHighlight();
	    }
	}
      QVectorIterator<Linkage*> it3(_edgeVector);
      while (it3.hasNext())
	{
	  Linkage *edge = it3.next();
	  QGraphicsItem *start = edge->getStart();
	  OccurrenceItem *current = qgraphicsitem_cast<OccurrenceItem*>(start);
	  if (current->getAttribute() == attribute)
	    {
	      edge->setPenWidth(1);
	      edge->setColor(QColor(Qt::black));
	    }
	}
    }
}

void OccurrenceGraphWidget::objectOneBackward() 
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

void OccurrenceGraphWidget::objectToFront() 
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

void OccurrenceGraphWidget::objectToBack() 
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

void OccurrenceGraphWidget::fixZValues() 
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
  setChangeLabel();  
}

void OccurrenceGraphWidget::plotLabels() 
{
  QVectorIterator<OccurrenceLabel*> it(_attributeLabelVector);
  while (it.hasNext()) 
    {
      OccurrenceLabel *currentItem = it.next();
      OccurrenceItem *currentOccurrence = currentItem->getOccurrence();
      if (currentOccurrence->isVisible()) 
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
  QVectorIterator<OccurrenceLabel*> it2(_relationshipLabelVector);
  while (it2.hasNext()) 
    {
      OccurrenceLabel *currentItem = it2.next();
      OccurrenceItem *currentOccurrence = currentItem->getOccurrence();
      if (currentOccurrence->isVisible()) 
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

void OccurrenceGraphWidget::toggleIncidentLabelsOnly() 
{
  if (incidentLabelsOnlyButton->isChecked()) 
    {
      _incidentLabelsOnly = true;
      _attributeLabelsOnly = false;
      if (attributeLabelsOnlyButton->isChecked())
	{
	  attributeLabelsOnlyButton->setChecked(false);
	}
      QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder());
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _attributeLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _attributeLabelVector.removeOne(oldLabel);
	}
      QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
      while (it2.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it2.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder());
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setTextWidth(newLabel->boundingRect().width());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _relationshipLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _relationshipLabelVector.removeOne(oldLabel);
	}
    }
  else 
    {
      _incidentLabelsOnly = false;
      QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder()) + " - " +
	    currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _attributeLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _attributeLabelVector.removeOne(oldLabel);
	}
      QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
      while (it2.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it2.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder()) + " - " +
	    currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _relationshipLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _relationshipLabelVector.removeOne(oldLabel);
	}
    }
  setVisibility();
}

void OccurrenceGraphWidget::toggleAttributeLabelsOnly() 
{
  if (attributeLabelsOnlyButton->isChecked()) 
    {
      _incidentLabelsOnly = false;
      _attributeLabelsOnly = true;
      if (incidentLabelsOnlyButton->isChecked())
	{
	  incidentLabelsOnlyButton->setChecked(false);
	}
      QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _attributeLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _attributeLabelVector.removeOne(oldLabel);
	}
      QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
      while (it2.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it2.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _relationshipLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _relationshipLabelVector.removeOne(oldLabel);
	}
    }
  else 
    {
      _attributeLabelsOnly = false;
      QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder()) + " - " +
	    currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _attributeLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _attributeLabelVector.removeOne(oldLabel);
	}
      QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
      while (it2.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it2.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder()) + " - " +
	    currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  _relationshipLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(oldLabel->zValue());
	  scene->addItem(newLabel);
	  if (!newLabel->getOccurrence()->isVisible())
	    {
	      newLabel->hide();
	    }
	  delete oldLabel;
	  _relationshipLabelVector.removeOne(oldLabel);
	}
    }
  setVisibility();
}

void OccurrenceGraphWidget::increaseLabelSize()
{
  if (_labelSize < 100)
    {
      _labelSize++;
      QVectorIterator<OccurrenceLabel*> it(_attributeLabelVector);
      while (it.hasNext())
	{
	  OccurrenceLabel *currentLabel = it.next();
	  currentLabel->setFontSize(_labelSize);
	}
      QVectorIterator<OccurrenceLabel*> it2(_relationshipLabelVector);
      while (it2.hasNext())
	{
	  OccurrenceLabel *currentLabel = it2.next();
	  currentLabel->setFontSize(_labelSize);
	}
      setChangeLabel();
    }
}

void OccurrenceGraphWidget::decreaseLabelSize()
{
    if (_labelSize > 5)
    {
      _labelSize--;
      QVectorIterator<OccurrenceLabel*> it(_attributeLabelVector);
      while (it.hasNext())
	{
	  OccurrenceLabel *currentLabel = it.next();
	  currentLabel->setFontSize(_labelSize);
	}
      QVectorIterator<OccurrenceLabel*> it2(_relationshipLabelVector);
      while (it2.hasNext())
	{
	  OccurrenceLabel *currentLabel = it2.next();
	  currentLabel->setFontSize(_labelSize);
	}
      setChangeLabel();
    }
}

void OccurrenceGraphWidget::setBackgroundColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      setChangeLabel();
      QColor color = colorDialog->selectedColor();
      scene->setBackgroundBrush(color);
    }
  delete colorDialog;
}

void OccurrenceGraphWidget::increaseDistance() 
{
  setChangeLabel();
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  OccurrenceItem *first = allOccurrences.first();
  QVectorIterator<OccurrenceItem*> it3(allOccurrences);
  while (it3.hasNext())
    {
      OccurrenceItem *current = it3.next();
      if (current != first)
	{
	  qreal distance = current->scenePos().x() - first->scenePos().x();
	  qreal newDistance = distance * 1.1;
	  current->setPos(QPointF(first->scenePos().x() + newDistance, current->scenePos().y()));
	  current->getLabel()->setNewPos(current->scenePos());
	}
    }
  if (!_matched)
    {
      _distance *= 1.1;
    }
  updateLinkages();
}

void OccurrenceGraphWidget::decreaseDistance() 
{
  setChangeLabel();    
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  OccurrenceItem *first = allOccurrences.first();
  QVectorIterator<OccurrenceItem*> it3(allOccurrences);
  while (it3.hasNext())
    {
      OccurrenceItem *current = it3.next();
      if (current != first)
	{
	  qreal distance = current->scenePos().x() - first->scenePos().x();
	  qreal newDistance = distance * 0.9;
	  current->setPos(QPointF(first->scenePos().x() + newDistance, current->scenePos().y()));
	  current->getLabel()->setNewPos(current->scenePos());
	}
    }
  if (!_matched)
    {
      _distance *= 0.9;
    }
  updateLinkages();
}

void OccurrenceGraphWidget::setRangeControls() 
{
  lowerRangeDial->setEnabled(true);
  upperRangeDial->setEnabled(true);
  lowerRangeSpinBox->setEnabled(true);
  upperRangeSpinBox->setEnabled(true);
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  int minOrder = -1;
  int maxOrder = -1;
  std::sort(allOccurrences.begin(), allOccurrences.end(), occurrencesSort);
  QVectorIterator<OccurrenceItem*> it3(allOccurrences);
  while (it3.hasNext()) 
    {
      OccurrenceItem *current = it3.next();
      if (minOrder == -1) 
	{
	  minOrder = current->getOrder();
	}
      else if (current->getOrder() < minOrder) 
	{
	  minOrder = current->getOrder();
	}
      if (maxOrder == -1) 
	{
	  maxOrder = current->getOrder();
	}
      else if (current->getOrder() > maxOrder) 
	{
	  maxOrder = current->getOrder();
	}
    }
  lowerRangeDial->setRange(minOrder, maxOrder - 1);
  upperRangeDial->setRange(minOrder + 1, maxOrder);
  lowerRangeSpinBox->setRange(minOrder, maxOrder - 1);
  upperRangeSpinBox->setRange(minOrder + 1, maxOrder);
  lowerRangeDial->setValue(minOrder);
  lowerRangeDial->setValue(minOrder);
  upperRangeDial->setValue(maxOrder);
  upperRangeSpinBox->setValue(maxOrder);
}

void OccurrenceGraphWidget::setTimeRange()
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

void OccurrenceGraphWidget::processLowerRange(int value) 
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
 
void OccurrenceGraphWidget::processUpperRange(int value) 
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

void OccurrenceGraphWidget::setVisibility() 
{
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  QVectorIterator<OccurrenceItem*> it3(allOccurrences);
  while (it3.hasNext()) 
    {
      OccurrenceItem* currentItem = it3.next();
      int order = currentItem->getOrder();
      if (currentItem->isPermHidden()) 
	{
	  currentItem->hide();
	}
      else 
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
      if (_checkedCases.size() > 0) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT incident FROM incidents_to_cases "
			 "WHERE incident = :incident AND casename = :casename");
	  int id = currentItem->getId();
	  if (id < 0) 
	    { // Occurrence items for abstractNode events have negative IDs.
	      bool keep = false;
	      QVector<AbstractNode*> abstractNodes = _eventGraphWidgetPtr->getAbstractNodes();
	      QVectorIterator<AbstractNode*> it4(abstractNodes);
	      while (it4.hasNext()) 
		{
		  AbstractNode *currentAbstractNode = it4.next();
		  if (currentAbstractNode->getId() == id) 
		    {
		      QVector<IncidentNode*> contents = currentAbstractNode->getIncidents();
		      QVectorIterator<IncidentNode*> it5(contents);
		      while (it5.hasNext()) 
			{
			  IncidentNode *currentIncident = it5.next();
			  QVectorIterator<QString> it6(_checkedCases);
			  while (it6.hasNext()) 
			    {
			      QString currentCase = it6.next();
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
		    }
		}
	      if (!keep) 
		{
		  currentItem->hide();
		}
	    }
	  else 
	    {
	      bool found = false;
	      QVectorIterator<QString> it4(_checkedCases);
	      while (it4.hasNext()) 
		{
		  QString currentCase = it4.next();
		  query->bindValue(":incident", id);
		  query->bindValue(":casename", currentCase);
		  query->exec();
		  query->first();
		  if (!query->isNull(0)) 
		    {
		      found = true;
		    }
		  if (_matched) 
		    {
		      QVector<IncidentNode*> events = _eventGraphWidgetPtr->getIncidentNodes();
		      QVectorIterator<IncidentNode*> it5(events);
		      while (it5.hasNext()) 
			{
			  IncidentNode *currentIncidentNode = it5.next();
			  if (currentIncidentNode->getId() == currentItem->getId()) 
			    {
			      if (currentIncidentNode->getAbstractNode() != NULL) 
				{
				  QVector<IncidentNode*> contents = currentIncidentNode->getAbstractNode()->getIncidents();
				  QVectorIterator<IncidentNode*> it6(contents);
				  while (it6.hasNext()) 
				    {
				      IncidentNode *currentIncident = it6.next();
				      query->bindValue(":incident", currentIncident->getId());
				      query->bindValue(":casename", currentCase);
				      query->exec();
				      query->first();
				      if (!query->isNull(0)) 
					{
					  found = true;
					}
				    }
				}
			    }
			}
		    }
		}
	      if (!found) 
		{
		  currentItem->hide();
		}
	    }
	  delete query;
	}
    }
  wireLinkages();
  QVectorIterator<Linkage*> it4(_edgeVector);
  while (it4.hasNext()) 
    {
      Linkage *currentEdge = it4.next();
      OccurrenceItem *tail = qgraphicsitem_cast<OccurrenceItem*>(currentEdge->getStart());
      OccurrenceItem *head = qgraphicsitem_cast<OccurrenceItem*>(currentEdge->getEnd());
      bool show = true;
      if (tail) 
	{
	  if(!tail->isVisible()) 
	    {
	      show = false;
	    } 
	  if (head) 
	    {
	      if (!head->isVisible()) 
		{
		  show = false;
		}
	    }
	  if (!show) 
	    {
	      currentEdge->hide();
	    }
	  else 
	    {
	      currentEdge->show();
	    }
	}
    }
  QVectorIterator<OccurrenceLabel*> it5(_attributeLabelVector);
  while (it5.hasNext()) 
    {
      OccurrenceLabel *currentText = it5.next();
      if (!_labelsVisible)
	{
	  currentText->hide();
	}
      else
	{
	  OccurrenceItem *currentParent = currentText->getOccurrence();
	  if (!(currentParent->isVisible())) 
	    {
	      currentText->hide();
	    }
	  else 
	    {
	      currentText->show();
	    }
	}
    }
  QVectorIterator<OccurrenceLabel*> it6(_relationshipLabelVector);
  while (it6.hasNext()) 
    {
      OccurrenceLabel *currentText = it6.next();
      if (!_labelsVisible)
	{
	  currentText->hide();
	}
      else
	{
	  OccurrenceItem *currentParent = currentText->getOccurrence();
	  if (!(currentParent->isVisible())) 
	    {
	      currentText->hide();
	    }
	  else 
	    {
	      currentText->show();
	    }
	}
    }
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
}

void OccurrenceGraphWidget::exportSvg() 
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

void OccurrenceGraphWidget::viewConcordancePlot()
{
  // We create a vector to manage the memory of our temporary graphics.
  QVector<QGraphicsItem*> drawItems;
  // Then we put all occurrences in one vector.
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      OccurrenceItem *current = it.next();
      if (current->isVisible())
	{
	  allOccurrences.push_back(current);
	}
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      OccurrenceItem *current = it2.next();
      if (current->isVisible())
	{
	  allOccurrences.push_back(current);
	}
    }
  // Let us sort the occurrences.
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  // We need to take our first occurrence item as an orientation point.
  OccurrenceItem *first = allOccurrences.first();
  // Now we can normalise the distances.
  QMap<OccurrenceItem*, qreal> positions;
  QVectorIterator<OccurrenceItem*> it3(allOccurrences);
  // We need to remember the furthest distance for our rectangles.
  qreal rectWidth = 0.0;
  while (it3.hasNext())
    {
      OccurrenceItem *current = it3.next();
      qreal distance = current->scenePos().x() - first->scenePos().x();
      qreal newDistance = distance / _distance;
      positions.insert(current, first->scenePos().x() + newDistance);
      if (newDistance > rectWidth)
	{
	  rectWidth = newDistance;
	}
    }
  rectWidth += 1.0;
  // Then we need to identify the attributes and relationships we need to export;
  QVector<QString> items;
  // We also want to store the colors so that we can assign them to our lines
  QVector<QColor> colors;
  for (int i = 0; i != attributeListWidget->rowCount(); i++) 
    {
      QString currentAttribute = attributeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
      QColor currentColor = attributeListWidget->item(i, 1)->background().color(); 
      items.push_back(currentAttribute);
      colors.push_back(currentColor);
    }
  for (int i = 0; i != relationshipListWidget->rowCount(); i++) 
    {
      QString currentRelationship = relationshipListWidget->item(i,0)->data(Qt::DisplayRole).toString();
      QColor currentColor = relationshipListWidget->item(i, 1)->background().color(); 
      items.push_back(currentRelationship);
      colors.push_back(currentColor);
    }
  QVectorIterator<QString> it4(items);
  qreal y = 0.0;
  int colorIterator = 0;
  while (it4.hasNext()) 
    {
      QString currentItem = it4.next();
      QColor currentColor = colors[colorIterator];
      // We create a label for our current occurrence item.
      QGraphicsTextItem *text = new QGraphicsTextItem;
      text->setPlainText(currentItem);
      drawItems.push_back(text);
      qreal textWidth = text->boundingRect().width();
      text->setPos(first->scenePos().x() - (textWidth + 30.0), y + 5.0);
      // Let us also add labels for the boundaries of the plots
      QGraphicsTextItem *lowerBound = new QGraphicsTextItem;
      lowerBound->setPlainText(QString::number(first->getOrder()));
      drawItems.push_back(lowerBound);
      textWidth = lowerBound->boundingRect().width();
      lowerBound->setPos(first->scenePos().x() - (textWidth + 5.0), y + 5.0);
      QGraphicsTextItem *upperBound = new QGraphicsTextItem;
      upperBound->setPlainText(QString::number(allOccurrences.last()->getOrder()));
      drawItems.push_back(upperBound);
      textWidth = upperBound->boundingRect().width();
      upperBound->setPos(positions.value(allOccurrences.last()) + 5.0, y + 5.0);
      // Let's also create a rectangle.
      QGraphicsRectItem *rect = new QGraphicsRectItem();
      rect->setPen(QPen(Qt::gray, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
      rect->setRect(first->scenePos().x() - 1.0, y - 1.0, rectWidth + 1, 42);
      drawItems.push_back(rect);
      QVectorIterator<OccurrenceItem*> it5(allOccurrences);
      while (it5.hasNext()) 
	{
	  OccurrenceItem *occurrence = it5.next();
	  if (occurrence->getAttribute() == currentItem && occurrence->isVisible())
	    {
	      qreal x = positions.value(occurrence);
	      QGraphicsLineItem *line = new QGraphicsLineItem();
	      line->setPen(QPen(currentColor, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
	      line->setLine(x, y, x, y + 40.0);
	      line->setData(1, x);
	      drawItems.push_back(line);
	    }
	}
      y += 42.0;
      colorIterator++;
    }
  QVectorIterator<QGraphicsItem*> it5(drawItems);
  while (it5.hasNext())
    {
      QGraphicsItem *first = it5.next();
      QGraphicsLineItem *firstLine = qgraphicsitem_cast<QGraphicsLineItem*>(first);
      if (firstLine)
	{
	    QVectorIterator<QGraphicsItem*> it6(drawItems);
	    while (it6.hasNext())
	      {
		QGraphicsItem *second = it6.next();
		QGraphicsLineItem *secondLine = qgraphicsitem_cast<QGraphicsLineItem*>(second);
		if (secondLine)
		  {
		    if (firstLine != secondLine)
		      {
			
			if (firstLine->data(1) == secondLine->data(1))
			  {
			    QColor firstColor = firstLine->pen().color();
			    QColor secondColor = secondLine->pen().color();
			    int red  = (firstColor.red() + secondColor.red()) / 2;
			    int green  = (firstColor.green() + secondColor.green()) / 2;
			    int blue  = (firstColor.blue() + secondColor.blue()) / 2;
			    QColor blendColor = QColor(red, green, blue);
			    firstLine->setPen(QPen(blendColor, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
			    secondLine->setPen(QPen(blendColor, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
			  }
		      }
		  }
	      }
	}
    }
  // Now we create the dialog
  QPointer<ConcordanceDialog> dialog = new ConcordanceDialog(this, drawItems, true);
  dialog->setWindowTitle("Line plot");
  dialog->exec();
}

void OccurrenceGraphWidget::exportMatrix() 
{
  // First we put all occurrences together
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(_attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      OccurrenceItem *current = it.next();
      if (current->isVisible())
	{
	  allOccurrences.push_back(current);
	}
    }
  QVectorIterator<OccurrenceItem*> it2(_relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      OccurrenceItem *current = it2.next();
      if (current->isVisible())
	{
	  allOccurrences.push_back(current);
	}
    }
  // Then we make a matrix (in the form of a vector of vectors).
  QVector<QVector<int>> matrix;
  QVector<QString> headerRow;
  QVector<QString> items;
  for (int i = 0; i != attributeListWidget->rowCount(); i++) 
    {
      QString currentAttribute = attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
      items.push_back(currentAttribute);
    }
  for (int i = 0; i != relationshipListWidget->rowCount(); i++) 
    {
      QString currentRelationship = relationshipListWidget->item(i,0)->data(Qt::DisplayRole).toString();
      items.push_back(currentRelationship);
    }
  QVectorIterator<QString> it3(items);
  while (it3.hasNext()) 
    {
      QVector<int> currentRow;
      QString currentItem = it3.next();
      headerRow.push_back(currentItem);
      QVectorIterator<QString> it4(items);
      while (it4.hasNext()) 
	{
	  QString currentPartner = it4.next();
	  int count = 0;
	  QVectorIterator<OccurrenceItem*> it5(allOccurrences);
	  while (it5.hasNext()) 
	    {
	      OccurrenceItem *first = it5.next();
	      if (first->getAttribute() == currentItem) 
		{
		  QVectorIterator<OccurrenceItem*> it6(allOccurrences);
		  while (it6.hasNext()) 
		    {
		      OccurrenceItem *second = it6.next();
		      if (second->getAttribute() == currentPartner &&
			  first->scenePos().x() == second->scenePos().x()) 
			{
			  count++;
			}
		    }
		}
	    }
	  currentRow.push_back(count);
	}
      matrix.push_back(currentRow);
    }
  // Then we can create a file.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header.
      QVectorIterator<QString> it3(headerRow);
      while (it3.hasNext()) 
	{
	  QString currentHeader = it3.next();
	  fileOut << "," << "\"" << doubleQuote(currentHeader).toStdString() << "\"";
	}
      fileOut << "\n"; // End the header with a newline symbol.
      // Then we write the other data.
      int counter = 0;
      QVectorIterator<QVector<int>> it4(matrix);
      while (it4.hasNext()) 
	{
	  QVector<int> currentRow = it4.next();
	  // The first row should always be the attribute label.
	  fileOut << doubleQuote(headerRow[counter]).toStdString(); 
	  counter++;
	  QVectorIterator<int> it5(currentRow);
	  while (it5.hasNext()) 
	    {
	      int currentValue = it5.next();
	      fileOut << "," << currentValue;
	    }
	  fileOut << "\n"; // End each row with a newline symbol.
	}
      // And that should be it!
      fileOut.close();
    }
}

void OccurrenceGraphWidget::saveCurrentPlot() 
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
      query->prepare("SELECT plot FROM saved_og_plots WHERE plot = :name");
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
	  // Clear out all data before writing.
	  QColor color = scene->backgroundBrush().color();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  query->prepare("UPDATE saved_og_plots "
			 "SET distance = :distance, red = :red, green = :green, blue = :blue "
			 "WHERE plot = :plot");
	  query->bindValue(":distance", _distance);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_settings
	  query->prepare("DELETE FROM saved_og_plots_settings "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_occurence_items
	  query->prepare("DELETE FROM saved_og_plots_occurrence_items "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_occurrence_labels
	  query->prepare("DELETE FROM saved_og_plots_occurrence_labels "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_legend
	  query->prepare("DELETE FROM saved_og_plots_legend "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_lines
	  query->prepare("DELETE FROM saved_og_plots_lines "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_timelines
	  query->prepare("DELETE FROM saved_og_plots_timelines "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_texts
	  query->prepare("DELETE FROM saved_og_plots_texts "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_ellipses
	  query->prepare("DELETE FROM saved_og_plots_ellipses "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_rects
	  query->prepare("DELETE FROM saved_og_plots_rects "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_cases
	  query->prepare("DELETE FROM saved_og_plots_cases "
			 "WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	  // saved_og_plots_guides
	  query->prepare("DELETE FROM saved_og_plots_guides "
			"WHERE plot = :plot");
	  query->bindValue(":plot", name);
	  query->exec();
	}
      else 
	{
	  // Insert new data into saved_og_plots and then write data.
	  QColor color = scene->backgroundBrush().color();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  query->prepare("INSERT INTO saved_og_plots (plot, distance, red, green, blue) "
			 "VALUES (:name, :distance, :red, :green, :blue)");
	  query->bindValue(":name", name);
	  query->bindValue(":distance", _distance);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->exec();
	}
      query->prepare("INSERT INTO saved_og_plots_settings "
		     "(plot, lowerbound, upperbound, labelson, incidentlabelsonly, "
		     "attributelabelsonly, labelsize) "
		     "VALUES (:plot, :lowerbound, :upperbound, :labelson, :incidentlabelsonly, "
		     ":attributelabelsonly, :labelsize)");
      query->bindValue(":plot", name);
      query->bindValue(":lowerbound", lowerRangeDial->value());
      query->bindValue(":upperbound", upperRangeDial->value());
      int labelson = 0;
      if (_labelsVisible)
	{
	  labelson = 1;
	}
      query->bindValue(":labelson", labelson);
      int incidentlabelsonly = 0;
      if (_incidentLabelsOnly)
	{
	  incidentlabelsonly = 1;
	}
      int attributelabelsonly = 0;
      if (_attributeLabelsOnly)
	{
	  attributelabelsonly = 1;
	}
      query->bindValue(":incidentlabelsonly", incidentlabelsonly);
      query->bindValue(":attributelabelsonly", attributelabelsonly);
      query->bindValue(":labelsize", _labelSize);
      query->exec();      
      QVector<OccurrenceItem*> allOccurrences;
      QVectorIterator<OccurrenceItem*> aIt(_attributeOccurrenceVector);
      while (aIt.hasNext()) 
	{
	  allOccurrences.push_back(aIt.next());
	}
      QVectorIterator<OccurrenceItem*> rIt(_relationshipOccurrenceVector);
      while (rIt.hasNext()) 
	{
	  allOccurrences.push_back(rIt.next());
	}
      QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, allOccurrences.size());
      saveProgress->setWindowTitle("Saving occurrence items");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      int counter = 1;
      saveProgress->show();
      QSqlDatabase::database().transaction();
      query->prepare("INSERT INTO saved_og_plots_occurrence_items "
		     "(plot, incident, ch_order, attribute, width, curxpos, curypos, orixpos, "
		     "oriypos, red, green, blue, alpha, highlighted, hred, hgreen, hblue, "
		     "halpha, zvalue, hidden, perm, relationship, grouped) "
		     "VALUES (:plot, :incident, :order, :attribute, :width, :curxpos, :curypos, "
		     ":orixpos, :oriypos, :red, :green, :blue, :alpha, :highlighted, :hred, "
		     ":hgreen, :hblue, :halpha, :zvalue, :hidden, :perm, :relationship, :grouped)");
      QVectorIterator<OccurrenceItem*> it(allOccurrences);
      while (it.hasNext()) 
	{
	  OccurrenceItem *currentItem = it.next();
	  qreal currentX = currentItem->pos().x();
	  qreal currentY = currentItem->pos().y();
	  qreal originalX = currentItem->getOriginalPos().x();
	  qreal originalY = currentItem->getOriginalPos().y();
	  int incident = currentItem->getId();
	  int order = currentItem->getOrder();
	  QString attribute = currentItem->getAttribute();
	  int width = currentItem->getWidth();
	  QColor color = currentItem->getColor();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  int alpha = color.alpha();
	  int highlighted = 0;
	  QColor highlightColor = currentItem->getHighlightColor();
	  int hred = highlightColor.red();
	  int hgreen = highlightColor.green();
	  int hblue = highlightColor.blue();
	  int halpha = highlightColor.alpha();
	  int zvalue = currentItem->zValue();
	  int hidden = 1;
	  int perm = 0;
	  int relationship = 0;
	  int grouped = 0;
	  if (currentItem->isHighlighted())
	    {
	      highlighted = 1;
	    }
	  if (currentItem->isVisible()) 
	    {
	      hidden = 0;
	    }
	  if (currentItem->isPermHidden()) 
	    {
	      perm = 1;
	    }
	  if (_relationshipOccurrenceVector.contains(currentItem)) 
	    {
	      relationship = 1;
	    }
	  if (currentItem->isGrouped())
	    {
	      grouped = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":incident", incident);
	  query->bindValue(":order", order);
	  query->bindValue(":attribute", attribute);
	  query->bindValue(":width", width);
	  query->bindValue(":curxpos", currentX);
	  query->bindValue(":curypos", currentY);
	  query->bindValue(":orixpos", originalX);
	  query->bindValue(":oriypos", originalY);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->bindValue(":alpha", alpha);
	  query->bindValue(":highlighted", highlighted);
	  query->bindValue(":hred", hred);
	  query->bindValue(":hgreen", hgreen);
	  query->bindValue(":hblue", hblue);
	  query->bindValue(":halpha", halpha);
	  query->bindValue(":zvalue", zvalue);
	  query->bindValue(":hidden", hidden);
	  query->bindValue(":perm", perm);
	  query->bindValue(":relationship", relationship);
	  query->bindValue(":grouped", grouped);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      QVector<OccurrenceLabel*> allLabels;
      QVectorIterator<OccurrenceLabel*> alIt(_attributeLabelVector);
      while (alIt.hasNext()) 
	{
	  allLabels.push_back(alIt.next());
	}
      QVectorIterator<OccurrenceLabel*> rlIt(_relationshipLabelVector);
      while (rlIt.hasNext()) 
	{
	  allLabels.push_back(rlIt.next());
	}
      saveProgress = new ProgressBar(0, 1, allLabels.size());
      saveProgress->setWindowTitle("Saving labels");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_og_plots_occurrence_labels "
		     "(plot, incident, attribute, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, zvalue, hidden, relationship) "
		     "VALUES (:plot, :incident, :attribute, :label, :curxpos, :curypos, "
		     ":xoffset, :yoffset, :red, :green, :blue, :alpha, :zvalue, :hidden, "
		     ":relationship)");
      QVectorIterator<OccurrenceLabel*> it2(allLabels);
      while (it2.hasNext()) 
	{
	  OccurrenceLabel *currentLabel = it2.next();
	  int id = currentLabel->getOccurrence()->getId();
	  QString attribute = currentLabel->getOccurrence()->getAttribute();
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
	  int relationship = 0;
	  if (currentLabel->isVisible()) 
	    {
	      hidden = 0;
	    }
	  if (_relationshipLabelVector.contains(currentLabel)) 
	    {
	      relationship = 1;
	    }
	  query->bindValue(":plot", name);
	  query->bindValue(":incident", id);
	  query->bindValue(":attribute", attribute);
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
	  query->bindValue(":relationship", relationship);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, attributeListWidget->rowCount());
      saveProgress->setWindowTitle("Saving attribute legend");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_og_plots_legend (plot, name, tip, "
		     "red, green, blue, alpha, relationship) "
		     "VALUES (:plot, :name, :tip, :red, :green, :blue, :alpha, :relationship)");
      for (int i = 0; i != attributeListWidget->rowCount(); i++) 
	{
	  QTableWidgetItem *item = attributeListWidget->item(i, 0);
	  QString title = item->data(Qt::DisplayRole).toString();
	  QString tip = item->data(Qt::ToolTipRole).toString();
	  QColor color = attributeListWidget->item(i, 1)->background().color();
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
	  query->bindValue(":relationship", 0);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, relationshipListWidget->rowCount());
      saveProgress->setWindowTitle("Saving relationship legend");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_og_plots_legend (plot, name, tip, "
		     "red, green, blue, alpha, relationship) "
		     "VALUES (:plot, :name, :tip, :red, :green, :blue, :alpha, :relationship)");
      for (int i = 0; i != relationshipListWidget->rowCount(); i++) 
	{
	  QTableWidgetItem *item = relationshipListWidget->item(i, 0);
	  QString title = item->data(Qt::DisplayRole).toString();
	  QString tip = item->data(Qt::ToolTipRole).toString();
	  QColor color = relationshipListWidget->item(i, 1)->background().color();
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
	  query->bindValue(":relationship", 1);
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
      query->prepare("INSERT INTO saved_og_plots_lines "
		     "(plot, startx, starty, endx, endy, arone, artwo, penwidth, penstyle, "
		     "zvalue, red, green, blue, alpha) "
		     "VALUES (:plot, :startx, :starty, :endx, :endy, :arone, :artwo, "
		     ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha)");
      QVectorIterator<LineObject*> it8(_lineVector);
      while (it8.hasNext()) 
	{
	  LineObject *currentLine = it8.next();
	  qreal startx = currentLine->getStartPos().x();
	  qreal starty = currentLine->getStartPos().y();
	  qreal endx = currentLine->getEndPos().x();
	  qreal endy = currentLine->getEndPos().y();
	  int zValue = currentLine->zValue();
	  QColor color = currentLine->getColor();
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
      saveProgress = new ProgressBar(0, 1, _timeLineVector.size());
      saveProgress->setWindowTitle("Saving timeline objects");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_og_plots_timelines "
		     "(plot, startx, endx, y, penwidth, majorinterval, minordivision, "
		     "majorsize, minorsize, firsttick, lasttick, zvalue, red, green, blue, alpha) "
		     "VALUES (:plot, :startx, :endx, :y, :penwidth, :majorinterval, :minordivision, "
		     ":majorsize, :minorsize, :firsttick, :lasttick, "
		     ":zvalue, :red, :green, :blue, :alpha)");
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
      saveProgress = new ProgressBar(0, 1, _textVector.size());
      saveProgress->setWindowTitle("Saving text items");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_og_plots_texts "
		     "(plot, desc, xpos, ypos, width, size, rotation, "
		     "zvalue, red, green, blue, alpha) "
		     "VALUES (:plot, :desc, :xpos, :ypos, :width, :size, :rotation, "
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
      query->prepare("INSERT INTO saved_og_plots_ellipses "
		     "(plot, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha) "
		     "VALUES (:plot, :xpos, :ypos, :topleftx, :toplefty, :toprightx, :toprighty, "
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
      query->prepare("INSERT INTO saved_og_plots_rects "
		     "(plot, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha) "
		     "VALUES (:plot, :xpos, :ypos, :topleftx, :toplefty, :toprightx, :toprighty, "
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
      saveProgress = new ProgressBar(0, 1, caseListWidget->count());
      saveProgress->setWindowTitle("Saving cases");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_og_plots_cases "
		     "(plot, casename, checked) "
		     "VALUES (:plot, :casename, :checked)");
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
	  query->bindValue(":casename", casename);
	  query->bindValue(":checked", checked);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      plotLabel->setText(name);
      changeLabel->setText("");
      delete saveProgress;
      saveProgress = new ProgressBar(0, 1, _guidesVector.size());
      saveProgress->setWindowTitle("Saving guides");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      counter = 1;
      saveProgress->show();
      query->prepare("INSERT INTO saved_og_plots_guides "
		     "(plot, xpos, ypos, horizontal) "
		     "VALUES (:plot, :xpos, :ypos, :horizontal)");
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
      QSqlDatabase::database().commit();
    }
  delete saveDialog;
}

void OccurrenceGraphWidget::seePlots() 
{
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this, OCCURRENCEGRAPH);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) 
    {
      hideAnnotationsButton->setChecked(false);
      savePlotButton->setEnabled(true);
      clearPlotButton->setEnabled(true);
      cleanUp();
      QString plot = savedPlotsDialog->getSelectedPlot();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT distance, red, green, blue "
		     "FROM saved_og_plots "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      query->first();
      _distance = query->value(0).toReal();
      int red = query->value(1).toInt();
      int green = query->value(2).toInt();
      int blue = query->value(3).toInt();
      scene->setBackgroundBrush(QBrush(QColor(red, green, blue)));
      query->prepare("SELECT lowerbound, upperbound, labelson, incidentlabelsonly, "
		     "attributelabelsonly, labelsize "
		     "FROM saved_og_plots_settings "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      query->first();
      int lowerbound = query->value(0).toInt();
      int upperbound = query->value(1).toInt();
      int labelson = query->value(2).toInt();
      int incidentlabelsonly = query->value(3).toInt();
      int attributelabelsonly = query->value(4).toInt();
      int labelsize = query->value(5).toInt();
      if (labelson == 1)
	{
	  _labelsVisible = true;
	}
      else
	{
	  _labelsVisible = false;
	}
      if (incidentlabelsonly == 1)
	{
	  _incidentLabelsOnly = true;
	}
      else
	{
	  _incidentLabelsOnly = false;
	}
      if (attributelabelsonly == 1)
	{
	  _attributeLabelsOnly = true;
	}
      else
	{
	  _attributeLabelsOnly = false;
	}
      _labelSize = labelsize;
      query->prepare("SELECT incident, ch_order, attribute, width, curxpos, curypos, orixpos, "
		     "oriypos, red, green, blue, alpha, highlighted, hred, hgreen, "
		     "hblue, halpha, zvalue, hidden, perm, relationship, grouped "
		     "FROM saved_og_plots_occurrence_items "
		     "WHERE plot = :plot ");
      query->bindValue(":plot", plot);
      query->exec();
      QVector<OccurrenceItem*> allOccurrences;
      while (query->next()) 
	{
	  int id = query->value(0).toInt();
	  int order = query->value(1).toInt();
	  QString attribute = query->value(2).toString();
	  int width = query->value(3).toInt();
	  qreal currentX = query->value(4).toReal();
	  qreal currentY = query->value(5).toReal();
	  qreal originalX = query->value(6).toReal();
	  qreal originalY = query->value(7).toReal();
	  int red = query->value(8).toInt();
	  int green = query->value(9).toInt();
	  int blue = query->value(10).toInt();
	  int alpha = query->value(11).toInt();
	  int highlighted = query->value(12).toInt();
	  int hred = query->value(13).toInt();
	  int hgreen = query->value(14).toInt();
	  int hblue = query->value(15).toInt();
	  int halpha = query->value(16).toInt();
	  int zvalue = query->value(17).toInt();
	  int hidden = query->value(18).toInt();
	  int perm = query->value(19).toInt();
	  int relationship = query->value(20).toInt();
	  int grouped = query->value(21).toInt();
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
	      toolTip = query2->value(0).toString();
	    }
	  delete query2;
	  QPointF currentPos = QPointF(currentX, currentY);
	  QPointF originalPos = QPointF(originalX, originalY);
	  OccurrenceItem *currentItem = new OccurrenceItem(width, toolTip, originalPos,
							   id, order, attribute);
	  currentItem->setPos(currentPos);
	  currentItem->setColor(QColor(red, green, blue, alpha));
	  if (highlighted == 1)
	    {
	      currentItem->setHighlight(QColor(hred, hgreen, hblue, halpha));
	    }
	  currentItem->setZValue(zvalue);
	  if (relationship == 0) 
	    {
	      _attributeOccurrenceVector.push_back(currentItem);
	    }
	  else 
	    {
	      _relationshipOccurrenceVector.push_back(currentItem);
	    }
	  allOccurrences.push_back(currentItem);
	  scene->addItem(currentItem);
	  if (hidden == 1) 
	    {
	      currentItem->hide();
	    }
	  else 
	    {
	      currentItem->show();
	    }
	  if (perm == 1) 
	    {
	      currentItem->setPermHidden(true);
	    }
	  else 
	    {
	      currentItem->setPermHidden(false);
	    }
	  if (grouped == 0)
	    {
 	      currentItem->setGrouped(false);
	    }
	  else
	    {
	      currentItem->setGrouped(true);
	    }	  
	}
      query->prepare("SELECT incident, attribute, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, zvalue, hidden, relationship "
		     "FROM saved_og_plots_occurrence_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      while (query->next()) 
	{
	  int id = query->value(0).toInt();
	  QString attribute = query->value(1).toString();
	  QString text = query->value(2).toString();
	  qreal currentX = query->value(3).toReal();
	  qreal currentY = query->value(4).toReal();
	  qreal xOffset = query->value(5).toReal();
	  qreal yOffset = query->value(6).toReal();
	  int red = query->value(7).toInt();
	  int green = query->value(8).toInt();
	  int blue = query->value(9).toInt();
	  int alpha = query->value(10).toInt();
	  int zvalue = query->value(11).toInt();
	  int hidden = query->value(12).toInt();
	  int relationship = query->value(13).toInt();
	  QVectorIterator<OccurrenceItem*> it(allOccurrences);
	  while (it.hasNext()) 
	    {
	      OccurrenceItem *currentItem = it.next();
	      if (currentItem->getId() == id && currentItem->getAttribute() == attribute) 
		{
		  OccurrenceLabel *currentLabel = new OccurrenceLabel(currentItem);
		  currentLabel->setPlainText(text);
		  currentLabel->setPos(QPointF(currentX, currentY));
		  currentLabel->setOffset(QPointF(xOffset, yOffset));
		  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
		  currentLabel->setZValue(zvalue);
		  currentItem->setLabel(currentLabel);
		  if (relationship == 0) 
		    {
		      _attributeLabelVector.push_back(currentLabel);
		    }
		  else 
		    {
		      _relationshipLabelVector.push_back(currentLabel);
		    }
		  scene->addItem(currentLabel);
		  if (hidden == 1) 
		    {
		      currentLabel->hide();
		    }
		  else 
		    {
		      currentLabel->hide();
		    }
		  break;
		}
	    }
	}
      query->prepare("SELECT name, tip, red, green, blue, alpha, relationship "
		     "FROM saved_og_plots_legend "
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
	  int relationship = query->value(6).toInt();
	  QColor color = QColor(red, green, blue, alpha);
	  QTableWidgetItem *item = new QTableWidgetItem(name);
	  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	  item->setToolTip(tip);
	  item->setData(Qt::DisplayRole, name);
	  if (relationship == 0) 
	    {
	      attributeListWidget->setRowCount(attributeListWidget->rowCount() + 1);
	      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 0, item);
	      attributeListWidget->setItem(attributeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
	      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->setBackground(color);
	      attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->
		setFlags(attributeListWidget->item(attributeListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	      _presentAttributes.push_back(name);
	    }
	  else 
	    {
	      relationshipListWidget->setRowCount(relationshipListWidget->rowCount() + 1);
	      relationshipListWidget->setItem(relationshipListWidget->rowCount() - 1, 0, item);
	      relationshipListWidget->setItem(relationshipListWidget->rowCount() - 1, 1,
					      new QTableWidgetItem);
	      relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->setBackground(color);
	      relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->
		setFlags(relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	      _presentRelationships.push_back(name);
	    }
	}
      query->prepare("SELECT startx, starty, endx, endy, arone, artwo, penwidth, penstyle, "
		     "zvalue, red, green, blue, alpha "
		     "FROM saved_og_plots_lines "
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
		     "FROM saved_og_plots_timelines "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
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
      query->prepare("SELECT desc, xpos, ypos, width, size, rotation, zvalue, "
		     "red, green, blue, alpha "
		     "FROM saved_og_plots_texts "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
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
	  newText->setTextWidth(width);
	  QFont font = newText->font();
	  font.setPointSize(size);
	  newText->setFont(font);
	  newText->setPos(xpos, ypos);
	  newText->setRotationValue(rotation);
	  scene->addItem(newText);
	}
      query->prepare("SELECT xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
		     "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
		     "penwidth, penstyle, zvalue, red, green, blue, alpha, "
		     "fillred, fillgreen, fillblue, fillalpha "
		     "FROM saved_og_plots_ellipses "
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
		     "FROM saved_og_plots_rects "
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
      query->prepare("SELECT casename, checked "
		     "FROM saved_og_plots_cases "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
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
		     "FROM saved_og_plots_guides "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
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
      fixZValues();
      caseListWidget->setEnabled(true);
      _distance = 70;
      plotLabel->setText(plot);
      changeLabel->setText("");
      scene->update();
      setRangeControls();
      lowerRangeDial->setValue(lowerbound);
      upperRangeDial->setValue(upperbound);
      setGraphControls(true);
      savePlotButton->setEnabled(true);
      clearPlotButton->setEnabled(true);
      if (_incidentLabelsOnly)
	{
	  incidentLabelsOnlyButton->setChecked(true);
	  toggleIncidentLabelsOnly();
	}
      else if(_attributeLabelsOnly)
	{
	  attributeLabelsOnlyButton->setChecked(true);
	  toggleAttributeLabelsOnly();
	}
      setVisibility();
      checkCongruence();
      delete query;
    }
  else if (savedPlotsDialog->getExitStatus() == 2) 
    {
      // DON'T FORGET TO UPDATE THIS FUNCTION!!!!
      QString plot = savedPlotsDialog->getSelectedPlot();
      QSqlQuery *query = new QSqlQuery;
      // saved_og_plots
      query->prepare("DELETE FROM saved_og_plots "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_settings
      query->prepare("DELETE FROM saved_og_plots_settings "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_occurrence_items
      query->prepare("DELETE FROM saved_og_plots_occurrence_items "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_occurrence_labels
      query->prepare("DELETE FROM saved_og_plots_occurrence_labels "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_legend
      query->prepare("DELETE FROM saved_og_plots_legend "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_lines
      query->prepare("DELETE FROM saved_og_plots_lines "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_timelines
      query->prepare("DELETE FROM saved_og_plots_timelines "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_texts
      query->prepare("DELETE FROM saved_og_plots_texts "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_ellipses
      query->prepare("DELETE FROM saved_og_plots_ellipses "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_rects
      query->prepare("DELETE FROM saved_og_plots_rects "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_cases
      query->prepare("DELETE FROM saved_og_plots_cases "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_og_plots_cases
      query->prepare("DELETE FROM saved_og_plots_guides "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      delete query;
      seePlots();
    }
}

void OccurrenceGraphWidget::clearPlot()
{
  QPointer<QMessageBox> warningBox = new QMessageBox(this);
  warningBox->setWindowTitle("Clearing plot");
  warningBox->addButton(QMessageBox::Yes);
  warningBox->addButton(QMessageBox::No);
  warningBox->setIcon(QMessageBox::Warning);
  warningBox->setText("<h2>Are you sure?</h2>");
  warningBox->setInformativeText("Clearing the plot cannot be undone. "
				 "Are you sure you want to proceed?");
  if (warningBox->exec() == QMessageBox::Yes) 
    {
      delete warningBox;
      cleanUp();
      changeLabel->setText("");
      plotLabel->setText("Unsaved plot");
    }
  else 
    {
      delete warningBox;
      return;
    }
  updateCases();
  hideAnnotationsButton->setChecked(false);
}

void OccurrenceGraphWidget::hideAnnotations()
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

void OccurrenceGraphWidget::setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr) 
{
  _eventGraphWidgetPtr = eventGraphWidgetPtr;
}

void OccurrenceGraphWidget::cleanUp() 
{
  scene->clearSelection();
  qDeleteAll(_attributeOccurrenceVector);
  _attributeOccurrenceVector.clear();
  qDeleteAll(_relationshipOccurrenceVector);
  _relationshipOccurrenceVector.clear();
  qDeleteAll(_attributeLabelVector);
  _attributeLabelVector.clear();
  qDeleteAll(_relationshipLabelVector);
  _relationshipLabelVector.clear();
  qDeleteAll(_edgeVector);
  _edgeVector.clear();
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
  attributeListWidget->setRowCount(0);
  relationshipListWidget->setRowCount(0);
  caseListWidget->clear();
  _presentAttributes.clear();
  _presentRelationships.clear();
  _checkedCases.clear();
  snapGuidesButton->setChecked(false);
  toggleSnapGuides();
  incidentLabelsOnlyButton->setChecked(false);
  attributeLabelsOnlyButton->setChecked(false);
  setGraphControls(false);
  clearPlotButton->setEnabled(false);
  savePlotButton->setEnabled(false);
  _labelSize = 10;
}

void OccurrenceGraphWidget::finalBusiness() 
{
  cleanUp();
}

bool OccurrenceGraphWidget::eventFilter(QObject *object, QEvent *event) 
{
  if (event->type() == QEvent::MouseButtonRelease)
    {
      resetZoomSlider();
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
  return false;
}
