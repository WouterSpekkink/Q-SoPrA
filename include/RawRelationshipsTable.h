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
#include "RelationshipTypeDialog.h"
#include "RelationshipsWidget.h"
#include "ComboBoxDialog.h"

class RelationshipsWidget;

class RawRelationshipsTable : public QWidget {
  Q_OBJECT

  friend class MainWindow;

public:
  RawRelationshipsTable(QWidget *parent = 0);
  ~RawRelationshipsTable() {};

  void setRelationshipsWidget(RelationshipsWidget *rw);
  void setNetworkGraph(NetworkGraphWidget *ngw);
						      
private slots:
  void updateTable();  
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void removeText();
  void editType();
  void switchType();
  void editRelationship();
  void exportTable();
  
private:
  RelationshipsWidget *relationshipsWidget;
  NetworkGraphWidget *networkGraph;
  
  QPointer<RelationalTable> relationshipsModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;

  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;

  QPointer<QLineEdit> filterField;

  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> editTypeButton;
  QPointer<QPushButton> switchTypeButton;
  QPointer<QPushButton> editRelationshipButton;
  
  QPointer<QComboBox> filterComboBox;
};

#endif
