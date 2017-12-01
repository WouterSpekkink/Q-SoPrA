#include "../include/MultimodeDialog.h"

MultimodeDialog::MultimodeDialog(QWidget *parent,
				 QVector<DirectedEdge*> *directed,
				 QVector<UndirectedEdge*> *undirected) : QDialog(parent) {
  pDirected = directed;
  pUndirected = undirected;
  name = "";
  description = "";
  modeOne = DEFAULT;
  modeTwo = DEFAULT;
  relationship = DEFAULT;
  exitStatus = 1;

  modeOneLabel = new QLabel(tr("Mode one:"), this);
  modeOneLabel->setToolTip("<FONT SIZE = 3>The new relationship will be created between nodes of this mode.</FONT>");
  modeTwoLabel = new QLabel(tr("Mode two:"), this);
  modeTwoLabel->setToolTip("<FONT SIZE = 3>Nodes of this mode will be used to determine which nodes of mode one should be related.</FONT>");
  relationshipLabel = new QLabel(tr("Relationship:"), this);
  relationshipLabel->setToolTip("<FONT SIZE = 3>The relationship between the two modes that will be used in the transformation.</FONT>");
  nameLabel = new QLabel(tr("New relationship:"), this);
  descriptionLabel = new QLabel(tr("Description:"), this);
  
  nameField = new QLineEdit();

  descriptionField = new QTextEdit();

  modeOneComboBox = new QComboBox(this);
  modeOneComboBox->addItem(DEFAULT);
  modeTwoComboBox = new QComboBox(this);
  modeTwoComboBox->addItem(DEFAULT);
  relationshipComboBox = new QComboBox(this);
  relationshipComboBox->addItem(DEFAULT);
  relationshipComboBox->setEnabled(false);
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save relationship type"), this);

  connect(modeOneComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setModeOne(const QString &)));
  connect(modeTwoComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setModeTwo(const QString &)));
  connect(relationshipComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setRelationship(const QString &)));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> modeOneLayout = new QHBoxLayout;
  modeOneLayout->addWidget(modeOneLabel);
  modeOneLayout->addWidget(modeOneComboBox);
  mainLayout->addLayout(modeOneLayout);
  QPointer<QHBoxLayout> modeTwoLayout = new QHBoxLayout;
  modeTwoLayout->addWidget(modeTwoLabel);
  modeTwoLayout->addWidget(modeTwoComboBox);
  mainLayout->addLayout(modeTwoLayout);
  QPointer<QHBoxLayout> relationshipLayout = new QHBoxLayout;
  relationshipLayout->addWidget(relationshipLabel);
  relationshipLayout->addWidget(relationshipComboBox);
  mainLayout->addLayout(relationshipLayout);
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
  setWindowTitle("Multimode transformation");
  setFixedHeight(sizeHint().height());
}

void MultimodeDialog::setModes(QVector<QString> submittedModes) {
  modes = submittedModes;
  QVectorIterator<QString> it(modes);
  while (it.hasNext()) {
    QString currentMode = it.next();
    modeOneComboBox->addItem(currentMode);
    modeTwoComboBox->addItem(currentMode);
  }
}

