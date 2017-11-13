#include "../include/RelationshipsDialog.h"

RelationshipsDialog::RelationshipsDialog(QWidget *parent) : QDialog(parent) {
  // First we declare the entities of this dialog.
  tailLabel = new QLabel(DIRECTEDTAIL, this);
  headLabel = new QLabel(DIRECTEDHEAD, this);
  selectedSourceLabel = new QLabel(DEFAULT, this);
  selectedTargetLabel = new QLabel(DEFAULT, this);
  name = "";
  oldName = "";
  type = "";
  exitStatus = 1;
  
  sourceLabel = new QLabel(tr("Source filter:"), this);
  typeLabel = new QLabel("", this);
  entitiesTable = new EntityTableModel(this);
  entitiesTable->setTable("entities");
  entitiesTable->setSort(1, Qt::AscendingOrder);
  entitiesTable->select();
  entitiesFilter = new QSortFilterProxyModel(this);
  entitiesFilter->setSourceModel(entitiesTable);
  entitiesFilter->setFilterKeyColumn(1);
  entitiesFilter->setDynamicSortFilter(true);
  entitiesView = new QListView(this);
  entitiesView->setModel(entitiesFilter);
  entitiesView->setModelColumn(1);
  entitiesView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    
  assignLeftEntityButton = new QPushButton("use selected", this);
  assignRightEntityButton = new QPushButton("use selected", this);
  
  entityFilterField = new QLineEdit();
  newEntityButton = new QPushButton(tr("Define new entity"), this);
  editEntityButton = new QPushButton(tr("Edit highlighted entity"), this);
  editLeftAssignedEntityButton = new QPushButton(tr("Edit left assigned entity"), this);
  editRightAssignedEntityButton = new QPushButton(tr("Edit right assigned entity"), this);
  removeEntitiesButton = new QPushButton(tr("Remove unused entities"), this);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  resetButton = new QPushButton(tr("Reset"), this);
  saveCloseButton = new QPushButton(tr("Save relationship"), this);
  
  //connect(sourceListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editEntity()));
  //connect(targetListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editEntity()));
  connect(entityFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(filterEntity(const QString &)));
  connect(assignLeftEntityButton, SIGNAL(clicked()), this, SLOT(assignLeftEntity()));
  connect(assignRightEntityButton, SIGNAL(clicked()), this, SLOT(assignRightEntity()));
  connect(newEntityButton, SIGNAL(clicked()), this, SLOT(addEntity()));
  connect(editEntityButton, SIGNAL(clicked()), this, SLOT(editEntity()));
  connect(editLeftAssignedEntityButton, SIGNAL(clicked()), this, SLOT(editLeftAssignedEntity()));
  connect(editRightAssignedEntityButton, SIGNAL(clicked()), this, SLOT(editRightAssignedEntity()));
  connect(removeEntitiesButton, SIGNAL(clicked()), this, SLOT(removeEntities()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> labelLayout = new QHBoxLayout;
  labelLayout->addWidget(sourceLabel);
  labelLayout->addWidget(entityFilterField);
  mainLayout->addLayout(labelLayout);
  QPointer<QHBoxLayout> listsLayout = new QHBoxLayout;
  listsLayout->addWidget(entitiesView);
  mainLayout->addLayout(listsLayout);
  QPointer<QHBoxLayout> assignLayout = new QHBoxLayout;
  assignLayout->addWidget(assignLeftEntityButton);
  assignLayout->addWidget(assignRightEntityButton);
  mainLayout->addLayout(assignLayout);
  QPointer<QHBoxLayout> arrowLayout = new QHBoxLayout;
  arrowLayout->addWidget(selectedSourceLabel);
  selectedSourceLabel->setAlignment(Qt::AlignHCenter);
  arrowLayout->addWidget(tailLabel);
  tailLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  arrowLayout->addWidget(typeLabel);
  typeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  arrowLayout->addWidget(headLabel);
  headLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  arrowLayout->addWidget(selectedTargetLabel);
  selectedTargetLabel->setAlignment(Qt::AlignHCenter);
  mainLayout->addLayout(arrowLayout);

  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  QPointer<QVBoxLayout> buttonsLayout = new QVBoxLayout;
  buttonsLayout->addWidget(newEntityButton);
  buttonsLayout->addWidget(editEntityButton);
  buttonsLayout->addWidget(editLeftAssignedEntityButton);
  buttonsLayout->addWidget(editRightAssignedEntityButton);
  buttonsLayout->addWidget(removeEntitiesButton);
  mainLayout->addLayout(buttonsLayout);
  
  QPointer<QFrame> bottomLine = new QFrame;
  bottomLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(bottomLine);

  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(resetButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Add / Edit Relationship");
  resize(600, 600);
}

void RelationshipsDialog::submitLeftEntity(QString entity) {
  selectedSourceLabel->setText(entity);
  for (int i = 0; i != entitiesTable->rowCount(); i++) {
    QString name = entitiesTable->record(i).field(1).value().toString();
    if (name == entity) {
      entitiesView->setRowHidden(i, true);
    }
  }
}

void RelationshipsDialog::submitRightEntity(QString entity) {
  selectedTargetLabel->setText(entity);
  for (int i = 0; i != entitiesTable->rowCount(); i++) {
    QString name = entitiesTable->record(i).field(1).value().toString();
    if (name == entity) {
      entitiesView->setRowHidden(i, true);
    }
  }
}

void RelationshipsDialog::submitType(QString type) {
  typeLabel->setText(type);
}

void RelationshipsDialog::submitDescription(QString description) {
  QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
  typeLabel->setToolTip(hint);
}

void RelationshipsDialog::submitDirectedness(QString directedness) {
  if (directedness == DIRECTED) {
    tailLabel->setText(DIRECTEDTAIL);
  } else {
    tailLabel->setText(UNDIRECTEDTAIL);
  }
}

void RelationshipsDialog::submitName(QString name) {
  oldName = name;
}

void RelationshipsDialog::assignLeftEntity() {
  if (entitiesView->currentIndex().isValid()) {
    for (int i = 0; i != entitiesTable->rowCount(); i++) {
      QModelIndex currentIndex = entitiesTable->createIndex(i, 1);
      if (currentIndex.data(Qt::DisplayRole).toString() != selectedTargetLabel->text()) {
	entitiesView->setRowHidden(i, false);
      }
    }
    QString selected = entitiesView->currentIndex().data(Qt::DisplayRole).toString();
    int row = entitiesView->currentIndex().row();
    entitiesView->setRowHidden(row, true);
    selectedSourceLabel->setText(selected);
  }
}

void RelationshipsDialog::assignRightEntity() {
  if (entitiesView->currentIndex().isValid()) {
    for (int i = 0; i != entitiesTable->rowCount(); i++) {
      QModelIndex currentIndex = entitiesTable->createIndex(i, 1);
      if (currentIndex.data(Qt::DisplayRole).toString() != selectedSourceLabel->text()) {
	entitiesView->setRowHidden(i, false);
      }
    }
    QString selected = entitiesView->currentIndex().data(Qt::DisplayRole).toString();
    int row = entitiesView->currentIndex().row();
    entitiesView->setRowHidden(row, true);
    selectedTargetLabel->setText(selected);
  }
}


void RelationshipsDialog::filterEntity(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);
  entitiesFilter->setFilterRegExp(regExp);
}

void RelationshipsDialog::addEntity() {
  EntityDialog *entityDialog = new EntityDialog(this);
  entityDialog->setNew();
  entityDialog->exec();
  if (entityDialog->getExitStatus() == 0) {
    QString name = entityDialog->getName();
    QString description = entityDialog->getDescription();
    entitiesTable->select();
    int max = entitiesTable->rowCount();
    entitiesTable->insertRow(max);
    entitiesTable->setData(entitiesTable->index(max, 1), name);
    entitiesTable->setData(entitiesTable->index(max, 2), description);
    entitiesTable->submitAll();
  }
  delete entityDialog;
  for (int i = 0; i != entitiesTable->rowCount(); i++) {
    QModelIndex currentIndex = entitiesTable->createIndex(i, 1);
    if (currentIndex.data(Qt::DisplayRole).toString() == selectedSourceLabel->text() ||
	currentIndex.data(Qt::DisplayRole).toString() == selectedTargetLabel->text()) {
      entitiesView->setRowHidden(i, true);
    }
  }
  entitiesFilter->sort(1, Qt::AscendingOrder);
}

void RelationshipsDialog::editEntity() {
  if (entitiesView->currentIndex().isValid()) {
    QString selected = entitiesView->currentIndex().data(Qt::DisplayRole).toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entities WHERE name = :name");
    query->bindValue(":name", selected);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    EntityDialog *entityDialog = new EntityDialog(this);
    entityDialog->submitName(selected);
    entityDialog->submitDescription(description);
    entityDialog->exec();
    if (entityDialog->getExitStatus() == 0) {
      QString name = entityDialog->getName();
      QString description = entityDialog->getDescription();
      query->prepare("UPDATE TABLE entities SET name = :name, description = :description WHERE name = :oldName");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":oldName", selected);
      query->exec();
    }
    delete query;
    delete entityDialog;
    for (int i = 0; i != entitiesTable->rowCount(); i++) {
      QModelIndex currentIndex = entitiesTable->createIndex(i, 1);
      if (currentIndex.data(Qt::DisplayRole).toString() == selectedSourceLabel->text() ||
	  currentIndex.data(Qt::DisplayRole).toString() == selectedTargetLabel->text()) {
	entitiesView->setRowHidden(i, true);
      }
    }
    entitiesFilter->sort(1, Qt::AscendingOrder);
  }
}

void RelationshipsDialog::editLeftAssignedEntity() {
  if (selectedSourceLabel->text() != DEFAULT) {
    QString selected = selectedSourceLabel->text();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entities WHERE name = :name");
    query->bindValue(":name", selected);
    query->exec();
    query->first();
    QString description =  query->value(0).toString();
    EntityDialog *entityDialog = new EntityDialog(this);
    entityDialog->submitName(selected);
    entityDialog->submitDescription(description);
    entityDialog->exec();
    if (entityDialog->getExitStatus() == 0) {
      QString name = entityDialog->getName();
      QString description = entityDialog->getDescription();
      query->prepare("UPDATE TABLE entities SET name = :name, description = :description WHERE name = :oldName");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":oldName", selected);
      query->exec();
    }
    delete query;
    delete entityDialog;
    for (int i = 0; i != entitiesTable->rowCount(); i++) {
      QModelIndex currentIndex = entitiesTable->createIndex(i, 1);
      if (currentIndex.data(Qt::DisplayRole).toString() == selectedSourceLabel->text() ||
	  currentIndex.data(Qt::DisplayRole).toString() == selectedTargetLabel->text()) {
	entitiesView->setRowHidden(i, true);
      }
    }
    entitiesFilter->sort(1, Qt::AscendingOrder);
  }
}

