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

#ifndef EVIDENCEDIALOG_H
#define EVIDENCEDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QScrollBar>
#include <QPointer>
#include <QtSql>
#include "SupportingFunctions.h"

class EvidenceDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  EvidenceDialog(int tail = 0, int head = 0, QString type = QString(), QString coder = QString(),
		 QWidget *parent = 0);
  ~EvidenceDialog() {};

  //Getters
  QString getComment();
		      
private slots:
  // Private member functions
  void getData();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> coderLabel;
  QPointer<QLabel> typeReportLabel;
  QPointer<QLabel> coderReportLabel;
  QPointer<QLabel> tailLabel;
  QPointer<QLabel> headLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QTextEdit> tailTextField;
  QPointer<QTextEdit> headTextField;
  QPointer<QTextEdit> commentField;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  int _tail;
  int _head;
  QString _type;
  QString _coder;
  QString _comment;
};

#endif
