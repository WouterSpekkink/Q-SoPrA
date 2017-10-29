#include "../include/WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent, EventSequenceDatabase *submittedEsd) : QDialog(parent) {
  esd = submittedEsd;
  exitStatus = 0;
  
  titleLabel = new QLabel("<h1>Start menu</h1>");
  newDatabaseButton = new QPushButton("Create new database");
  openDatabaseButton = new QPushButton("Open existing database");
  exitButton = new QPushButton("Exit program");

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
      QPointer<QMessageBox> errorBox = new QMessageBox;
      errorBox->setText(tr("<b>ERROR</b>"));
      errorBox->setInformativeText("Creation of new database failed.");
      errorBox->exec();
      return;
    } else {
      QSqlQuery *query = new QSqlQuery;
      query->exec("CREATE TABLE incidents "
		  "(id integer PRIMARY KEY, "
		  "ch_order integer, "
		  "timestamp varchar(20), "
		  "description text, "
		  "raw text, "
		  "comments text, "
		  "source text)");
      query->exec("CREATE TABLE incident_attributes "
		  "(id integer PRIMARY KEY, "
		  "label varchar(20) UNIQUE, "
		  "description text)");
      query->exec("CREATE TABLE attributes_to_incidents"
		  "(id integer PRIMARY KEY, "
		  "attribute integer, "
		  "incident integer)");
      // I should later set a way to store data in this table.
      query->exec("CREATE TABLE save_data "
		  "(attributes_record integer, "
		  "linkages_record integer, "
		  "relationships_record integer)");
      query->exec("INSERT INTO save_data "
		  "(attributes_record, "
		  "linkages_record, "
		  "relationships_record) "
		  "VALUES (1, 1, 1)");
    }
  } else {
    exitStatus = 2;
  }
  if (exitStatus == 0) {
    this->close();
  }
}

void WelcomeDialog::openDatabase() {
  QString dbName = QFileDialog::getOpenFileName(this, tr("Select database"),"", tr("db files (*.db)"));
  esd->openDB(dbName);
  bool ok = esd->db->open();
  if (!ok) {
    QPointer<QMessageBox> errorBox = new QMessageBox;
    errorBox->setText(tr("<b>ERROR</b>"));
    errorBox->setInformativeText("Creation of new database failed.");
    errorBox->exec();
    return;
  } else {
    exitStatus = 0;
    this->close();
  }
}

void WelcomeDialog::quitApp() {
  exitStatus = 1;
  this->close();
}

int WelcomeDialog::getExitStatus() {
  return exitStatus;
}
