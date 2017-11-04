#include "../include/EntityDialog.h"

EntityDialog::EntityDialog(QWidget *parent) : QDialog(parent) {
  name = "";
  oldName = "";
  exitStatus = 1;
    
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

  attributesTreeView = new DeselectableTreeView(this);
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
  setTree(); // Still have to write this.
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);

  // First we declare the entities of this dialog.
  nameLabel = new QLabel(tr("<b>Name:</b>"));
  nameField = new QLineEdit();

  descriptionLabel = new QLabel(tr("<b>Description:</b>"));
  descriptionField = new QPlainTextEdit();

  valueLabel = new QLabel(tr("<b>Value:</b>"));
  valueField = new QLineEdit();
  valueField->setEnabled(false);
    
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
  connect(nameField, SIGNAL(textChanged(const QString &)), this, SLOT(setName(const QString &)));
  connect(valueField, SIGNAL(textChanged(const QString &)), this, SLOT(setValue(const QString &)));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)), this, SLOT(setFilter(const QString &)));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
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

  // Temporary table to store attributes and values until the dialog is destroyed.
  QSqlQuery *query = new QSqlQuery;
  query->exec("CREATE TABLE temp_attributes_to_entities "
	      "(attribute text, "
	      "value text)");
}
    
void EntityDialog::setName(const QString &newName) {
  name = newName;
}

void EntityDialog::setValue(const QString &newValue) {
  // TODO
}

void EntityDialog::setFilter(const QString &text) {
  // TODO
}

void EntityDialog::assignAttribute() {
  // TODO
  // I should make a vector of QString with the attributes that should be written
  // and only write them into the sql table when the save button is actually used.
  // QVector.push_back(currentAttribute);

  // Or maybe temporarily add a new table, copy its contents to the main table, and delete it.
}

void EntityDialog::unassignAttribute() {
  // TODO
}

void EntityDialog::addAttribute() {
  // TODO
}

void EntityDialog::editAttribute() {
  // TODO
}

void EntityDialog::removeUnusedAttributes() {
  // TODO
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
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
}

void EntityDialog::buildHierarchy(QStandardItem *top, QString name) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entity_attributes WHERE  father = :father");
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

void EntityDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}
  
void EntityDialog::saveAndClose() {
  // To DO
  // See add attrributes comments.

  exitStatus = 0;
  this->close();
}

