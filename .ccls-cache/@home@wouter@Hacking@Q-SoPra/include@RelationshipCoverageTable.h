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

#ifndef RELATIONSHIPCOVERAGETABLE_H
#define RELATIONSHIPCOVERAGETABLE_H

#include <QtWidgets>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QStandardItemModel>
#include <QtSql>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include "QueryModel.h"
#include "ZoomableTableView.h"
#include "Constants.h"
#include "ConcordanceDialog.h"

class RelationshipCoverageTable : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;
  
public:
  // Constructor and destructor
  RelationshipCoverageTable(QWidget *parent=0);
  ~RelationshipCoverageTable() {};

  // Setters
  void setCurrentCoder(QString coder);
				 
private slots:
  void buildModel();
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void exportTable();
  void viewConcordancePlot();
  
private:
  // Interface elements
  QPointer<QStandardItemModel> relationshipsModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;
  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> viewConcordancePlotButton;
  QPointer<QLineEdit> filterField;
  QPointer<QComboBox> filterComboBox;
  
  // Private variables
  QString _selectedCoder;
  int _lastSortedHeader;
  bool _lastSortedAscending;
  
};

#endif
