#include "../include/MainWindow.h"

MainWindow::MainWindow(QWidget *parent, EventSequenceDatabase *submittedEsd) : QMainWindow(parent) {
  // We make sure that the sql database is set.
  esd = submittedEsd;

  // We set the windows size to the maximum possible.
  QPointer<QDesktopWidget> desktop = new QDesktopWidget;
  this->resize(desktop->screenGeometry(this).size());

  // Creating the stack and te  widgets it can display.
  stacked = new QStackedWidget(this);
  dataWidget = new DataWidget(this, esd);
  attributesWidget = new AttributesWidget(this);
  relationshipsWidget = new RelationshipsWidget(this);
  linkagesWidget = new LinkagesWidget(this);
  journalWidget = new JournalWidget(this);
  eventGraphWidget = new EventGraphWidget(this);
  networkGraphWidget = new NetworkGraphWidget(this);
  occurrenceGraphWidget = new OccurrenceGraphWidget(this);
  hierarchyGraphWidget = new HierarchyGraphWidget(this);
  
  // Some of these widgets need some pointers to each other to communicate properly.
  DataWidget *dw = qobject_cast<DataWidget*>(dataWidget);
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(attributesWidget);
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(linkagesWidget);
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(eventGraphWidget);
  NetworkGraphWidget *ngw = qobject_cast<NetworkGraphWidget*>(networkGraphWidget);
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(relationshipsWidget);
  OccurrenceGraphWidget *ogw = qobject_cast<OccurrenceGraphWidget*>(occurrenceGraphWidget);
  HierarchyGraphWidget *hgw = qobject_cast<HierarchyGraphWidget*>(hierarchyGraphWidget);
  
  dw->setEventGraph(egw);
  dw->setOccurrenceGraph(ogw);
  aw->setEventGraph(egw);
  aw->setOccurrenceGraph(ogw);
  lw->setEventGraph(egw);
  ogw->setEventGraph(egw);
  egw->setAttributesWidget(aw);
  egw->setOccurrenceGraph(ogw);
  hgw->setAttributesWidget(aw);
  hgw->setEventGraph(egw);
  rw->setNetworkGraph(ngw);
  
  stacked->addWidget(dataWidget);
  stacked->addWidget(attributesWidget);
  stacked->addWidget(relationshipsWidget);
  stacked->addWidget(linkagesWidget);
  stacked->addWidget(journalWidget);
  stacked->addWidget(eventGraphWidget);
  stacked->addWidget(networkGraphWidget);
  stacked->addWidget(occurrenceGraphWidget);
  stacked->addWidget(hierarchyGraphWidget);

  // We need only a few signals
  connect(egw, SIGNAL(seeHierarchy(MacroEvent *)),
	  this, SLOT(switchToHierarchyView(MacroEvent *)));
  connect(hgw, SIGNAL(goToEventGraph()), this, SLOT(switchToEventGraphView()));
  
  // Some things related to positioning.
  QPointer<QWidget> centralWidget = new QWidget(this); 
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
  setWindowTitle("SoPrA");  
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

  // Tool menu actions
  dataViewAct = new QAction(tr("&Data table"), this);
  dataViewAct->setStatusTip("Switch to data table");
  connect(dataViewAct, SIGNAL(triggered()), this, SLOT(switchToDataView()));

  attributeViewAct = new QAction(tr("&Attribute coder"), this);
  dataViewAct->setStatusTip("Switch to attribute coder");
  connect(attributeViewAct, SIGNAL(triggered()), this, SLOT(switchToAttributeView()));

  relationshipViewAct = new QAction(tr("&Relationship coder"), this);
  relationshipViewAct->setStatusTip("Switch to relationship coder");
  connect(relationshipViewAct, SIGNAL(triggered()), this, SLOT(switchToRelationshipView()));

  linkageViewAct = new QAction(tr("&Linkage coder"), this);
  linkageViewAct->setStatusTip("Switch to linkage coder");
  connect(linkageViewAct, SIGNAL(triggered()), this, SLOT(switchToLinkageView()));
  
  journalViewAct = new QAction(tr("&Journal"), this);
  journalViewAct->setStatusTip("Switch to journal");
  connect(journalViewAct, SIGNAL(triggered()), this, SLOT(switchToJournalView()));

  // Graph menu actions
  eventGraphViewAct = new QAction(tr("&Event graph"), this);
  eventGraphViewAct->setStatusTip("Switch to event graph");
  connect(eventGraphViewAct, SIGNAL(triggered()), this, SLOT(switchToEventGraphView()));

  networkGraphViewAct = new QAction(tr("&Network graph"), this);
  networkGraphViewAct->setStatusTip("Switch to network graph");
  connect(networkGraphViewAct, SIGNAL(triggered()), this, SLOT(switchToNetworkGraphView()));

  occurrenceGraphViewAct = new QAction(tr("&Occurrence graph"), this);
  occurrenceGraphViewAct->setStatusTip("Switch to occurrence graph");
  connect(occurrenceGraphViewAct, SIGNAL(triggered()), this, SLOT(switchToOccurrenceGraphView()));
}

