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

#include "../include/DataWidget.h"

DataWidget::DataWidget(QWidget *parent) : QWidget(parent) 
{
  _currentData = "";
  _currentFind = "";
  
  // This widget uses a table model.
  incidentsModel = new EventTableModel(this);
  tableView = new ZoomableTableView(this);

  // And the model shows data from the incidents table.
  incidentsModel->setTable("incidents");
  incidentsModel->setSort(1, Qt::AscendingOrder);
  incidentsModel->select();
  incidentsModel->setEditStrategy(QSqlTableModel::OnFieldChange);
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
  tableView->setColumnWidth(7, parent->width()/4);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);
  tableView->setItemDelegateForColumn(7, new CheckBoxDelegate(tableView));
  tableView->viewport()->installEventFilter(this);
  
  // Let's set up our search field
  findSelectLabel = new QLabel("<b>Set search field:</b>", this);
  
  findComboBox = new QComboBox(this);
  findComboBox->addItem("Timing");
  findComboBox->addItem("Source");
  findComboBox->addItem("Description");
  findComboBox->addItem("Raw");
  findComboBox->addItem("Comment");

  findFieldLabel = new QLabel(tr("<b>Search:</b>"), this);
  findField = new QLineEdit(this);

  // Then we create our other controls.
  appendRecordButton = new QPushButton("Append incident", this);
  editRecordButton = new QPushButton("Edit incident", this);
  insertRecordBeforeButton = new QPushButton("Insert before", this);
  insertRecordAfterButton = new QPushButton("Insert after", this);
  moveUpButton = new QPushButton("Move up", this);
  moveDownButton = new QPushButton("Move down", this);
  duplicateRowButton = new QPushButton("Duplicate incident", this);
  removeRowButton = new QPushButton("Remove incident", this);
  findPreviousButton = new QPushButton("Find previous", this);
  findNextButton = new QPushButton("Find next", this);

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
  connect(findPreviousButton, SIGNAL(clicked()), this, SLOT(findPrevious()));
  connect(findNextButton, SIGNAL(clicked()), this, SLOT(findNext()));
  connect(findField, SIGNAL(textChanged(const QString&)), this, SLOT(setFindKey(const QString&)));
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  QPointer<QHBoxLayout> findLayout = new QHBoxLayout;
  findLayout->addWidget(findSelectLabel);
  findLayout->addWidget(findComboBox);
  findLayout->addWidget(findFieldLabel);
  findLayout->addWidget(findField);
  findLayout->addWidget(findPreviousButton);
  findLayout->addWidget(findNextButton);
  mainLayout->addLayout(findLayout);
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

