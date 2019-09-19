/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../include/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) 
{
  // We set the windows size to the maximum possible.
  QSize availableSize = qApp->desktop()->availableGeometry().size();
  int width = availableSize.width();
  int height = availableSize.height();
  this->resize(width, height);

  // Creating the stack and te  widgets it can display.
  stacked = new QStackedWidget(this);
  dataWidget = new DataWidget(this);
  attributesWidget = new AttributesWidget(this);
  relationshipsWidget = new RelationshipsWidget(this);
  linkagesWidget = new LinkagesWidget(this);
  journalWidget = new JournalWidget(this);
  casingWidget = new CasingWidget(this);
  eventGraphWidget = new EventGraphWidget(this);
  networkGraphWidget = new NetworkGraphWidget(this);
  occurrenceGraphWidget = new OccurrenceGraphWidget(this);
  hierarchyGraphWidget = new HierarchyGraphWidget(this);
  rawAttributesTableWidget = new RawAttributesTable(this);
  rawRelationshipsTableWidget = new RawRelationshipsTable(this);
  rawLinkagesTableWidget = new RawLinkagesTable(this);
  incidentsAttributesTableWidget = new IncidentsAttributesTable(this);
  entitiesAttributesTableWidget = new EntitiesAttributesTable(this);
  missingAttributesTableWidget = new MissingAttributesTable(this);
  missingRelationshipsTableWidget = new MissingRelationshipsTable(this);
  attributeCoverageTableWidget = new AttributeCoverageTable(this);
  relationshipCoverageTableWidget = new RelationshipCoverageTable(this);
  
  // Some of these widgets need some pointers to each other to communicate properly.
  //  DataWidget *dw = qobject_cast<DataWidget*>(dataWidget);
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(attributesWidget);
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(eventGraphWidget);
  NetworkGraphWidget *ngw = qobject_cast<NetworkGraphWidget*>(networkGraphWidget);
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(relationshipsWidget);
  OccurrenceGraphWidget *ogw = qobject_cast<OccurrenceGraphWidget*>(occurrenceGraphWidget);
  HierarchyGraphWidget *hgw = qobject_cast<HierarchyGraphWidget*>(hierarchyGraphWidget);
  RawAttributesTable *rat = qobject_cast<RawAttributesTable*>(rawAttributesTableWidget);
  RawRelationshipsTable *rrt = qobject_cast<RawRelationshipsTable*>(rawRelationshipsTableWidget);
  CasingWidget *cw = qobject_cast<CasingWidget*>(casingWidget);

  aw->setEventGraphWidget(egw);
  aw->setRelationshipsWidget(rw);
  ogw->setEventGraphWidget(egw);
  egw->setAttributesWidget(aw);
  egw->setRelationshipsWidget(rw);
  hgw->setAttributesWidget(aw);
  hgw->setRelationshipsWidget(rw);
  rw->setNetworkGraphWidget(ngw);
  rw->setEventGraphWidget(egw);
  rw->setAttributesWidget(aw);
  rat->setEventGraphWidget(egw);
  rat->setAttributesWidget(aw);
  rat->setRelationshipsWidget(rw);
  rrt->setRelationshipsWidget(rw);
  cw->setEventGraphWidget(egw);
  cw->setNetworkGraphWidget(ngw);
  cw->setOccurrenceGraphWidget(ogw);
  
  QString sliderSheet = QString("QSlider::groove:horizontal { "
				"border: 1px solid #999999; "
				"height: 6px; "
				"background: white; "
				"margin 2px 0; "
				"border-radius: 3px;} "
				"QSlider::handle:horizontal { "
				"background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
				"stop:0 #b4b4b4, stop:1 #8f8f8f);"
				"border: 1px solid #5c5c5c; "
				"width: 18px; "
				"margin: -2px 0; "
				"border-radius: 3px;} "
				"QSlider::groove:horizontal:disabled { "
				"background: #b2b1b1;} "
				"QSlider::handle:horizontal:disabled { "
				"background: #b2b1b1; "
				"border: 1px solid #787676}");
  QString treeSheet = QString("QTreeView::branch:has-siblings:!adjoins-item { "
			      "border-image: url(\":/images/vlinewhole.svg\") 0; } "
			      "QTreeView::branch:has-siblings:adjoins-item { "
			      "border-image: url(\":/images/vlinewhole.svg\") 0; "
			      "image: url(\":/images/hline.svg\"); } "
			      "QTreeView::branch:!has-children:!has-siblings:adjoins-item { "
			      "border-image: url(\":/images/vlinehalf.svg\") 0; "
			      "image: url(\":/images/hline.svg\"); }  "
			      "QTreeView::branch:has-children:!has-siblings:closed, "
			      "QTreeView::branch:closed:has-children:has-siblings { "
			      "border-image: none; "
			      "image: url(\":/images/plus.svg\"); } " 
			      "QTreeView::branch:open:has-children:!has-siblings, "
			      "QTreeView::branch:open:has-children:has-siblings { "
			      "border-image: none; "
			      "image: url(\":/images/minus.svg\"); }");

  aw->setStyleSheet(treeSheet);
  rw->setStyleSheet(treeSheet);
  cw->setStyleSheet(treeSheet);
  egw->setStyleSheet(sliderSheet + treeSheet);
  ngw->setStyleSheet(sliderSheet + treeSheet);
  hgw->setStyleSheet(sliderSheet + treeSheet);
  ogw->setStyleSheet(sliderSheet + treeSheet);
  
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
  stacked->addWidget(rawLinkagesTableWidget); // 11
  stacked->addWidget(incidentsAttributesTableWidget); // 12
  stacked->addWidget(entitiesAttributesTableWidget); // 13
  stacked->addWidget(missingAttributesTableWidget); // 14
  stacked->addWidget(missingRelationshipsTableWidget); // 15
  stacked->addWidget(casingWidget); // 16
  stacked->addWidget(attributeCoverageTableWidget); //17
  stacked->addWidget(relationshipCoverageTableWidget); //18

  // We need only a few signals
  connect(egw, SIGNAL(seeHierarchy(AbstractNode *)),
	  this, SLOT(switchToHierarchyView(AbstractNode *)));
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

  stacked->showFullScreen();
  stacked->showMaximized();
}

