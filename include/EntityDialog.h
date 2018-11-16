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

#ifndef ENTITYDIALOG_H
#define ENTITYDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QHeaderView>
#include <QPointer>
#include <QVector>
#include <QSqlTableModel>
#include "DeselectableTreeViewEntities.h"
#include "AttributeTreeFilter.h"
#include "AttributeDialog.h"
#include "Constants.h"
#include "MergeAttributesDialog.h"
#include "RelationshipsWidget.h"
#include "SupportingFunctions.h"

class RelationshipsWidget;

class EntityDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  EntityDialog(QWidget *parent = 0);
  ~EntityDialog() {};

  // Setters
  void submitName(const QString &name);
  void submitDescription(const QString &description);
  void setNew();
  void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);
  
  // Getters
  QString getName();
  QString getDescription();
  int getExitStatus();

  // Public functions
  void clearSelection();
  void resetTree();

private slots:
  // Private member functions
  void setValueButton();
  void setValue();
  void getValue();
  void setFilter(const QString &text);
  void expandTree();
  void collapseTree();
  void assignAttribute();
  void unassignAttribute();
  void addAttribute();
  void editAttribute();
  void mergeAttributes();
  void removeUnusedAttributes();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void boldSelected(QAbstractItemModel *model, QString name = "", QModelIndex parent = QModelIndex());
  void cancelAndClose();
  void saveAndClose();
  void fixTree();
  void setButtons();
  bool eventFilter(QObject *object, QEvent *event);

private:
  // Interface elements
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeViewEntities> attributesTreeView;
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<AttributeDialog> attributeDialog;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QPushButton> valueButton; 
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> mergeAttributesButton;
  QPointer<QPushButton> removeUnusedAttributesButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QPushButton> expandButton;
  QPointer<QPushButton> collapseButton;
  QPointer<QLineEdit> nameField;
  QPointer<QLineEdit> valueField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QTextEdit> descriptionField;

  // Private variables
  QString _name;
  QString _description;
  QString _oldName;
  int _exitStatus;
  bool _isNew;
  bool _fresh; // Qt's selection behaviour forced me to do this.
  
  // Private data vector
  QVector<QString> _attributes;

  // Pointer to other widget
  // Do not delete
  QPointer<RelationshipsWidget> _relationshipsWidgetPtr;
};

#endif