void MultimodeDialog::setModeOne(const QString &name) {
  modeOne = name;
  if (modeOne != DEFAULT && modeTwo != DEFAULT && modeOne != modeTwo) {
    relationshipComboBox->clear();
    relationshipComboBox->addItem(DEFAULT);
    QList<QString> include;
    QVectorIterator<DirectedEdge*> it(*pDirected);
    while (it.hasNext()) {
      DirectedEdge *directed = it.next();
      if (directed->startItem()->getMode() == modeOne &&
	  directed->endItem()->getMode() == modeTwo &&
	  directed->isVisible()) {
	include.push_back(directed->getType());;
      } else if (directed->startItem()->getMode() == modeTwo &&
		 directed->endItem()->getMode() == modeOne &&
		 directed->isVisible()) {
	include.push_back(directed->getType());;
      }
    }
    
    QVectorIterator<UndirectedEdge*> it2(*pUndirected);
    while (it2.hasNext()) {
      UndirectedEdge *undirected = it2.next();
      if (undirected->startItem()->getMode() == modeOne &&
	  undirected->endItem()->getMode() == modeTwo &&
	  undirected->isVisible()) {
	include.push_back(undirected->getType());;
      } else if (undirected->startItem()->getMode() == modeTwo &&
		 undirected->endItem()->getMode() == modeOne &&
		 undirected->isVisible()) {
	include.push_back(undirected->getType());;
      }
    }
    qSort(include);
    QSet<QString> includeSet = QSet<QString>::fromList(include);
    QSet<QString>::iterator it3;
    for (it3 = includeSet.begin(); it3 != includeSet.end(); it3++) {
      relationshipComboBox->addItem(*it3);
    }
    relationshipComboBox->setEnabled(true);
  } else {
    relationshipComboBox->clear();
    relationshipComboBox->addItem(DEFAULT);
    relationshipComboBox->setEnabled(false);
  }
}

void MultimodeDialog::setModeTwo(const QString &name) {
  modeTwo = name;
  if (modeOne != DEFAULT && modeTwo != DEFAULT && modeOne != modeTwo) {
    relationshipComboBox->clear();
    relationshipComboBox->addItem(DEFAULT);
    QList<QString> include;
    QVectorIterator<DirectedEdge*> it(*pDirected);
    while (it.hasNext()) {
      DirectedEdge *directed = it.next();
      if (directed->startItem()->getMode() == modeOne &&
	  directed->endItem()->getMode() == modeTwo &&
	  directed->isVisible()) {
	include.push_back(directed->getType());;
      } else if (directed->startItem()->getMode() == modeTwo &&
		 directed->endItem()->getMode() == modeOne &&
		 directed->isVisible()) {
	include.push_back(directed->getType());;
      }
    }
    QVectorIterator<UndirectedEdge*> it2(*pUndirected);
    while (it2.hasNext()) {
      UndirectedEdge *undirected = it2.next();
      if (undirected->startItem()->getMode() == modeOne &&
	  undirected->endItem()->getMode() == modeTwo &&
	  undirected->isVisible()) {
	include.push_back(undirected->getType());;
      } else if (undirected->startItem()->getMode() == modeTwo &&
		 undirected->endItem()->getMode() == modeOne &&
		 undirected->isVisible()) {
	include.push_back(undirected->getType());;
      }
    }
    qSort(include);
    QSet<QString> includeSet = QSet<QString>::fromList(include);
    QSet<QString>::iterator it3;
    for (it3 = includeSet.begin(); it3 != includeSet.end(); it3++) {
      relationshipComboBox->addItem(*it3);
    }
    relationshipComboBox->setEnabled(true);
  } else {
    relationshipComboBox->clear();
    relationshipComboBox->addItem(DEFAULT);
    relationshipComboBox->setEnabled(false);
  }
}

void MultimodeDialog::setRelationship(const QString &name) {
  relationship = name;
}

QString MultimodeDialog::getModeOne() {
  return modeOne;
}

QString MultimodeDialog::getModeTwo() {
  return modeTwo;
}

QString MultimodeDialog::getRelationship() {
  return relationship;
}

QString MultimodeDialog::getName() {
  return name;
}

QString MultimodeDialog::getDescription() {
  return description;
}

int MultimodeDialog::getExitStatus() {
  return exitStatus;
}

void MultimodeDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void MultimodeDialog::saveAndClose() {
  description =  descriptionField->toPlainText().trimmed();
  name = nameField->text().trimmed();
  if (modeOne == DEFAULT || modeTwo == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("No valid modes selected.");
    warningBox->setInformativeText("One or both of the selected modes are invalid.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (modeOne == modeTwo) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("Modes should be different.");
    warningBox->setInformativeText("You cannot select the same mode twice.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  if (relationship == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("No relationship selected.");
    warningBox->setInformativeText("A relationship must be selected for the transformation to be possible.");
    warningBox->exec();
    delete warningBox;
    return;
    
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
  exitStatus = 0;
  this->close();
}


