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

#ifndef ENTITIESATTRIBUTESTABLE_H
#define ENTITIESATTRIBUTESTABLE_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtSql>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include "RelationalTable.h"
#include "ZoomableTableView.h"
#include "SortFunctions.h"
#include "ProgressBar.h"
#include "SimpleTextDialog.h"

class EntitiesAttributesTable : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to private members of this class
  friend class MainWindow;

public:
  // Constructor and destructor
  EntitiesAttributesTable(QWidget *parent = 0);
  ~EntitiesAttributesTable() {};

private slots:
  // Private member functions
  void updateTable();
  void resetHeader(const int &header);
  void sortHeader(const int &header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void editValue();
  void exportTable();
  void exportNormalMatrix();
  void exportValuedMatrix();
  void exportMatrix(bool valued);
  
private:
  // Interface elements
  QPointer<RelationalTable> attributesModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;
  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;
  QPointer<QLineEdit> filterField;
  QPointer<QPushButton> editValueButton;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> exportNormalMatrixButton;
  QPointer<QPushButton> exportValuedMatrixButton;
  QPointer<QComboBox> filterComboBox;
};

#endif
