#ifndef INCIDENTSATTRIBUTESTABLE_H
#define INCIDENTSATTRIBUTESTABLE_H

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
#include "SortFunctions.h"
#include "ProgressBar.h"

class IncidentsAttributesTable : public QWidget {
  Q_OBJECT

  friend class MainWindow;

public:
  IncidentsAttributesTable(QWidget *parent = 0);
  ~IncidentsAttributesTable() {};

private slots:
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void exportNormalMatrix();
  void exportValuedMatrix();
  void exportMatrix(bool valued);
  
private:
  QPointer<RelationalTable> attributesModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;

  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;

  QPointer<QLineEdit> filterField;

  QPointer<QPushButton> exportNormalMatrixButton;
  QPointer<QPushButton> exportValuedMatrixButton;
  
  QPointer<QComboBox> filterComboBox;
};

#endif
