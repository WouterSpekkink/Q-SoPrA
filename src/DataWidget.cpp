#include "../include/DataWidget.h"

DataWidget::DataWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;
  currentData = "";
  currentRow = 0;
  
  // This widget uses a table model.
  incidentsModel = new EventTableModel(this);
  tableView = new ZoomableTableView(this);

  // And the model shows data from the incidents table.
  incidentsModel->setTable("incidents");
  incidentsModel->setSort(1, Qt::AscendingOrder);
  incidentsModel->select();
  tableView->setModel(incidentsModel);
  updateTable();
  
  // We prepare some display related options.
  incidentsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Timing"));
  incidentsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Description"));
  incidentsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Raw"));
  incidentsModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Comments"));
  incidentsModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Source"));
  incidentsModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Marked"));
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
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);
  tableView->setItemDelegateForColumn(7, new CheckBoxDelegate(tableView));

  // Then we create our other controls.
  appendRecordButton = new QPushButton("Append incident", this);
  editRecordButton = new QPushButton("Edit incident", this);
  insertRecordBeforeButton = new QPushButton("Insert before", this);
  insertRecordAfterButton = new QPushButton("Insert after", this);
  moveUpButton = new QPushButton("Move up", this);
  moveDownButton = new QPushButton("Move down", this);
  duplicateRowButton = new QPushButton("Duplicate incident", this);
  removeRowButton = new QPushButton("Remove incident", this);

  // We disable some buttons initially.
  editRecordButton->setEnabled(false);
  removeRowButton->setEnabled(false);
  duplicateRowButton->setEnabled(false);
  insertRecordBeforeButton->setEnabled(false);
  moveUpButton->setEnabled(false);
  insertRecordAfterButton->setEnabled(false);
  moveDownButton->setEnabled(false);
  
  // We set the connections
  connect(appendRecordButton, SIGNAL(clicked()), this, SLOT(appendRecord()));
  connect(editRecordButton, SIGNAL(clicked()), this, SLOT(editRecord()));
  connect(insertRecordBeforeButton, SIGNAL(clicked()), this, SLOT(insertRecordBefore()));
  connect(insertRecordAfterButton, SIGNAL(clicked()), this, SLOT(insertRecordAfter()));
  connect(moveUpButton, SIGNAL(clicked()), this, SLOT(moveUp()));
  connect(moveDownButton, SIGNAL(clicked()), this, SLOT(moveDown()));
  connect(duplicateRowButton, SIGNAL(clicked()), this, SLOT(duplicateRow()));
  connect(removeRowButton, SIGNAL(clicked()), this, SLOT(removeRow()));
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(resetHeader(int)));
  connect(tableView, SIGNAL(doubleClicked(const QModelIndex &)),
	  this, SLOT(saveCurrent(const QModelIndex &)));
  connect(tableView, SIGNAL(entered(const QModelIndex &)),
	  this, SLOT(saveCurrent(const QModelIndex &)));
  connect(tableView->model(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
	  this, SLOT(checkChange(const QModelIndex &, const QModelIndex &)));
  connect(tableView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
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
  mainLayout->addLayout(recordButtonsLayout);

  // Final settings.
  setLayout(mainLayout);
}

void DataWidget::setData(const int index, RecordDialog *recordDialog, const QString type) {
  int order = index + 1;
  QString timeStamp = recordDialog->getTimeStamp();
  QString description = recordDialog->getDescription();
  QString raw = recordDialog->getRaw();
  QString comment = recordDialog->getComment();
  QString source = recordDialog->getSource();
  QSqlQuery *query = new QSqlQuery;
  if (type == NEW) {
    query->prepare("INSERT INTO incidents (ch_order, timestamp, description, "
		   "raw, comment, source) "
		   "VALUES (:order, :timestamp, :description, :raw, :comment, :source)");
    query->bindValue(":order", order);
    query->bindValue(":timestamp", timeStamp);
    query->bindValue(":description", description);
    query->bindValue(":raw", raw);
    query->bindValue(":comment", comment);
    query->bindValue(":source", source);
    query->exec();
    incidentsModel->select();
    incidentsModel->sort(1, Qt::AscendingOrder);
    updateTable();
  } else {
    query->prepare("UPDATE incidents "
		   "SET timestamp = :timestamp, description = :description, raw = :raw, "
		   "comment = :comment, source = :source WHERE ch_order = :order");
    query->bindValue(":timestamp", timeStamp);
    query->bindValue(":description", description);
    query->bindValue(":raw", raw);
    query->bindValue(":comment", comment);
    query->bindValue(":source", source);
    query->bindValue(":order", order);
    query->exec();
    incidentsModel->select();
    incidentsModel->sort(1, Qt::AscendingOrder);
    updateTable();
  }
  QModelIndex newIndex = tableView->model()->index(order, 0);
  tableView->setCurrentIndex(newIndex);
  delete query;
}

void DataWidget::appendRecord() {
  recordDialog = new RecordDialog(this, esd, NEW);
  recordDialog->exec();
  if (recordDialog->getExitStatus() == 0) {
    int max = incidentsModel->rowCount();
    setData(max, recordDialog, NEW);
    delete recordDialog;
  } else {
    delete recordDialog;
  }
  updateTable();
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
}

void DataWidget::editRecord() {
  if (tableView->currentIndex().isValid()) {
    int currentOrder = tableView->currentIndex().row() + 1;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT timestamp, source, description, raw, comment "
		   "FROM incidents WHERE ch_order = :order");
    query->bindValue(":order", currentOrder);
    query->exec();
    query->first();
    QString timeStamp = query->value(0).toString();
    QString source = query->value(1).toString();
    QString description = query->value(2).toString();
    QString raw = query->value(3).toString();
    QString comment = query->value(4).toString();
    
    recordDialog = new RecordDialog(this, esd, OLD);
    recordDialog->setTimeStamp(timeStamp);
    recordDialog->setSource(source);
    recordDialog->setDescription(description);
    recordDialog->setRaw(raw);
    recordDialog->setComment(comment);
    recordDialog->initialize();
    recordDialog->exec();
    if (recordDialog->getExitStatus() != 1) {
      setData(currentOrder - 1, recordDialog, OLD);
      delete recordDialog;
    } else {
      delete recordDialog;
    }
    QModelIndex newIndex = tableView->model()->index(currentOrder - 1, 0);
    tableView->setCurrentIndex(newIndex);
    delete query;
  }
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
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
      setData(currentRow, recordDialog, NEW);
      delete query;
      delete recordDialog;
     } else {
      delete recordDialog;
    }
  }
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
}

