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

#include "../include/AttributeCoverageTable.h"

AttributeCoverageTable::AttributeCoverageTable(QWidget *parent) : QWidget(parent)
{
  _lastSortedHeader = 0;
  _lastSortedAscending = true;
  
  buildModel();
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(attributesModel);
  filter->setFilterKeyColumn(0);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableView->horizontalHeader()->setSectionsMovable(true);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);
  tableView->setColumnWidth(0, 400);
  tableView->setColumnWidth(1, 600);
  tableView->horizontalHeader()->setStretchLastSection(true);

  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);

  exportTableButton = new QPushButton(tr("Export table"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Attribute");
  filterComboBox->addItem("Description");
  filterComboBox->addItem("Type");
  filterComboBox->addItem("Parent");
  filterComboBox->addItem("Hierarchy level");
  filterComboBox->addItem("Coverage absolute");
  filterComboBox->addItem("Coverage percentage");
  
  connect(filterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(resetHeader(int)));
  connect(tableView->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(sortHeader(int)));
  connect(filterComboBox, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setFilterColumn()));
  connect(exportTableButton, SIGNAL(clicked()), this, SLOT(exportTable()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  QPointer<QHBoxLayout>filterLayout = new QHBoxLayout;
  filterLayout->addWidget(filterComboLabel);
  filterLayout->addWidget(filterComboBox);
  filterLayout->addWidget(filterFieldLabel);
  filterLayout->addWidget(filterField);
  filterLayout->addWidget(exportTableButton);
  mainLayout->addLayout(filterLayout);
  
  setLayout(mainLayout);
}
			    
void AttributeCoverageTable::buildModel()
{
  // We first create a map that will hold the contents of our table.
  QMap<QString, QVector<QString>> data;
  // Then we perform a number of queries to gather our data.
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  QSqlQuery *query3 = new QSqlQuery;
  query2->prepare("SELECT father FROM incident_attributes "
		  "WHERE name = :name");
  query3->prepare("SELECT incident FROM attributes_to_incidents "
		  "WHERE attribute = :attribute");
  query->exec("SELECT COUNT(*) FROM incidents");
  query->first();
  int totalIncidents = query->value(0).toInt();
  query->exec("SELECT name, description, father FROM incident_attributes");
  while (query->next())
    {
      QString name = query->value(0).toString();
      QString description = query->value(1).toString();
      QString parent = query->value(2).toString();
      int level = 1;
      if (parent == "NONE")
	{
	  parent = "No parent";
	}
      else
	{
	  QString currentParent = parent;
	  while (currentParent != "NONE")
	    {
	      query2->bindValue(":name", currentParent);
	      query2->exec();
	      query2->first();
	      currentParent = query2->value(0).toString();
	      level++;
	    }
	}
      QString levelString = QString::number(level);
      QSet<int> incidents;
      QVector<QString> attributeVector;
      attributeVector.push_back(name);
      findChildren(name, &attributeVector, false);
      QVectorIterator<QString> it(attributeVector);
      while (it.hasNext())
	{
	  QString currentAttribute = it.next();
	  query3->bindValue(":attribute", currentAttribute);
	  query3->exec();
	  while (query3->next())
	    {
	      incidents.insert(query3->value(0).toInt());
	    }
	}
      int coverageAbs = incidents.size();
      QString coverageAbsString = QString::number(coverageAbs);
      float coveragePerc = (float) coverageAbs / totalIncidents * 100.0;
      coveragePerc = std::roundf(coveragePerc * 100)  / 100;
      QString coveragePercString = QString::number(coveragePerc);
      QString type = "Attribute";
      QVector<QString> currentData;
      currentData.push_back(description);
      currentData.push_back(parent);
      currentData.push_back(levelString);
      currentData.push_back(coverageAbsString);
      currentData.push_back(coveragePercString);
      currentData.push_back(type);
      data.insert(name, currentData);      
    }
  query2->prepare("SELECT father FROM entities "
		  "WHERE name = :name");
  query->exec("SELECT name, description, father FROM entities");
  while (query->next())
    {
      QString name = query->value(0).toString();
      QString description = query->value(1).toString();
      QString parent = query->value(2).toString();
      int level = 1;
      if (parent == "NONE")
	{
	  parent = "No parent";
	}
      else
	{
	  QString currentParent = parent;
	  while (currentParent != "NONE")
	    {
	      query2->bindValue(":name", currentParent);
	      query2->exec();
	      query2->first();
	      currentParent = query2->value(0).toString();
	      level++;
	    }
	}
      QString levelString = QString::number(level);
      QVector<QString> attributeVector;
      attributeVector.push_back(name);
      QSet<int> incidents;
      findChildren(name, &attributeVector, true);
      QVectorIterator<QString> it(attributeVector);
      while (it.hasNext())
	{
	  QString currentAttribute = it.next();
	  query3->bindValue(":attribute", currentAttribute);
	  query3->exec();
	  while (query3->next())
	    {
	      incidents.insert(query3->value(0).toInt());
	    }
	}
      int coverageAbs = incidents.size();
      QString coverageAbsString = QString::number(coverageAbs);
      float coveragePerc = (float) coverageAbs / totalIncidents * 100.0;
      coveragePerc = std::roundf(coveragePerc * 100) / 100;
      QString coveragePercString = QString::number(coveragePerc);
      QString type = "Entity";
      QVector<QString> currentData;
      currentData.push_back(description);
      currentData.push_back(parent);
      currentData.push_back(levelString);
      currentData.push_back(coverageAbsString);
      currentData.push_back(coveragePercString);
      currentData.push_back(type);
      data.insert(name, currentData);      
    }
  delete query;
  delete query2;
  delete query3;
  // Now we can build our data model
  attributesModel = new QStandardItemModel(data.size(), 7);
  QMapIterator<QString, QVector<QString>> it(data);
  int row = 0;
  while (it.hasNext())
    {
      it.next();
      QString attribute = it.key();
      QVector<QString> currentData = it.value();
      QString currentDescription = currentData[0];
      QString currentParent = currentData[1];
      int currentLevel = currentData[2].toInt();
      int currentAbsCoverage = currentData[3].toInt();
      float currentPercCoverage = currentData[4].toFloat();
      QString currentType = currentData[5];
      QStandardItem *nameItem = new QStandardItem(attribute);
      QStandardItem *descriptionItem = new QStandardItem(currentDescription);
      QStandardItem *typeItem = new QStandardItem(currentType);
      QStandardItem *parentItem = new QStandardItem(currentParent);
      QStandardItem *levelItem = new QStandardItem();
      levelItem->setData(QVariant(currentLevel), Qt::DisplayRole);
      QStandardItem *coverageAbsItem = new QStandardItem();
      coverageAbsItem->setData(QVariant(currentAbsCoverage), Qt::DisplayRole);
      QStandardItem *coveragePercItem = new QStandardItem();
      coveragePercItem->setData(QVariant(currentPercCoverage), Qt::DisplayRole);
      attributesModel->setItem(row, 0, nameItem);
      attributesModel->setItem(row, 1, descriptionItem);
      attributesModel->setItem(row, 2, typeItem);
      attributesModel->setItem(row, 3, parentItem);
      attributesModel->setItem(row, 4, levelItem);
      attributesModel->setItem(row, 5, coverageAbsItem);
      attributesModel->setItem(row, 6, coveragePercItem);
      row++;
    }
  // Let's set our headers
  attributesModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Attribute"));
  attributesModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));
  attributesModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
  attributesModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Parent"));
  attributesModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Hierarchy level"));
  attributesModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Coverage absolute"));
  attributesModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Coverage percentage"));
}

