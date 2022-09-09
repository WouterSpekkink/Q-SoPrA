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

#ifndef RELATIONSHIPCHECKBOXDIALOG_H
#define RELATIONSHIPCHECKBOXDIALOG_H

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

class RelationshipCheckBoxDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  RelationshipCheckBoxDialog(QWidget *parent = 0);
  ~RelationshipCheckBoxDialog() {};

  // Getters
  QVector<QPair<QString, QString> > getRelationships();
  int getExitStatus();
			    
private slots:
  // Private member functions
  void setTree();
  void changeFilter(const QString &text);
  void cancelAndClose();
  void saveAndClose();
  void getChecks(QAbstractItemModel *model);
    
private:
  // Interface elements
  QPointer<QLabel> relationshipLabel;
  QPointer<QStandardItemModel> relationshipsTree;
  QPointer<DeselectableTreeView> relationshipsTreeView;
  QPointer<RelationshipTreeFilter> treeFilter;
  QPointer<QLabel> relationshipsFilterLabel;
  QPointer<QLineEdit> relationshipsFilterField;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QVector<QPair<QString, QString> > _chosenRelationships;
  int _exitStatus;
};

#endif