void DataWidget::setData(const int index, QVector<QString> data, const QString type) 
{
  int order = index + 1;
  QString timeStamp = data[0];
  QString description = data[1];
  QString raw = data[2];
  QString comment = data[3];
  QString source = data[4];
  QSqlQuery *query = new QSqlQuery;
  if (type == NEW) 
    {
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
    }
  else 
    {
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

void DataWidget::appendRecord() 
{
  QPointer<RecordDialog> recordDialog = new RecordDialog(this);
  recordDialog->exec();
  if (recordDialog->getExitStatus() == 0) 
    {
      QVector<QString> data;
      data.push_back(recordDialog->getTimeStamp());
      data.push_back(recordDialog->getDescription());
      data.push_back(recordDialog->getRaw());
      data.push_back(recordDialog->getComment());
      data.push_back(recordDialog->getSource());
      int max = incidentsModel->rowCount();
      setData(max, data, NEW);
      delete recordDialog;
      updateTable();
    }
  else 
    {
      delete recordDialog;
    }
}

void DataWidget::editRecord() 
{
  if (tableView->currentIndex().isValid()) 
    {
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

      QPointer<RecordDialog> recordDialog = new RecordDialog(this);
      recordDialog->setTimeStamp(timeStamp);
      recordDialog->setSource(source);
      recordDialog->setDescription(description);
      recordDialog->setRaw(raw);
      recordDialog->setComment(comment);
      recordDialog->initialize();
      recordDialog->exec();
      if (recordDialog->getExitStatus() == 0) 
	{
	  QVector<QString> data;
	  data.push_back(recordDialog->getTimeStamp());
	  data.push_back(recordDialog->getDescription());
	  data.push_back(recordDialog->getRaw());
	  data.push_back(recordDialog->getComment());
	  data.push_back(recordDialog->getSource());
	  setData(currentOrder - 1, data, OLD);
	  delete recordDialog;
	}
      else 
	{
	  delete recordDialog;
	}
      QModelIndex newIndex = tableView->model()->index(currentOrder - 1, 0);
      tableView->setCurrentIndex(newIndex);
      delete query;
    }
}

void DataWidget::insertRecordBefore() 
{
  if (tableView->currentIndex().isValid())
    {
      int currentRow = tableView->selectionModel()->currentIndex().row();
      QPointer<RecordDialog> recordDialog = new RecordDialog(this);    
      recordDialog->exec();
      if (recordDialog->getExitStatus() == 0) 
	{
	  QVector<QString> data;
	  data.push_back(recordDialog->getTimeStamp());
	  data.push_back(recordDialog->getDescription());
	  data.push_back(recordDialog->getRaw());
	  data.push_back(recordDialog->getComment());
	  data.push_back(recordDialog->getSource());
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE incidents SET ch_order = ch_order + 1 WHERE ch_order > :oldOrder");
	  query->bindValue(":oldOrder", currentRow);
	  query->exec();
	  setData(currentRow, data, NEW);
	  delete query;
	  delete recordDialog;
	}
      else 
	{
	  delete recordDialog;
	}
      QModelIndex currentIndex = tableView->selectionModel()->currentIndex();
      tableView->selectRow(currentIndex.row() - 1);
    }
}

void DataWidget::insertRecordAfter() 
{
  if (tableView->currentIndex().isValid()) 
    {
      int nextRow = tableView->selectionModel()->currentIndex().row() + 1;
      QPointer<RecordDialog> recordDialog = new RecordDialog(this);
      recordDialog->exec();
      if (recordDialog->getExitStatus() == 0) 
	{
	  QVector<QString> data;
	  data.push_back(recordDialog->getTimeStamp());
	  data.push_back(recordDialog->getDescription());
	  data.push_back(recordDialog->getRaw());
	  data.push_back(recordDialog->getComment());
	  data.push_back(recordDialog->getSource());
	  QSqlDatabase::database().transaction();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE incidents SET ch_order = ch_order + 1 WHERE ch_order > :oldOrder");
	  query->bindValue(":oldOrder", nextRow);
	  query->exec();
	  QSqlDatabase::database().commit();
	  setData(nextRow, data, NEW);
	  delete query;
	  delete recordDialog;
	}
      else 
	{
	  delete recordDialog;
	}
      QModelIndex currentIndex = tableView->selectionModel()->currentIndex();
      tableView->selectRow(currentIndex.row() - 1);
    }
}

void DataWidget::moveUp() 
{
  if (tableView->currentIndex().isValid()) 
    {
      int currentOrder = tableView->currentIndex().row() + 1;
      if (currentOrder != 0) 
	{
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
	  tableView->selectRow(currentOrder - 2);
	  delete query;
	  flipEdges();
	}
    }
}

void DataWidget::moveDown() 
{
  if (tableView->currentIndex().isValid()) 
    {
      int currentOrder = tableView->currentIndex().row() + 1;
      if (currentOrder != tableView->verticalHeader()->count()) 
	{
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
	  tableView->selectRow(currentOrder);
	  delete query;
	  flipEdges();
	}
    }
}

void DataWidget::flipEdges()
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  QSqlQuery *query3 = new QSqlQuery;
  QSqlQuery *query4 = new QSqlQuery;
  query->exec("SELECT tail, head, type FROM linkages");
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :id");  
  query3->prepare("SELECT direction FROM linkage_types WHERE name = :type");
  query4->prepare("UPDATE linkages SET tail = :newTail, head = :newHead "
		  "WHERE type = :type AND tail = :oldTail AND head = :oldHead");
  while (query->next())
    {
      int tail = query->value(0).toInt();
      int head = query->value(1).toInt();
      QString type = query->value(2).toString();
      query2->bindValue(":id", tail);
      query2->exec();
      query2->first();
      int tailOrder = query2->value(0).toInt();
      query2->bindValue(":id", head);
      query2->exec();
      query2->first();
      int headOrder = query2->value(0).toInt();
      query3->bindValue(":type", type);
      query3->exec();
      query3->first();
      QString direction = query3->value(0).toString();
      if (direction == FUTURE && tailOrder > headOrder)
	{
	  query4->bindValue(":newTail", head);
	  query4->bindValue(":newHead", tail);
	  query4->bindValue(":type", type);
	  query4->bindValue(":oldTail", tail);
	  query4->bindValue(":oldHead", head);
	  query4->exec();
	}
      else if (direction == PAST && tailOrder < headOrder)
	{
	  query4->bindValue(":newTail", head);
	  query4->bindValue(":newHead", tail);
	  query4->bindValue(":type", type);
	  query4->bindValue(":oldTail", tail);
	  query4->bindValue(":oldHead", head);
	  query4->exec();
	}
    }
  QSqlDatabase::database().commit();
  delete query;
  delete query2;
  delete query3;
  delete query4;
}

void DataWidget::duplicateRow() 
{
  if (tableView->currentIndex().isValid()) 
    {
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
      QPointer<RecordDialog> recordDialog = new RecordDialog(this);
      recordDialog->setTimeStamp(timeStamp);
      recordDialog->setSource(source);
      recordDialog->setDescription(description);
      recordDialog->setRaw(raw);
      recordDialog->setComment(comment);
      recordDialog->initialize();
      recordDialog->exec();
      if (recordDialog->getExitStatus() == 0) 
	{
	  QVector<QString> data;
	  data.push_back(recordDialog->getTimeStamp());
	  data.push_back(recordDialog->getDescription());
	  data.push_back(recordDialog->getRaw());
	  data.push_back(recordDialog->getComment());
	  data.push_back(recordDialog->getSource());
	  query->prepare("UPDATE incidents SET ch_order = ch_order + 1 "
			 "WHERE ch_order > :oldOrder");
	  query->bindValue(":oldOrder", currentOrder);
	  query->exec();
	  setData(currentOrder, data, NEW);
	  delete recordDialog;
	}
      else 
	{
	  delete recordDialog;
	}
      delete query;
      QSqlDatabase::database().commit();
      updateTable();
      QModelIndex newIndex = tableView->model()->index(currentOrder, 0);
      tableView->setCurrentIndex(newIndex);
    }
}

void DataWidget::removeRow() 
{
  if (tableView->currentIndex().isValid()) 
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("Removing an incident cannot be undone. "
				     "Are you sure you want to remove this incident?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
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
	  query->prepare("DELETE FROM attributes_to_incidents_sources WHERE incident = :inc");
	  query->bindValue(":inc", id);
	  query->exec();
	  query->prepare("DELETE FROM relationships_to_incidents WHERE incident = :inc");
	  query->bindValue(":inc", id);
	  query->exec();
	  query->prepare("DELETE FROM relationships_to_incidents_sources WHERE incident = :inc");
	  query->bindValue(":inc", id);
	  query->exec();
	  query->prepare("DELETE FROM linkages WHERE tail = :tail");
	  query->bindValue(":tail", id);
	  query->exec();
	  query->prepare("DELETE FROM linkages WHERE head = :head");
	  query->bindValue(":head", id);
	  query->exec();
	  query->prepare("DELETE FROM linkage_comments WHERE tail = :tail");
	  query->bindValue(":tail", id);
	  query->exec();
	  query->prepare("DELETE FROM linkage_comments WHERE head = :head");
	  query->bindValue(":head", id);
	  query->exec();
	  query->prepare("DELETE FROM incidents_to_cases WHERE incident = :inc");
	  query->bindValue(":inc", id);
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
}

void DataWidget::findPrevious() 
{
  if (_currentFind != "") 
    {
      // If we have not selected a valid index, default to last row.
      while (incidentsModel->canFetchMore()) 
	{
	  incidentsModel->fetchMore();
	}
      int currentOrder = incidentsModel->rowCount() - 1;
      if (tableView->currentIndex().isValid()) 
	{
	  currentOrder = tableView->currentIndex().row() + 1;
	}
      QString searchText = "%" + _currentFind + "%";
      QSqlQuery *query = new QSqlQuery;
      if (findComboBox->currentText() == "Timing") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE timestamp LIKE :text "
			 "AND ch_order < :order "
			 "ORDER BY ch_order desc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Source") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE source LIKE :text "
			 "AND ch_order < :order "
			 "ORDER BY ch_order desc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Description") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE description LIKE :text "
			 "AND ch_order < :order "
			 "ORDER BY ch_order desc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Raw") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE raw LIKE :text "
			 "AND ch_order < :order "
			 "ORDER BY ch_order desc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Comment") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE comment LIKE :text "
			 "AND ch_order < :order "
			 "ORDER BY ch_order desc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      delete query;
    }
}

