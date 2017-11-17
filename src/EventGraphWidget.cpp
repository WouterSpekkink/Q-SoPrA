#include "../include/EventGraphWidget.h"

EventGraphWidget::EventGraphWidget(QWidget *parent) : QWidget(parent) {
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

  view->setBackgroundBrush(QColor(230, 200, 167)); // Sets the background colour.
  view->setRenderHint(QPainter::Antialiasing);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  setWindowTitle("Graphics Test");

  connect(scene, SIGNAL(widthIncreased(EventItem*)), this, SLOT(increaseWidth(EventItem*)));
  connect(scene, SIGNAL(widthDecreased(EventItem*)), this, SLOT(decreaseWidth(EventItem*)));
  connect(scene, SIGNAL(posIncreased(EventItem*)), this, SLOT(increasePos(EventItem*)));
  connect(scene, SIGNAL(posDecreased(EventItem*)), this, SLOT(decreasePos(EventItem*)));
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
    int vertical = qrand() % 240 - 120;
    QPointF position = QPointF((order * 70), vertical);
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
	int sourceHeight = tempSource->getOriginalPos().y();
	int newTargetHeight = sourceHeight + qrand() % 240 - 120;
	int targetOrder = tempTarget->getOriginalPos().x();
	tempTarget->setPos(targetOrder, newTargetHeight);
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

void EventGraphWidget::cleanUp() {
  qDeleteAll(eventVector);
  qDeleteAll(edgeVector);
  eventVector.clear();
  edgeVector.clear();
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
      //currentItem->resetOriginalPos(newPos);
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
      //currentItem->resetOriginalPos(newPos);
      currentItem->setPos(newPos);
    }
  }
}

