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

#include "../include/CaseDialog.h"

CaseDialog::CaseDialog(QWidget *parent) : QDialog(parent) 
{
  _name = "";
  _description = "";
  _oldName = "";
  _exitStatus = 1;
  
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

  setLayout(mainLayout);
  setWindowTitle("Add/Edit case");
}

void CaseDialog::setName(const QString &name) 
{
  _name = name;
}

void CaseDialog::setDescription(const QString &description) 
{
  _description =  description;
  descriptionField->setText(description);
}

QString CaseDialog::getName() 
{
  return _name;
}

void CaseDialog::submitName(const QString &name) 
{
  _name = name;
  _oldName = name;
  nameField->setText(name);
}
QString CaseDialog::getDescription() 
{
  return _description;
}

int CaseDialog::getExitStatus() 
{
  return _exitStatus;
}

void CaseDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

// TO DO: Check for cases with same name.
void CaseDialog::saveAndClose() 
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
    warningBox->setInformativeText("An case requires a description.");
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
    warningBox->setInformativeText("An case requires a label.");
    warningBox->exec();
    delete warningBox;
    return;
  }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM cases "
		 "WHERE name = :name");
  query->bindValue(":name", _name);
  query->exec();
  query->first();
  if (!query->isNull(0) && _name != _oldName) 
    {
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
  _exitStatus = 0;
  delete query;
  this->close();
}