MainWindow::~MainWindow()
{
  stacked->removeWidget(dataWidget); 
  stacked->removeWidget(attributesWidget); 
  stacked->removeWidget(relationshipsWidget);  
  stacked->removeWidget(linkagesWidget);
  stacked->removeWidget(journalWidget); 
  stacked->removeWidget(eventGraphWidget);
  stacked->removeWidget(networkGraphWidget);
  stacked->removeWidget(occurrenceGraphWidget);
  stacked->removeWidget(hierarchyGraphWidget); 
  stacked->removeWidget(rawAttributesTableWidget); 
  stacked->removeWidget(rawRelationshipsTableWidget);
  stacked->removeWidget(rawLinkagesTableWidget);
  stacked->removeWidget(incidentsAttributesTableWidget);
  stacked->removeWidget(entitiesAttributesTableWidget);
  stacked->removeWidget(missingAttributesTableWidget);
  stacked->removeWidget(missingRelationshipsTableWidget);
  stacked->removeWidget(casingWidget);
  stacked->removeWidget(attributeCoverageTableWidget);
  stacked->removeWidget(relationshipCoverageTableWidget);

  delete dataWidget;
  delete attributesWidget;
  delete relationshipsWidget;
  delete linkagesWidget;
  delete casingWidget;
  delete journalWidget;
  delete eventGraphWidget;
  delete networkGraphWidget;
  delete occurrenceGraphWidget;
  delete hierarchyGraphWidget;
  delete rawAttributesTableWidget;
  delete rawRelationshipsTableWidget;
  delete rawLinkagesTableWidget;
  delete incidentsAttributesTableWidget;
  delete entitiesAttributesTableWidget;
  delete missingAttributesTableWidget;
  delete missingRelationshipsTableWidget;
  delete attributeCoverageTableWidget;
  delete relationshipCoverageTableWidget;
  delete stacked;
}

void MainWindow::createActions() 
{
  // Exit and data export/import menu actions
  exitAct = new QAction(tr("&Exit program"), this);
  exitAct->setShortcuts(QKeySequence::Close);
  exitAct->setStatusTip("Exit the program");
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  importAct = new QAction(tr("&Import from csv"), this);
  importAct->setStatusTip("Import existing data from csv file");
  connect(importAct, SIGNAL(triggered()), this, SLOT(importFromCsv()));

  exportAct = new QAction(tr("&Export to csv"), this);
  exportAct->setStatusTip("Export current data set to csv file");
  connect(exportAct, SIGNAL(triggered()), this, SLOT(exportToCsv()));

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

  casingViewAct = new QAction(tr("&Casing"), this);
  casingViewAct->setStatusTip("Switch to casing widget");
  connect(casingViewAct, SIGNAL(triggered()), this, SLOT(switchToCasingView()));
  
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
  connect(rawAttributesTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToRawAttributesTableView()));

  rawRelationshipsTableViewAct = new QAction(tr("&Relationships texts table"), this);
  rawRelationshipsTableViewAct->setStatusTip("Switch to relationships texts table");
  connect(rawRelationshipsTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToRawRelationshipsTableView()));

  rawLinkagesTableViewAct = new QAction(tr("&Linkages texts table"), this);
  rawLinkagesTableViewAct->setStatusTip("Switch to linkages texts table");
  connect(rawLinkagesTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToRawLinkagesTableView()));
  
  incidentsAttributesTableViewAct = new QAction(tr("&Incidents-Attributes table"), this);
  incidentsAttributesTableViewAct->setStatusTip("Switch to incidents-attributes table");
  connect(incidentsAttributesTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToIncidentsAttributesTableView()));

  entitiesAttributesTableViewAct = new QAction(tr("&Entities-Attributes table"), this);
  entitiesAttributesTableViewAct->setStatusTip("Switch to entities-attributes table");
  connect(entitiesAttributesTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToEntitiesAttributesTableView()));

  missingAttributesTableViewAct = new QAction(tr("&Incidents without attributes table"), this);
  missingAttributesTableViewAct->setStatusTip("Switch to incidents without attributes table");
  connect(missingAttributesTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToMissingAttributesTableView()));

  missingRelationshipsTableViewAct = new QAction(tr("&Incidents without relationships table"), this);
  missingRelationshipsTableViewAct->setStatusTip("Switch to incidents without relationships table");
  connect(missingRelationshipsTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToMissingRelationshipsTableView()));

  attributeCoverageTableViewAct = new QAction(tr("&Attribute coverage table"), this);
  attributeCoverageTableViewAct->setStatusTip("Switch to attribute coverage table");
  connect(attributeCoverageTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToAttributeCoverageTableView()));

  relationshipCoverageTableViewAct = new QAction(tr("&Relationship coverage table"), this);
  relationshipCoverageTableViewAct->setStatusTip("Switch to relationship coverage table");
  connect(relationshipCoverageTableViewAct, SIGNAL(triggered()),
	  this, SLOT(switchToRelationshipCoverageTableView()));
  
  // Code management actions
  exportIncidentAttributesAct = new QAction(tr("&Export incident attributes coding tree"), this);
  exportIncidentAttributesAct->setStatusTip("Export attributes coding tree");
  connect(exportIncidentAttributesAct, SIGNAL(triggered()),
	  this, SLOT(exportIncidentAttributes()));
  
  importIncidentAttributesAct = new QAction(tr("&Import incident attributes coding tree"), this);
  importIncidentAttributesAct->setStatusTip("Import attributes coding tree");
  connect(importIncidentAttributesAct, SIGNAL(triggered()),
	  this, SLOT(importIncidentAttributes()));

  exportAssignedIncidentAttributesAct = new QAction(tr("&Export assigned incident attributes"), this);
  exportAssignedIncidentAttributesAct->setStatusTip("Export assigned incident attributes");
  connect(exportAssignedIncidentAttributesAct, SIGNAL(triggered()),
	  this, SLOT(exportAssignedIncidentAttributes()));

  exportEntitiesAct = new QAction(tr("&Export entities"), this);
  exportEntitiesAct->setStatusTip("Export entities");
  connect(exportEntitiesAct, SIGNAL(triggered()),
	  this, SLOT(exportEntities()));
  
  importEntitiesAct = new QAction(tr("&Import entities"), this);
  importEntitiesAct->setStatusTip("Import entities");
  connect(importEntitiesAct, SIGNAL(triggered()),
	  this, SLOT(importEntities()));
  
  exportRelTypesAct = new QAction(tr("&Export relationship types"), this);
  exportRelTypesAct->setStatusTip("Export relationship types");
  connect(exportRelTypesAct, SIGNAL(triggered()),
	  this, SLOT(exportRelTypes()));
  
  importRelTypesAct = new QAction(tr("&Import relationship types"), this);
  importRelTypesAct->setStatusTip("Import relationship types");
  connect(importRelTypesAct, SIGNAL(triggered()),
	  this, SLOT(importRelTypes()));

  exportAssignedRelationshipsAct = new QAction(tr("&Export assigned relationships"), this);
  exportAssignedRelationshipsAct->setStatusTip("Export assigned relationships");
  connect(exportAssignedRelationshipsAct, SIGNAL(triggered()),
	  this, SLOT(exportAssignedRelationships()));  
  
  exportEntityAttributesAct = new QAction(tr("&Export entity attributes coding tree"), this);
  exportEntityAttributesAct->setStatusTip("Export entity coding tree");
  connect(exportEntityAttributesAct, SIGNAL(triggered()),
	  this, SLOT(exportEntityAttributes()));
  
  importEntityAttributesAct = new QAction(tr("&Import entity attributes coding tree"), this);
  importEntityAttributesAct->setStatusTip("Import entity coding tree");
  connect(importEntityAttributesAct, SIGNAL(triggered()),
	  this, SLOT(importEntityAttributes()));

  exportAssignedEntityAttributesAct = new QAction(tr("&Export assigned entity attributes"), this);
  exportAssignedEntityAttributesAct->setStatusTip("Export assigned entity attributes");
  connect(exportAssignedEntityAttributesAct, SIGNAL(triggered()),
	  this, SLOT(exportAssignedEntityAttributes()));
  
  setOpenGLAct = new QAction(tr("&Activate OpenGL"), this);
  setOpenGLAct->setStatusTip("Activate OpenGL rendering");
  setOpenGLAct->setCheckable(true);
  setOpenGLAct->setEnabled(false);
  setOpenGLAct->setChecked(false);
