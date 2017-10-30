#include "../include/AttributesWidget.h"

AttributesWidget::AttributesWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  incidentsModel = new QSqlTableModel(this);  
  incidentsModel->setTable("incidents");
  incidentsModel->select();

  attributesModel = new QSqlTableModel(this);
  attributesModel->setTable("incident_attributes");
  attributesModel->select();

  assignedModel = new QSqlTableModel(this);
  assignedModel->setTable("attributes_to_incidents");
  assignedModel->select();

  attributesTreeView = new DeselectableTreeView(this);
  attributesTreeView->setModel(attributesTree);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->setDragEnabled(true);
  attributesTreeView->setAcceptDrops(true);
  attributesTreeView->setDropIndicatorShown(true);
  attributesTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  setTree();
  
  timeStampLabel = new QLabel("Timing:");
  sourceLabel = new QLabel("Source:");
  descriptionLabel = new QLabel("Description:");
  rawLabel = new QLabel("Raw:");
  commentLabel = new QLabel("Comments:");
  attributesLabel = new QLabel("<h2>Attributes</h2>");

  timeStampField = new QLineEdit();
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit();
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit();
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit();
  rawField->setReadOnly(true);
  commentField = new QTextEdit();
  commentField->setReadOnly(true);

  previousIncidentButton = new QPushButton("Previous incident");
  nextIncidentButton = new QPushButton("Next incident");
  newAttributeButton = new QPushButton("New attribute");
  assignAttributeButton = new QPushButton("Assign attribute");
  unassignAttributeButton = new QPushButton("Unassign attribute");
  removeUnusedAttributesButton = new QPushButton("Remove unused attributes");
  
  connect(previousIncidentButton, SIGNAL(clicked()), this, SLOT(previousIncident()));
  connect(nextIncidentButton, SIGNAL(clicked()), this, SLOT(nextIncident()));
  connect(newAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
									       
  QPointer<QHBoxLayout> mainLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  topFieldsLayout->addWidget(timeStampLabel);
  topFieldsLayout->addWidget(timeStampField);
  topFieldsLayout->addWidget(sourceLabel);
  topFieldsLayout->addWidget(sourceField);
  leftLayout->addLayout(topFieldsLayout);
  leftLayout->addWidget(descriptionLabel);
  leftLayout->addWidget(descriptionField);
  leftLayout->addWidget(rawLabel);
  leftLayout->addWidget(rawField);
  leftLayout->addWidget(commentLabel);
  leftLayout->addWidget(commentField);
  QPointer<QHBoxLayout> leftButtonLayout = new QHBoxLayout;
  leftButtonLayout->addWidget(previousIncidentButton);
  leftButtonLayout->addWidget(nextIncidentButton);
  leftLayout->addLayout(leftButtonLayout);
  mainLayout->addLayout(leftLayout);
  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  rightLayout->addWidget(attributesLabel);
  rightLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> rightButtonTopLayout = new QHBoxLayout;
  rightButtonTopLayout->addWidget(newAttributeButton);
  QPointer<QHBoxLayout> rightButtonBottomLayout = new QHBoxLayout;
  rightButtonBottomLayout->addWidget(assignAttributeButton);
  rightButtonBottomLayout->addWidget(unassignAttributeButton);
  rightButtonBottomLayout->addWidget(removeUnusedAttributesButton);
  rightLayout->addLayout(rightButtonTopLayout);
  rightLayout->addLayout(rightButtonBottomLayout);
  mainLayout->addLayout(rightLayout);
  
  setLayout(mainLayout);
}

void AttributesWidget::previousIncident() {
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  if (order != 1) {
    query->prepare("UPDATE save_data "
		   "SET attributes_record=:new");
    query->bindValue(":new", order - 1);
    query->exec();
    retrieveData();
  }  
}

