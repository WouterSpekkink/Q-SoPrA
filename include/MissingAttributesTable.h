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

#ifndef MISSINGATTRIBUTESTABLE_H
#define MISSINGATTRIBUTESTABLE_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtCore>
#include <QtSql>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <QSignalMapper>
#include "EventQueryModel.h"
#include "ZoomableTableView.h"
#include "CheckBoxDelegate.h"

class MissingAttributesTable : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  MissingAttributesTable(QWidget *parent = 0);
  ~MissingAttributesTable() {};

  // Setters
  void setCurrentCoder(QString coder);
			      
private slots:
  // Private member functions
  void updateTable();
  void resetHeader(int header);
  
private:
  // Interface elements
  QPointer<EventQueryModel> model;
  QPointer<ZoomableTableView> tableView;

  // Private variables
  QString _selectedCoder;
};

#endif
