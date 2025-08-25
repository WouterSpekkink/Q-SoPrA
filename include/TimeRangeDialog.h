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

#ifndef TIMERANGEDIALOG_H
#define TIMERANGEDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QCalendarWidget>
#include <QMessageBox>
#include <QPointer>

class TimeRangeDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  TimeRangeDialog(QWidget *parent = 0);
  ~TimeRangeDialog() {};

  // Getters
  QDate getStartDate();
  QDate getEndDate();
  int getExitStatus();

private slots:
  // Private member functions
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> startDateLabel;
  QPointer<QLabel> endDateLabel;
  QPointer<QLabel> startDateFeedbackLabel;
  QPointer<QLabel> endDateFeedbackLabel;
  QPointer<QCalendarWidget> startDateWidget;
  QPointer<QCalendarWidget> endDateWidget;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QDate _startDate;
  QDate _endDate;
  int _exitStatus;
  
};

#endif