void DataWidget::findNext() 
{
  if (_currentFind != "") 
    {
      // If we have not selected a valid index, default to the last row.
      int currentOrder = 1;
      if (tableView->currentIndex().isValid()) 
	{
	  currentOrder = tableView->currentIndex().row() + 1;
	}
      QString searchText = "%" + _currentFind + "%";
      QSqlQuery *query = new QSqlQuery;
      if (findComboBox->currentText() == "Timing") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE timestamp LIKE :text "
			 "AND ch_order > :order "
			 "ORDER BY ch_order asc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Source") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE source LIKE :text "
			 "AND ch_order > :order "
			 "ORDER BY ch_order asc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Description") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE description LIKE :text "
			 "AND ch_order > :order "
			 "ORDER BY ch_order asc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Raw") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE raw LIKE :text "
			 "AND ch_order > :order "
			 "ORDER BY ch_order asc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      else if (findComboBox->currentText() == "Comment") 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE comment LIKE :text "
			 "AND ch_order > :order "
			 "ORDER BY ch_order asc ");
	  query->bindValue(":text", searchText);
	  query->bindValue(":order", currentOrder);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      int newOrder = query->value(0).toInt();
	      tableView->selectRow(newOrder - 1);
	    }
	}
      delete query;
    }
}

void DataWidget::setFindKey(const QString &text) 
{
  _currentFind = text;
}

