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

#include "../include/LargeTextDialog.h"

LargeTextDialog::LargeTextDialog(QWidget *parent) : QDialog(parent) 
{
  _text = "";
  _subText = "";
  _exitStatus = 1;
  _emptyAllowed = false;
  textLabel = new QLabel(tr("<b>Text:</b>"), this);
  textField = new QTextEdit(this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(textLabel);
  mainLayout->addWidget(textField);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionsLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
}


QString LargeTextDialog::getText() 
{
  return _text;
}

void LargeTextDialog::submitText(const QString &text) 
{
  _text = text;
  _subText = text;
  textField->setText(text);
}

void LargeTextDialog::setLabel(const QString &label) 
{
  textLabel->setText(label);
}

int LargeTextDialog::getExitStatus() 
{
  return _exitStatus;
}

void LargeTextDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void LargeTextDialog::setEmptyAllowed(bool status) 
{
  _emptyAllowed = status;
}

void LargeTextDialog::saveAndClose() 
{
  _text = textField->toPlainText().trimmed();
  if (_text == "" && !_emptyAllowed) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving text");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Text required.");
      warningBox->setInformativeText("You need to enter text to save.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  _exitStatus = 0;
  this->close();
}

