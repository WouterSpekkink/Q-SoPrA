#include <QtWidgets/QApplication>
#include <QtSql>
#include <QTableView>
#include <iostream>

int main(int argc, char *argv[]) 
{
  // Setting up the app
  QApplication ESD(argc, argv);
  QSqlDatabase* db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")); // ("QMYSQL"));
  db->setDatabaseName("/home/wouterspekkink/Programming/ESD/ESD_Linux/Test.db");
  bool ok = db->open();
  QSqlQuery *query = new QSqlQuery;
  
  query->exec("CREATE TABLE incident "
	     "(id integer, "
	     "timestamp varchar(20), "
	     "description text, "
	     "raw text, "
	     "comments text, "
	     "source text)");
  //  query->exec("INSERT INTO incident(id, timestamp, description, source) "
  //	     "VALUES (1, 'Today', 'Nothing happened', 'My lack of imagination')");

  QSqlTableModel *model = new QSqlTableModel;
  int max = model->rowCount();
  model->setTable("incident");
  model->insertRows(max, 1);
  model->setData(model->index(max, 0), max + 1);
  model->setData(model->index(max, 1), "Today");
  model->setData(model->index(max, 2), "Nothing happened");
  model->setData(model->index(max, 3), "My lack of imagination");
  model->submitAll();

  max = model->rowCount();  
  model->insertRows(max, 1);  
  model->setData(model->index(max, 0), max + 1);
  model->setData(model->index(max, 1), "Tomorrow");
  model->setData(model->index(max, 2), "Nothing will happen either");
  model->setData(model->index(max, 3), "My lack of imagination");
  model->submitAll();
  
  QTableView *view = new QTableView;
  view->setModel(model);
  view->show();
  
  return ESD.exec();
}

