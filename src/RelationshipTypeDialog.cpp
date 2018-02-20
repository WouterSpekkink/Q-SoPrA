#include "../include/RelationshipTypeDialog.h"

RelationshipTypeDialog::RelationshipTypeDialog(QWidget *parent) : QDialog(parent) {
  directedness = DIRECTED;
  name = "";
  subName = "";
  description = "";
  exitStatus = 1;
    
  nameLabel = new QLabel(tr("<b>Label:</b>"), this);
  nameField = new QLineEdit();

  descriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  descriptionField = new QTextEdit();
  directednessLabel = new QLabel(tr("<b>Directedness:</b>"), this);
  directedButton = new QPushButton(tr("Directed"), this);
  directedButton->setCheckable(true);
  undirectedButton = new QPushButton(tr("Undirected"), this);
  undirectedButton->setCheckable(true);
  directedButton->setChecked(true);
  undirectedButton->setChecked(false);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save relationship type"), this);

  // Then we wire the signals.
  connect(nameField, SIGNAL(textChanged(const QString &)), this, SLOT(setName(const QString &)));
  connect(directedButton, SIGNAL(clicked()), this, SLOT(checkDirectedButton()));
  connect(undirectedButton, SIGNAL(clicked()), this, SLOT(checkUndirectedButton()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  mainLayout->addWidget(descriptionLabel);
  mainLayout->addWidget(descriptionField);
  QPointer<QHBoxLayout> directednessLayout = new QHBoxLayout;
  directednessLayout->addWidget(directednessLabel);
  directednessLayout->addWidget(directedButton);
  directednessLayout->addWidget(undirectedButton);
  mainLayout->addLayout(directednessLayout);

  QPointer<QFrame> middleLine = new QFrame;
  middleLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(middleLine);

  QPointer<QHBoxLayout> optionLayout = new QHBoxLayout;
  optionLayout->addWidget(cancelCloseButton);
  optionLayout->addWidget(saveCloseButton);

  mainLayout->addLayout(optionLayout);

  setLayout(mainLayout);
  setWindowTitle("Add / Edit Relationship Type");
  setFixedHeight(sizeHint().height());
  setMinimumWidth(400);
  // This finishes the constructor.
}

QString RelationshipTypeDialog::getName() {
  return name;
}

QString RelationshipTypeDialog::getDescription() {
  return description;
}

QString RelationshipTypeDialog::getDirectedness() {
  return directedness;
}

int RelationshipTypeDialog::getExitStatus() {
  return exitStatus;
}

void RelationshipTypeDialog::submitName(const QString &submittedName) {
  name = submittedName;
  subName = submittedName;
  nameField->setText(name);
}

void RelationshipTypeDialog::submitDescription(const QString &submittedDescription) {
  description = submittedDescription;
  descriptionField->setText(description);
}

void RelationshipTypeDialog::submitDirectedness(const QString &submittedDirectedness) {
  directedness = submittedDirectedness;
   if (directedness == DIRECTED) {
    directedButton->setChecked(true);
    undirectedButton->setChecked(false);
  } else if (directedness == UNDIRECTED) {
    directedButton->setChecked(false);
    undirectedButton->setChecked(true);
  }
}

void RelationshipTypeDialog::setName(const QString &newName) {
  name = newName;
}

void RelationshipTypeDialog::checkDirectedButton() {
  directedButton->setChecked(true);
  undirectedButton->setChecked(false);
  directedness = DIRECTED;
}

void RelationshipTypeDialog::checkUndirectedButton() {
  directedButton->setChecked(false);
  undirectedButton->setChecked(true);
  directedness = UNDIRECTED;
}

void RelationshipTypeDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void RelationshipTypeDialog::saveAndClose() {
  description = descriptionField->toPlainText().trimmed();
  name = name.trimmed();
  if (description == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Description required.");
    warningBox->setInformativeText("A relationship type requires a description.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (name == "") {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Name required.");
    warningBox->setInformativeText("A relationship type requires a name.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM relationship_types WHERE name = :name");
  query->bindValue(":name", name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && name != subName) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Duplicate name.");
    warningBox->setInformativeText("You cannot create relationship types with identical names.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  delete query;
  exitStatus = 0;
  this->close();
}
