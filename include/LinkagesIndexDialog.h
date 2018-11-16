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

#ifndef LINKAGESINDEXDIALOG_H
#define LINKAGESINDEXDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QIntValidator>
#include <QPointer>
#include <string>
#include <sstream>
#include "Constants.h"

class LinkagesIndexDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  LinkagesIndexDialog(QWidget *parent = 0, int size = 1, const QString &direction = PAST);
  ~LinkagesIndexDialog() {};

  // Getters
  int getTailIndex();
  int getHeadIndex();
  int getExitStatus();		

private slots:
  // Private member functions
  void go();
  void cancel();
  void setTailIndexText(const QString &newIndex);
  void setHeadIndexText(const QString &newIndex);
  void evaluateIndexes();

private:
  // Interface elements
  QPointer<QLabel> tailIndexLabel;
  QPointer<QLabel> headIndexLabel;
  QPointer<QLineEdit> tailIndexField;
  QPointer<QLineEdit> headIndexField;
  QPointer<QPushButton> goButton;
  QPointer<QPushButton> cancelButton;

  // Private variables
  int _max;
  int _submittedSize;
  int _tailIndex;
  int _headIndex;
  int _exitStatus;
  QString _tailIndexText;
  QString _headIndexText;
  QString _submittedDirection;
};

#endif
