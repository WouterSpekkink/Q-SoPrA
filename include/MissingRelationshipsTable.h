#ifndef MISSINGRELATIONSHIPSTABLE_H
#define MISSINGRELATIONSHIPSTABLE_H

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

class MissingRelationshipsTable : public QWidget
{
  Q_OBJECT

  friend class MainWindow;

public:
  MissingRelationshipsTable(QWidget *parent = 0);
  ~MissingRelationshipsTable() {};

private slots:
  void updateTable();
  void resetHeader(int header);
  
private:
  QPointer<EventQueryModel> model;
  QPointer<ZoomableTableView> tableView;
  
};

#endif
