#include "../include/AttributesWidget.h"

AttributesWidget::AttributesWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  descriptionFilter = "";
  rawFilter = "";
  commentFilter = "";
  commentBool = false;

  incidentsModel = new QSqlTableModel(this);  
  incidentsModel->setTable("incidents");
  incidentsModel->setSort(1, Qt::AscendingOrder);
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
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  
  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  attributesTreeView->installEventFilter(this);

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
  markButton = new QPushButton("Toggle mark");
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
  editAttributeButton->setEnabled(false);
  assignAttributeButton = new QPushButton("Assign attribute");
  assignAttributeButton->setEnabled(false);
  unassignAttributeButton = new QPushButton("Unassign attribute");
  unassignAttributeButton->setEnabled(false);
  removeUnusedAttributesButton = new QPushButton("Remove unused attributes");
  valueButton = new QPushButton("Store value");
  valueButton->setEnabled(false);
  expandTreeButton = new QPushButton("+");
  collapseTreeButton = new QPushButton("-");
  previousCodedButton = new QPushButton("Previous coded");
  previousCodedButton->setEnabled(false);
  nextCodedButton = new QPushButton("Next coded");
  nextCodedButton->setEnabled(false);
  resetTextsButton = new QPushButton("Reset sources");
  resetTextsButton->setEnabled(false);
  
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
  connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(attributeFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(changeFilter(const QString &)));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(attributesTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(getValue()));
  connect(attributesTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(highlightText()));
  connect(attributesTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(setButtons()));
  connect(attributesTreeView, SIGNAL(noneSelected()), this, SLOT(setButtons()));
  connect(expandTreeButton, SIGNAL(clicked()), this, SLOT(expandTree()));
  connect(collapseTreeButton, SIGNAL(clicked()), this, SLOT(collapseTree()));
  connect(previousCodedButton, SIGNAL(clicked()), this, SLOT(previousCoded()));
  connect(nextCodedButton, SIGNAL(clicked()), this, SLOT(nextCoded()));
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
  descriptionLayoutRight->setContentsMargins(100,0,0,0);
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
  rawLayoutRight->setContentsMargins(170,0,0,0);
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
  commentLayoutRight->setContentsMargins(115,0,0,0);
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
  QPointer<QHBoxLayout> titleLayout = new QHBoxLayout;
  titleLayout->addWidget(attributesLabel);
  QPointer<QHBoxLayout> collapseLayout = new QHBoxLayout;
  collapseLayout->addWidget(expandTreeButton);
  collapseLayout->addWidget(collapseTreeButton);
  titleLayout->addLayout(collapseLayout);
  rightLayout->addLayout(titleLayout);
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
  rightButtonTopLayout->addWidget(assignAttributeButton);
  rightButtonTopLayout->addWidget(unassignAttributeButton);
  rightButtonTopLayout->addWidget(resetTextsButton);
  rightLayout->addLayout(rightButtonTopLayout);
  QPointer<QHBoxLayout> rightButtonMiddleLayout = new QHBoxLayout;
  rightButtonMiddleLayout->addWidget(previousCodedButton);
  rightButtonMiddleLayout->addWidget(nextCodedButton);
  rightLayout->addLayout(rightButtonMiddleLayout);
  QPointer<QHBoxLayout> rightButtonBottomLayout = new QHBoxLayout;
  rightButtonBottomLayout->addWidget(newAttributeButton);
  rightButtonBottomLayout->addWidget(editAttributeButton);
  rightButtonBottomLayout->addWidget(removeUnusedAttributesButton);
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
    delete query;
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
  delete query;
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
  delete indexDialog;
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
  delete query;
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
  delete query;
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
  delete query;
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
  delete query;
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
    delete query;
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
    delete query;
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
    delete query;
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
    delete query;
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
    delete query;
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
    delete query;
  }
}

