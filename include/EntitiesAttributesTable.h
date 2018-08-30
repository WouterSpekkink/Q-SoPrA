#ifndef ENTITIESATTRIBUTESTABLE_H
#define ENTITIESATTRIBUTESTABLE_H

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
#include "SimpleTextDialog.h"

class EntitiesAttributesTable : public QWidget
{
  Q_OBJECT

  friend class MainWindow;

public:
  EntitiesAttributesTable(QWidget *parent = 0);
  ~EntitiesAttributesTable() {};

private slots:
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void editValue();
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

  QPointer<QPushButton> editValueButton;
  QPointer<QPushButton> exportNormalMatrixButton;
  QPointer<QPushButton> exportValuedMatrixButton;
  
  QPointer<QComboBox> filterComboBox;
};

#endif
