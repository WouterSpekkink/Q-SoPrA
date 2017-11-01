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
  attributesWidget = new AttributesWidget(this, esd);
  stacked->addWidget(dataWidget);
  stacked->addWidget(attributesWidget);

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
  // File menu actions
  exitAct = new QAction(tr("&Exit program"), this);
  exitAct->setShortcuts(QKeySequence::Close);
  exitAct->setStatusTip("Exit the program");
  connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

  importAct = new QAction(tr("&Import from csv"), this);
  importAct->setStatusTip("Import existing data from csv file");
  connect(importAct, SIGNAL(triggered()), this, SLOT(importFromCsv()));

  // View menu actions
  dataViewAct = new QAction(tr("&Data view"), this);
  dataViewAct->setStatusTip("Switch to data view");
  connect(dataViewAct, SIGNAL(triggered()), this, SLOT(switchToDataView()));

  attributeViewAct = new QAction(tr("&Attribute view"), this);
  dataViewAct->setStatusTip("Switch to attribute view");
  connect(attributeViewAct, SIGNAL(triggered()), this, SLOT(switchToAttributeView()));
}

void MainWindow::createMenus() {
  menuBar = new QMenuBar(this);

  fileMenu = menuBar->addMenu("File");
  fileMenu->addAction(importAct);
  fileMenu->addAction(exitAct);

  viewMenu = menuBar->addMenu("View");
  viewMenu->addAction(dataViewAct);
  viewMenu->addAction(attributeViewAct);
}

void MainWindow::importFromCsv() {
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"", tr("csv files (*.csv)"));
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data;
  bool headerFound = false;
  while (file) {
    std::string buffer;

    // First we find out if there are any break lines in the file we should get rid of.
    bool quoteFound = false;
    for (std::string::size_type i = 0; i != buffer.length(); i++) {
      if (quoteFound == false && buffer[i] == '"') {
	quoteFound = true;
      } else if (quoteFound == true && buffer[i] == '"') {
	  quoteFound = false;
      }
    }
    if (quoteFound == true) {
      QPointer<QMessageBox> errorBox = new QMessageBox;
      errorBox->setText(tr("<b>ERROR: Import cancelled</b>"));
      errorBox->setInformativeText("Unmatched quotes (\") were found in one of the lines of the file.");
      errorBox->exec();
      return;
    }
    
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
  loadProgress = new ProgressBar(0, 1, (int)data.size());
  loadProgress->setAttribute(Qt::WA_DeleteOnClose);
  loadProgress->setModal(true);
  loadProgress->show();
  
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
    dw->incidentsModel->setData(dw->incidentsModel->index(counter, 7), 0);
    dw->incidentsModel->submitAll();
    counter++;
    loadProgress->setProgress(counter + 1);
    qApp->processEvents();
  }
  loadProgress->close();
  delete loadProgress;
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1)); 
  aw->retrieveData();
}

void MainWindow::splitCsvLine(std::vector<std::string> *tokens, std::string line) {
  bool inTextField = false;
  std::string::size_type stringLength = 0;
  std::string::size_type previousPos = 0;
  int quoteCount = 0;
  for (std::string::size_type i = 0; i != line.length(); i++) {
    if (line[i] == '"') {
      quoteCount++;
    }
    if (inTextField == false && line[i] == '"') {
      inTextField = true;
      previousPos++;
      stringLength--;
    } else if (inTextField == true && line[i] == '"' && line[i + 1] != '"' && quoteCount % 2 == 0) {
      inTextField = false;
      stringLength--;
    }
    if (inTextField == false && line[i] == ',') {
      while (line[previousPos] == ' ') {
	previousPos++;
	stringLength--;
      }
      std::string tempString = line.substr(previousPos, stringLength);
      tokens->push_back(tempString);
      previousPos = i + 1;
      stringLength = 0;
    } else {
      stringLength++;
    }
  }
  while (line[previousPos] == ' ') {
    previousPos++;
    stringLength--;
  }
  std::string tempString = line.substr(previousPos, stringLength);
  tokens->push_back(tempString);
}

void MainWindow::switchToDataView() {
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  DataWidget *dw = qobject_cast<DataWidget*>(stacked->widget(0));
  dw->incidentsModel->select();
  stacked->setCurrentWidget(dataWidget);
}

void MainWindow::switchToAttributeView() {
  stacked->setCurrentWidget(attributesWidget);
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1)); 
  aw->retrieveData();
}

