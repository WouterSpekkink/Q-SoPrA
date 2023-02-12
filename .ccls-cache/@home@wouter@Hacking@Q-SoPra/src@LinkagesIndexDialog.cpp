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

#include "../include/LinkagesIndexDialog.h"

LinkagesIndexDialog::LinkagesIndexDialog(QWidget *parent, int size, const QString &direction) : QDialog(parent) 
{
  _max =  1;
  _submittedSize = size - 1;
  _submittedDirection = direction;
  tailIndexLabel = new QLabel(tr("<b>Tail index:</b>"), this);
  headIndexLabel = new QLabel(tr("<b>Head index:</b>"), this);
  _tailIndexText = "";
  _headIndexText = "";
  if (_submittedDirection == PAST) 
    {
      _tailIndex = 2;
      _headIndex = 1;
    }
  else 
    {
      _tailIndex = 1;
      _headIndex = 2;
    }
  _exitStatus = 1;
  tailIndexField = new QLineEdit(this);
  tailIndexField->setValidator(new QIntValidator(1, _submittedSize, this));
  headIndexField = new QLineEdit(this);
  headIndexField->setValidator(new QIntValidator(1, 1, this));

  goButton = new QPushButton(tr("Go"), this);
  goButton->setEnabled(false); // We only activate this button if there is something in the fields.
  cancelButton = new QPushButton(tr("Cancel"), this);

  // We connect the signals to the appropriate slots.
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
  connect(tailIndexField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setTailIndexText(const QString &)));
  connect(headIndexField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setHeadIndexText(const QString &)));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> fieldLayout = new QHBoxLayout;
  fieldLayout->addWidget(tailIndexLabel);
  fieldLayout->addWidget(tailIndexField);
  fieldLayout->addWidget(headIndexLabel);
  fieldLayout->addWidget(headIndexField);
  
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(goButton);
  buttonLayout->addWidget(cancelButton);
  
  mainLayout->addLayout(fieldLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("Jump to index"));
  setFixedHeight(sizeHint().height());
}

void LinkagesIndexDialog::setTailIndexText(const QString &newIndex) 
{
  _tailIndexText = newIndex.trimmed();
  if (_submittedDirection == PAST) 
    {
      std::istringstream ss(_tailIndexText.toStdString());
      ss >> _max;
      headIndexField->setValidator(new QIntValidator(1, _max, this));
    }
  else if (_submittedDirection == FUTURE) 
    {
      std::istringstream ss(_tailIndexText.toStdString());
      ss >> _max;
      headIndexField->setValidator(new QIntValidator(1, _submittedSize - _max + 1, this));
    }
  _tailIndex = _tailIndexText.toInt();
  evaluateIndexes();
}

void LinkagesIndexDialog::setHeadIndexText(const QString &newIndex) 
{
  _headIndexText = newIndex;
  _headIndex = _headIndexText.toInt();
  evaluateIndexes();
}
void LinkagesIndexDialog::evaluateIndexes() 
{
  if (_tailIndexText != "" && _tailIndex > 0 && _headIndexText != "" && _headIndex > 0) 
    {
      goButton->setEnabled(true);
    }
  else 
    {
      goButton->setEnabled(false);
    }
}

int LinkagesIndexDialog::getTailIndex() 
{
  return _tailIndex;
}

int LinkagesIndexDialog::getHeadIndex() 
{
  return _headIndex;
}

void LinkagesIndexDialog::go() 
{
  _exitStatus = 0;
  this->close();
}

void LinkagesIndexDialog::cancel() 
{
  _exitStatus = 1;
  this->close();
}

int LinkagesIndexDialog::getExitStatus() 
{
  return _exitStatus;
}
