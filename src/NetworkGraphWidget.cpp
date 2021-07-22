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

#include "../include/NetworkGraphWidget.h"

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) : QWidget(parent) 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT coder FROM save_data");
  query->first();
  _selectedCoder = query->value(0).toString();
  delete query;
  _selectedType = "";
  _selectedEntityName = "";
  _labelsShown = false;
  _massMove = false;
  _minOrder = 0;
  _maxOrder = 0;
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
  
  typeLabel = new QLabel(tr("Select type"), this);
  casesLabel = new QLabel(tr("<b>Case filtering:</b>"), graphicsWidget);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  weightLabel = new QLabel(tr("<b>Minimum occurrence:</b>"), graphicsWidget);
  nodeLegendLabel = new QLabel(tr("<b>Modes:</b>"), legendWidget);
  edgeLegendLabel = new QLabel(tr("<b>Edges:</b>"), legendWidget);
  nameLabel = new QLabel(tr("<b>Name:</b<"), infoWidget);
  descriptionLabel = new QLabel(tr("<b>Description:</b>"), infoWidget);
  attributesLabel = new QLabel(tr("<b>Attributes:</b>"), infoWidget);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), infoWidget);
  valueLabel = new QLabel(tr("<b>Value:</b>"), infoWidget);
  plotLabel = new QLabel(tr("Unsaved plot"), this);
  changeLabel = new QLabel(tr("*"), this);
  incongruenceLabel = new QLabel(tr(""), this);
  incongruenceLabel->setStyleSheet("QLabel {color : red;}");
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
  
  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);

  layoutComboBox = new QComboBox(this);
  layoutComboBox->addItem(SPRINGLAYOUT);
  layoutComboBox->addItem(CIRCULARLAYOUT);
  
  nameField = new QLineEdit(infoWidget);
  nameField->setReadOnly(true);
  attributesFilterField = new QLineEdit(infoWidget);
  valueField = new QLineEdit(infoWidget);
  valueField->setEnabled(false);
  
  descriptionField = new QTextEdit(infoWidget);
  descriptionField->setReadOnly(true);

  weightCheckBox = new QCheckBox(tr("Visualise"));
  weightCheckBox->setCheckState(Qt::Unchecked);
  
  plotButton = new QPushButton(tr("Plot new"), this);
  plotButton->setEnabled(false);
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
  valueButton = new QPushButton(tr("Store value"), infoWidget);
  valueButton->setEnabled(false);
  assignAttributeButton = new QPushButton(tr("Assign attribute"), infoWidget);
  assignAttributeButton->setEnabled(false);
  unassignAttributeButton = new QPushButton(tr("Unassign attribute"), infoWidget);
  unassignAttributeButton->setEnabled(false);
  addAttributeButton = new QPushButton(tr("New attribute"), infoWidget);
  editAttributeButton = new QPushButton(tr("Edit attribute"), infoWidget);
  editAttributeButton->setEnabled(false);
  removeUnusedAttributesButton = new QPushButton(tr("Remove unused"), infoWidget);
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  addButton = new QPushButton(tr("Add relationship type"), this);
  addButton->setEnabled(false);
  toggleLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  increaseFontSizeButton = new QPushButton(tr("+"));
  decreaseFontSizeButton = new QPushButton(tr("-"));
  addModeButton = new QPushButton(tr("Create single mode"), legendWidget);
  addModesButton = new QPushButton(tr("Create multiple modes"), legendWidget);
  nodeColorButton = new QPushButton(tr("Set node color"), graphicsWidget);
  labelColorButton = new QPushButton(tr("Set label color"), graphicsWidget);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  exportNodesButton = new QPushButton(tr("Export nodes"), graphicsWidget);
  exportEdgesButton = new QPushButton(tr("Export edges"), graphicsWidget);
  exportRelationalEventsButton = new QPushButton(tr("Export relevents"), graphicsWidget);
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
  multimodeButton = new QPushButton(tr("Multimode transformation"), legendWidget);
  removeModeButton = new QPushButton(tr("Remove mode"), legendWidget);
  removeModeButton->setEnabled(false);
  hideModeButton = new QPushButton(tr("Hide"), legendWidget);
  hideModeButton->setCheckable(true);
  hideModeButton->setChecked(false);
  showModeButton = new QPushButton(tr("Show"), legendWidget);
  showModeButton->setCheckable(true);
  showModeButton->setChecked(true);
  mergeButton = new QPushButton(tr("Merge"), legendWidget);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  plotButton->setEnabled(false);
  removeTypeButton = new QPushButton(tr("Remove"), legendWidget);
  removeTypeButton->setEnabled(false);
  restoreModeColorsButton = new QPushButton(tr("Restore modes"), legendWidget);
  moveModeUpButton = new QPushButton(tr("Up"), this);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton = new QPushButton(tr("Down"), this);
  moveModeDownButton->setEnabled(false);
  setTimeRangeButton = new QPushButton(tr("Set time range"), graphicsWidget);
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
  
  lowerRangeDial = new QDial(graphicsWidget);
  lowerRangeDial->setSingleStep(1);
  upperRangeDial = new QDial(graphicsWidget);
  upperRangeDial->setSingleStep(1);
  lowerRangeSpinBox = new QSpinBox(graphicsWidget);
  upperRangeSpinBox = new QSpinBox(graphicsWidget);
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

  caseListWidget = new QListWidget(graphicsWidget);
  caseListWidget->setEnabled(false);
  
  view->viewport()->installEventFilter(this);
  attributesTreeView->installEventFilter(this);
  
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
          this, SLOT(setFilter(const QString &)));
  connect(attributesTreeView->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(setButtons()));
  connect(attributesTreeView, SIGNAL(noneSelected()), this, SLOT(setButtons()));
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
  connect(layoutButton, SIGNAL(clicked()), this, SLOT(makeLayout()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotNewGraph()));
  connect(addButton, SIGNAL(clicked()), this, SLOT(addRelationshipType()));
  connect(removeTypeButton, SIGNAL(clicked()), this, SLOT(removeRelationshipType()));
  connect(addModeButton, SIGNAL(clicked()), this, SLOT(addMode()));
  connect(addModesButton, SIGNAL(clicked()), this, SLOT(addModes()));
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
  connect(setTimeRangeButton, SIGNAL(clicked()), this, SLOT(setTimeRange()));
  connect(weightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setVisibility()));
  connect(weightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setVisibility()));
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
          this, SLOT(disableLegendButtons()));
  connect(nodeListWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
          this, SLOT(changeModeColor(QTableWidgetItem *)));
  connect(caseListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(checkCases()));
  connect(setFilteredButton, SIGNAL(clicked()), this, SLOT(activateFilter()));
  connect(unsetFilteredButton, SIGNAL(clicked()), this, SLOT(deactivateFilter()));
  connect(hideTypeButton, SIGNAL(clicked()), this, SLOT(hideType()));
  connect(showTypeButton, SIGNAL(clicked()), this, SLOT(showType()));
  connect(hideModeButton, SIGNAL(clicked()), this, SLOT(hideMode()));
  connect(showModeButton, SIGNAL(clicked()), this, SLOT(showMode()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(exportNodesButton, SIGNAL(clicked()), this, SLOT(exportNodes()));
  connect(exportEdgesButton, SIGNAL(clicked()), this, SLOT(exportEdges()));
  connect(exportRelationalEventsButton, SIGNAL(clicked()), this, SLOT(exportRelationalEvents()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
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
  connect(scene, SIGNAL(relevantChange()), this, SLOT(setChangeLabel()));
  connect(scene, SIGNAL(relevantChange()), this, SLOT(updateEdges()));
  connect(scene, SIGNAL(moveItems(QGraphicsItem *, QPointF)),
          this, SLOT(processMoveItems(QGraphicsItem *, QPointF)));
  connect(scene, SIGNAL(NetworkNodeContextMenuAction(const QString &)),
          this, SLOT(processNetworkNodeContextMenu(const QString &)));
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
  connect(restoreModeColorsButton, SIGNAL(clicked()), this, SLOT(restoreModeColors()));
  connect(moveModeUpButton, SIGNAL(clicked()), this, SLOT(moveModeUp()));
  connect(moveModeDownButton, SIGNAL(clicked()), this, SLOT(moveModeDown()));
  connect(contractLayoutButton, SIGNAL(clicked()), this, SLOT(contractLayout()));
  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(processZoomSliderChange(int)));
  connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(resetZoomSlider()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
  
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
  plotOptionsLayout->addWidget(incongruenceLabel);

  topLayout->addLayout(plotOptionsLayout);
  plotOptionsLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(topLayout);
  QPointer<QFrame> topLine = new QFrame();
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

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
  QPointer<QHBoxLayout> hideShowModeButtonsLayout = new QHBoxLayout;
  hideShowModeButtonsLayout->addWidget(hideModeButton);
  hideShowModeButtonsLayout->addWidget(showModeButton);
  legendLayout->addLayout(hideShowModeButtonsLayout);
  legendLayout->addWidget(addModeButton);
  legendLayout->addWidget(addModesButton);
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
  legendWidget->setMinimumWidth(250);
  legendWidget->setMaximumWidth(250);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				     
  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(casesLabel);
  graphicsControlsLayout->addWidget(caseListWidget);
  QPointer<QFrame> sepLine3 = new QFrame();
  sepLine3->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine3);
  graphicsControlsLayout->addWidget(nodeColorButton);
  graphicsControlsLayout->addWidget(labelColorButton);
  graphicsControlsLayout->addWidget(backgroundColorButton);
  graphicsControlsLayout->addWidget(toggleLabelsButton);
  graphicsControlsLayout->addWidget(labelSizeLabel);
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
  graphicsControlsLayout->addWidget(setTimeRangeButton);
  graphicsControlsLayout->addWidget(weightLabel);
  QPointer<QBoxLayout> weightLayout = new QHBoxLayout;
  weightLayout->addWidget(weightSpinBox);
  weightLayout->addWidget(weightCheckBox);
  graphicsControlsLayout->addLayout(weightLayout);
  QPointer<QFrame> sepLine5 = new QFrame();
  sepLine5->setFrameShape(QFrame::HLine);
  graphicsControlsLayout->addWidget(sepLine5);
  graphicsControlsLayout->addWidget(exportSvgButton);
  graphicsControlsLayout->addWidget(exportNodesButton);
  graphicsControlsLayout->addWidget(exportEdgesButton);
  graphicsControlsLayout->addWidget(exportRelationalEventsButton);
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
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);

  setLayout(mainLayout);

  getTypes();
  updateCases();
  
  disableFilterButtons();
  infoWidget->hide();
  graphicsWidget->hide();
  legendWidget->hide();
  setGraphControls(false);
}

NetworkGraphWidget::~NetworkGraphWidget()
{
  qDeleteAll(_directedVector);
  _directedVector.clear();
  qDeleteAll(_undirectedVector);
  _undirectedVector.clear();
  qDeleteAll(_networkNodeVector);
  _networkNodeVector.clear();
  qDeleteAll(_networkNodeLabelVector);
  _networkNodeLabelVector.clear();
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

void NetworkGraphWidget::setOpenGL(bool state)
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

void NetworkGraphWidget::setAntialiasing(bool state)
{
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext())
  {
    DirectedEdge *current = it.next();
    current->setAntialiasing(state);
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext())
  {
    UndirectedEdge *current = it2.next();
    current->setAntialiasing(state);
  }
}


void NetworkGraphWidget::checkCongruence() 
{
  if (_networkNodeVector.size() > 0) 
  {
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT name, description FROM entities ORDER BY name ASC");
    QVector<QString> tempNames;
    QVector<QString> tempDescs;
    while (query->next())
    {
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
    while (query->next())
    {
      QString type = query->value(0).toString();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT name "
                      "FROM entity_relationships "
                      "WHERE type = :type");
      query2->bindValue(":type", type);
      query2->exec();
      while (query2->next())
      {
        QString relationship = query2->value(0).toString();
        tempDirected.push_back(relationship);
      }
      delete query2;
    }
    query->prepare("SELECT name FROM relationship_types "
                   "WHERE directedness = :undirected");
    query->bindValue(":undirected", UNDIRECTED);
    query->exec();
    while (query->next())
    {
      QString type = query->value(0).toString();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT name "
                      "FROM entity_relationships "
                      "WHERE type = :type");
      query2->bindValue(":type", type);
      query2->exec();
      while (query2->next())
      {
        QString relationship = query2->value(0).toString();
        tempUndirected.push_back(relationship);
      }
      delete query2;
    }
    std::sort(_networkNodeVector.begin(), _networkNodeVector.end(), nameLessThan);
    if (tempNames.size() != _networkNodeVector.size())
    {
      incongruenceLabel->setText("Incongruence detected");
      delete query;
      return;
    }
    for (QVector<NetworkNode*>::size_type i = 0; i != _networkNodeVector.size(); i++)
    {
      NetworkNode *current = _networkNodeVector[i];
      if (current->getName() != tempNames[i])
      {
        incongruenceLabel->setText("Incongruence detected");
        delete query;
        return;
      }
      if (current->getDescription() != tempDescs[i])
      {
        incongruenceLabel->setText("Incongruence detected");
        delete query;
        return;
      }
    }
    query->exec("SELECT COUNT(*) FROM entity_relationships");
    query->first();
    int originalRelationships = query->value(0).toInt();
    int count = 0;
    QVectorIterator<DirectedEdge*> it(_directedVector);
    while (it.hasNext())
    {
      DirectedEdge *current = it.next();
      if (current->getName() != MERGED && current->getName() != TRANSFORMED)
      {
        count++;
      }
    }
    QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
    while (it2.hasNext())
    {
      UndirectedEdge *current = it2.next();
      if (current->getName() != MERGED && current->getName() != TRANSFORMED)
      {
        count++;
      }
    }
    if (count != originalRelationships)
    {
      incongruenceLabel->setText("Incongruence detected");
      delete query;
      return;
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

void NetworkGraphWidget::toggleDetails() 
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

void NetworkGraphWidget::toggleGraphicsControls() 
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

void NetworkGraphWidget::toggleLegend() 
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

void NetworkGraphWidget::rescale()
{
  view->scale(2.0, 2.0);
  view->scale(0.5, 0.5);
}

void NetworkGraphWidget::processZoomSliderChange(int value)
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

void NetworkGraphWidget::resetZoomSlider()
{
  zoomSlider->blockSignals(true);
  zoomSlider->setValue(0);
  zoomSlider->blockSignals(false);
}


void NetworkGraphWidget::setGraphControls(bool state)
{
  zoomSlider->setEnabled(state);
  expandLayoutButton->setEnabled(state);
  contractLayoutButton->setEnabled(state);
  lowerRangeDial->setEnabled(state);
  upperRangeDial->setEnabled(state);
  lowerRangeSpinBox->setEnabled(state);
  upperRangeSpinBox->setEnabled(state);
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
  setTimeRangeButton->setEnabled(state);
  restoreModeColorsButton->setEnabled(state);
  addModeButton->setEnabled(state);
  addModesButton->setEnabled(state);
  multimodeButton->setEnabled(state);
  toggleLabelsButton->setEnabled(state);
  nodeColorButton->setEnabled(state);
  labelColorButton->setEnabled(state);
  backgroundColorButton->setEnabled(state);
  increaseFontSizeButton->setEnabled(state);
  decreaseFontSizeButton->setEnabled(state);
  exportSvgButton->setEnabled(state);
  exportNodesButton->setEnabled(state);
  exportEdgesButton->setEnabled(state);
  exportRelationalEventsButton->setEnabled(state);
  weightCheckBox->setEnabled(state);
  lowerRangeDial->setEnabled(state);
  upperRangeDial->setEnabled(state);
  lowerRangeSpinBox->setEnabled(state);
  upperRangeSpinBox->setEnabled(state);
}

void NetworkGraphWidget::checkCases() 
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

bool NetworkGraphWidget::typesPresent() 
{
  return _presentTypes.size() > 0;
}

void NetworkGraphWidget::retrieveData() 
{
  attributesTreeView->resetSelection();
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->blockSignals(false);
  valueField->setEnabled(false);
  valueButton->setEnabled(false);
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
      UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(it.peekNext());
      if (node && !(directed) && !(undirected))
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
      resetFont(attributesTree);
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute FROM attributes_to_entities "
                     "WHERE entity = :name");
      query->bindValue(":name", name);
      query->exec();
      while (query->next())
      {
        QString attribute = query->value(0).toString();
        boldSelected(attributesTree, attribute);
      }
      delete query;
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
    resetFont(attributesTree);
  }
}

void NetworkGraphWidget::setTree() 
{
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM entity_attributes WHERE father = 'NONE'");
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
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void NetworkGraphWidget::resetTree() 
{
  delete attributesTree;
  setTree();
}

void NetworkGraphWidget::buildHierarchy(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entity_attributes WHERE  father = :father");
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

void NetworkGraphWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
  {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
    QFont font;
    font.setBold(false);
    font.setItalic(false);
    currentAttribute->setFont(font);
    if (model->hasChildren(index))
    {
      resetFont(model, index);
    }
  }
}

void NetworkGraphWidget::boldSelected(QAbstractItemModel *model, QString name,
                                      QString entity, QModelIndex parent)
{
  for(int i = 0; i != model->rowCount(parent); i++) 
  {
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
    if (name == currentName)
    {
      if (currentAttribute->font().italic())
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
          QSqlQuery *query = new QSqlQuery;
          query->prepare("SELECT attribute, entity FROM attributes_to_entities "
                         "WHERE attribute = :attribute AND entity = :entity");
          query->bindValue(":attribute", parentName);
          query->bindValue(":entity", entity);
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
      }
    }
    if (model->hasChildren(index))
    {
      boldSelected(model, name, entity, index);
    }
  }
}

void NetworkGraphWidget::fixTree(QString entity) 
{
  resetFont(attributesTree);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT attribute FROM attributes_to_entities "
                 "WHERE entity = :entity");
  query->bindValue(":entity", entity);
  query->exec();
  while (query->next()) 
  {
    QString attribute = query->value(0).toString();
    boldSelected(attributesTree, attribute, entity);
  }
  delete query;
}