void AttributesWidget::changeFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void AttributesWidget::newAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString currentParent = treeFilter->mapToSource(attributesTreeView->currentIndex()).data().toString();
    QString name = "";
    QString description = "";
    attributeDialog = new AttributeDialog(this);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *attribute = new QStandardItem(name);    
      attribute->setToolTip(description);
      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
      QStandardItem *father = attributesTree->itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
      father->appendRow(attribute);
      attribute->setToolTip(hint);
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
    attributeDialog = new AttributeDialog(this);
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
      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
      attribute->setToolTip(hint);
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
    attributeDialog = new AttributeDialog(this);
    attributeDialog->submitName(name);
    attributeDialog->setDescription(description);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      QString newName = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
      currentAttribute->setData(newName);
      currentAttribute->setData(newName, Qt::DisplayRole);
      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
      currentAttribute->setToolTip(hint);
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
    delete query;
    delete attributeDialog;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void AttributesWidget::sourceText(const QString &attribute, const int &incident) {
  if (rawField->textCursor().selectedText().trimmed() != "") {
    QSqlQuery *query = new QSqlQuery;
    int end = 0;
    int begin = 0;
    QTextCursor selectCursor = rawField->textCursor();
    if (rawField->textCursor().anchor() >= rawField->textCursor().position()) {
      begin = rawField->textCursor().position();
      end = rawField->textCursor().anchor();
    } else {
      begin = rawField->textCursor().anchor();
      end = rawField->textCursor().position();
    }
    begin++;
    end--;
    
    selectCursor.setPosition(begin);
    selectCursor.movePosition(QTextCursor::StartOfWord);
    selectCursor.setPosition(end, QTextCursor::KeepAnchor);
    selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    rawField->setTextCursor(selectCursor);
    QString sourceText = rawField->textCursor().selectedText().trimmed();
    
    query->prepare("INSERT INTO attributes_to_incidents_sources (attribute, incident, source_text)"
		    "VALUES (:att, :inc, :text)");
    query->bindValue(":att", attribute);
    query->bindValue(":inc", incident);
    query->bindValue(":text", sourceText);
    query->exec();
    delete query;
  }
}

void AttributesWidget::highlightText() {
  QTextCursor currentPos = rawField->textCursor();
  if (attributesTreeView->currentIndex().isValid()) {
    QStandardItem *currentAttribute = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
    QString currentName = attributesTreeView->currentIndex().data().toString();
    QSqlQueryModel *query = new QSqlQueryModel;
    if (currentAttribute->font().bold()) {
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
      QTextCharFormat format;
      format.setFontWeight(QFont::Normal);
      format.setUnderlineStyle(QTextCharFormat::NoUnderline);
      rawField->selectAll();
      rawField->textCursor().mergeCharFormat(format);
      QTextCursor cursor = rawField->textCursor();
      cursor.movePosition(QTextCursor::Start);
      rawField->setTextCursor(cursor);
      query2->prepare("SELECT source_text FROM attributes_to_incidents_sources WHERE attribute = :attribute AND incident = :id");
      query2->bindValue(":attribute", currentName);
      query2->bindValue(":id", id);
      query2->exec();
      while (query2->next()) {
        QString currentText = query2->value(0).toString();
        while (rawField->find(currentText, QTextDocument::FindWholeWords)) {
          format.setFontWeight(QFont::Bold);
          format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
          format.setUnderlineColor(Qt::blue);
          rawField->textCursor().mergeCharFormat(format);
        }
        cursor = rawField->textCursor();
        cursor.movePosition(QTextCursor::Start);
        rawField->setTextCursor(cursor);
      }
      rawField->setTextCursor(currentPos);
      delete query2;
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
      rawField->setTextCursor(currentPos);
    }
    delete query;
  } else {
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setUnderlineStyle(QTextCharFormat::NoUnderline);
    rawField->selectAll();
    rawField->textCursor().mergeCharFormat(format);
    QTextCursor cursor = rawField->textCursor();
    cursor.movePosition(QTextCursor::Start);
    rawField->setTextCursor(cursor);
    rawField->setTextCursor(currentPos);
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
      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents WHERE attribute = :att AND incident = :inc");
      query2->bindValue(":att", attribute);
      query2->bindValue(":inc", id);
      query2->exec();
      query2->first();
      empty = query2->isNull(0);
      QTextCursor cursPos = rawField->textCursor();
      if (empty) {
        assignedModel->insertRow(max);
        assignedModel->setData(assignedModel->index(max, 1), attribute);
        assignedModel->setData(assignedModel->index(max, 2), id);
        assignedModel->submitAll();
	sourceText(attribute, id);
	resetFont(attributesTree);
        query2->exec("SELECT attribute, incident FROM attributes_to_incidents");
        while (query2->next()) {
          QString attribute = query2->value(0).toString();
          int incident = query2->value(1).toInt();
          if (incident == id) {
            boldSelected(attributesTree, attribute);
          }
        }
        highlightText();
	rawField->setTextCursor(cursPos);
	valueButton->setEnabled(true);
      } else {
        sourceText(attribute, id);
        highlightText();
	rawField->setTextCursor(cursPos);
      }
    }
    delete query;
    delete query2;
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
        query2->prepare("DELETE FROM attributes_to_incidents_sources WHERE attribute = :att AND incident = :inc");
        query2->bindValue(":att", attribute);
        query2->bindValue(":inc", id);
        query2->exec();
        assignedModel->select();
        resetFont(attributesTree);
        query2->exec("SELECT attribute, incident FROM attributes_to_incidents");
        while (query2->next()) {
          QString attribute = query2->value(0).toString();
          int incident = query2->value(1).toInt();
          if (incident == id) {
            boldSelected(attributesTree, attribute);
          }
        }
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
        valueField->setText("");
      }
    }
    delete query;
    delete query2;
  }
}

