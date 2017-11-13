#include "../include/CoderDialog.h"

CoderDialog::CoderDialog(QWidget *parent) : QDialog(parent) {
  name = "";
  subName = "";
  exitStatus = 1;
  nameLabel = new QLabel(tr("Coder name:"), this);
  nameField = new QLineEdit(this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionsLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Add / Edit Coder");
}

QString CoderDialog::getName() {
  return name;
}

void CoderDialog::submitName(const QString &submittedName) {
  name = submittedName;
  subName = submittedName;
  nameField->setText(submittedName);
}

int CoderDialog::getExitStatus() {
  return exitStatus;
}

void CoderDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void CoderDialog::saveAndClose() {
  name = nameField->text().trimmed();
  if (name == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Name required.");
    warningBox->setInformativeText("A coder requires a name.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM coders WHERE name = :name");
  query->bindValue(":name", name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && name != subName) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Duplicate name.");
    warningBox->setInformativeText("You cannot create coders with identical names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  delete query;
  exitStatus = 0;
  this->close();
}
