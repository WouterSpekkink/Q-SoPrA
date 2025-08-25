/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2025 Wouter Spekkink  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../include/MultimodeDialog.h"

MultimodeDialog::MultimodeDialog(QWidget *parent,
				 QVector<DirectedEdge*> *directed,
				 QVector<UndirectedEdge*> *undirected) : QDialog(parent) 
{
  _directed = directed;
  _undirected = undirected;
  _name = "";
  _description = "";
  _modeOne = DEFAULT;
  _modeTwo = DEFAULT;
  _relationshipOne = DEFAULT;
  _relationshipTwo = DEFAULT;
  _directedness = DIRECTED;
  _exitStatus = 1;
  
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

void MultimodeDialog::setModes(QVector<QString> modes) 
{
  _modes = modes;
  QVectorIterator<QString> it(_modes);
  while (it.hasNext()) 
    {
      QString currentMode = it.next();
      modeOneComboBox->addItem(currentMode);
      modeTwoComboBox->addItem(currentMode);
    }
}

void MultimodeDialog::setModeOne(const QString &mode)
{
  _modeOne = mode;
  if (_modeOne != DEFAULT && _modeTwo != DEFAULT && _modeOne != _modeTwo) 
    {
      relationshipOneComboBox->clear();
      relationshipOneComboBox->addItem(DEFAULT);
      relationshipTwoComboBox->clear();
      relationshipTwoComboBox->addItem(DEFAULT);
      QList<QString> include;
      QVectorIterator<DirectedEdge*> it(*_directed);
      while (it.hasNext()) 
	{
	  DirectedEdge *directed = it.next();
	  if (directed->getStart()->getMode() == _modeOne &&
	      directed->getEnd()->getMode() == _modeTwo &&
	      directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	  else if (directed->getStart()->getMode() == _modeTwo &&
		   directed->getEnd()->getMode() == _modeOne &&
		   directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	}
    
      QVectorIterator<UndirectedEdge*> it2(*_undirected);
      while (it2.hasNext()) 
	{
	  UndirectedEdge *undirected = it2.next();
	  if (undirected->getStart()->getMode() == _modeOne &&
	      undirected->getEnd()->getMode() == _modeTwo &&
	      undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	  else if (undirected->getStart()->getMode() == _modeTwo &&
		   undirected->getEnd()->getMode() == _modeOne &&
		   undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	}
      std::sort(include.begin(), include.end(), stringSort);
      QSet<QString> includeSet(include.begin(), include.end());
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

void MultimodeDialog::setModeTwo(const QString &mode)
{
  _modeTwo = mode;
  if (_modeOne != DEFAULT && _modeTwo != DEFAULT && _modeOne != _modeTwo) 
    {
      relationshipOneComboBox->clear();
      relationshipOneComboBox->addItem(DEFAULT);
      relationshipTwoComboBox->clear();
      relationshipTwoComboBox->addItem(DEFAULT);
      QList<QString> include;
      QVectorIterator<DirectedEdge*> it(*_directed);
      while (it.hasNext()) 
	{
	  DirectedEdge *directed = it.next();
	  if (directed->getStart()->getMode() == _modeOne &&
	      directed->getEnd()->getMode() == _modeTwo &&
	      directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	  else if (directed->getStart()->getMode() == _modeTwo &&
		   directed->getEnd()->getMode() == _modeOne &&
		   directed->isVisible()) 
	    {
	      include.push_back(directed->getType());;
	    }
	}
      QVectorIterator<UndirectedEdge*> it2(*_undirected);
      while (it2.hasNext()) 
	{
	  UndirectedEdge *undirected = it2.next();
	  if (undirected->getStart()->getMode() == _modeOne &&
	      undirected->getEnd()->getMode() == _modeTwo &&
	      undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	  else if (undirected->getStart()->getMode() == _modeTwo &&
		   undirected->getEnd()->getMode() == _modeOne &&
		   undirected->isVisible()) 
	    {
	      include.push_back(undirected->getType());;
	    }
	}
      std::sort(include.begin(), include.end(), stringSort);
      QSet<QString> includeSet(include.begin(), include.end());
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
  return _directedness;
}

void MultimodeDialog::checkDirectedButton() 
{
  directedButton->setChecked(true);
  undirectedButton->setChecked(false);
  _directedness = DIRECTED;
}

void MultimodeDialog::checkUndirectedButton() 
{
  directedButton->setChecked(false);
  undirectedButton->setChecked(true);
  _directedness = UNDIRECTED;
}

void MultimodeDialog::setRelationshipOne(const QString &relationship)
{
  _relationshipOne = relationship;
  if (_relationshipOne == _relationshipTwo) 
    {
      _directedness = UNDIRECTED;
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

void MultimodeDialog::setRelationshipTwo(const QString &relationship) 
{
  _relationshipTwo = relationship;
  if (_relationshipOne == _relationshipTwo) 
    {
      _directedness = UNDIRECTED;
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
  return _modeOne;
}

QString MultimodeDialog::getModeTwo() 
{
  return _modeTwo;
}

QString MultimodeDialog::getRelationshipOne() 
{
  return _relationshipOne;
}

QString MultimodeDialog::getRelationshipTwo() 
{
  return _relationshipTwo;
}

QString MultimodeDialog::getName() 
{
  return _name;
}

QString MultimodeDialog::getDescription() 
{
  return _description;
}

int MultimodeDialog::getExitStatus() 
{
  return _exitStatus;
}

void MultimodeDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void MultimodeDialog::saveAndClose() 
{
  _description =  descriptionField->toPlainText().trimmed();
  _name = nameField->text().trimmed();
  if (_modeOne == DEFAULT || _modeTwo == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Making transformation");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No valid modes selected.");
      warningBox->setInformativeText("One or both of the selected modes are invalid.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_modeOne == _modeTwo) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Making transformation");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Modes should be different.");
      warningBox->setInformativeText("You cannot select the same mode twice.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_relationshipOne == DEFAULT || _relationshipTwo == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Making transformation");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No valid relationships selected.");
      warningBox->setInformativeText("You must select valid relationships for the "
				     "transformation to be possible.");
      warningBox->exec();
      delete warningBox;
      return;
    
    }
  if (_description == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Making transformation");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Description required.");
      warningBox->setInformativeText("A relationship type requires a description.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_name == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Making transformation");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name required.");
      warningBox->setInformativeText("A relationship type requires a name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  bool found = false;
  QVectorIterator<DirectedEdge*> it(*_directed);
  while (it.hasNext()) 
    {
      DirectedEdge* directed = it.next();
      if (directed->getType() == _name) 
	{
	  found = true;
	}
    }
  QVectorIterator<UndirectedEdge*> it2(*_undirected);
  while (it2.hasNext()) 
    {
      UndirectedEdge* undirected = it2.next();
      if (undirected->getType() == _name) 
	{
	  found = true;
	}
    }
  if (found) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Making transformation");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name already exists.");
      warningBox->setInformativeText("You already have a relationship type with that name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  _exitStatus = 0;
  this->close();
}


