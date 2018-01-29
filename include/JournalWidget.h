#ifndef JOURNALWIDGET_H
#define JOURNALWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QDateTime>
#include <QPointer>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QItemSelection>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include "JournalTableModel.h"
#include "ZoomableTableView.h"
#include "SupportingFunctions.h"
#include "CheckBoxDelegate.h"

class JournalWidget : public QWidget {
  friend class MainWindow;
  Q_OBJECT

public:
  JournalWidget(QWidget *parent = 0);
  ~JournalWidget() {};

  void resetButtons();
	     
private slots:
  void addEntry();
  void saveChanges();
  void setData();
  void removeEntry();
  void exportJournal();
  void setButtons();
  void resetHeader(int header);
  bool eventFilter(QObject *object, QEvent *event);
  
private:

  QPointer<JournalTableModel> journalModel;
  QPointer<ZoomableTableView> tableView;

  QPointer<QLabel> journalLabel;
  QPointer<QLabel> logLabel;
  
  QPointer<QTextEdit> logField;

  QPointer<QPushButton> addEntryButton;
  QPointer<QPushButton> saveChangesButton;
  QPointer<QPushButton> removeEntryButton;
  QPointer<QPushButton> exportJournalButton;
};

#endif
