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

#include "../include/AttributeDialog.h"

AttributeDialog::AttributeDialog(QWidget *parent, QString type) : QDialog(parent) 
{
  _name = "";
  _description = "";
  _oldName = "";
  _exitStatus = 1;
  _type = type;
  
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

  setWindowTitle("Add/Edit attribute");
  setLayout(mainLayout);
}

void AttributeDialog::setName(const QString &name) 
{
  _name = name;
}

QString AttributeDialog::getName() 
{
  return _name;
}

void AttributeDialog::submitName(const QString &name) 
{
  _name = name;
  _oldName = name;
  nameField->setText(name);
}

void AttributeDialog::submitDescription(const QString &description) 
{
  _description =  description;
  descriptionField->setText(description);
}


QString AttributeDialog::getDescription() 
{
  return _description;
}

int AttributeDialog::getExitStatus() 
{
  return _exitStatus;
}

void AttributeDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void AttributeDialog::saveAndClose() 
{
  _description =  descriptionField->toPlainText();
  _name = _name.trimmed();
  _description = _description.trimmed();
  if (_description == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Description missing.");
      warningBox->setInformativeText("An attribute requires a description.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_name == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name missing.");
      warningBox->setInformativeText("An attribute requires a label.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  //  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  if (_type == INCIDENT) 
    {
      query->prepare("SELECT name FROM incident_attributes WHERE name = :name");
      query->bindValue(":name", _name);
      query->exec();
      query->first();
    }
  else if (_type == ENTITY) 
    {
      query->prepare("SELECT name FROM entity_attributes WHERE name = :name");
      query->bindValue(":name", _name);
      query->exec();
      query->first();
    }
  if (!query->isNull(0) && _name != _oldName) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Duplicate name.");
      warningBox->setInformativeText("You cannot create attributes with identical names.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_type == INCIDENT) 
    {
      query->prepare("SELECT name FROM entities WHERE name = :name");
      query->bindValue(":name", _name);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  QPointer <QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->addButton(QMessageBox::Ok);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("Entity name.");
	  warningBox->setInformativeText("Attributes and entities cannot share names.");
	  warningBox->exec();
	  delete warningBox;
	  return;
	}
    }
  if (_name == ENTITIES) 
    { 
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Cannot use this name.");
      warningBox->setInformativeText("This name has been reserved entities.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  _exitStatus = 0;
  delete query;
  this->close();
}

