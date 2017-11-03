#include "../include/RelationshipsDialog.h"

RelationshipsDialog::RelationshipsDialog(QWidget *parent) : QDialog(parent) {
  // First we declare the entities of this dialog.
  tailLabel = new QLabel(DIRECTEDTAIL);
  headLabel = new QLabel(DIRECTEDHEAD);
  selectedSourceLabel = new QLabel(DEFAULT);
  selectedTargetLabel = new QLabel(DEFAULT);
  name = "";
  oldName = "";
  type = "";
  exitStatus = 1;
  
  sourceLabel = new QLabel(tr("Source filter:"));
  typeLabel = new QLabel("PLACEHOLDER");
  targetLabel = new QLabel(tr("Target filter:"));
  entitiesTable = new EntityTableModel;
  entitiesTable->setTable("entities");
  entitiesTable->select();
  leftEntitiesFilter = new QSortFilterProxyModel;
  leftEntitiesFilter->setSourceModel(entitiesTable);
  leftEntitiesFilter->setFilterKeyColumn(1);
  rightEntitiesFilter = new QSortFilterProxyModel;
  rightEntitiesFilter->setSourceModel(entitiesTable);
  rightEntitiesFilter->setFilterKeyColumn(1);
  leftEntitiesView = new QListView;
  rightEntitiesView = new QListView;
  leftEntitiesView->setModel(leftEntitiesFilter);
  leftEntitiesView->setModelColumn(1);
  rightEntitiesView->setModel(rightEntitiesFilter);
  rightEntitiesView->setModelColumn(1);
  
  assignLeftEntityButton = new QPushButton("use selected");
  assignRightEntityButton = new QPushButton("use selected");
  
  leftEntityFilterField = new QLineEdit();
  rightEntityFilterField = new QLineEdit();
  newEntityButton = new QPushButton(tr("Define new entity"));
  editEntityButton = new QPushButton(tr("Edit highlighted entity"));
  editLeftAssignedEntityButton = new QPushButton(tr("Edit left assigned entity"));
  editRightAssignedEntityButton = new QPushButton(tr("Edit right assigned entity"));
  removeEntitiesButton = new QPushButton(tr("Remove unused entities"));

  cancelCloseButton = new QPushButton(tr("Cancel"));
  resetButton = new QPushButton(tr("Reset"));
  saveCloseButton = new QPushButton(tr("Save relationship"));
  
  //connect(leftEntitiesView, SIGNAL(clicked(QModelIndex &)), this, SLOT(setCurrentEntityLeft(QModelIndex &)));
  //connect(rightEntitiesView, SIGNAL(clicked(QModelIndex &)), this, SLOT(setCurrentEntityRight(QModelIndex &)));
  //connect(sourceListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editEntity()));
  //connect(targetListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editEntity()));
  connect(leftEntityFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(filterLeftEntity(const QString &)));
  connect(rightEntityFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(filterRightEntity(const QString &)));
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
  labelLayout->addWidget(leftEntityFilterField);
  labelLayout->addWidget(targetLabel);
  labelLayout->addWidget(rightEntityFilterField);
  mainLayout->addLayout(labelLayout);
  QPointer<QHBoxLayout> listsLayout = new QHBoxLayout;
  listsLayout->addWidget(leftEntitiesView);
  listsLayout->addWidget(rightEntitiesView);
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

  QPointer<QFrame> topLine = new QFrame();
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  QPointer<QVBoxLayout> buttonsLayout = new QVBoxLayout;
  buttonsLayout->addWidget(newEntityButton);
  buttonsLayout->addWidget(editEntityButton);
  buttonsLayout->addWidget(editLeftAssignedEntityButton);
  buttonsLayout->addWidget(editRightAssignedEntityButton);
  buttonsLayout->addWidget(removeEntitiesButton);
  mainLayout->addLayout(buttonsLayout);
  
  QPointer<QFrame> bottomLine = new QFrame();
  bottomLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(bottomLine);

  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(resetButton);
  optionsLayout->addWidget(saveCloseButton);
  optionsLayout->setSpacing(300);
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
      leftEntitiesView->setRowHidden(i, true);
      rightEntitiesView->setRowHidden(i, true);
    }
  }
}

