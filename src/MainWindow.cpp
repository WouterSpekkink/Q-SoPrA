#include "../include/MainWindow.h"

MainWindow::MainWindow(QWidget *parent, EventSequenceDatabase *submittedEsd) : QMainWindow(parent) {

  // We make sure that the sql database is set.
  esd = submittedEsd;

  // We set the windows size to the maximum possible.
  QPointer<QDesktopWidget> desktop = new QDesktopWidget;
  this->resize(desktop->screenGeometry(this).size());

  // Creating the stack and te  widgets it can display.
  stacked = new QStackedWidget;
  dataWidget = new DataWidget(this, esd);
  stacked->addWidget(dataWidget);

  // Some things related to positioning.
  QPointer<QWidget> centralWidget = new QWidget; 
  stacked->setCurrentWidget(dataWidget);
  setCentralWidget(centralWidget);
  stacked->move(0,0);
  centralWidget->move(0,0);

  // Creating the Layout
  QPointer<QVBoxLayout> windowLayout = new QVBoxLayout;
  windowLayout->addWidget(stacked);
  centralWidget->setLayout(windowLayout);

  // Functions to create the menu bar.
  createActions();
  createMenus();

  // Final stuff before showing the default widget (dataWidget).
  setWindowTitle("Event Dataset");  
  stacked->show();
}

void MainWindow::createActions() {
  exitAct = new QAction(tr("&Exit program"), this);
  exitAct->setShortcuts(QKeySequence::Close);
  exitAct->setStatusTip("Exit the program");
  connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

  importAct = new QAction(tr("&Import from csv"), this);
  importAct->setStatusTip("Import existing data from csv file");
  connect(importAct, SIGNAL(triggered()), this, SLOT(importFromCsv()));
}

void MainWindow::createMenus() {
  menuBar = new QMenuBar(this);

  fileMenu = menuBar->addMenu("File");
  fileMenu->addAction(exitAct);
  fileMenu->addAction(importAct);
}

void MainWindow::importFromCsv() {
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"", tr("csv files (*.csv)"));
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data;
  bool headerFound = false;
  while (file) {
    std::string buffer;
    if (!getline(file, buffer)) break;
    std::vector<std::string> tokens;
    std::vector<std::string>::iterator it;
    splitCsvLine(&tokens, buffer);
    if (headerFound == false) {
      if (tokens[0] != "Timing" && tokens[0] != "timing") {
	QPointer<QMessageBox> errorBox = new QMessageBox;
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Timing\" in first column.");
	errorBox->exec();
	return;
      }
      if (tokens[1] != "Description" && tokens[1] != "description") {
	QPointer<QMessageBox> errorBox = new QMessageBox;
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Description\" in second column.");
	errorBox->exec();
	return;
      }
      if (tokens[2] != "Raw" && tokens[2] != "raw") {
	QPointer<QMessageBox> errorBox = new QMessageBox;
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Raw\" in third column.");
	errorBox->exec();
	return;
      }
      if (tokens[3] != "Comments" && tokens[3] != "comments") {
	QPointer<QMessageBox> errorBox = new QMessageBox;
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Comments\" in fourth column.");
	errorBox->exec();
	return;
      }
      if (tokens[4] != "Source" && tokens[4] != "source") {
	QPointer<QMessageBox> errorBox = new QMessageBox;
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Source\" in third column.");
	errorBox->exec();
	return;
      }
      headerFound = true;
    } else {
      std::vector<std::string> row;
      for (it = tokens.begin(); it != tokens.end(); it++) {
	row.push_back(*it);
      }
      data.push_back(row);
    }
  }
  std::vector<std::vector <std::string> >::iterator it;
  int counter = 0;
  for (it = data.begin(); it != data.end(); it++) {
    std::vector<std::string> currentRow = *it;
    DataWidget *dw = qobject_cast<DataWidget*>(stacked->widget(0)); 
    QString timeStamp = QString::fromStdString(currentRow[0]);
    QString description = QString::fromStdString(currentRow[1]);
    QString raw = QString::fromStdString(currentRow[2]);
    QString comment = QString::fromStdString(currentRow[3]);
    QString source = QString::fromStdString(currentRow[4]);

    dw->incidentsModel->insertRow(counter);
    dw->incidentsModel->setData(dw->incidentsModel->index(counter, 1), counter + 1);
    dw->incidentsModel->setData(dw->incidentsModel->index(counter, 2), timeStamp);
    dw->incidentsModel->setData(dw->incidentsModel->index(counter, 3), description);
    dw->incidentsModel->setData(dw->incidentsModel->index(counter, 4), raw);
    dw->incidentsModel->setData(dw->incidentsModel->index(counter, 5), comment);
    dw->incidentsModel->setData(dw->incidentsModel->index(counter, 6), source);
    dw->incidentsModel->submitAll();
    counter++;
  }
}

void MainWindow::splitCsvLine(std::vector<std::string> *tokens, std::string line) {
  std::string elem;
  std::istringstream buffer(line);
  
  while(buffer) {
    std::string temp;
    if (std::getline(buffer, temp, ',')) {
      elem += temp;
    } else {
      if (!elem.empty()) {
	tokens->push_back(elem);
	return;
      }
    }
    if (0 == (std::count(elem.begin(), elem.end(), '\"') % 2)) {
      tokens->push_back(elem);
      elem = "";
    } else {
      elem += ",";
    }
  }
}
