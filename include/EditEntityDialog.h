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

#ifndef EDITENTITYDIALOG_H
#define EDITENTITYDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QPointer>
#include <QMessageBox>
#include <QSqlTableModel>
#include "EntitiesFilter.h"
#include "Constants.h"
#include "EntityTableModel.h"
#include "EntityDialog.h"
#include "ZoomableListView.h"
#include "SupportingFunctions.h"

class EntitiesFilter;

class EditEntityDialog : public QDialog
{
  Q_OBJECT
  friend class EntitiesFilter;

public:
  // Constructor and destructor
  EditEntityDialog(QWidget *parent = 0);
  ~EditEntityDialog() {};

  // Setters
  void setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr);
  void setAttributesWidget(AttributesWidget *attributesWidgetPtr);
  void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);
  
  // Getters
  bool entityEdited();
  bool entityAdded();
  QString getName();
  QString getLeftEntity();
  QString getRightEntity();
  			  
private slots:
  // Private member functions
  void filterEntity(const QString &text);
  void addEntity();
  void editEntity();
  void updateAfterEdit(const QString name, const QString description, const QString oldName);
  void removeEntities();
  void setButtons();
  void updateTable();
  void closeThis();
  
private:
  // Interface elements
  QPointer<QLabel> filterLabel;
  QPointer<EntityTableModel> entitiesTable;
  QPointer<ZoomableListView> entitiesView;
  QPointer<EntitiesFilter> entitiesFilter;
  QPointer<QPushButton> newEntityButton;
  QPointer<QPushButton> editEntityButton;
  QPointer<QPushButton> removeEntitiesButton;
  QPointer<QPushButton> closeButton;
  QPointer<QLineEdit> entityFilterField;

  // Private variables
  bool _entityEdited;
  bool _entityAdded;
  bool _fresh;

  // Pointers to other widgets
  // Do not delete
  QPointer<EventGraphWidget> _eventGraphWidgetPtr;
  QPointer<AttributesWidget> _attributesWidgetPtr;
  QPointer<RelationshipsWidget> _relationshipsWidgetPtr;
};

#endif