void AttributesWidget::resetTexts() {
  if (attributesTreeView->currentIndex().isValid()) {
    QPointer<QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Resetting source texts cannot be undone. Are you sure you want to proceed?");
    if (warningBox->exec() == QMessageBox::Yes) {
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
        query2->prepare("DELETE FROM attributes_to_incidents_sources WHERE attribute = :att AND incident = :inc");
        query2->bindValue(":att", attribute);
        query2->bindValue(":inc", id);
        query2->exec();
      }
      highlightText();
      delete query;
      delete query2;
    }
    delete warningBox;
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
  delete query;  
  delete query2;
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
    delete query;
    delete query2;
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
      QString value = query2->value(1).toString();
      valueField->setText(value);
    } else {
      valueField->setText("");
    }
    delete query;
    delete query2;
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

  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT id, timestamp, source, description, raw, comment, mark FROM incidents WHERE ch_order = :order");
  query2->bindValue(":order", order);
  query2->exec();
  query2->first();
  int id = query2->value(0).toInt();
  QString timeStamp = query2->value(1).toString();
  QString source = query2->value(2).toString();
  QString description = query2->value(3).toString();
  QString raw = query2->value(4).toString();
  QString comment = query2->value(5).toString();
  int mark = query2->value(6).toInt();
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
  delete query;
  delete query2;
}

void AttributesWidget::expandTree() {
  attributesTreeView->expandAll();
}

void AttributesWidget::collapseTree() {
  attributesTreeView->collapseAll();
}

void AttributesWidget::previousCoded() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery("SELECT * FROM save_data");
    int currentOrder = 0; 
    currentOrder = query->record(0).value("attributes_record").toInt();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT incident, ch_order FROM "
                    "(SELECT incident, ch_order, attribute FROM attributes_to_incidents "
                    "LEFT JOIN incidents ON attributes_to_incidents.incident = incidents.id "
                    "WHERE ch_order < :order AND attribute = :attribute)"
                    "ORDER BY ch_order desc");
    query2->bindValue(":order", currentOrder);
    query2->bindValue(":attribute", attribute);
    query2->exec();
    int id = 0;
    query2->first();
    id = query2->value(0).toInt();
    if (!(query2->isNull(0))) {
      id = query2->value(0).toInt();
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :id");
      query2->bindValue(":id", id);
      query2->exec();
      query2->first();
      int newOrder = query2->value(0).toInt();
      query2->prepare("UPDATE save_data "
                      "SET attributes_record=:new");
      query2->bindValue(":new", newOrder);
      query2->exec();
      retrieveData();
    }
    delete query;
    delete query2;
  }
}

