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

#include "../include/LinkageTypeDialog.h"

LinkageTypeDialog::LinkageTypeDialog(QWidget *parent) : QDialog(parent) 
{
  _name = "";
  _subName = "";
  _description = "";
  _direction = PAST;
  _exitStatus = 1;

  nameLabel = new QLabel(tr("<b>Name:</b>"), this);
  descriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  directionLabel = new QLabel(tr("<b>Linkage direction:</b>"), this);
  pastButton = new QPushButton(PAST, this);
  pastButton->setCheckable(true);
  pastButton->setChecked(true);
  futureButton = new QPushButton(FUTURE, this);
  futureButton->setCheckable(true);
  futureButton->setChecked(false);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  nameField = new QLineEdit(this);

  descriptionField = new QTextEdit(this);

  connect(nameField, SIGNAL(textChanged(const QString &)), this, SLOT(setName(const QString &)));
  connect(pastButton, SIGNAL(clicked()), this, SLOT(checkPastButton()));
  connect(futureButton, SIGNAL(clicked()), this, SLOT(checkFutureButton()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  mainLayout->addWidget(descriptionLabel);
  mainLayout->addWidget(descriptionField);
  QPointer<QHBoxLayout> directionLayout = new QHBoxLayout;
  directionLayout->addWidget(directionLabel);
  directionLayout->addWidget(pastButton);
  directionLayout->addWidget(futureButton);
  mainLayout->addLayout(directionLayout);

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
  setWindowTitle("Add / Edit Linkage Type");
  setFixedHeight(sizeHint().height());
  
}

void LinkageTypeDialog::setName(const QString &name) 
{
  _name = name;
}

void LinkageTypeDialog::checkPastButton() 
{
  futureButton->setChecked(false);
  _direction = PAST;
}

void LinkageTypeDialog::checkFutureButton() 
{
  pastButton->setChecked(false);
  _direction = FUTURE;
}

void LinkageTypeDialog::submitName(const QString &name) 
{
  _name = name;
  _subName = name;
  nameField->setText(_name);
}

void LinkageTypeDialog::submitDescription(const QString &description) 
{
  _description = description;
  descriptionField->setText(_description);
}

void LinkageTypeDialog::submitDirection(const QString &direction) 
{
  _direction = direction;
  if (_direction == PAST) 
    {
      pastButton->setChecked(true);
      futureButton->setChecked(false);
    }
  else if (_direction == FUTURE) 
    {
      pastButton->setChecked(false);
      futureButton->setChecked(true);
    }
}

QString LinkageTypeDialog::getName() 
{
  return _name;
}

QString LinkageTypeDialog::getDescription() 
{
  return _description;
}

QString LinkageTypeDialog::getDirection() 
{
  return _direction;
}

int LinkageTypeDialog::getExitStatus() 
{
  return _exitStatus;
}

void LinkageTypeDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void LinkageTypeDialog::saveAndClose() 
{
  _description = descriptionField->toPlainText().trimmed();
  _name = _name.trimmed();
  if (_description == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving linkage type");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Description required.");
      warningBox->setInformativeText("A linkage type requires a "
				     "description.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_name == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving linkage type");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name required.");
      warningBox->setInformativeText("A linkage type requires a name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM linkage_types WHERE name = :name");
  query->bindValue(":name", _name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && _name != _subName) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving linkage type");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Duplicate name.");
      warningBox->setInformativeText("You cannot create linkage "
				     "types with identical names.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  delete query;
  _exitStatus = 0;
  this->close();
}
