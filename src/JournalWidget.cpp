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

#include "../include/JournalWidget.h"

JournalWidget::JournalWidget(QWidget *parent) : QWidget(parent) 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT coder FROM save_data");
  query->first();
  _selectedCoder = query->value(0).toString();
  delete query;
  
  _lastSortedHeader = 0;
  _lastSortedAscending = true;
  
  journalLabel = new QLabel("<h2>Journal</h2>", this);
  logLabel = new QLabel("<h2>Selected entry</h2>", this);

  journalModel = new JournalQueryModel(this);
  journalModel->setQuery("SELECT time, coder, entry, mark FROM journal");

  tableView = new ZoomableTableView(this);
  tableView->setModel(journalModel);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  journalModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Log date"));
  journalModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Coder"));
  journalModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Entry"));
  journalModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Needs attention"));
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnWidth(0, (parent->width() / 2) / 4);
  tableView->setColumnWidth(1, (parent->width() / 2) / 7);
  tableView->setColumnWidth(2, (parent->width() / 2) / 3);
  tableView->setColumnWidth(3, (parent->width() / 2) / 7);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setItemDelegateForColumn(3, new CheckBoxDelegate(tableView));

  updateTable();
  
  logField = new QTextEdit(this);
  logField->setEnabled(false);
  
  addEntryButton = new QPushButton(tr("New entry"), this);
  saveChangesButton = new QPushButton(tr("Save changes"), this);
  saveChangesButton->setEnabled(false);
  removeEntryButton = new QPushButton(tr("Remove selected entry"), this);
  removeEntryButton->setEnabled(false);
  exportJournalButton = new QPushButton(tr("Export"), this);

  logField->installEventFilter(this);
  
  connect(logField, SIGNAL(textChanged()), this, SLOT(setButtons()));
  connect(addEntryButton, SIGNAL(clicked()), this, SLOT(addEntry()));
  connect(saveChangesButton, SIGNAL(clicked()), this, SLOT(saveChanges()));
  connect(removeEntryButton, SIGNAL(clicked()), this, SLOT(removeEntry()));
  connect(tableView->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(setData(const QItemSelection &, const QItemSelection &)));
  connect(journalModel, SIGNAL(armSelection()), this, SLOT(armIndex()));
  connect(journalModel, SIGNAL(setSelection()), this, SLOT(setIndex()));
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

void JournalWidget::updateTable()
{
  journalModel->setQuery("SELECT time, coder, entry, mark FROM journal");
  while (journalModel->canFetchMore())
  {
    journalModel->fetchMore();
  }
}

void JournalWidget::setButtons() 
{
  if (tableView->currentIndex().isValid()) 
  {
    saveChangesButton->setEnabled(true);
  }
}

void JournalWidget::resetButtons() 
{
  saveChangesButton->setEnabled(false);
  removeEntryButton->setEnabled(false);
  logField->setEnabled(false);
}

void JournalWidget::saveChanges() 
{
  if (tableView->currentIndex().isValid()) 
  {
    int currentRow = tableView->currentIndex().row();
    QString time = journalModel->data(journalModel->index(currentRow, 0),
                                      Qt::DisplayRole).toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("UPDATE journal SET entry = :entry WHERE time = :time");
    query->bindValue(":entry", logField->toPlainText());
    query->bindValue(":time", time);
    query->exec();
    saveChangesButton->setEnabled(false);
    updateTable();
  }
}

void JournalWidget::setData(const QItemSelection &selected, const QItemSelection &deselected) 
{
  Q_UNUSED(selected);
  if (checkChanges())
  {
    if (tableView->currentIndex().isValid())
    {
      int currentRow = tableView->currentIndex().row();
      QModelIndex wantedIndex = tableView->currentIndex().sibling(currentRow, 2);
      QString currentText = wantedIndex.data(Qt::DisplayRole).toString();
      logField->blockSignals(true);
      logField->setText(currentText);
      logField->blockSignals(false);
      logField->setEnabled(true);
      saveChangesButton->setEnabled(false);
      removeEntryButton->setEnabled(true);
    }
    else
    {
      removeEntryButton->setEnabled(false);
      saveChangesButton->setEnabled(false);
      logField->setEnabled(false);
    }
  }
  else
  {
    int currentRow = 0;
    if (deselected.empty())
    {
      currentRow = selected.indexes().first().row();
    }
    else
    {
      currentRow = deselected.indexes().first().row();
    }
    tableView->selectionModel()->blockSignals(true);
    tableView->selectRow(currentRow);
    tableView->selectionModel()->blockSignals(false);
  }
}