void AttributeCoverageTable::updateTable()
{
  delete attributesModel;
  buildModel();
  filter->setSourceModel(attributesModel);
}

void AttributeCoverageTable::resetHeader(int header) 
{
  tableView->verticalHeader()->resizeSection(header, 30);
  updateTable();
}

void AttributeCoverageTable::sortHeader(int header)
{
  if (header == _lastSortedHeader)
    {
      if (_lastSortedAscending)
	{
	  attributesModel->sort(header, Qt::DescendingOrder);
	}
      else
	{
	  attributesModel->sort(header, Qt::AscendingOrder);
	}
      _lastSortedAscending = !_lastSortedAscending;
    }
  else
    {
      attributesModel->sort(header, Qt::AscendingOrder);
      _lastSortedAscending = true;
    }
  _lastSortedHeader = header;
}

void AttributeCoverageTable::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  filter->setFilterRegExp(regExp);
  updateTable();
}

void AttributeCoverageTable::setFilterColumn() 
{
  if (filterComboBox->currentText() == "Attribute") 
    {
      filter->setFilterKeyColumn(0);
    }
  else if (filterComboBox->currentText() == "Description") 
    {
      filter->setFilterKeyColumn(1);
    }
  else if (filterComboBox->currentText() == "Type") 
    {
      filter->setFilterKeyColumn(2);
    }
  else if (filterComboBox->currentText() == "Parent") 
    {
      filter->setFilterKeyColumn(3);
    }
  else if (filterComboBox->currentText() == "Hierarchy level") 
    {
      filter->setFilterKeyColumn(4);
    }
  else if (filterComboBox->currentText() == "Coverage absolute") 
    {
      filter->setFilterKeyColumn(5);
    }
  else if (filterComboBox->currentText() == "Coverage percentage") 
    {
      filter->setFilterKeyColumn(6);
    }
}

void AttributeCoverageTable::findChildren(QString father,
					   QVector<QString> *children,
					   bool entity) 
{
  QSqlQuery *query = new QSqlQuery;
  if (entity) 
    {
      query->prepare("SELECT name FROM entities WHERE father = :father");
    }
  else 
    {
      query->prepare("SELECT name FROM incident_attributes WHERE father = :father");
    }
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) 
    {
      QString currentChild = query->value(0).toString();
      children->push_back(currentChild);
      findChildren(currentChild, children, entity);
    }
  delete query;
}

void AttributeCoverageTable::exportTable()
{
  // We will first let the user pick a file name and location
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first need to write the header row.
      fileOut << "Attribute,Description,Parent,Type,Hierarchy Level,Coverage absolute,Coverage percentage\n";
      // And then we can write the rest of the table.
      for (int i = 0; i != tableView->verticalHeader()->count(); i++)
	{
	  QString attribute = tableView->model()->index(i, 0).data(Qt::DisplayRole).toString();
	  QString description = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
	  QString parent = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
	  QString type = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
	  QString level = tableView->model()->index(i, 4).data(Qt::DisplayRole).toString();
	  QString coverageAbs = tableView->model()->index(i, 5).data(Qt::DisplayRole).toString();
	  QString coveragePerc = tableView->model()->index(i, 6).data(Qt::DisplayRole).toString();
	  fileOut << "\"" << doubleQuote(attribute).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(description).toStdString() << "\"" <<  ","
		  << "\"" << doubleQuote(parent).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(type).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(level).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(coverageAbs).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(coveragePerc).toStdString() << "\"" << "\n";
	}
    }
}
