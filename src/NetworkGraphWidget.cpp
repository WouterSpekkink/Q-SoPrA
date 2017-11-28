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
  
  typeLabel = new QLabel(tr("Select type"), this);
  upperRangeLabel = new QLabel(tr("<b>Upper bound:</b>"), graphicsWidget);
  lowerRangeLabel = new QLabel(tr("<b>Lower bound:</b>"), graphicsWidget);
  
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
  nodeColorButton = new QPushButton(tr("Set node color"), graphicsWidget);
  labelColorButton = new QPushButton(tr("Set label color"), graphicsWidget);
  backgroundColorButton = new QPushButton(tr("Change background"), graphicsWidget);

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
  
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(toggleGraphicsControlsButton, SIGNAL(clicked()), this, SLOT(toggleGraphicsControls()));
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotNewGraph()));
  connect(addButton, SIGNAL(clicked()), this, SLOT(addRelationshipType()));
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

  QPointer<QVBoxLayout> graphicsControlsLayout = new QVBoxLayout;
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
  //  graphicsControlsLayout->addWidget(exportSvgButton);
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
  drawOptionsRightLayout->addWidget(toggleGraphicsControlsButton);
  drawOptionsLayout->addLayout(drawOptionsRightLayout);
  drawOptionsRightLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(drawOptionsLayout);

  setLayout(mainLayout);

  getTypes();

  graphicsWidget->hide();
}

void NetworkGraphWidget::toggleGraphicsControls() {
  if (graphicsWidget->isHidden()) {
    graphicsWidget->show();
  } else {
    graphicsWidget->hide();
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
    label->setPlainText(name);
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
    currentNode->setColor(Qt::red);
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
      query3->prepare("SELECT name, source, target, comment "
		      "FROM entity_relationships "
		      "WHERE type = :type AND name = :name");
      query3->bindValue(":type", type);
      query3->bindValue(":name", relationship);
      query3->exec();
      while (query3->next()) {
	QString name  = query3->value(0).toString();
	QString source = query3->value(1).toString();
	QString target = query3->value(2).toString();
      
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
      delete query3;
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
      query3->prepare("SELECT name, source, target, comment "
		      "FROM entity_relationships "
		      "WHERE type = :type AND name = :name");
      query3->bindValue(":type", type);
      query3->bindValue(":name", relationship);
      query3->exec();
      while (query3->next()) {
	QString name = query3->value(0).toString();
	QString source = query3->value(1).toString();
	QString target = query3->value(2).toString();
      
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
	    UndirectedEdge *currentEdge = new UndirectedEdge(tempSource, tempTarget, type, name);
	    currentEdge->hide();
	    undirectedVector.push_back(currentEdge);
	    break;
	  }
	}
      }
      delete query3;
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
	  int newHeight = current->getHeight() + 60;
	  currentEdge->setHeight(newHeight);
	} else if (undirected) {
	  UndirectedEdge *current = qgraphicsitem_cast<UndirectedEdge*>(it2.next());
	  int newHeight = current->getHeight() + 60;
	  currentEdge->setHeight(newHeight);
	} else {
	  it2.next();
	}
      }
      currentEdge->setColor(color);
      currentEdge->show();
      currentEdge->startItem()->show();
      currentEdge->endItem()->show();
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
	  int newHeight = current->getHeight() + 60;
	  currentEdge->setHeight(newHeight);
	} else if (undirected) {
	  UndirectedEdge *current = qgraphicsitem_cast<UndirectedEdge*>(it2.next());
	  int newHeight = current->getHeight() + 60;
	  currentEdge->setHeight(newHeight);
	} else {
	  it2.next();
	}
      }
      currentEdge->setColor(color);
      currentEdge->show();
      currentEdge->startItem()->show();
      currentEdge->endItem()->show();
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
    currentNode->setColor(Qt::red);
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
	currentTarget->setPos(targetPoint);
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
}

void NetworkGraphWidget::addRelationshipType() {
  selectedType = typeComboBox->currentText();
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  QColor color = QColor(Qt::black);
  if (colorDialog->exec()) {
    color = colorDialog->selectedColor();
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
}

void NetworkGraphWidget::toggleLabels() {
  labelsShown = !labelsShown;
  QVectorIterator<NetworkNodeLabel*> it(labelVector);
  while (it.hasNext()) {
    NetworkNodeLabel *currentLabel = it.next();
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

void NetworkGraphWidget::processLowerRange(int value) {
  lowerRangeDial->setValue(value);
  lowerRangeSpinBox->setValue(value);
  if (upperRangeDial->value() <= value) {
    upperRangeDial->setValue(value + 1);
    upperRangeSpinBox->setValue(value + 1);
  }
  QVectorIterator<NetworkNode*> it(nodeVector);
  while (it.hasNext()) {
    NetworkNode* currentNode = it.next();
    currentNode->hide();
  }
  QVectorIterator<DirectedEdge*> it2(directedVector);
  while (it.hasNext()) {
    bool show = false;
    DirectedEdge *currentDirected = it2.next();
    QString relationship = currentDirected->getName();
    QString type = currentDirected->getType();
    if (presentTypes.contains(type)) {
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

void NetworkGraphWidget::processUpperRange(int value) {
  upperRangeDial->setValue(value);
  upperRangeSpinBox->setValue(value);
  if (lowerRangeDial->value() >= value) {
    lowerRangeDial->setValue(value - 1);
    lowerRangeSpinBox->setValue(value - 1);
  }
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
  selectedType = "";
  presentTypes.clear();
  minOrder = 0;
  maxOrder = 0;
}

