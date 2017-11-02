#include "../include/RelationshipsWidget.h"

RelationshipsWidget::RelationshipsWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  descriptionFilter = "";
  rawFilter = "";
  commentFilter = "";
  commentBool = false;

  incidentsModel = new QSqlTableModel(this);  
  incidentsModel->setTable("incidents");
  incidentsModel->select();

  typeModel = new QSqlTableModel(this);
  typeModel->setTable("relationship_types");
  typeModel->select();
 
  relationshipsModel = new QSqlTableModel(this);
  relationshipsModel->setTable("entity_relationships");
  relationshipsModel->select();

  assignedModel = new QSqlTableModel(this);
  assignedModel->setTable("relationships_to_incidents");
  assignedModel->select();

  relationshipsTreeView = new DeselectableTreeView(this);
  relationshipsTreeView->setHeaderHidden(true);
  relationshipsTreeView->setDragEnabled(true);
  relationshipsTreeView->setAcceptDrops(true);
  relationshipsTreeView->setDropIndicatorShown(true);
  relationshipsTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  relationshipsTreeView->setExpandsOnDoubleClick(false);
  relationshipsTreeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  relationshipsTreeView->header()->setStretchLastSection(false);
  relationshipsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  treeFilter = new RelationshipTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  relationshipsTreeView->setSortingEnabled(true);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);

  indexLabel = new QLabel("<b>Incident ( / )</b>");
  markLabel = new QLabel();
  markLabel->setStyleSheet("QLabel {color: red}");
  timeStampLabel = new QLabel("<b>Timing:</b>");
  sourceLabel = new QLabel("<b>Source:</b>");
  descriptionLabel = new QLabel("<b>Description:</b>");
  rawLabel = new QLabel("<b>Raw:</b>");
  commentLabel = new QLabel("<b>Comments:</b>");
  relationshipsLabel = new QLabel("<h2>Relationships</h2>");
  descriptionFilterLabel = new QLabel("<i>Search descriptions:</i>");
  rawFilterLabel = new QLabel("<i>Search raw texts:</i>");
  commentFilterLabel = new QLabel("<i>Search comments:</i>");
  relationshipFilterLabel = new QLabel("<b>Filter relationships:</b>");

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
  relationshipFilterField = new QLineEdit();

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
  newTypeButton = new QPushButton("Add relationship type");
  editTypeButton = new QPushButton("Edit relationship type");
  removeUnusedRelationshipsButton = new QPushButton("Removed unused relationships");
  newRelationshipButton = new QPushButton("Add relationship");
  expandTreeButton = new QPushButton("Expand");
  collapseTreeButton = new QPushButton("Collapse");

  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(previousIncidentButton, SIGNAL(clicked()), this, SLOT(previousIncident()));
  connect(nextIncidentButton, SIGNAL(clicked()), this, SLOT(nextIncident()));
  // connect(jumpButton, SIGNAL(clicked()), this, SLOT(jumpIncident()));
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
  connect(relationshipFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(changeFilter(const QString &)));
  connect(newTypeButton, SIGNAL(clicked()), this, SLOT(newType()));
  connect(editTypeButton, SIGNAL(clicked()), this, SLOT(editType()));
  connect(removeUnusedRelationshipsButton, SIGNAL(clicked()), this, SLOT(removeUnusedRelationships()));
  //connect(relationshipsTreeView, SIGNAL(selectionChanged()), this, SLOT(highlightText()));
  //connect(relationshipsTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(decideRelationshipsAction()));
  connect(newRelationshipButton, SIGNAL(clicked()), this, SLOT(newRelationship()));
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
  rightLayout->addWidget(relationshipsLabel);
  rightLayout->addWidget(relationshipsTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(relationshipFilterLabel);
  filterLayout->addWidget(relationshipFilterField);
  rightLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> rightButtonTopLayout = new QHBoxLayout;
  rightButtonTopLayout->addWidget(newTypeButton);
  rightButtonTopLayout->addWidget(editTypeButton);
  rightButtonTopLayout->addWidget(expandTreeButton);
  rightButtonTopLayout->addWidget(collapseTreeButton);
  QPointer<QHBoxLayout> rightButtonBottomLayout = new QHBoxLayout;
  rightButtonBottomLayout->addWidget(newRelationshipButton);
  rightButtonBottomLayout->addWidget(removeUnusedRelationshipsButton);
  rightLayout->addLayout(rightButtonTopLayout);
  rightLayout->addLayout(rightButtonBottomLayout);
  mainLayout->addLayout(rightLayout);

  setLayout(mainLayout);
}

