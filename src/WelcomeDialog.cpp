#include "../include/WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent, EventSequenceDatabase *submittedEsd) : QDialog(parent) {
  esd = submittedEsd;
  exitStatus = 0;
  
  titleLabel = new QLabel("<h1>Start menu</h1>", this);
  newDatabaseButton = new QPushButton("Create new database", this);
  openDatabaseButton = new QPushButton("Open existing database", this);
  exitButton = new QPushButton("Exit program", this);

  connect(newDatabaseButton, SIGNAL(clicked()), this, SLOT(newDatabase()));
  connect(openDatabaseButton, SIGNAL(clicked()), this, SLOT(openDatabase()));
  connect(exitButton, SIGNAL(clicked()), this, SLOT(quitApp()));

  QPointer<QVBoxLayout> welcomeLayout = new QVBoxLayout;
  welcomeLayout->addWidget(titleLabel);
  welcomeLayout->addWidget(newDatabaseButton);
  welcomeLayout->addWidget(openDatabaseButton);
  welcomeLayout->addWidget(exitButton);
  setWindowTitle("Event Dataset");
  setLayout(welcomeLayout);  
}

void WelcomeDialog::newDatabase() {
  QString dbName = QFileDialog::getSaveFileName(this, tr("New database"),"", tr("db files (*.db)"));
  if (!dbName.trimmed().isEmpty()) {
    if (!dbName.endsWith(".db")) {
      dbName.append(".db");
    }
    QFileInfo check_file(dbName);
    if (check_file.exists() && check_file.isFile()) {
      QFile::remove(dbName);
    }

    esd->openDB(dbName);
    bool ok = esd->db->open();
    if (!ok) {
      QPointer<QMessageBox> errorBox = new QMessageBox(this);
      errorBox->setText(tr("<b>ERROR</b>"));
      errorBox->setInformativeText("Creation of new database failed.");
      errorBox->exec();
      return;
    } else {
      QSqlQuery *query = new QSqlQuery;
      query->exec("CREATE TABLE incidents "
		  "(id integer PRIMARY KEY, "
		  "ch_order integer, "
		  "timestamp text, "
		  "description text, "
		  "raw text, "
		  "comment text, "
		  "source text, "
		  "mark integer)");
      query->exec("CREATE TABLE incident_attributes "
		  "(id integer PRIMARY KEY, "
		  "name varchar text, "
		  "description text, "
		  "father text)");
      query->exec("CREATE TABLE attributes_to_incidents "
		  "(id integer PRIMARY KEY, "
		  "attribute text, "
		  "incident integer, "
		  "value text)");
      query->exec("CREATE TABLE attributes_to_incidents_sources "
		  "(id integer PRIMARY KEY, "
		  "attribute text, "
		  "incident integer, "
		  "source_text)");
      query->exec("CREATE TABLE entity_attributes "
		  "(id integer PRIMARY KEY, "
		  "name text, "
		  "description text, "
		  "father text)");
      query->exec("CREATE TABLE attributes_to_entities "
		  "(id integer PRIMARY KEY, "
		  "attribute text, "
		  "entity text, "
		  "value text, "
		  "new integer)");
      query->exec("CREATE TABLE entity_relationships "
		  "(id integer PRIMARY KEY, "
		  "name text, "
		  "source text, "
		  "target text, "
		  "comment text, "
		  "type text)");
      query->exec("CREATE TABLE relationship_types "
		  "(id integer PRIMARY KEY, "
		  "name text, "
		  "directedness, "
		  "description)");
      query->exec("CREATE TABLE relationships_to_incidents "
		  "(id integer PRIMARY KEY, "
		  "relationship text, "
		  "incident integer)");
      query->exec("CREATE TABLE relationships_to_incidents_sources "
		  "(id integer PRIMARY KEY, "
		  "relationship text, "
		  "incident integer, "
		  "source_text text)");
      query->exec("CREATE TABLE entities "
		  "(id integer, "
		  "name text, "
		  "description text)");
      query->exec("CREATE TABLE journal "
		  "(id integer PRIMARY KEY, "
		  "time text, "
		  "entry text)");
      query->exec("CREATE TABLE linkage_types "
		  "(id integer PRIMARY KEY, "
		  "name TEXT, "
		  "description text, "
		  "question text, "
		  "direction text)");
      query->exec("CREATE TABLE linkages "
		  "(id integer PRIMARY KEY, "
		  "tail integer, "
		  "head integer, "
		  "type text, "
		  "coder text)");
      query->exec("CREATE TABLE coders "
		  "(id integer PRIMARY KEY, "
		  "name text)");
      query->exec("CREATE TABLE coders_to_linkage_types "
		  "(id integer PRIMARY KEY, "
		  "coder text, "
		  "type text, "
		  "tail integer, "
		  "head integer)");
      query->exec("CREATE TABLE linkage_comments "
		  "(id integer PRIMARY KEY, "
		  "tail integer, "
		  "head integer, "
		  "comment text, "
		  "coder text, "
		  "type text)");
      query->exec("CREATE TABLE save_data "
		  "(attributes_record integer, "
		  "relationships_record integer)");
      query->exec("INSERT INTO save_data "
		  "(attributes_record, "
		  "relationships_record) "
		  "VALUES (1, 1)");
      delete query;
    }
    exitStatus = 0;
  } else {
    exitStatus = 2;
  }
  if (exitStatus == 0) {
    this->close();
  }
}

void WelcomeDialog::openDatabase() {
  QString dbName = QFileDialog::getOpenFileName(this, tr("Select database"),"", tr("db files (*.db)"));
  if (!dbName.trimmed().isEmpty()) {
    esd->openDB(dbName);
    bool ok = esd->db->open();
    if (!ok) {
      QPointer<QMessageBox> errorBox = new QMessageBox(this);
      errorBox->setText(tr("<b>ERROR</b>"));
      errorBox->setInformativeText("Creation of new database failed.");
      errorBox->exec();
      delete errorBox;
      return;
    } else {
      exitStatus = 0;
      this->close();
    }
  } else {
    exitStatus = 2;
  }
}

void WelcomeDialog::quitApp() {
  exitStatus = 1;
  this->close();
}

int WelcomeDialog::getExitStatus() {
  return exitStatus;
}
