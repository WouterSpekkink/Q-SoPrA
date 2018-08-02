#ifndef CASINGWIDGET_H
#define CASINGWIDGET_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtSql>
#include <QHeaderView>
#include <QTableWidget>
#include "Constants.h"
#include "SupportingFunctions.h"
#include "CaseDialog.h"
#include "CheckBoxDelegate.h"
#include "ComboBoxDialog.h"
#include "Constants.h"
#include "SimpleAttributeSelectionDialog.h"
#include "EventGraphWidget.h"
#include "NetworkGraphWidget.h"
#include "OccurrenceGraphWidget.h"

class CasingWidget : public QWidget {
  Q_OBJECT

  friend class MainWindow;

public:
  CasingWidget(QWidget *parent = 0);
  ~CasingWidget() {};

  void setEventGraphWidget(EventGraphWidget *egw);
  void setNetworkGraphWidget(NetworkGraphWidget *ngw);
  void setOccurrenceGraphWidget(OccurrenceGraphWidget *ogw);

private slots:
  void createTable();
  void addCase();
  void editCase();
  void removeCase();
  void selectAll();
  
  void deselectAll();
  void attributeSelect();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  void updateTable(); // Call this on each switch
  void setCellState(QTableWidgetItem *item);
  
private:
  EventGraphWidget *eventGraph;
  NetworkGraphWidget *networkGraph;
  OccurrenceGraphWidget *occurrenceGraph;
  
  QPointer<QTableWidget> tableWidget;

  QPointer<QPushButton> addCaseButton;
  QPointer<QPushButton> editCaseButton;
  QPointer<QPushButton> removeCaseButton;
  QPointer<QPushButton> selectAllButton;
  QPointer<QPushButton> deselectAllButton;
  QPointer<QPushButton> attributeSelectButton;
};

#endif
