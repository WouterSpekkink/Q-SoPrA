#include "../include/MainDialog.h"

MainDialog::MainDialog(EventSequenceDatabase *submittedEsd) {
  esd = submittedEsd;

  incidentsModel = new QSqlTableModel;
  tableView = new QTableView;

  incidentsModel->setTable("incidents");
  incidentsModel->select();
  incidentsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Order"));

  tableView->setModel(incidentsModel);
  // We don't want to show the first two columns to the user.
  tableView->setColumnHidden(0, true);
  tableView->setColumnHidden(1, true);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnWidth(2, this->width()/5);
  tableView->setColumnWidth(3, this->width()/2);
  tableView->setColumnWidth(4, this->width()/2);
  tableView->setColumnWidth(5, this->width()/3);
  tableView->setColumnWidth(6, this->width()/5);
  
  dataViewButton = new QPushButton("Data view");
  linkageViewButton = new QPushButton("Linkage coder");
  attributesViewButton = new QPushButton("Attributes coder");
  relationshipsViewButton = new QPushButton("Relationships coder");
  exitButton = new QPushButton("Exit program");

  appendRecordButton = new QPushButton("Append incident");
  
  connect(exitButton, SIGNAL(clicked()), this, SLOT(close())); 
  
  QPointer<QHBoxLayout> switchButtonsLayout = new QHBoxLayout;
  switchButtonsLayout->addWidget(dataViewButton);
  switchButtonsLayout->addWidget(linkageViewButton);
  switchButtonsLayout->addWidget(attributesViewButton);
  switchButtonsLayout->addWidget(relationshipsViewButton);
  switchButtonsLayout->addWidget(exitButton);

  QPointer<QHBoxLayout> recordButtonsLayout = new QHBoxLayout;
  recordButtonsLayout->addWidget(appendRecordButton);
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addLayout(switchButtonsLayout);
  mainLayout->addWidget(tableView);
  mainLayout->addLayout(recordButtonsLayout);

  setWindowTitle("Event Dataset");
  setWindowState(Qt::WindowMaximized);
  
  setLayout(mainLayout);
}