#ifndef QT_NO_OPENGL
  setOpenGLAct->setEnabled(true);
  connect(setOpenGLAct, SIGNAL(triggered()), this, SLOT(setOpenGL()));
#endif
  setAntialiasingAct = new QAction(tr("&Activate edge antialiasing"), this);
  setAntialiasingAct->setStatusTip("Activate antialiasing of edges in visualisations");
  setAntialiasingAct->setCheckable(true);
  setAntialiasingAct->setChecked(true);
  connect(setAntialiasingAct, SIGNAL(triggered()), this, SLOT(setAntialiasing()));
}

void MainWindow::createMenus() 
{
  menuBar = new QMenuBar(this);

  // We create the graphics and transfer menus first,
  // because we want to include them in the options menu.
  graphicsMenu = new QMenu("Graphics");
  graphicsMenu->addAction(setOpenGLAct);
  graphicsMenu->addAction(setAntialiasingAct);

  transferDataMenu = new QMenu("Import/Export data");
  transferDataMenu->addAction(importAct);
  transferDataMenu->addAction(exportAct);

  transferIncidentAttributeMenu = new QMenu("Incident attributes");
  transferIncidentAttributeMenu->addAction(exportIncidentAttributesAct);
  transferIncidentAttributeMenu->addAction(importIncidentAttributesAct);
  transferIncidentAttributeMenu->addAction(exportAssignedIncidentAttributesAct);
  
  transferEntitiesMenu = new QMenu("Entities");
  transferEntitiesMenu->addAction(exportEntitiesAct);
  transferEntitiesMenu->addAction(importEntitiesAct);
  transferEntitiesMenu->addAction(exportEntityAttributesAct);
  transferEntitiesMenu->addAction(importEntityAttributesAct);
  transferEntitiesMenu->addAction(exportAssignedEntityAttributesAct);

  transferRelationshipsMenu = new QMenu("Entity relationships");
  transferRelationshipsMenu->addAction(exportRelTypesAct);
  transferRelationshipsMenu->addAction(importRelTypesAct);
  transferRelationshipsMenu->addAction(exportAssignedRelationshipsAct);
  
  transferCodesMenu = new QMenu("Import/Export codes");
  transferCodesMenu->addMenu(transferIncidentAttributeMenu);
  transferCodesMenu->addMenu(transferEntitiesMenu);
  transferCodesMenu->addMenu(transferRelationshipsMenu);
    
  optionsMenu = menuBar->addMenu("Options");
  optionsMenu->addMenu(transferDataMenu);
  optionsMenu->addMenu(transferCodesMenu);
  optionsMenu->addMenu(graphicsMenu);
  optionsMenu->addAction(exitAct);

  toolMenu = menuBar->addMenu("Tools");
  toolMenu->addAction(dataViewAct);
  toolMenu->addAction(attributeViewAct);
  toolMenu->addAction(relationshipViewAct);
  toolMenu->addAction(linkageViewAct);
  toolMenu->addAction(casingViewAct);
  toolMenu->addAction(journalViewAct);

  graphMenu = menuBar->addMenu("Graphs");
  graphMenu->addAction(eventGraphViewAct);
  graphMenu->addAction(networkGraphViewAct);
  graphMenu->addAction(occurrenceGraphViewAct);

  codingTextsMenu = new QMenu("Coding texts");
  codingTextsMenu->addAction(rawAttributesTableViewAct);
  codingTextsMenu->addAction(rawRelationshipsTableViewAct);
  codingTextsMenu->addAction(rawLinkagesTableViewAct);

  coverageMenu = new QMenu("Code coverage");
  coverageMenu->addAction(attributeCoverageTableViewAct);
  coverageMenu->addAction(relationshipCoverageTableViewAct);

  attributeTablesMenu = new QMenu("Attribute tables");
  attributeTablesMenu->addAction(incidentsAttributesTableViewAct);
  attributeTablesMenu->addAction(entitiesAttributesTableViewAct);

  missingTablesMenu = new QMenu("Missing codes");
  missingTablesMenu->addAction(missingAttributesTableViewAct);
  missingTablesMenu->addAction(missingRelationshipsTableViewAct);

  tableMenu = menuBar->addMenu("Tables");
  tableMenu->addMenu(codingTextsMenu);
  tableMenu->addMenu(attributeTablesMenu);
  tableMenu->addMenu(coverageMenu);
  tableMenu->addMenu(missingTablesMenu);
  
  setMenuBar(menuBar);
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
void MainWindow::importFromCsv() 
{
  // We ask the user to open a file to import from.
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"",
						 tr("csv files (*.csv)"));
  // We then create an ifstream object that goes through the file.
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data; // To store our data in.
  bool headerFound = false; // To be used in a condition further below.
  while (file) 
    {
      // The buffer will hold each line of data as we read the file.
      std::string buffer; 
      if (!getline(file, buffer)) break; // We get the current line/

      // We should check and handle any extra line breaks in the file.
      while (checkLineBreaks(buffer) == true) 
	{
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
      if (headerFound == false) 
	{
	  if (tokens[0] != "Timing" && tokens[0] != "timing") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"Timing\" in first column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[1] != "Description" && tokens[1] != "description") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"Description\" "
					   "in second column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[2] != "Raw" && tokens[2] != "raw") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"Raw\" in third column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[3] != "Comments" && tokens[3] != "comments") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"Comments\" in "
					   "fourth column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[4] != "Source" && tokens[4] != "source") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
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
	}
      else 
	{ 
	  // We iterate through the tokens and push them into the row vector.
	  std::vector<std::string> row;
	  std::vector<std::string>::iterator it;
	  if (tokens[0] == "" || tokens[1] == "" || tokens[4] == "") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Encountered an empty cell where it is not allowed.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  for (it = tokens.begin(); it != tokens.end(); it++) 
	    {
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
  QPointer<ProgressBar> loadProgress = new ProgressBar(0, 1, (int)data.size());
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
  // For efficiency, we'll do this as a transaction.
  QSqlDatabase::database().transaction(); 
  // We create and prepare the QSqlQuery object to make the process more efficient.
  QSqlQuery *query = new QSqlQuery; 
  query->prepare("INSERT INTO incidents "
		 "(ch_order, timestamp, description, raw, comment, "
		 "source, mark) "
		 "VALUES "
		 "(:order, :timestamp, :description, :raw, :comment, "
		 ":source, :mark)");
  for (it = data.begin(); it != data.end(); it++) 
    {
      // We create all the necessary variables and write them to the table.
      std::vector<std::string> currentRow = *it;
      QString timeStamp = QString::fromStdString(currentRow[0]);
      QString description = QString::fromStdString(currentRow[1]);
      QString raw = QString::fromStdString(currentRow[2]);
      QString comment = QString::fromStdString(currentRow[3]);
      QString source = QString::fromStdString(currentRow[4]);
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
    }
  delete query; // Memory management
  QSqlDatabase::database().commit(); // Commit our transaction. 
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
bool MainWindow::checkLineBreaks(std::string line) 
{
  bool lineBreak = false; // This is the variable that will be returned.
  for (std::string::size_type i = 0; i != line.length(); i++) 
    {
      /* 
	 If we did not find a quote yet, and we find one,
	 we set the lineBreak variable to true.
      */
      if (lineBreak == false && line[i] == '"') 
	{
	  lineBreak = true;
	  /*
	    If we then find another quote, 
	    we can assume that no embedded line break exists.
	  */
	}
      else if (lineBreak == true && line[i] == '"') 
	{
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
			      std::string line) 
{
  bool inTextField = false; // To check whether or not we are in a text field.
  /* 
     The next two variables are to make sure that 
     we cut out clean segments of the line.
  */
  std::string::size_type stringLength = 0;
  std::string::size_type previousPos = 0;
  // We use this integer to count the number of quotes in a line so far.
  int quoteCount = 0; 
  for (std::string::size_type i = 0; i != line.length(); i++) 
    {
      if (line[i] == '"') 
	{
	  quoteCount++; // If we found a quote, we increment this variable.
	}
      if (inTextField == false && line[i] == '"') 
	{
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
	}
      else if (inTextField == true && line[i] == '"' &&
	       line[i + 1] != '"' && quoteCount % 2 == 0) 
	{
	  // If these conditions hold, it means that we have now left the text field.
	  inTextField = false;
	  stringLength--; // We don't want to include the quote itself.
	}
      // If we encounter a comma, it means that we have encountered a new token.
      if (inTextField == false && line[i] == ',') 
	{
	  // We remove any white spaces before the start of this new token.
	  while (line[previousPos] == ' ') 
	    {
	      previousPos++;
	      stringLength--;
	    }
	  // We create a substring here.
	  std::string tempString = line.substr(previousPos, stringLength);
	  // In case we have any remaining double quotes, let us remove them.
	  if (tempString.size() > 1) 
	    {
	      for (std::string::iterator it = tempString.begin() + 1; it != tempString.end();) 
		{
		  if (*(it - 1) == '"' && *it == '"') 
		    {
		      tempString.erase(it);
		    }
		  else 
		    {
		      it++;
		    }
		}
	    }
	  // And then we store the resulting string as a new token.
	  tokens->push_back(tempString);
	  previousPos = i + 1; // We set a new starting position for the next token.
	  stringLength = 0; // And we reset the string length.
	}
      else 
	{
	  stringLength++; // We increment the string length as we go on.
	}
    }
  // After we finish reading the line, we should still include the last token.
  while (line[previousPos] == ' ') 
    {
      previousPos++;
      stringLength--;
    }
  std::string tempString = line.substr(previousPos, stringLength);
  if (tempString.size() > 1) 
    {
      for (std::string::iterator it = tempString.begin() + 1; it != tempString.end();) 
	{
	  if (*(it - 1) == '"' && *it == '"') 
	    {
	      tempString.erase(it);
	    }
	  else 
	    {
	      it++;
	    }
	}
    }
  tokens->push_back(tempString);
}

void MainWindow::exportToCsv() 
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "Timing" << ","
	      << "Description" << "," 
	      << "Raw" << ","
	      << "Comments" << ","
	      << "Source" << "\n";
      // Let us then just fetch the data from the current incidents table.
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT timestamp, description, raw, comment, source FROM incidents "
		  "ORDER BY ch_order ASC ");
      while (query->next()) 
	{
	  QString timing = doubleQuote(query->value(0).toString());
	  QString description = doubleQuote(query->value(1).toString());
	  QString raw = doubleQuote(query->value(2).toString());
	  QString comment = doubleQuote(query->value(3).toString());
	  QString source = doubleQuote(query->value(4).toString());
	  fileOut << "\"" << timing.toStdString() << "\"" << ","
		  << "\"" << description.toStdString() << "\"" << ","
		  << "\"" << raw.toStdString() << "\"" << ","
		  << "\"" << comment.toStdString() << "\"" << ","
		  << "\"" << source.toStdString() << "\"" << "\n";
	}
      // And that should be it!
      fileOut.close();
    }
}

void MainWindow::switchToDataView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
      aw->setComment();
      RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
      rw->setComment();
      LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
      lw->setComments();
      lw->setLinkageComment();
      DataWidget *dw = qobject_cast<DataWidget*>(stacked->widget(0));
      dw->incidentsModel->select();
      dw->updateTable();
      EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
      egw->getLinkageDetails();
      egw->setComment();
      stacked->setCurrentWidget(dataWidget);
    }
}

void MainWindow::switchToAttributeView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
      lw->setComments();
      lw->setLinkageComment();
      RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
      rw->setComment();
      EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
      egw->setComment();
      AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1)); 
      aw->incidentsModel->sort(1, Qt::AscendingOrder);
      aw->incidentsModel->select();
      aw->retrieveData();
      stacked->setCurrentWidget(attributesWidget);
      aw->attributesTreeView->clearSelection();
      const QModelIndex index;
      aw->attributesTreeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
      aw->setButtons();
    }
}

