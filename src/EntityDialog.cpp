#include "../include/EntityDialog.h"

EntityDialog::EntityDialog(QWidget *parent) : QDialog(parent) {
  name = "";
  oldName = "";
  exitStatus = 1;
  isNew = false;
  
  // I have to build these as well.
  incidentsModel = new QSqlTableModel(this);  
  incidentsModel->setTable("incidents");
  incidentsModel->select();

  attributesModel = new QSqlTableModel(this);
  attributesModel->setTable("entity_attributes");
  attributesModel->select();

  assignedModel = new QSqlTableModel(this);
  assignedModel->setTable("attributes_to_entities");
  assignedModel->select();

  attributesTreeView = new DeselectableTreeViewEntities(this);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->setDragEnabled(true);
  attributesTreeView->setAcceptDrops(true);
  attributesTreeView->setDropIndicatorShown(true);
  attributesTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  attributesTreeView->setExpandsOnDoubleClick(false);
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree(); 
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);

  // First we declare the entities of this dialog.
  nameLabel = new QLabel(tr("<b>Name:</b>"));
  nameField = new QLineEdit();

  descriptionLabel = new QLabel(tr("<b>Description:</b>"));
  descriptionField = new QTextEdit();

  valueLabel = new QLabel(tr("<b>Value:</b>"));
  valueField = new QLineEdit();
    
  attributesLabel = new QLabel(tr("<h2>Attributes</h2>"));
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"));

  attributesFilterField = new QLineEdit();
  
  valueButton = new QPushButton("Store value");
  valueButton->setEnabled(false);
  assignAttributeButton = new QPushButton("Assign attribute");
  unassignAttributeButton = new QPushButton("Unassign attribute");
  addAttributeButton = new QPushButton(tr("New attribute"));
  editAttributeButton = new QPushButton(tr("Edit attribute"));
  removeUnusedAttributesButton = new QPushButton(tr("Remove unused attributes"));
  cancelCloseButton = new QPushButton(tr("Cancel"));
  saveCloseButton = new QPushButton(tr("Save entity"));
  
  // Then we wire the signals of the dialog.
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(setFilter(const QString &)));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(attributesTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(getValue()));
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
  rightLayout->addWidget(attributesLabel);
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
}
    
void EntityDialog::setValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQuery *query = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    if (isNew) {
      query->prepare("UPDATE attributes_to_entities SET value = :val WHERE attribute = :attribute AND new = :new");
      query->bindValue(":val", valueField->text());
      query->bindValue(":attribute", attribute);
      query->bindValue(":new", 1);
      query->exec();
      valueField->setText("");
    } else {
      query->prepare("UPDATE attributes_to_entities SET value = :val WHERE attribute = :attribute AND entity = :oldName");
      query->bindValue(":val", valueField->text());
      query->bindValue(":attribute", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      valueField->setText("");
    }
    delete query;
  }
}

