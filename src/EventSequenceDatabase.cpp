#include "../include/EventSequenceDatabase.h"

EventSequenceDatabase::EventSequenceDatabase()
{
}

void EventSequenceDatabase::createNew(const QString &name) {
 db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
 db->setDatabaseName(name);
}

