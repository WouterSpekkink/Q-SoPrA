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

#include "../include/EventTextDialog.h"

EventTextDialog::EventTextDialog(QWidget *parent) : QDialog(parent) 
{
  _text = "";
  _subText = "";
  _timing = "";
  _subTiming = "";
  _exitStatus = 1;

  timingLabel = new QLabel(tr("<b>Timing:</b>"), this);
  textLabel = new QLabel(tr("<b>Event description:</b>"), this);
			   
  textField = new QTextEdit(this);

  timingField = new QLineEdit(this);
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> timingLayout = new QHBoxLayout;
  timingLayout->addWidget(timingLabel);
  timingLayout->addWidget(timingField);
  mainLayout->addLayout(timingLayout);
  mainLayout->addWidget(textLabel);
  mainLayout->addWidget(textField);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionsLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Event description");
}

QString EventTextDialog::getText() 
{
  return _text;
}

QString EventTextDialog::getTiming() 
{
  return _timing;
}

void EventTextDialog::submitText(const QString &text) 
{
  _text = text;
  _subText = text;
  textField->setText(text);
}

void EventTextDialog::submitTiming(const QString &timing) 
{
  _timing = timing;
  _subTiming = timing;
  timingField->setText(timing);
}

int EventTextDialog::getExitStatus() 
{
  return _exitStatus;
}

void EventTextDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void EventTextDialog::saveAndClose() 
{
  _text = textField->toPlainText().trimmed();
  _timing = timingField->text();
  if (_text == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving event");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Event description required.");
      warningBox->setInformativeText("Events cannot be saved without descriptions.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (_timing == "")
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving event");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Timing required.");
      warningBox->setInformativeText("Events cannot be saved without "
				     "an indication of their timing.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  _exitStatus = 0;
  this->close();
}