void NetworkGraphWidget::setFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void NetworkGraphWidget::previousDataItem() 
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
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
                   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next())
    {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;
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
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
                   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next())
    {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;
  }
}

void NetworkGraphWidget::nextDataItem() 
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
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
                   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next())
    {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;
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
    resetFont(attributesTree);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT attribute FROM attributes_to_entities "
                   "WHERE entity = :name");
    query->bindValue(":name", name);
    query->exec();
    while (query->next())
    {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute, name);
    }
    delete query;
  }
}

void NetworkGraphWidget::setValueButton() 
{
  valueButton->setEnabled(true);
}

void NetworkGraphWidget::setValue() 
{
  if (attributesTreeView->currentIndex().isValid()) 
  {
    QSqlQuery *query = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    query->prepare("UPDATE attributes_to_entities "
                   "SET value = :val "
                   "WHERE attribute = :attribute AND entity = :entity");
    query->bindValue(":val", valueField->text());
    query->bindValue(":attribute", attribute);
    query->bindValue(":entity", _selectedEntityName);
    query->exec();
    valueButton->setEnabled(false);
    delete query;
  }
}

void NetworkGraphWidget::assignAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
  {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    bool empty = false;
    query->prepare("SELECT attribute FROM "
                   "attributes_to_entities WHERE "
                   "attribute = :att AND entity = :entity");
    query->bindValue(":att", attribute);
    query->bindValue(":entity", _selectedEntityName);
    query->exec();
    query->first();
    empty = query->isNull(0);
    if (empty)
    {
      query->prepare("INSERT INTO attributes_to_entities (attribute, entity) "
                     "VALUES (:attribute, :entity)");
      query->bindValue(":attribute", attribute);
      query->bindValue(":entity", _selectedEntityName);
      query->exec();
      boldSelected(attributesTree, attribute, _selectedEntityName);
      valueField->setEnabled(true);
    }
    setButtons();
    delete query;
  }
}

void NetworkGraphWidget::unassignAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
  {
    QSqlQuery *query = new QSqlQuery;
    QSqlQuery *query2 = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    bool empty = false;
    query->prepare("SELECT attribute, entity "
                   "FROM attributes_to_entities "
                   "WHERE attribute = :att AND entity = :entity");
    query->bindValue(":att", attribute);
    query->bindValue(":entity", _selectedEntityName);
    query->exec();
    query->first();
    empty = query->isNull(0);
    if (!empty)
    {
      query->prepare("DELETE FROM attributes_to_entities "
                     "WHERE attribute = :att AND entity = :entity");
      query->bindValue(":att", attribute);
      query->bindValue(":entity", _selectedEntityName);
      query->exec();
      resetFont(attributesTree);
      query2->prepare("SELECT attribute, entity FROM attributes_to_entities "
                      "WHERE entity = :entity");
      query2->bindValue(":entity", _selectedEntityName);
      query2->exec();
      while (query2->next())
      {
        QString attribute = query2->value(0).toString();
        boldSelected(attributesTree, attribute, _selectedEntityName);
      }
      valueField->setText("");
      valueField->setEnabled(false);
      valueButton->setEnabled(false);
    }
    delete query;
    delete query2;
    setButtons();
  }
}

void NetworkGraphWidget::addAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
  {
    QString currentParent = treeFilter->mapToSource(attributesTreeView->
                                                    currentIndex()).data().toString();
    QString name = "";
    QString description = "";
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, ENTITY);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0)
    {
      QSqlQuery *query = new QSqlQuery;
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *attribute = new QStandardItem(name);
      attribute->setToolTip(description);
      QStandardItem *father = attributesTree->
        itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
      father->appendRow(attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);
      query->prepare("INSERT INTO entity_attributes (name, description, father) "
                     "VALUES (:name, :description, :father)");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":father", currentParent);
      query->exec();
      delete query;
    }
    delete attributeDialog;
  }
  else 
  {
    QSqlQuery *query = new QSqlQuery;
    QString name = "";
    QString description = "";
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, ENTITY);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0)
    {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      query->prepare("INSERT INTO entity_attributes (name, description, father) "
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

void NetworkGraphWidget::editAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
  {
    QString name = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entity_attributes WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, ENTITY);
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

void NetworkGraphWidget::removeUnusedAttributes() 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  while (unfinished) 
  {
    query->exec("SELECT name FROM entity_attributes EXCEPT SELECT attribute "
                "FROM attributes_to_entities EXCEPT SELECT father "
                "FROM entity_attributes");
    while (query->next())
    {
      QString current = query->value(0).toString();
      query2->prepare("DELETE FROM entity_attributes WHERE name = :current");
      query2->bindValue(":current", current);
      query2->exec();
    }
    query->first();
    if (query->isNull(0))
    {
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

void NetworkGraphWidget::getTypes() 
{
  typeComboBox->clear();
  typeComboBox->addItem(DEFAULT);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM relationship_types ORDER BY name ASC");
  while (query->next()) 
  {
    QString currentType = query->value(0).toString();
    typeComboBox->addItem(currentType);
  }
  delete query;
}

void NetworkGraphWidget::getEntities() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM entities");
  while (query->next()) 
  {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    NetworkNode *currentNode = new NetworkNode(name, description);
    NetworkNodeLabel *label = new NetworkNodeLabel(currentNode);
    currentNode->setColor(Qt::black);
    label->setPlainText(name);
    label->setDefaultTextColor(Qt::black);
    _networkNodeLabelVector.push_back(label);
    currentNode->setLabel(label);
    label->setZValue(4);
    label->hide();
    currentNode->setZValue(3);
    currentNode->hide();
    _networkNodeVector.push_back(currentNode);
  }
  delete query;
}

void NetworkGraphWidget::plotEntities() 
{
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *currentNode = it.next();
    scene->addItem(currentNode);
    scene->addItem(currentNode->getLabel());
  }
}

void NetworkGraphWidget::getDirectedEdges() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM relationship_types "
              "WHERE directedness = 'Directed'");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT name, source, target, comment "
                  "FROM entity_relationships "
                  "WHERE type = :type");
  QSqlQuery *query3 = new QSqlQuery;
  query3->prepare("SELECT incident FROM relationships_to_incidents "
                  "WHERE type = :type AND relationship = :relationship AND coder = :coder");
  QSqlQuery *query4 = new QSqlQuery;
  query4->prepare("SELECT ch_order FROM incidents "
                  "WHERE id = :id");
  while (query->next()) 
  {
    QString type = query->value(0).toString();
    query2->bindValue(":type", type);
    query2->exec();
    while (query2->next())
    {
      QString name  = query2->value(0).toString();
      QString source = query2->value(1).toString();
      QString target = query2->value(2).toString();
      QString comment = query2->value(3).toString();
      QVectorIterator<NetworkNode*> it(_networkNodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext())
      {
        NetworkNode* currentNode = it.next();
        if (currentNode->getName() == source)
        {
          tempSource = currentNode;
        }
        else if (currentNode->getName() == target)
        {
          tempTarget = currentNode;
        }
        if (tempSource != NULL && tempTarget != NULL)
        {
          DirectedEdge *currentEdge = new DirectedEdge(tempSource, tempTarget, type, name, 0);
          query3->bindValue(":type", type);
          query3->bindValue(":relationship", name);
          query3->bindValue(":coder", _selectedCoder);
          query3->exec();
          QSet<int> incidents;
          while (query3->next())
          {
            int currentIncident = query3->value(0).toInt();
            query4->bindValue(":id", currentIncident);
            query4->exec();
            query4->first();
            incidents.insert(query4->value(0).toInt());
          }
          currentEdge->setIncidents(incidents);
          currentEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
          currentEdge->setComment(comment);
          currentEdge->hide();
          _directedVector.push_back(currentEdge);
          break;
        }
      }
    }
  }
  delete query;
  delete query2;
  delete query3;
  delete query4;
}

void NetworkGraphWidget::getUndirectedEdges() 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM relationship_types "
                 "WHERE directedness = :directed");
  query->bindValue(":directed", "Undirected");
  query->exec();
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT name, source, target, comment "
                  "FROM entity_relationships "
                  "WHERE type = :type");
  QSqlQuery *query3 = new QSqlQuery;
  query3->prepare("SELECT incident FROM relationships_to_incidents "
                  "WHERE type = :type AND relationship = :relationship AND coder = :coder");
  QSqlQuery *query4 = new QSqlQuery;
  query4->prepare("SELECT ch_order FROM incidents "
                  "WHERE id = :id");
  while (query->next()) 
  {
    QString type = query->value(0).toString();
    query2->bindValue(":type", type);
    query2->exec();
    while (query2->next())
    {
      QString name = query2->value(0).toString();
      QString source = query2->value(1).toString();
      QString target = query2->value(2).toString();
      QString comment = query2->value(3).toString();
      QVectorIterator<NetworkNode*> it(_networkNodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext())
      {
        NetworkNode* currentNode = it.next();
        if (currentNode->getName() == source)
        {
          tempSource = currentNode;
        }
        else if (currentNode->getName() == target)
        {
          tempTarget = currentNode;
        }
        if (tempSource != NULL && tempTarget != NULL)
        {
          UndirectedEdge *currentEdge;
          if (tempSource->getName() < tempTarget->getName())
          {
            currentEdge = new UndirectedEdge(tempSource, tempTarget, type, name, 0);
          }
          else
          {
            currentEdge = new UndirectedEdge(tempTarget, tempSource, type, name, 0);
          }
          query3->bindValue(":type", type);
          query3->bindValue(":relationship", name);
          query3->bindValue(":coder", _selectedCoder);
          query3->exec();
          QSet<int> incidents;
          while (query3->next())
          {
            int currentIncident = query3->value(0).toInt();
            query4->bindValue(":id", currentIncident);
            query4->exec();
            query4->first();
            incidents.insert(query4->value(0).toInt());
          }
          currentEdge->setIncidents(incidents);

          currentEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
          currentEdge->setComment(comment);
          currentEdge->hide();
          _undirectedVector.push_back(currentEdge);
          break;
        }
      }
    }
  }
  delete query;
  delete query2;
  delete query3;
  delete query4;
}

void NetworkGraphWidget::plotDirectedEdges(QString type, QColor color) 
{
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext()) 
  {
    DirectedEdge* currentEdge = it.next();
    if (currentEdge->getType() == type)
    {
      currentEdge->setColor(color);
      currentEdge->hide();
      currentEdge->getStart()->hide();
      currentEdge->getEnd()->hide();
      scene->addItem(currentEdge);
    }
  }
  processHeights();
  updateEdges();
}

void NetworkGraphWidget::plotUndirectedEdges(QString type, QColor color) 
{
  QVectorIterator<UndirectedEdge*> it(_undirectedVector);
  while (it.hasNext()) 
  {
    UndirectedEdge* currentEdge = it.next();
    if (currentEdge->getType() == type)
    {
      currentEdge->setColor(color);
      currentEdge->hide();
      currentEdge->getStart()->hide();
      currentEdge->getEnd()->hide();
      scene->addItem(currentEdge);
    }
  }
  processHeights();
  updateEdges();
}

void NetworkGraphWidget::makeLayout()
{
  if (layoutComboBox->currentText() == SPRINGLAYOUT)
  {
    springLayout();
  }
  else if (layoutComboBox->currentText() == CIRCULARLAYOUT)
  {
    circularLayout();
  }
}

void NetworkGraphWidget::springLayout() 
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
    UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(current);
    NetworkNode *node = qgraphicsitem_cast<NetworkNode*>(current);
    if (directed)
    {
      edges.push_back(directed);
    }
    else if (undirected)
    {
      edges.push_back(undirected);
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
      UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(current);
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
      else if (undirected)
      {
        if (undirected->isVisible())
        {
          NetworkNode *tail = undirected->getStart();
          NetworkNode *head = undirected->getEnd();
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
  QVectorIterator<NetworkNode*> it5(_networkNodeVector);
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
      QVectorIterator<NetworkNode*> it6(_networkNodeVector);
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

void NetworkGraphWidget::circularLayout() 
{
  const qreal Pi = 3.14;
  QVector<NetworkNode*> nodes;
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
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
void NetworkGraphWidget::expandLayout() 
{
  QPointF virtualCenter = QPointF(0,0);
  qreal total = 0.0;
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    virtualCenter += current->scenePos();
    total = total + 1.0;
  }
  virtualCenter /= total;
  QVectorIterator<NetworkNode*> it2(_networkNodeVector);
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
void NetworkGraphWidget::contractLayout() 
{
  QPointF virtualCenter = QPointF(0,0);
  int total = 0;
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    virtualCenter += current->scenePos();
    total++;
  }
  virtualCenter /= total;
  QVectorIterator<NetworkNode*> it2(_networkNodeVector);
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

void NetworkGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) 
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
    QVectorIterator<NetworkNode*> it3(_networkNodeVector);
    while (it3.hasNext())
    {
      NetworkNode *first = it3.next();
      if (first->isSelected())
      {
        QVectorIterator<NetworkNode*> it4(_networkNodeVector);
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

void NetworkGraphWidget::processNetworkNodeContextMenu(const QString &action) 
{
  if (action == SETPERSISTENTACTION) 
  {
    setNodePersistence(true);
  }
  else if (action == UNSETPERSISTENTACTION) 
  {
    setNodePersistence(false);
  }
  else if (action == RECOLORNODESACTION)
  {
    recolorNodes();
  }
  else if (action == RECOLORNODELABELSACTION)
  {
    recolorLabels();
  }
}

void NetworkGraphWidget::setNodePersistence(bool state) 
{
  if (scene->selectedItems().size() > 0) 
  {
    QListIterator<QGraphicsItem*> it(scene->selectedItems());
    while (it.hasNext())
    {
      NetworkNode *currentNode = qgraphicsitem_cast<NetworkNode*>(it.peekNext());
      if (currentNode)
      {
        NetworkNode *selectedNode = qgraphicsitem_cast<NetworkNode*>(it.next());
        selectedNode->setPersistent(state);
      }
    }
  }
  setVisibility();
}

void NetworkGraphWidget::recolorNodes()
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
        NetworkNode *node = qgraphicsitem_cast<NetworkNode *>(it.peekNext());
        if (node)
        {
          NetworkNode *currentNode = qgraphicsitem_cast<NetworkNode *>(it.next());
          currentNode->setColor(color);
        }
        else
        {
          it.next();
        }
      }
    }
  }
}

void NetworkGraphWidget::recolorLabels() 
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
        NetworkNode *node = qgraphicsitem_cast<NetworkNode*>(it.peekNext());
        if (node)
        {
          NetworkNode *currentNode = qgraphicsitem_cast<NetworkNode*>(it.next());
          NetworkNodeLabel *currentLabel = currentNode->getLabel();
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

void NetworkGraphWidget::setPenStyle()
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

void NetworkGraphWidget::setPenWidth()
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

void NetworkGraphWidget::setLineColor()
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

void NetworkGraphWidget::setFillColor()
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

void NetworkGraphWidget::setFillOpacity(int value)
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

void NetworkGraphWidget::addLineObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  _lineVector.push_back(newLineObject);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_networkNodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void NetworkGraphWidget::addSingleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_networkNodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void NetworkGraphWidget::addDoubleArrowObject(const QPointF &start, const QPointF &end) 
{
  LineObject *newLineObject = new LineObject(start, end);
  newLineObject->setArrow1(true);
  newLineObject->setArrow2(true);
  newLineObject->setPenStyle(_currentPenStyle);
  newLineObject->setPenWidth(_currentPenWidth);
  newLineObject->setColor(_currentLineColor);
  _lineVector.push_back(newLineObject);
  scene->addItem(newLineObject);
  newLineObject->setZValue(_networkNodeVector[0]->zValue() + 2);
  newLineObject->setSelected(true);
}

void NetworkGraphWidget::addEllipseObject(const QRectF &area)
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
  newEllipse->setZValue(_networkNodeVector[0]->zValue() + 2);
  newEllipse->setSelected(true);
}

void NetworkGraphWidget::addRectObject(const QRectF &area) 
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
  newRect->setZValue(_networkNodeVector[0]->zValue() + 2);
  newRect->setSelected(true);
}

