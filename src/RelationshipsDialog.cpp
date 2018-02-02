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
  entityEdited = 0;
  
  sourceLabel = new QLabel(tr("Source filter:"), this);
  typeLabel = new QLabel("", this);
  entitiesTable = new EntityTableModel(this);
  entitiesTable->setTable("entities");
  entitiesTable->setSort(1, Qt::AscendingOrder);
  entitiesTable->select();
  entitiesFilter = new EntitiesFilter(this);
  entitiesFilter->setSourceModel(entitiesTable);
  entitiesFilter->setFilterKeyColumn(1);
  entitiesFilter->setDynamicSortFilter(true);
  entitiesView = new ZoomableListView(this);
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
  
  connect(entityFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(filterEntity(const QString &)));
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
  if (entity != selectedSourceLabel->text() && entity != selectedTargetLabel->text()) {
    selectedSourceLabel->setText(entity);
  }
}

void RelationshipsDialog::submitRightEntity(QString entity) {
  if (entity != selectedSourceLabel->text() && entity != selectedTargetLabel->text()) {
    selectedTargetLabel->setText(entity);
  }
}

void RelationshipsDialog::submitType(QString type) {
  typeLabel->setText(type);
}

void RelationshipsDialog::submitDescription(QString description) {
  QString hint = breakString(description);
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
    QString selected = entitiesView->currentIndex().data(Qt::DisplayRole).toString();
    selectedSourceLabel->setText(selected);
  }
  filterEntity(entityFilterField->text());
}

void RelationshipsDialog::assignRightEntity() {
  if (entitiesView->currentIndex().isValid()) {
    QString selected = entitiesView->currentIndex().data(Qt::DisplayRole).toString();
    selectedTargetLabel->setText(selected);
  }
  filterEntity(entityFilterField->text());
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
      updateAfterEdit(name, description, selected);
    }
    delete query;
    delete entityDialog;
    entitiesTable->select();
    entitiesFilter->sort(1, Qt::AscendingOrder);
  }
}

void RelationshipsDialog::updateAfterEdit(const QString name, const QString description,
					  const QString former) {
  entityEdited = 1;
  QSqlQuery *query = new QSqlQuery;
  // Update the entity itself.
  query->prepare("UPDATE entities "
		 "SET name = :name, description = :description "
		 "WHERE name = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();

  // Update attributes.
  query->prepare("UPDATE attributes_to_entities "
		 "SET entity = :new "
		 "WHERE entity = :old");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();

  /*
    Next up are the relationships in which the entity already participates.
    First, let us update all the relationships where the current entity is a source.
  */
  if (name != former) {
    query->prepare("SELECT name, target, type "
		   "FROM entity_relationships WHERE source = :current");
    query->bindValue(":current", former);
    query->exec();
    while (query->next()) {
      QString oldRelationship = query->value(0).toString();
      QString target = query->value(1).toString();
      QString type = query->value(2).toString();
      QSqlQuery *query2 = new QSqlQuery();
      query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
      query2->bindValue(":type", type);
      query2->exec();
      query2->first();
      QString directedness = query2->value(0).toString();
      QString arrow = "";
      if (directedness == UNDIRECTED) {
	arrow = "<-->";
      } else if (directedness == DIRECTED) {
	arrow = "--->";
      }
      QString newRelationship = name + arrow + target;
      query2->prepare("UPDATE entity_relationships "
		      "SET source = :source, name = :name "
		      "WHERE source = :oldSource AND name = :oldRelationship");
      query2->bindValue(":source", name);
      query2->bindValue(":name", newRelationship);
      query2->bindValue(":oldSource", former);
      query2->bindValue(":oldRelationship", oldRelationship);
      query2->exec();
      query2->prepare("UPDATE relationships_to_incidents "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query2->exec();
      query2->prepare("UPDATE relationships_to_incidents_sources "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query2->exec();
      delete query2;
    }
    // And then the relationships where the entity is a target.
    query->prepare("SELECT name, source, type "
		   "FROM entity_relationships WHERE target = :current");
    query->bindValue(":current", former);
    query->exec();
    while (query->next()) {
      QString oldRelationship = query->value(0).toString();
      QString source = query->value(1).toString();
      QString type = query->value(2).toString();
      QSqlQuery *query2 = new QSqlQuery();
      query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
      query2->bindValue(":type", type);
      query2->exec();
      query2->first();
      QString directedness = query2->value(0).toString();
      QString arrow = "";
      if (directedness == UNDIRECTED) {
	arrow = "<-->";
      } else if (directedness == DIRECTED) {
	arrow = "--->";
      }
      QString newRelationship = source + arrow + name;
      query2->prepare("UPDATE entity_relationships "
		      "SET target = :target, name = :name "
		      "WHERE target = :oldTarget AND name = :oldRelationship");
      query2->bindValue(":target", name);
      query2->bindValue(":name", newRelationship);
      query2->bindValue(":oldTarget", former);
      query2->bindValue(":oldRelationship", oldRelationship);
      query2->exec();
      query2->prepare("UPDATE relationships_to_incidents "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query->exec();
      query2->prepare("UPDATE relationships_to_incidents_sources "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query2->exec();
      delete query2;
    }
    delete query;
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
      updateAfterEdit(name, description, selected);
      selectedSourceLabel->setText(name);
      oldName = selectedSourceLabel->text() +
	tailLabel->text() + headLabel->text() +
	selectedTargetLabel->text();
    }
    delete query;
    delete entityDialog;
    filterEntity(entityFilterField->text());
    entitiesTable->select();
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
      updateAfterEdit(name, description, selected);
      selectedTargetLabel->setText(name);
      oldName = selectedSourceLabel->text() +
	tailLabel->text() + headLabel->text() +
	selectedTargetLabel->text();
    }
    delete query;
    delete entityDialog;
    entitiesTable->select();
    filterEntity(entityFilterField->text());
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

int RelationshipsDialog::getEntityEdited() {
  return entityEdited;
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
}

