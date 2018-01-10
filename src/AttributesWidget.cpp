#include "../include/AttributesWidget.h"

AttributesWidget::AttributesWidget(QWidget *parent) : QWidget(parent) {

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

  indexLabel = new QLabel("<b>Incident ( / )</b>", this);
  markLabel = new QLabel("", this);
  markLabel->setStyleSheet("QLabel {color: red}");
  timeStampLabel = new QLabel("<b>Timing:</b>", this);
  sourceLabel = new QLabel("<b>Source:</b>", this);
  descriptionLabel = new QLabel("<b>Description:</b>", this);
  rawLabel = new QLabel("<b>Raw:</b>", this);
  commentLabel = new QLabel("<b>Comments:</b>", this);
  attributesLabel = new QLabel("<h2>Attributes</h2>", this);
  descriptionFilterLabel = new QLabel("<i>Search descriptions:</i>", this);
  rawFilterLabel = new QLabel("<i>Search raw texts:</i>", this);
  commentFilterLabel = new QLabel("<i>Search comments:</i>", this);
  attributeFilterLabel = new QLabel("<b>Filter attributes:</b>", this);
  valueLabel = new QLabel("<b>Value:</b>", this);
  
  timeStampField = new QLineEdit(this);
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit(this);
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit(this);
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit(this);
  rawField->setReadOnly(true);
  commentField = new QTextEdit(this);
  commentField->installEventFilter(this);
  descriptionFilterField = new QLineEdit(this);
  rawFilterField = new QLineEdit(this);
  commentFilterField = new QLineEdit(this);
  attributeFilterField = new QLineEdit(this);
  valueField = new QLineEdit(this);
  valueField->setEnabled(false);
  
  previousIncidentButton = new QPushButton("Previous incident", this);
  previousIncidentButton->setStyleSheet("QPushButton {font-weight: bold}");
  nextIncidentButton = new QPushButton("Next incident", this);
  nextIncidentButton->setStyleSheet("QPushButton {font-weight: bold}");
  jumpButton = new QPushButton("Jump to", this);
  markButton = new QPushButton("Toggle mark", this);
  previousMarkedButton = new QPushButton("Previous marked", this);
  nextMarkedButton = new QPushButton("Next marked", this);
  descriptionPreviousButton = new QPushButton("Previous", this);
  descriptionNextButton = new QPushButton("Next", this);
  rawPreviousButton = new QPushButton("Previous", this);
  rawNextButton = new QPushButton("Next", this);
  commentPreviousButton = new QPushButton("Previous", this);
  commentNextButton = new QPushButton("Next", this);
  newAttributeButton = new QPushButton("New attribute", this);
  editAttributeButton = new QPushButton("Edit attribute", this);
  editAttributeButton->setEnabled(false);
  assignAttributeButton = new QPushButton("Assign attribute", this);
  assignAttributeButton->setEnabled(false);
  unassignAttributeButton = new QPushButton("Unassign attribute", this);
  unassignAttributeButton->setEnabled(false);
  removeUnusedAttributesButton = new QPushButton("Remove unused attributes", this);
  valueButton = new QPushButton("Store value", this);
  valueButton->setEnabled(false);
  expandTreeButton = new QPushButton("+", this);
  collapseTreeButton = new QPushButton("-", this);
  previousCodedButton = new QPushButton("Previous coded", this);
  previousCodedButton->setEnabled(false);
  nextCodedButton = new QPushButton("Next coded", this);
  nextCodedButton->setEnabled(false);
  removeTextButton = new QPushButton("Remove text", this);
  removeTextButton->setEnabled(false);
  resetTextsButton = new QPushButton("Reset texts", this);
  resetTextsButton->setEnabled(false);
  
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(previousIncidentButton, SIGNAL(clicked()), this, SLOT(previousIncident()));
  connect(nextIncidentButton, SIGNAL(clicked()), this, SLOT(nextIncident()));
  connect(jumpButton, SIGNAL(clicked()), this, SLOT(jumpIncident()));
  connect(markButton, SIGNAL(clicked()), this, SLOT(toggleMark()));
  connect(previousMarkedButton, SIGNAL(clicked()), this, SLOT(previousMarked()));
  connect(nextMarkedButton, SIGNAL(clicked()), this, SLOT(nextMarked()));
  connect(descriptionFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setDescriptionFilter(const QString &)));
  connect(descriptionPreviousButton, SIGNAL(clicked()), this, SLOT(previousDescription()));
  connect(descriptionNextButton, SIGNAL(clicked()), this, SLOT(nextDescription()));
  connect(rawFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setRawFilter(const QString &)));
  connect(rawPreviousButton, SIGNAL(clicked()), this, SLOT(previousRaw()));
  connect(rawNextButton, SIGNAL(clicked()), this, SLOT(nextRaw()));
  connect(commentFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setCommentFilter(const QString &)));
  connect(commentPreviousButton, SIGNAL(clicked()), this, SLOT(previousComment()));
  connect(commentNextButton, SIGNAL(clicked()), this, SLOT(nextComment()));
  connect(newAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(attributeFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(valueField, SIGNAL(textChanged(const QString &)), this, SLOT(setValueButton()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(getValue()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(highlightText()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
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
  topFieldsLayout->addSpacerItem(new QSpacerItem(20, 0));
  topFieldsLayout->addWidget(sourceLabel);
  topFieldsLayout->addWidget(sourceField);
  leftLayout->addLayout(topFieldsLayout);
  QPointer<QHBoxLayout> descriptionLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> descriptionLayoutLeft = new QHBoxLayout;
  descriptionLayoutLeft->addWidget(descriptionLabel);
  descriptionLayout->addLayout(descriptionLayoutLeft);
  QPointer<QHBoxLayout> descriptionLayoutRight = new QHBoxLayout;
  descriptionLayoutRight->addWidget(descriptionFilterLabel);
  descriptionFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  descriptionLayoutRight->addWidget(descriptionPreviousButton);
  descriptionPreviousButton->setMaximumWidth(descriptionPreviousButton->sizeHint().width());
  descriptionLayoutRight->addWidget(descriptionFilterField);
  descriptionFilterField->setFixedWidth(250);
  descriptionLayoutRight->addWidget(descriptionNextButton);
  descriptionNextButton->setMaximumWidth(descriptionNextButton->sizeHint().width());
  descriptionLayout->addLayout(descriptionLayoutRight);
  leftLayout->addLayout(descriptionLayout);
  leftLayout->addWidget(descriptionField);
  QPointer<QHBoxLayout> rawLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> rawLayoutLeft = new QHBoxLayout;
  rawLayoutLeft->addWidget(rawLabel);
  rawLayout->addLayout(rawLayoutLeft);
  QPointer<QHBoxLayout> rawLayoutRight = new QHBoxLayout;
  rawLayoutRight->addWidget(rawFilterLabel);
  rawFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  rawLayoutRight->addWidget(rawPreviousButton);
  rawPreviousButton->setMaximumWidth(rawPreviousButton->sizeHint().width());
  rawLayoutRight->addWidget(rawFilterField);
  rawFilterField->setFixedWidth(250);
  rawLayoutRight->addWidget(rawNextButton);
  rawNextButton->setMaximumWidth(rawNextButton->sizeHint().width());
  rawLayout->addLayout(rawLayoutRight);
  leftLayout->addLayout(rawLayout);
  leftLayout->addWidget(rawField);
  QPointer<QHBoxLayout> commentLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> commentLayoutLeft = new QHBoxLayout;
  commentLayoutLeft->addWidget(commentLabel);
  commentLayout->addLayout(commentLayoutLeft);
  QPointer<QHBoxLayout> commentLayoutRight = new QHBoxLayout;
  commentLayoutRight->addWidget(commentFilterLabel);
  commentFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  commentLayoutRight->addWidget(commentPreviousButton);
  commentPreviousButton->setMaximumWidth(commentPreviousButton->sizeHint().width());
  commentLayoutRight->addWidget(commentFilterField);
  commentFilterField->setFixedWidth(250);
  commentLayoutRight->addWidget(commentNextButton);
  commentNextButton->setMaximumWidth(commentNextButton->sizeHint().width());
  commentLayout->addLayout(commentLayoutRight);
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
  titleLayout->addSpacerItem(new QSpacerItem(20, 0));
  titleLayout->addWidget(attributesLabel);
  QPointer<QHBoxLayout> collapseLayout = new QHBoxLayout;
  collapseLayout->addWidget(expandTreeButton);
  expandTreeButton->setMaximumWidth(expandTreeButton->sizeHint().width());
  collapseLayout->addWidget(collapseTreeButton);
  collapseTreeButton->setMaximumWidth(collapseTreeButton->sizeHint().width());
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
  rightButtonTopLayout->addWidget(removeTextButton);
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
  indexDialog->exec();
  int order = 0;
  if (indexDialog->getExitStatus() == 0) {
    order = indexDialog->getIndex();
    QSqlQuery *query = new QSqlQuery;
    if (order > 0) {
      query->prepare("UPDATE save_data "
                     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
    delete query;
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
  query->prepare("SELECT ch_order FROM incidents "
		 "WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
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
    attributeDialog = new AttributeDialog(this, INCIDENT);
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
      eventGraph->resetTree();
    }
    delete attributeDialog;
  } else {
    QString name = "";
    QString description = "";
    attributeDialog = new AttributeDialog(this, INCIDENT);
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
      QString hint = breakString(description);
      attribute->setToolTip(hint);
      attribute->setEditable(false);
      eventGraph->resetTree();
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
    attributeDialog = new AttributeDialog(this, INCIDENT);
    attributeDialog->submitName(name);
    attributeDialog->setDescription(description);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      QString newName = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *currentAttribute = attributesTree->
	itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
      currentAttribute->setData(newName);
      currentAttribute->setData(newName, Qt::DisplayRole);
      QString hint = breakString(description);
      currentAttribute->setToolTip(hint);
      query->prepare("UPDATE incident_attributes "
		     "SET name = :newname, description = :newdescription "
		     "WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdescription", description);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE incident_attributes "
		     "SET father = :newname "
		     "WHERE father = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_incidents "
		     "SET attribute = :newname "
		     "WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_incidents_sources "
		     "SET attribute = :newname "
		     "WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE saved_eg_plots_attributes_to_macro_events "
		     "SET attribute = :newname "
		     "WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      this->setCursor(Qt::WaitCursor);
      retrieveData();
      this->setCursor(Qt::ArrowCursor);
      eventGraph->resetTree();
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
    QStandardItem *currentAttribute = attributesTree->
      itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
    QString currentName = attributesTreeView->currentIndex().data().toString();
    QSqlQueryModel *query = new QSqlQueryModel(this);
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
      query2->prepare("SELECT source_text "
		      "FROM attributes_to_incidents_sources "
		      "WHERE attribute = :attribute AND incident = :id");
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
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    query->first();
    int order = 0; 
    order = query->value(0).toInt();
    query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
    query->bindValue(":order", order);
    query->exec();
    query->first();
    if (!(query->isNull(0))) {
      int id = 0;
      id = query->value(0).toInt();
      QString attribute = attributesTreeView->currentIndex().data().toString();
      bool empty = false;
      query->prepare("SELECT attribute, incident "
		      "FROM attributes_to_incidents "
		      "WHERE attribute = :att AND incident = :inc");
      query->bindValue(":att", attribute);
      query->bindValue(":inc", id);
      query->exec();
      query->first();
      empty = query->isNull(0);
      QTextCursor cursPos = rawField->textCursor();
      if (empty) {
	query->prepare("INSERT INTO attributes_to_incidents "
		       "(attribute, incident) "
		       "VALUES (:attribute, :incident)");
	query->bindValue(":attribute", attribute);
	query->bindValue(":incident", id);
	query->exec();
	sourceText(attribute, id);
	boldSelected(attributesTree, attribute);
        highlightText();
	rawField->setTextCursor(cursPos);
	valueField->setEnabled(true);
      } else {
        sourceText(attribute, id);
        highlightText();
	rawField->setTextCursor(cursPos);
      }
      setButtons();
    }
    delete query;
  }
  occurrenceGraph->checkCongruency();
}

void AttributesWidget::unassignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQueryModel *query = new QSqlQueryModel(this);
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
      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents "
		      "WHERE attribute = :att AND incident = :inc  ");
      query2->bindValue(":att", attribute);
      query2->bindValue(":inc", id);
      query2->exec();
      query2->first();
      empty = query2->isNull(0);
      if (!empty) {
        query2->prepare("DELETE FROM attributes_to_incidents "
			"WHERE attribute = :att AND incident = :inc");
        query2->bindValue(":att", attribute);
        query2->bindValue(":inc", id);
        query2->exec();
        query2->prepare("DELETE FROM attributes_to_incidents_sources "
			"WHERE attribute = :att AND incident = :inc");
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
	valueField->setText("");
	valueField->setEnabled(false);
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
    delete query;
    delete query2;
  }
  occurrenceGraph->checkCongruency();
}

void AttributesWidget::removeText() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    query->first();
    int order = 0; 
    order = query->value(0).toInt();
    query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
    query->bindValue(":order", order);
    query->exec();
    query->first();
    int id = 0;
    id = query->value(0).toInt();
    QString attribute = attributesTreeView->currentIndex().data().toString();
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
      query->prepare("DELETE FROM attributes_to_incidents_sources "
		     "WHERE attribute = :att AND incident = :inc AND source_text = :text");
      query->bindValue(":att", attribute);
      query->bindValue(":inc", id);
      query->bindValue(":text", sourceText);
      query->exec();
    }
    setButtons();
    highlightText();
    delete query;
  }
}

void AttributesWidget::resetTexts() {
  if (attributesTreeView->currentIndex().isValid()) {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Resetting source texts cannot be undone. "
				   "Are you sure you want to proceed?");
    if (warningBox->exec() == QMessageBox::Yes) {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0; 
      order = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int id = 0;
      if (!(query->isNull(0))) {
        id = query->value(0).toInt();
        QString attribute = attributesTreeView->currentIndex().data().toString();
        query->prepare("DELETE FROM attributes_to_incidents_sources "
		       "WHERE attribute = :att AND incident = :inc");
        query->bindValue(":att", attribute);
        query->bindValue(":inc", id);
        query->exec();
      }
      highlightText();
      delete query;
    }
    setButtons();
    delete warningBox;
  }
}

void AttributesWidget::removeUnusedAttributes() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  QVector<MacroEvent*> macroVector = eventGraph->getMacros();
  QSet<QString> takenAttributes;
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) {
    MacroEvent* current = it.next();
    QSet<QString> attributes = current->getAttributes();
    QSet<QString>::iterator it2;
    for (it2 = attributes.begin(); it2 != attributes.end(); it2++) {
      takenAttributes.insert(*it2);
    }
  }
  while (unfinished) {
    query->exec("SELECT name FROM incident_attributes "
		"EXCEPT SELECT attribute FROM attributes_to_incidents "
		"EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_macro_events "
		"EXCEPT SELECT father FROM incident_attributes");
    QSet<QString> temp;
    while (query->next()) {
      QString current = query->value(0).toString();			   
      temp.insert(current);
    }
    QSet<QString>::iterator it3;
    bool found = false;
    for (it3 = temp.begin(); it3 != temp.end(); it3++) {
      if (!takenAttributes.contains(*it3)) {
	found = true;
	query2->prepare("DELETE FROM incident_attributes WHERE name = :current");
	query2->bindValue(":current", *it3);
	query2->exec();
      }
    }
    if (!found) {
      unfinished = false;
    }
  }
  this->setCursor(Qt::WaitCursor);
  attributesTreeView->setSortingEnabled(false);
  resetTree();
  eventGraph->resetTree();
  occurrenceGraph->checkCongruency();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
  delete query;  
  delete query2;
}