void NetworkGraphWidget::addTextObject(const QRectF &area, const qreal &size)
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
    newText->setZValue(_networkNodeVector[0]->zValue() + 3);
    newText->adjustSize();
    newText->setSelected(true);
  }
  delete textDialog;
}

void NetworkGraphWidget::addHorizontalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(true);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void NetworkGraphWidget::addVerticalGuideLine(const QPointF &pos)
{
  GuideLine *guide = new GuideLine(false);
  _guidesVector.push_back(guide);
  scene->addItem(guide);
  guide->setOrientationPoint(pos);
  fixZValues();
}

void NetworkGraphWidget::toggleSnapGuides()
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

void NetworkGraphWidget::processShapeSelection()
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

void NetworkGraphWidget::processLineContextMenu(const QString &action) 
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

void NetworkGraphWidget::toggleArrow1() 
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

void NetworkGraphWidget::toggleArrow2() 
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

void NetworkGraphWidget::deleteLine() 
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

void NetworkGraphWidget::duplicateLine() 
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

void NetworkGraphWidget::processTextContextMenu(const QString &action) 
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

void NetworkGraphWidget::changeText() 
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

void NetworkGraphWidget::deleteText() 
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

void NetworkGraphWidget::duplicateText() 
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

void NetworkGraphWidget::processEllipseContextMenu(const QString &action) 
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

void NetworkGraphWidget::deleteEllipse() 
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

void NetworkGraphWidget::duplicateEllipse() 
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

void NetworkGraphWidget::processRectContextMenu(const QString &action) 
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

void NetworkGraphWidget::deleteRect() 
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

void NetworkGraphWidget::duplicateRect() 
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

void NetworkGraphWidget::processGuideLineContextMenu(const QString &action) 
{
  if (action == DELETEGUIDEACTION) 
  {
    deleteGuideLine();
  }
}

void NetworkGraphWidget::deleteGuideLine()
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

void NetworkGraphWidget::objectOneForward() 
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

void NetworkGraphWidget::objectOneBackward() 
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

void NetworkGraphWidget::objectToFront() 
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

void NetworkGraphWidget::objectToBack() 
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

void NetworkGraphWidget::fixZValues() 
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

void NetworkGraphWidget::addMode() 
{
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, ENTITY);
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) 
  {
    QSqlQuery *query = new QSqlQuery;
    QSqlQuery *query2 = new QSqlQuery;
    query->prepare("SELECT description FROM entity_attributes "
                   "WHERE name = :name");
    query2->prepare("SELECT entity FROM attributes_to_entities "
                    "WHERE attribute = :currentAttribute");
    setChangeLabel();
    QColor color = attributeColorDialog->getColor();
    QColor textColor = attributeColorDialog->getTextColor();
    QString attribute = attributeColorDialog->getAttribute();
    query->bindValue(":name", attribute);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QVector<QString> attributeVector;
    attributeVector.push_back(attribute);
    findChildren(attribute, &attributeVector);
    QVectorIterator<QString> it(attributeVector);
    while (it.hasNext())
    {
      QString currentAttribute = it.next();
      query2->bindValue(":currentAttribute", currentAttribute);
      query2->exec();
      while (query2->next())
      {
        QString currentName = query2->value(0).toString();
        QVectorIterator<NetworkNode*> it2(_networkNodeVector);
        while (it2.hasNext())
        {
          NetworkNode* currentNode = it2.next();
          if (currentNode->getName() == currentName)
          {
            currentNode->setColor(color);
            currentNode->setMode(attribute);
            currentNode->getLabel()->setDefaultTextColor(textColor);
            currentNode->setMassHidden(false);
          }
        }
      }
    }
    bool found = false;
    for (int i = 0; i < nodeListWidget->rowCount(); i++)
    {
      if (nodeListWidget->item(i, 0)->data(Qt::DisplayRole) == attribute)
      {
        found = true;
        QTableWidgetItem *item = nodeListWidget->item(i,0);
        QString toolTip = breakString(attribute + " - " + description);
        item->setToolTip(toolTip);
        nodeListWidget->item(i, 1)->setBackground(color);
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
      nodeListWidget->setRowCount(nodeListWidget->rowCount() + 1);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 0, item);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setBackground(color);
      QVariant textColorVar = QVariant(textColor.rgb());
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setData(Qt::UserRole, textColorVar);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->
        setFlags(nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->flags() ^
                 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    }
    delete query;
    delete query2;
  }
  delete attributeColorDialog;
  setVisibility();
}

void NetworkGraphWidget::addModes() 
{
  QPointer<AttributeCheckBoxDialog> attributeDialog = new AttributeCheckBoxDialog(this, ENTITY);
  attributeDialog->exec();
  if (attributeDialog->getExitStatus() == 0) 
  {
    setChangeLabel();
    // Let's first create a few queries
    QSqlQuery *query = new QSqlQuery;
    QSqlQuery *query2= new QSqlQuery;
    query->prepare("SELECT description FROM entity_attributes "
                   "WHERE name = :name");
    query2->prepare("SELECT entity FROM attributes_to_entities "
                    "WHERE attribute = :currentAttribute");
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
      QString attribute = currentPair.first;
      // We do not need the second variable in this pair in this case
      query->bindValue(":name", attribute);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QVector<QString> attributeVector;
      attributeVector.push_back(attribute);
      findChildren(attribute, &attributeVector);
      QVectorIterator<QString> it2(attributeVector);
      while (it2.hasNext())
      {
        QString currentAttribute = it2.next();
        query2->bindValue(":currentAttribute", currentAttribute);
        query2->exec();
        while (query2->next())
        {
          QString currentName = query2->value(0).toString();
          QVectorIterator<NetworkNode*> it3(_networkNodeVector);
          while (it3.hasNext())
          {
            NetworkNode* currentNode = it3.next();
            if (currentNode->getName() == currentName)
            {
              currentNode->setColor(currentColor);
              currentNode->setMode(attribute);
              currentNode->getLabel()->setDefaultTextColor(Qt::black);
              currentNode->setMassHidden(false);
            }
          }
        }
      }
      bool found = false;
      for (int i = 0; i < nodeListWidget->rowCount(); i++)
      {
        if (nodeListWidget->item(i, 0)->data(Qt::DisplayRole) == attribute)
        {
          found = true;
          QTableWidgetItem *item = nodeListWidget->item(i,0);
          QString toolTip = breakString(attribute + " - " + description);
          item->setToolTip(toolTip);
          nodeListWidget->item(i, 1)->setBackground(currentColor);
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
        nodeListWidget->setRowCount(nodeListWidget->rowCount() + 1);
        nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 0, item);
        nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
        nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setBackground(currentColor);
        QVariant textColorVar = QVariant(QColor(Qt::black).rgb());
        nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)
          ->setData(Qt::UserRole, textColorVar);
        nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->
          setFlags(nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->flags() ^
                   Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
      }
    }
    delete query;
    delete query2;
  }
  delete attributeDialog;
  setVisibility();
}

void NetworkGraphWidget::findChildren(QString father, QVector<QString> *children) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM entity_attributes WHERE father = :father");
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) 
  {
    QString currentChild = query->value(0).toString();
    children->push_back(currentChild);
    findChildren(currentChild, children);
  }
  delete query;
}

void NetworkGraphWidget::multimodeTransformation() 
{
  QVector<QString> modeVector;
  for (int i = 0; i != nodeListWidget->rowCount(); i++) 
  {
    QString currentMode = nodeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    bool found = false;
    QVectorIterator<NetworkNode*> mit(_networkNodeVector);
    while (mit.hasNext())
    {
      NetworkNode* currentNode = mit.next();
      if (currentNode->isVisible() && currentNode->getMode() == currentMode)
      {
        found = true;
      }
    }
    if (found)
    {
      modeVector.push_back(currentMode);
    }
  }
  QPointer<MultimodeDialog> multimodeDialog = new MultimodeDialog(this, &_directedVector,
                                                                  &_undirectedVector);
  multimodeDialog->setModes(modeVector);
  multimodeDialog->exec();
  if (multimodeDialog->getExitStatus() == 0) 
  {
    QString modeOne = multimodeDialog->getModeOne();
    QString modeTwo = multimodeDialog->getModeTwo();
    QString relationshipOne = multimodeDialog->getRelationshipOne();
    QString relationshipTwo = multimodeDialog->getRelationshipTwo();
    QString name = multimodeDialog->getName();
    QString directedness = multimodeDialog->getDirectedness();
    for (int i = 0; i != edgeListWidget->rowCount(); i++)
    {
      QString currentRel = edgeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
      if (currentRel ==  name)
      {
        QPointer <QMessageBox> warningBox = new QMessageBox(this);
        warningBox->setWindowTitle("Making transformation");
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
    if (colorDialog->exec())
    {
      color = colorDialog->selectedColor();
    }
    else
    {
      delete colorDialog;
      return;
    }
    delete colorDialog;
    QString description = multimodeDialog->getDescription();
    QVectorIterator<DirectedEdge*> it(_directedVector);
    while (it.hasNext())
    {
      DirectedEdge *first = it.next();
      if (first->getType() == relationshipOne)
      {
        if (first->getStart()->getMode() == modeOne &&
            first->getEnd()->getMode() == modeTwo)
        {
          QVectorIterator<DirectedEdge*> it2(_directedVector);
          while (it2.hasNext())
          {
            DirectedEdge *second = it2.next();
            if (second->getType() == relationshipTwo)
            {
              if (second->getEnd() == first->getEnd() &&
                  second->getStart() != first->getStart() &&
                  second->getStart()->getMode() == modeOne &&
                  second->getEnd()->getMode() == modeTwo &&
                  first->getStart()->isVisible() &&
                  first->getEnd()->isVisible() &&
                  second->getStart()->isVisible() &&
                  second->getEnd()->isVisible())
              {
                if (directedness == DIRECTED)
                {
                  DirectedEdge *newEdge = new DirectedEdge(first->getStart(),
                                                           second->getStart(),
                                                           name, TRANSFORMED, 0);
                  bool found = false;
                  QVectorIterator<DirectedEdge*> it3(_directedVector);
                  while (it3.hasNext())
                  {
                    DirectedEdge* alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if (alter->getStart() == newEdge->getStart() &&
                          alter->getEnd() == newEdge->getEnd())
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _directedVector.push_back(newEdge);
                  }
                }
                else if (directedness == UNDIRECTED)
                {
                  UndirectedEdge *newEdge;
                  if (first->getStart()->getName() < second->getStart()->getName())
                  {
                    newEdge = new UndirectedEdge(first->getStart(),
                                                 second->getStart(),
                                                 name, TRANSFORMED, 0);
                  }
                  else
                  {
                    newEdge = new UndirectedEdge(second->getStart(),
                                                 first->getStart(),
                                                 name, TRANSFORMED, 0);
                  }
                  bool found = false;
                  QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
                  while (it3.hasNext())
                  {
                    UndirectedEdge* alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if ((alter->getStart() == newEdge->getStart() &&
                           alter->getEnd() == newEdge->getEnd()) ||
                          (alter->getStart() == newEdge->getEnd() &&
                           alter->getEnd() == newEdge->getStart()))
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _undirectedVector.push_back(newEdge);
                  }
                  else
                  {
                    delete newEdge;
                  }
                }
              }
            }
          }
        }
        else if (first->getStart()->getMode() == modeTwo &&
                 first->getEnd()->getMode() == modeOne)
        {
          QVectorIterator<DirectedEdge*> it2(_directedVector);
          while (it2.hasNext())
          {
            DirectedEdge *second = it2.next();
            if (second->getType() == relationshipTwo)
            {
              if (second->getStart() == first->getStart() &&
                  second->getEnd() != first->getEnd() &&
                  second->getEnd()->getMode() == modeOne &&
                  second->getStart()->getMode() == modeTwo &&
                  first->getStart()->isVisible() &&
                  first->getEnd()->isVisible() &&
                  second->getStart()->isVisible() &&
                  second->getEnd()->isVisible())
              {
                if (directedness == DIRECTED)
                {
                  DirectedEdge *newEdge = new DirectedEdge(first->getEnd(),
                                                           second->getEnd(),
                                                           name, TRANSFORMED, 0);
                  bool found = false;
                  QVectorIterator<DirectedEdge*> it3(_directedVector);
                  while (it3.hasNext())
                  {
                    DirectedEdge* alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if (alter->getStart() == newEdge->getStart() &&
                          alter->getEnd() == newEdge->getEnd())
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _directedVector.push_back(newEdge);
                  }
                  else
                  {
                    delete newEdge;
                  }
                }
                else if (directedness == UNDIRECTED)
                {
                  UndirectedEdge *newEdge;
                  if (first->getEnd()->getName() < second->getEnd()->getName())
                  {
                    newEdge = new UndirectedEdge(first->getEnd(),
                                                 second->getEnd(),
                                                 name, TRANSFORMED, 0);
                  }
                  else
                  {
                    newEdge = new UndirectedEdge(second->getEnd(),
                                                 first->getEnd(),
                                                 name, TRANSFORMED, 0);
                  }
                  bool found = false;
                  QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
                  while (it3.hasNext())
                  {
                    UndirectedEdge* alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if ((alter->getStart() == newEdge->getStart() &&
                           alter->getEnd() == newEdge->getEnd()) ||
                          (alter->getStart() == newEdge->getEnd() &&
                           alter->getEnd() == newEdge->getStart()))
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _undirectedVector.push_back(newEdge);
                  }
                  else
                  {
                    delete newEdge;
                  }
                }
              }
            }
          }
        }
      }
    }
    QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
    while (it3.hasNext())
    {
      UndirectedEdge *first = it3.next();
      if (first->getType() == relationshipOne)
      {
        if (first->getStart()->getMode() == modeOne &&
            first->getEnd()->getMode() == modeTwo)
        {
          QVectorIterator<UndirectedEdge*> it4(_undirectedVector);
          while (it4.hasNext())
          {
            UndirectedEdge *second = it4.next();
            if (second->getType() == relationshipTwo)
            {
              if (second->getEnd() == first->getEnd() &&
                  second->getStart() != first->getStart() &&
                  second->getStart()->getMode() == modeOne &&
                  second->getEnd()->getMode() == modeTwo &&
                  first->getStart()->isVisible() &&
                  first->getEnd()->isVisible() &&
                  second->getStart()->isVisible() &&
                  second->getEnd()->isVisible())
              {
                if (directedness == DIRECTED)
                {
                  DirectedEdge *newEdge = new DirectedEdge(first->getStart(),
                                                           second->getStart(),
                                                           name, TRANSFORMED, 0);
                  bool found = false;
                  QVectorIterator<DirectedEdge*> it3(_directedVector);
                  while (it3.hasNext())
                  {
                    DirectedEdge* alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if (alter->getStart() == newEdge->getStart() &&
                          alter->getEnd() == newEdge->getEnd())
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _directedVector.push_back(newEdge);
                  }
                  else
                  {
                    delete newEdge;
                  }
                }
                else if (directedness == UNDIRECTED)
                {
                  UndirectedEdge *newEdge;
                  if (first->getStart()->getName() < second->getStart()->getName())
                  {
                    newEdge = new UndirectedEdge(first->getStart(),
                                                 second->getStart(),
                                                 name, TRANSFORMED, 0);
                  }
                  else
                  {
                    newEdge = new UndirectedEdge(second->getStart(),
                                                 first->getStart(),
                                                 name, TRANSFORMED, 0);
                  }
                  QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
                  bool found = false;
                  while (it3.hasNext())
                  {
                    UndirectedEdge *alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if ((alter->getStart() == newEdge->getStart() &&
                           alter->getEnd() == newEdge->getEnd()) ||
                          (alter->getStart() == newEdge->getEnd() &&
                           alter->getEnd() == newEdge->getStart()))
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _undirectedVector.push_back(newEdge);
                  }
                  else
                  {
                    delete newEdge;
                  }
                }
              }
            }
          }
        }
        else if (first->getStart()->getMode() == modeTwo &&
                 first->getEnd()->getMode() == modeOne)
        {
          QVectorIterator<UndirectedEdge*> it4(_undirectedVector);
          while (it4.hasNext())
          {
            UndirectedEdge *second = it4.next();
            if (second->getType() == relationshipTwo)
            {
              if (second->getEnd() == first->getEnd() &&
                  second->getStart() != first->getStart() &&
                  second->getEnd()->getMode() == modeOne &&
                  second->getStart()->getMode() == modeTwo &&
                  first->getStart()->isVisible() &&
                  first->getEnd()->isVisible() &&
                  second->getStart()->isVisible() &&
                  second->getEnd()->isVisible())
              {
                if (directedness == DIRECTED)
                {
                  DirectedEdge *newEdge = new DirectedEdge(first->getEnd(),
                                                           second->getEnd(),
                                                           name, TRANSFORMED, 0);
                  bool found = false;
                  QVectorIterator<DirectedEdge*> it3(_directedVector);
                  while (it3.hasNext())
                  {
                    DirectedEdge* alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if (alter->getStart() == newEdge->getStart() &&
                          alter->getEnd() == newEdge->getEnd())
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _directedVector.push_back(newEdge);
                  }
                  else
                  {
                    delete newEdge;
                  }
                }
                else if (directedness == UNDIRECTED)
                {
                  UndirectedEdge *newEdge;
                  if (first->getEnd()->getName() < second->getEnd()->getName())
                  {
                    newEdge = new UndirectedEdge(first->getEnd(),
                                                 second->getEnd(),
                                                 name, TRANSFORMED, 0);
                  }
                  else
                  {
                    newEdge = new UndirectedEdge(second->getEnd(),
                                                 first->getEnd(),
                                                 name, TRANSFORMED, 0);
                  }
                  bool found = false;
                  QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
                  while (it3.hasNext())
                  {
                    UndirectedEdge* alter = it3.next();
                    if (alter->getType() == name)
                    {
                      if ((alter->getStart() == newEdge->getStart() &&
                           alter->getEnd() == newEdge->getEnd()) ||
                          (alter->getStart() == newEdge->getEnd() &&
                           alter->getEnd() == newEdge->getStart()))
                      {
                        found = true;
                      }
                    }
                  }
                  if (!found)
                  {
                    newEdge->setZValue(_networkNodeVector[0]->zValue() - 1);
                    _undirectedVector.push_back(newEdge);
                  }
                  else
                  {
                    delete newEdge;
                  }
                }
              }
            }
          }
        }
      }
    }
    _presentTypes.push_back(name);
    plotDirectedEdges(name, color);
    plotUndirectedEdges(name, color);
    QTableWidgetItem *item = new QTableWidgetItem(_selectedType);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    QString toolTip = breakString(name + " (Undirected) - " + description + " (MMT)");
    item->setToolTip(toolTip);
    item->setData(Qt::DisplayRole, name);
    edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
      setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags()
               ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
    lowerRangeDial->setEnabled(false);
    upperRangeDial->setEnabled(false);
    lowerRangeSpinBox->setEnabled(false);
    upperRangeSpinBox->setEnabled(false);
    setVisibility();
  }
}

void NetworkGraphWidget::removeMode() 
{
  setChangeLabel();
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    if (current->getMode() == text)
    {
      current->setColor(Qt::black);
      current->setMode("");
      current->setMassHidden(false);
    }
  }
  for (int i = 0; i != nodeListWidget->rowCount();) 
  {
    if (nodeListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text)
    {
      nodeListWidget->removeRow(i);
    }
    if (i != nodeListWidget->rowCount())
    {
      i++;
    }
  }
  for (int i = 0; i != nodeListWidget->rowCount(); i++) 
  {
    QString currentMode = nodeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
    QColor color = nodeListWidget->item(i, 1)->background().color();
    bool hidden = false;
    if (nodeListWidget->item(i, 0)->background().color() == Qt::gray)
    {
      hidden = true;
    }
    QVariant textColorVar = nodeListWidget->item(i, 1)->data(Qt::UserRole);
    QColor textColor = QColor::fromRgb(textColorVar.toUInt());
    QVector<QString> attributeVector;
    attributeVector.push_back(currentMode);
    findChildren(currentMode, &attributeVector);
    QVectorIterator<QString> it3(attributeVector);
    while (it3.hasNext())
    {
      QString currentAttribute = it3.next();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT entity FROM attributes_to_entities "
                     "WHERE attribute = :currentAttribute");
      query->bindValue(":currentAttribute", currentAttribute);
      query->exec();
      while (query->next())
      {
        QString entity = query->value(0).toString();
        QVectorIterator<NetworkNode*> it4(_networkNodeVector);
        while (it4.hasNext())
        {
          NetworkNode* currentEntity = it4.next();
          if (currentEntity->getName() == entity)
          {
            currentEntity->setColor(color);
            currentEntity->getLabel()->setDefaultTextColor(textColor);
            currentEntity->setMode(currentMode);
            if (hidden)
            {
              currentEntity->setMassHidden(true);
            }
            else
            {
              currentEntity->setMassHidden(false);
            }
          }
        }
      }
      delete query;
    }
  }
  for (int i = 0; i < nodeListWidget->rowCount(); i++) 
  {
    QString mode = nodeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    QColor color = nodeListWidget->item(i, 1)->background().color();
    QVariant textColorVar = nodeListWidget->item(i, 1)->data(Qt::UserRole);
    QColor textColor = QColor::fromRgb(textColorVar.toUInt());
    QVectorIterator<NetworkNode*> it(_networkNodeVector);
    while (it.hasNext())
    {
      NetworkNode *node = it.next();
      if (node->getMode() == mode)
      {
        node->setColor(color);
        node->getLabel()->setDefaultTextColor(textColor);
      }
    }
  }
  setVisibility();
}

