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

#include "../include/ComboBoxDialog.h"

ComboBoxDialog::ComboBoxDialog(QWidget *parent, QVector<QString> contents) : QDialog(parent) 
{
  _exitStatus = 1;
  _selection = DEFAULT;
  
  selectionLabel = new QLabel(tr("<b>Choose:</b>"), this);

  selectionComboBox = new QComboBox(this);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Select"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(selectionLabel);
  mainLayout->addWidget(selectionComboBox);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  QVectorIterator<QString> it(contents);
  while (it.hasNext()) 
    {
      QString current = it.next();
      selectionComboBox->addItem(current);
    }
}

void ComboBoxDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void ComboBoxDialog::saveAndClose() 
{
  _selection = selectionComboBox->currentText();
  if (_selection == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving selection");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No selection made.");
      warningBox->setInformativeText("You have to make a selection to proceed.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else 
    {
      _exitStatus = 0;
      this->close();
    }
}

QString ComboBoxDialog::getSelection() 
{
  return _selection;
}

int ComboBoxDialog::getExitStatus() 
{
  return _exitStatus;
}

				    
