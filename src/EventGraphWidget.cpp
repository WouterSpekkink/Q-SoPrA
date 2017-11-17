#include "../include/EventGraphWidget.h"

EventGraphWidget::EventGraphWidget(QWidget *parent) : QWidget(parent) {
  selectedCoder = "";
  selectedCompare = "";
  selectedType = "";

  distance = 0;
    
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

  view->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setWindowTitle("Graphics Test");

  coderLabel = new QLabel(tr("Choose coder:"), this);
  typeLabel = new QLabel(tr("Choose linkage:"), this);
  compareLabel = new QLabel(tr("Compare with:"), this);
  
  coderComboBox = new QComboBox(this);
  coderComboBox->addItem(DEFAULT);

  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);

  compareComboBox = new QComboBox(this);
  compareComboBox->addItem(DEFAULT);
  
  plotButton = new QPushButton(tr("Plot new"), this);
  plotButton->setEnabled(false);
  increaseDistanceButton = new QPushButton(tr("< >"), this);
  decreaseDistanceButton = new QPushButton(tr("> <"), this);
  compareButton = new QPushButton(tr("Compare"), this);
  
  connect(coderComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setPlotButton()));
  connect(compareComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setCompareButton()));  
  connect(plotButton, SIGNAL(clicked()), this, SLOT(plotGraph()));
  connect(compareButton, SIGNAL(clicked()), this, SLOT(compare()));
  connect(scene, SIGNAL(widthIncreased(EventItem*)), this, SLOT(increaseWidth(EventItem*)));
  connect(scene, SIGNAL(widthDecreased(EventItem*)), this, SLOT(decreaseWidth(EventItem*)));
  connect(scene, SIGNAL(posIncreased(EventItem*)), this, SLOT(increasePos(EventItem*)));
  connect(scene, SIGNAL(posDecreased(EventItem*)), this, SLOT(decreasePos(EventItem*)));
  connect(increaseDistanceButton, SIGNAL(clicked()), this, SLOT(increaseDistance()));
  connect(decreaseDistanceButton, SIGNAL(clicked()), this, SLOT(decreaseDistance()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(cleanUp()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> plotOptionsLayout = new QHBoxLayout;
  plotOptionsLayout->addWidget(coderLabel);
  plotOptionsLayout->addWidget(coderComboBox);
  plotOptionsLayout->addWidget(typeLabel);
  plotOptionsLayout->addWidget(typeComboBox);
  plotOptionsLayout->addWidget(plotButton);
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
  
  mainLayout->addWidget(view);

  QPointer<QHBoxLayout> drawOptionsLayout = new QHBoxLayout;
  drawOptionsLayout->addWidget(increaseDistanceButton);
  drawOptionsLayout->addWidget(decreaseDistanceButton);
  increaseDistanceButton->setMaximumWidth(increaseDistanceButton->sizeHint().width());
  decreaseDistanceButton->setMaximumWidth(decreaseDistanceButton->sizeHint().width());
  mainLayout->addLayout(drawOptionsLayout);
  drawOptionsLayout->setAlignment(Qt::AlignLeft);
  
  setLayout(mainLayout);
      
}

void EventGraphWidget::getEvents() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT ch_order, description FROM incidents ORDER BY ch_order");
  while (query->next()) {
    int order = query->value(0).toInt();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT id FROM incidents WHERE ch_order = :order");
    query2->bindValue(":order", order);
    query2->exec();
    query2->first();
    int id = query2->value(0).toInt();
    QString toolTip = "<FONT SIZE = 3>" + query->value(1).toString() + "</FONT>";
    int vertical = qrand() % 240 - 120;
    QPointF position = QPointF((order * distance), vertical);
    EventItem *currentItem = new EventItem(30, toolTip, position, id); 
    eventVector.push_back(currentItem);
    delete query2;
  }
  delete query;
}


void EventGraphWidget::plotEvents() {
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    currentItem->setPos(currentItem->getOriginalPos());
    currentItem->setZValue(1);
    scene->addItem(currentItem);
  }
}

void EventGraphWidget::getEdges(QString coder, QString type) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", coder);
  query->bindValue(":type", type);
  query->exec();
  while (query->next()) {
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
    if (!(query2->isNull(0))) {
      comment = query2->value(0).toString();
      commentCoder = query2->value(1).toString();
      toolTip = "<FONT SIZE = 3>" + commentCoder + " - " + comment + "</FONT>";
    } else {
      toolTip = "No comments";
    }
    QVectorIterator<EventItem*> it(eventVector);
    EventItem *tempSource = NULL;
    EventItem *tempTarget = NULL;
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      if (currentItem->getId() == tail) {
	tempSource = currentItem;
      } else if (currentItem->getId() == head) {
	tempTarget = currentItem;
      }
      if (tempSource != NULL && tempTarget != NULL) {
	int sourceHeight = tempSource->getOriginalPos().y();
	int newTargetHeight = sourceHeight + qrand() % 240 - 120;
	int targetOrder = tempTarget->getOriginalPos().x();
	tempTarget->setPos(targetOrder, newTargetHeight);
	Arrow *currentEdge = new Arrow(tempSource, tempTarget, type, coder);
	currentEdge->setToolTip(toolTip);
	edgeVector.push_back(currentEdge);
	break;
      }
    }
    delete query2;
  }
  delete query;
}

void EventGraphWidget::plotEdges() {
  QVectorIterator<Arrow*> it(edgeVector);
  while (it.hasNext()) {
    Arrow *currentEdge = it.next();
    scene->addItem(currentEdge);
  }
}