void AttributesWidget::setValueButton() {
  valueButton->setEnabled(true);
}

void AttributesWidget::setValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQueryModel *query = new QSqlQueryModel(this);
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
    delete query;
    delete query2;
    valueButton->setEnabled(false);
  }
}

void AttributesWidget::getValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQueryModel *query = new QSqlQueryModel(this);
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
    query2->prepare("SELECT attribute, value FROM attributes_to_incidents "
		    "WHERE incident = :id AND attribute =:att");
    query2->bindValue(":id", id);
    query2->bindValue(":att", attribute);
    query2->exec();
    query2->first();
    if (!(query2->isNull(0))) {
      valueField->setEnabled(true);
    } else {
      valueField->setEnabled(false);
    }
    if (!(query2->isNull(1))) {
      QString value = query2->value(1).toString();
      valueField->setText(value);
    } else {
      valueField->setText("");
    }
    valueButton->setEnabled(false);
    delete query;
    delete query2;
  } else {
    valueField->setText("");
    valueButton->setEnabled(false);
  }
}

void AttributesWidget::retrieveData() {
  QSqlQueryModel *query = new QSqlQueryModel(this);
  query->setQuery("SELECT * FROM save_data");
  int order = 0;
  order = query->record(0).value("attributes_record").toInt();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  int total = incidentsModel->rowCount();

  QString indexText = "<b>Incident (" + QString::number(order) + " / " +
    QString::number(total) + ")<b>";
  indexLabel->setText(indexText);
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT id, timestamp, source, description, raw, comment, mark FROM "
		  "incidents WHERE ch_order = :order");
  query2->bindValue(":order", order);
  query2->exec();
  query2->first();
  if (!(query2->isNull(0))) {
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
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    query->first();
    int currentOrder = query->value(0).toInt();
    QVector<QString> attributeVector;
    attributeVector.push_back(attribute);
    findChildren(attribute, &attributeVector);
    QVectorIterator<QString> it(attributeVector);
    int order = -1;
    while (it.hasNext()) {
      QString currentAttribute = it.next();
      query->prepare("SELECT ch_order FROM "
		      "(SELECT incident, ch_order, attribute FROM attributes_to_incidents "
		      "LEFT JOIN incidents ON attributes_to_incidents.incident = incidents.id "
		      "WHERE ch_order < :order AND attribute = :attribute)"
		      "ORDER BY ch_order desc");
      query->bindValue(":order", currentOrder);
      query->bindValue(":attribute", currentAttribute);
      query->exec();
      query->first();
      if (order == -1 && !(query->isNull(0))) {
	order = query->value(0).toInt();
      } else if (!(query->isNull(0)) && query->value(0).toInt() > order) {
	order = query->value(0).toInt();
      }
      
    }
    if (order != -1) {
      query->prepare("UPDATE save_data "
		      "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
    delete query;
  }
}

void AttributesWidget::nextCoded() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    query->first();
    int currentOrder = query->value(0).toInt();
    QVector<QString> attributeVector;
    attributeVector.push_back(attribute);
    findChildren(attribute, &attributeVector);
    QVectorIterator<QString> it(attributeVector);
    int order = -1;
    while (it.hasNext()) {
      QString currentAttribute = it.next();
      query->prepare("SELECT ch_order FROM "
		      "(SELECT incident, ch_order, attribute FROM attributes_to_incidents "
		      "LEFT JOIN incidents ON attributes_to_incidents.incident = incidents.id "
		      "WHERE ch_order > :order AND attribute = :attribute)"
		      "ORDER BY ch_order asc");
      query->bindValue(":order", currentOrder);
      query->bindValue(":attribute", currentAttribute);
      query->exec();
      query->first();
      if (order == -1 && !(query->isNull(0))) {
	order = query->value(0).toInt();
      } else if (!(query->isNull(0)) && query->value(0).toInt() < order) {
	order = query->value(0).toInt();
      }
    }
    if (order != -1) {
      query->prepare("UPDATE save_data "
                      "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
    delete query;
  }
}

void AttributesWidget::findChildren(QString father, QVector<QString> *children) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM incident_attributes WHERE father = :father");
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) {
    QString currentChild = query->value(0).toString();
    children->push_back(currentChild);
    findChildren(currentChild, children);
  }
  delete query;
}

