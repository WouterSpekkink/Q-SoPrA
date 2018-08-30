#ifndef RAWATTRIBUTESTABLE_H
#define RAWATTRIBUTESTABLE_H

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
#include "QueryModel.h"
#include "ZoomableTableView.h"
#include "AttributeDialog.h"
#include "EventGraphWidget.h"
#include "AttributesWidget.h"
#include "EntityDialog.h"

class AttributesWidget;

class RawAttributesTable : public QWidget
{
  Q_OBJECT

  friend class MainWindow;

public:
  RawAttributesTable(QWidget *parent = 0);
  ~RawAttributesTable() {};

  void setEventGraph(EventGraphWidget *egw);
  void setAttributesWidget(AttributesWidget *aw);
  void setRelationshipsWidget(RelationshipsWidget *rw);

private slots:
  void updateTable();
  void resetHeader(int header);
  void sortHeader(int header);
  void changeFilter(const QString &text);
  void setFilterColumn();
  void removeText();
  void editAttribute();
  void updateEntityAfterEdit(const QString name,
			     const QString description,
			     const QString former);
  void exportTable();
  
private:
  EventGraphWidget *eventGraph;
  AttributesWidget *attributesWidget;
  RelationshipsWidget *relationshipsWidget;
  
  QPointer<QueryModel> attributesModel;
  QPointer<ZoomableTableView> tableView;
  QPointer<QSortFilterProxyModel> filter;

  QPointer<QLabel> filterComboLabel;
  QPointer<QLabel> filterFieldLabel;

  QPointer<QLineEdit> filterField;

  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> editAttributeButton;
  
  QPointer<QComboBox> filterComboBox;
};

#endif
