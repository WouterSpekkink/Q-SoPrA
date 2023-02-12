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

#include "../include/IncidentsAttributesTable.h"

IncidentsAttributesTable::IncidentsAttributesTable(QWidget *parent) : QWidget(parent) 
{
  _lastSortedHeader = 1;
  _lastSortedAscending = true;
  
  // We first create our model, our table, the view and the filter of the view
  attributesModel = new QueryModel(this);
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(attributesModel);
  filter->setFilterKeyColumn(0);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                            "ch_order, incidents.description, value "
                            "FROM attributes_to_incidents "
                            "INNER JOIN incidents ON "
                            "incidents.id = attributes_to_incidents.incident "
                            "INNER JOIN incident_attributes ON "
                            "incident_attributes.name = attributes_to_incidents.attribute "
                            "ORDER BY attribute ASC");
  
  // Then we set how the data are displayed.
  attributesModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Coder"));
  attributesModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Attribute"));
  attributesModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Attribute description"));
  attributesModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Incident"));
  attributesModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Incident description"));
  attributesModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Value"));
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnWidth(0, 100);
  tableView->setColumnWidth(1, 300);
  tableView->setColumnWidth(2, 600);
  tableView->setColumnWidth(4, 600);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);

  attributesModel->sort(1, Qt::AscendingOrder);
  
  // We add the controls.
  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Coder");
  filterComboBox->addItem("Attribute");
  filterComboBox->addItem("Attribute description");
  filterComboBox->addItem("Incident");
  filterComboBox->addItem("Incident description");
  filterComboBox->addItem("Value");

  editValueButton = new QPushButton(tr("Edit value"), this);
  exportTableButton = new QPushButton(tr("Export table"), this);
  exportNormalMatrixButton = new QPushButton(tr("Export normal matrix"), this);
  exportValuedMatrixButton = new QPushButton(tr("Export valued matrix"), this);

  // Connecting the signals
  connect(filterField, SIGNAL(textChanged(const QString &)),
          this, SLOT(changeFilter(const QString &)));
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
          this, SLOT(resetHeader(int)));
  connect(tableView->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),
          this, SLOT(sortHeader(int)));
  connect(filterComboBox, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(setFilterColumn()));
  connect(editValueButton, SIGNAL(clicked()), this, SLOT(editValue()));
  connect(exportTableButton, SIGNAL(clicked()), this, SLOT(exportTable()));
  connect(exportNormalMatrixButton, SIGNAL(clicked()), this, SLOT(exportNormalMatrix()));
  connect(exportValuedMatrixButton, SIGNAL(clicked()), this, SLOT(exportValuedMatrix()));
  
  // We fetch and sort the data.
  updateTable();

  // And we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  QPointer<QHBoxLayout>filterLayout = new QHBoxLayout;
  filterLayout->addWidget(filterComboLabel);
  filterLayout->addWidget(filterComboBox);
  filterLayout->addWidget(filterFieldLabel);
  filterLayout->addWidget(filterField);
  filterLayout->addWidget(editValueButton);
  filterLayout->addWidget(exportTableButton);
  filterLayout->addWidget(exportNormalMatrixButton);
  filterLayout->addWidget(exportValuedMatrixButton);
  mainLayout->addLayout(filterLayout);

  setLayout(mainLayout);
}

void IncidentsAttributesTable::updateTable() 
{
  QString queryString = attributesModel->query().executedQuery();
  attributesModel->setQuery(queryString);
  while (attributesModel->canFetchMore())
  {
    attributesModel->fetchMore();
  }
}

void IncidentsAttributesTable::resetHeader(int header) 
{
  tableView->verticalHeader()->resizeSection(header, 30);
  updateTable();
}