void JournalWidget::addEntry() 
{
  QDateTime time = QDateTime::currentDateTime();
  QString timeText = time.toString(Qt::TextDate);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("INSERT INTO journal (time, mark, coder) "
                 "VALUES (:time, :mark, :coder)");
  query->bindValue(":time", timeText);
  query->bindValue(":mark", 0);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  delete query;
  updateTable();
  QModelIndex newIndex = tableView->model()->index(journalModel->rowCount() - 1, 0);
  tableView->setCurrentIndex(newIndex);
}

void JournalWidget::removeEntry() 
{
  if (tableView->currentIndex().isValid()) 
  {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->setWindowTitle("Removing entry");
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing a journal entry cannot be undone. "
                                   "Are you sure you want to remove this entry?");
    if (warningBox->exec() == QMessageBox::Yes)
    {
      int currentRow = tableView->currentIndex().row();
      QString time = journalModel->data(journalModel->index(currentRow, 0),
                                        Qt::DisplayRole).toString();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("DELETE FROM journal WHERE time = :time");
      query->bindValue(":time", time);
      query->exec();
      delete query;
      logField->blockSignals(true);
      logField->setText("");
      logField->blockSignals(false);
      logField->setEnabled(false);
      updateTable();
    }
  }
}

void JournalWidget::resetHeader(int header) 
{
  tableView->verticalHeader()->resizeSection(header, 30);
}

bool JournalWidget::eventFilter(QObject *object, QEvent *event) 
{
  if (event->type() == QEvent::Wheel) 
  {
    QWheelEvent *wheelEvent = (QWheelEvent*) event;
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
    if (textEdit)
    {
      if(wheelEvent->modifiers() & Qt::ControlModifier)
      {
        if (wheelEvent->angleDelta().y() > 0)
        {
          textEdit->zoomIn(1);
        }
        else if (wheelEvent->angleDelta().y() < 0)
        {
          textEdit->zoomOut(1);
        }
      }
    }
  }
  return false;
}

void JournalWidget::exportJournal() 
{
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
  {
    if(!fileName.endsWith(".csv"))
    {
      fileName.append(".csv");
    }
    // And we create a file outstream.
    std::ofstream fileOut(fileName.toStdString().c_str());
    // We first write the header of the file.
    fileOut << "Time" << ","
            << "Entry" << "\n";
    // And then we fetch the journal entries.
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT time, coder, entry FROM journal");
    while (query->next())
    {
      QString time = query->value(0).toString();
      QString coder = query->value(1).toString();
      QString entry = query->value(2).toString();
      fileOut << "\"" << time.toStdString() << "\"" << ","
              << "\"" << coder.toStdString() << "\"" << ","
              << "\"" << doubleQuote(entry).toStdString() << "\"" << "\n";
    }
    delete query;
    // And that's it.
    fileOut.close();
  }
}

bool JournalWidget::checkChanges()
{
  if (saveChangesButton->isEnabled())
  {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->setWindowTitle("Changing widget");
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Unsaved changes</h2>");
    warningBox->setInformativeText("You have unsaved changes to journal entries, "
                                   "are you want to continue?");
    if (warningBox->exec() == QMessageBox::Yes)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return true;
  }
}

void JournalWidget::setCurrentCoder(QString coder)
{
  _selectedCoder = coder;
}

void JournalWidget::armIndex()
{
  _tempIndex = tableView->currentIndex();
}

void JournalWidget::setIndex()
{
  tableView->setCurrentIndex(_tempIndex);
}
