#ifndef EVENTSEQUENCEDATABASE_H
#define EVENTSEQUENCEDATABASE_H

#include <QtSql>
#include <QObject>
#include <QMessageBox>

class EventSequenceDatabase : public QObject {
  friend class WelcomeDialog;
  friend class MainDialog;
  Q_OBJECT

public:
  EventSequenceDatabase();
  ~EventSequenceDatabase() {};

  void createNew(const QString &name);
  
private:
  QSqlDatabase* db;
  
};






#endif
