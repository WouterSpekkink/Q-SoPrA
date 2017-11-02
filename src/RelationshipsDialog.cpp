#include "../include/RelationshipsDialog.h"

RelationsDialog::RelationsDialog(QWidget *parent) : QDialog(parent) {
  // First we declare the entities of this dialog.
  tailLabel = new QLabel(DIRECTEDTAIL);
  headLabel = new QLabel(DIRECTEDHEAD);
  currentLeftEntity = "";
  currentRightEntity = "";
  currentLeftEntityFilter = "";
  currentRightEntityFilter = "";
  selectedSourceLabel = new QLabel(DEFAULT);
  selectedTargetLabel = new QLabel(DEFAULT);
  name = "";
  oldName = "";
  currentLeftEntitySelected = "";
  typeLabel = "";
  currentRightEntitySelected = "";
  currentDirectedness = DIRECTED;
  
  sourceLabel = new QLabel(tr("Source filter:"));
  targetLabel = new QLabel(tr("Target filter:"));

  entitiesTable = new QSqlTableModel;
  leftEntitiesView = new QListView;
  rightEntitiesView = new QListView;
  leftEntitiesFilter = new QSortFilterProxyModel;
  rightEntitiesFilter = new QSortFilterProxyModel;
    
  //sourceListWidget = new QListWidget();
  //sourceListWidget->setSortingEnabled(true);
  //targetListWidget = new QListWidget();
  //targetListWidget->setSortingEnabled(true);
  
  assignLeftEntityButton = new QPushButton("use selected");
  assignLeftEntityButton->setEnabled(false);
  assignRightEntityButton = new QPushButton("use selected");
  assignRightEntityButton->setEnabled(false);
  
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
  
  // Then we wire the signals.

  connect(sourceListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setCurrentEntityLeft(QListWidgetItem*)));
  connect(targetListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setCurrentEntityRight(QListWidgetItem*)));
  connect(sourceListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editEntity()));
  connect(targetListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editEntity()));
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
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(informativeLabel);
  informativeLabel->setAlignment(Qt::AlignHCenter);

  QPointer<QHBoxLayout> labelLayout = new QHBoxLayout;
  labelLayout->addWidget(sourceLabel);
  labelLayout->addWidget(leftEntityFilterField);
  labelLayout->addWidget(targetLabel);
  labelLayout->addWidget(rightEntityFilterField);
  mainLayout->addLayout(labelLayout);
  QPointer<QHBoxLayout> listsLayout = new QHBoxLayout;
  listsLayout->addWidget(sourceListWidget);
  listsLayout->addWidget(targetListWidget);
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

  QPointer<QVBoxLayout`> buttonsLayout = new QVBoxLayout;
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


void RelationshipsDialog::setCurrentEntityLeft(QListWidgetItem *item) {
  currentLeftEntity = item->text();
  lastSelectedEntity = item->text();
  assignLeftEntityButton->setEnabled(true);
  assignRightEntityButton->setEnabled(false);
}

void RelationshipsDialog::setCurrentEntityRight(QListWidgetItem *item) {
  currentRightEntity = item->text();
  lastSelectedEntity = item->text();
  assignLeftEntityButton->setEnabled(false);
  assignRightEntityButton->setEnabled(true);
}

void RelationshipsDialog::assignLeftEntity() {
  if (currentLeftEntity != "") {
    currentLeftEntitySelected = currentLeftEntity;
    selectedSourceLabel->setText(currentLeftEntitySelected);
  }
  disableAssign();
}

void RelationshipsDialog::assignRightEntity() {
  if (currentRightEntity != "") {
    currentRightEntitySelected = currentRightEntity;
    selectedTargetLabel->setText(currentRightEntitySelected);
  }
  disableAssign();
}

void RelationshipsDialog::filterLeftEntity(const QString &text) {
  // TODO
}

void RelationshipsDialog::filterRightEntity(const QString &text) {
  // TODO
}

void RelationshipsDialog::addEntity() {
  disableAssign();
  //EntityDialog *entityDialog = new EntityDialog(this, dataInterface, EMPTY, logger);
  //entityDialog->exec();
  //delete entityDialog;
}

void RelationshipsDialog::editEntity() {
  if (lastSelectedEntity != "") {
    //EntityDialog *entityDialog = new EntityDialog(this, dataInterface, lastSelectedEntity, logger);
    //entityDialog->exec();
    //disableAssign();
    //delete entityDialog;
  }
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
  disableAssign();
  // TO DO
}


void RelationshipsDialog::disableAssign() {
  lastSelectedEntity = "";
  currentRelType = "";
  assignLeftEntityButton->setEnabled(false);
  assignRightEntityButton->setEnabled(false);
  assignTypeButton->setEnabled(false);
}

void RelationsDialog::updateTexts() {

}


