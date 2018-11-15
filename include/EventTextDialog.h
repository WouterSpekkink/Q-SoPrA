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

#ifndef EVENTTEXTDIALOG_H
#define EVENTTEXTDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>

class EventTextDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  EventTextDialog(QWidget *parent = 0);
  ~EventTextDialog() {};

  // Setters
  void submitText(const QString &text);
  void submitTiming(const QString &timing);
  
  // Getters
  QString getText();
  QString getTiming();
  int getExitStatus();
		     
private slots:
  // Private member functions
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> timingLabel;
  QPointer<QLabel> textLabel;
  QPointer<QLineEdit> timingField;
  QPointer<QTextEdit> textField;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QString _text;
  QString _timing;
  QString _subText;
  QString _subTiming;
  int _exitStatus;
};

#endif
