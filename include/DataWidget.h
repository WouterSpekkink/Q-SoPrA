#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtSql>
#include <QTableView>
#include <QHeaderView>
#include "EventSequenceDatabase.h"
#include "RecordDialog.h"

class DataWidget : public QWidget {
  Q_OBJECT

public:
  DataWidget(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);
  ~DataWidget() {};

private slots:

  void appendRecord();
  
private:
  QPointer<EventSequenceDatabase> esd;
  QPointer<RecordDialog> recordDialog;
  
  QPointer<QPushButton> appendRecordButton;
  
  QSqlTableModel *incidentsModel;
  
  QTableView *tableView;
};


#endif
