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

#ifndef ATTRIBUTEINDEXDIALOG_H
#define ATTRIBUTEINDEXDIALOG_H

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

class AttributeIndexDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and desconstructor
  AttributeIndexDialog(QWidget *parent = 0, int = 1);
  ~AttributeIndexDialog() {};

  // Getters
  int getIndex();
  int getExitStatus();		

private slots:
  // Private member functions
  void setIndexText(const QString index);
  void go();
  void cancel();

private:
  // Interface elements
  QPointer<QLabel> indexLabel;
  QPointer<QLineEdit> indexField;
  QPointer<QPushButton> goButton;
  QPointer<QPushButton> cancelButton;

  // private variables
  int _index;
  int _exitStatus;
};

#endif
