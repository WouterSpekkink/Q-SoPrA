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
  rawAttributesTableWidget = new RawAttributesTable(this);
  rawRelationshipsTableWidget = new RawRelationshipsTable(this);
  
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
  
  stacked->addWidget(dataWidget); // 0
  stacked->addWidget(attributesWidget); // 1
  stacked->addWidget(relationshipsWidget); // 2
  stacked->addWidget(linkagesWidget); // 3
  stacked->addWidget(journalWidget); // 4
  stacked->addWidget(eventGraphWidget); // 5
  stacked->addWidget(networkGraphWidget); // 6
  stacked->addWidget(occurrenceGraphWidget); // 7
  stacked->addWidget(hierarchyGraphWidget); // 8
  stacked->addWidget(rawAttributesTableWidget); // 9
  stacked->addWidget(rawRelationshipsTableWidget); // 10
  
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
  setWindowTitle("Q-SoPrA");  
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

  // Table menu actions
  rawAttributesTableViewAct = new QAction(tr("&Attributes texts table"), this);
  rawAttributesTableViewAct->setStatusTip("Switch to attributes texts table");
  connect(rawAttributesTableViewAct, SIGNAL(triggered()), this, SLOT(switchToRawAttributesTableView()));

  rawRelationshipsTableViewAct = new QAction(tr("&Relationships texts table"), this);
  rawRelationshipsTableViewAct->setStatusTip("Switch to relationships texts table");
  connect(rawRelationshipsTableViewAct, SIGNAL(triggered()), this, SLOT(switchToRawRelationshipsTableView()));

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

  tableMenu = menu->addMenu("Tables");
  tableMenu->addAction(rawAttributesTableViewAct);
  tableMenu->addAction(rawRelationshipsTableViewAct);
  
  setMenuBar(menu);
}

/* 
   This function is designed to facilitate importing data from 
   an external csv file. We read each line of data in turn, 
   check whether we should append any other lines due to 
   embedded line breaks, then cut up the resulting line 
   into tokens, and store these in a data vector before writing 
   the data into our sqlite database.

   We also check whether the file has the correct headers.
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
    // The buffer will hold each line of data as we read the file.
    std::string buffer; 
    if (!getline(file, buffer)) break; // We get the current line/

    // We should check and handle any extra line breaks in the file.
    while (checkLineBreaks(buffer) == true) {
      std::string extra;
      getline(file, extra);
      buffer = buffer + "\n\n" + extra;
    }
    // We need an object to keep the separate tokens in a line.
    std::vector<std::string> tokens; 
    // We then split the current line into different tokens.
    splitCsvLine(&tokens, buffer);
    /* 
       If we still need to import the header, let's do that first.
       We immediately check if the correct headers were used.
       If not, we report an error and return, letting the user fix the issue.
    */
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
	errorBox->setInformativeText("Expected \"Description\" "
				     "in second column.");
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
	errorBox->setInformativeText("Expected \"Comments\" in "
				     "fourth column.");
	errorBox->exec();
	delete errorBox;
	return;
      }
      if (tokens[4] != "Source" && tokens[4] != "source") {
	QPointer<QMessageBox> errorBox = new QMessageBox(this);
	errorBox->setText(tr("<b>ERROR</b>"));
	errorBox->setInformativeText("Expected \"Source\" in "
				     "fifth column.");
	errorBox->exec();
	delete errorBox;
	return;
      }
      /* 
	 If we checked all headers and imported the header correctly,
	 we can set the headerFound bool to true, so that this block 
	 is skipped in all subsequent line reads.
      */
      headerFound = true;

      // This is the block that is run after the header was already imported.
    } else { 
      // We iterate through the tokens and push them into the row vector.
      std::vector<std::string> row;
      std::vector<std::string>::iterator it; 
      for (it = tokens.begin(); it != tokens.end(); it++) {
	row.push_back(*it);
      }
      // Then we push the row vector into the data vector.
      data.push_back(row);
    }
  }
  /* 
     Writing to sqlite databases is quite slow, so if the 
     csv-file is large, it will take a while. We use a progress bar 
     to report to the user how much we have already imported 
     into the sqlite database.
  */
  loadProgress = new ProgressBar(0, 1, (int)data.size());
  loadProgress->setAttribute(Qt::WA_DeleteOnClose);
  loadProgress->setModal(true);
  loadProgress->show();

  /* 
     We need a pointer to the data widget to determine the 
     number of rows already in the table
  */
  DataWidget *dw = qobject_cast<DataWidget*>(stacked->widget(0));
  dw->updateTable(); // A function to make sure all table rows are read.
  /* 
     Since imported rows are appended to the end of the data base, 
     we can assume that the order variable is the number of 
     existing rows + 1.
  */
  int order = dw->incidentsModel->rowCount() + 1;
  std::vector<std::vector <std::string> >::iterator it;
  for (it = data.begin(); it != data.end(); it++) {
    // We create all the necessary variables and write them to the table.
    std::vector<std::string> currentRow = *it;
    QString timeStamp = QString::fromStdString(currentRow[0]);
    QString description = QString::fromStdString(currentRow[1]);
    QString raw = QString::fromStdString(currentRow[2]);
    QString comment = QString::fromStdString(currentRow[3]);
    QString source = QString::fromStdString(currentRow[4]);
    QSqlQuery *query = new QSqlQuery; // For this we use the QSqlQuery object.
    query->prepare("INSERT INTO incidents "
		   "(ch_order, timestamp, description, raw, comment, "
		   "source, mark) "
		   "VALUES "
		   "(:order, :timestamp, :description, :raw, :comment, "
		   ":source, :mark)");
    query->bindValue(":order", order);
    query->bindValue(":timestamp", timeStamp);
    query->bindValue(":description", description);
    query->bindValue(":raw", raw);
    query->bindValue(":comment", comment);
    query->bindValue(":source", source);
    query->bindValue(":mark", 0);
    query->exec();		   
    order++; // Make sure that we increment the order variable.
    loadProgress->setProgress(order); // Set progress and report
    qApp->processEvents(); // Make sure that the progress is visible
    delete query; // Memory management
  }
  loadProgress->close(); // We can close the progress bar.
  delete loadProgress; // Memory management
  // The imported data won't be visible unless we run the following lines.
  dw->incidentsModel->select();
  dw->updateTable();
  // We should also update the attributes widget at this point. 
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1)); 
  aw->retrieveData();
}

