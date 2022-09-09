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

#include "../include/AttributeIndexDialog.h"

AttributeIndexDialog::AttributeIndexDialog(QWidget *parent, int max) : QDialog(parent) 
{
  // First we construct our dialog's entities.
  _exitStatus = 1;
  indexLabel = new QLabel(tr("Incident index:"), this);
  indexField = new QLineEdit(this);
  _index = 1;
  indexField->setValidator(new QIntValidator(1, max, this)); 
  goButton = new QPushButton(tr("Go"), this);
  goButton->setEnabled(false); // We only activate this button if there is something in the fields.
  cancelButton = new QPushButton(tr("Cancel"), this);

  // We connect the signals to the appropriate slots.
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
  connect(indexField, SIGNAL(textChanged(const QString &)), this, SLOT(setIndexText(const QString &)));
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> fieldLayout = new QHBoxLayout;
  fieldLayout->addWidget(indexLabel);
  fieldLayout->addWidget(indexField);

  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(goButton);
  buttonLayout->addWidget(cancelButton);
  
  mainLayout->addLayout(fieldLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("Jump to index"));
  setFixedHeight(sizeHint().height());
  // And this finished the constructor.
}

void AttributeIndexDialog::setIndexText(const QString index) 
{
  QString indexText = index.trimmed();
  // Let us immediately check if we should active the go Button.
  if (indexText != "") 
    {
      goButton->setEnabled(true); 
    }
  else 
    {
      goButton->setEnabled(false);
    }
  _index = indexText.toInt();
}

void AttributeIndexDialog::go() 
{
  _exitStatus = 0;
  this->close();
}

void AttributeIndexDialog::cancel() 
{
  _exitStatus = 1;
  this->close();
}

int AttributeIndexDialog::getIndex() 
{
  return _index;
}

int AttributeIndexDialog::getExitStatus() 
{
  return _exitStatus;
}

