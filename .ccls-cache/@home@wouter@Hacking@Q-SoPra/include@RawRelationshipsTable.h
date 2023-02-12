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

#ifndef RAWRELATIONSHIPSTABLE_H
#define RAWRELATIONSHIPSTABLE_H

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
#include "RelationalTable.h"
#include "ZoomableTableView.h"
#include "RelationshipTypeDialog.h"
#include "RelationshipsWidget.h"
#include "ComboBoxDialog.h"

class RelationshipsWidget;

class RawRelationshipsTable : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  RawRelationshipsTable(QWidget *parent = 0);
  ~RawRelationshipsTable() {};

  // Setters
  void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);
						      
private slots:
  // Private member functions
  void updateTable();  
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void removeText();
  void editType();
  void switchType();
  void editRelationship();
  void exportTable();
  
private:
  // Interface elements 
  QPointer<RelationalTable> relationshipsModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;
  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;
  QPointer<QLineEdit> filterField;
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> editTypeButton;
  QPointer<QPushButton> switchTypeButton;
  QPointer<QPushButton> editRelationshipButton;
  QPointer<QComboBox> filterComboBox;

  // Private variables
  int _lastSortedHeader;
  bool _lastSortedAscending;
  
  // Pointer to other widget
  // Do not remove
  QPointer<RelationshipsWidget> _relationshipsWidgetPtr;
};

#endif
