#include "../include/EventSequenceDatabase.h"

EventSequenceDatabase::EventSequenceDatabase() {
}

void EventSequenceDatabase::openDB(const QString &name) {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(name);
}


