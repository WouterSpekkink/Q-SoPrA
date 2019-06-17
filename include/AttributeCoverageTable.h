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

#ifndef ATTRIBUTECOVERAGETABLE_H
#define ATTRIBUTECOVERAGETABLE_H

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
#include <QGraphicsScene>
#include <QSvgGenerator>
#include <fstream>
#include "QueryModel.h"
#include "ZoomableTableView.h"
#include "RectObject.h"

class AttributeCoverageTable : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;
  
public:
  // Constructor and destructor
  AttributeCoverageTable(QWidget *parent=0);
  ~AttributeCoverageTable() {};

private slots:
  void buildModel();
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  void exportTable();
  void exportConcordancePlot();
  
private:
  // Interface elements
  QPointer<QStandardItemModel> attributesModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;
  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> exportConcordancePlotButton;
  QPointer<QLineEdit> filterField;
  QPointer<QComboBox> filterComboBox;
  
  // Private variables
  int _lastSortedHeader;
  bool _lastSortedAscending;
  
};

#endif