void MainWindow::switchToRelationshipView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
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
      stacked->setCurrentWidget(relationshipsWidget);
      rw->relationshipsTreeView->clearSelection();
      const QModelIndex index;
      rw->relationshipsTreeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
      rw->setButtons();
    }
}

void MainWindow::switchToLinkageView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
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
      stacked->setCurrentWidget(linkagesWidget);
    }
}


void MainWindow::switchToJournalView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
      aw->setComment();
      RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
      rw->setComment();
      LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
      lw->setComments();
      lw->setLinkageComment();
      EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
      egw->setComment();
      jw->tableView->clearSelection();
      jw->logField->setText("");
      jw->resetButtons();
      stacked->setCurrentWidget(journalWidget);
    }
}

void MainWindow::switchToCasingView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
      aw->setComment();
      RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
      rw->setComment();
      LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
      lw->setComments();
      lw->setLinkageComment();
      EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
      egw->setComment();
      CasingWidget *cw = qobject_cast<CasingWidget*>(stacked->widget(16));
      cw->updateTable();
      stacked->setCurrentWidget(casingWidget);
    }
}

void MainWindow::switchToEventGraphView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
      aw->setComment();
      RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
      rw->setComment();
      LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
      lw->setComments();
      lw->setLinkageComment();
      EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
      egw->getLinkageDetails();
      egw->checkCongruence();
      showMenus(true);
      menuBar->setEnabled(true);
      egw->scene->resetItemSelection();
      stacked->setCurrentWidget(eventGraphWidget);
      egw->attributesTreeView->clearSelection();
      const QModelIndex index;
      egw->attributesTreeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
      egw->setButtons();
    }
}