void NetworkGraphWidget::setModeButtons(QTableWidgetItem *item) 
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
  if (text != nodeListWidget->item(0, 0)->data(Qt::DisplayRole).toString()) 
  {
    moveModeUpButton->setEnabled(true);
  }
  else 
  {
    moveModeUpButton->setEnabled(false);
  }
  if (text != nodeListWidget->item(nodeListWidget->rowCount() - 1, 0)
      ->data(Qt::DisplayRole).toString()) 
  {
    moveModeDownButton->setEnabled(true);
  }
  else 
  {
    moveModeDownButton->setEnabled(false);
  }
  bool hidden = false;
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    QString mode = current->getMode();
    if (text == mode)
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

void NetworkGraphWidget::disableLegendButtons() 
{
  removeModeButton->setEnabled(false);
  moveModeUpButton->setEnabled(false);
  moveModeDownButton->setEnabled(false);
  hideModeButton->setEnabled(false);
  showModeButton->setEnabled(false);
  setFilteredButton->setEnabled(false);
  unsetFilteredButton->setEnabled(false);
  hideTypeButton->setEnabled(false);
  showTypeButton->setEnabled(false);
  removeTypeButton->setEnabled(false);
  mergeButton->setEnabled(false);
}

void NetworkGraphWidget::restoreModeColors() 
{
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext())
  {
    NetworkNode *currentNode = it.next();
    currentNode->setColor(Qt::black);
    currentNode->getLabel()->setDefaultTextColor(Qt::black);
    currentNode->setMode("");
  }
  for (int i = 0; i != nodeListWidget->rowCount(); i++) 
  {
    QString currentMode = nodeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
    QColor color = nodeListWidget->item(i, 1)->background().color();
    bool hidden = false;
    if (nodeListWidget->item(i, 0)->background().color() == Qt::gray)
    {
      hidden = true;
    }
    QVariant textColorVar = nodeListWidget->item(i, 1)->data(Qt::UserRole);
    QColor textColor = QColor::fromRgb(textColorVar.toUInt());
    QVector<QString> attributeVector;
    attributeVector.push_back(currentMode);
    findChildren(currentMode, &attributeVector);
    QVectorIterator<QString> it3(attributeVector);
    while (it3.hasNext())
    {
      QString currentAttribute = it3.next();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT entity FROM attributes_to_entities "
                     "WHERE attribute = :currentAttribute");
      query->bindValue(":currentAttribute", currentAttribute);
      query->exec();
      while (query->next())
      {
        QString entity = query->value(0).toString();
        QVectorIterator<NetworkNode*> it4(_networkNodeVector);
        while (it4.hasNext())
        {
          NetworkNode* currentEntity = it4.next();
          if (currentEntity->getName() == entity)
          {
            currentEntity->setColor(color);
            currentEntity->getLabel()->setDefaultTextColor(textColor);
            currentEntity->setMode(currentMode);
            if (hidden)
            {
              currentEntity->setMassHidden(true);
            }
            else
            {
              currentEntity->setMassHidden(false);
            }
          }
        }
      }
      delete query;
    }
  }
  for (int i = 0; i < nodeListWidget->rowCount(); i++) 
  {
    QString mode = nodeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    QColor color = nodeListWidget->item(i, 1)->background().color();
    QVariant textColorVar = nodeListWidget->item(i, 1)->data(Qt::UserRole);
    QColor textColor = QColor::fromRgb(textColorVar.toUInt());
    QVectorIterator<NetworkNode*> it(_networkNodeVector);
    while (it.hasNext())
    {
      NetworkNode *node = it.next();
      if (node->getMode() == mode)
      {
        node->setColor(color);
        node->getLabel()->setDefaultTextColor(textColor);
      }
    }
  }
  setChangeLabel();
  setVisibility();
}

void NetworkGraphWidget::moveModeUp() 
{
  setChangeLabel();
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != nodeListWidget->item(0,0)->data(Qt::DisplayRole).toString()) 
  {
    int currentRow = nodeListWidget->row(nodeListWidget->currentItem());
    QTableWidgetItem *currentItem = nodeListWidget->takeItem(currentRow,0);
    QColor currentColor = nodeListWidget->item(currentRow, 1)->background().color();
    QVariant currentTextColorVar = nodeListWidget->item(currentRow, 1)->data(Qt::UserRole);
    int newRow = currentRow - 1;
    QTableWidgetItem *otherItem = nodeListWidget->takeItem(newRow, 0);
    QColor otherColor = nodeListWidget->item(newRow, 1)->background().color();
    QVariant otherTextColorVar = nodeListWidget->item(newRow, 1)->data(Qt::UserRole);
    nodeListWidget->setItem(newRow, 0, currentItem);
    nodeListWidget->item(newRow, 1)->setBackground(currentColor);
    nodeListWidget->item(newRow, 1)->setData(Qt::UserRole, currentTextColorVar);
    nodeListWidget->setItem(currentRow, 0, otherItem);
    nodeListWidget->item(currentRow, 1)->setBackground(otherColor);
    nodeListWidget->item(currentRow, 1)->setData(Qt::UserRole, otherTextColorVar);
    restoreModeColors();
    QModelIndex newIndex = nodeListWidget->model()->index(newRow, 0);
    nodeListWidget->setCurrentIndex(newIndex);
    setModeButtons(nodeListWidget->currentItem());
  }
}

void NetworkGraphWidget::moveModeDown() 
{
  setChangeLabel();
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  if (text != nodeListWidget->item(nodeListWidget->rowCount() - 1, 0)->
      data(Qt::DisplayRole).toString()) 
  {
    int currentRow = nodeListWidget->row(nodeListWidget->currentItem());
    QTableWidgetItem *currentItem = nodeListWidget->takeItem(currentRow, 0);
    QColor currentColor = nodeListWidget->item(currentRow, 1)->background().color();
    QVariant currentTextColorVar = nodeListWidget->item(currentRow, 1)->data(Qt::UserRole);
    int newRow = currentRow + 1;
    QTableWidgetItem *otherItem = nodeListWidget->takeItem(newRow, 0);
    QColor otherColor = nodeListWidget->item(newRow, 1)->background().color();
    QVariant otherTextColorVar = nodeListWidget->item(newRow, 1)->data(Qt::UserRole);
    nodeListWidget->setItem(newRow, 0, currentItem);
    nodeListWidget->item(newRow, 1)->setBackground(currentColor);
    nodeListWidget->item(newRow, 1)->setData(Qt::UserRole, currentTextColorVar);
    nodeListWidget->setItem(currentRow, 0, otherItem);
    nodeListWidget->item(currentRow, 1)->setBackground(otherColor);
    nodeListWidget->item(currentRow, 1)->setData(Qt::UserRole, otherTextColorVar);
    restoreModeColors();
    QModelIndex newIndex = nodeListWidget->model()->index(newRow, 0);
    nodeListWidget->setCurrentIndex(newIndex);
    setModeButtons(nodeListWidget->currentItem());
  }
}


