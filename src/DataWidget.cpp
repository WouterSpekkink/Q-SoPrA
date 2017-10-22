#include "../include/DataWidget.h"

DataWidget::DataWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  // This widget uses a table model.
  incidentsModel = new QSqlTableModel;
  tableView = new QTableView(this);

  // And the model shows data from the incidents table.
  incidentsModel->setTable("incidents");
  incidentsModel->select();
  tableView->setModel(incidentsModel);
  
  // We prepare some display related options.
  incidentsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Timing"));
  incidentsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Description"));
  incidentsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Raw"));
  incidentsModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Comments"));
  incidentsModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Source"));
  tableView->setColumnHidden(0, true);
  tableView->setColumnHidden(1, true);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnWidth(2, parent->width()/15);
  tableView->setColumnWidth(3, parent->width()/4);
  tableView->setColumnWidth(4, parent->width()/4);
  tableView->setColumnWidth(5, parent->width()/5);
  tableView->setColumnWidth(6, parent->width()/8);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );


  // Then we create our other controls.
  appendRecordButton = new QPushButton("Append incident");

  // We set the connections
  connect(appendRecordButton, SIGNAL(clicked()), this, SLOT(appendRecord()));
  
  // Then we create the layout.
  QPointer<QHBoxLayout> recordButtonsLayout = new QHBoxLayout;
  recordButtonsLayout->addWidget(appendRecordButton);
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  mainLayout->addLayout(recordButtonsLayout);

  // Final settings.
  setLayout(mainLayout);
}

void DataWidget::appendRecord() {
  recordDialog = new RecordDialog;
  recordDialog->exec();
  if (recordDialog->getExitStatus() != 1) {
    QString timeStamp = recordDialog->getTimeStamp();
    QString description = recordDialog->getDescription();
    QString raw = recordDialog->getRaw();
    QString comment = recordDialog->getComment();
    QString source = recordDialog->getSource();
    // I should abstract this to a function maybe?
    int max = incidentsModel->rowCount();
    incidentsModel->insertRows(max, 1);
    incidentsModel->setData(incidentsModel->index(max, 1), max + 1);
    incidentsModel->setData(incidentsModel->index(max, 2), timeStamp);
    incidentsModel->setData(incidentsModel->index(max, 3), description);
    incidentsModel->setData(incidentsModel->index(max, 4), raw);
    incidentsModel->setData(incidentsModel->index(max, 5), comment);
    incidentsModel->setData(incidentsModel->index(max, 6), source);
    incidentsModel->submitAll();
    delete recordDialog;
  } else {
    delete recordDialog;
  }
}