void MainWindow::switchToNetworkGraphView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
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
      ngw->getTypes();
      ngw->checkCongruence();
      ngw->setVisibility();
      ngw->scene->resetItemSelection();
      stacked->setCurrentWidget(networkGraphWidget);
    }
}

void MainWindow::switchToOccurrenceGraphView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
      aw->setComment();
      RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2));
      rw->setComment();
      LinkagesWidget *lw = qobject_cast<LinkagesWidget*>(stacked->widget(3));
      lw->setComments();
      lw->setLinkageComment();
      EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
      egw->setComment();
      OccurrenceGraphWidget *ogw = qobject_cast<OccurrenceGraphWidget*>(stacked->widget(7));
      ogw->checkCongruence();
      ogw->scene->resetItemSelection();
      stacked->setCurrentWidget(occurrenceGraphWidget);
    }
}

void MainWindow::switchToHierarchyView(AbstractNode *selectedAbstractNode) 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
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
      hgw->setLabelSize(egw->getLabelSize());
      hgw->setEvents(egw->getIncidentNodes());
      hgw->setAbstractNodes(egw->getAbstractNodes());
      hgw->setEdges(egw->getEdgeVector());
      hgw->setOrigin(selectedAbstractNode);
      hgw->getEdges();
      showMenus(false);
      menuBar->setEnabled(false);
      hgw->resetTree();
      stacked->setCurrentWidget(hierarchyGraphWidget);
      hgw->attributesTreeView->clearSelection();
      const QModelIndex index;
      hgw->attributesTreeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
      hgw->setButtons();
    }
}

void MainWindow::switchToRawAttributesTableView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      // Still need to figure out what else needs to happen here.
      RawAttributesTable *rat = qobject_cast<RawAttributesTable*>(stacked->widget(9));
      rat->updateTable();
      stacked->setCurrentWidget(rawAttributesTableWidget);
    }
}

void MainWindow::switchToRawRelationshipsTableView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      // Still need to figure out what else needs to happen here.
      RawRelationshipsTable *rrt = qobject_cast<RawRelationshipsTable*>(stacked->widget(10));
      rrt->updateTable();
      stacked->setCurrentWidget(rawRelationshipsTableWidget);
    }
}

void MainWindow::switchToRawLinkagesTableView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      // Still need to figure out what else needs to happen here.
      RawLinkagesTable *rlt = qobject_cast<RawLinkagesTable*>(stacked->widget(11));
      rlt->updateTable();
      stacked->setCurrentWidget(rawLinkagesTableWidget);
    }
}

void MainWindow::switchToAttributeCoverageTableView()
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      AttributeCoverageTable *act = qobject_cast<AttributeCoverageTable*>(stacked->widget(17));
      act->updateTable();
      stacked->setCurrentWidget(attributeCoverageTableWidget);
    }
}


void MainWindow::switchToRelationshipCoverageTableView()
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      RelationshipCoverageTable *rct = qobject_cast<RelationshipCoverageTable*>(stacked->widget(18));
      rct->updateTable();
      stacked->setCurrentWidget(relationshipCoverageTableWidget);
    }
}


void MainWindow::switchToIncidentsAttributesTableView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      // Still need to figure out what else needs to happen here.
      IncidentsAttributesTable *iat = qobject_cast<IncidentsAttributesTable*>(stacked->widget(12));
      iat->updateTable();
      stacked->setCurrentWidget(incidentsAttributesTableWidget);
    }
}

void MainWindow::switchToEntitiesAttributesTableView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      // Still need to figure out what else needs to happen here.
      EntitiesAttributesTable *eat = qobject_cast<EntitiesAttributesTable*>(stacked->widget(13));
      eat->updateTable();
      stacked->setCurrentWidget(entitiesAttributesTableWidget);
    }
}

void MainWindow::switchToMissingAttributesTableView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      // Still need to figure out what else needs to happen here.
      MissingAttributesTable *mat = qobject_cast<MissingAttributesTable*>(stacked->widget(14));
      mat->updateTable();
      stacked->setCurrentWidget(missingAttributesTableWidget);
    }
}

