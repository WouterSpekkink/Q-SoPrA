#include "../include/OccurrenceGraphWidget.h"

/*
  Notes:
  I want to fetch the macro events from saved event graph plots later,
  and then group occurrence in the events that are visible.
*/

OccurrenceGraphWidget::OccurrenceGraphWidget(QWidget *parent) : QWidget(parent) 
{
  distance = 70;
  labelsVisible = true;
  matched = false;
  
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
  incongruencyLabel = new QLabel(tr(""), this);
  incongruencyLabel->setStyleSheet("QLabel {color : red;}");
  casesLabel = new QLabel(tr("<b>Case filtering:</b>"), graphicsWidget);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  
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
  
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  toggleLegendButton->setCheckable(true);
  toggleLegendButton->setChecked(true);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  addAttributeButton = new QPushButton(tr("Add attribute"), legendWidget);
  removeAttributeModeButton = new QPushButton(tr("Remove attribute"), legendWidget);
  removeAttributeModeButton->setEnabled(false);
  addRelationshipButton = new QPushButton(tr("Add relationship"), legendWidget);
  removeRelationshipModeButton = new QPushButton(tr("Remove relationship"), legendWidget);
  removeRelationshipModeButton->setEnabled(false);
  getEventsButton = new QPushButton(tr("Match current event graph"), this);
  restoreButton = new QPushButton(tr("Restore to original"), this);
  plotLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  changeLabelsButton = new QPushButton(tr("Short labels"), graphicsWidget);
  changeLabelsButton->setCheckable(true);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  increaseDistanceButton = new QPushButton(tr("< >"), this);
  decreaseDistanceButton = new QPushButton(tr("> <"), this);
  exportSvgButton = new QPushButton(tr("Export svg"), graphicsWidget);
  exportMatrixButton = new QPushButton(tr("Export matrix"), graphicsWidget);
  savePlotButton = new QPushButton(tr("Save plot"), this);
  savePlotButton->setEnabled(false);
  seePlotsButton = new QPushButton(tr("Saved plots"), this);
  
  view->viewport()->installEventFilter(this);
  
  // connections
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(addRelationshipButton, SIGNAL(clicked()), this, SLOT(addRelationship()));
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
  connect(getEventsButton, SIGNAL(clicked()), this, SLOT(getEvents()));
  connect(restoreButton, SIGNAL(clicked()), this, SLOT(restore()));
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
  connect(view, SIGNAL(OccurrenceGraphContextMenuAction(const QString &, const QPoint&)),
	  this, SLOT(processOccurrenceGraphContextMenu(const QString &, const QPoint&)));
  connect(caseListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(checkCases()));
  connect(plotLabelsButton, SIGNAL(clicked()), this, SLOT(plotLabels()));
  connect(changeLabelsButton, SIGNAL(clicked()), this, SLOT(changeLabels()));
  connect(backgroundColorButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(lowerRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeDial, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(lowerRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processLowerRange(int)));
  connect(upperRangeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(processUpperRange(int)));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  connect(exportMatrixButton, SIGNAL(clicked()), this, SLOT(exportMatrix()));
  connect(savePlotButton, SIGNAL(clicked()), this, SLOT(saveCurrentPlot()));
  connect(seePlotsButton, SIGNAL(clicked()), this, SLOT(seePlots()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
    
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(savePlotButton);
  plotOptionsLayout->addWidget(seePlotsButton);
  plotOptionsLayout->addWidget(plotLabel);
  plotOptionsLayout->addWidget(changeLabel);
  plotOptionsLayout->addWidget(incongruencyLabel);
  plotOptionsLayout->addSpacerItem(new QSpacerItem(100,0));
  topLayout->addLayout(plotOptionsLayout);
  plotOptionsLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(topLayout);

  QPointer<QHBoxLayout> screenLayout = new QHBoxLayout;

  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  leftLayout->addWidget(view);
  view->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
  screenLayout->addLayout(leftLayout);

  QPointer<QVBoxLayout> legendLayout = new QVBoxLayout;
  legendLayout->addWidget(attributeLegendLabel);
  legendLayout->addWidget(attributeListWidget);
  legendLayout->addWidget(addAttributeButton);
  legendLayout->addWidget(removeAttributeModeButton);
  legendLayout->addWidget(relationshipLegendLabel);
  legendLayout->addWidget(relationshipListWidget);
  legendLayout->addWidget(addRelationshipButton);
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
  graphicsControlsLayout->addWidget(changeLabelsButton);
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
  graphicsControlsLayout->addWidget(exportMatrixButton);
  graphicsWidget->setMaximumWidth(175);
  graphicsWidget->setMinimumWidth(175);
  graphicsWidget->setLayout(graphicsControlsLayout);
  graphicsControlsLayout->setAlignment(Qt::AlignBottom);
  screenLayout->addWidget(graphicsWidget);

  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  drawOptionsLeftLayout->addWidget(increaseDistanceButton);
  drawOptionsLeftLayout->addWidget(decreaseDistanceButton);
  increaseDistanceButton->setMaximumWidth(increaseDistanceButton->sizeHint().width());
  decreaseDistanceButton->setMaximumWidth(decreaseDistanceButton->sizeHint().width());
  drawOptionsLayout->addLayout(drawOptionsLeftLayout);
  drawOptionsLeftLayout->setAlignment(Qt::AlignLeft);

  QPointer<QHBoxLayout> drawOptionsRightLayout = new QHBoxLayout;
  drawOptionsRightLayout->addWidget(getEventsButton);
  drawOptionsRightLayout->addWidget(restoreButton);
  drawOptionsRightLayout->addWidget(toggleLegendButton);
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);

  setLayout(mainLayout);
  graphicsWidget->hide();
  updateCases();
}

void OccurrenceGraphWidget::checkCongruency() 
{
  qApp->setOverrideCursor(Qt::WaitCursor);
  if (attributeOccurrenceVector.size() > 0) 
    {
      QSqlDatabase::database().transaction();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute, incident FROM attributes_to_incidents "
		     "WHERE attribute = :attribute AND incident = :incident");
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents "
		      "WHERE id = :incident");
      QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem* current = it.next();
	  int id = current->getId();
	  if (id >= 0) 
	    {
	      int order = current->getOrder();
	      QString attribute = current->getAttribute();
	      QSqlQuery *query3 = new QSqlQuery;
	      bool entity = false;
	      query3->prepare("SELECT name FROM entities WHERE name = :name");
	      query3->bindValue(":name", attribute);
	      query3->exec();
	      query3->first();
	      if (!query3->isNull(0)) 
		{
		  entity = true;
		}
	      QVector<QString> attributeVector;
	      attributeVector.push_back(attribute);
	      findChildren(attribute, &attributeVector, entity);
	      QVectorIterator<QString> it2(attributeVector);
	      bool found = false;
	      while (it2.hasNext()) 
		{
		  QString currentAttribute = it2.next();
		  query->bindValue(":attribute", currentAttribute);
		  query->bindValue(":incident", id);
		  query->exec();
		  query->first();
		  if (!query->isNull(0)) 
		    {
		      found = true;
		    }
		}
	      if (!found) 
		{
		  incongruencyLabel->setText("Incongruency detected");
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
		  incongruencyLabel->setText("Incongruency detected");
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	      else if (query2->value(0).toInt() != order) 
		{
		  incongruencyLabel->setText("Incongruency detected");
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	    }
	}
      query->prepare("SELECT incident FROM attributes_to_incident "
		     "WHERE attribute = :attribute");
      for (int i = 0; i != attributeListWidget->rowCount(); i++) 
	{
	  QString text = attributeListWidget->item(i,0)->data(Qt::DisplayRole).toString();
	  query->bindValue(":attribute", text);
	  query->exec();
	  while (query->next()) 
	    {
	      int current = query->value(0).toInt();
	      bool found = false;
	      QVectorIterator<OccurrenceItem*> it2(attributeOccurrenceVector);
	      while (it2.hasNext()) 
		{
		  OccurrenceItem *item = it2.next();
		  if (item->getId() == current &&
		      item->getAttribute() == text) 
		    {
		      found = true;
		      break;
		    }
		}
	      if (!found) 
		{
		  incongruencyLabel->setText("Incongruency detected");
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
      incongruencyLabel->setText("");
      QSqlDatabase::database().commit();
    }
  if (relationshipOccurrenceVector.size() > 0) 
    {
      QSqlDatabase::database().transaction();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT relationship, type, incident FROM relationships_to_incidents "
		     "WHERE relationship = :relationship AND type = :type AND incident = :incident");
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents "
		      "WHERE id = :incident");
      QVectorIterator<OccurrenceItem*> it(relationshipOccurrenceVector);
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
	      bool relFin = false;
	      for (int i = 0; i != combi.length(); i++) 
		{
		  if (combi[i] != '(' && !relFin) 
		    {
		      relationship.append(combi[i]);
		    }
		  else if (combi[i] == '(') 
		    {
		      relFin = true;
		    }
		  if (relFin && combi[i] != ')' && combi[i] != '(') 
		    {
		      type.append(combi[i]);
		    }
		}
	      query->bindValue(":relationship", relationship);
	      query->bindValue(":type", type);
	      query->bindValue(":incident", id);
	      query->exec();
	      query->first();
	      if (!query->isNull(0)) 
		{
		  incongruencyLabel->setText("Incongruency detected");
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
		  incongruencyLabel->setText("Incongruency detected");
		  qApp->restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  delete query2;
		  return;
		}
	      else if (query2->value(0).toInt() != order) 
		{
		  incongruencyLabel->setText("Incongruency detected");
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
	  bool relFin = false;
	  for (int i = 0; i != combi.length(); i++) 
	    {
	      if (combi[i] != '(' && !relFin) 
		{
		  relationship.append(combi[i]);
		}
	      else if (combi[i] == '(') 
		{
		  relFin = true;
		}
	      if (relFin && combi[i] != ')' && combi[i] != '(') 
		{
		  type.append(combi[i]);
		}
	    }
	  query->bindValue(":relationship", relationship);
	  query->bindValue(":type", type);
	  query->exec();
	  while (query->next()) 
	    {
	      int current = query->value(0).toInt();
	      bool found = false;
	      QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
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
		  incongruencyLabel->setText("Incongruency detected");
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

bool OccurrenceGraphWidget::attributesPresent() 
{
  return presentAttributes.size() > 0;
}

bool OccurrenceGraphWidget::relationshipsPresent() 
{
  return presentRelationships.size() > 0;
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
      if (!presentAttributes.contains(attribute)) 
	{
	  presentAttributes.push_back(attribute);
	  savePlotButton->setEnabled(true);
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
		      QPointF position = QPointF((order * distance), 0);
		      OccurrenceItem *newOccurrence = new OccurrenceItem(40, incidentDescription, position,
									 currentIncident, order, attribute);
		      newOccurrence->setPos(newOccurrence->getOriginalPos());
		      newOccurrence->setColor(color);
		      newOccurrence->setZValue(3);
		      attributeOccurrenceVector.push_back(newOccurrence);
		      scene->addItem(newOccurrence);
		      OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
		      QString text = QString::number(order) + " - " + attribute;
		      label->setPlainText(text);
		      label->setDefaultTextColor(textColor);
		      label->setTextWidth(label->boundingRect().width());
		      label->setNewPos(newOccurrence->scenePos());
		      attributeLabelVector.push_back(label);
		      newOccurrence->setLabel(label);
		      label->setZValue(4);
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
    }
  delete attributeColorDialog;
  setRangeControls();
  scene->update();
  view->update();
  updateLinkages();
  checkCongruency();
  if (!caseListWidget->isEnabled()) 
    {
      caseListWidget->setEnabled(true);
    }
}

void OccurrenceGraphWidget::addRelationship() 
{
  setChangeLabel();
  QPointer<RelationshipColorDialog> relationshipColorDialog = new RelationshipColorDialog(this);
  relationshipColorDialog->setWindowTitle("Add relationship to plot");
  relationshipColorDialog->exec();
  if (relationshipColorDialog->getExitStatus() == 0) 
    {
      reset();
      QColor color = relationshipColorDialog->getColor();
      QColor textColor = relationshipColorDialog->getTextColor();
      QString relationship = relationshipColorDialog->getRelationship();
      QString type = relationshipColorDialog->getType();
      QString combi = relationship + " (" + type + ")";
      if (!presentRelationships.contains(combi)) 
	{
	  presentRelationships.push_back(combi);
	  savePlotButton->setEnabled(true);
	  QSqlQuery *query = new QSqlQuery;
	  QSqlQuery *query2 = new QSqlQuery;
	  query->prepare("SELECT description FROM relationship_types "
			 "WHERE name = :type");
	  query2->prepare("SELECT ch_order, description FROM incidents WHERE id = :id");
	  query->bindValue(":type", type);
	  query->exec();
	  query->first();
	  QString description = query->value(0).toString();
	  QVector<int> orders;
	  QSqlDatabase::database().transaction();
	  query->prepare("SELECT incident FROM relationships_to_incidents "
			 "WHERE relationship = :relationship AND type = :type");
	  query->bindValue(":relationship", relationship);
	  query->bindValue(":type", type);
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
		  QPointF position = QPointF((order * distance), 0);
		  OccurrenceItem *newOccurrence = new OccurrenceItem(40, incidentDescription, position,
								     currentIncident, order, combi);
		  newOccurrence->setPos(newOccurrence->getOriginalPos());
		  newOccurrence->setColor(color);
		  newOccurrence->setZValue(3);
		  relationshipOccurrenceVector.push_back(newOccurrence);
		  scene->addItem(newOccurrence);
		  OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
		  QString text = QString::number(order) + " - " + combi;
		  label->setPlainText(text);
		  label->setDefaultTextColor(textColor);
		  label->setTextWidth(label->boundingRect().width());
		  label->setNewPos(newOccurrence->scenePos());
		  relationshipLabelVector.push_back(label);
		  newOccurrence->setLabel(label);
		  label->setZValue(4);
		  scene->addItem(label);
		}
	    }
	  delete query;
	  delete query2;
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
	      relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->setBackground(color);
	      relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->
		setFlags(relationshipListWidget->item(relationshipListWidget->rowCount() - 1, 1)->flags() ^
			 Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
	    }
	  QSqlDatabase::database().commit();
	  groupOccurrences();
	  wireLinkages();
	}
    }
  delete relationshipColorDialog;
  setRangeControls();
  scene->update();
  view->update();
  updateLinkages();
  checkCongruency();
  if (!caseListWidget->isEnabled()) 
    {
      caseListWidget->setEnabled(true);
    }
}

void OccurrenceGraphWidget::setChangeLabel() 
{
  if (changeLabel->text() == "" && (attributeOccurrenceVector.size() > 0 ||
				    relationshipOccurrenceVector.size() > 0)) 
    {
      changeLabel->setText("*");
    }
}

void OccurrenceGraphWidget::updateLinkages() 
{
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) 
    {
      Arrow *current = it.next();
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
  for (QVector<OccurrenceItem*>::iterator it = attributeOccurrenceVector.begin();
       it != attributeOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getAttribute() == text) 
	{
	  delete current->getLabel();
	  attributeLabelVector.removeOne(current->getLabel());
	  delete current;
	  attributeOccurrenceVector.removeOne(current);
	  presentAttributes.removeOne(text);
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
  if (presentAttributes.size() > 0 || presentRelationships.size() > 0) 
    {
      groupOccurrences();
    }
  else if (presentAttributes.size() == 0 && presentRelationships.size() == 0) 
    {
      savePlotButton->setEnabled(false);
      caseListWidget->setEnabled(false);
    }
  wireLinkages();
  removeAttributeModeButton->setEnabled(false);
}

void OccurrenceGraphWidget::removeRelationshipMode() 
{
  setChangeLabel();
  QString text = relationshipListWidget->currentItem()->data(Qt::DisplayRole).toString();
  for (QVector<OccurrenceItem*>::iterator it = relationshipOccurrenceVector.begin();
       it != relationshipOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getAttribute() == text) 
	{
	  delete current->getLabel();
	  relationshipLabelVector.removeOne(current->getLabel());
	  delete current;
	  relationshipOccurrenceVector.removeOne(current);
	  presentRelationships.removeOne(text);
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
  if (presentAttributes.size() > 0 || presentRelationships.size() > 0) 
    {
      groupOccurrences();
    }
  else if (presentAttributes.size() == 0 && presentRelationships.size() == 0) 
    {
      savePlotButton->setEnabled(false);
    }
  wireLinkages();
  removeRelationshipModeButton->setEnabled(false);
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
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setCurrentColor(item->background().color());
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      colorDialog->setModal(true);
      if (colorDialog->exec()) 
	{
	  QColor color = colorDialog->selectedColor();
	  item->setBackground(color);
	  QTableWidgetItem* neighbour = attributeListWidget->item(item->row(), 0);
	  QString attribute = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
	  while (it.hasNext()) 
	    {
	      OccurrenceItem *current = it.next();
	      if (current->getAttribute() == attribute) 
		{
		  current->setColor(color);
		}
	    }
	}
    }
}

void OccurrenceGraphWidget::changeRelationshipModeColor(QTableWidgetItem *item) 
{
  if (item->column() == 1) 
    {
      QPointer<QColorDialog> colorDialog = new QColorDialog(this);
      colorDialog->setCurrentColor(item->background().color());
      colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
      colorDialog->setModal(true);
      if (colorDialog->exec()) 
	{
	  QColor color = colorDialog->selectedColor();
	  item->setBackground(color);
	  QTableWidgetItem* neighbour = relationshipListWidget->item(item->row(), 0);
	  QString relationship = neighbour->data(Qt::DisplayRole).toString();
	  QVectorIterator<OccurrenceItem*> it(relationshipOccurrenceVector);
	  while (it.hasNext()) 
	    {
	      OccurrenceItem *current = it.next();
	      if (current->getAttribute() == relationship) 
		{
		  current->setColor(color);
		}
	    }
	}
    }
}

void OccurrenceGraphWidget::groupOccurrences() 
{
  QVector<OccurrenceItem*> allOccurrences;
  std::sort(presentAttributes.begin(), presentAttributes.end(), stringSort);
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  int persistentDist = 0;
  while (it.hasNext()) 
    {
      OccurrenceItem *current = it.next();
      allOccurrences.push_back(current);
      current->setGrouped(false);
      int dist = 0;
      QVectorIterator<QString> it2(presentAttributes);
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
	      dist -= 250;
	      persistentDist -= 250;
	    }
	}
    }
  persistentDist -= 250;
  std::sort(presentRelationships.begin(), presentRelationships.end(), stringSort);
  QVectorIterator<OccurrenceItem*> it3(relationshipOccurrenceVector);
  while (it3.hasNext()) 
    {
      OccurrenceItem *current = it3.next();
      allOccurrences.push_back(current);
      current->setGrouped(false);
      int dist = persistentDist;
      QVectorIterator<QString> it4(presentRelationships);
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
	      dist -= 250;
	    }
	}
    }
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  QVector<OccurrenceItem*> finished;
  QVector<OccurrenceItem*> ignored;
  QVector<OccurrenceItem*>::iterator it5;
  for (it5 = allOccurrences.begin(); it5 != allOccurrences.end(); it5++) 
    {
      OccurrenceItem *first = *it5;
      QVector<OccurrenceItem*> group;
      QVector<QString> closedAttributes;
      if (!finished.contains(first) && !ignored.contains(first)) 
	{
	  group.push_back(first);
	  finished.push_back(first);
	  closedAttributes.push_back(first->getAttribute());
	  QVector<OccurrenceItem*>::iterator it6;
	  for (it6 = it5 + 1; it6 != allOccurrences.end(); it6++) 
	    {
	      OccurrenceItem *second = *it6;
	      if (!finished.contains(second) && !ignored.contains(second)) 
		{
		  // First we need to check if they are in the same x coordinate
		  if (second->scenePos().x() == first->scenePos().x()) 
		    {
		      // Next we need to check if they are of a different attribute.
		      if (second->getAttribute() != first->getAttribute()) 
			{
			  // We also need to check if we perhaps already have an entity with this attribute.
			  if (!closedAttributes.contains(second->getAttribute())) 
			    {
			      // Now we are sure that this entity belongs in this group.
			      group.push_back(second);
			      finished.push_back(second);
			      closedAttributes.push_back(second->getAttribute());
			    }
			  else 
			    {
			      ignored.push_back(second);
			    }
			}
		      else 
			{
			  ignored.push_back(second);
			}
		    }
		}
	    }
	  // Now we should have a finished group.
	  std::sort(group.begin(), group.end(), attributeLessThan);
	  qreal x = group.first()->scenePos().x();
	  qreal startY = group.first()->scenePos().y();
	  int dist = 80;
	  QVector<OccurrenceItem*>::iterator it7;
	  for (it7 = group.begin() + 1; it7 != group.end(); it7++) 
	    {
	      OccurrenceItem *current = *it7;
	      if (group.size() > 1) 
		{
		  current->setGrouped(true);
		}
	      current->setPos(x, startY - dist);
	      current->getLabel()->setNewPos(current->scenePos());
	      dist += 80;
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
  qDeleteAll(edgeVector);
  edgeVector.clear();
  std::sort(attributeOccurrenceVector.begin(), attributeOccurrenceVector.end(), eventLessThan);
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      OccurrenceItem *tempSource = it.next();
      OccurrenceItem *tempTarget = NULL;
      if (tempSource->isVisible()) 
	{
	  QVectorIterator<OccurrenceItem*> it2(attributeOccurrenceVector);
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
	      Arrow *newArrow = new Arrow(tempSource->getAttribute(), "");
	      newArrow->setZValue(2);
	      newArrow->setStartItem(tempSource);
	      newArrow->setEndItem(tempTarget);
	      newArrow->setCopy(true);
	      edgeVector.push_back(newArrow);
	      scene->addItem(newArrow);
	    }
	}
    }
  // And we do the same for the relationship->oriented occurrences
  std::sort(relationshipOccurrenceVector.begin(), relationshipOccurrenceVector.end(), eventLessThan);
  QVectorIterator<OccurrenceItem*> it3(relationshipOccurrenceVector);
  while (it3.hasNext()) 
    {
      OccurrenceItem *tempSource = it3.next();
      OccurrenceItem *tempTarget = NULL;
      if (tempSource->isVisible()) 
	{
	  QVectorIterator<OccurrenceItem*> it4(relationshipOccurrenceVector);
	  while (it4.hasNext()) 
	    {
	      OccurrenceItem *temp = it4.next();
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
	      Arrow *newArrow = new Arrow(tempSource->getAttribute(), "");
	      newArrow->setZValue(2);
	      newArrow->setStartItem(tempSource);
	      newArrow->setEndItem(tempTarget);
	      newArrow->setCopy(true);
	      edgeVector.push_back(newArrow);
	      scene->addItem(newArrow);
	    }
	}
    }
  updateLinkages();
}

void OccurrenceGraphWidget::restore() 
{
  reset();
  groupOccurrences();
  wireLinkages();
  setVisibility();
}

void OccurrenceGraphWidget::reset() 
{
  matched = false;
  QVector<OccurrenceItem*>::iterator it;
  for (it = attributeOccurrenceVector.begin(); it != attributeOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getId() < 0) 
	{
	  delete current->getLabel();
	  attributeLabelVector.removeOne(current->getLabel());
	  current->setLabel(NULL);
	  delete current;
	  attributeOccurrenceVector.removeOne(current);
	}
      else 
	{
	  current->setPos(QPointF((current->getOrder() * distance), 0));
	  current->setPermHidden(false); // We reset this here.2
	  QString text = QString::number(current->getOrder()) + " - " + current->getAttribute();
	  QColor textColor = current->getLabel()->defaultTextColor();
	  delete current->getLabel();
	  attributeLabelVector.removeOne(current->getLabel());
	  OccurrenceLabel *newLabel = new OccurrenceLabel(current);
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(textColor);
	  current->show();
	  current->setLabel(newLabel);
	  newLabel->setTextWidth(current->getLabel()->boundingRect().width());
	  current->getLabel()->show();
	  current->getLabel()->setNewPos(current->scenePos());
	  scene->addItem(current->getLabel());
	  attributeLabelVector.push_back(current->getLabel());
	  it++;
	}
    }
  // And now we do the same for the relationship-oriented occurrences
  for (it = relationshipOccurrenceVector.begin(); it != relationshipOccurrenceVector.end();) 
    {
      OccurrenceItem *current = *it;
      if (current->getId() < 0) 
	{
	  delete current->getLabel();
	  relationshipLabelVector.removeOne(current->getLabel());
	  current->setLabel(NULL);
	  delete current;
	  relationshipOccurrenceVector.removeOne(current);
	}
      else 
	{
	  current->setPos(QPointF((current->getOrder() * distance), 0));
	  current->setPermHidden(false); // We reset this here.2
	  QString text = QString::number(current->getOrder()) + " - " + current->getAttribute();
	  QColor textColor = current->getLabel()->defaultTextColor();
	  delete current->getLabel();
	  relationshipLabelVector.removeOne(current->getLabel());
	  OccurrenceLabel *newLabel = new OccurrenceLabel(current);
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(textColor);
	  current->show();
	  current->setLabel(newLabel);
	  newLabel->setTextWidth(current->getLabel()->boundingRect().width());
	  current->getLabel()->show();
	  current->getLabel()->setNewPos(current->scenePos());
	  scene->addItem(current->getLabel());
	  relationshipLabelVector.push_back(current->getLabel());
	  it++;
	}
    }
  setVisibility();
}

void OccurrenceGraphWidget::getEvents() 
{
  reset();
  matched = true;
  QVector<EventItem*> incidents = eventGraph->getEventItems();
  if (incidents.size() > 0) 
    {
      QVectorIterator<EventItem*> it(incidents);
      while (it.hasNext()) 
	{
	  EventItem *incident = it.next();
	  // We first do this for the attributes-oriented occurrences.
	  QVectorIterator<OccurrenceItem*> it2(attributeOccurrenceVector);
	  while (it2.hasNext()) 
	    {
	      OccurrenceItem *occurrence = it2.next();
	      if (incident->getId() == occurrence->getId()) 
		{
		  if (incident->getMacroEvent() != NULL) 
		    {
		      MacroEvent *macro = incident->getMacroEvent();
		      while (macro->getMacroEvent() != NULL) 
			{
			  macro = macro->getMacroEvent();
			}
		      QString type = "";
		      if (macro->getConstraint() == PATHS ||
			  macro->getConstraint() == PATHSATT) 
			{
			  type = "P";
			}
		      else if (macro->getConstraint() == SEMIPATHS ||
			       macro->getConstraint() == SEMIPATHSATT) 
			{
			  type = "S";
			}
		      else if (macro->getConstraint() == NOCONSTRAINT ||
			       macro->getConstraint() == NOCONSTRAINTATT) 
			{
			  type = "N";
			}
		      QString text = type + QString::number(macro->getOrder()) + " - " +
			occurrence->getAttribute();
		      QColor textColor = occurrence->getLabel()->defaultTextColor();
		      delete occurrence->getLabel();
		      attributeLabelVector.removeOne(occurrence->getLabel());
		      OccurrenceLabel *newLabel = new OccurrenceLabel(occurrence);
		      newLabel->setPlainText(text);
		      newLabel->setDefaultTextColor(textColor);
		      occurrence->setLabel(newLabel);
		      newLabel->setTextWidth(occurrence->getLabel()->boundingRect().width());
		      occurrence->show();
		      occurrence->getLabel()->show();
		      occurrence->setPos(macro->scenePos().x(), 0);
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		      scene->addItem(occurrence->getLabel());
		      attributeLabelVector.push_back(occurrence->getLabel());
		    }
		  else 
		    {
		      occurrence->setPos(incident->scenePos().x(), 0);
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		    }
		}
	    }
	  // And then we do the relationships-oriented occurrences.
	  QVectorIterator<OccurrenceItem*> it3(relationshipOccurrenceVector);
	  while (it3.hasNext()) 
	    {
	      OccurrenceItem *occurrence = it3.next();
	      if (incident->getId() == occurrence->getId()) 
		{
		  if (incident->getMacroEvent() != NULL) 
		    {
		      MacroEvent *macro = incident->getMacroEvent();
		      while (macro->getMacroEvent() != NULL) 
			{
			  macro = macro->getMacroEvent();
			}
		      QString type = "";
		      if (macro->getConstraint() == PATHS ||
			  macro->getConstraint() == PATHSATT) 
			{
			  type = "P";
			}
		      else if (macro->getConstraint() == SEMIPATHS ||
			       macro->getConstraint() == SEMIPATHSATT) 
			{
			  type = "S";
			}
		      else if (macro->getConstraint() == NOCONSTRAINT ||
			       macro->getConstraint() == NOCONSTRAINTATT) 
			{
			  type = "N";
			}
		      QString text = type + QString::number(macro->getOrder()) + " - " +
			occurrence->getAttribute();
		      QColor textColor = occurrence->getLabel()->defaultTextColor();
		      delete occurrence->getLabel();
		      relationshipLabelVector.removeOne(occurrence->getLabel());
		      OccurrenceLabel *newLabel = new OccurrenceLabel(occurrence);
		      newLabel->setPlainText(text);
		      newLabel->setDefaultTextColor(textColor);
		      occurrence->setLabel(newLabel);
		      newLabel->setTextWidth(occurrence->getLabel()->boundingRect().width());
		      occurrence->show();
		      occurrence->getLabel()->show();
		      occurrence->setPos(macro->scenePos().x(), 0);
		      occurrence->getLabel()->setNewPos(occurrence->scenePos());
		      scene->addItem(occurrence->getLabel());
		      attributeLabelVector.push_back(occurrence->getLabel());
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
     Then we do the macros for the attribute-oriented occurrences. 
     We don't need to do this for the relationship-oriented occurrences.
  */
  QVector<MacroEvent*> macros = eventGraph->getMacros();
  if (macros.size() > 0) 
    {
      QVectorIterator<MacroEvent*> it(macros);
      while (it.hasNext()) 
	{
	  MacroEvent *macro = it.next();
	  QSet<QString> attributes = macro->getAttributes();
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
		  while (macro->getMacroEvent() != NULL) 
		    {
		      macro = macro->getMacroEvent();
		    }
		  QPointF position = QPointF(macro->scenePos().x(), 0);
		  // I am setting macro id's to negatives to distinguish them from the incident ids.
		  OccurrenceItem *newOccurrence = new OccurrenceItem(40, macro->getDescription(),
								     position, (macro->getId() * -1),
								     macro->getOrder(),
								     currentAttribute);
		  newOccurrence->setPos(newOccurrence->getOriginalPos());
		  newOccurrence->setColor(color);
		  newOccurrence->setZValue(3);
		  attributeOccurrenceVector.push_back(newOccurrence);
		  scene->addItem(newOccurrence);
		  OccurrenceLabel *label = new OccurrenceLabel(newOccurrence);
		  QString type = "";
		  if (macro->getConstraint() == PATHS ||
		      macro->getConstraint() == PATHSATT) 
		    {
		      type = "P";
		    }
		  else if (macro->getConstraint() == SEMIPATHS ||
			   macro->getConstraint() == SEMIPATHSATT) 
		    {
		      type = "S"; 
		    }
		  else if (macro->getConstraint() == NOCONSTRAINT ||
			   macro->getConstraint() == NOCONSTRAINTATT) 
		    {
		      type = "N";
		    }
		  QString text = type + QString::number(macro->getOrder()) + " - " + currentAttribute;
		  label->setPlainText(text);
		  label->setDefaultTextColor(Qt::black);
		  label->setTextWidth(label->boundingRect().width());
		  label->setNewPos(newOccurrence->scenePos());
		  attributeLabelVector.push_back(label);
		  newOccurrence->setLabel(label);
		  label->setZValue(4);
		  scene->addItem(label);
		}
	    }
	}
    }
  groupOccurrences();
  wireLinkages();
}

void OccurrenceGraphWidget::processMoveItems(QGraphicsItem *item, QPointF pos) 
{
  OccurrenceItem *source = NULL;
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
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
      qreal currentY = source->scenePos().y();
      qreal newY = pos.y();
      qreal yDiff = newY - currentY;
      it3.toFront();
      while (it3.hasNext()) 
	{
	  OccurrenceItem *current = it3.next();
	  if (current->scenePos().x() == source->scenePos().x()) 
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
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
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
      qreal currentY = source->scenePos().y();
      qreal newY = pos.y();
      qreal yDiff = newY - currentY;
      if (!source->isGrouped()) 
	{
	  it3.toFront();
	  while (it3.hasNext()) 
	    {
	      OccurrenceItem *current = it3.next();
	      if (current->getAttribute() == source->getAttribute() &&
		  !current->isGrouped()) 
		{
		  current->setPos(current->scenePos().x(), current->scenePos().y() + yDiff);
		  current->getLabel()->setNewPos(current->scenePos());
		}
	    }
	}
    }
}

void OccurrenceGraphWidget::processOccurrenceGraphContextMenu(const QString &action,
							      const QPoint &pos) 
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

void OccurrenceGraphWidget::addLineObject(bool arrow1, bool arrow2, const QPointF &pos) 
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

void OccurrenceGraphWidget::addTextObject(const QPointF &pos) 
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

void OccurrenceGraphWidget::addEllipseObject(const QPointF &pos) 
{
  EllipseObject *newEllipse = new EllipseObject();
  ellipseVector.push_back(newEllipse);
  scene->addItem(newEllipse);
  newEllipse->setZValue(5);
  newEllipse->setPos(pos);
  newEllipse->moveCenter(newEllipse->mapFromScene(pos));
}

void OccurrenceGraphWidget::addRectObject(const QPointF &pos) 
{
  RectObject *newRect = new RectObject();
  rectVector.push_back(newRect);
  scene->addItem(newRect);
  newRect->setZValue(5);
  newRect->setPos(pos);
  newRect->moveCenter(newRect->mapFromScene(pos));
}

void OccurrenceGraphWidget::processLineContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::changeLineColor() 
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
	  lineVector.removeOne(line);
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

void OccurrenceGraphWidget::processTextContextMenu(const QString &action) 
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
	}
    }
}

void OccurrenceGraphWidget::changeTextColor() 
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

void OccurrenceGraphWidget::deleteText() 
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

void OccurrenceGraphWidget::processEllipseContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::changeEllipseColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());    
      if (ellipse) 
	{
	  QColor currentColor = ellipse->getColor();
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

void OccurrenceGraphWidget::changeEllipseFillColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      EllipseObject *ellipse = qgraphicsitem_cast<EllipseObject*>(scene->selectedItems().first());
      if (ellipse) 
	{
	  QColor currentColor = ellipse->getFillColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  colorDialog->setCurrentColor(currentColor);
	  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      ellipse->setFillColor(color);
	    }
	  delete colorDialog;
	}
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
	  ellipseVector.removeOne(ellipse);
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