void RelationshipsDialog::editRightAssignedEntity() {
  if (selectedTargetLabel->text() != DEFAULT) {
    QString selected = selectedTargetLabel->text();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entities WHERE name = :name");
    query->bindValue(":name", selected);
    query->exec();
    query->first();
    QString description =  query->value(0).toString();
    EntityDialog *entityDialog = new EntityDialog(this);
    entityDialog->submitName(selected);
    entityDialog->submitDescription(description);
    entityDialog->exec();
    if (entityDialog->getExitStatus() == 0) {
      QString name = entityDialog->getName();
      QString description = entityDialog->getDescription();
      query->prepare("UPDATE TABLE entities SET name = :name, description = :description WHERE name = :oldName");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":oldName", selected);
      query->exec();
    }
    delete query;
    delete entityDialog;
    for (int i = 0; i != entitiesTable->rowCount(); i++) {
      QModelIndex currentIndex = entitiesTable->createIndex(i, 1);
      if (currentIndex.data(Qt::DisplayRole).toString() == selectedSourceLabel->text() ||
	  currentIndex.data(Qt::DisplayRole).toString() == selectedTargetLabel->text()) {
	entitiesView->setRowHidden(i, true);
      }
    }
    entitiesFilter->sort(1, Qt::AscendingOrder);
  }
}

