#include "../include/MultimodeDialog.h"

MultimodeDialog::MultimodeDialog(QWidget *parent,
				 QVector<DirectedEdge*> *directed,
				 QVector<UndirectedEdge*> *undirected) : QDialog(parent) 
{
  pDirected = directed;
  pUndirected = undirected;
  name = "";
  description = "";
  modeOne = DEFAULT;
  modeTwo = DEFAULT;
  relationshipOne = DEFAULT;
  relationshipTwo = DEFAULT;
  directedness = DIRECTED;
  exitStatus = 1;
  
  modeOneLabel = new QLabel(tr("<b>Mode one:</b>"), this);
  modeOneLabel->setToolTip(breakString("The new relationship "
				       "will be created between nodes of this mode."));
  modeTwoLabel = new QLabel(tr("<b>Mode two:</b>"), this);
  modeTwoLabel->setToolTip(breakString("Nodes of this mode will "
				       "be used to determine which nodes of mode "
				       "one should be related."));
  relationshipOneLabel = new QLabel(tr("<b>Relationship source:</b>"), this);
  relationshipOneLabel->setToolTip(breakString("What relationship does the source (mode one) "
					       "have to nodes of mode two?"));
  relationshipTwoLabel = new QLabel(tr("<b>Relationship target:</b>"), this);
  relationshipTwoLabel->setToolTip(breakString("What relationship does the target (mode one) "
					       "have to nodes of mode two?"));
  nameLabel = new QLabel(tr("<b>New relationship:</b>"), this);
  descriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  directednessLabel = new QLabel(tr("<b>Directedness:</b>"), this);
  
  nameField = new QLineEdit();

  descriptionField = new QTextEdit();

  modeOneComboBox = new QComboBox(this);
  modeOneComboBox->addItem(DEFAULT);
  modeOneComboBox->setToolTip(breakString("The new relationship "
					  "will be created between nodes of this mode."));
  modeTwoComboBox = new QComboBox(this);
  modeTwoComboBox->addItem(DEFAULT);
  modeTwoComboBox->setToolTip(breakString("Nodes of this mode will "
					  "be used to determine which nodes of mode "
					  "one should be related.<"));
  relationshipOneComboBox = new QComboBox(this);
  relationshipOneComboBox->addItem(DEFAULT);
  relationshipOneComboBox->setEnabled(false);
  relationshipOneComboBox->setToolTip(breakString("What relationship does the source (mode one) "
						  "have to nodes of mode two?"));
  relationshipTwoComboBox = new QComboBox(this);
  relationshipTwoComboBox->addItem(DEFAULT);
  relationshipTwoComboBox->setEnabled(false);
  relationshipTwoComboBox->setToolTip(breakString("What relationship does the target (mode one) "
						  "have to nodes of mode two?"));
  directedButton = new QPushButton(tr("Directed"), this);
  directedButton->setCheckable(true);
  directedButton->setChecked(true);
  directedButton->setEnabled(false);
  undirectedButton = new QPushButton(tr("Undirected"), this);
  undirectedButton->setCheckable(true);
  undirectedButton->setEnabled(false);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save relationship type"), this);

  connect(modeOneComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setModeOne(const QString &)));
  connect(modeTwoComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setModeTwo(const QString &)));
  connect(relationshipOneComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setRelationshipOne(const QString &)));
  connect(relationshipTwoComboBox, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setRelationshipTwo(const QString &)));
  connect(directedButton, SIGNAL(clicked()), this, SLOT(checkDirectedButton()));
  connect(undirectedButton, SIGNAL(clicked()), this, SLOT(checkUndirectedButton()));
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
  QPointer<QHBoxLayout> relationshipOneLayout = new QHBoxLayout;
  relationshipOneLayout->addWidget(relationshipOneLabel);
  relationshipOneLayout->addWidget(relationshipOneComboBox);
  mainLayout->addLayout(relationshipOneLayout);
  QPointer<QHBoxLayout> relationshipTwoLayout = new QHBoxLayout;
  relationshipTwoLayout->addWidget(relationshipTwoLabel);
  relationshipTwoLayout->addWidget(relationshipTwoComboBox);
  mainLayout->addLayout(relationshipTwoLayout);
  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

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
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionLayout);

  setLayout(mainLayout);
  setWindowTitle("Multimode transformation");
  setFixedHeight(sizeHint().height());
}

