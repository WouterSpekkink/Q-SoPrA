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

#ifndef JOURNALWIDGET_H
#define JOURNALWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QDateTime>
#include <QPointer>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QItemSelection>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include "JournalQueryModel.h"
#include "ZoomableTableView.h"
#include "SupportingFunctions.h"
#include "CheckBoxDelegate.h"

class JournalWidget : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  JournalWidget(QWidget *parent = 0);
  ~JournalWidget() {};

  // Setters
  void setCurrentCoder(QString coder);
		     
private slots:
  // Private member functions
  void updateTable();
  void resetButtons();
  void addEntry();
  void saveChanges();
  void setData(const QItemSelection &selected, const QItemSelection &deselected);
  void removeEntry();
  void exportJournal();
  void setButtons();
  bool checkChanges();
  void resetHeader(int header);
  bool eventFilter(QObject *object, QEvent *event);
  void armIndex();
  void setIndex();
  
private:
  // Interface elements
  QPointer<JournalQueryModel> journalModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QLabel> journalLabel;
  QPointer<QLabel> logLabel;
  QPointer<QTextEdit> logField;
  QPointer<QPushButton> addEntryButton;
  QPointer<QPushButton> saveChangesButton;
  QPointer<QPushButton> removeEntryButton;
  QPointer<QPushButton> exportJournalButton;

  // Private variables
  QString _selectedCoder;
  int _lastSortedHeader;
  bool _lastSortedAscending;
  QModelIndex _tempIndex;
};

#endif