void MainWindow::createMenus() {
  menu = new QMenuBar(this);

  fileMenu = menu->addMenu("File");
  fileMenu->addAction(importAct);
  fileMenu->addAction(exitAct);

  toolMenu = menu->addMenu("Tools");
  toolMenu->addAction(dataViewAct);
  toolMenu->addAction(attributeViewAct);
  toolMenu->addAction(relationshipViewAct);
  toolMenu->addAction(linkageViewAct);
  toolMenu->addAction(journalViewAct);

  graphMenu = menu->addMenu("Graphs");
  graphMenu->addAction(eventGraphViewAct);
  graphMenu->addAction(networkGraphViewAct);
  graphMenu->addAction(occurrenceGraphViewAct);

  setMenuBar(menu);
}

/* 
   This function is designed to facilitate importing data from an external csv file.

*/
void MainWindow::importFromCsv() {
  // We ask the user to open a file to import from.
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"",
						 tr("csv files (*.csv)"));
  // We then create an ifstream object that goes through the file.
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data; // To store our data in.
  bool headerFound = false; // To be used in a condition further below.
  while (file) {
    std::string buffer; // The buffer will hold each line of data as we read the file.
    if (!getline(file, buffer)) break; // We get the current line/

    // We should check if there are any extra line breaks in the file and handle these.
    while (checkLineBreaks(buffer) == true) {
      std::string extra;
      getline(file, extra);
      buffer = buffer + "\n\n" + extra;
    }
    std::vector<std::string> tokens; // An object to keep the separate tokens in a line.
    // Let us now it
    std::vector<std::string>::iterator it;
    splitCsvLine(&tokens, buffer);
    if (headerFound == false) {
      if (tokens[0] != "Timing" && tokens[0] != "timing") {
	QPointer<QMessageBox> errorBox = new QMessageBox(this);
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Timing\" in first column.");
	errorBox->exec();
	delete errorBox;
	return;
      }
      if (tokens[1] != "Description" && tokens[1] != "description") {
	QPointer<QMessageBox> errorBox = new QMessageBox(this);
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Description\" in second column.");
	errorBox->exec();
	delete errorBox;
	return;
      }
      if (tokens[2] != "Raw" && tokens[2] != "raw") {
	QPointer<QMessageBox> errorBox = new QMessageBox(this);
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Raw\" in third column.");
	errorBox->exec();
	delete errorBox;
	return;
      }
      if (tokens[3] != "Comments" && tokens[3] != "comments") {
	QPointer<QMessageBox> errorBox = new QMessageBox(this);
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Comments\" in fourth column.");
	errorBox->exec();
	delete errorBox;
	return;
      }
      if (tokens[4] != "Source" && tokens[4] != "source") {
	QPointer<QMessageBox> errorBox = new QMessageBox(this);
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Source\" in fifth column.");
	errorBox->exec();
	delete errorBox;
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
  DataWidget *dw = qobject_cast<DataWidget*>(stacked->widget(0));
  dw->updateTable();
  int order = dw->incidentsModel->rowCount() + 1;
  for (it = data.begin(); it != data.end(); it++) {
    std::vector<std::string> currentRow = *it;
    QString timeStamp = QString::fromStdString(currentRow[0]);
    QString description = QString::fromStdString(currentRow[1]);
    QString raw = QString::fromStdString(currentRow[2]);
    QString comment = QString::fromStdString(currentRow[3]);
    QString source = QString::fromStdString(currentRow[4]);
    QSqlQuery *query = new QSqlQuery;
    query->prepare("INSERT INTO incidents "
		   "(ch_order, timestamp, description, raw, comment, source, mark) "
		   "VALUES "
		   "(:order, :timestamp, :description, :raw, :comment, :source, :mark)");
    query->bindValue(":order", order);
    query->bindValue(":timestamp", timeStamp);
    query->bindValue(":description", description);
    query->bindValue(":raw", raw);
    query->bindValue(":comment", comment);
    query->bindValue(":source", source);
    query->bindValue(":mark", 0);
    query->exec();		   
    order++;
    loadProgress->setProgress(order);
    qApp->processEvents();
    delete query;
  }
  dw->incidentsModel->select();
  dw->updateTable();
  loadProgress->close();
  delete loadProgress;
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1)); 
  aw->retrieveData();
}

