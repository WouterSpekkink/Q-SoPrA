#include "../include/MergeRelationshipsDialog.h"

MergeRelationshipsDialog::MergeRelationshipsDialog(QWidget *parent,
						   QVector<DirectedEdge*> *directed,
						   QVector<UndirectedEdge*> *undirected)
  : QDialog(parent) {
  
  pDirected = directed;
  pUndirected = undirected;
  name = "";
  description = "";
  directedness = "";
  relOne = DEFAULT;
  relTwo = DEFAULT;
  exitStatus = 1;  

  relOneLabel = new QLabel(tr("Relationship one:"), this);
  relTwoLabel = new QLabel(tr("Relationship two:"), this);
  nameLabel = new QLabel(tr("Merged relationship:"), this);
  descriptionLabel = new QLabel(tr("Description:"), this);
  
  nameField = new QLineEdit();

  descriptionField = new QTextEdit();

  relOneComboBox = new QComboBox(this);
  relOneComboBox->addItem(DEFAULT);
  relTwoComboBox = new QComboBox(this);
  relTwoComboBox->addItem(DEFAULT);
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save new relationship"), this);

  connect(relOneComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setRelOne(const QString &)));
  connect(relTwoComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setRelTwo(const QString &)));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> relOneLayout = new QHBoxLayout;
  relOneLayout->addWidget(relOneLabel);
  relOneLayout->addWidget(relOneComboBox);
  mainLayout->addLayout(relOneLayout);
  QPointer<QHBoxLayout> relTwoLayout = new QHBoxLayout;
  relTwoLayout->addWidget(relTwoLabel);
  relTwoLayout->addWidget(relTwoComboBox);
  mainLayout->addLayout(relTwoLayout);
  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  QPointer<QHBoxLayout> descriptionLayout = new QHBoxLayout;
  descriptionLayout->addWidget(descriptionLabel);
  descriptionLayout->addWidget(descriptionField);
  mainLayout->addLayout(descriptionLayout);

  QPointer<QFrame> middleLine = new QFrame;
  middleLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(middleLine);

  QPointer<QHBoxLayout> optionLayout = new QHBoxLayout;
  optionLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionLayout);

  setLayout(mainLayout);
  setWindowTitle("Merge relationships");
  setFixedHeight(sizeHint().height());
}

void MergeRelationshipsDialog::setRelationships(QVector<QString> submittedRelationships) {
  QVectorIterator<QString> it(submittedRelationships);
  while (it.hasNext()) {
    QString currentRel = it.next();
    relOneComboBox->addItem(currentRel);
    relTwoComboBox->addItem(currentRel);
  }
}

void MergeRelationshipsDialog::setRelOne(const QString &name) {
  relOne = name;
}

void MergeRelationshipsDialog::setRelTwo(const QString &name) {
  relTwo = name;
}

QString MergeRelationshipsDialog::getRelOne() {
  return relOne;
}

QString MergeRelationshipsDialog::getRelTwo() {
  return relTwo;
}

QString MergeRelationshipsDialog::getName() {
  return name;
}

QString MergeRelationshipsDialog::getDescription() {
  return description;
}

QString MergeRelationshipsDialog::getDirectedness() {
  return directedness;
}

int MergeRelationshipsDialog::getExitStatus() {
  return exitStatus;
}

void MergeRelationshipsDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void MergeRelationshipsDialog::saveAndClose() {
  description =  descriptionField->toPlainText().trimmed();
  name = nameField->text().trimmed();
  if (relOne == DEFAULT || relTwo == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("No valid relationships selected.");
    warningBox->setInformativeText("One or both of the selected relationships are invalid.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (relOne == relTwo) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Relationships should be different.");
    warningBox->setInformativeText("You cannot select the same relationship twice.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness FROM relationship_types "
		 "WHERE name = :relOne");
  query->bindValue(":relOne", relOne);
  query->exec();
  query->first();
  /*
    If a relationship type is not in the list of types, 
    then it was created through multimode transformation, which means it 
    is always undirected if not found in the list.
  */
  QString directionOne = UNDIRECTED; 
  if (!(query->isNull(0))) {  
    directionOne = query->value(0).toString();
  }    
  query->prepare("SELECT directedness FROM relationship_types "
		 "WHERE name = :relTwo");
  query->bindValue(":relTwo", relTwo);
  query->exec();
  query->first();
  QString directionTwo = UNDIRECTED;
  if (!(query->isNull(0))) {  
    directionTwo = query->value(0).toString();
  }
  if (directionOne != directionTwo) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Directedness is different.");
    warningBox->setInformativeText("You cannot merge relationships with different a directedness.");
    warningBox->exec();
    delete warningBox;
    return;
  } else {
    directedness = directionOne;
  }
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
  bool found = false;
  QVectorIterator<DirectedEdge*> it(*pDirected);
  while (it.hasNext()) {
    DirectedEdge* directed = it.next();
    if (directed->getType() == name) {
      found = true;
    }
  }
  QVectorIterator<UndirectedEdge*> it2(*pUndirected);
  while (it2.hasNext()) {
    UndirectedEdge* undirected = it2.next();
    if (undirected->getType() == name) {
      found = true;
    }
  }
  if (found) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Name already exists.");
    warningBox->setInformativeText("You already have a relationship type with that name.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  delete query;
  exitStatus = 0;
  this->close();
}



