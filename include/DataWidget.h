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

#ifndef DATAWIDGET_H
#define DATAWIDGET_H
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtSql>
#include <QTableView>
#include <QHeaderView>
#include "EventSequenceDatabase.h"
#include "RecordDialog.h"
#include "EventTableModel.h"
#include "ZoomableTableView.h"
#include "Constants.h"
#include "EventGraphWidget.h"
#include "OccurrenceGraphWidget.h"
#include "CheckBoxDelegate.h"

class DataWidget : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to private members
  friend class MainWindow;
  
public:
  // Constructor and destructor
  DataWidget(QWidget *parent = 0);
  ~DataWidget() {};

private slots:
  // Private member functions
  void updateTable();
  void resetHeader(int header);
  void setData(const int index, QVector<QString> data, const QString type);
  void appendRecord();
  void editRecord();
  void insertRecordBefore();
  void insertRecordAfter();
  void moveUp();
  void moveDown();
  void flipEdges();
  void duplicateRow();
  void removeRow();
  void saveCurrent(const QModelIndex &index);
  void checkChange(const QModelIndex &topLeft, const QModelIndex &bottomRight);
  void setButtons();
  void findPrevious();
  void findNext();
  void setFindKey(const QString &text);
  bool eventFilter(QObject *object, QEvent *event);
  
private:
  // Interface elements
  QPointer<EventTableModel> incidentsModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QLabel> findSelectLabel;
  QPointer<QLabel> findFieldLabel;
  QPointer<QComboBox> findComboBox;
  QPointer<QLineEdit> findField;
  QPointer<QPushButton> appendRecordButton;
  QPointer<QPushButton> editRecordButton;
  QPointer<QPushButton> insertRecordBeforeButton;
  QPointer<QPushButton> insertRecordAfterButton;
  QPointer<QPushButton> moveUpButton;
  QPointer<QPushButton> moveDownButton;
  QPointer<QPushButton> duplicateRowButton;
  QPointer<QPushButton> removeRowButton;
  QPointer<QPushButton> findPreviousButton;
  QPointer<QPushButton> findNextButton;

  // Private variables.
  QString _currentData;
  QString _currentFind;
};


#endif
