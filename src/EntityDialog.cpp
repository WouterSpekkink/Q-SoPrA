#include "../include/EntityDialog.h"
#include "../include/RelationshipsWidget.h"

EntityDialog::EntityDialog(QWidget *parent) : QDialog(parent) {
  name = "";
  oldName = "";
  exitStatus = 1;
  isNew = false;
  
  // I have to build these as well.
  incidentsModel = new QSqlTableModel(this);  
  incidentsModel->setTable("incidents");
  incidentsModel->select();

  attributesTreeView = new DeselectableTreeViewEntities(this);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->setDragEnabled(true);
  attributesTreeView->setAcceptDrops(true);
  attributesTreeView->setDropIndicatorShown(true);
  attributesTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  attributesTreeView->setExpandsOnDoubleClick(false);
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  attributesTreeView->installEventFilter(this);
  
  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree(); 
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);

  // First we declare the entities of this dialog.
  nameLabel = new QLabel(tr("<b>Name:</b>"), this);
  nameField = new QLineEdit(this);

  descriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  descriptionField = new QTextEdit(this);
  descriptionField->installEventFilter(this);

  valueLabel = new QLabel(tr("<b>Value:</b>"), this);
  valueField = new QLineEdit(this);
  valueField->setEnabled(false);
    
  attributesLabel = new QLabel(tr("<h2>Attributes</h2>"), this);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), this);

  attributesFilterField = new QLineEdit(this);
  
  valueButton = new QPushButton("Store value", this);
  valueButton->setEnabled(false);
  assignAttributeButton = new QPushButton("Assign attribute", this);
  assignAttributeButton->setEnabled(false);
  unassignAttributeButton = new QPushButton("Unassign attribute", this);
  unassignAttributeButton->setEnabled(false);
  addAttributeButton = new QPushButton(tr("New attribute"), this);
  editAttributeButton = new QPushButton(tr("Edit attribute"), this);
  editAttributeButton->setEnabled(false);
  mergeAttributesButton = new QPushButton(tr("Merge with..."), this);
  mergeAttributesButton->setEnabled(false);
  removeUnusedAttributesButton = new QPushButton(tr("Remove unused attributes"), this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save entity"), this);
  expandButton = new QPushButton("+", this);
  collapseButton = new QPushButton("-", this);
  
  // Then we wire the signals of the dialog.
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setFilter(const QString &)));
  connect(valueField, SIGNAL(textChanged(const QString &)), this, SLOT(setValueButton()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(mergeAttributesButton, SIGNAL(clicked()), this, SLOT(mergeAttributes()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(getValue()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(attributesTreeView, SIGNAL(noneSelected()), this, SLOT(setButtons()));
  connect(expandButton, SIGNAL(clicked()), this, SLOT(expandTree()));
  connect(collapseButton, SIGNAL(clicked()), this, SLOT(collapseTree()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  // Then we create the layout for the dialog.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> subLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  leftLayout->addLayout(nameLayout);
  leftLayout->addWidget(descriptionLabel);
  leftLayout->addWidget(descriptionField);
  subLayout->addLayout(leftLayout);
  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> titleLayout = new QHBoxLayout;
  titleLayout->addWidget(attributesLabel);
  QPointer<QHBoxLayout> collapseLayout = new QHBoxLayout;
  collapseLayout->addWidget(expandButton);
  expandButton->setMaximumWidth(expandButton->sizeHint().width());
  collapseLayout->addWidget(collapseButton);
  collapseButton->setMaximumWidth(collapseButton->sizeHint().width());
  titleLayout->addLayout(collapseLayout);
  rightLayout->addLayout(titleLayout);
  rightLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributesFilterLabel);
  filterLayout->addWidget(attributesFilterField);
  rightLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> valueLayout = new QHBoxLayout;
  valueLayout->addWidget(valueLabel);
  valueLayout->addWidget(valueField);
  valueLayout->addWidget(valueButton);
  rightLayout->addLayout(valueLayout);
  QPointer<QHBoxLayout> assignLayout = new QHBoxLayout;
  assignLayout->addWidget(assignAttributeButton);
  assignLayout->addWidget(unassignAttributeButton);
  rightLayout->addLayout(assignLayout);
  QPointer<QHBoxLayout> attributeButtonLayout = new QHBoxLayout;
  attributeButtonLayout->addWidget(addAttributeButton);
  attributeButtonLayout->addWidget(editAttributeButton);
  attributeButtonLayout->addWidget(mergeAttributesButton);
  attributeButtonLayout->addWidget(removeUnusedAttributesButton);
  attributeButtonLayout->setAlignment(Qt::AlignVCenter);
  rightLayout->addLayout(attributeButtonLayout);

  subLayout->addLayout(rightLayout);
  mainLayout->addLayout(subLayout);

  QPointer<QHBoxLayout> optionLayout = new QHBoxLayout;
  optionLayout->addWidget(cancelCloseButton);
  optionLayout->addWidget(saveCloseButton);
  optionLayout->setSpacing(600);
  mainLayout->addLayout(optionLayout);

  setLayout(mainLayout);
  setWindowTitle("Add /Edit Entity");
  setFixedHeight(sizeHint().height());
  fresh = true;
}

void EntityDialog::setValueButton() {
  valueButton->setEnabled(true);
}

void EntityDialog::setValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQuery *query = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    if (isNew) {
      query->prepare("UPDATE attributes_to_entities SET value = :val "
		     "WHERE attribute = :attribute AND new = :new");
      query->bindValue(":val", valueField->text());
      query->bindValue(":attribute", attribute);
      query->bindValue(":new", 1);
      query->exec();
    } else {
      query->prepare("UPDATE attributes_to_entities SET value = :val "
		     "WHERE attribute = :attribute AND entity = :oldName");
      query->bindValue(":val", valueField->text());
      query->bindValue(":attribute", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
    }
    valueButton->setEnabled(false);
    delete query;
  }
}

void EntityDialog::getValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    if (isNew) {
      query->prepare("SELECT attribute, value FROM "
		     "attributes_to_entities "
		     "WHERE attribute =:att AND new = 1");
      query->bindValue(":att", attribute);
      query->exec();
      query->first();
      if (!(query->isNull(0))) {
	valueField->setEnabled(true);
      } else {
	valueField->setEnabled(false);
	valueField->setText("");
      }
      if (!(query->isNull(1))) {
	QString value = query->value(1).toString();
	valueField->setText(value);
      } else {
	valueField->setText("");
      }
      valueButton->setEnabled(false);
    } else {
      query->prepare("SELECT attribute, value FROM attributes_to_entities "
		     "WHERE attribute =:att AND entity = :oldName");
      query->bindValue(":att", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      query->first();
      if (!(query->isNull(0))) {
	valueField->setEnabled(true);
      } else {
	valueField->setEnabled(false);
	valueField->setText("");
      }
      if (!(query->isNull(1))) {
	QString value = query->value(1).toString();
	valueField->setText(value);
      } else {
	valueField->setText("");
      }
    }
    valueButton->setEnabled(false);
    delete query;
  } else {
    valueField->setText("");
    valueButton->setEnabled(false);
  }
}

void EntityDialog::setFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void EntityDialog::assignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    if (isNew) {
      bool empty = false;
      query->prepare("SELECT attribute, new FROM attributes_to_entities "
		     "WHERE attribute = :att AND new = 1");
      query->bindValue(":att", attribute);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (empty) {
	QSqlQuery *query2 = new QSqlQuery;
	query2->prepare("INSERT INTO attributes_to_entities (attribute, new) "
			"VALUES (:attribute, :new)");
	query2->bindValue(":attribute", attribute);
	query2->bindValue(":new", 1);
	query2->exec();
	delete query2;	
        boldSelected(attributesTree, attribute);
	valueField->setEnabled(true);
      }
    } else {
      bool empty = false;
      query->prepare("SELECT attribute, new FROM attributes_to_entities "
		     "WHERE attribute = :att AND entity = :oldName");
      query->bindValue(":att", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (empty) {
	QSqlQuery *query2 = new QSqlQuery;
	query2->prepare("INSERT INTO attributes_to_entities (attribute, entity) "
			"VALUES (:attribute, :entity)");
	query2->bindValue(":attribute", attribute);
	query2->bindValue(":entity", oldName);
	query2->exec();
	boldSelected(attributesTree, attribute);
	valueField->setEnabled(true);
      }
    }
    setButtons();
    delete query;
  }
}

void EntityDialog::unassignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQuery *query = new QSqlQuery;
    QSqlQuery *query2 = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    bool empty = false;
    if (isNew) {
      query->prepare("SELECT attribute FROM attributes_to_entities "
		     "WHERE attribute = :att AND new = 1");
      query->bindValue(":att", attribute);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	query->prepare("DELETE FROM attributes_to_entities "
		       "WHERE attribute = :att AND new = 1");
	query->bindValue(":att", attribute);
	query->exec();
	resetFont(attributesTree);
	query2->exec("SELECT attribute, new FROM attributes_to_entities");
	while (query2->next()) {
	  QString attribute = query2->value(0).toString();
	  int mark = query2->value(1).toInt();
	  if (mark == 1) {
	    boldSelected(attributesTree, attribute);
	  }
	}
	valueField->setText("");
	valueField->setEnabled(false);
	valueButton->setEnabled(false);
      }
    } else {
      query->prepare("SELECT attribute, entity FROM attributes_to_entities "
		     "WHERE attribute = :att AND entity = :oldName");
      query->bindValue(":att", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	query->prepare("DELETE FROM attributes_to_entities "
		       "WHERE attribute = :att AND entity = :oldName");
	query->bindValue(":att", attribute);
	query->bindValue(":oldName", oldName);
	query->exec();
	resetFont(attributesTree);
	query2->exec("SELECT attribute, entity FROM attributes_to_entities");
	while (query2->next()) {
	  QString attribute = query2->value(0).toString();
	  QString entity = query2->value(1).toString();
	  if (entity == oldName) {
	    boldSelected(attributesTree, attribute);
	  }
	}
	valueField->setText("");
	valueField->setEnabled(false);
	valueButton->setEnabled(false);
      }
    }
    setButtons();
    delete query;
    delete query2;
  }
}

