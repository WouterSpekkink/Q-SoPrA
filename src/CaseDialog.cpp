#include "../include/CaseDialog.h"

CaseDialog::CaseDialog(QWidget *parent) : QDialog(parent) {
  name = "";
  description = "";
  oldName = "";
  exitStatus = 1;
  
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

  setWindowTitle("Add/Edit case");
  setLayout(mainLayout);
}

void CaseDialog::setName(const QString &newName) {
  name = newName;
}

void CaseDialog::setDescription(const QString &newDescription) {
  description =  newDescription;
  descriptionField->setText(newDescription);
}

QString CaseDialog::getName() {
  return name;
}

void CaseDialog::submitName(const QString &submittedName) {
  name = submittedName;
  oldName = submittedName;
  nameField->setText(submittedName);
}

QString CaseDialog::getDescription() {
  return description;
}

int CaseDialog::getExitStatus() {
  return exitStatus;
}

void CaseDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

// TO DO: Check for cases with same name.
void CaseDialog::saveAndClose() {
  description =  descriptionField->toPlainText();
  name = name.trimmed();
  description = description.trimmed();
  if (description == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Description missing.");
    warningBox->setInformativeText("An case requires a description.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Name missing.");
    warningBox->setInformativeText("An case requires a label.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM cases "
		 "WHERE name = :name");
  query->bindValue(":name", name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && name != oldName) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Duplicate name.");
    warningBox->setInformativeText("You cannot create cases "
				   "with identical names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  exitStatus = 0;
  delete query;
  this->close();
}