void RelationshipsWidget::setCommentBool() {
  commentBool = true;
}

void RelationshipsWidget::setComment() {
  if (commentBool) {
    QString comment = commentField->toPlainText();
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT relationships_record FROM save_data");
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

void RelationshipsWidget::retrieveData() {
  QSqlQueryModel *query = new QSqlQueryModel;
  query->setQuery("SELECT * FROM save_data");
  int order = 0;
  order = query->record(0).value("relationships_record").toInt();
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
  resetFont(relationshipsTree);
  QSqlQuery *query2 = new QSqlQuery;
  query2->exec("SELECT relationship, incident FROM relationships_to_incidents");
  while (query2->next()) {
    QString relationship = query2->value(0).toString();
    int incident = query2->value(1).toInt();
    if (incident == id) {
      boldSelected(relationshipsTree, relationship);
    }
  }
  relationshipsTreeView->setModel(treeFilter);
  relationshipsTreeView->resetSelection();
  relationshipsTree->sort(0, Qt::AscendingOrder);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
  QTextCharFormat format;
  format.setFontWeight(QFont::Normal);
  format.setUnderlineStyle(QTextCharFormat::NoUnderline);
  rawField->selectAll();
  rawField->textCursor().mergeCharFormat(format);
  QTextCursor cursor = rawField->textCursor();
  cursor.movePosition(QTextCursor::Start);
  rawField->setTextCursor(cursor);
}

void RelationshipsWidget::changeFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void RelationshipsWidget::newType() {
  typeDialog = new RelationshipTypeDialog(this);
  typeDialog->exec();
  if (typeDialog->getExitStatus() == 0) {
    QString name = typeDialog->getName();
    QString description = typeDialog->getDescription();
    QString directedness = typeDialog->getDirectedness();
    QStandardItem *type = new QStandardItem(name);
    QString hint =  "<FONT SIZE = 3>" + directedness + " - " + description + "<FONT SIZE = 3>";
    type->setToolTip(hint);
    type->setEditable(false);
    relationshipsTree->appendRow(type);
    int newIndex = typeModel->rowCount();
    typeModel->insertRow(newIndex);
    typeModel->setData(typeModel->index(newIndex, 1), name);
    typeModel->setData(typeModel->index(newIndex, 2), directedness);
    typeModel->setData(typeModel->index(newIndex, 3), description);
    typeModel->submitAll();
  }
  delete typeDialog;
  relationshipsTree->sort(0, Qt::AscendingOrder);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void RelationshipsWidget::editType() {
  if (relationshipsTreeView->currentIndex().isValid()) {
    QStandardItem *currentItem = relationshipsTree->itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
    QString currentName = relationshipsTreeView->currentIndex().data().toString();
    // Check if this is a type, and not a relationship
    if (!currentItem->parent()) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT directedness, description FROM relationship_types WHERE name = :name");
      query->bindValue(":name", currentName);
      query->exec();
      query->first();
      QString directedness = query->value(0).toString();
      QString description = query->value(1).toString();
      typeDialog = new RelationshipTypeDialog(this);
      typeDialog->submitName(currentName);
      typeDialog->submitDescription(description);
      typeDialog->submitDirectedness(directedness);
      typeDialog->exec();
      if (typeDialog->getExitStatus() == 0) {
	QString newName = typeDialog->getName();
	description = typeDialog->getDescription();
	directedness = typeDialog->getDirectedness();
	QStandardItem *currentType = relationshipsTree->itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
	currentType->setData(newName);
	currentType->setData(newName, Qt::DisplayRole);      
	currentType->setToolTip(description);
	query->prepare("UPDATE relationship_types SET name = :newname, "
		       "directedness = :newdirectedness, description = :newdescription "
		       "WHERE name = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":newdirectedness", directedness);
	query->bindValue(":newdescription", description);
	query->bindValue(":oldname", currentName);
	query->exec();
	query->prepare("UPDATE entity_relationships SET type = :newtype WHERE type = :oldtype");
	query->bindValue(":newtype", newName);
	query->bindValue(":oldtype", currentName);
	query->exec();
	this->setCursor(Qt::WaitCursor);
	retrieveData();
	this->setCursor(Qt::ArrowCursor);
      }
    }
    relationshipsTree->sort(0, Qt::AscendingOrder);
    relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
  }
}

void RelationshipsWidget::newRelationship() {
  RelationshipsDialog *relationshipsDialog = new RelationshipsDialog;
  relationshipsDialog->exec();
  delete relationshipsDialog;
}

void RelationshipsWidget::removeUnusedRelationships() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->exec("SELECT name FROM entity_relationships EXCEPT SELECT relationship "
	      "FROM relationships_to_incidents");
  while (query->next()) {
    QString current = query->value(0).toString();
    query2->prepare("DELETE FROM entity_relationships WHERE name = :current");
    query2->bindValue(":current", current);
    query2->exec();
  }
  query->exec("SELECT name FROM relationship_types EXCEPT SELECT type "
  	      "FROM entity_relationships");
  while(query->next()) {
    QString current = query->value(0).toString();
    query2->prepare("DELETE FROM relationship_types WHERE name = :current");
    query2->bindValue(":current", current);
    query2->exec();
  }
  this->setCursor(Qt::WaitCursor);
  relationshipsTreeView->setSortingEnabled(false);
  setTree();
  relationshipsTreeView->setSortingEnabled(true);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
}

void RelationshipsWidget::setTree() {
  relationshipsTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, directedness, description FROM relationship_types");
  while (query->next()) {
    QString currentType = query->value(0).toString();
    QString currentDirection = query->value(1).toString();
    QString typeDescription = query->value(2).toString();
    QStandardItem *type = new QStandardItem(currentType);    
    relationshipsTree->appendRow(type);
    QString hint =  "<FONT SIZE = 3>" + currentDirection + " - " + typeDescription + "<FONT SIZE = 3>";
    type->setToolTip(hint);
    type->setEditable(false);
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT name, comment FROM entity_relationships WHERE type = :type");
    query2->bindValue(":type", currentType);
    query2->exec();
    int children = 0;
    while (query2->next()) {
      QString name = query2->value(0).toString();
      QString comment = query2->value(1).toString();
      QStandardItem *relationship = new QStandardItem(name);
      type->setChild(children, relationship);
      relationship->setToolTip(comment);
      relationship->setEditable(false);
      children++;
    }
  }
  treeFilter->setSourceModel(relationshipsTree);
  relationshipsTreeView->setModel(treeFilter);
}

void RelationshipsWidget::previousIncident() {
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  if (order != 1) {
    query->prepare("UPDATE save_data "
		   "SET relationships_record=:new");
    query->bindValue(":new", order - 1);
    query->exec();
    retrieveData();
  }  
}

void RelationshipsWidget::nextIncident() {
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order != incidentsModel->rowCount()) {
    query->prepare("UPDATE save_data "
		   "SET relationships_record=:new");
    query->bindValue(":new", order + 1);
    query->exec();
    retrieveData();
  }
}

void RelationshipsWidget::toggleMark() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
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


void RelationshipsWidget::previousMarked() {
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::nextMarked() {
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::setDescriptionFilter(const QString &text) {
  descriptionFilter = text;
}

void RelationshipsWidget::previousDescription() {
  setComment();
  if (descriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::nextDescription() {
  setComment();
  if (descriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::setRawFilter(const QString &text) {
  rawFilter = text;
}

void RelationshipsWidget::previousRaw() {
  setComment();
  if (rawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::nextRaw() {
  setComment();
  if (rawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::setCommentFilter(const QString &text) {
  commentFilter = text;
}

void RelationshipsWidget::previousComment() {
  setComment();
  if (commentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::nextComment() {
  setComment();
  if (commentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT relationships_record FROM save_data");
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
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  }
}

void RelationshipsWidget::boldSelected(QAbstractItemModel *model, QString name, QModelIndex parent) {
  for(int i = 0; i != model->rowCount(parent); i++) {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QStandardItem *currentRelationship = relationshipsTree->itemFromIndex(index);
    QFont font;
    if (name == currentName) {
      font.setBold(true);
      currentRelationship->setFont(font);
    }
    if (model->hasChildren(index)) {
      boldSelected(model, name, index);
    }
  }
}

void RelationshipsWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) {
  for(int i = 0; i != model->rowCount(parent); i++) {
    QModelIndex index = model->index(i, 0, parent);
    QString currentName = model->data(index).toString();
    QStandardItem *currentRelationship = relationshipsTree->itemFromIndex(index);
    QFont font;
    font.setBold(false);
    currentRelationship->setFont(font);
    if (model->hasChildren(index)) {
      resetFont(model, index);
    }
  }
}

void RelationshipsWidget::expandTree() {
  relationshipsTreeView->expandAll();
}

void RelationshipsWidget::collapseTree() {
  relationshipsTreeView->collapseAll();
}


void RelationshipsWidget::finalBusiness() {
  setComment();
}


