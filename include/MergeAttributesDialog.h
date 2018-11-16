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

#ifndef MERGEATTRIBUTESDIALOG_H
#define MERGEATTRIBUTESDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "DeselectableTreeView.h"
#include "AttributeTreeFilter.h"
#include "SupportingFunctions.h"

class MergeAttributesDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  MergeAttributesDialog(QWidget *parent = 0, QString origin = DEFAULT, QString type = DEFAULT);
  ~MergeAttributesDialog() {};

  // Getters
  QString getAttribute();
  int getExitStatus();
			    
private slots:
  // Private member functions
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void changeFilter(const QString &text);
  void setAttribute();
  void cancelAndClose();
  void saveAndClose();
    
private:
  // Interface elements
  QPointer<QLabel> attributeLabel;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QString _chosenAttribute;
  QString _type;
  QString _origin;
  int _exitStatus;
};

#endif