void DataWidget::insertRecordAfter() {
  if (tableView->currentIndex().isValid()) {
    int nextRow = tableView->selectionModel()->currentIndex().row() + 1;
    recordDialog = new RecordDialog(this, esd, NEW);
    recordDialog->exec();
    if (recordDialog->getExitStatus() != 1) {
      QSqlDatabase::database().transaction();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET ch_order = ch_order + 1 WHERE ch_order > :oldOrder");
      query->bindValue(":oldOrder", nextRow);
      query->exec();
      QSqlDatabase::database().commit();
      setData(nextRow, recordDialog, NEW);
      delete query;
      delete recordDialog;
    } else {
      delete recordDialog;
    }
  }
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
}

void DataWidget::moveUp() {
  if (tableView->currentIndex().isValid()) {
    int currentOrder = tableView->currentIndex().row() + 1;
    if (currentOrder != 1) {
      QSqlDatabase::database().transaction();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET ch_order = 0 WHERE ch_order = :oldOrder");
      query->bindValue(":oldOrder", currentOrder);
      query->exec();
      query->prepare("UPDATE incidents SET ch_order = :newOrder WHERE ch_order = :oldOrder");
      query->bindValue(":newOrder", currentOrder);
      query->bindValue(":oldOrder", currentOrder - 1);
      query->exec();
      query->prepare("UPDATE incidents SET ch_order = :newOrder WHERE ch_order = 0");
      query->bindValue(":newOrder", currentOrder - 1);
      query->exec();
      QSqlDatabase::database().commit();
      incidentsModel->sort(1, Qt::AscendingOrder);
      incidentsModel->select();
      updateTable();
      QModelIndex newIndex = tableView->model()->index(currentOrder - 2, 0);
      tableView->setCurrentIndex(newIndex);
      delete query;
    }
  }
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
}

void DataWidget::moveDown() {
  if (tableView->currentIndex().isValid()) {
    int currentOrder = tableView->currentIndex().row() + 1;
    if (currentOrder != tableView->verticalHeader()->count()) {
      QSqlDatabase::database().transaction();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET ch_order = 0 WHERE ch_order = :oldOrder");
      query->bindValue(":oldOrder", currentOrder);
      query->exec();
      query->prepare("UPDATE incidents SET ch_order = :newOrder WHERE ch_order = :oldOrder");
      query->bindValue(":newOrder", currentOrder);
      query->bindValue(":oldOrder", currentOrder + 1);
      query->exec();
      query->prepare("UPDATE incidents SET ch_order = :newOrder WHERE ch_order = 0");
      query->bindValue(":newOrder", currentOrder + 1);
      query->exec();
      QSqlDatabase::database().commit();
      incidentsModel->sort(1, Qt::AscendingOrder);
      incidentsModel->select();
      updateTable();
      QModelIndex newIndex = tableView->model()->index(currentOrder, 0);
      tableView->setCurrentIndex(newIndex);
      delete query;
    }
  }
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
}