void RelationshipsDialog::submitRightEntity(QString entity) {
  selectedTargetLabel->setText(entity);
  for (int i = 0; i != entitiesTable->rowCount(); i++) {
    QString name = entitiesTable->record(i).field(1).value().toString();
    if (name == entity) {
      leftEntitiesView->setRowHidden(i, true);
      rightEntitiesView->setRowHidden(i, true);
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
  if (leftEntitiesView->currentIndex().isValid()) {
    for (int i = 0; i != entitiesTable->rowCount(); i++) {
      leftEntitiesView->setRowHidden(i, false);
      rightEntitiesView->setRowHidden(i, false);
    }
    QString selected = leftEntitiesView->currentIndex().data(Qt::DisplayRole).toString();
    int row = leftEntitiesView->currentIndex().row();
    leftEntitiesView->setRowHidden(row, true);
    rightEntitiesView->setRowHidden(row, true);
    selectedSourceLabel->setText(selected);
  }
}

void RelationshipsDialog::assignRightEntity() {
  if (rightEntitiesView->currentIndex().isValid()) {
    for (int i = 0; i != entitiesTable->rowCount(); i++) {
      leftEntitiesView->setRowHidden(i, false);
      rightEntitiesView->setRowHidden(i, false);
    }
    QString selected = rightEntitiesView->currentIndex().data(Qt::DisplayRole).toString();
    int row = rightEntitiesView->currentIndex().row();
    leftEntitiesView->setRowHidden(row, true);
    rightEntitiesView->setRowHidden(row, true);
    selectedTargetLabel->setText(selected);
  }
}

void RelationshipsDialog::filterLeftEntity(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);
  leftEntitiesFilter->setFilterRegExp(regExp);
}

void RelationshipsDialog::filterRightEntity(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);
  rightEntitiesFilter->setFilterRegExp(regExp);
}

void RelationshipsDialog::addEntity() {
  //EntityDialog *entityDialog = new EntityDialog(this, dataInterface, EMPTY, logger);
  //entityDialog->exec();
  //delete entityDialog;
}

void RelationshipsDialog::editEntity() {

}

void RelationshipsDialog::editLeftAssignedEntity() {
  /*if (currentLeftEntitySelected != DEFAULT && currentLeftEntitySelected != EMPTY) {
    EntityDialog *entityDialog = new EntityDialog(this, dataInterface, currentLeftEntitySelected, logger);
    entityDialog->deleteLater();
    disableAssign();
    entityDialog->exec();
    std::string oName = entityDialog->getOldName();
    std::string nName = entityDialog->getNewName();
    if (oName != nName) {
      if (currentLeftEntitySelected == QString::fromStdString(oName)) {
	currentLeftEntitySelected = QString::fromStdString(nName);
	selectedSourceLabel->setText(currentLeftEntitySelected);
	if (submittedLabel != EMPTY) {
	  std::string tail = "";
	  std::string head = "]-->";
	  if (currentDirectedness == DIRECTED) {
	    tail = "---[";
	  } else if (currentDirectedness == UNDIRECTED) {
	    tail = "<--[";
	  }
	  std::string label = selectedSourceLabel->text().toStdString() + tail +
	    selectedTypeLabel->text().toStdString() + head + selectedTargetLabel->text().toStdString();
	  permanentLabel = QString::fromStdString(label);
	}
      }
    }
    delete entityDialog;
    updateTexts();
    }*/
}

void RelationshipsDialog::editRightAssignedEntity() {
  /*  if (currentRightEntitySelected != DEFAULT && currentRightEntitySelected != EMPTY) {
    EntityDialog *entityDialog = new EntityDialog(this, dataInterface, currentRightEntitySelected, logger);
    entityDialog->deleteLater();
    disableAssign();
    entityDialog->exec();
    std::string oName = entityDialog->getOldName();
    std::string nName = entityDialog->getNewName();
    if (oName != nName) {
      if (currentRightEntitySelected == QString::fromStdString(oName)) {
	currentRightEntitySelected = QString::fromStdString(nName);
	selectedTargetLabel->setText(currentRightEntitySelected);
	if (submittedLabel != EMPTY) {
	  std::string tail = "";
	  std::string head = "]-->";
	  if (currentDirectedness == DIRECTED) {
	    tail = "---[";
	  } else if (currentDirectedness == UNDIRECTED) {
	    tail = "<--[";
	  }
	  std::string label = selectedSourceLabel->text().toStdString() + tail +
	    selectedTypeLabel->text().toStdString() + head + selectedTargetLabel->text().toStdString();
	  permanentLabel = QString::fromStdString(label);
	}
      }
    }
    delete entityDialog;
    updateTexts();
    }*/
}

void RelationshipsDialog::removeEntities() {
  // TO DO
}

void RelationshipsDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void RelationshipsDialog::saveAndClose() {
  if (selectedSourceLabel->text() == DEFAULT) {
    QPointer<QMessageBox> errorBox = new QMessageBox;
    errorBox->setText(tr("<b>No source selected</b>"));
    errorBox->setInformativeText("You did not select a source for this relationship.");
    errorBox->exec();
    return;
  }
  if (selectedTargetLabel->text() == DEFAULT) {
    QPointer<QMessageBox> errorBox = new QMessageBox;
    errorBox->setText(tr("<b>No target selected</b>"));
    errorBox->setInformativeText("You did not select a target for this relationship.");
    errorBox->exec();
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
    QPointer<QMessageBox> errorBox = new QMessageBox;
    errorBox->setText(tr("<b>Relationship already exists</b>"));
    errorBox->setInformativeText("An identical relationship already exists.");
    errorBox->exec();
    return;
  }
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
    leftEntitiesView->setRowHidden(i, false);
    rightEntitiesView->setRowHidden(i, false);
  }
}