void MainWindow::switchToMissingRelationshipsTableView() 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      // Still need to figure out what else needs to happen here.
      MissingRelationshipsTable *mrt = qobject_cast<MissingRelationshipsTable*>(stacked->widget(15));
      mrt->updateTable();
      stacked->setCurrentWidget(missingRelationshipsTableWidget);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) 
{
  JournalWidget *jw = qobject_cast<JournalWidget*>(stacked->widget(4));
  if (jw->checkChanges())
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Exiting program");
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("Are you sure you want to exit the program?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
	  event->accept();
	}
      else 
	{
	  event->ignore();
	}
    }
}

void MainWindow::exportIncidentAttributes() 
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "name" << ","
	      << "description" << ","
	      << "father" << "\n";
      /* 
	 The rest is relatively simple. We make a query return almost the entire table
	 and simply write that to a file.
      */
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT name, description, father FROM incident_attributes");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QString father = query->value(2).toString();
	  fileOut << "\"" << doubleQuote(name).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(description).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(father).toStdString() << "\"" << "\n";
	}
      delete query;
      // And that is it.
      fileOut.close();
    }
}

void MainWindow::importIncidentAttributes() 
{
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"",
						 tr("csv files (*.csv)"));
  // We then create an ifstream object that goes through the file.
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data; // To store our data in.
  bool headerFound = false; // To be used in a condition further below.
  while (file) 
    {
      // The buffer will hold each line of data as we read the file.
      std::string buffer; 
      if (!getline(file, buffer)) break; // We get the current line/
      // We should check and handle any extra line breaks in the file.
      while (checkLineBreaks(buffer) == true) 
	{
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
      if (headerFound == false) 
	{
	  if (tokens[0] != "name") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"name\" in first column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[1] != "description") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"description\" "
					   "in second column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[2] != "father") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"father\" in third column.");
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
	}
      else 
	{ 
	  // We iterate through the tokens and push them into the row vector.
	  std::vector<std::string> row;
	  std::vector<std::string>::iterator it;
	  if (tokens[0] == "" || tokens[1] == "" || tokens[2] == "") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Encountered an empty cell where it is not allowed.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  for (it = tokens.begin(); it != tokens.end(); it++) 
	    {
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
  QPointer<ProgressBar> loadProgress = new ProgressBar(0, 1, (int)data.size());
  loadProgress->setWindowTitle("Importing attributes");
  loadProgress->setAttribute(Qt::WA_DeleteOnClose);
  loadProgress->setModal(true);
  loadProgress->show();
  int counter = 1;
  std::vector<std::vector <std::string> >::iterator it;
  for (it = data.begin(); it != data.end(); it++) 
    {
      // We create all the necessary variables and write them to the table.
      std::vector<std::string> currentRow = *it;
      QString name = QString::fromStdString(currentRow[0]);
      QString description = QString::fromStdString(currentRow[1]);
      QString father = QString::fromStdString(currentRow[2]);
      QSqlQuery *query = new QSqlQuery; // For this we use the QSqlQuery object.
      // Let us make sure that we are not creating duplicates.
      query->prepare("SELECT name FROM incident_attributes "
		     "WHERE name = :name");
      query->bindValue(":name", name);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO incident_attributes "
			 "(name, description, father) "
			 "VALUES "
			 "(:name, :description, :father)");
	  query->bindValue(":name", name);
	  query->bindValue(":description", description);
	  query->bindValue(":father", father);
	  query->exec();
	}
      counter++;
      loadProgress->setProgress(counter); // Set progress and report
      qApp->processEvents(); // Make sure that the progress is visible
      delete query; // Memory management
    }
  loadProgress->close(); // We can close the progress bar.
  delete loadProgress; // Memory management
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1));
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  aw->resetTree();
  egw->resetTree();  
}

void MainWindow::exportAssignedIncidentAttributes()
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "attribute" << ","
	      << "incident" << ","
	      << "value" << "\n";
      /* 
	 Then we fetch the table, but we also have to translate the incident 
	 ids into their order values
      */
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attribute, incident, value FROM attributes_to_incidents "
		  "ORDER BY attribute");
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :id");
      while (query->next()) 
	{
	  QString attribute = query->value(0).toString();
	  int id = query->value(1).toInt();
	  QString value = query->value(2).toString();
	  query2->bindValue(":id", id);
	  query2->exec();
	  query2->first();
	  int order = query2->value(0).toInt();
	  fileOut << "\"" << doubleQuote(attribute).toStdString() << "\"" << ","
		  << "\"" << order << "\"" << ","
		  << "\"" << doubleQuote(value).toStdString() << "\"" << "\n";
	}
      delete query;
      delete query2;
      // And that is it.
      fileOut.close();
    }
}

void MainWindow::exportEntities() 
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "name" << ","
	      << "description" << ","
	      << "father" << "\n";
      /* 
	 The rest is relatively simple. We make a query return almost the entire table
	 and simply write that to a file.
      */
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT name, description, father FROM entities");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QString father = query->value(2).toString();
	  fileOut << "\"" << doubleQuote(name).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(description).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(father).toStdString() << "\"" << "\n";
	}
      delete query;
      // And that is it.
      fileOut.close();
    }
}

