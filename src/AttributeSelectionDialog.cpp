#include "../include/AttributeSelectionDialog.h"

AttributeSelectionDialog::AttributeSelectionDialog(QWidget *parent, QString submittedType) : QDialog(parent) {
  type = submittedType;
  exitStatus = 1;
  chosenAttribute = DEFAULT;
  checked = false;

  attributeLabel = new QLabel(tr("Choose attribute:"), this);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), this);

  attributesTreeView = new DeselectableTreeView(this);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  attributesTreeView->installEventFilter(this);

  attributesFilterField = new QLineEdit(this);

  includeValuesCheckBox = new QCheckBox(tr("Include values"), this);
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setAttribute()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(attributeLabel);
  mainLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributesFilterLabel);
  filterLayout->addWidget(attributesFilterField);
  mainLayout->addLayout(filterLayout);
  mainLayout->addWidget(includeValuesCheckBox);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setMinimumWidth(400);
  setMinimumHeight(600);
  setWindowTitle("Select attribute");
}

void AttributeSelectionDialog::setTree() {
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  if (type == ENTITY) {
    query->exec("SELECT name, description FROM entity_attributes WHERE father = 'NONE'");
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
  } else if (type == INCIDENT) {
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
  }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void AttributeSelectionDialog::buildHierarchy(QStandardItem *top, QString name) {
  QSqlQuery *query = new QSqlQuery;
  if (type == ENTITY) {
    query->prepare("SELECT name, description FROM entity_attributes WHERE  father = :father");
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
  } else {
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
  }
  delete query;
}

void AttributeSelectionDialog::changeFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}
 
void AttributeSelectionDialog::setAttribute() {
  if (attributesTreeView->currentIndex().isValid()) {
    chosenAttribute = attributesTreeView->currentIndex().data().toString();
  }
}

void AttributeSelectionDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void AttributeSelectionDialog::saveAndClose() {
  if (chosenAttribute == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("No attribute chosen.");
    warningBox->setInformativeText("You have to choose an attribute to proceed.");
    warningBox->exec();
    delete warningBox;
    return;
  } else {
    exitStatus = 0;
    checked = includeValuesCheckBox->isChecked();
    this->close();
  }
}

QString AttributeSelectionDialog::getAttribute() {
    return chosenAttribute;
}

int AttributeSelectionDialog::getExitStatus() {
  return exitStatus;
}

bool AttributeSelectionDialog::getChecked() {
  return checked;
}
