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
  setLayout(windowLayout);

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
}

void MainWindow::createMenus() {
  menuBar = new QMenuBar(this);

  fileMenu = menuBar->addMenu("File");
  fileMenu->addAction(exitAct);
}