void DataWidget::duplicateRow() {
  if (tableView->currentIndex().isValid()) {
    QSqlDatabase::database().transaction();
    int currentOrder = tableView->selectionModel()->currentIndex().row() + 1;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT timestamp, source, description, raw, comment "
		   "FROM incidents WHERE ch_order = :order");
    query->bindValue(":order", currentOrder);
    query->exec();
    query->first();
    QString timeStamp = query->value(0).toString();
    QString source = query->value(1).toString();
    QString description = query->value(2).toString();
    QString raw = query->value(3).toString();
    QString comment = query->value(4).toString();
    recordDialog = new RecordDialog(this, esd, OLD);
    recordDialog->setTimeStamp(timeStamp);
    recordDialog->setSource(source);
    recordDialog->setDescription(description);
    recordDialog->setRaw(raw);
    recordDialog->setComment(comment);
    recordDialog->initialize();
    recordDialog->exec();
    if (recordDialog->getExitStatus() == 0) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET ch_order = ch_order + 1 "
		     "WHERE ch_order > :oldOrder");
      query->bindValue(":oldOrder", currentOrder);
      query->exec();
      setData(currentOrder, recordDialog, NEW);
      delete recordDialog;
    } else {
      delete recordDialog;
    }
    delete query;
    QSqlDatabase::database().commit();
    updateTable();
    QModelIndex newIndex = tableView->model()->index(currentOrder, 0);
    tableView->setCurrentIndex(newIndex);
  }
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
}

void DataWidget::removeRow() {
  if (tableView->currentIndex().isValid()) {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing an incident cannot be undone. "
				   "Are you sure you want to remove this incident?");
    if (warningBox->exec() == QMessageBox::Yes) {
      int currentOrder = tableView->currentIndex().row() + 1;
      QSqlDatabase::database().transaction();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order");
      query->bindValue(":order", currentOrder);
      query->exec();
      query->first();
      int id = 0;
      id = query->value(0).toInt();
      query->prepare("DELETE FROM incidents WHERE id = :inc");
      query->bindValue(":inc", id);
      query->exec();
      query->prepare("DELETE FROM attributes_to_incidents WHERE incident = :inc");
      query->bindValue(":inc", id);
      query->exec();
      query->prepare("DELETE FROM attributes_to_incidents_sources WHERE id = :inc");
      query->bindValue(":inc", id);
      query->exec();
      query->prepare("DELETE FROM relationships_to_incidents WHERE incident = :inc");
      query->bindValue(":inc", id);
      query->exec();
      query->prepare("DELETE FROM relationships_to_incidents_sources WHERE id = :inc");
      query->bindValue(":inc", id);
      query->exec();
      query->prepare("DELETE FROM linkages WHERE tail = :tail");
      query->bindValue(":tail", id);
      query->exec();
      query->prepare("DELETE FROM linkages WHERE head = :head");
      query->bindValue(":tail", id);
      query->exec();
      query->prepare("DELETE FROM linkage_comments WHERE tail = :tail");
      query->bindValue(":tail", id);
      query->exec();
      query->prepare("DELETE FROM linkage_comments WHERE head = :head");
      query->bindValue(":tail", id);
      query->exec();
      query->prepare("UPDATE incidents SET ch_order = ch_order - 1 WHERE ch_order > :oldOrder");
      query->bindValue(":oldOrder", currentOrder - 1);
      query->exec();
      incidentsModel->select();
      updateTable();
      QModelIndex newIndex = tableView->model()->index(currentOrder - 1, 0);
      tableView->setCurrentIndex(newIndex);
      delete query;
      QSqlDatabase::database().commit();
    }
    delete warningBox;
  }
  eventGraph->checkCongruency();
  occurrenceGraph->checkCongruency();
}

void DataWidget::resetHeader(int header) {

  tableView->verticalHeader()->resizeSection(header, 30);
}

void DataWidget::setEventGraph(EventGraphWidget *egw) {
  eventGraph = egw;
}

void DataWidget::setOccurrenceGraph(OccurrenceGraphWidget *ogw) {
  occurrenceGraph = ogw;
}

void DataWidget::updateTable() {
  while (incidentsModel->canFetchMore()) {
    incidentsModel->fetchMore();
  }
}

void DataWidget::saveCurrent(const QModelIndex &index) {
  currentData = tableView->model()->
    index(index.row(), index.column()).data(Qt::DisplayRole).toString();
}

void DataWidget::checkChange(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
  if (topLeft == bottomRight) {
    if (topLeft.column() == 2 ||
	topLeft.column() == 3 ||
	topLeft.column() == 6) {
      if (tableView->model()->index(topLeft.row(),
				    topLeft.column()).data().toString().trimmed() == "") {
	tableView->model()->setData(topLeft, currentData);
      }
    }
  }
}

void DataWidget::setButtons() {
  if (tableView->currentIndex().isValid()) {
    editRecordButton->setEnabled(true);
    removeRowButton->setEnabled(true);
    duplicateRowButton->setEnabled(true);
    insertRecordBeforeButton->setEnabled(true);
    insertRecordAfterButton->setEnabled(true);
    if (tableView->currentIndex().row() == 0) {
      moveUpButton->setEnabled(false);
    } else {
      moveUpButton->setEnabled(true);;
    }
    if (tableView->currentIndex().row() == tableView->verticalHeader()->count() - 1) {
      moveDownButton->setEnabled(false);
    } else {
      moveDownButton->setEnabled(true);
    }
  } else {
    editRecordButton->setEnabled(false);
    removeRowButton->setEnabled(false);
    duplicateRowButton->setEnabled(false);
    insertRecordBeforeButton->setEnabled(false);
    moveUpButton->setEnabled(false);
    insertRecordAfterButton->setEnabled(false);
    moveDownButton->setEnabled(false);
  }    
}
