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
#include "EventTableModel.h"
#include "ZoomableTableView.h"
#include "Constants.h"
#include "EventGraphWidget.h"

class EventGraphWidget;

class DataWidget : public QWidget {
  Q_OBJECT

  friend class MainWindow;
public:
  DataWidget(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);
  ~DataWidget() {};
  void setEventGraph(EventGraphWidget *egw);
		  
private slots:

  void resetHeader(int header);
  void setData(const int index, RecordDialog *recordDialog, const QString type);
  void appendRecord();
  void editRecord();
  void insertRecordBefore();
  void insertRecordAfter();
  void moveUp();
  void moveDown();
  void duplicateRow();
  void removeRow();
  
private:
  QPointer<EventSequenceDatabase> esd;
  QPointer<RecordDialog> recordDialog;
  QPointer<EventTableModel> incidentsModel;
  QPointer<ZoomableTableView> tableView;
  EventGraphWidget *eventGraph;
  
  QPointer<QPushButton> appendRecordButton;
  QPointer<QPushButton> editRecordButton;
  QPointer<QPushButton> insertRecordBeforeButton;
  QPointer<QPushButton> insertRecordAfterButton;
  QPointer<QPushButton> moveUpButton;
  QPointer<QPushButton> moveDownButton;
  QPointer<QPushButton> duplicateRowButton;
  QPointer<QPushButton> removeRowButton;
};


#endif