void MainWindow::importEntities() 
{
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"",
						 tr("csv files (*.csv)"));
  // We then create an ifstream object that goes through the file.
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data; // To store our data in.
  bool headerFound = false; // To be used in a condition further below.
  while (file) 
    {
      // The buffer will hold each line of data as we read the file.
      std::string buffer; 
      if (!getline(file, buffer)) break; // We get the current line/
      // We should check and handle any extra line breaks in the file.
      while (checkLineBreaks(buffer) == true) 
	{
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
      if (headerFound == false) 
	{
	  if (tokens[0] != "name") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"name\" in first column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[1] != "description") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"description\" "
					   "in second column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[2] != "father") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"father\" in third column.");
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
	}
      else 
	{ 
	  // We iterate through the tokens and push them into the row vector.
	  std::vector<std::string> row;
	  std::vector<std::string>::iterator it;
	  if (tokens[0] == "" || tokens[1] == "" || tokens[2] == "") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Encountered an empty cell where it is not allowed.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  for (it = tokens.begin(); it != tokens.end(); it++) 
	    {
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
  QPointer<ProgressBar> loadProgress = new ProgressBar(0, 1, (int)data.size());
  loadProgress->setWindowTitle("Importing entities");
  loadProgress->setAttribute(Qt::WA_DeleteOnClose);
  loadProgress->setModal(true);
  loadProgress->show();
  int counter = 1;
  std::vector<std::vector <std::string> >::iterator it;
  for (it = data.begin(); it != data.end(); it++) 
    {
      // We create all the necessary variables and write them to the table.
      std::vector<std::string> currentRow = *it;
      QString name = QString::fromStdString(currentRow[0]);
      QString description = QString::fromStdString(currentRow[1]);
      QString father = QString::fromStdString(currentRow[2]);
      QSqlQuery *query = new QSqlQuery; // For this we use the QSqlQuery object.
      // Let us make sure that we are not creating duplicates.
      query->prepare("SELECT name FROM entities "
		     "WHERE name = :name");
      query->bindValue(":name", name);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO entities "
			 "(name, description, father) "
			 "VALUES "
			 "(:name, :description, :father)");
	  query->bindValue(":name", name);
	  query->bindValue(":description", description);
	  query->bindValue(":father", father);
	  query->exec();
	}
      counter++;
      loadProgress->setProgress(counter); // Set progress and report
      qApp->processEvents(); // Make sure that the progress is visible
      delete query; // Memory management
    }
  loadProgress->close(); // We can close the progress bar.
  delete loadProgress; // Memory management
  AttributesWidget *aw = qobject_cast<AttributesWidget*>(stacked->widget(1)); 
  aw->resetTree();
}

void MainWindow::exportRelTypes() 
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "name" << ","
	      << "directedness" << ","
	      << "description" << "\n";
      /* 
	 The rest is relatively simple. We make a query return almost the entire table
	 and simply write that to a file.
      */
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT name, directedness, description FROM relationship_types");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString directedness = query->value(1).toString();
	  QString description = query->value(2).toString();
	  fileOut << "\"" << doubleQuote(name).toStdString() << "\"" << ","
		  << directedness.toStdString() << ","
		  << "\"" << doubleQuote(description).toStdString() << "\"" << "\n";
	}
      delete query;
      // And that is it.
      fileOut.close();
    }
}

void MainWindow::importRelTypes() 
{
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"",
						 tr("csv files (*.csv)"));
  // We then create an ifstream object that goes through the file.
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data; // To store our data in.
  bool headerFound = false; // To be used in a condition further below.
  while (file) 
    {
      // The buffer will hold each line of data as we read the file.
      std::string buffer; 
      if (!getline(file, buffer)) break; // We get the current line/
      // We should check and handle any extra line breaks in the file.
      while (checkLineBreaks(buffer) == true) 
	{
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
      if (headerFound == false) 
	{
	  if (tokens[0] != "name") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"name\" in first column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[1] != "directedness") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"directedness\" "
					   "in second column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[2] != "description") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"description\" in third column.");
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
	}
      else 
	{ 
	  // We iterate through the tokens and push them into the row vector.
	  std::vector<std::string> row;
	  std::vector<std::string>::iterator it;
	  if (tokens[0] == "" || tokens[1] == "" || tokens[2] == "") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Encountered an empty cell where it is not allowed.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  else if (!(tokens[1] == DIRECTED.toStdString() || tokens[1] == UNDIRECTED.toStdString())) 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Encountered an invalid directedness value.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  for (it = tokens.begin(); it != tokens.end(); it++) 
	    {
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
  QPointer<ProgressBar>loadProgress = new ProgressBar(0, 1, (int)data.size());
  loadProgress->setWindowTitle("Importing relationship types");
  loadProgress->setAttribute(Qt::WA_DeleteOnClose);
  loadProgress->setModal(true);
  loadProgress->show();
  int counter = 1;
  std::vector<std::vector <std::string> >::iterator it;
  for (it = data.begin(); it != data.end(); it++) 
    {
      // We create all the necessary variables and write them to the table.
      std::vector<std::string> currentRow = *it;
      QString name = QString::fromStdString(currentRow[0]);
      QString directedness = QString::fromStdString(currentRow[1]);
      QString description = QString::fromStdString(currentRow[2]);
      QSqlQuery *query = new QSqlQuery; // For this we use the QSqlQuery object.
      // Let us make sure that we are not creating duplicates.
      query->prepare("SELECT name FROM relationship_types "
		     "WHERE name = :name");
      query->bindValue(":name", name);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO relationship_types "
			 "(name, directedness, description) "
			 "VALUES "
			 "(:name, :description, :father)");
	  query->bindValue(":name", name);
	  query->bindValue(":directedness", directedness);
	  query->bindValue(":description", description);
	  query->exec();
	}
      counter++;
      loadProgress->setProgress(counter); // Set progress and report
      qApp->processEvents(); // Make sure that the progress is visible
      delete query; // Memory management
    }
  loadProgress->close(); // We can close the progress bar.
  delete loadProgress; // Memory management
  RelationshipsWidget *rw = qobject_cast<RelationshipsWidget*>(stacked->widget(2)); 
  rw->resetTree();
}


void MainWindow::exportAssignedRelationships()
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "relationship" << ","
	      << "incident" << "\n";
      /* 
	 The rest is relatively simple. We make a query return almost the entire table,
	 but we also need to translate the incident ids into their order values.

	 In addition, to understand where to split the string, we need to do a third query.
      */
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2 = new QSqlQuery;
      QSqlQuery *query3 = new QSqlQuery;
      query->exec("SELECT relationship, type, incident FROM relationships_to_incidents "
		  "ORDER BY relationship");
      query2->prepare("SELECT ch_order FROM incidents WHERE id = :id");
      query3->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
      while (query->next()) 
	{
	  QString relationship = query->value(0).toString();
	  QString type = query->value(1).toString();
	  QString id = query->value(2).toString();
	  query2->bindValue(":id", id);
	  query2->exec();
	  query2->first();
	  int order = query2->value(0).toInt();
	  query3->bindValue(":type", type);
	  query3->exec();
	  query3->first();
	  QString directedness = query3->value(0).toString();
	  QStringList relationshipParts = QStringList();
	  QString outputRelationship = QString();
	  if (directedness == DIRECTED)
	    {
	      relationshipParts = relationship.split("--->");
	      outputRelationship = relationshipParts[0] + "-[" + type + "]->" + relationshipParts[1];
	    }
	  else if (directedness == UNDIRECTED)
	    {
	      relationshipParts = relationship.split("<-->");
	      outputRelationship = relationshipParts[0] + "<-[" + type + "]->" + relationshipParts[1];
	    }

	  
	  fileOut << "\"" << doubleQuote(outputRelationship).toStdString() << "\"" << ","
		  << "\"" << order << "\"" << "\n";
	}
      delete query;
      delete query2;
      delete query3;
      // And that is it.
      fileOut.close();
    }
}