void AttributesWidget::nextIncident() {
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order != incidentsModel->rowCount()) {
    query->prepare("UPDATE save_data "
		   "SET attributes_record=:new");
    query->bindValue(":new", order + 1);
    query->exec();
    retrieveData();
  }
}

void AttributesWidget::newAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString currentParent = attributesTreeView->currentIndex().data().toString();
    QString name = "";
    QString description = "";
    attributeDialog = new AttributeDialog(this, esd);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *attribute = new QStandardItem(name);    
      attribute->setToolTip(description);
      QStandardItem *father = attributesTree->itemFromIndex(attributesTreeView->currentIndex());
      father->setChild(father->rowCount(), attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);

      attributesModel->select();
      int newIndex = attributesModel->rowCount();
      attributesModel->insertRow(newIndex);
      attributesModel->setData(attributesModel->index(newIndex, 2), name);
      attributesModel->setData(attributesModel->index(newIndex, 3), description);
      attributesModel->setData(attributesModel->index(newIndex, 4), currentParent);
      attributesModel->submitAll();
    }
    delete attributeDialog;
  } else {
    QString name = "";
    QString description = "";
    attributeDialog = new AttributeDialog(this, esd);
    attributeDialog->exec();
    
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
    
      int newIndex = attributesModel->rowCount();
      
      attributesModel->insertRow(newIndex);
      
      attributesModel->setData(attributesModel->index(newIndex, 2), name);
      attributesModel->setData(attributesModel->index(newIndex, 3), description);
      attributesModel->setData(attributesModel->index(newIndex, 4), "NONE");
      attributesModel->submitAll();
      QStandardItem *attribute = new QStandardItem(name);    
      attributesTree->appendRow(attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);
    }
    delete attributeDialog;
  }
}

void AttributesWidget::assignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery("SELECT * FROM save_data");
    int order = 0; 
    order = query->record(0).value("attributes_record").toInt();

    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
    query2->bindValue(":order", order);
    query2->exec();
    query2->first();
    if (!(query2->isNull(0))) {
      int id = 0;
      id = query2->value(0).toInt();
      QString attribute = attributesTreeView->currentIndex().data().toString();
      
      assignedModel->select();
      int max = assignedModel->rowCount();
      
      bool empty = false;
      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents WHERE attribute = :att AND incident = :inc  ");
      query2->bindValue(":att", attribute);
      query2->bindValue(":inc", id);
      query2->exec();
      query2->first();
      empty = query2->isNull(0);
      if (empty) {
	assignedModel->insertRow(max);
	assignedModel->setData(assignedModel->index(max, 1), attribute);
	assignedModel->setData(assignedModel->index(max, 2), id);
	assignedModel->submitAll();
	QStandardItem *currentAttribute = attributesTree->itemFromIndex(attributesTreeView->currentIndex());
	QFont font;
	font.setBold(true);
	currentAttribute->setFont(font);
      }
    }
  }
}

void AttributesWidget::unassignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery("SELECT * FROM save_data");
    int order = 0; 
    order = query->record(0).value("attributes_record").toInt();

    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
    query2->bindValue(":order", order);
    query2->exec();
    query2->first();
    int id = 0;
    if (!(query2->isNull(0))) {
      id = query2->value(0).toInt();
      QString attribute = attributesTreeView->currentIndex().data().toString();
      
      assignedModel->select();
      bool empty = false;
      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents WHERE attribute = :att AND incident = :inc  ");
      query2->bindValue(":att", attribute);
      query2->bindValue(":inc", id);
      query2->exec();
      query2->first();
      empty = query2->isNull(0);
      if (!empty) {
	query2->prepare("DELETE FROM attributes_to_incidents WHERE attribute = :att AND incident = :inc");
	query2->bindValue(":att", attribute);
	query2->bindValue(":inc", id);
	query2->exec();
	assignedModel->select();
	QStandardItem *currentAttribute = attributesTree->itemFromIndex(attributesTreeView->currentIndex());
	QFont font;
	font.setBold(false);
	currentAttribute->setFont(font);
      }
    }
  }
}