void DataWidget::resetHeader(int header) 
{

  tableView->verticalHeader()->resizeSection(header, 30);
}

void DataWidget::updateTable() 
{
  while (incidentsModel->canFetchMore()) 
    {
      incidentsModel->fetchMore();
    }
}

void DataWidget::saveCurrent(const QModelIndex &index) 
{
  _currentData = tableView->model()->
    index(index.row(), index.column()).data(Qt::DisplayRole).toString();
}

void DataWidget::checkChange(const QModelIndex &topLeft, const QModelIndex &bottomRight) 
{
  if (topLeft == bottomRight) 
    {
      if (topLeft.column() == 2 ||
	  topLeft.column() == 3 ||
	  topLeft.column() == 6) 
	{
	  if (tableView->model()->index(topLeft.row(),
					topLeft.column()).data().toString().trimmed() == "") 
	    {
	      tableView->model()->setData(topLeft, _currentData);
	    }
	}
    }
}

void DataWidget::setButtons() 
{
  if (tableView->currentIndex().isValid()) 
    {
      editRecordButton->setEnabled(true);
      removeRowButton->setEnabled(true);
      duplicateRowButton->setEnabled(true);
      insertRecordBeforeButton->setEnabled(true);
      insertRecordAfterButton->setEnabled(true);
      if (tableView->currentIndex().row() == 0) 
	{
	  moveUpButton->setEnabled(false);
	}
      else 
	{
	  moveUpButton->setEnabled(true);;
	}
      if (tableView->currentIndex().row() == tableView->verticalHeader()->count() - 1) 
	{
	  moveDownButton->setEnabled(false);
	}
      else 
	{
	  moveDownButton->setEnabled(true);
	}
    }
  else 
    {
      editRecordButton->setEnabled(false);
      removeRowButton->setEnabled(false);
      duplicateRowButton->setEnabled(false);
      insertRecordBeforeButton->setEnabled(false);
      moveUpButton->setEnabled(false);
      insertRecordAfterButton->setEnabled(false);
      moveDownButton->setEnabled(false);
    }    
}

bool DataWidget::eventFilter(QObject *object, QEvent *event)
{
  if (object == tableView->viewport() && event->type() == QEvent::MouseButtonDblClick)
    {
      QPoint globalCursorPos = tableView->viewport()->mapFromGlobal(QCursor::pos());
      QModelIndex index = tableView->indexAt(globalCursorPos);
      if (index.column() == 7)
	{
	  return true;
	}
    }
  return false;
}
