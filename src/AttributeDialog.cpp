#include "../include/AttributeDialog.h"

AttributeDialog::AttributeDialog(QWidget *parent) : QDialog(parent) {
  name = "";
  description = "";
  oldName = "";
  exitStatus = 1;
  
  nameLabel = new QLabel("Name:");
  descriptionLabel = new QLabel("Description:");

  cancelCloseButton = new QPushButton("Cancel");
  saveCloseButton = new QPushButton("Save");

  nameField = new QLineEdit();

  descriptionField = new QTextEdit();

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(nameField, SIGNAL(textChanged(const QString &)), this, SLOT(setName(const QString &)));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  QPointer<QHBoxLayout> descriptionLayout = new QHBoxLayout;
  descriptionLayout->addWidget(descriptionLabel);
  descriptionLayout->addWidget(descriptionField);
  mainLayout->addLayout(descriptionLayout);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(cancelCloseButton);
  buttonLayout->addWidget(saveCloseButton);
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
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM incident_attributes WHERE name = :name");
  query->bindValue(":name", name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && name != oldName) {
   QPointer <QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Duplicate name.");
    warningBox->setInformativeText("You cannot create attributes with identical names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  exitStatus = 0;
  delete query;
  this->close();
}