void MultimodeDialog::setModes(QVector<QString> submittedModes) 
{
  modes = submittedModes;
  QVectorIterator<QString> it(modes);
  while (it.hasNext()) 
    {
      QString currentMode = it.next();
      modeOneComboBox->addItem(currentMode);
      modeTwoComboBox->addItem(currentMode);
    }
}

void MultimodeDialog::setModeOne(const QString &name) 
{
  modeOne = name;
  if (modeOne != DEFAULT && modeTwo != DEFAULT && modeOne != modeTwo) 
    {
      relationshipOneComboBox->clear();
      relationshipOneComboBox->addItem(DEFAULT);
      relationshipTwoComboBox->clear();
      relationshipTwoComboBox->addItem(DEFAULT);
      QList<QString> include;
      QVectorIterator<DirectedEdge*> it(*pDirected);
      while (it.hasNext()) 
	{
	  DirectedEdge *directed = it.next();
	  if (directed->startItem()->getMode() == modeOne &&
	      directed->endItem()->getMode() == modeTwo &&
	      directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	  else if (directed->startItem()->getMode() == modeTwo &&
		   directed->endItem()->getMode() == modeOne &&
		   directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	}
    
      QVectorIterator<UndirectedEdge*> it2(*pUndirected);
      while (it2.hasNext()) 
	{
	  UndirectedEdge *undirected = it2.next();
	  if (undirected->startItem()->getMode() == modeOne &&
	      undirected->endItem()->getMode() == modeTwo &&
	      undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	  else if (undirected->startItem()->getMode() == modeTwo &&
		   undirected->endItem()->getMode() == modeOne &&
		   undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	}
      std::sort(include.begin(), include.end(), stringSort);
      QSet<QString> includeSet = QSet<QString>::fromList(include);
      QSet<QString>::iterator it3;
      for (it3 = includeSet.begin(); it3 != includeSet.end(); it3++) 
	{
	  relationshipOneComboBox->addItem(*it3);
	  relationshipTwoComboBox->addItem(*it3);
	}
      relationshipOneComboBox->setEnabled(true);
      relationshipTwoComboBox->setEnabled(true);
    }
  else 
    {
      relationshipOneComboBox->clear();
      relationshipOneComboBox->addItem(DEFAULT);
      relationshipOneComboBox->setEnabled(false);
      relationshipTwoComboBox->clear();
      relationshipTwoComboBox->addItem(DEFAULT);
      relationshipTwoComboBox->setEnabled(false);
    }
}

void MultimodeDialog::setModeTwo(const QString &name) 
{
  modeTwo = name;
  if (modeOne != DEFAULT && modeTwo != DEFAULT && modeOne != modeTwo) 
    {
      relationshipOneComboBox->clear();
      relationshipOneComboBox->addItem(DEFAULT);
      relationshipTwoComboBox->clear();
      relationshipTwoComboBox->addItem(DEFAULT);
      QList<QString> include;
      QVectorIterator<DirectedEdge*> it(*pDirected);
      while (it.hasNext()) 
	{
	  DirectedEdge *directed = it.next();
	  if (directed->startItem()->getMode() == modeOne &&
	      directed->endItem()->getMode() == modeTwo &&
	      directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	  else if (directed->startItem()->getMode() == modeTwo &&
		   directed->endItem()->getMode() == modeOne &&
		   directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	}
      QVectorIterator<UndirectedEdge*> it2(*pUndirected);
      while (it2.hasNext()) 
	{
	  UndirectedEdge *undirected = it2.next();
	  if (undirected->startItem()->getMode() == modeOne &&
	      undirected->endItem()->getMode() == modeTwo &&
	      undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	  else if (undirected->startItem()->getMode() == modeTwo &&
		   undirected->endItem()->getMode() == modeOne &&
		   undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	}
      std::sort(include.begin(), include.end(), stringSort);
      QSet<QString> includeSet = QSet<QString>::fromList(include);
      QSet<QString>::iterator it3;
      for (it3 = includeSet.begin(); it3 != includeSet.end(); it3++) 
	{
	  relationshipOneComboBox->addItem(*it3);
	  relationshipTwoComboBox->addItem(*it3);
	}
      relationshipOneComboBox->setEnabled(true);
      relationshipTwoComboBox->setEnabled(true);
    }
  else 
    {
      relationshipOneComboBox->clear();
      relationshipOneComboBox->addItem(DEFAULT);
      relationshipOneComboBox->setEnabled(false);
      relationshipTwoComboBox->clear();
      relationshipTwoComboBox->addItem(DEFAULT);
      relationshipTwoComboBox->setEnabled(false);
    }
}

QString MultimodeDialog::getDirectedness() 
{
  return directedness;
}

void MultimodeDialog::checkDirectedButton() 
{
  directedButton->setChecked(true);
  undirectedButton->setChecked(false);
  directedness = DIRECTED;
}

void MultimodeDialog::checkUndirectedButton() 
{
  directedButton->setChecked(false);
  undirectedButton->setChecked(true);
  directedness = UNDIRECTED;
}

void MultimodeDialog::setRelationshipOne(const QString &name) 
{
  relationshipOne = name;
  if (relationshipOne == relationshipTwo) 
    {
      directedness = UNDIRECTED;
      directedButton->setChecked(false);
      undirectedButton->setChecked(true);
      directedButton->setEnabled(false);
      undirectedButton->setEnabled(false);
    }
  else 
    {
      directedButton->setEnabled(true);
      undirectedButton->setEnabled(true);
    }
}

void MultimodeDialog::setRelationshipTwo(const QString &name) 
{
  relationshipTwo = name;
  if (relationshipOne == relationshipTwo) 
    {
      directedness = UNDIRECTED;
      directedButton->setChecked(false);
      undirectedButton->setChecked(true);
      directedButton->setEnabled(false);
      undirectedButton->setEnabled(false);
    }
  else 
    {
      directedButton->setEnabled(true);
      undirectedButton->setEnabled(true);
    }
}

QString MultimodeDialog::getModeOne() 
{
  return modeOne;
}

QString MultimodeDialog::getModeTwo() 
{
  return modeTwo;
}

QString MultimodeDialog::getRelationshipOne() 
{
  return relationshipOne;
}

QString MultimodeDialog::getRelationshipTwo() 
{
  return relationshipTwo;
}

QString MultimodeDialog::getName() 
{
  return name;
}

QString MultimodeDialog::getDescription() 
{
  return description;
}

int MultimodeDialog::getExitStatus() 
{
  return exitStatus;
}

void MultimodeDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void MultimodeDialog::saveAndClose() 
{
  description =  descriptionField->toPlainText().trimmed();
  name = nameField->text().trimmed();
  if (modeOne == DEFAULT || modeTwo == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No valid modes selected.");
      warningBox->setInformativeText("One or both of the selected modes are invalid.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (modeOne == modeTwo) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Modes should be different.");
      warningBox->setInformativeText("You cannot select the same mode twice.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (relationshipOne == DEFAULT || relationshipTwo == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No valid relationships selected.");
      warningBox->setInformativeText("You must select valid relationships for the transformation to be possible.");
      warningBox->exec();
      delete warningBox;
      return;
    
    }
  if (description == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Description required.");
      warningBox->setInformativeText("A relationship type requires a description.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (name == "") 
    {
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
  while (it.hasNext()) 
    {
      DirectedEdge* directed = it.next();
      if (directed->getType() == name) 
	{
	  found = true;
	}
    }
  QVectorIterator<UndirectedEdge*> it2(*pUndirected);
  while (it2.hasNext()) 
    {
      UndirectedEdge* undirected = it2.next();
      if (undirected->getType() == name) 
	{
	  found = true;
	}
    }
  if (found) 
    {
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