void EventGraphWidget::cleanUp() {
  qDeleteAll(eventVector);
  qDeleteAll(edgeVector);
  eventVector.clear();
  edgeVector.clear();
  qDeleteAll(compareVector);
  compareVector.clear();
  selectedType = "";
  selectedCoder = "";
  selectedCompare = "";
}

void EventGraphWidget::increaseWidth(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      currentItem->resetOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::decreaseWidth(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());
      currentItem->resetOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::increasePos(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() + 1, currentItem->pos().y());
      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::decreasePos(EventItem *item) {
  QPointF original = item->scenePos();
  int order = original.x();
  QVectorIterator<EventItem*> it(eventVector);
  while (it.hasNext()) {
    EventItem *currentItem = it.next();
    if (currentItem->pos().x() > order) {
      QPointF newPos = QPointF(currentItem->pos().x() - 1, currentItem->pos().y());

      currentItem->setPos(newPos);
    }
  }
}

void EventGraphWidget::increaseDistance() {
  QVectorIterator<EventItem*> it(eventVector);
  int unit = 1;
  if (distance < 100) {
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      QPointF newPos = QPointF(currentItem->pos().x() + unit, currentItem->pos().y());
      currentItem->resetOriginalPos(newPos);
      currentItem->setPos(newPos);
      unit++;
    }
  }
  distance++;
}

void EventGraphWidget::decreaseDistance() {
  QVectorIterator<EventItem*> it(eventVector);
  int unit = 1;
  if (distance > 35) {
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      QPointF newPos = QPointF(currentItem->pos().x() - unit, currentItem->pos().y());
      currentItem->resetOriginalPos(newPos);
      currentItem->setPos(newPos);
      unit++;
    }
  }
  distance--;
}

void EventGraphWidget::setPlotButton() {
  if (typeComboBox->currentText() != DEFAULT && coderComboBox->currentText() != DEFAULT) {
    plotButton->setEnabled(true);
  } else {
    plotButton->setEnabled(false);
  }
  if (coderComboBox->currentText() != DEFAULT) {
    compareComboBox->clear();
    compareComboBox->addItem(DEFAULT);
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT name FROM coders");
    while (query->next()) {
      QString currentCoder = query->value(0).toString();
      if (currentCoder != coderComboBox->currentText()) {
	compareComboBox->addItem(currentCoder);
      }
    }
    delete query;
  }
}

void EventGraphWidget::getLinkageDetails() {
  coderComboBox->clear();
  coderComboBox->addItem(DEFAULT);
  typeComboBox->clear();
  typeComboBox->addItem(DEFAULT);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM coders");
  while (query->next()) {
    QString currentCoder = query->value(0).toString();
    coderComboBox->blockSignals(true);
    coderComboBox->addItem(currentCoder);
    coderComboBox->blockSignals(false);
  }
  query->exec("SELECT name FROM linkage_types");
  while (query->next()) {
    QString currentType = query->value(0).toString();
    typeComboBox->addItem(currentType);
  }
  delete query;
}

void EventGraphWidget::plotGraph() {
  cleanUp();
  selectedCoder = coderComboBox->currentText();
  selectedType = typeComboBox->currentText();
  distance = 70;
  getEvents();
  plotEvents(); // Should allow for range to be set here.
  getEdges(selectedCoder, selectedType);
  plotEdges();
  compareComboBox->setCurrentIndex(0);
  // should allow someone to save plot.
}

void EventGraphWidget::setCompareButton() {
  if (compareComboBox->currentText() != DEFAULT && selectedCoder != "") {
    compareButton->setEnabled(true);
  } else {
    compareButton->setEnabled(false);
  }
}

void EventGraphWidget::compare() {
  selectedCompare = compareComboBox->currentText();
  getCompareEdges(selectedCompare, selectedType);
  plotCompareEdges();
  compareButton->setEnabled(false);
}

void EventGraphWidget::getCompareEdges(QString coder, QString type) {
  qDeleteAll(compareVector);
  compareVector.clear();

  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", coder);
  query->bindValue(":type", type);
  query->exec();
  while (query->next()) {
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
    if (!(query2->isNull(0))) {
      comment = query2->value(0).toString();
      commentCoder = query2->value(1).toString();
      toolTip = "<FONT SIZE = 3>" + commentCoder + " - " + comment + "</FONT>";
    } else {
      toolTip = "No comments";
    }
    QVectorIterator<EventItem*> it(eventVector);
    EventItem *tempSource = NULL;
    EventItem *tempTarget = NULL;
    while (it.hasNext()) {
      EventItem *currentItem = it.next();
      if (currentItem->getId() == tail) {
	tempSource = currentItem;
      } else if (currentItem->getId() == head) {
	tempTarget = currentItem;
      }
      if (tempSource != NULL && tempTarget != NULL) {
	bool sameFound = false;
	QVectorIterator<Arrow*> it2(edgeVector);
	while (it2.hasNext()) {
	  Arrow *currentEdge = it2.next();
	  EventItem *tempS = currentEdge->startItem();
	  EventItem *tempT = currentEdge->endItem();
	  if (tempSource == tempS && tempTarget == tempT) {
	    currentEdge->setColor(Qt::darkGreen);
	    sameFound = true;
	  }
	}
	if (sameFound == false) {
	  Arrow *compareEdge = new Arrow(tempSource, tempTarget, type, coder);
	  compareEdge->setColor(Qt::darkRed);
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

void EventGraphWidget::plotCompareEdges() {
  QVectorIterator<Arrow*> it(compareVector);
  while (it.hasNext()) {
    Arrow *currentEdge = it.next();
    scene->addItem(currentEdge);
  }
}
