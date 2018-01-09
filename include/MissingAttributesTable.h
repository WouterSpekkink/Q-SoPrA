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
#include "RelationalTable.h"
#include "ZoomableTableView.h"

class MissingAttributesTable : public QWidget {
  Q_OBJECT

  friend class MainWindow;

public:
  MissingAttributesTable(QWidget *parent = 0);
  ~MissingAttributesTable() {};

private slots:
  void updateTable();
  void resetHeader(int header);
  void markIncident(int state);
  void exportTable();
  void setRow(int row);
  
private:
  QPointer<QSqlQueryModel> model;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;
  QPointer<QSignalMapper> mapper;
  
  int currentRow = 0;
  
};

#endif
