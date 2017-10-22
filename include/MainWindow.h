#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QVBoxLayout>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include "EventSequenceDatabase.h"
#include "DataWidget.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);

  ~MainWindow() {};
    
private slots:
  void createActions();
  void createMenus();

private:
  QPointer<QMenuBar> menuBar;
  QPointer<QMenu> fileMenu;
  QPointer<QAction> exitAct; 

  QPointer<EventSequenceDatabase> esd;

  QPointer<QPushButton> dataViewButton;
  QPointer<QPushButton> linkagesViewButton;
  QPointer<QPushButton> attributesViewButton;
  QPointer<QPushButton> relationsViewButton;
  
  QStackedWidget *stacked;
  QWidget *dataWidget;
  
};

#endif
