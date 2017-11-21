#include "../include/NetworkGraphWidget.h"

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) : QWidget(parent) {
  scene = new Scene(this);
  view = new GraphicsView(scene);
  view->setBackgroundBrush(QColor(230,230,250)); // Sets the background colour.

  getEntities();
  getDirectedEdges();
  simpleLayout();
  plotEntities();
  plotDirectedEdges();
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(view);

  setLayout(mainLayout);
}

void NetworkGraphWidget::getEntities() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM entities");
  while (query->next()) {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    NetworkNode *currentNode = new NetworkNode(name, description);
    currentNode->setZValue(1);
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
    query2->prepare("SELECT name, source, target, comment "
		   "FROM entity_relationships "
		   "WHERE type = :type");
    query2->bindValue(":type", type);
    query2->exec();
    while (query2->next()) {
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
	  int height = 20;
	  QVectorIterator<DirectedEdge*> it2(directedVector);
	  while (it2.hasNext()) {
	    DirectedEdge* tempEdge = it2.next();
	  if (tempEdge->startItem() == tempSource && tempEdge->endItem() == tempTarget) {
	    height = height + 80;
	  }
	  }
	  DirectedEdge *currentEdge = new DirectedEdge(tempSource, tempTarget, height);
	  directedVector.push_back(currentEdge);
	  break;
	}
      }
    }
    delete query2;
      
  }
  delete query;
}

void NetworkGraphWidget::plotDirectedEdges() {
  QVectorIterator<DirectedEdge*> it(directedVector);
  while (it.hasNext()) {
    DirectedEdge* currentEdge = it.next();
    scene->addItem(currentEdge);
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
    QVectorIterator<DirectedEdge*> it(directedVector);
    while (it.hasNext()) {
      DirectedEdge *currentEdge = it.next();
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
    }
  }
}
