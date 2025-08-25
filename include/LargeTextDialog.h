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

#ifndef LARGETEXTDIALOG_H
#define LARGETEXTDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>

class LargeTextDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  LargeTextDialog(QWidget *parent = 0);
  ~LargeTextDialog() {};

  // Setters
  void submitText(const QString &text);
  void setLabel(const QString &label);
  void setEmptyAllowed(bool status);

  // Getters
  QString getText();
  int getExitStatus();
	     
private slots:
  // Private member functions
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> textLabel;
  QPointer<QTextEdit> textField;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QString _text;
  QString _subText;
  int _exitStatus;
  bool _emptyAllowed;
};

#endif
