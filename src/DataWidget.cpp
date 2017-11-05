#include "../include/DataWidget.h"

DataWidget::DataWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  // This widget uses a table model.
  incidentsModel = new EventTableModel;
  tableView = new QTableView(this);

  // And the model shows data from the incidents table.
  incidentsModel->setTable("incidents");
  incidentsModel->setSort(1, Qt::AscendingOrder);
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
  tableView->setColumnHidden(7, true);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnWidth(2, parent->width()/15);
  tableView->setColumnWidth(3, parent->width()/4);
  tableView->setColumnWidth(4, parent->width()/4);
  tableView->setColumnWidth(5, parent->width()/5);
  tableView->setColumnWidth(6, parent->width()/8);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);

  // Then we create our other controls.
  appendRecordButton = new QPushButton("Append incident");
  editRecordButton = new QPushButton("Edit incident");
  insertRecordBeforeButton = new QPushButton("Insert before");
  insertRecordAfterButton = new QPushButton("Insert after");
  moveUpButton = new QPushButton("Move up");
  moveDownButton = new QPushButton("Move down");
  duplicateRowButton = new QPushButton("Duplicate incident");
  removeRowButton = new QPushButton("Remove incident");
  
  // We set the connections
  connect(appendRecordButton, SIGNAL(clicked()), this, SLOT(appendRecord()));
  connect(editRecordButton, SIGNAL(clicked()), this, SLOT(editRecord()));
  connect(insertRecordBeforeButton, SIGNAL(clicked()), this, SLOT(insertRecordBefore()));
  connect(insertRecordAfterButton, SIGNAL(clicked()), this, SLOT(insertRecordAfter()));
  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(moveUp()));
  connect(moveDownButton, SIGNAL(clicked()), this, SLOT(moveDown()));
  connect(duplicateRowButton, SIGNAL(clicked()), this, SLOT(duplicateRow()));
  connect(removeRowButton, SIGNAL(clicked()), this, SLOT(removeRow()));
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(resetHeader(int)));
  
  // Then we create the layout.
  QPointer<QHBoxLayout> recordButtonsLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> recordButtonsLeftLayout = new QVBoxLayout;
  recordButtonsLeftLayout->addWidget(appendRecordButton);
  recordButtonsLeftLayout->addWidget(editRecordButton);
  recordButtonsLayout->addLayout(recordButtonsLeftLayout);
  QPointer<QVBoxLayout> recordButtonsMiddleLeftLayout = new QVBoxLayout;
  recordButtonsMiddleLeftLayout->addWidget(insertRecordBeforeButton);
  recordButtonsMiddleLeftLayout->addWidget(insertRecordAfterButton);
  recordButtonsLayout->addLayout(recordButtonsMiddleLeftLayout);
  QPointer<QVBoxLayout> recordButtonsMiddleRightLayout = new QVBoxLayout;
  recordButtonsMiddleRightLayout->addWidget(moveUpButton);
  recordButtonsMiddleRightLayout->addWidget(moveDownButton);
  recordButtonsLayout->addLayout(recordButtonsMiddleRightLayout);
  QPointer<QVBoxLayout> recordButtonsRightLayout = new QVBoxLayout;
  recordButtonsRightLayout->addWidget(duplicateRowButton);
  recordButtonsRightLayout->addWidget(removeRowButton);
  recordButtonsLayout->addLayout(recordButtonsRightLayout);
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  mainLayout->addLayout(recordButtonsLayout);

  // Final settings.
  setLayout(mainLayout);
}

void DataWidget::setData(const int index, RecordDialog *recordDialog, const QString type) {
  QString timeStamp = recordDialog->getTimeStamp();
  QString description = recordDialog->getDescription();
  QString raw = recordDialog->getRaw();
  QString comment = recordDialog->getComment();
  QString source = recordDialog->getSource();
  if (type == NEW) {
    incidentsModel->insertRow(index);
  } 
  incidentsModel->setData(incidentsModel->index(index, 1), index + 1);
  incidentsModel->setData(incidentsModel->index(index, 2), timeStamp);
  incidentsModel->setData(incidentsModel->index(index, 3), description);
  incidentsModel->setData(incidentsModel->index(index, 4), raw);
  incidentsModel->setData(incidentsModel->index(index, 5), comment);
  incidentsModel->setData(incidentsModel->index(index, 6), source);
  incidentsModel->setData(incidentsModel->index(index, 7), 0);
  incidentsModel->submitAll();
}

void DataWidget::appendRecord() {
  recordDialog = new RecordDialog(this, esd, NEW);
  recordDialog->exec();
  if (recordDialog->getExitStatus() == 0) {
    int max = tableView->verticalHeader()->count();
    setData(max, recordDialog, NEW);
    delete recordDialog;
  } else {
    delete recordDialog;
  }
}

void DataWidget::editRecord() {
  if (tableView->currentIndex().isValid()) {
    int currentRow = tableView->selectionModel()->currentIndex().row();
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery("SELECT * FROM incidents");
    QString timeStamp = query->record(currentRow).value("timestamp").toString();
    QString source = query->record(currentRow).value("source").toString();
    QString description = query->record(currentRow).value("description").toString();
    QString raw = query->record(currentRow).value("raw").toString();
    QString comment = query->record(currentRow).value("comment").toString();
    
    recordDialog = new RecordDialog(this, esd, OLD);
    recordDialog->setTimeStamp(timeStamp);
    recordDialog->setSource(source);
    recordDialog->setDescription(description);
    recordDialog->setRaw(raw);
    recordDialog->setComment(comment);
    recordDialog->initialize();
    recordDialog->exec();
    if (recordDialog->getExitStatus() != 1) {
      setData(currentRow, recordDialog, OLD);
      delete recordDialog;
    } else {
      delete recordDialog;
    }
    delete query;
  }
}

