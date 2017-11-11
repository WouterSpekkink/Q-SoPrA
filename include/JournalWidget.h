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
#include "EventTableModel.h"

class JournalWidget : public QWidget {
  friend class MainWindow;
  Q_OBJECT

public:
  JournalWidget(QWidget *parent = 0);
  ~JournalWidget() {};

private slots:
  void addEntry();
  void saveChanges();
  void setData();
  void removeEntry();
  void setButton();
  void resetHeader(int header);
  
private:

  QPointer<EventTableModel> journalModel;
  QPointer<QTableView> tableView;

  QPointer<QLabel> journalLabel;
  QPointer<QLabel> logLabel;
  
  QPointer<QTextEdit> logField;

  QPointer<QPushButton> addEntryButton;
  QPointer<QPushButton> saveChangesButton;
  QPointer<QPushButton> removeEntryButton;
};

#endif
