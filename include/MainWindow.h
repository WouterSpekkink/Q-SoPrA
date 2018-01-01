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

class MainWindow : public QMainWindow {
  Q_OBJECT
  
public:
  MainWindow(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);

  ~MainWindow() {};

  QPointer<QStackedWidget> getStacked();
				       
private slots:
  void createActions();
  void createMenus();

  void switchToDataView();
  void switchToAttributeView();
  void switchToRelationshipView();
  void switchToLinkageView();
  void switchToJournalView();
  void switchToEventGraphView();
  void switchToNetworkGraphView();
  void switchToOccurrenceGraphView();
  void switchToHierarchyView(MacroEvent *selectedMacro);
  void switchToRawAttributesTableView();
  void switchToRawRelationshipsTableView();
  
  void importFromCsv();
  bool checkLineBreaks(std::string line);
  void splitCsvLine(std::vector<std::string> *tokens, std::string line);
  
private:
  QPointer<QStackedWidget> stacked;
  QPointer<QWidget> dataWidget;
  QPointer<QWidget> attributesWidget;
  QPointer<QWidget> relationshipsWidget;
  QPointer<QWidget> linkagesWidget;
  QPointer<QWidget> journalWidget;
  QPointer<QWidget> eventGraphWidget;
  QPointer<QWidget> networkGraphWidget;
  QPointer<QWidget> occurrenceGraphWidget;
  QPointer<QWidget> hierarchyGraphWidget;
  QPointer<QWidget> rawAttributesTableWidget;
  QPointer<QWidget> rawRelationshipsTableWidget;
  
  QPointer<ProgressBar> loadProgress;
  QPointer<QMenuBar> menu;
  QPointer<QMenu> fileMenu;
  QPointer<QMenu> toolMenu;
  QPointer<QMenu> graphMenu;
  QPointer<QMenu> tableMenu;
  
  QPointer<QAction> exitAct;
  QPointer<QAction> importAct;

  QPointer<QAction> dataViewAct;
  QPointer<QAction> attributeViewAct;
  QPointer<QAction> relationshipViewAct;
  QPointer<QAction> linkageViewAct;
  QPointer<QAction> journalViewAct;
  
  QPointer<QAction> eventGraphViewAct;
  QPointer<QAction> networkGraphViewAct;
  QPointer<QAction> occurrenceGraphViewAct;

  QPointer<QAction> rawAttributesTableViewAct;
  QPointer<QAction> rawRelationshipsTableViewAct;
  
  QPointer<EventSequenceDatabase> esd;
};

#endif