void AttributesWidget::removeUnusedAttributes() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  while (unfinished) {
    query->exec("SELECT name FROM incident_attributes EXCEPT SELECT attribute "
		"FROM attributes_to_incidents EXCEPT SELECT father "
		"FROM incident_attributes");
    while (query->next()) {
      QString current = query->value(0).toString();
      query2->prepare("DELETE FROM incident_attributes WHERE name = :current");
      query2->bindValue(":current", current);
      query2->exec();
    }
    query->first();
    if (query->isNull(0)) {
      unfinished = false;
    }
  }
  this->setCursor(Qt::WaitCursor);
  treeOrder(attributesTree);
  setTree();
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
}

void AttributesWidget::retrieveData() {
  QSqlQueryModel *query = new QSqlQueryModel;
  query->setQuery("SELECT * FROM save_data");
  int order = 0;
  order = query->record(0).value("attributes_record").toInt();
  query->setQuery("SELECT * FROM incidents");
  int id = query->record(order - 1).value("id").toInt();
  QString timeStamp = query->record(order - 1).value("timestamp").toString();
  QString source = query->record(order - 1).value("source").toString();
  QString description = query->record(order - 1).value("description").toString();
  QString raw = query->record(order - 1).value("raw").toString();
  QString comment = query->record(order - 1).value("comment").toString();
  timeStampField->setText(timeStamp);
  sourceField->setText(source);
  descriptionField->setText(description);
  rawField->setText(raw);
  commentField->setText(comment);

  resetFont(attributesTree);
  QSqlQuery *query2 = new QSqlQuery;
  query2->exec("SELECT attribute, incident FROM attributes_to_incidents");
  while (query2->next()) {
    QString attribute = query2->value(0).toString();
    int incident = query2->value(1).toInt();
    if (incident == id) {
      boldSelected(attributesTree, attribute);
    }
  }
  attributesTreeView->setModel(attributesTree);
  attributesTreeView->resetSelection();
}

void AttributesWidget::setTree() {
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE' ORDER BY sort_order asc");
  while (query->next()) {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *father = new QStandardItem(name);    
    attributesTree->appendRow(father);
    father->setToolTip(description);
    father->setEditable(false);
    buildHierarchy(father, name);
  }
  attributesTreeView->setModel(attributesTree);
  attributesTreeView->expandAll();
}

void AttributesWidget::buildHierarchy(QStandardItem *top, QString name) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father ORDER BY sort_order asc");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) {
    QString childName = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *child = new QStandardItem(childName);
    top->setChild(children, child);
    child->setToolTip(description);
    child->setEditable(false);
    children++;
    buildHierarchy(child, childName);
  }
}

void AttributesWidget::boldSelected(QAbstractItemModel *model, QString name, QModelIndex parent) {
  for(int i = 0; i != model->rowCount(parent); i++) {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
    QFont font;
    if (name == currentName) {
      font.setBold(true);
      currentAttribute->setFont(font);
    }
    if (model->hasChildren(index)) {
      boldSelected(model, name, index);
    }
  }
}

void AttributesWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) {
  for(int i = 0; i != model->rowCount(parent); i++) {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
    QFont font;
    font.setBold(false);
    currentAttribute->setFont(font);
    if (model->hasChildren(index)) {
      resetFont(model, index);
    }
  }
}

void AttributesWidget::treeOrder(QAbstractItemModel *model, QModelIndex parent) {
  for(int i = 0; i != model->rowCount(parent); i++) {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("UPDATE incident_attributes SET sort_order = :so WHERE name = :current");
    query->bindValue(":so", i);
    query->bindValue(":current", currentName);
    query->exec();
    if (model->hasChildren(index)) {
      treeOrder(model, index);
    }
  }
}

void AttributesWidget::finalBusiness() {
  treeOrder(attributesTree);
}

