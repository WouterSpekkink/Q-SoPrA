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

#ifndef SAVEDPLOTSDIALOG_H
#define SAVEDPLOTSDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include <QMessageBox>
#include "Constants.h"

class SavedPlotsDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  SavedPlotsDialog(QWidget *parent = 0, QString type = QString(), QString coder = QString());
  ~SavedPlotsDialog() {};

  // Getters
  int getExitStatus();
  QString getSelectedPlot();
		 
private slots:
  // Private member functions
  void cancelAndClose();
  void loadPlot();
  void removePlot();
  
private:
  // Interface elements
  QPointer<QLabel> titleLabel;
  QPointer<QComboBox> plotsComboBox;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> loadPlotButton;
  QPointer<QPushButton> removePlotButton;

  // Private variables
  QString _selectedPlot;
  QString _type;
  QString _selectedCoder;
  int _exitStatus;
};


#endif