void NetworkGraphWidget::mergeRelationships() 
{
  QVector<QString> relVector;
  QVector<QString> directednessVector;
  for (int i = 0; i != edgeListWidget->rowCount(); i++) 
  {
    QString currentType = edgeListWidget->item(i, 0)->data(Qt::DisplayRole).toString();
    bool found = false;
    QVectorIterator<DirectedEdge*> rit(_directedVector);
    QString directedness = "";
    while (rit.hasNext())
    {
      DirectedEdge* directed = rit.next();
      if (directed->isVisible() && directed->getType() == currentType)
      {
        found = true;
        directedness = DIRECTED;
      }
    }
    QVectorIterator<UndirectedEdge*> rit2(_undirectedVector);
    while (rit2.hasNext())
    {
      UndirectedEdge* undirected = rit2.next();
      if (undirected->isVisible() && undirected->getType() == currentType)
      {
        found = true;
        directedness = UNDIRECTED;
      }
    }
    if (found)
    {
      relVector.push_back(currentType);
      directednessVector.push_back(directedness);
    }
  }
  QPointer<MergeRelationshipsDialog> mergeRelationshipsDialog =
    new MergeRelationshipsDialog(this, &_directedVector, &_undirectedVector,
                                 relVector, directednessVector);
  mergeRelationshipsDialog->exec();
  if (mergeRelationshipsDialog->getExitStatus() == 0) 
  {
    QVector<QString> types = mergeRelationshipsDialog->getTypes();
    QString name = mergeRelationshipsDialog->getName();
    QString description = mergeRelationshipsDialog->getDescription();
    QString directedness = mergeRelationshipsDialog->getDirectedness();
    QPointer<QColorDialog> colorDialog = new QColorDialog(this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
    QColor color = QColor(Qt::black);
    if (colorDialog->exec())
    {
      color = colorDialog->selectedColor();
    }
    else
    {
      delete colorDialog;
      return;
    }
    delete colorDialog;
    if (directedness == DIRECTED)
    {
      QVectorIterator<DirectedEdge*> it(_directedVector);
      while (it.hasNext())
      {
        DirectedEdge* directed = it.next();
        if (types.contains(directed->getType()))
        {
          DirectedEdge *newDirected = new DirectedEdge(directed->getStart(),
                                                       directed->getEnd(),
                                                       name, MERGED, 0);
          newDirected->setZValue(_networkNodeVector[0]->zValue() - 1);
          newDirected->setColor(color);
          newDirected->setIncidents(directed->getIncidents());
          _directedVector.push_back(newDirected);
          delete directed;
          _directedVector.removeOne(directed);
          scene->addItem(newDirected);
        }
      }
      QVectorIterator<DirectedEdge*> it2(_directedVector);
      while (it2.hasNext())
      {
        DirectedEdge *first = it2.next();
        bool found = false;
        QVectorIterator<DirectedEdge*> it3(_directedVector);
        while (it3.hasNext())
        {
          DirectedEdge *second = it3.next();
          if (first != second)
          {
            if (first->getStart() == second->getStart() &&
                first->getEnd() == second->getEnd() &&
                first->getName() == MERGED &&
                second->getName() == MERGED)
            {
              found = true;
              second->insertIncidents(first->getIncidents());
            }
          }
        }
        if (found)
        {
          delete first;
          _directedVector.removeOne(first);
        }
      }
    }
    else if (directedness == UNDIRECTED)
    {
      QVectorIterator<UndirectedEdge*> it(_undirectedVector);
      while (it.hasNext())
      {
        UndirectedEdge *undirected = it.next();
        if (types.contains(undirected->getType()))
        {
          UndirectedEdge *newUndirected = new UndirectedEdge(undirected->getStart(),
                                                             undirected->getEnd(),
                                                             name, MERGED, 0);
          newUndirected->setColor(color);
          newUndirected->setZValue(_networkNodeVector[0]->zValue() - 1);
          newUndirected->setIncidents(undirected->getIncidents());
          _undirectedVector.push_back(newUndirected);
          _undirectedVector.removeOne(undirected);
          delete undirected;
          scene->addItem(newUndirected);
        }
      }
      QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
      while (it2.hasNext())
      {
        UndirectedEdge *first = it2.next();
        bool found = false;
        QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
        while (it3.hasNext())
        {
          UndirectedEdge *second = it3.next();
          if (first != second)
          {
            if (first->getStart() == second->getStart() &&
                first->getEnd() == second->getEnd() &&
                first->getName() == MERGED &&
                second->getName() == MERGED)
            {
              found = true;
              second->insertIncidents(first->getIncidents());
            }
          }
        }
        if (found)
        {
          _undirectedVector.removeOne(first);
          delete first;
        }
      }
    }
    QVectorIterator<QString> rit(types);
    while (rit.hasNext())
    {
      _presentTypes.removeOne(rit.next());
    }
    _presentTypes.push_back(name);
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
    for (int i = 0; i != edgeListWidget->rowCount();)
    {
      while (types.contains(edgeListWidget->item(i,0)->data(Qt::DisplayRole).toString()))
      {
        edgeListWidget->removeRow(i);
      }
      if (i != edgeListWidget->rowCount())
      {
        i++;
      }
    }
    lowerRangeDial->setEnabled(false);
    upperRangeDial->setEnabled(false);
    lowerRangeSpinBox->setEnabled(false);
    upperRangeSpinBox->setEnabled(false);
    setVisibility();
  }
}

void NetworkGraphWidget::setFilterButtons(QTableWidgetItem *item) 
{
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") 
  {
    setFilteredButton->setEnabled(true);
    unsetFilteredButton->setEnabled(true);
    hideTypeButton->setEnabled(true);
    showTypeButton->setEnabled(true);
    mergeButton->setEnabled(true);
    removeTypeButton->setEnabled(true);
  }
  else 
  {
    setFilteredButton->setEnabled(false);
    unsetFilteredButton->setEnabled(false);
    hideTypeButton->setEnabled(false);
    showTypeButton->setEnabled(false);
    mergeButton->setEnabled(false);
    removeTypeButton->setEnabled(false);
    return;
  }
  QBrush brush = item->foreground();
  QVectorIterator<DirectedEdge*> it(_directedVector);
  bool filtered = false;
  bool hidden = false;
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      filtered = current->isFiltered();
      hidden = current->isMassHidden();
      break;
    }
  }
  if (filtered) 
  {
    setFilteredButton->setChecked(true);
    unsetFilteredButton->setChecked(false);
  }
  else 
  {
    setFilteredButton->setChecked(false);
    unsetFilteredButton->setChecked(true);
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
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext()) 
  {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type)
    {
      filtered = current->isFiltered();
      hidden = current->isMassHidden();
      break;
    }
  }
  if (filtered) 
  {
    setFilteredButton->setChecked(true);
    unsetFilteredButton->setChecked(false);
  }
  else 
  {
    setFilteredButton->setChecked(false);
    unsetFilteredButton->setChecked(true);
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

void NetworkGraphWidget::disableFilterButtons() 
{
  setFilteredButton->setEnabled(false);
  unsetFilteredButton->setEnabled(false);
  hideTypeButton->setEnabled(false);
  showTypeButton->setEnabled(false);
  removeTypeButton->setEnabled(false);
}

void NetworkGraphWidget::activateFilter() 
{
  setFilteredButton->setChecked(true);
  unsetFilteredButton->setChecked(false);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setFiltered(true);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext()) 
  {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setFiltered(true);
    }
  }
  updateWeightControls();
  setVisibility();
}

void NetworkGraphWidget::deactivateFilter() 
{
  setFilteredButton->setChecked(false);
  unsetFilteredButton->setChecked(true);  
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setFiltered(false);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext()) 
  {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setFiltered(false);
    }
  }
  updateWeightControls();
  setVisibility();
}

void NetworkGraphWidget::hideType() 
{
  hideTypeButton->setChecked(true);
  showTypeButton->setChecked(false);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  edgeListWidget->currentItem()->setBackground(Qt::gray);
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setMassHidden(true);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext()) 
  {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setMassHidden(true);
    }
  }
  updateWeightControls();
  setVisibility();
}

void NetworkGraphWidget::showType() 
{
  hideTypeButton->setChecked(false);
  showTypeButton->setChecked(true);
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  edgeListWidget->currentItem()->setBackground(Qt::transparent);
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setMassHidden(false);
    }
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext()) 
  {
    UndirectedEdge *current = it2.next();
    QString type = current->getType();
    if (text == type)
    {
      current->setMassHidden(false);
    }
  }
  updateWeightControls();
  setVisibility();
}

void NetworkGraphWidget::hideMode() 
{
  hideModeButton->setChecked(true);
  showModeButton->setChecked(false);
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  nodeListWidget->currentItem()->setBackground(Qt::gray);
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    QString mode = current->getMode();
    if (text == mode)
    {
      current->setMassHidden(true);
    }
  }
  updateEdges();
  setVisibility();
}

void NetworkGraphWidget::showMode() 
{
  hideModeButton->setChecked(false);
  showModeButton->setChecked(true);
  QString text = nodeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  nodeListWidget->currentItem()->setBackground(Qt::transparent);
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *current = it.next();
    QString type = current->getMode();
    if (text == type)
    {
      current->setMassHidden(false);
    }
  }
  updateEdges();
  setVisibility();
}

void NetworkGraphWidget::setNodeColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
  {
    setChangeLabel();
    QColor color = colorDialog->selectedColor();
    QVectorIterator<NetworkNode*> it(_networkNodeVector);
    while (it.hasNext())
    {
      NetworkNode *currentNode = it.next();
      currentNode->setColor(color);
    }
  }
  delete colorDialog;
}

void NetworkGraphWidget::setLabelColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
  {
    setChangeLabel();
    QColor color = colorDialog->selectedColor();
    QVectorIterator<NetworkNodeLabel*> it(_networkNodeLabelVector);
    while (it.hasNext())
    {
      NetworkNodeLabel *currentLabel = it.next();
      currentLabel->setDefaultTextColor(color);
    }
  }
  delete colorDialog;
}

void NetworkGraphWidget::setBackgroundColor() 
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

void NetworkGraphWidget::changeEdgeColor(QTableWidgetItem *item) 
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
      QVectorIterator<DirectedEdge*> it(_directedVector);
      while (it.hasNext())
      {
        DirectedEdge *directed = it.next();
        if (directed->getType() == type)
        {
          directed->setColor(color);
        }
      }
      QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
      while (it2.hasNext())
      {
        UndirectedEdge *undirected = it2.next();
        if (undirected->getType() == type)
        {
          undirected->setColor(color);
        }
      }
    }
  }
}

void NetworkGraphWidget::changeModeColor(QTableWidgetItem *item) 
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
      QVectorIterator<NetworkNode*> it(_networkNodeVector);
      while (it.hasNext())
      {
        NetworkNode *current = it.next();
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

void NetworkGraphWidget::setPlotButton() 
{
  if (typeComboBox->currentText() != DEFAULT) 
  {
    plotButton->setEnabled(true);
    if (_presentTypes.size() > 0)
    {
      QVectorIterator<QString> it(_presentTypes);
      bool found = false;
      while (it.hasNext())
      {
        QString currentType = it.next();
        if (currentType == typeComboBox->currentText())
        {
          found = true;
        }
      }
      if (!found)
      {
        addButton->setEnabled(true);
      }
      else
      {
        addButton->setEnabled(false);
      }
    }
  }
  else 
  {
    plotButton->setEnabled(false);
  }
}

void NetworkGraphWidget::hideAnnotations()
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

void NetworkGraphWidget::plotNewGraph() 
{
  cleanUp();
  updateCases();
  _selectedType = typeComboBox->currentText();
  getEntities();
  plotEntities(); // Should allow for range to be set here.
  getDirectedEdges();
  getUndirectedEdges();
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
  delete colorDialog;
  plotDirectedEdges(_selectedType, color);
  plotUndirectedEdges(_selectedType, color);
  _presentTypes.push_back(_selectedType);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness, description FROM relationship_types "
                 "WHERE name = :name");
  query->bindValue(":name", _selectedType);
  query->exec();
  query->first();
  QString directedness = query->value(0).toString();
  QString description = query->value(1).toString();
  QString toolTip = breakString(_selectedType + " (" + directedness + ") - " + description);
  QTableWidgetItem *item = new QTableWidgetItem(_selectedType);
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setToolTip(toolTip);
  item->setData(Qt::DisplayRole, _selectedType);
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
  setWeightControls();
  plotLabel->setText("Unsaved plot");
  updateEdges();
  checkCongruence();
  caseListWidget->setEnabled(true);
  setVisibility();
  makeLayout();
  view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
  setGraphControls(true);
  hideAnnotationsButton->setChecked(false);
}

void NetworkGraphWidget::addRelationshipType() 
{
  _selectedType = typeComboBox->currentText();  
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  QColor color = QColor(Qt::black);
  if (colorDialog->exec()) 
  {
    color = colorDialog->selectedColor();
  }
  else 
  {
    delete colorDialog;
    return;
  }
  delete colorDialog;
  plotDirectedEdges(_selectedType, color);
  plotUndirectedEdges(_selectedType, color);
  _presentTypes.push_back(_selectedType);
  updateRangeControls();
  updateWeightControls();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness, description FROM relationship_types "
                 "WHERE name = :name");
  query->bindValue(":name", _selectedType);
  query->exec();
  query->first();
  QString directedness = query->value(0).toString();
  QString description = query->value(1).toString();
  QString toolTip = breakString(_selectedType + " (" + directedness + ") - " + description);
  QTableWidgetItem *item = new QTableWidgetItem(_selectedType);
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  item->setToolTip(toolTip);
  item->setData(Qt::DisplayRole, _selectedType);
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
  setVisibility();
  makeLayout();  
  updateEdges();
}

void NetworkGraphWidget::removeRelationshipType() 
{
  setChangeLabel();
  QString text = edgeListWidget->currentItem()->data(Qt::DisplayRole).toString();
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext()) 
  {
    DirectedEdge *directed = it.next();
    if (directed->getType() == text)
    {
      _directedVector.removeOne(directed);
      delete directed;
    }
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext()) 
  {
    UndirectedEdge *undirected = it2.next();
    if (undirected->getType() == text)
    {
      _undirectedVector.removeOne(undirected);
      delete undirected;
    }
  }
  for (int i = 0; i != edgeListWidget->rowCount();) 
  {
    if (edgeListWidget->item(i,0)->data(Qt::DisplayRole).toString() == text)
    {
      edgeListWidget->removeRow(i);
    }
    if (i != edgeListWidget->rowCount())
    {
      i++;
    }
  }
  _presentTypes.removeOne(text);
  if (_presentTypes.size() == 0) 
  {
    caseListWidget->setEnabled(false);
    setGraphControls(false);
    cleanUp();
  }
  else
  {
    bool foundTransformed = false;
    it.toFront();
    it2.toFront();
    while (it.hasNext())
    {
      DirectedEdge *directed = it.next();
      if (directed->getType() == TRANSFORMED)
      {
        foundTransformed = true;
        break;
      }
    }
    if (!foundTransformed)
    {
      while (it2.hasNext())
      {
        UndirectedEdge *undirected = it2.next();
        if (undirected->getType() == TRANSFORMED)
        {
          foundTransformed = true;
          break;
        }
      }
    }
    if (!foundTransformed)
    {
      lowerRangeDial->setEnabled(true);
      lowerRangeSpinBox->setEnabled(true);
      upperRangeDial->setEnabled(true);
      upperRangeSpinBox->setEnabled(true);
    }
  }
  updateRangeControls();
  updateWeightControls();
  setVisibility();
}

void NetworkGraphWidget::toggleLabels() 
{
  _labelsShown = !_labelsShown;
  QVectorIterator<NetworkNodeLabel*> it2(_networkNodeLabelVector);
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

void NetworkGraphWidget::increaseLabelSize() 
{
  QVectorIterator<NetworkNodeLabel*> it(_networkNodeLabelVector);
  while (it.hasNext()) 
  {
    NetworkNodeLabel *currentLabel = it.next();
    currentLabel->increaseFontSize();
  }
}

void NetworkGraphWidget::decreaseLabelSize() 
{
  QVectorIterator<NetworkNodeLabel*> it(_networkNodeLabelVector);
  while (it.hasNext()) 
  {
    NetworkNodeLabel *currentLabel = it.next();
    currentLabel->decreaseFontSize();
  }
}

void NetworkGraphWidget::setRangeControls() 
{
  QVectorIterator<QString> it(_presentTypes);
  while (it.hasNext())
  {
    QString currentType = it.next();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT DISTINCT relationship, incident "
                   "FROM relationships_to_incidents "
                   "WHERE type = :type AND coder = :coder");
    query->bindValue(":type", currentType);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
      QString relationship = query->value(0).toString();
      int incident = query->value(1).toInt();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
      query2->bindValue(":incident", incident);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (_minOrder == 0)
      {
        _minOrder = order;
      }
      else if (order < _minOrder)
      {
        _minOrder = order;
      }
      if (_maxOrder == 0)
      {
        _maxOrder = order;
      }
      else if (order > _maxOrder)
      {
        _maxOrder = order;
      }
      delete query2;
    }
    delete query;
  }
  lowerRangeDial->setRange(_minOrder, _maxOrder - 1);
  upperRangeDial->setRange(_minOrder + 1, _maxOrder);
  lowerRangeSpinBox->setRange(_minOrder, _maxOrder - 1);
  upperRangeSpinBox->setRange(_minOrder + 1, _maxOrder);
  lowerRangeDial->setValue(_minOrder);
  lowerRangeSpinBox->setValue(_minOrder);
  upperRangeDial->setValue(_maxOrder);
  upperRangeSpinBox->setValue(_maxOrder);
}

void NetworkGraphWidget::updateRangeControls() 
{
  QVectorIterator<QString> it(_presentTypes);
  while (it.hasNext())
  {
    QString currentType = it.next();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT DISTINCT relationship, incident "
                   "FROM relationships_to_incidents "
                   "WHERE type = :type AND coder = :coder");
    query->bindValue(":type", currentType);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
      QString relationship = query->value(0).toString();
      int incident = query->value(1).toInt();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
      query2->bindValue(":incident", incident);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (_minOrder == 0)
      {
        _minOrder = order;
      }
      else if (order < _minOrder)
      {
        _minOrder = order;
      }
      if (_maxOrder == 0)
      {
        _maxOrder = order;
      }
      else if (order > _maxOrder)
      {
        _maxOrder = order;
      }
      delete query2;
    }
    delete query;
  }
  int currentLower = lowerRangeDial->value();
  int currentUpper = upperRangeDial->value();
  lowerRangeDial->setRange(_minOrder, _maxOrder - 1);
  upperRangeDial->setRange(_minOrder + 1, _maxOrder);
  lowerRangeSpinBox->setRange(_minOrder, _maxOrder - 1);
  upperRangeSpinBox->setRange(_minOrder + 1, _maxOrder);
  lowerRangeDial->setValue(currentLower);
  lowerRangeSpinBox->setValue(currentLower);
  upperRangeDial->setValue(currentUpper);
  upperRangeSpinBox->setValue(currentUpper);
}