void IncidentsAttributesTable::sortHeader(int header) 
{
  if (header == _lastSortedHeader)
  {
    if (_lastSortedAscending)
    {
      if (header == 0)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY coder DESC");
      }
      else if (header == 1)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY attribute DESC");
      }
      else if (header == 2)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY incident_attributes.description DESC");
      }
      else if (header == 3)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY incidents.ch_order DESC");
      }
      else if (header == 4)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY incidents.description DESC");
      }
      else if (header == 5)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY value DESC");
      }
    }
    else
    {
      if (header == 0)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY coder ASC");
      }
      else if (header == 1)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY attribute ASC");
      }
      else if (header == 2)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY incident_attributes.description ASC");
      }
      else if (header == 3)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY incidents.ch_order ASC");
      }
      else if (header == 4)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY incidents.description ASC");
      }
      else if (header == 5)
      {
        attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                  "incidents.ch_order, incidents.description, value "
                                  "FROM attributes_to_incidents "
                                  "INNER JOIN incidents ON "
                                  "incidents.id = attributes_to_incidents.incident "
                                  "INNER JOIN incident_attributes ON "
                                  "incident_attributes.name = "
                                  "attributes_to_incidents.attribute "
                                  "ORDER BY value ASC");
      }
    }
    _lastSortedAscending = !_lastSortedAscending;
  }
  else
  {
    if (header == 0)
    {
      attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                "incidents.ch_order, incidents.description, value "
                                "FROM attributes_to_incidents "
                                "INNER JOIN incidents ON "
                                "incidents.id = attributes_to_incidents.incident "
                                "INNER JOIN incident_attributes ON "
                                "incident_attributes.name = "
                                "attributes_to_incidents.attribute "
                                "ORDER BY coder DESC");
    }
    else if (header == 1)
    {
      attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                "incidents.ch_order, incidents.description, value "
                                "FROM attributes_to_incidents "
                                "INNER JOIN incidents ON "
                                "incidents.id = attributes_to_incidents.incident "
                                "INNER JOIN incident_attributes ON "
                                "incident_attributes.name = "
                                "attributes_to_incidents.attribute "
                                "ORDER BY attribute DESC");
    }
    else if (header == 2)
    {
      attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                "incidents.ch_order, incidents.description, value "
                                "FROM attributes_to_incidents "
                                "INNER JOIN incidents ON "
                                "incidents.id = attributes_to_incidents.incident "
                                "INNER JOIN incident_attributes ON "
                                "incident_attributes.name = "
                                "attributes_to_incidents.attribute "
                                "ORDER BY incident_attributes.description DESC");
    }
    else if (header == 3)
    {
      attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                "incidents.ch_order, incidents.description, value "
                                "FROM attributes_to_incidents "
                                "INNER JOIN incidents ON "
                                "incidents.id = attributes_to_incidents.incident "
                                "INNER JOIN incident_attributes ON "
                                "incident_attributes.name = "
                                "attributes_to_incidents.attribute "
                                "ORDER BY incidents.ch_order DESC");
    }
    else if (header == 4)
    {
      attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                "incidents.ch_order, incidents.description, value "
                                "FROM attributes_to_incidents "
                                "INNER JOIN incidents ON "
                                "incidents.id = attributes_to_incidents.incident "
                                "INNER JOIN incident_attributes ON "
                                "incident_attributes.name = "
                                "attributes_to_incidents.attribute "
                                "ORDER BY incidents.description DESC");
    }
    else if (header == 5)
    {
      attributesModel->setQuery("SELECT coder, attribute, incident_attributes.description, "
                                "incidents.ch_order, incidents.description, value "
                                "FROM attributes_to_incidents "
                                "INNER JOIN incidents ON "
                                "incidents.id = attributes_to_incidents.incident "
                                "INNER JOIN incident_attributes ON "
                                "incident_attributes.name = "
                                "attributes_to_incidents.attribute "
                                "ORDER BY value DESC");
    }
    _lastSortedAscending = true;
  }
  _lastSortedHeader = header;
  updateTable();
}

void IncidentsAttributesTable::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  filter->setFilterRegExp(regExp);
  updateTable();
}

void IncidentsAttributesTable::setFilterColumn() 
{
  if (filterComboBox->currentText() == "Coder") 
  {
    filter->setFilterKeyColumn(0);
  }
  else if (filterComboBox->currentText() == "Attribute") 
  {
    filter->setFilterKeyColumn(1);
  }
  else if (filterComboBox->currentText() == "Attribute description") 
  {
    filter->setFilterKeyColumn(2);
  }
  else if (filterComboBox->currentText() == "Incident") 
  {
    filter->setFilterKeyColumn(3);
  }
  else if (filterComboBox->currentText() == "Incident description") 
  {
    filter->setFilterKeyColumn(4);
  }
  else if (filterComboBox->currentText() == "Values") 
  {
    filter->setFilterKeyColumn(5);
  }
}

void IncidentsAttributesTable::editValue() 
{
  if (tableView->currentIndex().isValid()) 
  {
    int row = tableView->currentIndex().row();
    QString attribute = tableView->model()->index(row, 1).data(Qt::DisplayRole).toString();
    int order = tableView->model()->index(row, 3).data(Qt::DisplayRole).toInt();
    QString coder = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT id FROM incidents WHERE ch_order = :order");
    query->bindValue(":order", order);
    query->exec();
    query->first();
    int incident = query->value(0).toInt();
    QString value = tableView->model()->index(row,5).data(Qt::DisplayRole).toString();
    QPointer<SimpleTextDialog> simpleTextDialog = new SimpleTextDialog(this);
    simpleTextDialog->submitText(value);
    simpleTextDialog->setLabel("<b>Change value:</b>");
    simpleTextDialog->setWindowTitle("Edit value");
    simpleTextDialog->exec();
    if (simpleTextDialog->getExitStatus() == 0)
    {
      QString newValue = simpleTextDialog->getText();
      query->prepare("UPDATE attributes_to_incidents SET value = :newValue "
                     "WHERE value = :oldValue AND attribute = :attribute "
                     "AND incident = :incident AND coder = :coder");
      query->bindValue(":newValue", newValue);
      query->bindValue(":oldValue", value);
      query->bindValue(":attribute", attribute);
      query->bindValue(":incident", incident);
      query->bindValue(":coder", coder);
      query->exec();
      updateTable();
    }
    delete query;
  }
}