bool MainWindow::checkLineBreaks(std::string line) {
  bool lineBreak = false;
  for (std::string::size_type i = 0; i != line.length(); i++) {
    if (lineBreak == false && line[i] == '"') {
      lineBreak = true;
    } else if (lineBreak == true && line[i] == '"') {
      lineBreak = false;
    }
  }
  return lineBreak;  
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
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  DataWidget *dw = qobject_cast<DataWidget*>(stacked->widget(0));
  dw->incidentsModel->select();
  while (dw->incidentsModel->canFetchMore()) {
    dw->incidentsModel->fetchMore();
  }
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->getLinkageDetails();
  egw->setComment();
  menuBar()->setEnabled(true);
  stacked->setCurrentWidget(dataWidget);
}

void MainWindow::switchToAttributeView() {
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1)); 
  aw->incidentsModel->sort(1, Qt::AscendingOrder);
  aw->incidentsModel->select();
  aw->retrieveData();
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->setComment();
  menuBar()->setEnabled(true);
  stacked->setCurrentWidget(attributesWidget);
}

void MainWindow::switchToRelationshipView() {
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->incidentsModel->sort(1, Qt::AscendingOrder);
  rw->incidentsModel->select();
  rw->retrieveData();
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->setComment();
  menuBar()->setEnabled(true);
  stacked->setCurrentWidget(relationshipsWidget);
}

void MainWindow::switchToLinkageView() {
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->incidentsModel->sort(1, Qt::AscendingOrder);
  lw->incidentsModel->select();
  lw->retrieveData();
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->setComment();
  menuBar()->setEnabled(true);
  stacked->setCurrentWidget(linkagesWidget);
}


void MainWindow::switchToJournalView() {
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  const QModelIndex index;
  jw->tableView->clearSelection();
  jw->tableView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
  jw->tableView->setCurrentIndex(index);
  jw->logField->setText("");
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->setComment();
  menuBar()->setEnabled(true);
  stacked->setCurrentWidget(journalWidget);
}

void MainWindow::switchToEventGraphView() {
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->getLinkageDetails();
  egw->checkCongruency();
  const QModelIndex index;
  menuBar()->setEnabled(true);
  stacked->setCurrentWidget(eventGraphWidget);
}

void MainWindow::switchToNetworkGraphView() {
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->setComment();
  NetworkGraphWidget *ngw = qobject_cast<NetworkGraphWidget*>(stacked->widget(6));
  ngw->checkCongruency();
  menuBar()->setEnabled(true);
  stacked->setCurrentWidget(networkGraphWidget);
}

void MainWindow::switchToOccurrenceGraphView() {
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->setComment();
  menuBar()->setEnabled(true);
  OccurrenceGraphWidget *ogw = qobject_cast<OccurrenceGraphWidget*>(stacked->widget(7));
  ogw->checkCongruency();
  stacked->setCurrentWidget(occurrenceGraphWidget);
}

void MainWindow::switchToHierarchyView(MacroEvent *selectedMacro) {
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  aw->setComment();
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
  rw->setComment();
  LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
  lw->setComments();
  lw->setLinkageComment();
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  egw->setComment();
  HierarchyGraphWidget *hgw = qobject_cast<HierarchyGraphWidget*>(stacked->widget(8));
  hgw->cleanUp();
  hgw->setEvents(egw->getEventItems());
  hgw->setMacros(egw->getMacros());
  hgw->setOrigin(selectedMacro);
  menuBar()->setEnabled(false);
  stacked->setCurrentWidget(hierarchyGraphWidget);
}
