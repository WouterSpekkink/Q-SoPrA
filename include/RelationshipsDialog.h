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

#ifndef RELATIONSHIPSDIALOG_H
#define RELATIONSHIPSDIALOG_H

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
#include "EventGraphWidget.h"
#include "AttributesWidget.h"

class EntitiesFilter;
class AttributesWidget;
class EventGraphWidget;

class RelationshipsDialog : public QDialog
{
  Q_OBJECT
  // Two other classes need access to this class
  friend class EntitiesFilter;
  friend class RelationshipsWidget;

public:
  // Constructor and destructor
  RelationshipsDialog(QWidget *parent = 0);
  ~RelationshipsDialog() {};

  // Setters
  void submitName(const QString &name);
  void submitLeftEntity(const QString &entity);
  void submitRightEntity(const QString &entity);
  void submitType(const QString &type);
  void submitDescription(const QString &description);
  void submitDirectedness(const QString &directedness);					     
  void setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr);
  void setAttributesWidget(AttributesWidget *attributesWidgetPtr);
  
  // Getters
  int getExitStatus();
  int getEntityEdited();
  QString getName();
  QString getLeftEntity();
  QString getRightEntity();
						     
private slots:
  // Private member functions
  void filterEntity(const QString &text);
  void assignLeftEntity();
  void assignRightEntity();
  void addEntity();
  void editEntity();
  void updateAfterEdit(const QString name, const QString description, const QString oldName);
  void editLeftAssignedEntity();
  void editRightAssignedEntity();
  void removeEntities();
  void reset();
  void updateTable();
  void setButtons();
  void cancelAndClose();
  void saveAndClose();
    
private:
  // Interface elements
  QPointer<EntityTableModel> entitiesTable;
  QPointer<ZoomableListView> entitiesView;
  QPointer<EntitiesFilter> entitiesFilter;
  QPointer<QLabel> filterLabel;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> tailLabel;
  QPointer<QLabel> headLabel;
  QPointer<QLabel> selectedSourceLabel;
  QPointer<QLabel> selectedTypeLabel;
  QPointer<QLabel> selectedTargetLabel;
  QPointer<QPushButton> resetButton;
  QPointer<QPushButton> newEntityButton;
  QPointer<QPushButton> editEntityButton;
  QPointer<QPushButton> editLeftAssignedEntityButton;
  QPointer<QPushButton> editRightAssignedEntityButton;
  QPointer<QPushButton> removeEntitiesButton;
  QPointer<QPushButton> assignLeftEntityButton;
  QPointer<QPushButton> assignRightEntityButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QLineEdit> entityFilterField;

  // Private variables
  QString _name;
  QString _oldName;
  int _exitStatus;
  int _entityEdited;
  bool _fresh;

  // Pointers to other widgets
  // Do not delete
  QPointer<EventGraphWidget> _eventGraphWidgetPtr;
  QPointer<AttributesWidget> _attributesWidgetPtr;
};

#endif