void EntityDialog::getValue() {
  if (attributesTreeView->currentIndex().isValid()) {
    QString attribute = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    if (isNew) {
      query->prepare("SELECT attribute, value FROM attributes_to_entities WHERE attribute =:att AND new = 1");
      query->bindValue(":att", attribute);
      query->exec();
      query->first();
      if (!(query->isNull(0))) {
	valueButton->setEnabled(true);
      } else {
	valueButton->setEnabled(false);
	valueField->setText("");
      }
      if (!(query->isNull(1))) {
	QString value = query->value(1).toString();
	valueField->setText(value);
      } else {
	valueField->setText("");
      }
    } else {
      query->prepare("SELECT attribute, value FROM attributes_to_entities WHERE attribute =:att AND name = :oldName");
      query->bindValue(":att", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      query->first();
      if (!(query->isNull(0))) {
	valueButton->setEnabled(true);
      } else {
	valueButton->setEnabled(false);
	valueField->setText("");
      }
      if (!(query->isNull(1))) {
	QString value = query->value(1).toString();
	valueField->setText(value);
      } else {
	valueField->setText("");
      }
    }
    delete query;
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
      assignedModel->select();
      int max = assignedModel->rowCount();
      bool empty = false;
      query->prepare("SELECT attribute, new FROM attributes_to_entities WHERE attribute = :att AND new = 1");
      query->bindValue(":att", attribute);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (empty) {
	assignedModel->insertRow(max);
	assignedModel->setData(assignedModel->index(max, 1), attribute);
	assignedModel->setData(assignedModel->index(max, 4), 1);
	assignedModel->submitAll();
	QStandardItem *currentAttribute = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
	QFont font;
	font.setBold(true);
	currentAttribute->setFont(font);
	valueButton->setEnabled(true);
      }
    } else {
      assignedModel->select();
      int max = assignedModel->rowCount();
      bool empty = false;
      query->prepare("SELECT attribute, new FROM attributes_to_entities WHERE attribute = :att AND entity = :oldName");
      query->bindValue(":att", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (empty) {
	assignedModel->insertRow(max);
	assignedModel->setData(assignedModel->index(max, 1), attribute);
	assignedModel->setData(assignedModel->index(max, 2), name);
	assignedModel->submitAll();
	QStandardItem *currentAttribute = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
	QFont font;
	font.setBold(true);
	currentAttribute->setFont(font);
	valueButton->setEnabled(true);
      }
    }
    delete query;
  }
}

void EntityDialog::unassignAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    QSqlQuery *query = new QSqlQuery;
    QString attribute = attributesTreeView->currentIndex().data().toString();
    assignedModel->select();
    bool empty = false;
    if (isNew) {
      query->prepare("SELECT attribute FROM attributes_to_entities WHERE attribute = :att AND new = 1");
      query->bindValue(":att", attribute);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	query->prepare("DELETE FROM attributes_to_entities WHERE attribute = :att AND new = 1");
	query->bindValue(":att", attribute);
	query->exec();
	assignedModel->select();
	QStandardItem *currentAttribute = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
	QFont font;
	font.setBold(false);
	currentAttribute->setFont(font);
	valueButton->setEnabled(false);
      }
    } else {
      query->prepare("SELECT attribute, entity FROM attributes_to_entities WHERE attribute = :att AND entity = :oldName");
      query->bindValue(":att", attribute);
      query->bindValue(":oldName", oldName);
      query->exec();
      query->first();
      empty = query->isNull(0);
      if (!empty) {
	query->prepare("DELETE FROM attributes_to_entities WHERE attribute = :att AND entity = :oldName");
	query->bindValue(":att", attribute);
	query->bindValue(":oldName", oldName);
	query->exec();
	assignedModel->select();
	QStandardItem *currentAttribute = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
	QFont font;
	font.setBold(false);
	currentAttribute->setFont(font);
	valueButton->setEnabled(false);
      }
    }
    delete query;
  }
}

void EntityDialog::addAttribute() {
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
      QStandardItem *father = attributesTree->itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
      father->appendRow(attribute);
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
      attribute->setToolTip(description);
      attribute->setEditable(false);
    }
    delete attributeDialog;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void EntityDialog::editAttribute() {
    if (attributesTreeView->currentIndex().isValid()) {
    QString name = attributesTreeView->currentIndex().data().toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description FROM entity_attributes WHERE name = :name");
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
      currentAttribute->setToolTip(description);
      query->prepare("UPDATE entity_attributes SET name = :newname, description = :newdescription WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdescription", description);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE entity_attributes SET father = :newname WHERE father = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
      query->prepare("UPDATE attributes_to_entities SET attribute = :newname WHERE attribute = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", name);
      query->exec();
    }
    delete attributeDialog;
    delete query;
  }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
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
  this->setCursor(Qt::WaitCursor);
  attributesTreeView->setSortingEnabled(false);
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
  oldName = newName;
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
    query->exec("SELECT attribute, entity FROM attributes_to_entities");
    while (query->next()) {
      QString attribute = query->value(0).toString();
      QString entity = query->value(1).toString();
      if (entity == oldName) {
	boldSelected(attributesTree, attribute);
      }
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
  if (description == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Description missing.");
    warningBox->setInformativeText("An attribute requires a description.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Name missing.");
    warningBox->setInformativeText("An attribute requires a label.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox;
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
    QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Duplicate name.");
    warningBox->setInformativeText("You cannot create entities with identical names.");
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

void EntityDialog::boldSelected(QAbstractItemModel *model, QString name, QModelIndex parent) {
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