void RelationshipsDialog::removeEntities() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->exec("SELECT name FROM entities EXCEPT SELECT source "
	      "FROM entity_relationships EXCEPT SELECT target "
	      "FROM entity_relationships");
  while (query->next()) {
    QString current = query->value(0).toString();
    query2->prepare("DELETE FROM entities WHERE name = :current");
    query2->bindValue(":current", current);
    query2->exec();
    query2->prepare("DELETE FROM attributes_to_entities WHERE entity = :current");
    query2->bindValue(":current", current);
    query2->exec();
  }
  reset();
  entitiesTable->select();
  delete query;  
  delete query2;
  for (int i = 0; i != entitiesTable->rowCount(); i++) {
    QModelIndex currentIndex = entitiesTable->createIndex(i, 1);
    if (currentIndex.data(Qt::DisplayRole).toString() == selectedSourceLabel->text() ||
	currentIndex.data(Qt::DisplayRole).toString() == selectedTargetLabel->text()) {
      entitiesView->setRowHidden(i, true);
    }
  }
  entitiesFilter->sort(1, Qt::AscendingOrder);
}

void RelationshipsDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void RelationshipsDialog::saveAndClose() {
  if (selectedSourceLabel->text() == DEFAULT) {
    QPointer<QMessageBox> errorBox = new QMessageBox(this);
    errorBox->setText(tr("<b>No source selected</b>"));
    errorBox->setInformativeText("You did not select a source for this relationship.");
    errorBox->exec();
    delete errorBox;
    return;
  }
  if (selectedTargetLabel->text() == DEFAULT) {
    QPointer<QMessageBox> errorBox = new QMessageBox(this);
    errorBox->setText(tr("<b>No target selected</b>"));
    errorBox->setInformativeText("You did not select a target for this relationship.");
    errorBox->exec();
    delete errorBox;
    return;
  }
  name = selectedSourceLabel->text() + tailLabel->text() + headLabel->text() + selectedTargetLabel->text();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, type FROM entity_relationships WHERE name = :name AND type = :type");
  query->bindValue(":name", name);
  query->bindValue(":type", typeLabel->text());
  query->exec();
  query->first();
  if (query->isNull(0) || name == oldName) {
    exitStatus = 0;
    this->close();
  } else {
    QPointer<QMessageBox> errorBox = new QMessageBox(this);
    errorBox->setText(tr("<b>Relationship already exists</b>"));
    errorBox->setInformativeText("An identical relationship already exists.");
    errorBox->exec();
    delete errorBox;
    return;
  }
  delete query;
}

int RelationshipsDialog::getExitStatus() {
  return exitStatus;
}

QString RelationshipsDialog::getName() {
  return name;
}

QString RelationshipsDialog::getLeftEntity() {
  return selectedSourceLabel->text();
}

QString RelationshipsDialog::getRightEntity() {
  return selectedTargetLabel->text();
}

void RelationshipsDialog::reset() {
  selectedSourceLabel->setText(DEFAULT);
  selectedTargetLabel->setText(DEFAULT);
  for (int i = 0; i != entitiesTable->rowCount(); i++) {
    entitiesView->setRowHidden(i, false);
  }
}
