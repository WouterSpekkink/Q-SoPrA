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

#ifndef EXPORTTRANSITIONMATRIXDIALOG_H
#define EXPORTTRANSITIONMATRIXDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "SupportingFunctions.h"

class ExportTransitionMatrixDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and deconstructor
  ExportTransitionMatrixDialog(QWidget *parent = 0);
  ~ExportTransitionMatrixDialog() {};

  // Getters
  bool isProbability();
  int getExitStatus();
  bool ignoreDuplicates();
		     
private slots:
  // Private member functions
  void setRaw();
  void setProb();
  void setIgnoreDuplicates();
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> probLabel;
  QPointer<QPushButton> rawButton;
  QPointer<QPushButton> probButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QCheckBox> ignoreDuplicatesCheckBox;
  
  // Private variables
  bool _ignoreDuplicates;
  int _exitStatus;
};

#endif
