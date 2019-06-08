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

#ifndef RAWLINKAGESTABLE_H
#define RAWLINKAGESTABLE_H

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

class RawLinkagesTable : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  RawLinkagesTable(QWidget *parent = 0);
  ~RawLinkagesTable() {};

private slots:
  // Private member functions
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void exportTable();
  
private:
  // Interface elements
  QPointer<RelationalTable> linkagesModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;
  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;
  QPointer<QLineEdit> filterField;
  QPointer<QPushButton> exportTableButton;
  QPointer<QComboBox> filterComboBox;

  // Private variables
  int _lastSortedHeader;
  bool _lastSortedAscending;
  
};

#endif
