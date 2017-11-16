#include "../include/EventGraphWidget.h"

EventGraphWidget::EventGraphWidget(QWidget *parent) : QWidget(parent) {
  scene = new QGraphicsScene(this);
  view = new GraphicsView(scene);
  
  //  temp();
  getEvents();
  plotEvents();
  getEdges();
  plotEdges();

  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);

  view->setBackgroundBrush(QColor(230, 200, 167)); // Sets the background colour.
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setWindowTitle("Graphics Test");

  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(cleanUp()));

  QPointer<QHBoxLayout> mainLayout = new QHBoxLayout;
  mainLayout->addWidget(view);

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
    QPointF position = QPointF((order * 70), 0);
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

void EventGraphWidget::getEdges() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT tail, head, type, coder FROM linkages");
  while (query->next()) {
    int tail = query->value(0).toInt();
    int head = query->value(1).toInt();
    QString type = query->value(2).toString();
    QString coder = query->value(3).toString();
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
	Arrow *currentEdge = new Arrow(tempSource, tempTarget, type, coder);
	edgeVector.push_back(currentEdge);
	break;
      }
    }
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


void EventGraphWidget::temp() {
  /*
  EventItem *itemOne = new EventItem(100, "test", );
  int horPos = 1 * 35;
  itemOne->setPos(horPos, 200);
  itemOne->setOriginalPos(horPos, 100);
  scene->addItem(itemOne);

  EventItem *itemTwo = new EventItem(300);
  horPos = 5 * 35;
  itemTwo->setPos(horPos, 200);
  itemTwo->setOriginalPos(horPos, 100);
  scene->addItem(itemTwo);

  EventItem *itemThree = new EventItem(30);
  horPos = 30 * 35;
  itemThree->setPos(horPos, 200);
  itemThree->setOriginalPos(horPos, 100);
  scene->addItem(itemThree);

  EventItem *itemFour = new EventItem(100);
  horPos = 1 * 35;
  itemFour->setPos(horPos, 400);
  itemFour->setOriginalPos(horPos, 0);
  scene->addItem(itemFour);

  EventItem *itemFive = new EventItem(300);
  horPos = 5 * 35;
  itemFive->setPos(horPos, 400);
  itemFive->setOriginalPos(horPos, 0);
  scene->addItem(itemFive);

  EventItem *itemSix = new EventItem(30);
  horPos = 30 * 35;
  itemSix->setPos(horPos, 400);
  itemSix->setOriginalPos(horPos, 0);
  scene->addItem(itemSix);

  EventItem *itemSeven = new EventItem(30);
  horPos = 5 * 35;
  itemSeven->setPos(horPos, 100);
  itemSeven->setOriginalPos(horPos, 0);
  scene->addItem(itemSeven);

  Arrow *line = new Arrow(itemTwo, itemOne);
  Arrow *line2 = new Arrow(itemThree, itemTwo);

  Arrow *line3 = new Arrow(itemFour, itemFive);
  Arrow *line4 = new Arrow(itemFive, itemSix);

  Arrow *line5 = new Arrow(itemSeven, itemOne);
  
  scene->addItem(line);
  scene->addItem(line2);

  scene->addItem(line3);
  scene->addItem(line4);

  scene->addItem(line5);
  
  itemOne->setZValue(1);
  itemTwo->setZValue(1);
  itemThree->setZValue(1);
  itemFour->setZValue(1);
  itemFive->setZValue(1);
  itemSix->setZValue(1);
  itemSeven->setZValue(1);*/
}

void EventGraphWidget::cleanUp() {
  qDeleteAll(eventVector);
  qDeleteAll(edgeVector);
  eventVector.clear();
  edgeVector.clear();
}
