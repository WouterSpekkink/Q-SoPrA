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

class CasingWidget : public QWidget {
  Q_OBJECT

  friend class MainWindow;

public:
  CasingWidget(QWidget *parent = 0);
  ~CasingWidget() {};

private slots:
  void createTable();
  void addCase();
  void removeCase();
  void selectAll();
  void deselectAll();
  void updateTable(); // Call this on each switch
  void setCellState(QTableWidgetItem *item);
  
private:
  QPointer<QTableWidget> tableWidget;

  QPointer<QPushButton> addCaseButton;
  QPointer<QPushButton> removeCaseButton;
  QPointer<QPushButton> selectAllButton;
  QPointer<QPushButton> deselectAllButton;
  
};

#endif
