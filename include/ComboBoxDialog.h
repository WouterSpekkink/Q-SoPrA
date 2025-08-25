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


#ifndef COMBOBOXDIALOG_H
#define COMBOBOXDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "SupportingFunctions.h"

class ComboBoxDialog : public QDialog
{
  Q_OBJECT
  
public:
  // Constructor and destructor
  ComboBoxDialog(QWidget *parent = 0, QVector<QString> contents = QVector<QString>());
  ~ComboBoxDialog() {};

  // Getters
  QString getSelection();
  int getExitStatus();

private slots:
  // Private member functions
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> selectionLabel;
  QPointer<QComboBox> selectionComboBox;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QString _selection;
  int _exitStatus;
};

#endif