void EntityDialog::addAttribute() {
  if (attributesTreeView->currentIndex().isValid() && !fresh) {
    QString currentParent = treeFilter->
      mapToSource(attributesTreeView->currentIndex()).data().toString();
    QString name = "";
    QString description = "";
    attributeDialog = new AttributeDialog(this, ENTITY);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QStandardItem *attribute = new QStandardItem(name);    
      attribute->setToolTip(breakString(description));
      QStandardItem *father = attributesTree->
	itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
      father->appendRow(attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);
      QSqlQuery *query = new QSqlQuery;
      query->prepare("INSERT INTO entity_attributes (name, description, father) "
		     "VALUES (:name, :description, :father)");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":father", currentParent);
      query->exec();
      delete query;
      relationshipsWidget->networkGraph->resetTree();
    }
    delete attributeDialog;
  } else {
    QString name = "";
    QString description = "";
    attributeDialog = new AttributeDialog(this, ENTITY);
    attributeDialog->exec();
    if (attributeDialog->getExitStatus() == 0) {
      name = attributeDialog->getName();
      description = attributeDialog->getDescription();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("INSERT INTO entity_attributes (name, description, father) "
		     "VALUES (:name, :description, :father)");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":father", "NONE");
      query->exec();
      delete query;
      QStandardItem *attribute = new QStandardItem(name);    
      attributesTree->appendRow(attribute);
      attribute->setToolTip(description);
      attribute->setEditable(false);
      // The entity dialog does not necessarily have the relationships widget as its parent.
      relationshipsWidget->networkGraph->resetTree();
    }
    delete attributeDialog;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void EntityDialog::editAttribute() {
    if (attributesTreeView->currentIndex().isValid() && !fresh) {
    QString name = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entity_attributes WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    attributeDialog = new AttributeDialog(this, ENTITY);
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
      currentAttribute->setToolTip(description);
      query->prepare("UPDATE entity_attributes SET name = :newname, description = :newdescription "
		     "WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdescription", description);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE entity_attributes SET father = :newname WHERE father = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_entities SET attribute = :newname "
		     "WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      relationshipsWidget->networkGraph->resetTree();
    }
    delete attributeDialog;
    delete query;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void EntityDialog::mergeAttributes() {
  if (attributesTreeView->currentIndex().isValid() && !fresh) {
    QString origin = attributesTreeView->currentIndex().data().toString();
    QPointer<MergeAttributesDialog> mergeDialog = new MergeAttributesDialog(this, origin, ENTITY);
    mergeDialog->setWindowTitle("Select attribute to merge with");
    mergeDialog->exec();
    if (mergeDialog->getExitStatus() == 0) {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("Merging attributes cannot be undone. "
				     "Are you sure you want to proceed?");
      if (warningBox->exec() == QMessageBox::Yes) {
	QString partner = mergeDialog->getAttribute();
	QSqlQuery *query = new QSqlQuery;
	query->prepare("UPDATE entity_attributes SET father = :new "
		       "WHERE father = :old");
	query->bindValue(":new", partner);
	query->bindValue(":old", origin);
	query->exec();
	query->prepare("DELETE FROM entity_attributes WHERE name = :old");
	query->bindValue(":old", origin);
	query->exec();
	query->prepare("UPDATE attributes_to_entities SET attribute = :new "
		       "WHERE attribute = :old");
	query->bindValue(":new", partner);
	query->bindValue(":old", origin);
	query->exec();
	delete mergeDialog;
	query->prepare("SELECT description FROM entity_attributes WHERE name = :name");
	query->bindValue(":name", partner);
	query->exec();
	query->first();
	QString description = query->value(0).toString();
	attributeDialog = new AttributeDialog(this, ENTITY);
	attributeDialog->submitName(partner);
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
	  query->prepare("UPDATE entity_attributes "
			 "SET name = :newname, description = :newdescription "
			 "WHERE name = :oldname");
	  query->bindValue(":newname", newName);
	  query->bindValue(":newdescription", description);
	  query->bindValue(":oldname", partner);
	  query->exec();
	  query->prepare("UPDATE entity_attributes "
			 "SET father = :newname "
			 "WHERE father = :oldname");
	  query->bindValue(":newname", newName);
	  query->bindValue(":oldname", partner);
	  query->exec();
	  query->prepare("UPDATE attributes_to_entities "
			 "SET attribute = :newname "
			 "WHERE attribute = :oldname");
	  query->bindValue(":newname", newName);
	  query->bindValue(":oldname", partner);
	  query->exec();
	}
	delete query;
	delete attributeDialog;
	attributesTree->sort(0, Qt::AscendingOrder);
	attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
	resetTree();
	fixTree();
      }
    }
  }
}