void NetworkGraphWidget::setTimeRange()
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

void NetworkGraphWidget::setWeightControls()
{
  _maxWeight = 0;
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext())
  {
    DirectedEdge *directed = it.next();
    if (directed->isVisible())
    {
      if (_presentTypes.contains(directed->getType()))
      {
        QList<int> incidents = directed->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        int count = 0;
        if (directed->isFiltered())
        {
          QListIterator<int> it2(incidents);
          while (it2.hasNext())
          {
            int incident = it2.next();
            if (incident >= lowerRangeDial->value() && incident <= upperRangeDial->value())
            {
              count++;
            }
          }
        }
        else
        {
          count = incidents.size();
        }
        if (count > _maxWeight)
        {
          _maxWeight = count;
        }
      }
    }
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext())
  {
    UndirectedEdge *undirected = it2.next();
    if (undirected->isVisible())
    {
      if (_presentTypes.contains(undirected->getType()))
      {
        QList<int> incidents = undirected->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        int count = 0;
        QListIterator<int> it3(incidents);
        if (undirected->isFiltered())
        {
          while (it3.hasNext())
          {
            int incident = it3.next();
            if (incident >= lowerRangeDial->value() && incident <= upperRangeDial->value())
            {
              count++;
            }
          }
        }
        else
        {
          count = incidents.size();
        }
        if (count > _maxWeight)
        {
          _maxWeight = count;
        }
      }
    }
  }
  weightSpinBox->setEnabled(true);
  weightSpinBox->setRange(1, _maxWeight);
}