void DataWidget::insertRecordBefore() {
  if (tableView->currentIndex().isValid()) {
    int currentRow = tableView->selectionModel()->currentIndex().row();
    recordDialog = new RecordDialog(this, esd, NEW);    
    recordDialog->exec();
    if (recordDialog->getExitStatus() != 1) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET ch_order = ch_order + 1 WHERE ch_order > :oldOrder");
      query->bindValue(":oldOrder", currentRow);
      query->exec();
      incidentsModel->select();
      incidentsModel->submitAll();
      setData(currentRow, recordDialog, NEW);
      delete query;
      delete recordDialog;
     } else {
      delete recordDialog;
    }
  }
}

void DataWidget::insertRecordAfter() {
  if (tableView->currentIndex().isValid()) {
    int nextRow = tableView->selectionModel()->currentIndex().row() + 1;
    recordDialog = new RecordDialog(this, esd, NEW);
    recordDialog->exec();
    if (recordDialog->getExitStatus() != 1) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET ch_order = ch_order + 1 WHERE ch_order > :oldOrder");
      query->bindValue(":oldOrder", nextRow);
      query->exec();
      incidentsModel->select();
      incidentsModel->submitAll();
      setData(nextRow, recordDialog, NEW);
      delete query;
      delete recordDialog;
    } else {
      delete recordDialog;
    }
  }
}

void DataWidget::moveUp() {
  if (tableView->currentIndex().isValid()) {
    int currentRow = tableView->selectionModel()->currentIndex().row();
    if (currentRow + 1 != 1) {
      incidentsModel->setData(incidentsModel->index(currentRow - 1, 1), currentRow + 1);
      incidentsModel->submitAll();
      incidentsModel->setData(incidentsModel->index(currentRow, 1), currentRow);
      incidentsModel->submitAll();
      incidentsModel->sort(1, Qt::AscendingOrder);
      QModelIndex newIndex = tableView->model()->index(currentRow - 1, 0);
      tableView->setCurrentIndex(newIndex);
    }
  }
}

void DataWidget::moveDown() {
  if (tableView->currentIndex().isValid()) {
    int currentRow = tableView->selectionModel()->currentIndex().row();
    if (currentRow + 1 != tableView->verticalHeader()->count()) {
      incidentsModel->setData(incidentsModel->index(currentRow + 1, 1), currentRow + 1);
      incidentsModel->submitAll();
      incidentsModel->setData(incidentsModel->index(currentRow, 1), currentRow + 2);
      incidentsModel->submitAll();
      incidentsModel->sort(1, Qt::AscendingOrder);
      QModelIndex newIndex = tableView->model()->index(currentRow + 1, 0);
      tableView->setCurrentIndex(newIndex);
    }
  }
}

void DataWidget::duplicateRow() {
  if (tableView->currentIndex().isValid()) {
    int currentRow = tableView->selectionModel()->currentIndex().row();
    QSqlQueryModel *query = new QSqlQueryModel;
    query->setQuery("SELECT * FROM incidents");
    QString timeStamp = query->record(currentRow).value("timestamp").toString();
    QString source = query->record(currentRow).value("source").toString();
    QString description = query->record(currentRow).value("description").toString();
    QString raw = query->record(currentRow).value("raw").toString();
    QString comment = query->record(currentRow).value("comment").toString();
    recordDialog = new RecordDialog(this, esd, OLD);
    recordDialog->setTimeStamp(timeStamp);
    recordDialog->setSource(source);
    recordDialog->setDescription(description);
    recordDialog->setRaw(raw);
    recordDialog->setComment(comment);
    recordDialog->initialize();
    recordDialog->exec();
    if (recordDialog->getExitStatus() != 1) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET ch_order = ch_order + 1 WHERE ch_order > :oldOrder");
      query->bindValue(":oldOrder", currentRow + 1);
      query->exec();
      incidentsModel->select();
      incidentsModel->submitAll();
      setData(currentRow + 1, recordDialog, NEW);
      delete recordDialog;
    } else {
      delete recordDialog;
    }
    delete query;
  }
}

void DataWidget::removeRow() {
  if (tableView->currentIndex().isValid()) {
    QPointer<QMessageBox> warningBox = new QMessageBox;
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing an incident cannot be undone. Are you sure you want to remove this incident?");
    if (warningBox->exec() == QMessageBox::Yes) {
      int currentRow = tableView->selectionModel()->currentIndex().row();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order");
      query->bindValue(":order", currentRow + 1);
      query->exec();
      query->first();
      int id = 0;
      id = query->value(0).toInt();
      qDebug() << id;
      incidentsModel->removeRow(currentRow);
      incidentsModel->submitAll();
      incidentsModel->select();
      query->prepare("DELETE FROM attributes_to_incidents WHERE incident = :inc");
      query->bindValue(":inc", id);
      query->exec();
      query->prepare("DELETE FROM relationships_to_incidents WHERE incident = :inc");
      query->bindValue(":inc", id);
      query->exec();
      query->prepare("UPDATE incidents SET ch_order = ch_order - 1 WHERE ch_order > :oldOrder");
      query->bindValue(":oldOrder", currentRow);
      query->exec();
      incidentsModel->select();
      incidentsModel->submitAll();
    }
    delete warningBox;
  }
}

void DataWidget::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
}