void EntityDialog::removeUnusedAttributes() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  while (unfinished) {
    query->exec("SELECT name FROM entity_attributes EXCEPT SELECT attribute "
		"FROM attributes_to_entities EXCEPT SELECT father "
		"FROM entity_attributes");
    while (query->next()) {
      QString current = query->value(0).toString();
      query2->prepare("DELETE FROM entity_attributes WHERE name = :current");
      query2->bindValue(":current", current);
      query2->exec();
    }
    query->first();
    if (query->isNull(0)) {
      unfinished = false;
    }
  }
  relationshipsWidget->networkGraph->resetTree();
  this->setCursor(Qt::WaitCursor);
  attributesTreeView->setSortingEnabled(false);
  delete attributesTree;
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  this->setCursor(Qt::ArrowCursor);
  delete query;  
  delete query2;
}

QString EntityDialog::getName() {
  return name;
}

QString EntityDialog::getDescription() {
  return description;
}

int EntityDialog::getExitStatus() {
  return exitStatus;
}

void EntityDialog::setNew() {
  isNew = true;
  delete attributesTree;
  setTree();
}

void EntityDialog::submitName(const QString &newName) {
  nameField->setText(newName);
  name = newName;
  oldName = newName;
  delete attributesTree;
  setTree();
}

void EntityDialog::submitDescription(const QString &newDescription) {
  description = newDescription;
  descriptionField->setText(description);
}

