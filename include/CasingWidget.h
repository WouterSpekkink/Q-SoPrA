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

class CasingWidget : public QWidget {
  Q_OBJECT

  friend class MainWindow;

public:
  CasingWidget(QWidget *parent = 0);
  ~CasingWidget() {};

private slots:
  void createTable();
  void addCase(); // Should open a dialog.
  void updateTable(); // Call this on each switch
  
private:
  QPointer<QTableWidget> tableWidget;

  QPointer<QPushButton> addCaseButton;
  
};

#endif
