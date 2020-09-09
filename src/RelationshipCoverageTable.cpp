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

#include "../include/RelationshipCoverageTable.h"

RelationshipCoverageTable::RelationshipCoverageTable(QWidget *parent) : QWidget(parent)
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT coder FROM save_data");
  query->first();
  _selectedCoder = query->value(0).toString();
  delete query;
  _lastSortedHeader = 0;
  _lastSortedAscending = true;
  
  buildModel();
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(relationshipsModel);
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
  tableView->setColumnWidth(1, 300);
  tableView->setColumnWidth(2, 800);
  tableView->setColumnWidth(3, 150);
  tableView->horizontalHeader()->setStretchLastSection(true);

  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);

  exportTableButton = new QPushButton(tr("Export table"), this);
  viewConcordancePlotButton = new QPushButton(tr("View concordance plot"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Relationship");
  filterComboBox->addItem("Type");
  filterComboBox->addItem("Description");
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
  connect(viewConcordancePlotButton, SIGNAL(clicked()), this, SLOT(viewConcordancePlot()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  QPointer<QHBoxLayout>filterLayout = new QHBoxLayout;
  filterLayout->addWidget(filterComboLabel);
  filterLayout->addWidget(filterComboBox);
  filterLayout->addWidget(filterFieldLabel);
  filterLayout->addWidget(filterField);
  filterLayout->addWidget(exportTableButton);
  filterLayout->addWidget(viewConcordancePlotButton);
  mainLayout->addLayout(filterLayout);
  
  setLayout(mainLayout);
}
			    
void RelationshipCoverageTable::buildModel()
{
  // We first create a map that will hold the contents of our table.
  QMap<QString, QVector<QString>> data;
  // Then we perform a number of queries to gather our data.
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  QSqlQuery *query3 = new QSqlQuery;
  query2->prepare("SELECT description, directedness FROM relationship_types "
		  "WHERE name = :name");
  query3->prepare("SELECT COUNT(*) FROM relationships_to_incidents "
		  "WHERE relationship = :relationship AND type = :type AND coder = :coder");
  query->exec("SELECT COUNT(*) FROM incidents");
  query->first();
  int totalIncidents = query->value(0).toInt();
  query->exec("SELECT name, type FROM entity_relationships");
  while (query->next())
    {
      QString name = query->value(0).toString();
      QString type = query->value(1).toString();
      query2->bindValue(":name", type);
      query2->exec();
      query2->first();
      QString description = query2->value(0).toString();
      QString direction = query2->value(1).toString();
      QString itemName = QString();
      QStringList relationshipParts = QStringList();
      if (direction == DIRECTED)
	{
	  relationshipParts = name.split("--->");
	  itemName = relationshipParts[0] + "-[" + type
	    + "]->" + relationshipParts[1]; 
	}
      else if (direction == UNDIRECTED)
	{
	  relationshipParts = name.split("<-->");
	  itemName = relationshipParts[0] + "<-[" + type
	    + "]->" + relationshipParts[1]; 
	}
      QSet<int> incidents;
      query3->bindValue(":relationship", name);
      query3->bindValue(":type", type);
      query3->bindValue(":coder", _selectedCoder);
      query3->exec();
      query3->first();
      int coverageAbs = query3->value(0).toInt();
      QString coverageAbsString = QString::number(coverageAbs);
      float coveragePerc = (float) coverageAbs / totalIncidents * 100.0;
      coveragePerc = std::round(coveragePerc * 100) / 100;
      QString coveragePercString = QString::number(coveragePerc);
      QVector<QString> currentData;
      currentData.push_back(description);
      currentData.push_back(type);
      currentData.push_back(coverageAbsString);
      currentData.push_back(coveragePercString);
      currentData.push_back(name);
      data.insert(itemName, currentData);      
    }
  delete query;
  delete query2;
  delete query3;
  // Now we can build our data model
  relationshipsModel = new QStandardItemModel(data.size(), 5);
  QMapIterator<QString, QVector<QString>> it(data);
  int row = 0;
  while (it.hasNext())
    {
      it.next();
      QString relationship = it.key();
      QVector<QString> currentData = it.value();
      QString currentDescription = currentData[0];
      QString currentType = currentData[1];
      int currentAbsCoverage = currentData[2].toInt();
      float currentPercCoverage = currentData[3].toFloat();
      QString shortName = currentData[4];
      QStandardItem *nameItem = new QStandardItem(relationship);
      nameItem->setData(shortName, Qt::UserRole);
      QStandardItem *descriptionItem = new QStandardItem(currentDescription);
      QStandardItem *typeItem = new QStandardItem(currentType);
      QStandardItem *coverageAbsItem = new QStandardItem();
      coverageAbsItem->setData(QVariant(currentAbsCoverage), Qt::DisplayRole);
      QStandardItem *coveragePercItem = new QStandardItem();
      coveragePercItem->setData(QVariant(currentPercCoverage), Qt::DisplayRole);
      relationshipsModel->setItem(row, 0, nameItem);
      relationshipsModel->setItem(row, 1, typeItem);
      relationshipsModel->setItem(row, 2, descriptionItem);
      relationshipsModel->setItem(row, 3, coverageAbsItem);
      relationshipsModel->setItem(row, 4, coveragePercItem);
      row++;
    }
  // Let's set our headers
  relationshipsModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Relationship"));
  relationshipsModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
  relationshipsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
  relationshipsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Coverage absolute"));
  relationshipsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Coverage percentage"));
}

void RelationshipCoverageTable::updateTable()
{
  delete relationshipsModel;
  buildModel();
  filter->setSourceModel(relationshipsModel);
}

void RelationshipCoverageTable::resetHeader(int header) 
{
  tableView->verticalHeader()->resizeSection(header, 30);
  updateTable();
}

void RelationshipCoverageTable::sortHeader(int header)
{
  if (header == _lastSortedHeader)
    {
      if (_lastSortedAscending)
	{
	  relationshipsModel->sort(header, Qt::DescendingOrder);
	}
      else
	{
	  relationshipsModel->sort(header, Qt::AscendingOrder);
	}
      _lastSortedAscending = !_lastSortedAscending;
    }
  else
    {
      relationshipsModel->sort(header, Qt::AscendingOrder);
      _lastSortedAscending = true;
    }
  _lastSortedHeader = header;
}

void RelationshipCoverageTable::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  filter->setFilterRegExp(regExp);
  updateTable();
}

void RelationshipCoverageTable::setFilterColumn() 
{
  if (filterComboBox->currentText() == "Relationship") 
    {
      filter->setFilterKeyColumn(0);
    }
  else if (filterComboBox->currentText() == "Type") 
    {
      filter->setFilterKeyColumn(1);
    }
  else if (filterComboBox->currentText() == "Description") 
    {
      filter->setFilterKeyColumn(2);
    }
  else if (filterComboBox->currentText() == "Coverage absolute") 
    {
      filter->setFilterKeyColumn(3);
    }
    else if (filterComboBox->currentText() == "Coverage percentage") 
    {
      filter->setFilterKeyColumn(4);
    }
}

void RelationshipCoverageTable::exportTable()
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
      fileOut << "Relationship,Type,Description,Coverage absolute,Coverage percentage\n";
      // And then we can write the rest of the table.
      for (int i = 0; i != tableView->verticalHeader()->count(); i++)
	{
	  QString relationship = tableView->model()->index(i, 0).data(Qt::DisplayRole).toString();
	  QString type  = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
	  QString description = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
	  QString coverageAbs = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
	  QString coveragePerc = tableView->model()->index(i, 4).data(Qt::DisplayRole).toString();
	  fileOut << "\"" << doubleQuote(relationship).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(type).toStdString() << "\"" <<  ","
		  << "\"" << doubleQuote(description).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(coverageAbs).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(coveragePerc).toStdString() << "\"" << "\n";
	}
    }
}