void EntityDialog::setTree() {
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM entity_attributes WHERE father = 'NONE'");
  while (query->next()) {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    QStandardItem *father = new QStandardItem(name);    
    attributesTree->appendRow(father);
    father->setToolTip(description);
    father->setEditable(false);
    buildHierarchy(father, name);
  }
  if (isNew == false) {
    query->prepare("SELECT attribute FROM attributes_to_entities "
		"WHERE entity = :name");
    query->bindValue(":name", oldName);
    query->exec();
    while (query->next()) {
      QString attribute = query->value(0).toString();
      boldSelected(attributesTree, attribute);
    }
  }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void EntityDialog::buildHierarchy(QStandardItem *top, QString name) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entity_attributes WHERE father = :father");
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
  delete query;
}

void EntityDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}
  
void EntityDialog::saveAndClose() {
  description =  descriptionField->toPlainText();
  name = nameField->text();
  description = description.trimmed();
  name = name.trimmed();
  if (name.contains("(") || name.contains(")")) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Name contains illegal character.");
    warningBox->setInformativeText("The name of an entity cannot "
				   "contain the '(' and ')' characters.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (description == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Description missing.");
    warningBox->setInformativeText("An attribute requires a description.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Name missing.");
    warningBox->setInformativeText("The entity requires a name.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Invalid name.");
    warningBox->setInformativeText("You cannot use this name for an entity.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == ENTITIES) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Invalid name.");
    warningBox->setInformativeText("You cannot use this name for an entity.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM entities WHERE name = :name");
  query->bindValue(":name", name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && name != oldName) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Duplicate name.");
    warningBox->setInformativeText("You cannot create entities with identical names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  query->prepare("SELECT name FROM incident_attributes WHERE name = :name");
  query->bindValue(":name", name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Attribute name.");
    warningBox->setInformativeText("Incident attributes and entities cannot share names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (isNew) {
    query->prepare("UPDATE attributes_to_entities SET entity = :entity, new = 0 WHERE new = 1");
    query->bindValue(":entity", name);
    query->exec();
  } else {
    query->prepare("UPDATE attributes_to_entities SET entity = :entity WHERE name = :oldName");
    query->bindValue(":entity", name);
    query->bindValue(":oldName", oldName);
    query->exec();
  }
  exitStatus = 0;
  delete query;
  this->close();
}

void EntityDialog::resetFont(QAbstractItemModel *model, QModelIndex parent) {
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

void EntityDialog::boldSelected(QAbstractItemModel *model, QString name, QModelIndex parent) {
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
	  query->prepare("SELECT attribute, entity FROM attributes_to_entities "
			 "WHERE attribute = :attribute AND entity = :entity");
	  query->bindValue(":attribute", parentName);
	  query->bindValue(":entity", oldName);
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

void EntityDialog::expandTree() {
  attributesTreeView->expandAll();
}

void EntityDialog::collapseTree() {
  attributesTreeView->collapseAll();
}

void EntityDialog::fixTree() {
  resetFont(attributesTree);
  QSqlQuery *query = new QSqlQuery;
  if (isNew) {
    query->exec("SELECT attribute, new FROM attributes_to_entities");
    while (query->next()) {
      QString attribute = query->value(0).toString();
      int mark = query->value(1).toInt();
      if (mark == 1) {
	boldSelected(attributesTree, attribute);
      }
    }
  } else {
    query->exec("SELECT attribute, entity FROM attributes_to_entities");
    while (query->next()) {
      QString attribute = query->value(0).toString();
      QString entity = query->value(1).toString();
      if (entity == oldName) {
	boldSelected(attributesTree, attribute);
      }
    }
  }
  delete query;
}

void EntityDialog::setButtons() {
  if (fresh) {
    fresh = false;
  }
  if (attributesTreeView->currentIndex().isValid()) {
    QString currentAttribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    if (isNew) {
      bool empty = false;
      query->prepare("SELECT attribute, new FROM attributes_to_entities "
		     "WHERE attribute = :att AND new = 1");
      query->bindValue(":att", currentAttribute);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	unassignAttributeButton->setEnabled(true);
      } else {
	unassignAttributeButton->setEnabled(false);
      }
    } else {
      bool empty = false;
      query->prepare("SELECT attribute FROM attributes_to_entities "
		     "WHERE attribute = :att AND entity = :oldName");
      query->bindValue(":att", currentAttribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	unassignAttributeButton->setEnabled(true);
      } else {
	unassignAttributeButton->setEnabled(false);
      }
    }
    delete query;
    assignAttributeButton->setEnabled(true);
    editAttributeButton->setEnabled(true);
    mergeAttributesButton->setEnabled(true);
  } else {
   assignAttributeButton->setEnabled(false);
   editAttributeButton->setEnabled(false);
   unassignAttributeButton->setEnabled(false);
   mergeAttributesButton->setEnabled(false);
  }
}

bool EntityDialog::eventFilter(QObject *object, QEvent *event) {
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

void EntityDialog::resetTree() {
  delete attributesTree;
  setTree();
}

void EntityDialog::setRelationshipsWidget(RelationshipsWidget *rw) {
  relationshipsWidget = rw;
}
