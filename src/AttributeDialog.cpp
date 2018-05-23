#include "../include/AttributeDialog.h"

AttributeDialog::AttributeDialog(QWidget *parent, QString submittedType) : QDialog(parent) {
  name = "";
  description = "";
  oldName = "";
  exitStatus = 1;
  type = submittedType;
  
  nameLabel = new QLabel("<b>Name:</b>", this);
  descriptionLabel = new QLabel("<b>Description:</b>", this);

  cancelCloseButton = new QPushButton("Cancel", this);
  saveCloseButton = new QPushButton("Save", this);

  nameField = new QLineEdit(this);

  descriptionField = new QTextEdit(this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(nameField, SIGNAL(textChanged(const QString &)), this, SLOT(setName(const QString &)));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  mainLayout->addWidget(descriptionLabel);
  mainLayout->addWidget(descriptionField);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  buttonLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(buttonLayout);

  setWindowTitle("Add/Edit attribute");
  setLayout(mainLayout);
}

void AttributeDialog::setName(const QString &newName) {
  name = newName;
}

void AttributeDialog::setDescription(const QString &newDescription) {
  description =  newDescription;
  descriptionField->setText(newDescription);
}

QString AttributeDialog::getName() {
  return name;
}

void AttributeDialog::submitName(const QString &submittedName) {
  name = submittedName;
  oldName = submittedName;
  nameField->setText(submittedName);
}

QString AttributeDialog::getDescription() {
  return description;
}

int AttributeDialog::getExitStatus() {
  return exitStatus;
}

void AttributeDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void AttributeDialog::saveAndClose() {
  description =  descriptionField->toPlainText();
  name = name.trimmed();
  description = description.trimmed();
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
    warningBox->setInformativeText("An attribute requires a label.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  if (type == INCIDENT) {
    query->prepare("SELECT name FROM incident_attributes WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
  } else if (type == ENTITY) {
    query->prepare("SELECT name FROM entity_attributes WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
  }
  empty = query->isNull(0);
  if (!empty && name != oldName) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Duplicate name.");
    warningBox->setInformativeText("You cannot create attributes with identical names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (type == INCIDENT) {
    query->prepare("SELECT name FROM entities WHERE name = :name");
    query->bindValue(":name", name);
    query->exec();
    query->first();
  }
  empty = query->isNull(0);
  if (!empty) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Entity name.");
    warningBox->setInformativeText("Attributes and entities cannot share names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == ENTITIES) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Cannot use this name.");
    warningBox->setInformativeText("This name has been reserved entities.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  exitStatus = 0;
  qDebug() << "THIS";
  delete query;
  this->close();
}

