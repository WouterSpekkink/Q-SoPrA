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

#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPointer>
#include <QFileInfo>
#include <QSql>
#include "EventSequenceDatabase.h"
#include "Constants.h"

class WelcomeDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  WelcomeDialog(QWidget *parent = 0,
		EventSequenceDatabase *esdPtr = new EventSequenceDatabase);
  ~WelcomeDialog() {};

  // Getters
  int getExitStatus();		     
		     
private slots:
  // Private member functions
  void newDatabase();
  void openDatabase();
  void quitApp();

private:
  // Interface elements
  QPointer<QLabel> imageLabel;
  QPointer<QLabel> titleLabel;
  QPointer<QPushButton> newDatabaseButton;
  QPointer<QPushButton> openDatabaseButton;
  QPointer<QPushButton> exitButton;

  // Private variable
  int _exitStatus;

  // Pointer to database
  // Do not delete
  QPointer<EventSequenceDatabase> _esdPtr;
};

#endif
