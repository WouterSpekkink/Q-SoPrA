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

#ifndef RELATIONSHIPCOLORDIALOG_H
#define RELATIONSHIPCOLORDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"
#include "DeselectableTreeView.h"
#include "SupportingFunctions.h"
#include "RelationshipTreeFilter.h"

class RelationshipColorDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  RelationshipColorDialog(QWidget *parent = 0);
  ~RelationshipColorDialog() {};

  // Getters
  QColor getColor();
  QColor getTextColor();
  QString getRelationship();
  QString getType();
  int getExitStatus();
			    
private slots:
  // Private member functions
  void setTree();
  void changeFilter(const QString &text);
  void setColor();
  void setTextColor();
  void setRelationship();
  void cancelAndClose();
  void saveAndClose();
    
private:
  // Interface elements
  QPointer<QLabel> relationshipLabel;
  QPointer<QStandardItemModel> relationshipsTree;
  QPointer<DeselectableTreeView> relationshipsTreeView;
  QPointer<RelationshipTreeFilter> treeFilter;
  QPointer<QLabel> relationshipsFilterLabel;
  QPointer<QLabel> colorOneLabel;
  QPointer<QLabel> colorTwoLabel;
  QPointer<QLineEdit> relationshipsFilterField;
  QPointer<QPushButton> colorButton;
  QPointer<QPushButton> textColorButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QColor _chosenColor;
  QColor _chosenTextColor;
  QString _chosenRelationship;
  QString _relationshipType;
  int _exitStatus;
};

#endif