void AttributesWidget::setButtons() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString currentAttribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT attributes_record FROM save_data");
    query->first();
    int order = 0;
    order = query->value(0).toInt();
    query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
    query->bindValue(":order", order);
    query->exec();
    query->first();
    if (!(query->isNull(0))) {
      int id = query->value(0).toInt();
      assignedModel->select();
      bool empty = false;
      query->prepare("SELECT attribute, incident FROM "
		     "attributes_to_incidents "
		     "WHERE attribute = :att AND incident = :inc  ");
      query->bindValue(":att", currentAttribute);
      query->bindValue(":inc", id);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	unassignAttributeButton->setEnabled(true);
      } else {
	unassignAttributeButton->setEnabled(false);
      }
      query->prepare("SELECT attribute, incident FROM "
		     "attributes_to_incidents_sources "
		     "WHERE attribute = :att AND incident = :inc");
      query->bindValue(":att", currentAttribute);
      query->bindValue(":inc", id);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	removeTextButton->setEnabled(true);
	resetTextsButton->setEnabled(true);
      } else {
	removeTextButton->setEnabled(false);
	resetTextsButton->setEnabled(false);
      }
      assignAttributeButton->setEnabled(true);
      previousCodedButton->setEnabled(true);
      nextCodedButton->setEnabled(true);
      editAttributeButton->setEnabled(true);
    }
    delete query;
  } else {
   assignAttributeButton->setEnabled(false);
   previousCodedButton->setEnabled(false);
   nextCodedButton->setEnabled(false);
   editAttributeButton->setEnabled(false);
   unassignAttributeButton->setEnabled(false);
   removeTextButton->setEnabled(false);
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
    QString hint = breakString(description);
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
    QString hint = breakString(description);
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
    font.setBold(true);
    QFont font2;
    font2.setItalic(true);
    QFont font3;
    font3.setBold(true);
    font3.setItalic(true);
    if (name == currentName) {
      if (currentAttribute->font().italic()) {
	currentAttribute->setFont(font3);
      } else {
	currentAttribute->setFont(font);
      }
      if (currentAttribute->parent()) {
	while (currentAttribute->parent()) {
          currentAttribute = currentAttribute->parent();
	  QString parentName = currentAttribute->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->exec("SELECT attributes_record FROM save_data");
	  query->first();
	  int order = query->value(0).toInt();
	  query->prepare("SELECT id FROM incidents WHERE ch_order = :order");
	  query->bindValue(":order", order);
	  query->exec();
	  query->first();
	  int incident = query->value(0).toInt();
	  query->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			 "WHERE attribute = :attribute AND incident = :incident");
	  query->bindValue(":attribute", parentName);
	  query->bindValue(":incident", incident);
	  query->exec();
	  query->first();
	  if (query->isNull(0)) {
	    currentAttribute->setFont(font2);      
	  } else {
	    currentAttribute->setFont(font3);
	  }
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
  QSqlQueryModel *query = new QSqlQueryModel(this);
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
  } else if (event->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = (QWheelEvent*) event;
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
    if (textEdit) {
      if(wheelEvent->modifiers() & Qt::ControlModifier) {
        if (wheelEvent->angleDelta().y() > 0) {
	  textEdit->zoomIn(1);
	} else if (wheelEvent->angleDelta().y() < 0) {
	  textEdit->zoomOut(1);
	}
      }
    }
  }
  return false;
}

void AttributesWidget::setEventGraph(EventGraphWidget *egw) {
  eventGraph = egw;
}

void AttributesWidget::setOccurrenceGraph(OccurrenceGraphWidget *ogw) {
  occurrenceGraph = ogw;
}

void AttributesWidget::resetTree() {
  delete attributesTree;
  setTree();
}
