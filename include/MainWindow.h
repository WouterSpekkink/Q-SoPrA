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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QVBoxLayout>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <string>
#include "EventSequenceDatabase.h"
#include "DataWidget.h"
#include "AttributesWidget.h"
#include "RelationshipsWidget.h"
#include "LinkagesWidget.h"
#include "JournalWidget.h"
#include "ProgressBar.h"
#include "EventGraphWidget.h"
#include "NetworkGraphWidget.h"
#include "OccurrenceGraphWidget.h"
#include "HierarchyGraphWidget.h"
#include "RawAttributesTable.h"
#include "RawRelationshipsTable.h"
#include "IncidentsAttributesTable.h"
#include "EntitiesAttributesTable.h"
#include "MissingAttributesTable.h"
#include "MissingRelationshipsTable.h"
#include "SupportingFunctions.h"
#include "Constants.h"
#include "CasingWidget.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  // Constructor and destructor
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

  // Getter
  QPointer<QStackedWidget> getStacked();
				       
private slots:
  // Private member functions
  void createActions();
  void createMenus();
  void showMenus(bool state);
  void switchToDataView();
  void switchToAttributeView();
  void switchToRelationshipView();
  void switchToLinkageView();
  void switchToJournalView();
  void switchToCasingView();
  void switchToEventGraphView();
  void switchToNetworkGraphView();
  void switchToOccurrenceGraphView();
  void switchToHierarchyView(AbstractNode *selectedAbstractNode);
  void switchToRawAttributesTableView();
  void switchToRawRelationshipsTableView();
  void switchToIncidentsAttributesTableView();
  void switchToEntitiesAttributesTableView();
  void switchToMissingAttributesTableView();
  void switchToMissingRelationshipsTableView();
  void exportIncidentAttributes();
  void importIncidentAttributes();
  void exportEntities();
  void importEntities();
  void exportRelTypes();
  void importRelTypes();
  void exportEntityAttributes();
  void importEntityAttributes();
  void importFromCsv();
  void exportToCsv();
  bool checkLineBreaks(std::string line);
  void splitCsvLine(std::vector<std::string> *tokens, std::string line);

protected:
  // Overrride event
  void closeEvent(QCloseEvent *event) override;
  
private:
  // Widgets
  QPointer<QStackedWidget> stacked;
  QPointer<QWidget> dataWidget;
  QPointer<QWidget> attributesWidget;
  QPointer<QWidget> relationshipsWidget;
  QPointer<QWidget> linkagesWidget;
  QPointer<QWidget> casingWidget;
  QPointer<QWidget> journalWidget;
  QPointer<QWidget> eventGraphWidget;
  QPointer<QWidget> networkGraphWidget;
  QPointer<QWidget> occurrenceGraphWidget;
  QPointer<QWidget> hierarchyGraphWidget;
  QPointer<QWidget> rawAttributesTableWidget;
  QPointer<QWidget> rawRelationshipsTableWidget;
  QPointer<QWidget> incidentsAttributesTableWidget;
  QPointer<QWidget> entitiesAttributesTableWidget;
  QPointer<QWidget> missingAttributesTableWidget;
  QPointer<QWidget> missingRelationshipsTableWidget;

  // Menus and actions
  QPointer<QMenuBar> menuBar;
  QPointer<QMenu> fileMenu;
  QPointer<QMenu> toolMenu;
  QPointer<QMenu> graphMenu;
  QPointer<QMenu> tableMenu;
  QPointer<QMenu> transferMenu;
  QPointer<QAction> exitAct;
  QPointer<QAction> importAct;
  QPointer<QAction> exportAct;
  QPointer<QAction> dataViewAct;
  QPointer<QAction> attributeViewAct;
  QPointer<QAction> relationshipViewAct;
  QPointer<QAction> linkageViewAct;
  QPointer<QAction> casingViewAct;
  QPointer<QAction> journalViewAct;
  QPointer<QAction> eventGraphViewAct;
  QPointer<QAction> networkGraphViewAct;
  QPointer<QAction> occurrenceGraphViewAct;
  QPointer<QAction> rawAttributesTableViewAct;
  QPointer<QAction> rawRelationshipsTableViewAct;
  QPointer<QAction> incidentsAttributesTableViewAct;
  QPointer<QAction> entitiesAttributesTableViewAct;
  QPointer<QAction> missingAttributesTableViewAct;
  QPointer<QAction> missingRelationshipsTableViewAct;
  QPointer<QAction> exportIncidentAttributesAct;
  QPointer<QAction> importIncidentAttributesAct;
  QPointer<QAction> exportEntitiesAct;
  QPointer<QAction> importEntitiesAct;
  QPointer<QAction> exportRelTypesAct;
  QPointer<QAction> importRelTypesAct;
  QPointer<QAction> exportEntityAttributesAct;
  QPointer<QAction> importEntityAttributesAct;
};

#endif