void AttributesWidget::nextCoded() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery("SELECT * FROM save_data");
    int currentOrder = 0; 
    currentOrder = query->record(0).value("attributes_record").toInt();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT incident, ch_order FROM "
                    "(SELECT incident, ch_order, attribute FROM attributes_to_incidents "
                    "LEFT JOIN incidents ON attributes_to_incidents.incident = incidents.id "
                    "WHERE ch_order > :order AND attribute = :attribute)"
                    "ORDER BY ch_order asc");
    query2->bindValue(":order", currentOrder);
    query2->bindValue(":attribute", attribute);
    query2->exec();
    int id = 0;
    query2->first();
    id = query2->value(0).toInt();
    if (!(query2->isNull(0))) {
      id = query2->value(0).toInt();
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :id");
      query2->bindValue(":id", id);
      query2->exec();
      query2->first();
      int newOrder = query2->value(0).toInt();
      query2->prepare("UPDATE save_data "
                      "SET attributes_record=:new");
      query2->bindValue(":new", newOrder);
      query2->exec();
      retrieveData();
    }
    delete query;
    delete query2;
  }
}

void AttributesWidget::setButtons() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString currentAttribute = attributesTreeView->currentIndex().data().toString();
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
      assignedModel->select();
      bool empty = false;
      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents WHERE attribute = :att AND incident = :inc  ");
      query2->bindValue(":att", currentAttribute);
      query2->bindValue(":inc", id);
      query2->exec();
      query2->first();
      empty = query2->isNull(0);
      if (!empty) {
	unassignAttributeButton->setEnabled(true);
	resetTextsButton->setEnabled(true);
      } else {
	unassignAttributeButton->setEnabled(false);
	resetTextsButton->setEnabled(false);
      }
      assignAttributeButton->setEnabled(true);
      previousCodedButton->setEnabled(true);
      nextCodedButton->setEnabled(true);
      editAttributeButton->setEnabled(true);
    }
  } else {
   assignAttributeButton->setEnabled(false);
   previousCodedButton->setEnabled(false);
   nextCodedButton->setEnabled(false);
   editAttributeButton->setEnabled(false);
   unassignAttributeButton->setEnabled(false);
   resetTextsButton->setEnabled(false);
  }
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
    QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
    father->setToolTip(hint);
    father->setEditable(false);
    buildHierarchy(father, name);
  }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
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
    QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
    child->setToolTip(hint);
    child->setEditable(false);
    children++;
    buildHierarchy(child, childName);
  }
  delete query;
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
      if (currentAttribute->parent()) {
	font.setBold(false);
        font.setItalic(true);
	while (currentAttribute->parent()) {
          currentAttribute = currentAttribute->parent();
          currentAttribute->setFont(font);      
        }
      }
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
    font.setItalic(false);
    currentAttribute->setFont(font);
    if (model->hasChildren(index)) {
      resetFont(model, index);
    }
  }
}

void AttributesWidget::fixTree() {
  resetFont(attributesTree);
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
  id = query2->value(0).toInt();
  query2->exec("SELECT attribute, incident FROM attributes_to_incidents");
  while (query2->next()) {
    QString attribute = query2->value(0).toString();
    int incident = query2->value(1).toInt();
    if (incident == id) {
      boldSelected(attributesTree, attribute);
    }
  }
  delete query;
  delete query2;
}

void AttributesWidget::finalBusiness() {
  setComment();
}

bool AttributesWidget::eventFilter(QObject *object, QEvent *event) {
  if (object == attributesTreeView && event->type() == QEvent::ChildRemoved) {
    fixTree();
  }
  return false;
}