void NetworkGraphWidget::updateWeightControls()
{
  int currentWeight = weightSpinBox->value();
  _maxWeight = 0;
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext())
  {
    DirectedEdge *directed = it.next();
    if (directed->isVisible())
    {
      if (_presentTypes.contains(directed->getType()))
      {
        QList<int> incidents = directed->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        int count = 0;
        if (directed->isFiltered())
        {
          QListIterator<int> it2(incidents);
          while (it2.hasNext())
          {
            int incident = it2.next();
            if (incident >= lowerRangeDial->value() && incident <= upperRangeDial->value())
            {
              count++;
            }
          }
        }
        else
        {
          count = incidents.size();
        }
        if (count > _maxWeight)
        {
          _maxWeight = count;
        }
      }
    }
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext())
  {
    UndirectedEdge *undirected = it2.next();
    if (undirected->isVisible())
    {
      if (_presentTypes.contains(undirected->getType()))
      {
        QList<int> incidents = undirected->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        int count = 0;
        if (undirected->isFiltered())
        {
          QListIterator<int> it3(incidents);
          while (it3.hasNext())
          {
            int incident = it3.next();
            if (incident >= lowerRangeDial->value() && incident <= upperRangeDial->value())
            {
              count++;
            }
          }
        }
        else
        {
          count = incidents.size();
        }
        if (count > _maxWeight)
        {
          _maxWeight = count;
        }
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

void NetworkGraphWidget::exportSvg() 
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

void NetworkGraphWidget::exportNodes() 
{
  QVector<QString> entities;
  QVector<QString> descriptions;
  QVector<QString> modes;
  
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode *node = it.next();
    if (node->isVisible())
    {
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

void NetworkGraphWidget::exportEdges() 
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
            << "Description" << ","
            << "Comment" << "\n";
    // Then we iterate through the directed edges first.
    QVectorIterator<DirectedEdge*> it(_directedVector);
    while (it.hasNext())
    {
      DirectedEdge *directed = it.next();
      if (directed->isVisible())
      {
        QString source = directed->getStart()->getName();
        QString target = directed->getEnd()->getName();
        QString description = directed->getType();
        QList<int> incidents = directed->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        int weight = 0;
        if (weightCheckBox->checkState() == Qt::Checked)
        {
          QListIterator<int> it2(incidents);
          while (it2.hasNext())
          {
            int incident = it2.next();
            if (incident >= lowerRangeDial->value() &&
                incident <= upperRangeDial->value())
            {
              weight++;
            }
          }
        }
        else
        {
          weight = 1;
        }
        QString comment = directed->getComment();
        fileOut << "\"" << doubleQuote(source).toStdString() << "\"" << ","
                << "\"" << doubleQuote(target).toStdString() << "\"" << ","
                << "Directed" << ","
                << weight << ","
                << "\"" << doubleQuote(description).toStdString() << "\"" << ","
                << "\"" << doubleQuote(comment).toStdString() << "\"" << "\n";
      }
    }
    // Then we iterate through the undirected edges second.
    QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
    while (it2.hasNext())
    {
      UndirectedEdge* undirected = it2.next();
      if (undirected->isVisible())
      {
        QString source = undirected->getStart()->getName();
        QString target = undirected->getEnd()->getName();
        QString description = undirected->getType();
        QString comment = undirected->getComment();
        QList<int> incidents = undirected->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        int weight = 0;
        if (weightCheckBox->checkState() == Qt::Checked)
        {
          QListIterator<int> it2(incidents);
          while (it2.hasNext())
          {
            int incident = it2.next();
            if (incident >= lowerRangeDial->value() &&
                incident <= upperRangeDial->value())
            {
              weight++;
            }
          }
        }
        else
        {
          weight = 1;
        }
        fileOut << "\"" << doubleQuote(source).toStdString() << "\"" << ","
                << "\"" << doubleQuote(target).toStdString() << "\"" << ","
                << "Undirected" << ","
                << weight << ","
                << "\"" << doubleQuote(description).toStdString() << "\"" << ","
                << "\"" << doubleQuote(comment).toStdString() << "\"" << "\n";
      }
    }
    // And that should be it!
    fileOut.close();
  }
}

void NetworkGraphWidget::exportRelationalEvents()
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
    fileOut << "Order " << ","
            << "Timing " << ","
            << "Source" << ","
            << "Target" << ","
            << "Interaction" << "\n";
    // Now we need to gather the information we wish to output.
    QMultiMap<int, QGraphicsItem*> interactions;
    QVectorIterator<DirectedEdge*> it(_directedVector);
    while (it.hasNext())
    {
      DirectedEdge *directed = it.next();
      if (directed->isVisible())
      {
        QList<int> incidents = directed->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        QListIterator<int> it2(incidents);
        while (it2.hasNext())
        {
          int incident = it2.next();
          interactions.insert(incident, directed);
        }
      }
    }
    QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
    while (it3.hasNext())
    {
      UndirectedEdge *undirected = it3.next();
      if (undirected->isVisible())
      {
        QList<int> incidents = undirected->getIncidents().toList();
        std::sort(incidents.begin(), incidents.end());
        QListIterator<int> it4(incidents);
        while (it4.hasNext())
        {
          int incident = it4.next();
          interactions.insert(incident, undirected);
        }
      }
    }
    // Then we write the file.
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT timestamp FROM incidents WHERE ch_order = :ch_order");
    QList<int> incidents = interactions.keys();
    std::sort(incidents.begin(), incidents.end());
    QListIterator<int> it5(incidents);
    while (it5.hasNext())
    {
      int incident = it5.next();
      query->bindValue(":ch_order", incident);
      query->exec();
      query->first();
      QString timing = query->value(0).toString();
      QList<QGraphicsItem*> relationships = interactions.values(incident);
      QListIterator<QGraphicsItem*> it6(relationships);
      while (it6.hasNext())
      {
        QGraphicsItem *current = it6.next();
        DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(current);
        UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(current);
        QString type = QString();
        QString source = QString();
        QString target = QString();
        if (directed)
        {
          type = directed->getType();
          source = directed->getStart()->getName();
          target = directed->getEnd()->getName();
        }
        else if (undirected)
        {
          type = undirected->getType();
          source = undirected->getStart()->getName();
          target = undirected->getEnd()->getName();
        }
        fileOut << incident << ","
                << "\"" << doubleQuote(timing).toStdString() << "\"" << ","
                << "\"" << doubleQuote(source).toStdString() << "\"" << ","
                << "\"" << doubleQuote(target).toStdString() << "\"" << ","
                << "\"" << doubleQuote(type).toStdString() << "\"" << "\n";
      }
    }
    delete query;
  }
}

void NetworkGraphWidget::saveCurrentPlot() 
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
    query->prepare("SELECT plot FROM saved_ng_plots WHERE plot = :name AND coder = :coder");
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
      QColor color = scene->backgroundBrush().color();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      query->prepare("UPDATE saved_ng_plots "
                     "SET red = :red, green = :green, blue = :blue "
                     "WHERE plot = :name AND coder = :coder");
      query->bindValue(":red", red);
      query->bindValue(":green", green);
      query->bindValue(":blue", blue);
      query->bindValue(":name", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_network_nodes
      query->prepare("DELETE FROM saved_ng_plots_network_nodes "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_directed
      query->prepare("DELETE FROM saved_ng_plots_directed "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_undirected
      query->prepare("DELETE FROM saved_ng_plots_undirected "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_incidents_to_edges
      query->prepare("DELETE FROM saved_ng_plots_incidents_to_edges "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_node_labels
      query->prepare("DELETE FROM saved_ng_plots_node_labels "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_nodelegend
      query->prepare("DELETE FROM saved_ng_plots_nodelegend "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_edgelegend
      query->prepare("DELETE FROM saved_ng_plots_edgelegend "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_lines
      query->prepare("DELETE FROM saved_ng_plots_lines "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_texts
      query->prepare("DELETE FROM saved_ng_plots_texts "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_ellipses
      query->prepare("DELETE FROM saved_ng_plots_ellipses "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_rects
      query->prepare("DELETE FROM saved_ng_plots_rects "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_settings
      query->prepare("DELETE FROM saved_ng_plots_settings "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_ng_plots_cases
      query->prepare("DELETE FROM saved_ng_plots_cases "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      // saved_n_plots_guides
      query->prepare("DELETE FROM saved_ng_plots_guides "
                     "WHERE plot = :plot AND coder = :coder");
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
    }
    else
    {
      // Insert new data into saved_ng_plots and then write data.
      QColor color = scene->backgroundBrush().color();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      query->prepare("INSERT INTO saved_ng_plots (plot, coder, red, green, blue) "
                     "VALUES (:name, :coder, :red, :green, :blue)");
      query->bindValue(":name", name);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":red", red);
      query->bindValue(":green", green);
      query->bindValue(":blue", blue);
      query->exec();
    }
    QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, _networkNodeVector.size());
    saveProgress->setWindowTitle("Saving nodes");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    int counter = 1;
    saveProgress->show();
    QSqlDatabase::database().transaction();
    query->prepare("INSERT INTO saved_ng_plots_network_nodes "
                   "(plot, coder, entity, description, mode, curxpos, curypos, "
                   "red, green, blue, alpha, zvalue, hidden, persistent, masshidden) "
                   "VALUES (:plot, :coder, :entity, :description, :mode, :curxpos, :curypos, "
                   ":red, :green, :blue, :alpha, :zvalue, :hidden, :persistent, :masshidden)");
    QVectorIterator<NetworkNode*> it(_networkNodeVector);
    while (it.hasNext())
    {
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
      int zvalue = current->zValue();
      int hidden = 1;
      int persistent = 0;
      int masshidden = 0;
      if (current->isVisible())
      {
        hidden = 0;
      }
      if (current->isPersistent())
      {
        persistent = 1;
      }
      if (current->isMassHidden())
      {
        masshidden = 1;
      }
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":entity", entity);
      query->bindValue(":description", description);
      query->bindValue(":mode", mode);
      query->bindValue(":curxpos", x);
      query->bindValue(":curypos", y);
      query->bindValue(":red", red);
      query->bindValue(":green", green);
      query->bindValue(":blue", blue);
      query->bindValue(":alpha", alpha);
      query->bindValue(":zvalue", zvalue);
      query->bindValue(":hidden", hidden);
      query->bindValue(":persistent", persistent);
      query->bindValue(":masshidden", masshidden);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, _networkNodeLabelVector.size());
    saveProgress->setWindowTitle("Saving node labels");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_node_labels "
                   "(plot, coder, entity, curxpos, curypos, xoffset, yoffset, fontsize, "
                   "red, green, blue, alpha, zvalue, hidden) "
                   "VALUES (:plot, :coder, :entity, :curxpos, :curypos, :xoffset, :yoffset, "
                   ":fontsize, "
                   ":red, :green, :blue, :alpha, :zvalue, :hidden)");
    QVectorIterator<NetworkNodeLabel*> it2(_networkNodeLabelVector);
    while (it2.hasNext())
    {
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
      int zvalue = current->zValue();
      int hidden = 1;
      if (current->isVisible())
      {
        hidden = 0;
      }
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
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
      query->bindValue(":zvalue",zvalue);
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
    query->prepare("INSERT INTO saved_ng_plots_nodelegend (plot, coder, name, tip, "
                   "red, green, blue, alpha, textred, textgreen, textblue, textalpha, hidden) "
                   "VALUES (:plot, :coder, :name, :tip, :red, :green, :blue, :alpha, "
                   ":textred, :textgreen, :textblue, :textalpha, :hidden)");
    for (int i = 0; i != nodeListWidget->rowCount(); i++)
    {
      QTableWidgetItem *item = nodeListWidget->item(i, 0);
      QString title = item->data(Qt::DisplayRole).toString();
      QString tip = item->data(Qt::ToolTipRole).toString();
      QColor color = nodeListWidget->item(i, 1)->background().color();
      QVariant textColorVar = nodeListWidget->item(i, 1)->data(Qt::UserRole);
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
      if (nodeListWidget->item(i, 0)->background() == QColor(Qt::gray))
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
    saveProgress = new ProgressBar(0, 1, edgeListWidget->rowCount());
    saveProgress->setWindowTitle("Saving edge legend");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_edgelegend (plot, coder, name, tip, "
                   "red, green, blue, alpha, hidden) "
                   "VALUES (:plot, :coder, :name, :tip, :red, :green, :blue, :alpha, :hidden)");
    for (int i = 0; i != edgeListWidget->rowCount(); i++)
    {
      QTableWidgetItem *item = edgeListWidget->item(i, 0);
      QString title = item->data(Qt::DisplayRole).toString();
      QString tip = item->data(Qt::ToolTipRole).toString();
      QColor color = edgeListWidget->item(i, 1)->background().color();
      int red = color.red();
      int green = color.green();
      int blue = color.blue();
      int alpha = color.alpha();
      int hidden = 0;
      if (edgeListWidget->item(i, 0)->background() == QColor(Qt::gray))
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
      query->bindValue(":hidden", hidden);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, _directedVector.size());
    saveProgress->setWindowTitle("Saving directed edges");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_directed "
                   "(plot, coder, tail, head, name, comment, type, height, filtered, masshidden, "
                   "red, green, blue, alpha, zvalue, hidden) "
                   "VALUES (:plot, :coder, :tail, :head, :name, :comment, :type, :height, "
                   ":filtered, "
                   ":masshidden, :red, :green, :blue, :alpha, :zvalue, :hidden)");
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("INSERT INTO saved_ng_plots_incidents_to_edges"
                    "(plot, coder, incident, relationship, type) "
                    "VALUES (:plot, :coder, :incident, :relationship, :type)");
    QVectorIterator<DirectedEdge*> it3(_directedVector);
    while (it3.hasNext())
    {
      DirectedEdge *current = it3.next();
      QString tail = current->getStart()->getName();
      QString head = current->getEnd()->getName();
      QString relationship = current->getName();
      QString comment = current->getComment();
      QString type = current->getType();
      QSet<int> incidents = current->getIncidents();
      QSetIterator<int> incIt(incidents);
      while (incIt.hasNext())
      {
        int incident = incIt.next();
        query2->bindValue(":plot", name);
        query2->bindValue(":coder", _selectedCoder);
        query2->bindValue(":incident", incident);
        query2->bindValue(":type", type);
        query2->bindValue(":relationship", relationship);
        query2->exec();
      }
      int height = current->getHeight();
      int filtered = 0;
      if (current->isFiltered())
      {
        filtered = 1;
      }
      int massHidden = 0;
      if (current->isMassHidden())
      {
        massHidden = 1;
      }
      int red = current->getColor().red();
      int green = current->getColor().green();
      int blue = current->getColor().blue();
      int alpha = current->getColor().alpha();
      int zvalue = current->zValue();
      int hidden = 1;
      if (current->isVisible())
      {
        hidden = 0;
      }
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
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
      query->bindValue(":zvalue", zvalue);
      query->bindValue(":hidden", hidden);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    saveProgress->close();
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, _undirectedVector.size());
    saveProgress->setWindowTitle("Saving undirected edges");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_undirected "
                   "(plot, coder, tail, head, name, comment, type, height, filtered, masshidden, "
                   "red, green, blue, alpha, zvalue, hidden) "
                   "VALUES (:plot, :coder, :tail, :head, :name, :comment, :type, :height, "
                   ":filtered, "
                   ":masshidden, :red, :green, :blue, :alpha, :zvalue, :hidden)");
    QVectorIterator<UndirectedEdge*> it4(_undirectedVector);
    while (it4.hasNext())
    {
      UndirectedEdge *current = it4.next();
      QString tail = current->getStart()->getName();
      QString head = current->getEnd()->getName();
      QString relationship = current->getName();
      QString comment = current->getComment();
      QString type = current->getType();
      QSet<int> incidents = current->getIncidents();
      QSetIterator<int> incIt(incidents);
      while (incIt.hasNext())
      {
        int incident = incIt.next();
        query2->bindValue(":plot", name);
        query2->bindValue(":coder", _selectedCoder);
        query2->bindValue(":incident", incident);
        query2->bindValue(":type", type);
        query2->bindValue(":relationship", relationship);
        query2->exec();
      }
      int height = current->getHeight();
      int filtered = 0;
      if (current->isFiltered())
      {
        filtered = 1;
      }
      int massHidden = 0;
      if (current->isMassHidden())
      {
        massHidden = 1;
      }
      int red = current->getColor().red();
      int green = current->getColor().green();
      int blue = current->getColor().blue();
      int alpha = current->getColor().alpha();
      int zvalue = current->zValue();
      int hidden = 1;
      if (current->isVisible())
      {
        hidden = 0;
      }
      query->bindValue(":plot", name);
      query->bindValue(":coder", _selectedCoder);
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
      query->bindValue(":zvalue", zvalue);
      query->bindValue(":hidden", hidden);
      query->exec();
      counter++;
      saveProgress->setProgress(counter);
      qApp->processEvents();
    }
    delete query2;
    saveProgress->close();
    plotLabel->setText(name);
    changeLabel->setText("");
    delete saveProgress;
    saveProgress = new ProgressBar(0, 1, _lineVector.size());
    saveProgress->setWindowTitle("Saving line objects");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_lines "
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
    saveProgress = new ProgressBar(0, 1, _textVector.size());
    saveProgress->setWindowTitle("Saving text items");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_texts "
                   "(plot, coder, desc, xpos, ypos, width, size, rotation, zvalue, "
                   "red, green, blue, alpha) "
                   "VALUES (:plot, :coder, :desc, :xpos, :ypos, :width, :size, :rotation, "
                   ":zvalue, :red, :green, :blue, :alpha)");
    QVectorIterator<TextObject*> it9(_textVector);
    while (it9.hasNext())
    {
      TextObject *currentText = it9.next();
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
    query->prepare("INSERT INTO saved_ng_plots_ellipses "
                   "(plot, coder, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
                   "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
                   "penwidth, penstyle, zvalue, red, green, blue, alpha, "
                   "fillred, fillgreen, fillblue, fillalpha) "
                   "VALUES (:plot, :coder, :xpos, :ypos, :topleftx, :toplefty, :toprightx, "
                   ":toprighty, "
                   ":bottomleftx, :bottomlefty, :bottomrightx, :bottomrighty, :rotation, "
                   ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha, "
                   ":fillred, :fillgreen, :fillblue, :fillalpha)");
    QVectorIterator<EllipseObject*> it10(_ellipseVector);
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
    query->prepare("INSERT INTO saved_ng_plots_rects "
                   "(plot, coder, xpos, ypos, topleftx, toplefty, toprightx, toprighty, "
                   "bottomleftx, bottomlefty, bottomrightx, bottomrighty, rotation, "
                   "penwidth, penstyle, zvalue, red, green, blue, alpha, "
                   "fillred, fillgreen, fillblue, fillalpha) "
                   "VALUES (:plot, :coder, :xpos, :ypos, :topleftx, :toplefty, :toprightx, "
                   ":toprighty, "
                   ":bottomleftx, :bottomlefty, :bottomrightx, :bottomrighty, :rotation, "
                   ":penwidth, :penstyle, :zvalue, :red, :green, :blue, :alpha, "
                   ":fillred, :fillgreen, :fillblue, :fillalpha)");
    QVectorIterator<RectObject*> it11(_rectVector);
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
      QColor color = rect->getColor();
      int zValue = rect->zValue();
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
    query->prepare("INSERT INTO saved_ng_plots_settings "
                   "(plot, coder, lowerbound, upperbound, boundson, "
                   "weight, weighton, labelson) "
                   "VALUES (:plot, :coder, :lowerbound, :upperbound, :boundson, "
                   ":weight, :weighton, :labelson)");
    int boundson = 0;
    if (lowerRangeDial->isEnabled())
    {
      boundson = 1;
    }
    int weighton = 0;
    if (weightCheckBox->checkState() == Qt::Checked)
    {
      weighton = 1;
    }
    int labelson = 0;
    if (_labelsShown)
    {
      labelson = 1;
    }
    query->bindValue(":plot", name);
    query->bindValue(":coder", _selectedCoder);
    query->bindValue(":lowerbound", lowerRangeDial->value());
    query->bindValue(":upperbound", upperRangeDial->value());
    query->bindValue(":boundson", boundson);
    query->bindValue(":weight", weightSpinBox->value());
    query->bindValue(":weighton", weighton);
    query->bindValue(":labelson", labelson);
    query->exec();
    saveProgress = new ProgressBar(0, 1, caseListWidget->count());
    saveProgress->setWindowTitle("Saving cases");
    saveProgress->setAttribute(Qt::WA_DeleteOnClose);
    saveProgress->setModal(true);
    counter = 1;
    saveProgress->show();
    query->prepare("INSERT INTO saved_ng_plots_cases "
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
    query->prepare("INSERT INTO saved_ng_plots_guides "
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
    QSqlDatabase::database().commit();
  }
  delete saveDialog;
}		      

void NetworkGraphWidget::seePlots() 
{
  QPointer<SavedPlotsDialog> savedPlotsDialog = new SavedPlotsDialog(this,
                                                                     NETWORKGRAPH,
                                                                     _selectedCoder);
  savedPlotsDialog->exec();
  if (savedPlotsDialog->getExitStatus() == 0) 
  {
    hideAnnotationsButton->setChecked(false);
    savePlotButton->setEnabled(true);
    cleanUp();
    scene->clear();
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT red, green, blue "
                   "FROM saved_ng_plots "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    query->first();
    int red = query->value(0).toInt();
    int green = query->value(1).toInt();
    int blue = query->value(2).toInt();
    scene->setBackgroundBrush(QBrush(QColor(red, green, blue)));
    query->prepare("SELECT entity, description, mode, curxpos, curypos, "
                   "red, green, blue, alpha, zvalue, hidden, persistent, masshidden "
                   "FROM saved_ng_plots_network_nodes "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
      QString entity = query->value(0).toString();
      QString description = query->value(1).toString();
      QString mode = query->value(2).toString();
      qreal currentX = query->value(3).toReal();
      qreal currentY = query->value(4).toReal();
      int red = query->value(5).toInt();
      int green = query->value(6).toInt();
      int blue = query->value(7).toInt();
      int alpha = query->value(8).toInt();
      int zvalue = query->value(9).toInt();
      int hidden = query->value(10).toInt();
      int persistent = query->value(11).toInt();
      int masshidden = query->value(12).toInt();
      QPointF currentPos = QPointF(currentX, currentY);
      NetworkNode *node = new NetworkNode(entity, description);
      node->setColor(QColor(red, green, blue, alpha));
      node->setMode(mode);
      scene->addItem(node);
      node->setPos(currentPos);
      node->setZValue(zvalue);
      if (hidden == 1)
      {
        node->hide();
      }
      else
      {
        node->show();
      }
      if (persistent == 1)
      {
        node->setPersistent(true);
      }
      else
      {
        node->setPersistent(false);
      }
      _networkNodeVector.push_back(node);
      if (masshidden == 1)
      {
        node->setMassHidden(true);
      }
      else
      {
        node->setMassHidden(false);
      }
    }
    query->prepare("SELECT entity, curxpos, curypos, xoffset, yoffset, fontsize, "
                   "red, green, blue, alpha, zvalue, hidden "
                   "FROM saved_ng_plots_node_labels "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
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
      int zvalue = query->value(10).toInt();
      int hidden = query->value(11).toInt();
      QVectorIterator<NetworkNode*> it(_networkNodeVector);
      while (it.hasNext())
      {
        NetworkNode *node = it.next();
        QString nodeName = node->getName();
        if (nodeName == entity)
        {
          NetworkNodeLabel *currentLabel = new NetworkNodeLabel(node);
          currentLabel->setPlainText(entity);
          currentLabel->setPos(QPointF(currentX, currentY));
          currentLabel->setFontSize(fontSize);
          currentLabel->setOffset(QPointF(xOffset, yOffset));
          currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
          scene->addItem(currentLabel);
          currentLabel->setZValue(zvalue);
          if (hidden == 1)
          {
            currentLabel->hide();
          }
          else
          {
            currentLabel->show();
          }
          node->setLabel(currentLabel);
          _networkNodeLabelVector.push_back(currentLabel);
          break;
        }
      }
    }
    query->prepare("SELECT name, tip, red, green, blue, alpha, "
                   "textred, textgreen, textblue, textalpha, hidden "
                   "FROM saved_ng_plots_nodelegend "
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
      nodeListWidget->setRowCount(nodeListWidget->rowCount() + 1);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 0, item);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setBackground(color);
      QVariant textColorVar = QVariant(textColor.rgb());
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setData(Qt::UserRole,
                                                                       textColorVar);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->
        setFlags(nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->flags() ^
                 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
      if (hidden == 1)
      {
        nodeListWidget->item(nodeListWidget->rowCount() - 1, 0)->setBackground(Qt::gray);
      }
    }
    query->prepare("SELECT name, tip, red, green, blue, alpha, hidden "
                   "FROM saved_ng_plots_edgelegend "
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
      QColor color = QColor(red, green, blue, alpha);
      int hidden = query->value(6).toInt();
      _presentTypes.push_back(name);
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
      if (hidden == 1)
      {
        edgeListWidget->item(edgeListWidget->rowCount() - 1, 0)->setBackground(Qt::gray);
      }
    }
    query->prepare("SELECT tail, head, name, comment, type, height, filtered, masshidden, "
                   "red, green, blue, alpha, zvalue, hidden "
                   "FROM saved_ng_plots_directed "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
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
      int zvalue = query->value(12).toInt();
      int hidden = query->value(13).toInt();
      QVectorIterator<NetworkNode*> it(_networkNodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext())
      {
        NetworkNode *currentItem = it.next();
        if (currentItem->getName() == tail)
        {
          tempSource = currentItem;
        }
        else if (currentItem->getName() == head)
        {
          tempTarget = currentItem;
        }
        if (tempSource != NULL && tempTarget != NULL)
        {
          DirectedEdge *currentEdge = new DirectedEdge(tempSource, tempTarget,
                                                       type, name, 0);
          currentEdge->setZValue(zvalue);
          currentEdge->setComment(comment);
          currentEdge->setHeight(height);
          if (filtered == 1)
          {
            currentEdge->setFiltered(true);
          }
          if (massHidden == 1)
          {
            currentEdge->setMassHidden(true);
          }
          currentEdge->setColor(QColor(red, green, blue, alpha));
          if (hidden == 1)
          {
            currentEdge->hide();
          }
          scene->addItem(currentEdge);
          _directedVector.push_back(currentEdge);
          break;
        }
      }
    }
    plotLabel->setText(plot);
    changeLabel->setText("");
    query->prepare("SELECT tail, head, name, comment, type, height, filtered, masshidden, "
                   "red, green, blue, alpha, zvalue, hidden "
                   "FROM saved_ng_plots_undirected "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
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
      int zvalue = query->value(12).toInt();
      int hidden = query->value(13).toInt();
      QVectorIterator<NetworkNode*> it(_networkNodeVector);
      NetworkNode *tempSource = NULL;
      NetworkNode *tempTarget = NULL;
      while (it.hasNext())
      {
        NetworkNode *currentItem = it.next();
        if (currentItem->getName() == tail)
        {
          tempSource = currentItem;
        }
        else if (currentItem->getName() == head)
        {
          tempTarget = currentItem;
        }
        if (tempSource != NULL && tempTarget != NULL)
        {
          UndirectedEdge *currentEdge = new UndirectedEdge(tempSource, tempTarget,
                                                           type, name, 0);
          currentEdge->setZValue(zvalue);
          currentEdge->setComment(comment);
          currentEdge->setHeight(height);
          if (filtered == 0)
          {
            currentEdge->setFiltered(false);
          }
          if (massHidden == 1)
          {
            currentEdge->setMassHidden(true);
          }
          currentEdge->setColor(QColor(red, green, blue, alpha));
          if (hidden == 1)
          {
            currentEdge->hide();
          }
          scene->addItem(currentEdge);
          _undirectedVector.push_back(currentEdge);
          break;
        }
      }
    }
    query->prepare("SELECT incident, relationship, type "
                   "FROM saved_ng_plots_incidents_to_edges "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
      int incident = query->value(0).toInt();
      QString relationship = query->value(1).toString();
      QString type = query->value(2).toString();
      QVectorIterator<DirectedEdge*> dit(_directedVector);
      while (dit.hasNext())
      {
        DirectedEdge *current = dit.next();
        if (current->getName() == relationship && current->getType() == type)
        {
          current->insertIncident(incident);
        }
      }
      QVectorIterator<UndirectedEdge*> uit(_undirectedVector);
      while (uit.hasNext())
      {
        UndirectedEdge *current = uit.next();
        if (current->getName() == relationship && current->getType() == type)
        {
          current->insertIncident(incident);
        }
      }
    }
    query->prepare("SELECT DISTINCT incident "
                   "FROM relationships_to_incidents "
                   "WHERE coder = :coder");
    query->bindValue("coder", _selectedCoder);
    query->exec();
    while (query->next())
    {
      int incident = query->value(0).toInt();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
      query2->bindValue(":incident", incident);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (_minOrder == 0)
      {
        _minOrder = order;
      }
      else if (order < _minOrder)
      {
        _minOrder = order;
      }
      if (_maxOrder == 0)
      {
        _maxOrder = order;
      }
      else if (order > _maxOrder)
      {
        _maxOrder = order;
      }
      delete query2;
    }
    setRangeControls();
    query->prepare("SELECT startx, starty, endx, endy, arone, artwo, penwidth, penstyle, "
                   "zvalue, red, green, blue, alpha "
                   "FROM saved_ng_plots_lines "
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
    query->prepare("SELECT desc, xpos, ypos, width, size, rotation, zvalue, "
                   "red, green, blue, alpha "
                   "FROM saved_ng_plots_texts "
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
                   "FROM saved_ng_plots_ellipses "
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
                   "FROM saved_ng_plots_rects "
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
    setWeightControls();
    query->prepare("SELECT lowerbound, upperbound, boundson, weight, weighton, labelson "
                   "FROM saved_ng_plots_settings "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    query->first();
    int lowerbound = query->value(0).toInt();
    int upperbound = query->value(1).toInt();
    int boundson = query->value(2).toInt();
    int weight = query->value(3).toInt();
    int weighton = query->value(4).toInt();
    int labelson = query->value(5).toInt();
    lowerRangeDial->setValue(lowerbound);
    lowerRangeSpinBox->setValue(lowerbound);
    upperRangeDial->setValue(upperbound);
    upperRangeSpinBox->setValue(upperbound);
    if (boundson == 0)
    {
      lowerRangeDial->setEnabled(false);
      lowerRangeSpinBox->setEnabled(false);
      upperRangeDial->setEnabled(false);
      upperRangeSpinBox->setEnabled(false);
    }
    else
    {
      lowerRangeDial->setEnabled(true);
      lowerRangeSpinBox->setEnabled(true);
      upperRangeDial->setEnabled(true);
      upperRangeSpinBox->setEnabled(true);
    }
    weightSpinBox->setValue(weight);
    if (weighton == 0)
    {
      weightCheckBox->setCheckState(Qt::Unchecked);
    }
    else
    {
      weightCheckBox->setCheckState(Qt::Checked);
    }
    if (labelson == 0)
    {
      _labelsShown = false;
    }
    else
    {
      _labelsShown = true;
    }
    query->prepare("SELECT casename, checked "
                   "FROM saved_ng_plots_cases "
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
                   "FROM saved_ng_plots_guides "
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
    fixZValues();
    caseListWidget->setEnabled(true);
    // Now let's do the final processing
    checkCongruence();
    setVisibility();
    setGraphControls(true);
    plotLabel->setText(plot);
    changeLabel->setText("");
    view->fitInView(this->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    delete query;
  }
  else if (savedPlotsDialog->getExitStatus() == 2) 
  {
    // DON'T FORGET TO UPDATE THIS FUNCTION!!!!
    QString plot = savedPlotsDialog->getSelectedPlot();
    QSqlQuery *query = new QSqlQuery;
    // saved_ng_plots
    query->prepare("DELETE FROM saved_ng_plots "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_network_nodes
    query->prepare("DELETE FROM saved_ng_plots_network_nodes "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_node_labels
    query->prepare("DELETE FROM saved_ng_plots_node_labels "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    // saved_ng_plots_nodelegend
    query->prepare("DELETE FROM saved_ng_plots_nodelegend "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_edgelegend
    query->prepare("DELETE FROM saved_ng_plots_edgelegend "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_directed
    query->prepare("DELETE FROM saved_ng_plots_directed "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_undirected
    query->prepare("DELETE FROM saved_ng_plots_undirected "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_incidents_to_edges
    query->prepare("DELETE FROM saved_ng_plots_incidents_to_edges "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_lines
    query->prepare("DELETE FROM saved_ng_plots_lines "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_texts
    query->prepare("DELETE FROM saved_ng_plots_texts "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_ellipses
    query->prepare("DELETE FROM saved_ng_plots_ellipses "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_rects
    query->prepare("DELETE FROM saved_ng_plots_rects "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_settings
    query->prepare("DELETE FROM saved_ng_plots_settings "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_cases
    query->prepare("DELETE FROM saved_ng_plots_cases "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    // saved_ng_plots_guides
    query->prepare("DELETE FROM saved_ng_plots_guides "
                   "WHERE plot = :plot AND coder = :coder");
    query->bindValue(":plot", plot);
    query->bindValue(":coder", _selectedCoder);
    query->exec();
    delete query;
    seePlots();
  }
}

void NetworkGraphWidget::setChangeLabel() 
{
  if (changeLabel->text() == "" && _networkNodeVector.size() > 0) 
  {
    changeLabel->setText("*");
  }
}

void NetworkGraphWidget::updateEdges() 
{
  QVectorIterator<DirectedEdge*> it(_directedVector);
  while (it.hasNext()) 
  {
    DirectedEdge *current = it.next();
    current->updatePosition();
  }
  QVectorIterator<UndirectedEdge*> it2(_undirectedVector);
  while (it2.hasNext()) 
  {
    UndirectedEdge *current = it2.next();
    current->updatePosition();
  }
}

void NetworkGraphWidget::processLowerRange(int value) 
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

void NetworkGraphWidget::processUpperRange(int value) 
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

void NetworkGraphWidget::updateCases() 
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

void NetworkGraphWidget::setVisibility() 
{
  updateWeightControls();
  QSqlDatabase::database().transaction(); 
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT incident FROM relationships_to_incidents "
                 "WHERE relationship = :relationship AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
  QSqlQuery *query3 = new QSqlQuery;
  query3->prepare("SELECT incident FROM incidents_to_cases "
                  "WHERE incident = :incident AND casename = :casename");
  QVectorIterator<NetworkNode*> it(_networkNodeVector);
  while (it.hasNext()) 
  {
    NetworkNode* currentNode = it.next();
    currentNode->hide();
    if (currentNode->isPersistent())
    {
      currentNode->show();
    }
    if (currentNode->isMassHidden())
    {
      currentNode->hide();
    }
  }
  QVectorIterator<DirectedEdge*> it2(_directedVector);
  while (it2.hasNext()) 
  {
    bool show = false;
    DirectedEdge *currentDirected = it2.next();
    QString relationship = currentDirected->getName();
    QString type = currentDirected->getType();
    if (_presentTypes.contains(type))
    {
      if ((relationship == MERGED || relationship == TRANSFORMED) &&
          !(currentDirected->isMassHidden()))
      {
        show = true;
      }
      else if (currentDirected->isMassHidden())
      {
        show = false;
      }
      else if (currentDirected->isFiltered())
      {
        query->bindValue(":relationship", relationship);
        query->bindValue(":type", type);
        query->bindValue(":coder", _selectedCoder);
        query->exec();
        while (query->next())
        {
          int incident = query->value(0).toInt();
          query2->bindValue(":incident", incident);
          query2->exec();
          query2->first();
          int order = query2->value(0).toInt();
          if (order >= lowerRangeDial->value() && order <= upperRangeDial->value())
          {
            show = true;
          }
        }
      }
      else
      {
        show = true;
      }
    }
    if (_checkedCases.size() > 0)
    {
      query->bindValue(":relationship", relationship);
      query->bindValue(":type", type);
      query->exec();
      bool found = false;
      while (query->next())
      {
        int incident = query->value(0).toInt();
        QVectorIterator<QString> it3(_checkedCases);
        while (it3.hasNext())
        {
          QString currentCase = it3.next();
          query3->bindValue(":incident", incident);
          query3->bindValue(":casename", currentCase);
          query3->exec();
          query3->first();
          if (!query3->isNull(0))
          {
            found = true;
          }
        }
      }
      if (!found)
      {
        show = false;
      }
    }
    if (currentDirected->getName() != TRANSFORMED)
    {
      QList<int> incidents = currentDirected->getIncidents().toList();
      std::sort(incidents.begin(), incidents.end());
      int count = 0;
      if (currentDirected->isFiltered())
      {
        QListIterator<int> incIt(incidents);
        while (incIt.hasNext())
        {
          int current = incIt.next();
          if (current >= lowerRangeDial->value() && current <= upperRangeDial->value())
          {
            count++;
          }
        }
      }
      else
      {
        count = incidents.size();
      }
      if (count < weightSpinBox->value())
      {
        show = false;
      }
      else
      {
        currentDirected->setToolTip("Occurrence: " + QString::number(count));
        if (weightCheckBox->checkState() == Qt::Checked)
        {
          qreal originalWeight = (qreal) count;
          if (originalWeight == 1.0f || _maxWeight == weightSpinBox->value())
          {
            currentDirected->setPenWidth(1.0f);
          }
          else
          {
            qreal normalizedWeight =
              ((originalWeight - weightSpinBox->value()) /
               (_maxWeight - weightSpinBox->value())) * (5.0f * 1.0f) + 1.0f ;
            currentDirected->setPenWidth(normalizedWeight);
          }
        }
        else
        {
          currentDirected->setPenWidth(1.0f);
        }
      }
    }
    else
    {
      currentDirected->setPenWidth(1.0f);
      currentDirected->setToolTip("Created through multimode transformation");
    }
    if (show)
    {
      if (!currentDirected->getStart()->isMassHidden() &&
          !currentDirected->getEnd()->isMassHidden())
      {
        currentDirected->show();
        currentDirected->getStart()->show();
        currentDirected->getEnd()->show();
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
  QSqlDatabase::database().commit();
  QSqlDatabase::database().transaction();
  query->prepare("SELECT incident FROM relationships_to_incidents "
                 "WHERE relationship = :relationship AND type = :type AND coder = :coder");
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :incident");
  QVectorIterator<UndirectedEdge*> it3(_undirectedVector);
  while (it3.hasNext()) 
  {
    bool show = false;
    UndirectedEdge *currentUndirected = it3.next();
    QString relationship = currentUndirected->getName();
    QString type = currentUndirected->getType();
    if (_presentTypes.contains(type))
    {
      if ((relationship == MERGED || relationship == TRANSFORMED)
          && !(currentUndirected->isMassHidden()))
      {
        show = true;
      }
      else if (currentUndirected->isMassHidden())
      {
        show = false;
      }
      else if (currentUndirected->isFiltered())
      {
        query->bindValue(":relationship", relationship);
        query->bindValue(":type", type);
        query->bindValue("coder", _selectedCoder);
        query->exec();
        while (query->next())
        {
          int incident = query->value(0).toInt();
          query2->bindValue(":incident", incident);
          query2->exec();
          query2->first();
          int order = query2->value(0).toInt();
          if (order >= lowerRangeDial->value() && order <= upperRangeDial->value())
          {
            show = true;
          }
        }
      }
      else
      {
        show = true;
      }
    }
    if (_checkedCases.size() > 0)
    {
      query->bindValue(":relationship", relationship);
      query->bindValue(":type", type);
      query->exec();
      bool found = false;
      while (query->next())
      {
        int incident = query->value(0).toInt();
        QVectorIterator<QString> it4(_checkedCases);
        while (it4.hasNext())
        {
          QString currentCase = it4.next();
          query3->bindValue(":incident", incident);
          query3->bindValue(":casename", currentCase);
          query3->exec();
          query3->first();
          if (!query3->isNull(0))
          {
            found = true;
          }
        }
      }
      if (!found)
      {
        show = false;
      }
    }
    if (currentUndirected->getName() != TRANSFORMED)
    {
      QList<int> incidents = currentUndirected->getIncidents().toList();
      std::sort(incidents.begin(), incidents.end());
      int count = 0;
      QListIterator<int> incIt(incidents);
      while (incIt.hasNext())
      {
        int current = incIt.next();
        if (current >= lowerRangeDial->value() && current <= upperRangeDial->value())
        {
          count++;
        }
      }
      if (count < weightSpinBox->value())
      {
        show = false;
      }
      else
      {
        currentUndirected->setToolTip("Occurrence: " + QString::number(count));
        if (weightCheckBox->checkState() == Qt::Checked)
        {
          qreal originalWeight = (qreal) count;
          if (originalWeight == 1.0f)
          {
            currentUndirected->setPenWidth(1.0f);
          }
          else
          {
            qreal normalizedWeight = ((originalWeight - 1.0f) /
                                      (_maxWeight - 1.0f)) * (5.0f * 1.0f) + 1.0f ;
            currentUndirected->setPenWidth(normalizedWeight);
          }
        }
        else
        {
          currentUndirected->setPenWidth(1.0f);
        }
      }
    }
    else
    {
      currentUndirected->setPenWidth(1.0f);
      currentUndirected->setToolTip("Created through mutltimode transformation");
    }
    if (show)
    {
      if (!currentUndirected->getStart()->isMassHidden() &&
          !currentUndirected->getEnd()->isMassHidden())
      {
        currentUndirected->show();
        currentUndirected->getStart()->show();
        currentUndirected->getEnd()->show();
      }
      else
      {
        currentUndirected->hide();
      }
    }
    else
    {
      currentUndirected->hide();
    }
  }
  QSqlDatabase::database().commit(); 
  processHeights();
  QVectorIterator<NetworkNodeLabel*> it4(_networkNodeLabelVector);
  while (it4.hasNext()) 
  {
    NetworkNodeLabel *label = it4.next();
    if (label->getNode()->isVisible() && _labelsShown)
    {
      label->show();
    }
    else
    {
      label->hide();
    }
  }
  delete query;
  delete query2;
  delete query3;
  updateEdges();
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
}

void NetworkGraphWidget::processHeights() 
{
  QList<QGraphicsItem*> edgeList;
  for (QVector<DirectedEdge*>::size_type i = 0; i != _directedVector.length(); i++) 
  {
    if (_directedVector[i]->isVisible())
    {
      _directedVector[i]->setHeight(20);
      edgeList.push_back(_directedVector[i]);
    }
  }
  for (QVector<UndirectedEdge*>::size_type i = 0; i != _undirectedVector.length(); i++) 
  {
    if (_undirectedVector[i]->isVisible())
    {
      _undirectedVector[i]->setHeight(20);
      edgeList.push_back(_undirectedVector[i]);
    }
  }
  for (QList<QGraphicsItem*>::size_type i = 0; i != edgeList.length(); i++) 
  {
    DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(edgeList[i]);
    UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(edgeList[i]);
    if (i != edgeList.length() - 1)
    {
      for (QList<QGraphicsItem*>::size_type j = i + 1; j != edgeList.length(); j++)
      {
        DirectedEdge *secondDirected = qgraphicsitem_cast<DirectedEdge*>(edgeList[j]);
        UndirectedEdge *secondUndirected = qgraphicsitem_cast<UndirectedEdge*>(edgeList[j]);
        if (directed && secondDirected)
        {
          if (secondDirected->getStart() == directed->getStart() &&
              secondDirected->getEnd() == directed->getEnd())
          {
            directed->setHeight(directed->getHeight() + 60);
          }
        }
        else if (directed && secondUndirected)
        {
          if (secondUndirected->getStart() == directed->getStart() &&
              secondUndirected->getEnd() == directed->getEnd())
          {
            directed->setHeight(directed->getHeight() + 60);
          }
        }
        else if (undirected && secondDirected)
        {
          if (secondDirected->getStart() == undirected->getStart() &&
              secondDirected->getEnd() == undirected->getEnd())
          {
            undirected->setHeight(undirected->getHeight() + 60);
          }
        }
        else if (undirected && secondUndirected)
        {
          if (secondUndirected->getStart() == undirected->getStart() &&
              secondUndirected->getEnd() == undirected->getEnd())
          {
            undirected->setHeight(undirected->getHeight() + 60);
          }
        }
      }
    }
  }
}

void NetworkGraphWidget::setButtons() 
{
  valueButton->setEnabled(false);
  valueField->setEnabled(false);
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->blockSignals(false);
  if (attributesTreeView->currentIndex().isValid()) 
  {
    QString currentAttribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    bool empty = false;
    query->prepare("SELECT attribute, entity, value FROM "
                   "attributes_to_entities "
                   "WHERE attribute = :att AND entity = :ent  ");
    query->bindValue(":att", currentAttribute);
    query->bindValue(":ent", _selectedEntityName);
    query->exec();
    query->first();
    empty = query->isNull(0);
    if (!empty)
    {
      unassignAttributeButton->setEnabled(true);
      assignAttributeButton->setEnabled(false);
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
      assignAttributeButton->setEnabled(true);
    }
    editAttributeButton->setEnabled(true);
    delete query;
  }
  else 
  {
    editAttributeButton->setEnabled(false);
    assignAttributeButton->setEnabled(false);
    unassignAttributeButton->setEnabled(false);
  }
}

void NetworkGraphWidget::cleanUp() 
{
  _currentData.clear();
  scene->clearSelection();
  qDeleteAll(_directedVector);
  _directedVector.clear();
  qDeleteAll(_undirectedVector);
  _undirectedVector.clear();
  qDeleteAll(_networkNodeVector);
  _networkNodeVector.clear();
  qDeleteAll(_networkNodeLabelVector);
  _networkNodeLabelVector.clear();
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
  caseListWidget->clear();
  _presentTypes.clear();
  _checkedCases.clear();
  _minOrder = 0;
  _maxOrder = 0;
  _maxWeight = 0;
  _selectedEntityName = "";
  _selectedType = "";
  nameField->clear();
  descriptionField->clear();
  resetFont(attributesTree);
  snapGuidesButton->setChecked(false);
  toggleSnapGuides();
  setGraphControls(false);
  weightCheckBox->setCheckState(Qt::Unchecked);
  disableLegendButtons();
  plotLabel->setText("");
  incongruenceLabel->setText("");
  changeLabel->setText("");
}

void NetworkGraphWidget::finalBusiness() 
{
  cleanUp();
}

void NetworkGraphWidget::setCurrentCoder(QString coder)
{
  _selectedCoder = coder;
}

bool NetworkGraphWidget::eventFilter(QObject *object, QEvent *event) 
{
  if (event->type() == QEvent::MouseButtonRelease)
  {
    resetZoomSlider();
  }
  if (object == view->viewport() && event->type() == QEvent::MouseButtonRelease) 
  {
    retrieveData();
    setButtons();
    if (_massMove)
    {
      _massMove = false;
    }
  }
  else if (object == attributesTreeView && event->type() == QEvent::ChildRemoved) 
  {
    fixTree(_selectedEntityName);
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