void RelationshipCoverageTable::viewConcordancePlot()
{
  // We will need queries for various tasks.
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents "
		  "WHERE id = :id");
  // We let the user pick a file name.
  query->exec("SELECT COUNT(*) FROM incidents");
  query->first();
  qreal totalIncidents = query->value(0).toReal();
  // We will use count for an iterator.
  int count = query->value(0).toInt();
  // We also need to set an initial y-coordinate for our first bar.
  // We will set this coordinate to other values as we move to our list of attributes.
  qreal y = 0.0;
  // We create a vector to manage the memory of our items.
  QVector<QGraphicsItem*> drawItems;
  // Now we need to fetch each visible attribute in turn.
  for (int i = 0; i != tableView->verticalHeader()->count(); i++)
    {
      QSet<int> hits;
      QString relationship = tableView->model()->index(i, 0).data(Qt::DisplayRole).toString();
      QString shortName = tableView->model()->index(i, 0).data(Qt::UserRole).toString();
      QString type = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
      query->prepare("SELECT incident FROM relationships_to_incidents "
		     "WHERE relationship = :relationship AND type = :type AND coder = :coder");
      query->bindValue(":relationship", shortName);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", type);
      query->exec();
      while (query->next())
	{
	  int incident = query->value(0).toInt();
	  query2->bindValue(":id", incident);
	  query2->exec();
	  query2->first();
	  hits.insert(query2->value(0).toInt());
	}
      // We need to set the initial x-position
      // We will increment this for every incident.
      qreal x = 0.0;
      QGraphicsTextItem *text = new QGraphicsTextItem();
      text->setPlainText(relationship);
      // We create a label for the current attribute.
      drawItems.push_back(text);
      qreal textWidth = text->boundingRect().width();
      text->setPos(0.0 - (textWidth + 20.0), y + 5.0);
      // We create a rectangle to draw around our lines.
      QGraphicsRectItem *rect = new QGraphicsRectItem();
      rect->setPen(QPen(Qt::gray, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
      rect->setRect(x, y - 1.0, totalIncidents, 42);
      drawItems.push_back(rect);
      // Now we will create the lines for our plot objects.
      for (int j = 0; j != count; j++)
	{
	  if (hits.contains(j))
	    {
	      QGraphicsLineItem *line = new QGraphicsLineItem();
	      line->setPen(QPen(Qt::black, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
	      line->setLine(x, y, x, y + 40.0);
	      drawItems.push_back(line);
	    }
	  x += 1.0;
	}
      // We need to change the y-coordinate for each variable.
      y += 42.0;
    }
  QPointer<ConcordanceDialog> dialog = new ConcordanceDialog(this, drawItems);
  dialog->setWindowTitle("Concordance plot");
  dialog->exec();
  // Now we create the svg object
  delete query;
  delete query2;
}

void RelationshipCoverageTable::setCurrentCoder(QString coder)
{
  _selectedCoder = coder;
}
