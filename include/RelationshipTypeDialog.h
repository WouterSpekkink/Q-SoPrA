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

#ifndef RELATIONSHIPTYPEDIALOG_H
#define RELATIONSHIPTYPEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QPointer>
#include <QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QDateTime>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QtSql>
#include <QSqlQuery>
#include "Constants.h"

class RelationshipTypeDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  RelationshipTypeDialog(QWidget *parent = 0);
  ~RelationshipTypeDialog() {};

  // Setters
  void submitName(const QString &name);
  void submitDescription(const QString &description);
  void submitDirectedness(const QString &directedness);
  
  // Getters
  QString getName();
  QString getDescription();
  QString getDirectedness();
  int getExitStatus();
		     
private slots:
  // Private member functions
  void setName(const QString &name);
  void checkDirectedButton();
  void checkUndirectedButton();
    void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> directednessLabel;
  QPointer<QPushButton> directedButton;
  QPointer<QPushButton> undirectedButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QLineEdit> nameField;
  QPointer<QTextEdit> descriptionField;

  // Private variables
  QString _name;
  QString _subName;
  QString _directedness;
  QString _description;
  int _exitStatus;
};

#endif
