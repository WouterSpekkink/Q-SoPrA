#include "../include/JournalWidget.h"

JournalWidget::JournalWidget(QWidget *parent) : QWidget(parent) {
  journalLabel = new QLabel("<h2>Journal</h2>", this);
  logLabel = new QLabel("<h2>Selected entry</h2>", this);

  journalModel = new JournalTableModel(this);
  journalModel->setTable("journal");
  journalModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Log date"));
  journalModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Entry"));
  journalModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Needs attention"));
  journalModel->select();

  tableView = new ZoomableTableView(this);
  tableView->setModel(journalModel);
  tableView->setColumnHidden(0, true);
  tableView->setColumnWidth(1, (parent->width() / 2) / 4);
  tableView->setColumnWidth(2, (parent->width() / 2) / 2);
  tableView->setColumnWidth(3, (parent->width() / 2) / 7);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setItemDelegateForColumn(3, new CheckBoxDelegate(tableView));
  
  logField = new QTextEdit(this);
  logField->setEnabled(false);
  logField->installEventFilter(this);
  
  addEntryButton = new QPushButton(tr("New entry"), this);
  saveChangesButton = new QPushButton(tr("Save changes"), this);
  saveChangesButton->setEnabled(false);
  removeEntryButton = new QPushButton(tr("Remove selected entry"), this);
  removeEntryButton->setEnabled(false);
  exportJournalButton = new QPushButton(tr("Export"), this);
  
  connect(logField, SIGNAL(textChanged()), this, SLOT(setButtons()));
  connect(addEntryButton, SIGNAL(clicked()), this, SLOT(addEntry()));
  connect(saveChangesButton, SIGNAL(clicked()), this, SLOT(saveChanges()));
  connect(removeEntryButton, SIGNAL(clicked()), this, SLOT(removeEntry()));
  connect(tableView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setData()));
  connect(tableView->verticalHeader(),
	  SIGNAL(sectionDoubleClicked(int)), this, SLOT(resetHeader(int)));
  connect(exportJournalButton, SIGNAL(clicked()), this, SLOT(exportJournal()));
  
  QPointer<QHBoxLayout> mainLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  leftLayout->addWidget(journalLabel);
  leftLayout->addWidget(tableView);
  mainLayout->addLayout(leftLayout);
  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  rightLayout->addWidget(logLabel);
  rightLayout->addWidget(logField);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(addEntryButton);
  buttonLayout->addWidget(removeEntryButton);
  buttonLayout->addWidget(saveChangesButton);
  buttonLayout->addWidget(exportJournalButton);
  rightLayout->addLayout(buttonLayout);
  mainLayout->addLayout(rightLayout);

  setLayout(mainLayout);
}

void JournalWidget::setButtons() {
  if (tableView->currentIndex().isValid()) {
    saveChangesButton->setEnabled(true);
  }
}

void JournalWidget::resetButtons() {
  saveChangesButton->setEnabled(false);
  removeEntryButton->setEnabled(false);
  logField->setEnabled(false);
}

void JournalWidget::saveChanges() {
  if (tableView->currentIndex().isValid()) {
    int currentRow = tableView->currentIndex().row();
    journalModel->setData(journalModel->index(currentRow, 2), logField->toPlainText());
    journalModel->submitAll();
    saveChangesButton->setEnabled(false);
  }
}

void JournalWidget::setData() {
  if (tableView->currentIndex().isValid()) {
    int currentRow = tableView->currentIndex().row();
    QModelIndex wantedIndex = tableView->currentIndex().sibling(currentRow, 2);
    QString currentText = wantedIndex.data(Qt::DisplayRole).toString();
    logField->blockSignals(true);
    logField->setText(currentText);
    logField->blockSignals(false);
    logField->setEnabled(true);
    saveChangesButton->setEnabled(false);
    removeEntryButton->setEnabled(true);
  } else {
    removeEntryButton->setEnabled(false);
    saveChangesButton->setEnabled(false);
    logField->setEnabled(false);
  }
}

void JournalWidget::addEntry() {
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("INSERT INTO journal (time, mark) "
		 "VALUES (:time, :mark)");
  query->bindValue(":time", timeText);
  query->bindValue(":mark", 0);
  query->exec();
  journalModel->select();
  delete query;
  if (journalModel->canFetchMore()) {
    journalModel->fetchMore();
  }
  QModelIndex newIndex = tableView->model()->index(journalModel->rowCount() - 1, 0);
  tableView->setCurrentIndex(newIndex);
}

void JournalWidget::removeEntry() {
  if (tableView->currentIndex().isValid()) {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing a journal entry cannot be undone. Are you sure you want to remove this entry?");
    if (warningBox->exec() == QMessageBox::Yes) {
      int currentRow = tableView->currentIndex().row();
      journalModel->removeRow(currentRow);
      journalModel->submitAll();
      journalModel->select();
      logField->blockSignals(true);
      logField->setText("");
      logField->blockSignals(false);
      logField->setEnabled(false);
    }
  }
}

void JournalWidget::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
}

bool JournalWidget::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = (QWheelEvent*) event;
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
    if (textEdit) {
      if(wheelEvent->modifiers() & Qt::ControlModifier) {
        if (wheelEvent->angleDelta().y() > 0) {
	  textEdit->zoomIn(1);
	} else if (wheelEvent->angleDelta().y() < 0) {
	  textEdit->zoomOut(1);
	}
      }
    }
  }
  return false;
}

void JournalWidget::exportJournal() {
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) {
    if(!fileName.endsWith(".csv")) {
      fileName.append(".csv");
    }
    // And we create a file outstream.  
    std::ofstream fileOut(fileName.toStdString().c_str());
    // We first write the header of the file.
    fileOut << "Time" << ","
	    << "Entry" << "\n";
    // And then we fetch the journal entries.
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT time, entry FROM journal");
    while (query->next()) {
      QString time = query->value(0).toString();
      QString entry = query->value(1).toString();
      fileOut << "\"" << time.toStdString() << "\"" << ","
	      << "\"" << doubleQuote(entry).toStdString() << "\"" << "\n";
    }
    delete query;
    // And that's it.
    fileOut.close();
  }
}
