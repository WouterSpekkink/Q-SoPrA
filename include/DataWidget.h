#ifndef DATAWIDGET_H
#define DATAWIDGET_H
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
#include "OccurrenceGraphWidget.h"
#include "CheckBoxDelegate.h"

class EventGraphWidget;
class OccurrenceGraphWidget;

class DataWidget : public QWidget
{
  Q_OBJECT

  friend class MainWindow;
  
public:
  DataWidget(QWidget *parent = 0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);
  ~DataWidget() {};
  void setEventGraph(EventGraphWidget *egw);
  void setOccurrenceGraph(OccurrenceGraphWidget *ogw);
		  
private slots:
  void updateTable();
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
  void saveCurrent(const QModelIndex &index);
  void checkChange(const QModelIndex &topLeft, const QModelIndex &bottomRight);
  void setButtons();
  void findPrevious();
  void findNext();
  void setFindKey(const QString &text);
  
private:
  QPointer<EventSequenceDatabase> esd;
  QPointer<RecordDialog> recordDialog;
  QPointer<EventTableModel> incidentsModel;
  QPointer<ZoomableTableView> tableView;
  EventGraphWidget *eventGraph;
  OccurrenceGraphWidget *occurrenceGraph;

  QPointer<QLabel> findSelectLabel;
  QPointer<QLabel> findFieldLabel;
  
  QPointer<QComboBox> findComboBox;
  
  QPointer<QLineEdit> findField;
  
  QPointer<QPushButton> appendRecordButton;
  QPointer<QPushButton> editRecordButton;
  QPointer<QPushButton> insertRecordBeforeButton;
  QPointer<QPushButton> insertRecordAfterButton;
  QPointer<QPushButton> moveUpButton;
  QPointer<QPushButton> moveDownButton;
  QPointer<QPushButton> duplicateRowButton;
  QPointer<QPushButton> removeRowButton;
  QPointer<QPushButton> findPreviousButton;
  QPointer<QPushButton> findNextButton;

  QString currentData;
  QString currentFind;

  int currentRow;
};


#endif
