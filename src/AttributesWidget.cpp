#include "../include/AttributesWidget.h"

AttributesWidget::AttributesWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  descriptionFilter = "";
  rawFilter = "";
  commentFilter = "";
  commentBool = false;

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
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->setDragEnabled(true);
  attributesTreeView->setAcceptDrops(true);
  attributesTreeView->setDropIndicatorShown(true);
  attributesTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  attributesTreeView->setExpandsOnDoubleClick(false);
  treeFilter = new AttributeTreeFilter(this);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);

  indexLabel = new QLabel("<b>Incident ( / )</b>");
  markLabel = new QLabel();
  markLabel->setStyleSheet("QLabel {color: red}");
  timeStampLabel = new QLabel("<b>Timing:</b>");
  sourceLabel = new QLabel("<b>Source:</b>");
  descriptionLabel = new QLabel("<b>Description:</b>");
  rawLabel = new QLabel("<b>Raw:</b>");
  commentLabel = new QLabel("<b>Comments:</b>");
  attributesLabel = new QLabel("<h2>Attributes</h2>");
  descriptionFilterLabel = new QLabel("<i>Search descriptions:</i>");
  rawFilterLabel = new QLabel("<i>Search raw texts:</i>");
  commentFilterLabel = new QLabel("<i>Search comments:</i>");
  attributeFilterLabel = new QLabel("<b>Filter attributes:</b>");
  valueLabel = new QLabel("<b>Value:</b>");
  
  timeStampField = new QLineEdit();
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit();
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit();
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit();
  rawField->setReadOnly(true);
  commentField = new QTextEdit();
  descriptionFilterField = new QLineEdit();
  rawFilterField = new QLineEdit();
  commentFilterField = new QLineEdit();
  attributeFilterField = new QLineEdit();
  valueField = new QLineEdit();

  previousIncidentButton = new QPushButton("Previous incident");
  previousIncidentButton->setStyleSheet("QPushButton {font-weight: bold}");
  nextIncidentButton = new QPushButton("Next incident");
  nextIncidentButton->setStyleSheet("QPushButton {font-weight: bold}");
  jumpButton = new QPushButton("Jump to");
  markButton = new QPushButton("Mark incident");
  previousMarkedButton = new QPushButton("Previous marked");
  nextMarkedButton = new QPushButton("Next marked");
  descriptionPreviousButton = new QPushButton("Previous");
  descriptionNextButton = new QPushButton("Next");
  rawPreviousButton = new QPushButton("Previous");
  rawNextButton = new QPushButton("Next");
  commentPreviousButton = new QPushButton("Previous");
  commentNextButton = new QPushButton("Next");
  newAttributeButton = new QPushButton("New attribute");
  editAttributeButton = new QPushButton("Edit attribute");
  assignAttributeButton = new QPushButton("Assign attribute");
  unassignAttributeButton = new QPushButton("Unassign attribute");
  removeUnusedAttributesButton = new QPushButton("Remove unused attributes");
  valueButton = new QPushButton("Store value");
  valueButton->setEnabled(false);
  expandTreeButton = new QPushButton("Expand");
  collapseTreeButton = new QPushButton("Collapse");
  
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(previousIncidentButton, SIGNAL(clicked()), this, SLOT(previousIncident()));
  connect(nextIncidentButton, SIGNAL(clicked()), this, SLOT(nextIncident()));
  connect(jumpButton, SIGNAL(clicked()), this, SLOT(jumpIncident()));
  connect(markButton, SIGNAL(clicked()), this, SLOT(toggleMark()));
  connect(previousMarkedButton, SIGNAL(clicked()), this, SLOT(previousMarked()));
  connect(nextMarkedButton, SIGNAL(clicked()), this, SLOT(nextMarked()));
  connect(descriptionFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(setDescriptionFilter(const QString &)));
  connect(descriptionPreviousButton, SIGNAL(clicked()), this, SLOT(previousDescription()));
  connect(descriptionNextButton, SIGNAL(clicked()), this, SLOT(nextDescription()));
  connect(rawFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(setRawFilter(const QString &)));
  connect(rawPreviousButton, SIGNAL(clicked()), this, SLOT(previousRaw()));
  connect(rawNextButton, SIGNAL(clicked()), this, SLOT(nextRaw()));
  connect(commentFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(setCommentFilter(const QString &)));
  connect(commentPreviousButton, SIGNAL(clicked()), this, SLOT(previousComment()));
  connect(commentNextButton, SIGNAL(clicked()), this, SLOT(nextComment()));
  connect(newAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(attributeFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(changeFilter(const QString &)));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(attributesTreeView, SIGNAL(selectionChanged()), this, SLOT(getValue()));
  connect(attributesTreeView, SIGNAL(selectionChanged()), this, SLOT(highlightText()));
  connect(attributesTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(decideAttributeAction()));
  connect(expandTreeButton, SIGNAL(clicked()), this, SLOT(expandTree()));
  connect(collapseTreeButton, SIGNAL(clicked()), this, SLOT(collapseTree()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
									       
  QPointer<QHBoxLayout> mainLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> indexLayout = new QHBoxLayout;
  indexLayout->addWidget(indexLabel);
  indexLabel->setAlignment(Qt::AlignHCenter);
  indexLayout->addWidget(markLabel);
  markLabel->setAlignment(Qt::AlignHCenter);
  leftLayout->addLayout(indexLayout);
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  topFieldsLayout->addWidget(timeStampLabel);
  topFieldsLayout->addWidget(timeStampField);
  topFieldsLayout->addWidget(sourceLabel);
  topFieldsLayout->addWidget(sourceField);
  leftLayout->addLayout(topFieldsLayout);
  QPointer<QHBoxLayout> descriptionLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> descriptionLayoutLeft = new QHBoxLayout;
  descriptionLayoutLeft->addWidget(descriptionLabel);
  descriptionLayout->addLayout(descriptionLayoutLeft);
  QPointer<QHBoxLayout> descriptionLayoutRight = new QHBoxLayout;
  descriptionLayoutRight->addWidget(descriptionFilterLabel);
  descriptionLayoutRight->addWidget(descriptionPreviousButton);
  descriptionLayoutRight->addWidget(descriptionFilterField);
  descriptionLayoutRight->addWidget(descriptionNextButton);
  descriptionLayout->addLayout(descriptionLayoutRight);
  descriptionLayoutRight->setContentsMargins(200,0,0,0);
  leftLayout->addLayout(descriptionLayout);
  leftLayout->addWidget(descriptionField);
  QPointer<QHBoxLayout> rawLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> rawLayoutLeft = new QHBoxLayout;
  rawLayoutLeft->addWidget(rawLabel);
  rawLayout->addLayout(rawLayoutLeft);
  QPointer<QHBoxLayout> rawLayoutRight = new QHBoxLayout;
  rawLayoutRight->addWidget(rawFilterLabel);
  rawLayoutRight->addWidget(rawPreviousButton);
  rawLayoutRight->addWidget(rawFilterField);
  rawLayoutRight->addWidget(rawNextButton);
  rawLayout->addLayout(rawLayoutRight);
  rawLayoutRight->setContentsMargins(270,0,0,0);
  leftLayout->addLayout(rawLayout);
  leftLayout->addWidget(rawField);
  QPointer<QHBoxLayout> commentLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> commentLayoutLeft = new QHBoxLayout;
  commentLayoutLeft->addWidget(commentLabel);
  commentLayout->addLayout(commentLayoutLeft);
  QPointer<QHBoxLayout> commentLayoutRight = new QHBoxLayout;
  commentLayoutRight->addWidget(commentFilterLabel);
  commentLayoutRight->addWidget(commentPreviousButton);
  commentLayoutRight->addWidget(commentFilterField);
  commentLayoutRight->addWidget(commentNextButton);
  commentLayout->addLayout(commentLayoutRight);
  commentLayoutRight->setContentsMargins(200,0,0,0);
  leftLayout->addLayout(commentLayout);
  leftLayout->addWidget(commentField);
  QPointer<QHBoxLayout> leftButtonTopLayout = new QHBoxLayout;
  leftButtonTopLayout->addWidget(previousIncidentButton);
  leftButtonTopLayout->addWidget(jumpButton);
  leftButtonTopLayout->addWidget(nextIncidentButton);
  leftLayout->addLayout(leftButtonTopLayout);
  QPointer<QHBoxLayout> leftButtonBottomLayout = new QHBoxLayout;
  leftButtonBottomLayout->addWidget(previousMarkedButton);
  leftButtonBottomLayout->addWidget(markButton);
  leftButtonBottomLayout->addWidget(nextMarkedButton);
  leftLayout->addLayout(leftButtonBottomLayout);
  mainLayout->addLayout(leftLayout);
  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  rightLayout->addWidget(attributesLabel);
  rightLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributeFilterLabel);
  filterLayout->addWidget(attributeFilterField);
  rightLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> valueLayout = new QHBoxLayout;
  valueLayout->addWidget(valueLabel);
  valueLayout->addWidget(valueField);
  valueLayout->addWidget(valueButton);
  rightLayout->addLayout(valueLayout);
  QPointer<QHBoxLayout> rightButtonTopLayout = new QHBoxLayout;
  rightButtonTopLayout->addWidget(newAttributeButton);
  rightButtonTopLayout->addWidget(editAttributeButton);
  rightButtonTopLayout->addWidget(expandTreeButton);
  rightButtonTopLayout->addWidget(collapseTreeButton);
  QPointer<QHBoxLayout> rightButtonBottomLayout = new QHBoxLayout;
  rightButtonBottomLayout->addWidget(assignAttributeButton);
  rightButtonBottomLayout->addWidget(unassignAttributeButton);
  rightButtonBottomLayout->addWidget(removeUnusedAttributesButton);
  rightLayout->addLayout(rightButtonTopLayout);
  rightLayout->addLayout(rightButtonBottomLayout);
  mainLayout->addLayout(rightLayout);
  
  setLayout(mainLayout);
}

void AttributesWidget::setCommentBool() {
  commentBool = true;
}

void AttributesWidget::setComment() {
  if (commentBool) {
    QString comment = commentField->toPlainText();
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    int order = 0;
    query->first();
    order = query->value(0).toInt();
    query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :order");
    query->bindValue(":comment", comment);
    query->bindValue(":order", order);
    query->exec();
    commentBool = false;
  }
}

void AttributesWidget::previousIncident() {
  setComment();
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

void AttributesWidget::jumpIncident() {
  setComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  AttributeIndexDialog *indexDialog = new AttributeIndexDialog(this, incidentsModel->rowCount());
  indexDialog->deleteLater();
  indexDialog->exec();
  int order = 0;
  if (indexDialog->getExitStatus() != 1) {
    order = indexDialog->getIndex();
    QSqlQuery *query = new QSqlQuery;
    if (order > 0) {
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::nextIncident() {
  setComment();
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

void AttributesWidget::toggleMark() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  query->first();
  int order = 0;
  order = query->value(0).toInt();
  query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
  query->bindValue(":order", order);
  query->exec();
  query->first();
  int currentMark = 0;
  currentMark = query->value(0).toInt();
  if (currentMark == 0) {
    int newMark = 1;
    query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
    query->bindValue(":order", order);
    query->bindValue(":newMark", newMark);
    query->exec();
    markLabel->setText("MARKED");
  } else if (currentMark == 1) {
    int newMark = 0;
    query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
    query->bindValue(":order", order);
    query->bindValue(":newMark", newMark);
    query->exec();
    markLabel->setText("");
  }
}

void AttributesWidget::previousMarked() {
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  query->prepare("SELECT ch_order FROM incidents WHERE ch_order < :order AND mark = 1 ORDER BY ch_order desc");
  query->bindValue(":order", order);
  query->exec();
  query->first();
  if (order != 1) {
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::nextMarked() {
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  query->prepare("SELECT ch_order FROM incidents WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
  query->bindValue(":order", order);
  query->exec();
  query->first();

  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order != incidentsModel->rowCount()) {
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}


void AttributesWidget::setDescriptionFilter(const QString &text) {
  descriptionFilter = text;
}

void AttributesWidget::previousDescription() {
  setComment();
  if (descriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    int order = 0;
    query->first();
    order = query->value(0).toInt();

    QString searchText = "%" + descriptionFilter + "%";
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE description LIKE :text "
		   "AND ch_order < :order "
		   "ORDER BY ch_order desc");
    query->bindValue(":text", searchText);
    query->bindValue(":order", order);
    query->exec();
    query->first();
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::nextDescription() {
  setComment();
  if (descriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    int order = 0;
    query->first();
    order = query->value(0).toInt();

    QString searchText = "%" + descriptionFilter + "%";
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE description LIKE :text "
		   "AND ch_order > :order "
		   "ORDER BY ch_order asc");
    query->bindValue(":text", searchText);
    query->bindValue(":order", order);
    query->exec();
    query->first();
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::setRawFilter(const QString &text) {
  rawFilter = text;
}

void AttributesWidget::previousRaw() {
  setComment();
  if (rawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    int order = 0;
    query->first();
    order = query->value(0).toInt();

    QString searchText = "%" + rawFilter + "%";
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE raw LIKE :text "
		   "AND ch_order < :order "
		   "ORDER BY ch_order desc");
    query->bindValue(":text", searchText);
    query->bindValue(":order", order);
    query->exec();
    query->first();
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::nextRaw() {
  setComment();
  if (rawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    int order = 0;
    query->first();
    order = query->value(0).toInt();

    QString searchText = "%" + rawFilter + "%";
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE raw LIKE :text "
		   "AND ch_order > :order "
		   "ORDER BY ch_order asc");
    query->bindValue(":text", searchText);
    query->bindValue(":order", order);
    query->exec();
    query->first();
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::setCommentFilter(const QString &text) {
  commentFilter = text;
}

void AttributesWidget::previousComment() {
  setComment();
  if (commentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    int order = 0;
    query->first();
    order = query->value(0).toInt();

    QString searchText = "%" + commentFilter + "%";
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE comment LIKE :text "
		   "AND ch_order < :order "
		   "ORDER BY ch_order desc");
    query->bindValue(":text", searchText);
    query->bindValue(":order", order);
    query->exec();
    query->first();
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::nextComment() {
  setComment();
  if (commentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    int order = 0;
    query->first();
    order = query->value(0).toInt();

    QString searchText = "%" + commentFilter + "%";
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE comment LIKE :text "
		   "AND ch_order > :order "
		   "ORDER BY ch_order asc");
    query->bindValue(":text", searchText);
    query->bindValue(":order", order);
    query->exec();
    query->first();
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (!query->isNull(0)) {
      order = query->value(0).toInt();
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void AttributesWidget::changeFilter(const QString &text) {
  QRegExp regExp(text);
  treeFilter->setFilterRegExp(regExp);
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
      attributesModel->setData(attributesModel->index(newIndex, 1), name);
      attributesModel->setData(attributesModel->index(newIndex, 2), description);
      attributesModel->setData(attributesModel->index(newIndex, 3), currentParent);
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
      
      attributesModel->setData(attributesModel->index(newIndex, 1), name);
      attributesModel->setData(attributesModel->index(newIndex, 2), description);
      attributesModel->setData(attributesModel->index(newIndex, 3), "NONE");
      attributesModel->submitAll();
      QStandardItem *attribute = new QStandardItem(name);    
      attributesTree->appendRow(attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);
    }
    delete attributeDialog;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void AttributesWidget::editAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString name = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM incident_attributes WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    attributeDialog = new AttributeDialog(this, esd);
    attributeDialog->submitName(name);
    attributeDialog->setDescription(description);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      QString newName = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(attributesTreeView->currentIndex());
      currentAttribute->setData(newName);
      currentAttribute->setData(newName, Qt::DisplayRole);      
      currentAttribute->setToolTip(description);
      query->prepare("UPDATE incident_attributes SET name = :newname, description = :newdescription WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdescription", description);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE incident_attributes SET father = :newname WHERE father = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_incidents SET attribute = :newname WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      this->setCursor(Qt::WaitCursor);
      retrieveData();
      this->setCursor(Qt::ArrowCursor);
    }
    delete attributeDialog;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void AttributesWidget::decideAttributeAction() {
  if (attributesTreeView->currentIndex().isValid()) {
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(attributesTreeView->currentIndex());
    QFont currentFont = currentAttribute->font();
    if (currentFont.bold()) {
      unassignAttribute();
    } else {
      assignAttribute();
    }
  }
}

void AttributesWidget::highlightText() {
  if (attributesTreeView->currentIndex().isValid()) {
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(attributesTreeView->currentIndex());
    QString currentName = attributesTreeView->currentIndex().data().toString();
    QFont currentFont = currentAttribute->font();
    QSqlQueryModel *query = new QSqlQueryModel;
    if (currentFont.bold()) {
      query->setQuery("SELECT * FROM save_data");
      int order = 0; 
      order = query->record(0).value("attributes_record").toInt();
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query2->bindValue(":order", order);
      query2->exec();
      query2->first();
      int id = 0;
      id = query2->value(0).toInt();
      query2->prepare("SELECT source_text FROM attributes_to_incidents WHERE attribute = :attribute AND incident = :id");
      query2->bindValue(":attribute", currentName);
      query2->bindValue(":id", id);
      query2->exec();
      query2->first();
      QTextCharFormat format;
      format.setFontWeight(QFont::Normal);
      format.setUnderlineStyle(QTextCharFormat::NoUnderline);
      rawField->selectAll();
      rawField->textCursor().mergeCharFormat(format);
      QTextCursor cursor = rawField->textCursor();
      cursor.movePosition(QTextCursor::Start);
      rawField->setTextCursor(cursor);
      QString currentText = query2->value(0).toString();
      rawField->find(currentText);
      format.setFontWeight(QFont::Bold);
      format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
      format.setUnderlineColor(Qt::blue);
      rawField->textCursor().mergeCharFormat(format);
      cursor = rawField->textCursor();
      cursor.movePosition(QTextCursor::Start);
      rawField->setTextCursor(cursor);
    } else {
      QString currentSelected = rawField->textCursor().selectedText();
      QTextCharFormat format;
      format.setFontWeight(QFont::Normal);
      format.setUnderlineStyle(QTextCharFormat::NoUnderline);
      rawField->selectAll();
      rawField->textCursor().mergeCharFormat(format);
      QTextCursor cursor = rawField->textCursor();
      cursor.movePosition(QTextCursor::Start);
      rawField->setTextCursor(cursor);
      rawField->find(currentSelected);      
    }
  } else {
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setUnderlineStyle(QTextCharFormat::NoUnderline);
    rawField->selectAll();
    rawField->textCursor().mergeCharFormat(format);
    QTextCursor cursor = rawField->textCursor();
    cursor.movePosition(QTextCursor::Start);
    rawField->setTextCursor(cursor);
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
	if (rawField->textCursor().selectedText().trimmed() != "") {
	  QString sourceText = rawField->textCursor().selectedText().trimmed();
	  assignedModel->setData(assignedModel->index(max, 4), sourceText);
	}
	QTextCharFormat format;
	format.setFontWeight(QFont::Bold);
	format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
	format.setUnderlineColor(Qt::blue);
	rawField->textCursor().mergeCharFormat(format);
	QTextCursor cursor = rawField->textCursor();
	cursor.movePosition(QTextCursor::Start);
	rawField->setTextCursor(cursor);

	assignedModel->submitAll();
	QStandardItem *currentAttribute = attributesTree->itemFromIndex(attributesTreeView->currentIndex());
	QFont font;
	font.setBold(true);
	currentAttribute->setFont(font);
	valueButton->setEnabled(true);
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
	valueButton->setEnabled(false);
	QTextCharFormat format;
	format.setFontWeight(QFont::Normal);
	format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	rawField->selectAll();
	rawField->textCursor().mergeCharFormat(format);
	rawField->setFontWeight(QFont::Normal);
	QTextCursor cursor = rawField->textCursor();
	cursor.movePosition(QTextCursor::Start);
	rawField->setTextCursor(cursor);
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
  attributesTreeView->setSortingEnabled(false);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
}

void AttributesWidget::setValue() {
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
      query2->prepare("UPDATE attributes_to_incidents SET value = :val WHERE attribute = :attribute AND incident = :id");
      query2->bindValue(":val", valueField->text());
      query2->bindValue(":attribute", attribute);
      query2->bindValue(":id", id);
      query2->exec();
    }
    valueField->setText("");
  }
}

void AttributesWidget::getValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery("SELECT * FROM save_data");
    int order = 0; 
    order = query->record(0).value("attributes_record").toInt();
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
    query2->bindValue(":order", order);
    query2->exec();
    query2->first();
    int id = 0;
    id = query2->value(0).toInt();    
    query2->prepare("SELECT attribute, value FROM attributes_to_incidents WHERE incident = :id AND attribute =:att");
    query2->bindValue(":id", id);
    query2->bindValue(":att", attribute);
    query2->exec();
    query2->first();
    if (!(query2->isNull(0))) {
      valueButton->setEnabled(true);
    } else {
      valueButton->setEnabled(false);
    }
    if (!(query2->isNull(1))) {
      QString value = query2->value(0).toString();
      valueField->setText(value);
    } else {
      valueField->setText("");
    }
  } else {
    valueField->setText("");
  }
}

void AttributesWidget::retrieveData() {
  QSqlQueryModel *query = new QSqlQueryModel;
  query->setQuery("SELECT * FROM save_data");
  int order = 0;
  order = query->record(0).value("attributes_record").toInt();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  int total = incidentsModel->rowCount();

  QString indexText = "<b>Incident (" + QString::number(order) + " / " + QString::number(total) + ")<b>";
  indexLabel->setText(indexText);
  
  query->setQuery("SELECT * FROM incidents");
  int id = query->record(order - 1).value("id").toInt();
  QString timeStamp = query->record(order - 1).value("timestamp").toString();
  QString source = query->record(order - 1).value("source").toString();
  QString description = query->record(order - 1).value("description").toString();
  QString raw = query->record(order - 1).value("raw").toString();
  QString comment = query->record(order - 1).value("comment").toString();
  int mark = query->record(order - 1).value("mark").toInt();
  timeStampField->setText(timeStamp);
  sourceField->setText(source);
  descriptionField->setText(description);
  rawField->setText(raw);
  commentField->blockSignals(true);
  commentField->setText(comment);
  commentField->blockSignals(false);
  if (mark == 0) {
    markLabel->setText("");
  } else {
    markLabel->setText("MARKED");
  }

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
  attributesTreeView->setModel(treeFilter);
  attributesTreeView->resetSelection();
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  QTextCharFormat format;
  format.setFontWeight(QFont::Normal);
  format.setUnderlineStyle(QTextCharFormat::NoUnderline);
  rawField->selectAll();
  rawField->textCursor().mergeCharFormat(format);
  QTextCursor cursor = rawField->textCursor();
  cursor.movePosition(QTextCursor::Start);
  rawField->setTextCursor(cursor);
}

void AttributesWidget::expandTree() {
  attributesTreeView->expandAll();
}

void AttributesWidget::collapseTree() {
  attributesTreeView->collapseAll();
}
 
void AttributesWidget::setTree() {
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE'");
  while (query->next()) {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *father = new QStandardItem(name);    
    attributesTree->appendRow(father);
    father->setToolTip(description);
    father->setEditable(false);
    buildHierarchy(father, name);
  }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
}

void AttributesWidget::buildHierarchy(QStandardItem *top, QString name) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father");
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

void AttributesWidget::finalBusiness() {
  setComment();
}