void IncidentsAttributesTable::exportTable()
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
    fileOut << "Coder,Attribute,Attribute Description,Incident,Incident Description,Value\n";
    // And then we can write the rest of the table.
    for (int i = 0; i != tableView->verticalHeader()->count(); i++)
    {
      QString coder = tableView->model()->index(i, 0).data(Qt::DisplayRole).toString();
      QString attribute = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
      QString attributeDescription = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
      int incident = tableView->model()->index(i, 3).data(Qt::DisplayRole).toInt();
      QString incidentDescription = tableView->model()->index(i, 4).data(Qt::DisplayRole).toString();
      QString value = tableView->model()->index(i, 5).data(Qt::DisplayRole).toString();
      fileOut << "\"" << coder.toStdString() << "\"" << ","
              << "\"" << doubleQuote(attribute).toStdString() << "\"" << ","
              << "\"" << doubleQuote(attributeDescription).toStdString() << "\"" << ","
              << incident << ","
              << "\"" << doubleQuote(incidentDescription).toStdString() << "\"" << ","
              << "\"" << doubleQuote(value).toStdString() << "\"" << "\n";
    }
  }
}
  
void IncidentsAttributesTable::exportNormalMatrix() 
{
  exportMatrix(false);
}

void IncidentsAttributesTable::exportValuedMatrix() 
{
  exportMatrix(true);
}

void IncidentsAttributesTable::exportMatrix(bool valued) 
{
  // First let's select a coder
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM coders");
  QVector<QString> coders;
  while (query->next())
  {
    coders.push_back(query->value(0).toString());
  }
  delete query;
  QPointer<ComboBoxDialog> coderDialog = new ComboBoxDialog(this, coders);
  coderDialog->setWindowTitle("Select coder to export data for");
  coderDialog->exec();
  if (coderDialog->getExitStatus() == 0)
  {
    QString coder = coderDialog->getSelection();
    updateTable();
    // We first need a list with unique names of attributes and a list with unique incidents.
    QSet<QString> attributeSet;
    QSet<int> incidentSet;
    QMultiMap<int, QVector<QString>> valueMap;
    for (int i = 0; i != tableView->verticalHeader()->count(); i++)
    {
      if (tableView->model()->index(i, 0).data(Qt::DisplayRole).toString() == coder)
	    {
	      QString attribute = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
	      int incident = tableView->model()->index(i, 3).data(Qt::DisplayRole).toInt();
	      QString value = tableView->model()->index(i, 5).data(Qt::DisplayRole).toString();
	      attributeSet.insert(attribute);
	      incidentSet.insert(incident);
	      QVector<QString> temp;
	      temp.push_back(attribute);
	      temp.push_back(value);
	      valueMap.insert(incident, temp);
	    }
    }
    QList<QString> attributeList = attributeSet.toList();
    QList<int> incidentList = incidentSet.toList();
    // Let's sort the attribute list alphabetically.
    std::sort(attributeList.begin(), attributeList.end(), stringSort);
    // And let's sort the incident list numerically.
    std::sort(incidentList.begin(), incidentList.end(), intLessThan);
    // We let the user set the file name and location.
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save table"),"", tr("csv files (*.csv)"));
    if (!fileName.trimmed().isEmpty())
    {
      if(!fileName.endsWith(".csv"))
	    {
	      fileName.append(".csv");
	    }
      // And we create a file outstream.
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first need to write the header row.
      QListIterator<QString> it(attributeList);
      while (it.hasNext())
	    {
	      QString currentAttribute = it.next();
	      fileOut << "," << "\"" << doubleQuote(currentAttribute).toStdString() << "\"";
	    }
      fileOut << "\n"; // we need a newline symbol at the end of the header.
      // Then we iterate through our lists and fetch values from the value map.
      QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, attributeList.size());
      saveProgress->setWindowTitle("Compiling matrix");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      int counter = 1;
      saveProgress->show();
      QListIterator<int> it2(incidentList);
      while (it2.hasNext())
	    {
	      int currentIncident = it2.next();
	      QListIterator<QString> it3(attributeList);
	      fileOut << currentIncident;
	      while (it3.hasNext()) 
        {
          QString currentAttribute = it3.next();
          QList<QVector<QString>> values = valueMap.values(currentIncident);
          bool found = false;
          for (int i = 0; i != values.size(); i++)
          {
            QVector<QString> currentPair = values[i];
            if (currentPair[0] == currentAttribute)
            {
              found = true;
              if (valued)
              {
                if (currentPair[1] == "")
                {
                  fileOut << "," << "1";
                }
                else
                {
                  fileOut << "," << "\"" <<
                    doubleQuote(currentPair[1]).toStdString() << "\"";
                }
              }
              else
              {
                fileOut << "," << "1";
              }
            }
          }
          if (!found)
          {
            fileOut << "," << "0";
          }
        }
	      fileOut << "\n"; // We need a newline symbol at the end of each row.
	      counter++;
	      saveProgress->setProgress(counter);
	      qApp->processEvents();
	    }
      // And that should be it!
      fileOut.close();
      delete saveProgress;
    }
  }
}
