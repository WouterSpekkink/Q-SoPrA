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

#ifndef LINKAGETYPEDIALOG_H
#define LINKAGETYPEDIALOG_H

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextEdit>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "SupportingFunctions.h"

class LinkageTypeDialog : public QDialog
{
  Q_OBJECT
  
public:
  // Constructor and destructor
  LinkageTypeDialog(QWidget *parent = 0);
  ~LinkageTypeDialog() {};
  
  // Setters
  void submitName(const QString &name);
  void submitDescription (const QString &description);
  void submitQuestion(const QString &question);
  void submitDirection (const QString &direction);

  // Getters
  QString getName();
  QString getDescription();
  QString getQuestion();
  QString getDirection();
  int getExitStatus();

private slots:
  // Private member functions
  void setName(const QString &name);
  void checkPastButton();
  void checkFutureButton();
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> questionLabel;
  QPointer<QLabel> directionLabel;
  QPointer<QPushButton> pastButton;
  QPointer<QPushButton> futureButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QLineEdit> nameField;
  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> questionField;

  // Private variables
  QString _name;
  QString _subName;
  QString _description;
  QString _question;
  QString _direction;
  int _exitStatus;
};

#endif
