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

#ifndef ATTRIBUTEDIALOG_H
#define ATTRIBUTEDIALOG_H

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

class AttributeDialog : public QDialog
{
  Q_OBJECT
  
public:
  // Constructor and destructor
  AttributeDialog(QWidget *parent = 0, QString type = DEFAULT);
  ~AttributeDialog() {};

  // Setters
  void submitName(const QString &name);
  void submitDescription(const QString &description);

  // Getters
  QString getName();
  QString getDescription();		   
  int getExitStatus();
			  
private slots:
  // Private members functions
  void setName(const QString &name);
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QLineEdit> nameField;
  QPointer<QTextEdit> descriptionField;

  // Private variables
  QString _name;
  QString _oldName;
  QString _description;
  QString _type;
  int _exitStatus;
};

#endif