/* 
   Function to check for line breaks in current line of imported csv file.
   The logic is that a finished line of data will always have an 
   equal number of quotes ("). If this is not the case, we can assume 
   that another line of data should be appended. 
*/
bool MainWindow::checkLineBreaks(std::string line) {
  bool lineBreak = false; // This is the variable that will be returned.
  for (std::string::size_type i = 0; i != line.length(); i++) {
    /* 
       If we did not find a quote yet, and we find one,
       we set the lineBreak variable to true.
    */
    if (lineBreak == false && line[i] == '"') {
      lineBreak = true;
      /*
	If we then find another quote, 
	we can assume that no embedded line break exists.
      */
    } else if (lineBreak == true && line[i] == '"') {
      lineBreak = false;
    }
  }
  // After reading the line, we return the result.
  return lineBreak;  
}

/* 
   Function to split csv lines.
   Here, we should be careful with embedded quotes in text fields.
   Text fields in csv-files are marked by quotes.
   We repeatedly check whether or not we are in a text field and 
   respond accordingly.

   We also clean up the line of text a bit before we cut it up.
*/
void MainWindow::splitCsvLine(std::vector<std::string> *tokens,
			      std::string line) {
  bool inTextField = false; // To check whether or not we are in a text field.
  /* 
     The next two variables are to make sure that 
     we cut out clean segments of the line.
  */
  std::string::size_type stringLength = 0;
  std::string::size_type previousPos = 0;
  // We use this integer to count the number of quotes in a line so far.
  int quoteCount = 0; 
  for (std::string::size_type i = 0; i != line.length(); i++) {
    if (line[i] == '"') {
      quoteCount++; // If we found a quote, we increment this variable.
    }
    if (inTextField == false && line[i] == '"') {
      /* 
	 Finding a quote will mean that we are now 
	 in a text field if we weren't already.
      */
      inTextField = true;
      previousPos++; // We don't want to include the quote itself.
      stringLength--;
      /* 
	 If we are in a text field, and we encounter another quote, then we
	 should first check if it is accompanied by yet another pair of quotes. 
	 This is based on the fact that csv files (if written correctly), 
	 always use double quotes for
	 quotes that are embedded in text fields. 

	 We also double check by checking if the current amount of 
	 quotes is divisible by 2, that is, if we have an 
	 equal number of quotes.
      */
    } else if (inTextField == true && line[i] == '"' &&
	       line[i + 1] != '"' && quoteCount % 2 == 0) {
      // If these conditions hold, it means that we have now left the text field.
      inTextField = false;
      stringLength--; // We don't want to include the quote itself.
    }
    // If we encounter a comma, it means that we have encountered a new token.
    if (inTextField == false && line[i] == ',') {
      // We remove any white spaces before the start of this new token.
      while (line[previousPos] == ' ') {
	previousPos++;
	stringLength--;
      }
      // We create a substring here.
      std::string tempString = line.substr(previousPos, stringLength);
      // In case we have any remaining double quotes, let us remove them.
      if (tempString.size() > 1) {
	for (std::string::iterator it = tempString.begin() + 1; it != tempString.end();) {
	  if (*(it - 1) == '"' && *it == '"') {
	    tempString.erase(it);
	  } else {
	    it++;
	  }
	}
      }
      // And then we store the resulting string as a new token.
      tokens->push_back(tempString);
      previousPos = i + 1; // We set a new starting position for the next token.
      stringLength = 0; // And we reset the string length.
    } else {
      stringLength++; // We increment the string length as we go on.
    }
  }
  // After we finish reading the line, we should still include the last token.
  while (line[previousPos] == ' ') {
    previousPos++;
    stringLength--;
  }
  std::string tempString = line.substr(previousPos, stringLength);
  if (tempString.size() > 1) {
    for (std::string::iterator it = tempString.begin() + 1; it != tempString.end();) {
      if (*(it - 1) == '"' && *it == '"') {
	tempString.erase(it);
      } else {
	it++;
      }
    }
  }
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

void MainWindow::switchToRawAttributesTableView() {
  // Still need to figure out what else needs to happen here.
  RawAttributesTable *rat = qobject_cast<RawAttributesTable*>(stacked->widget(9));
  rat->updateTable();
  stacked->setCurrentWidget(rawAttributesTableWidget);
}

void MainWindow::switchToRawRelationshipsTableView() {
  // Still need to figure out what else needs to happen here.
  RawRelationshipsTable *rrt = qobject_cast<RawRelationshipsTable*>(stacked->widget(10));
  rrt->updateTable();
  stacked->setCurrentWidget(rawRelationshipsTableWidget);
}