void OccurrenceGraphWidget::processRectContextMenu(const QString &action) 
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

void OccurrenceGraphWidget::changeRectColor() 
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

void OccurrenceGraphWidget::changeRectFillColor() 
{
  if (scene->selectedItems().size() == 1) 
    {
      RectObject *rect = qgraphicsitem_cast<RectObject*>(scene->selectedItems().first());
      if (rect) 
	{
	  QColor currentColor = rect->getFillColor();
	  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
	  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
	  colorDialog->setCurrentColor(currentColor);
	  colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);
	  if (colorDialog->exec()) 
	    {
	      QColor color = colorDialog->selectedColor();
	      rect->setFillColor(color);
	    }
	  delete colorDialog;
	}
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
	  rectVector.removeOne(rect);
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

void OccurrenceGraphWidget::objectOneBackward() 
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

void OccurrenceGraphWidget::objectToBack() 
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
  setChangeLabel();  
}

void OccurrenceGraphWidget::plotLabels() 
{
  QVectorIterator<OccurrenceLabel*> it(attributeLabelVector);
  while (it.hasNext()) 
    {
      OccurrenceLabel *currentItem = it.next();
      OccurrenceItem *currentOccurrence = currentItem->getOccurrence();
      if (currentOccurrence->isVisible()) 
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
  QVectorIterator<OccurrenceLabel*> it2(relationshipLabelVector);
  while (it2.hasNext()) 
    {
      OccurrenceLabel *currentItem = it2.next();
      OccurrenceItem *currentOccurrence = currentItem->getOccurrence();
      if (currentOccurrence->isVisible()) 
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

void OccurrenceGraphWidget::changeLabels() 
{
  if (changeLabelsButton->isChecked()) 
    {
      QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder());
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setTextWidth(newLabel->boundingRect().width());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  attributeLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(4);
	  scene->addItem(newLabel);
	  delete oldLabel;
	  attributeLabelVector.removeOne(oldLabel);
	}
      QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
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
	  relationshipLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(4);
	  scene->addItem(newLabel);
	  delete oldLabel;
	  relationshipLabelVector.removeOne(oldLabel);
	}
    }
  else 
    {
      QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
      while (it.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder()) + " - " +
	    currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setTextWidth(newLabel->boundingRect().width());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  attributeLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(4);
	  scene->addItem(newLabel);
	  delete oldLabel;
	  attributeLabelVector.removeOne(oldLabel);
	}
      QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
      while (it2.hasNext()) 
	{
	  OccurrenceItem *currentOccurrence = it2.next();
	  OccurrenceLabel *oldLabel = currentOccurrence->getLabel();
	  OccurrenceLabel *newLabel = new OccurrenceLabel(currentOccurrence);
	  QString text = QString::number(currentOccurrence->getOrder()) + " - " +
	    currentOccurrence->getAttribute();
	  newLabel->setPlainText(text);
	  newLabel->setDefaultTextColor(oldLabel->defaultTextColor());
	  newLabel->setTextWidth(newLabel->boundingRect().width());
	  newLabel->setNewPos(currentOccurrence->scenePos());
	  relationshipLabelVector.push_back(newLabel);
	  currentOccurrence->setLabel(newLabel);
	  newLabel->setZValue(4);
	  scene->addItem(newLabel);
	  delete oldLabel;
	  relationshipLabelVector.removeOne(oldLabel);
	}
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
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  int unit = 0;
  qreal last = -9999;
  if (distance < 300) 
    {
      QVectorIterator<OccurrenceItem*> it3(allOccurrences);
      while (it3.hasNext()) 
	{
	  OccurrenceItem *current = it3.next();
	  qreal oldX = current->scenePos().x();
	  if (last == -9999) 
	    {
	      last = current->scenePos().x();
	    }
	  if (last != oldX) 
	    {
	      unit++;
	    }
	  QPointF newPos = QPointF(oldX + unit, current->pos().y());
	  current->getLabel()->setNewPos(newPos);
	  current->setOriginalPos(newPos);
	  current->setPos(newPos);
	  last = oldX;
	}
    }
  distance++;
}

