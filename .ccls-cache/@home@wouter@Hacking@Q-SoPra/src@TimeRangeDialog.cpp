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

#include "../include/TimeRangeDialog.h"

TimeRangeDialog::TimeRangeDialog(QWidget *parent) : QDialog(parent)
{
  _exitStatus = 0;

  startDateLabel = new QLabel(tr("<b>Set start date:</b>"), this);
  endDateLabel = new QLabel(tr("<b>Set end date:</b>"), this);
  startDateFeedbackLabel = new QLabel("", this);
  endDateFeedbackLabel = new QLabel("", this);

  startDateWidget = new QCalendarWidget(this);
  endDateWidget = new QCalendarWidget(this);

  cancelCloseButton = new QPushButton(tr("Cancel"),this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> dateLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftDateLayout = new QVBoxLayout;
  leftDateLayout->addWidget(startDateLabel);
  leftDateLayout->addWidget(startDateWidget);
  leftDateLayout->setAlignment(Qt::AlignCenter);
  QPointer<QVBoxLayout> rightDateLayout = new QVBoxLayout;
  rightDateLayout->addWidget(endDateLabel);
  rightDateLayout->addWidget(endDateWidget);
  rightDateLayout->setAlignment(Qt::AlignCenter);
  dateLayout->addLayout(leftDateLayout);
  dateLayout->addLayout(rightDateLayout);
  mainLayout->addLayout(dateLayout);
  QPointer<QHBoxLayout> choiceLayout = new QHBoxLayout;
  choiceLayout->addWidget(startDateFeedbackLabel);
  choiceLayout->addWidget(endDateFeedbackLabel);
  mainLayout->addLayout(choiceLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionsLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionsLayout);
    
  setLayout(mainLayout);
  setWindowTitle("Set time range");
}

QDate TimeRangeDialog::getStartDate()
{
  return _startDate;
}

QDate TimeRangeDialog::getEndDate()
{
  return _endDate;
}

int TimeRangeDialog::getExitStatus()
{
  return _exitStatus;
}

void TimeRangeDialog::cancelAndClose()
{
  _exitStatus = 1;
  close();
}

void TimeRangeDialog::saveAndClose()
{
  _startDate = startDateWidget->selectedDate();
  _endDate = endDateWidget->selectedDate();
  if (_startDate.daysTo(_endDate) < 0)
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Setting time range");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Invalid dates</h2>");
      warningBox->setInformativeText("The current time range is invalid.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  _exitStatus = 0;
  close();
}
