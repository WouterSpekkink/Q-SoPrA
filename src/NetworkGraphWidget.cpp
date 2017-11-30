#include "../include/NetworkGraphWidget.h"

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) : QWidget(parent) {
  selectedType = "";
  labelsShown = false;

  minOrder = 0;
  maxOrder = 0;
  
  scene = new Scene(this);
  view = new GraphicsView(scene);
  view->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.

  graphicsWidget = new QWidget(this);
  legendWidget = new QWidget(this);
  
  typeLabel = new QLabel(tr("Select type"), this);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  nodeLegendLabel = new QLabel(tr("<b>Modes:</b>"), legendWidget);
  edgeLegendLabel = new QLabel(tr("<b>Edge legend:</b>"), legendWidget);
  
  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);
  
  plotButton = new QPushButton(tr("Plot new"), this);
  plotButton->setEnabled(false);
  toggleGraphicsControlsButton = new QPushButton(tr("Toggle controls"), this);
  toggleGraphicsControlsButton->setCheckable(true);
  addButton = new QPushButton(tr("Add relationship type"), this);
  addButton->setEnabled(false);
  toggleLabelsButton = new QPushButton(tr("Toggle labels"), graphicsWidget);
  toggleLabelsButton->setCheckable(true);
  increaseFontSizeButton = new QPushButton(tr("+"));
  decreaseFontSizeButton = new QPushButton(tr("-"));
  colorByAttributeButton = new QPushButton(tr("Create mode"), graphicsWidget);
  nodeColorButton = new QPushButton(tr("Set node color"), graphicsWidget);
  labelColorButton = new QPushButton(tr("Set label color"), graphicsWidget);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);
  toggleLegendButton = new QPushButton(tr("Toggle legend"), this);
  exportSvgButton = new QPushButton(tr("Export"), graphicsWidget);
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
  
  edgeListWidget->setColumnCount(2);
  edgeListWidget->horizontalHeader()->hide();
  edgeListWidget->verticalHeader()->hide();
  edgeListWidget->setColumnWidth(1, 20);
  edgeListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  edgeListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  edgeListWidget->setStyleSheet("QTableView {gridline-color: black}");
  
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(toggleLegendButton, SIGNAL(clicked()), this, SLOT(toggleLegend()));
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotNewGraph()));
  connect(addButton, SIGNAL(clicked()), this, SLOT(addRelationshipType()));
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
  connect(edgeListWidget, SIGNAL(itemClicked(QTableWidgetItem *)),
	  this, SLOT(setFilterButtons(QTableWidgetItem *)));
  connect(edgeListWidget, SIGNAL(noneSelected()),
	  this, SLOT(disableFilterButtons()));
  connect(setFilteredButton, SIGNAL(clicked()), this, SLOT(activateFilter()));
  connect(unsetFilteredButton, SIGNAL(clicked()), this, SLOT(deactivateFilter()));
  connect(hideTypeButton, SIGNAL(clicked()), this, SLOT(hideType()));
  connect(showTypeButton, SIGNAL(clicked()), this, SLOT(showType()));
  connect(exportSvgButton, SIGNAL(clicked()), this, SLOT(exportSvg()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(typeLabel);
  plotOptionsLayout->addWidget(typeComboBox);
  plotOptionsLayout->addWidget(plotButton);
  plotOptionsLayout->addWidget(addButton);
  topLayout->addLayout(plotOptionsLayout);
  plotOptionsLayout->setAlignment(Qt::AlignLeft);
  mainLayout->addLayout(topLayout);

  QPointer<QHBoxLayout> screenLayout = new QHBoxLayout;

  QPointer<QVBoxLayout> middleLayout = new QVBoxLayout;
  middleLayout->addWidget(view);
  view->sizePolicy().setHorizontalPolicy(QSizePolicy::Maximum);
  screenLayout->addLayout(middleLayout);

  QPointer<QVBoxLayout> legendLayout = new QVBoxLayout;
  legendLayout->addWidget(nodeLegendLabel);
  legendLayout->addWidget(nodeListWidget);
  legendLayout->addWidget(multimodeButton);
  legendLayout->addWidget(edgeLegendLabel);
  legendLayout->addWidget(edgeListWidget);
  legendLayout->addWidget(setFilteredButton);
  legendLayout->addWidget(unsetFilteredButton);
  QPointer<QFrame> sepLine = new QFrame();
  sepLine->setFrameShape(QFrame::HLine);
  legendLayout->addWidget(sepLine);
  legendLayout->addWidget(hideTypeButton);
  legendLayout->addWidget(showTypeButton);
  legendWidget->setMinimumWidth(175);
  legendWidget->setMaximumWidth(175);
  legendWidget->setLayout(legendLayout);
  screenLayout->addWidget(legendWidget);				   
  
  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
  graphicsControlsLayout->addWidget(colorByAttributeButton);
  graphicsControlsLayout->addWidget(nodeColorButton);
  graphicsControlsLayout->addWidget(labelColorButton);
  graphicsControlsLayout->addWidget(backgroundColorButton);
  graphicsControlsLayout->addWidget(toggleLabelsButton);
  QPointer<QHBoxLayout> fontSizeLayout = new QHBoxLayout;
  fontSizeLayout->addWidget(increaseFontSizeButton);
  fontSizeLayout->addWidget(decreaseFontSizeButton);
  graphicsControlsLayout->addLayout(fontSizeLayout);
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
  graphicsControlsLayout->addWidget(exportSvgButton);
  graphicsWidget->setMaximumWidth(175);
  graphicsWidget->setMinimumWidth(175);
  graphicsWidget->setLayout(graphicsControlsLayout);
  graphicsControlsLayout->setAlignment(Qt::AlignBottom);
  screenLayout->addWidget(graphicsWidget);

  mainLayout->addLayout(screenLayout);
  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> drawOptionsLeftLayout = new QHBoxLayout;
  //  drawOptionsLeftLayout->addWidget(toggleDetailsButton);
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

  graphicsWidget->hide();
  legendWidget->hide();
}

void NetworkGraphWidget::toggleGraphicsControls() {
  if (graphicsWidget->isHidden()) {
    graphicsWidget->show();
  } else {
    graphicsWidget->hide();
  }
}

void NetworkGraphWidget::toggleLegend() {
  if (legendWidget->isHidden()) {
    legendWidget->show();
  } else {
    legendWidget->hide();
  }
}

void NetworkGraphWidget::getTypes() {
  typeComboBox->clear();
  typeComboBox->addItem(DEFAULT);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM relationship_types");
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
    currentNode->setColor(Qt::red);
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
      } else if (incident < minOrder) {
	minOrder = order;
      }
      if (maxOrder == 0) {
	maxOrder = order;
      } else if (incident > maxOrder) {
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
      } else if (incident < minOrder) {
	minOrder = order;
      }
      if (maxOrder == 0) {
	maxOrder = order;
      } else if (incident > maxOrder) {
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
      QListIterator<QGraphicsItem*> it2(scene->items());
      while (it2.hasNext()) {
	DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(it2.peekNext());
	UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(it2.peekNext());
	if (directed) {
	  DirectedEdge *current = qgraphicsitem_cast<DirectedEdge*>(it2.next());
	  if (current->startItem() == currentEdge->startItem() &&
	      current->endItem() == currentEdge->endItem()) {
	    int newHeight = current->getHeight() + 60;
	    currentEdge->setHeight(newHeight);
	  }
	} else if (undirected) {
	  UndirectedEdge *current = qgraphicsitem_cast<UndirectedEdge*>(it2.next());
	  if (current->startItem() == currentEdge->startItem() &&
	      current->endItem() == currentEdge->endItem()) {
	    int newHeight = current->getHeight() + 60;
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
	    int newHeight = current->getHeight() + 60;
	    currentEdge->setHeight(newHeight);
	  }
	} else if (undirected) {
	  UndirectedEdge *current = qgraphicsitem_cast<UndirectedEdge*>(it2.next());
	  if (current->startItem() == currentEdge->startItem() &&
	      current->endItem() == currentEdge->endItem()) {
	    int newHeight = current->getHeight() + 60;
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

  for(int i = 0; i != 2; i++) {
    QListIterator<QGraphicsItem*> it(scene->items());
    while (it.hasNext()) {
      DirectedEdge *directed = qgraphicsitem_cast<DirectedEdge*>(it.peekNext());
      UndirectedEdge *undirected = qgraphicsitem_cast<UndirectedEdge*>(it.peekNext());
      if (directed) {
	DirectedEdge *currentEdge = qgraphicsitem_cast<DirectedEdge*>(it.next());
	NetworkNode *currentSource = currentEdge->startItem();
	NetworkNode *currentTarget = currentEdge->endItem();
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
      } else if (undirected) {
	UndirectedEdge *currentEdge = qgraphicsitem_cast<UndirectedEdge*>(it.next());
	NetworkNode *currentSource = currentEdge->startItem();
	NetworkNode *currentTarget = currentEdge->endItem();
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
      } else {
	it.next();
      }
    }
  }
}

void NetworkGraphWidget::colorByAttribute() {
  QPointer<AttributeColorDialog> attributeColorDialog = new AttributeColorDialog(this, ENTITY);
  attributeColorDialog->exec();
  if (attributeColorDialog->getExitStatus() == 0) {
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
	QString toolTip = "<FONT SIZE = 3>" + attribute + " - " + description + "</FONT SIZE>";
	item->setToolTip(toolTip);
	nodeListWidget->item(i, 1)->setBackground(color);
	break;
      }
    }
    if (!found) {
      QTableWidgetItem *item = new QTableWidgetItem(attribute);
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
      QString toolTip = "<FONT SIZE = 3>" + attribute + " - " + description + "</FONT SIZE>";
      item->setToolTip(toolTip);
      item->setData(Qt::DisplayRole, attribute);
      nodeListWidget->setRowCount(nodeListWidget->rowCount() + 1);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 0, item);
      nodeListWidget->setItem(nodeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
      nodeListWidget->item(nodeListWidget->rowCount() - 1, 1)->setBackground(color);
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
    QString relationship = multimodeDialog->getRelationship();
    QString name = multimodeDialog->getName();
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
      if (first->getType() == relationship) {
	if (first->startItem()->getMode() == modeOne &&
	    first->endItem()->getMode() == modeTwo) {
	  QVectorIterator<DirectedEdge*> it2(directedVector);
	  while (it2.hasNext()) {
	    DirectedEdge *second = it2.next();
	    if (second->getType() == relationship) {
	      if (second->endItem() == first->endItem() &&
		  second->startItem() != first->startItem() &&
		  second->startItem()->getMode() == modeOne &&
		  second->endItem()->getMode() == modeTwo &&
		  first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		UndirectedEdge *newEdge;
		if (first->startItem()->getName() < second->startItem()->getName()) {
		   newEdge = new UndirectedEdge(first->startItem(),
						second->startItem(),
						name, "MM");
		} else {
		  newEdge = new UndirectedEdge(second->startItem(),
					       first->startItem(),
					       name, "MM");
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
	} else if (first->startItem()->getMode() == modeTwo &&
		    first->endItem()->getMode() == modeOne) {
	  QVectorIterator<DirectedEdge*> it2(directedVector);
	  while (it2.hasNext()) {
	    DirectedEdge *second = it2.next();
	    if (second->getType() == relationship) {
	      if (second->startItem() == first->startItem() &&
		  second->endItem() != first->endItem() &&
		  second->endItem()->getMode() == modeOne &&
		  second->startItem()->getMode() == modeTwo &&
		  first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		UndirectedEdge *newEdge;
		if (first->endItem()->getName() < second->endItem()->getName()) {
		  newEdge = new UndirectedEdge(first->endItem(),
					       second->endItem(),
					       name, "MM");
		} else {
		  newEdge = new UndirectedEdge(second->endItem(),
					       first->endItem(),
					       name, "MM");
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
    QVectorIterator<UndirectedEdge*> it3(undirectedVector);
    while (it3.hasNext()) {
      UndirectedEdge *first = it3.next();
      if (first->getType() == relationship) {
	if (first->startItem()->getMode() == modeOne &&
	    first->endItem()->getMode() == modeTwo) {
	  QVectorIterator<UndirectedEdge*> it4(undirectedVector);
	  while (it4.hasNext()) {
	    UndirectedEdge *second = it4.next();
	    if (second->getType() == relationship) {
	      if (second->endItem() == first->endItem() &&
		  second->startItem() != first->startItem() &&
		  second->startItem()->getMode() == modeOne &&
		  second->endItem()->getMode() == modeTwo &&
		  first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		UndirectedEdge *newEdge;
		if (first->startItem()->getName() < second->startItem()->getName()) {
		  newEdge = new UndirectedEdge(first->startItem(),
					       second->startItem(),
					       name, "MM");
		} else {
		  newEdge = new UndirectedEdge(second->startItem(),
					       first->startItem(),
					       name, "MM");
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
	} else if (first->startItem()->getMode() == modeTwo &&
		   first->endItem()->getMode() == modeOne) {
	  QVectorIterator<UndirectedEdge*> it4(undirectedVector);
	  while (it4.hasNext()) {
	    UndirectedEdge *second = it4.next();
	    if (second->getType() == relationship) {
	      if (second->endItem() == first->endItem() &&
		  second->startItem() != first->startItem() &&
		  second->endItem()->getMode() == modeOne &&
		  second->startItem()->getMode() == modeTwo &&
		first->startItem()->isVisible() &&
		  first->endItem()->isVisible() &&
		  second->startItem()->isVisible() &&
		  second->endItem()->isVisible()) {
		UndirectedEdge *newEdge;
		if (first->endItem()->getName() < second->endItem()->getName()) {
		  newEdge = new UndirectedEdge(first->endItem(),
					       second->endItem(),
					       name, "MM");
		} else {
		  newEdge = new UndirectedEdge(second->endItem(),
					       first->endItem(),
					       name, "MM");
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
    presentTypes.push_back(name);
    plotDirectedEdges(name, color);
    plotUndirectedEdges(name, color);
    QTableWidgetItem *item = new QTableWidgetItem(selectedType);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    QString toolTip = "<FONT SIZE = 3>" + name + " (Undirected) - " + description + "</FONT>";
    item->setToolTip(toolTip);
    item->setData(Qt::DisplayRole, name);
    edgeListWidget->setRowCount(edgeListWidget->rowCount() + 1);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 0, item);
    edgeListWidget->setItem(edgeListWidget->rowCount() - 1, 1, new QTableWidgetItem);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->setBackground(color);
    edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->
      setFlags(edgeListWidget->item(edgeListWidget->rowCount() - 1, 1)->flags()
	       ^ Qt::ItemIsEditable ^ Qt::ItemIsSelectable);
  }
}

void NetworkGraphWidget::setFilterButtons(QTableWidgetItem *item) {
  QString text = item->data(Qt::DisplayRole).toString();
  if (text != "") {
    setFilteredButton->setEnabled(true);
    unsetFilteredButton->setEnabled(true);
    hideTypeButton->setEnabled(true);
    showTypeButton->setEnabled(true);
  } else {
    setFilteredButton->setEnabled(false);
    unsetFilteredButton->setEnabled(false);
    hideTypeButton->setEnabled(false);
    showTypeButton->setEnabled(false);
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
  QVectorIterator<NetworkNode*> it3(nodeVector);
  while (it3.hasNext()) {
    NetworkNode* currentNode = it3.next();
    currentNode->hide();
  }
  QVectorIterator<DirectedEdge*> it4(directedVector);
  while (it4.hasNext()) {
    bool show = false;
    DirectedEdge *currentDirected = it4.next();
    QString relationship = currentDirected->getName();
    QString type = currentDirected->getType();
    if (presentTypes.contains(type)) {
      if (currentDirected->isMassHidden()) {
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
  QVectorIterator<UndirectedEdge*> it5(undirectedVector);
  while (it5.hasNext()) {
    bool show = false;
    UndirectedEdge *currentUndirected = it5.next();
    QString relationship = currentUndirected->getName();
    QString type = currentUndirected->getType();
    if (presentTypes.contains(type)) {
      if (currentUndirected->isMassHidden()) {
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
  QVectorIterator<NetworkNodeLabel*> it6(labelVector);
  while (it4.hasNext()) {
    NetworkNodeLabel *label = it6.next();
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
  if (colorDialog->exec()) {
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
  if (colorDialog->exec()) {
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
  if (colorDialog->exec()) {
    QColor color = colorDialog->selectedColor();
    view->setBackgroundBrush(color);
  }
  delete colorDialog;
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
  setRangeControls();

  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness, description FROM relationship_types "
		 "WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  QString directedness = query->value(0).toString();
  QString description = query->value(1).toString();
  QString toolTip = "<FONT SIZE = 3>" + selectedType + " (" + directedness + ") - " +
    description + "</FONT SIZE>";
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
  //  savePlotButton->setEnabled(true);
  //setRangeControls();
  //plotLabel->setText("Unsaved plot");
}


void NetworkGraphWidget::addRelationshipType() {
  selectedType = typeComboBox->currentText();  
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
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
  //  savePlotButton->setEnabled(true);
  //setRangeControls();
  //plotLabel->setText("Unsaved plot");
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness, description FROM relationship_types "
		 "WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  QString directedness = query->value(0).toString();
  QString description = query->value(1).toString();
  QString toolTip = "<FONT SIZE = 3>" + selectedType + " (" + directedness + ") - " + description
    + "</FONT SIZE>";
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
      if (currentDirected->getName() == "MM" && !(currentDirected->isMassHidden())) {
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
      if (currentUndirected->getName() == "MM" && !(currentUndirected->isMassHidden())) {
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

void NetworkGraphWidget::cleanUp() {
  scene->clearSelection();
  qDeleteAll(directedVector);
  directedVector.clear();
  qDeleteAll(undirectedVector);
  undirectedVector.clear();
  qDeleteAll(nodeVector);
  nodeVector.clear();
  qDeleteAll(labelVector);
  labelVector.clear();
  nodeListWidget->setRowCount(0);
  edgeListWidget->setRowCount(0);
  presentTypes.clear();
  minOrder = 0;
  maxOrder = 0;
}
