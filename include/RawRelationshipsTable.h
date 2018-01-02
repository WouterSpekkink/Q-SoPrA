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

class RawRelationshipsTable : public QWidget {
  Q_OBJECT

  friend class MainWindow;

public:
  RawRelationshipsTable(QWidget *parent = 0);
  ~RawRelationshipsTable() {};

private slots:
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void removeText();
  void exportTable();
  
private:
  QPointer<RelationalTable> relationshipsModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;

  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;

  QPointer<QLineEdit> filterField;

  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> exportTableButton;
  
  QPointer<QComboBox> filterComboBox;
};

#endif
