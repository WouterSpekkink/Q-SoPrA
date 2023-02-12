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

#ifndef RAWATTRIBUTESTABLE_H
#define RAWATTRIBUTESTABLE_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtSql>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include "QueryModel.h"
#include "ZoomableTableView.h"
#include "AttributeDialog.h"
#include "EventGraphWidget.h"
#include "AttributesWidget.h"
#include "EntityDialog.h"

class AttributesWidget;

class RawAttributesTable : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  RawAttributesTable(QWidget *parent = 0);
  ~RawAttributesTable() {};

  // Setters
  void setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr);
  void setAttributesWidget(AttributesWidget *attributesWidgetPtr);
  void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);

private slots:
  // Private member functions
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void removeText();
  void editAttribute();
  void updateEntityAfterEdit(const QString name,
			     const QString description,
			     const QString former);
  void exportTable();
  
private:
  // Interface elements  
  QPointer<QueryModel> attributesModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;
  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;
  QPointer<QLineEdit> filterField;
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QComboBox> filterComboBox;

  // Private variables
  int _lastSortedHeader;
  bool _lastSortedAscending;

  // Pointers to other widgets
  // Do not delete
  QPointer<EventGraphWidget> _eventGraphWidgetPtr;
  QPointer<AttributesWidget> _attributesWidgetPtr;
  QPointer<RelationshipsWidget> _relationshipsWidgetPtr;

};

#endif