void MainWindow::exportEntityAttributes() 
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "name" << ","
	      << "description" << ","
	      << "father" << "\n";
      /* 
	 The rest is relatively simple. We make a query return almost the entire table
	 and simply write that to a file.
      */
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT name, description, father FROM entity_attributes");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QString father = query->value(2).toString();
	  fileOut << "\"" << doubleQuote(name).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(description).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(father).toStdString() << "\"" << "\n";
	}
      delete query;
      // And that is it.
      fileOut.close();
    }
}

void MainWindow::importEntityAttributes() 
{
  QString csvName = QFileDialog::getOpenFileName(this, tr("Select csv file"),"",
						 tr("csv files (*.csv)"));
  // We then create an ifstream object that goes through the file.
  std::ifstream file ((csvName.toStdString()).c_str());
  std::vector<std::vector <std::string> > data; // To store our data in.
  bool headerFound = false; // To be used in a condition further below.
  while (file) 
    {
      // The buffer will hold each line of data as we read the file.
      std::string buffer; 
      if (!getline(file, buffer)) break; // We get the current line/
      // We should check and handle any extra line breaks in the file.
      while (checkLineBreaks(buffer) == true) 
	{
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
      if (headerFound == false) 
	{
	  if (tokens[0] != "name") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"name\" in first column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[1] != "description") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"description\" "
					   "in second column.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  if (tokens[2] != "father") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Expected \"father\" in third column.");
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
	}
      else 
	{ 
	  // We iterate through the tokens and push them into the row vector.
	  std::vector<std::string> row;
	  std::vector<std::string>::iterator it;
	  if (tokens[0] == "" || tokens[1] == "" || tokens[2] == "") 
	    {
	      QPointer<QMessageBox> errorBox = new QMessageBox(this);
	      errorBox->setWindowTitle("Error");
	      errorBox->setText(tr("<b>ERROR</b>"));
	      errorBox->setInformativeText("Encountered an empty cell where it is not allowed.");
	      errorBox->exec();
	      delete errorBox;
	      return;
	    }
	  for (it = tokens.begin(); it != tokens.end(); it++) 
	    {
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
  QPointer<ProgressBar> loadProgress = new ProgressBar(0, 1, (int)data.size());
  loadProgress->setWindowTitle("Importing attributes");
  loadProgress->setAttribute(Qt::WA_DeleteOnClose);
  loadProgress->setModal(true);
  loadProgress->show();
  int counter = 1;
  std::vector<std::vector <std::string> >::iterator it;
  for (it = data.begin(); it != data.end(); it++) 
    {
      // We create all the necessary variables and write them to the table.
      std::vector<std::string> currentRow = *it;
      QString name = QString::fromStdString(currentRow[0]);
      QString description = QString::fromStdString(currentRow[1]);
      QString father = QString::fromStdString(currentRow[2]);
      QSqlQuery *query = new QSqlQuery; // For this we use the QSqlQuery object.
      // Let us make sure that we are not creating duplicates.
      query->prepare("SELECT name FROM entity_attributes "
		     "WHERE name = :name");
      query->bindValue(":name", name);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO entity_attributes "
			 "(name, description, father) "
			 "VALUES "
			 "(:name, :description, :father)");
	  query->bindValue(":name", name);
	  query->bindValue(":description", description);
	  query->bindValue(":father", father);
	  query->exec();
	}
      counter++;
      loadProgress->setProgress(counter); // Set progress and report
      qApp->processEvents(); // Make sure that the progress is visible
      delete query; // Memory management
    }
  loadProgress->close(); // We can close the progress bar.
  delete loadProgress; // Memory management
}

void MainWindow::exportAssignedEntityAttributes()
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header of the file.
      fileOut << "attribute" << ","
	      << "entity" << ","
	      << "value" << "\n";
      // Then we fetch the table and write the results
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attribute, entity, value FROM attributes_to_entities "
		  "ORDER BY attribute");
      while (query->next()) 
	{
	  QString attribute = query->value(0).toString();
	  QString entity = query->value(1).toString();
	  QString value = query->value(2).toString();
	  fileOut << "\"" << doubleQuote(attribute).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(entity).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(value).toStdString() << "\"" << "\n";
	}
      delete query;
      // And that is it.
      fileOut.close();
    }
}

void MainWindow::showMenus(bool status) 
{
  optionsMenu->menuAction()->setVisible(status);
  graphicsMenu->menuAction()->setVisible(status);
  transferDataMenu->menuAction()->setVisible(status);
  transferCodesMenu->menuAction()->setVisible(status);
  toolMenu->menuAction()->setVisible(status);
  graphMenu->menuAction()->setVisible(status);
  tableMenu->menuAction()->setVisible(status);
}

void MainWindow::setOpenGL()
{
  bool openGL = false;
  if ((graphicsMenu->actions()[0])->isChecked())
    {
      openGL = true;
    }
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  NetworkGraphWidget *ngw = qobject_cast<NetworkGraphWidget*>(stacked->widget(6));
  OccurrenceGraphWidget *ogw = qobject_cast<OccurrenceGraphWidget*>(stacked->widget(7));
  HierarchyGraphWidget *hgw = qobject_cast<HierarchyGraphWidget*>(stacked->widget(8));
  egw->setOpenGL(openGL);
  ngw->setOpenGL(openGL);
  ogw->setOpenGL(openGL);
  hgw->setOpenGL(openGL);
}

void MainWindow::setAntialiasing()
{
  bool antialiasing = false;
  if ((graphicsMenu->actions()[1])->isChecked())
    {
      antialiasing = true;
    }
  EventGraphWidget *egw = qobject_cast<EventGraphWidget*>(stacked->widget(5));
  NetworkGraphWidget *ngw = qobject_cast<NetworkGraphWidget*>(stacked->widget(6));
  OccurrenceGraphWidget *ogw = qobject_cast<OccurrenceGraphWidget*>(stacked->widget(7));
  HierarchyGraphWidget *hgw = qobject_cast<HierarchyGraphWidget*>(stacked->widget(8));
  egw->setAntialiasing(antialiasing);
  ngw->setAntialiasing(antialiasing);
  ogw->setAntialiasing(antialiasing);
  hgw->setAntialiasing(antialiasing);
}
