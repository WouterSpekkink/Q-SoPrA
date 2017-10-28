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
#include "ProgressBar.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);

  ~MainWindow() {};

private slots:
  void createActions();
  void createMenus();

  void switchToDataView();
  void switchToAttributeView();

  void importFromCsv();
  void splitCsvLine(std::vector<std::string> *tokens, std::string line);

private:
  QPointer<ProgressBar> loadProgress;
  QPointer<QMenuBar> menuBar;
  QPointer<QMenu> fileMenu;
  QPointer<QMenu> viewMenu;
  
  QPointer<QAction> exitAct;
  QPointer<QAction> importAct;

  QPointer<QAction> dataViewAct;
  QPointer<QAction> attributeViewAct;

  QPointer<EventSequenceDatabase> esd;

  QPointer<QPushButton> dataViewButton;
  QPointer<QPushButton> linkagesViewButton;
  QPointer<QPushButton> attributesViewButton;
  QPointer<QPushButton> relationsViewButton;
  
  QStackedWidget *stacked;
  QWidget *dataWidget;
  QWidget *attributesWidget;
  
};

#endif
