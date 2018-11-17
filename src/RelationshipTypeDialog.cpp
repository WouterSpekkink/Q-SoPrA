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

#include "../include/RelationshipTypeDialog.h"

RelationshipTypeDialog::RelationshipTypeDialog(QWidget *parent) : QDialog(parent) 
{
  _directedness = DIRECTED;
  _name = "";
  _subName = "";
  _description = "";
  _exitStatus = 1;
    
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

QString RelationshipTypeDialog::getName() 
{
  return _name;
}

QString RelationshipTypeDialog::getDescription() 
{
  return _description;
}

QString RelationshipTypeDialog::getDirectedness() 
{
  return _directedness;
}

int RelationshipTypeDialog::getExitStatus() 
{
  return _exitStatus;
}

void RelationshipTypeDialog::submitName(const QString &name) 
{
  _name = name;
  _subName = name;
  nameField->setText(_name);
}

void RelationshipTypeDialog::submitDescription(const QString &description) 
{
  _description = description;
  descriptionField->setText(_description);
}

void RelationshipTypeDialog::submitDirectedness(const QString &directedness) 
{
  _directedness = directedness;
  if (_directedness == DIRECTED) 
    {
      directedButton->setChecked(true);
      undirectedButton->setChecked(false);
    }
  else if (_directedness == UNDIRECTED) 
    {
      directedButton->setChecked(false);
      undirectedButton->setChecked(true);
    }
}

void RelationshipTypeDialog::setName(const QString &name) 
{
  _name = name;
}

void RelationshipTypeDialog::checkDirectedButton() 
{
  directedButton->setChecked(true);
  undirectedButton->setChecked(false);
  _directedness = DIRECTED;
}

void RelationshipTypeDialog::checkUndirectedButton() 
{
  directedButton->setChecked(false);
  undirectedButton->setChecked(true);
  _directedness = UNDIRECTED;
}

void RelationshipTypeDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void RelationshipTypeDialog::saveAndClose() 
{
  _description = descriptionField->toPlainText().trimmed();
  _name = _name.trimmed();
  if (_name.contains("(") || _name.contains(")")) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name contains illegal character.");
      warningBox->setInformativeText("The name of a relationship type cannot "
				     "contain the '(' and ')' characters.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_description == "") 
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
  if (_name == "") 
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
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM relationship_types WHERE name = :name");
  query->bindValue(":name", _name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && _name != _subName) 
    {
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
  _exitStatus = 0;
  this->close();
}
