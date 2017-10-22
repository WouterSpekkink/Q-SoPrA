#include "../include/WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent, EventSequenceDatabase *submittedEsd) : QDialog(parent) {
  esd = submittedEsd;
  exitStatus = 0;
  
  titleLabel = new QLabel("<h1>Start menu</h1>");
  newDatabaseButton = new QPushButton("Create new database");
  openDatabaseButton = new QPushButton("Open existing database");
  exitButton = new QPushButton("Exit program");

  connect(newDatabaseButton, SIGNAL(clicked()), this, SLOT(newDatabase()));
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

    esd->createNew(dbName);
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
    }
  }
  this->close();
}

void WelcomeDialog::quitApp() {
  exitStatus = 1;
  this->close();
}

int WelcomeDialog::getExitStatus() {
  return exitStatus;
}
