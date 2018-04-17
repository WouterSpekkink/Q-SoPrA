#ifndef EVENTSEQUENCEDATABASE_H
#define EVENTSEQUENCEDATABASE_H

#include <QtSql>
#include <QObject>
#include <QMessageBox>
#include <QPointer>

class EventSequenceDatabase : public QObject {
  friend class WelcomeDialog;
  friend class MainDialog;
  friend class AddRecordDialog;
  Q_OBJECT

public:
  EventSequenceDatabase();
  ~EventSequenceDatabase() {};

  void openDB(const QString &name);
  
private:
  QSqlDatabase db;
  
};

#endif