void OccurrenceGraphWidget::decreaseDistance() 
{
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
    }
  std::sort(allOccurrences.begin(), allOccurrences.end(), eventLessThan);
  int unit = 0;
  qreal last = -9999;
  if (distance > 40) 
    {
      QVectorIterator<OccurrenceItem*> it3(allOccurrences);
      while (it3.hasNext()) 
	{
	  OccurrenceItem *current = it3.next();
	  qreal oldX = current->scenePos().x();
	  if (last == -9999) 
	    {
	      last = current->scenePos().x();
	    }
	  if (last != oldX) 
	    {
	      unit++;
	    }      
	  QPointF newPos = QPointF(oldX - unit, current->pos().y());
	  current->getLabel()->setNewPos(newPos);
	  current->setOriginalPos(newPos);
	  current->setPos(newPos);
	  last = oldX;
	}
    }
  distance--;
}

void OccurrenceGraphWidget::setRangeControls() 
{
  lowerRangeDial->setEnabled(true);
  upperRangeDial->setEnabled(true);
  lowerRangeSpinBox->setEnabled(true);
  upperRangeSpinBox->setEnabled(true);
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
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
  upperRangeDial->setValue(maxOrder);
  upperRangeSpinBox->setValue(maxOrder);
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
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
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
	    }  else 
	    {
	      currentItem->hide();
	    }
	}
      if (checkedCases.size() > 0) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT incident FROM incidents_to_cases "
			 "WHERE incident = :incident AND casename = :casename");
	  int id = currentItem->getId();
	  if (id < 0) 
	    { // Occurrence items for macro events have negative IDs.
	      bool keep = false;
	      QVector<MacroEvent*> macros = eventGraph->getMacros();
	      QVectorIterator<MacroEvent*> it4(macros);
	      while (it4.hasNext()) 
		{
		  MacroEvent *currentMacro = it4.next();
		  if (currentMacro->getId() == id * -1) 
		    {
		      QVector<EventItem*> contents = currentMacro->getIncidents();
		      QVectorIterator<EventItem*> it5(contents);
		      while (it5.hasNext()) 
			{
			  EventItem *currentIncident = it5.next();
			  QVectorIterator<QString> it6(checkedCases);
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
	      QVectorIterator<QString> it4(checkedCases);
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
		  if (matched) 
		    {
		      QVector<EventItem*> events = eventGraph->getEventItems();
		      QVectorIterator<EventItem*> it5(events);
		      while (it5.hasNext()) 
			{
			  EventItem *currentEvent = it5.next();
			  if (currentEvent->getId() == currentItem->getId()) 
			    {
			      if (currentEvent->getMacroEvent() != NULL) 
				{
				  QVector<EventItem*> contents = currentEvent->getMacroEvent()->getIncidents();
				  QVectorIterator<EventItem*> it6(contents);
				  while (it6.hasNext()) 
				    {
				      EventItem *currentIncident = it6.next();
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
  QVectorIterator<Arrow*> it4(edgeVector);
  while (it4.hasNext()) 
    {
      Arrow *currentEdge = it4.next();
      OccurrenceItem *tail = qgraphicsitem_cast<OccurrenceItem*>(currentEdge->startItem());
      OccurrenceItem *head = qgraphicsitem_cast<OccurrenceItem*>(currentEdge->endItem());
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
  QVectorIterator<OccurrenceLabel*> it5(attributeLabelVector);
  while (it5.hasNext()) 
    {
      OccurrenceLabel *currentText = it5.next();
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
  QVectorIterator<OccurrenceLabel*> it6(relationshipLabelVector);
  while (it6.hasNext()) 
    {
      OccurrenceLabel *currentText = it6.next();
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
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
}

void OccurrenceGraphWidget::exportSvg() 
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

void OccurrenceGraphWidget::exportMatrix() 
{
  // First we put all occurrences together
  QVector<OccurrenceItem*> allOccurrences;
  QVectorIterator<OccurrenceItem*> it(attributeOccurrenceVector);
  while (it.hasNext()) 
    {
      allOccurrences.push_back(it.next());
    }
  QVectorIterator<OccurrenceItem*> it2(relationshipOccurrenceVector);
  while (it2.hasNext()) 
    {
      allOccurrences.push_back(it2.next());
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
			 "SET red = :red, green = :green, blue = :blue "
			 "WHERE plot = :plot");
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
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
	}
      else 
	{
	  // Insert new data into saved_og_plots and then write data.
	  QColor color = scene->backgroundBrush().color();
	  int red = color.red();
	  int green = color.green();
	  int blue = color.blue();
	  query->prepare("INSERT INTO saved_og_plots (plot, red, green, blue) "
			 "VALUES (:name, :red, :green, :blue)");
	  query->bindValue(":name", name);
	  query->bindValue(":red", red);
	  query->bindValue(":green", green);
	  query->bindValue(":blue", blue);
	  query->exec();
	}
      QVector<OccurrenceItem*> allOccurrences;
      QVectorIterator<OccurrenceItem*> aIt(attributeOccurrenceVector);
      while (aIt.hasNext()) 
	{
	  allOccurrences.push_back(aIt.next());
	}
      QVectorIterator<OccurrenceItem*> rIt(relationshipOccurrenceVector);
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
		     "oriypos, red, green, blue, alpha, hidden, perm, relationship) "
		     "VALUES (:plot, :incident, :order, :attribute, :width, :curxpos, :curypos, "
		     ":orixpos, :oriypos, :red, :green, :blue, :alpha, :hidden, :perm, "
		     ":relationship)");
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
	  int hidden = 1;
	  int perm = 0;
	  int relationship = 0;
	  if (currentItem->isVisible()) 
	    {
	      hidden = 0;
	    }
	  if (currentItem->isPermHidden()) 
	    {
	      perm = 1;
	    }
	  if (relationshipOccurrenceVector.contains(currentItem)) 
	    {
	      relationship = 1;
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
	  query->bindValue(":hidden", hidden);
	  query->bindValue(":perm", perm);
	  query->bindValue(":relationship", relationship);
	  query->exec();
	  counter++;
	  saveProgress->setProgress(counter);
	  qApp->processEvents();
	}
      saveProgress->close();
      delete saveProgress;
      QVector<OccurrenceLabel*> allLabels;
      QVectorIterator<OccurrenceLabel*> alIt(attributeLabelVector);
      while (alIt.hasNext()) 
	{
	  allLabels.push_back(alIt.next());
	}
      QVectorIterator<OccurrenceLabel*> rlIt(relationshipLabelVector);
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
		     "red, green, blue, alpha, hidden, relationship) "
		     "VALUES (:plot, :incident, :attribute, :label, :curxpos, :curypos, "
		     ":xoffset, :yoffset, :red, :green, :blue, :alpha, :hidden, :relationship)");
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
	  int hidden = 1;
	  int relationship = 0;
	  if (currentLabel->isVisible()) 
	    {
	      hidden = 0;
	    }
	  if (relationshipLabelVector.contains(currentLabel)) 
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
      saveProgress = new ProgressBar(0, 1, lineVector.size());
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
      QVectorIterator<LineObject*> it8(lineVector);
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
      saveProgress = new ProgressBar(0, 1, textVector.size());
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
      query->prepare("INSERT INTO saved_og_plots_ellipses "
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
      query->prepare("INSERT INTO saved_og_plots_rects "
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
      plotLabel->setText(name);
      changeLabel->setText("");
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
      savePlotButton->setEnabled(true);
      cleanUp();
      QString plot = savedPlotsDialog->getSelectedPlot();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT red, green, blue "
		     "FROM saved_og_plots "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      query->first();
      int red = query->value(0).toInt();
      int green = query->value(1).toInt();
      int blue = query->value(2).toInt();
      scene->setBackgroundBrush(QBrush(QColor(red, green, blue)));
      query->prepare("SELECT incident, ch_order, attribute, width, curxpos, curypos, orixpos, "
		     "oriypos, red, green, blue, alpha, hidden, perm, relationship "
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
	  int hidden = query->value(12).toInt();
	  int perm = query->value(13).toInt();
	  int relationship = query->value(14).toInt();
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
	  currentItem->setZValue(3);
	  if (relationship == 0) 
	    {
	      attributeOccurrenceVector.push_back(currentItem);
	    }
	  else 
	    {
	      relationshipOccurrenceVector.push_back(currentItem);
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
	}
      query->prepare("SELECT incident, attribute, label, curxpos, curypos, xoffset, yoffset, "
		     "red, green, blue, alpha, hidden, relationship "
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
	  int hidden = query->value(11).toInt();
	  int relationship = query->value(12).toInt();
	  QVectorIterator<OccurrenceItem*> it(allOccurrences);
	  while (it.hasNext()) 
	    {
	      OccurrenceItem *currentItem = it.next();
	      if (currentItem->getId() == id && currentItem->getAttribute() == attribute) 
		{
		  OccurrenceLabel *currentLabel = new OccurrenceLabel(currentItem);
		  currentLabel->setPlainText(text);
		  currentLabel->setTextWidth(currentLabel->boundingRect().width());
		  currentLabel->setPos(QPointF(currentX, currentY));
		  currentLabel->setOffset(QPointF(xOffset, yOffset));
		  currentLabel->setDefaultTextColor(QColor(red, green, blue, alpha));
		  currentLabel->setZValue(4);
		  currentItem->setLabel(currentLabel);
		  if (relationship == 0) 
		    {
		      attributeLabelVector.push_back(currentLabel);
		    }
		  else 
		    {
		      relationshipLabelVector.push_back(currentLabel);
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
	      presentAttributes.push_back(name);
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
	      presentRelationships.push_back(name);
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
      groupOccurrences();
      wireLinkages();
      setRangeControls();
      setVisibility();
      checkCongruency();
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
      // saved_og_plots_occurrence_items
      query->prepare("DELETE FROM saved_og_plots_occurrence_items "
		     "WHERE plot = :plot");
      query->bindValue(":plot", plot);
      query->exec();
      // saved_eg_plots_occurrence_labels
      query->prepare("DELETE FROM saved_eg_plots_occurrence_labels "
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
      delete query;
      seePlots();
    }
}


void OccurrenceGraphWidget::setEventGraph(EventGraphWidget *egw) 
{
  eventGraph = egw;
}

void OccurrenceGraphWidget::cleanUp() 
{
  scene->clearSelection();
  qDeleteAll(attributeOccurrenceVector);
  attributeOccurrenceVector.clear();
  qDeleteAll(relationshipOccurrenceVector);
  relationshipOccurrenceVector.clear();
  qDeleteAll(attributeLabelVector);
  attributeLabelVector.clear();
  qDeleteAll(relationshipLabelVector);
  relationshipLabelVector.clear();
  qDeleteAll(edgeVector);
  edgeVector.clear();
  qDeleteAll(lineVector);
  lineVector.clear();
  qDeleteAll(textVector);
  textVector.clear();
  qDeleteAll(ellipseVector);
  ellipseVector.clear();
  qDeleteAll(rectVector);
  rectVector.clear();
  attributeListWidget->setRowCount(0);
  relationshipListWidget->setRowCount(0);
  presentAttributes.clear();
  presentRelationships.clear();
  checkedCases.clear();
}

void OccurrenceGraphWidget::finalBusiness() 
{
  cleanUp();
}
