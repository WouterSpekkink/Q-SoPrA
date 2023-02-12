/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

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

#include "../include/MergeRelationshipsDialog.h"

MergeRelationshipsDialog::MergeRelationshipsDialog(QWidget *parent,
						   QVector<DirectedEdge*> *directed,
						   QVector<UndirectedEdge*> *undirected,
						   QVector<QString> relationshipsVector,
						   QVector<QString> directednessVector)
  : QDialog(parent) 
{
  
  _directed = directed;
  _undirected = undirected;
  _directednessVector = directednessVector;
  _name = "";
  _description = "";
  _directedness = "";
  _exitStatus = 1;  

  relationshipsLabel = new QLabel(tr("<b>Relationships:</b>"), this);
  nameLabel = new QLabel(tr("<b>Merged relationship name:</b>"), this);
  descriptionLabel = new QLabel(tr("<b>Merged relationship description:</b>"), this);
  
  nameField = new QLineEdit();

  descriptionField = new QTextEdit();

  QVectorIterator<QString> it(relationshipsVector);
  while (it.hasNext()) 
    {
      QString currentRel = it.next();
      QPointer<QCheckBox> checkBox = new QCheckBox(currentRel, this);
      relationshipsCheckBoxVector.push_back(checkBox);
    }
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save new relationship"), this);
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(relationshipsLabel);
  QVectorIterator<QPointer<QCheckBox>> it2(relationshipsCheckBoxVector);
  while (it2.hasNext()) 
    {
      mainLayout->addWidget(it2.next()); 
    }
  
  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  mainLayout->addWidget(descriptionLabel);
  mainLayout->addWidget(descriptionField);

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

QVector<QString> MergeRelationshipsDialog::getTypes() 
{
  return _types;
}

QString MergeRelationshipsDialog::getName() 
{
  return _name;
}

QString MergeRelationshipsDialog::getDescription() 
{
  return _description;
}

QString MergeRelationshipsDialog::getDirectedness() 
{
  return _directedness;
}

int MergeRelationshipsDialog::getExitStatus() 
{
  return _exitStatus;
}

void MergeRelationshipsDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void MergeRelationshipsDialog::saveAndClose() 
{
  _description =  descriptionField->toPlainText().trimmed();
  _name = nameField->text().trimmed();
  QVectorIterator<QPointer<QCheckBox>> it(relationshipsCheckBoxVector);
  int checked = 0;
  while (it.hasNext()) 
    {
      QPointer<QCheckBox> current = it.next();
      if (current->isChecked()) 
	{
	  checked++;
	}
    }
  if (checked < 2) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Merging relationships");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Less than 2 relationships checked.");
      warningBox->setInformativeText("You have to check at least 2 relationships.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  bool directedFound = false;
  bool undirectedFound = false;
  for (int i = 0; i != relationshipsCheckBoxVector.size(); i++) 
    {
      QPointer<QCheckBox> current = relationshipsCheckBoxVector[i];
      if (current->isChecked()) 
	{
	  if (_directednessVector[i] == DIRECTED) 
	    {
	      directedFound = true;
	    }
	  else if (_directednessVector[i] == UNDIRECTED) 
	    {
	      undirectedFound = true;
	    }
	}
    }
  if (directedFound && undirectedFound) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Merging relationships");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Directedness is different.");
      warningBox->setInformativeText("You cannot merge relationships with a different directedness.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else if (directedFound) 
    {
      _directedness = DIRECTED;
    }
  else if (undirectedFound) 
    {
      _directedness = UNDIRECTED;
    }
  if (_description == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Merging relationships");
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
      warningBox->setWindowTitle("Merging relationships");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name required.");
      warningBox->setInformativeText("A relationship type requires a name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  bool found = false;
  QVectorIterator<DirectedEdge*> it2(*_directed);
  while (it2.hasNext()) 
    {
      DirectedEdge* directedEdge = it2.next();
      if (directedEdge->getType() == _name) 
	{
	  found = true;
	}
    }
  QVectorIterator<UndirectedEdge*> it3(*_undirected);
  while (it3.hasNext()) 
    {
      UndirectedEdge* undirectedEdge = it3.next();
      if (undirectedEdge->getType() == _name) 
	{
	  found = true;
	}
    }
  if (found) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Merging relationships");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name already exists.");
      warningBox->setInformativeText("You already have a relationship type with that name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  QVectorIterator<QPointer<QCheckBox>> it4(relationshipsCheckBoxVector);
  while (it4.hasNext()) 
    {
      QPointer<QCheckBox> current = it4.next();
      if (current->isChecked()) 
	{
	  QString currentText = removeChar(current->text(), '&');
	  _types.push_back(currentText);
	}
    }
  _exitStatus = 0;
  this->close();
}



