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

#ifndef ATTRIBUTECOLORDIALOG_H
#define ATTRIBUTECOLORDIALOG_H

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
#include "AttributeTreeFilter.h"
#include "SupportingFunctions.h"

class AttributeColorDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and desconstructor
  AttributeColorDialog(QWidget *parent = 0, QString type = DEFAULT);
  ~AttributeColorDialog() {};

  // Getters
  QColor getColor();
  QColor getTextColor();
  QString getAttribute();
  bool isEntity();
  int getExitStatus();
			    
private slots:
  // Private members functions
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void buildEntities(QStandardItem *top, QString name);
  void changeFilter(const QString &text);
  void setColor();
  void setTextColor();
  void setAttribute();
  void cancelAndClose();
  void saveAndClose();
    
private:
  // Interface elements
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<QLabel> attributeLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> colorOneLabel;
  QPointer<QLabel> colorTwoLabel;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QPushButton> colorButton;
  QPointer<QPushButton> textColorButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private variables
  QColor _chosenColor;
  QColor _chosenTextColor;
  QString _chosenAttribute;
  QString _type;
  bool _entity;
  int _exitStatus;
};

#endif
