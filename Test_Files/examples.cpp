#include <QtWidgets/QApplication>
#include <QtSql>
#include <QTableView>
#include <QSqlRelationalTableModel>
#include <iostream>

int main(int argc, char *argv[]) 
{
  // Setting up the app
  QApplication ESD(argc, argv);
  QSqlDatabase* db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")); // ("QMYSQL"));
  db->setDatabaseName("./Test.db");
  bool ok = db->open();
  QSqlQuery *query = new QSqlQuery;
  
  query->exec("CREATE TABLE incidents "
	      "(id integer PRIMARY KEY, "
	      "timestamp varchar(20), "
	      "description text, "
	      "raw text, "
	      "comments text, "
	      "source text)");

  query->exec("CREATE TABLE linkages "
	      "(source integer, "
	      "description text, "
	      "target integer)");
  
  QSqlTableModel *model = new QSqlTableModel;
  
  int max = model->rowCount();
  model->setTable("incidents");
  model->insertRows(max, 1);
  model->setData(model->index(max, 1), "Today");
  model->setData(model->index(max, 2), "Nothing happened");
  model->setData(model->index(max, 3), "My lack of imagination");
  model->submitAll();

  max = model->rowCount();  
  model->insertRows(max, 1);
  model->setData(model->index(max, 1), "Tomorrow");
  model->setData(model->index(max, 2), "Nothing will happen either");
  model->setData(model->index(max, 3), "My lack of imagination");
  model->submitAll();

  model->setTable("linkages");

  QSqlQueryModel *qModel = new QSqlQueryModel;
  qModel->setQuery("SELECT * FROM incidents");
  int tempSource = qModel->record(1).value("id").toInt();
  int tempTarget = qModel->record(0).value("id").toInt();

  int rows = 0;

  // This query and while loop will give me the row numbers
  query->exec("SELECT id, (SELECT COUNT(*) FROM incidents b WHERE b.description <= a.description) FROM incidents a");
  while (query->next()) {
    rows = query->value(1).toInt();
    std::cout << rows << "\n";
  }

  max = model->rowCount();
  model->insertRows(max, 1);
  model->setData(model->index(max, 0), tempSource);
  model->setData(model->index(max, 1), "is response to");
  model->setData(model->index(max, 2), tempTarget);
  model->submitAll();

    QSqlRelationalTableModel *rModel = new QSqlRelationalTableModel;
  rModel->setTable("linkages");
  rModel->setRelation(0, QSqlRelation("incidents", "id", "description"));
  rModel->setRelation(2, QSqlRelation("incidents", "id", "description"));
  rModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Source"));
  rModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Label"));
  rModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Target"));
  
  rModel->select();
  
  // But this can also be achieved by the table model.
  QTableView *view = new QTableView;
  model->setTable("incidents"); // To set the table model back to incidents.
  model->select();  // This is necessary to populate the model with data.
  view->setModel(model);
  view->setColumnHidden(0, true); // This is necessary to Hide the id field, because we don't want it.
  view->show();

  delete query;
  delete qModel;   
  
  return ESD.exec();
}

