#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtSql>
#include <QTableView>
#include <QHeaderView>
#include "EventSequenceDatabase.h"

class MainDialog : public QDialog {
  Q_OBJECT

public:
  MainDialog(EventSequenceDatabase *submittedEsd);
  ~MainDialog() {};

private slots:

  void appendRecord();
  
private:
  EventSequenceDatabase *esd;
  
  QPointer<QPushButton> dataViewButton;
  QPointer<QPushButton> linkageViewButton;
  QPointer<QPushButton> attributesViewButton;
  QPointer<QPushButton> relationshipsViewButton;
  QPointer<QPushButton> exitButton;

  QPointer<QPushButton> appendRecordButton;
  
  QSqlTableModel *incidentsModel;
  
  QTableView *tableView;
};


#endif
