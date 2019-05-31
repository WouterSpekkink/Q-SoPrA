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

#include "../include/RawLinkagesTable.h"

RawLinkagesTable::RawLinkagesTable(QWidget *parent) : QWidget(parent) 
{
  // We first create our model, our table, the view and the filter of the view
  linkagesModel = new RelationalTable(this);
  linkagesModel->setTable("linkages_sources");
  linkagesModel->select();
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(linkagesModel);
  filter->setFilterKeyColumn(2);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // We set the tail and head columns to show the corresponding order variables
  linkagesModel->setRelation(1, QSqlRelation("incidents", "id", "ch_order"));
  linkagesModel->setRelation(2, QSqlRelation("incidents", "id", "ch_order"));
  linkagesModel->setRelation(5, QSqlRelation("linkages_sources_reference", "code", "label"));
  // Then we set how the data are displayed.
  linkagesModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Tail"));
  linkagesModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Head"));
  linkagesModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
  linkagesModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Coder"));
  linkagesModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Source"));
  linkagesModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Source text"));
  tableView->setColumnHidden(0, true);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->horizontalHeader()->setSectionsMovable(true);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);
  
  updateTable();
  
  // We add the controls.
  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Tail");
  filterComboBox->addItem("Head");
  filterComboBox->addItem("Type");
  filterComboBox->addItem("Coder");
  filterComboBox->addItem("Source");
  filterComboBox->addItem("Source text");
  filter->setFilterKeyColumn(1);
  
  exportTableButton = new QPushButton(tr("Export visible table"), this);

  // Connecting the signals
  connect(filterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(resetHeader(int)));
  connect(tableView->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(sortHeader(int)));
  connect(filterComboBox, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setFilterColumn()));
  connect(exportTableButton, SIGNAL(clicked()), this, SLOT(exportTable()));
  
  // And we create the layout.
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

void RawLinkagesTable::updateTable() 
{
  linkagesModel->select();
  while (linkagesModel->canFetchMore()) 
    {
      linkagesModel->fetchMore();
    }
}

void RawLinkagesTable::resetHeader(int header) 
{
  tableView->verticalHeader()->resizeSection(header, 30);
  updateTable();
}

void RawLinkagesTable::sortHeader(int header) 
{
  linkagesModel->sort(header, Qt::AscendingOrder);
  updateTable();
}

void RawLinkagesTable::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  filter->setFilterRegExp(regExp);
  updateTable();
}

void RawLinkagesTable::setFilterColumn() 
{
  if (filterComboBox->currentText() == "Tail") 
    {
      filter->setFilterKeyColumn(1);
    }
  else if (filterComboBox->currentText() == "Head") 
    {
      filter->setFilterKeyColumn(2);
    }
  else if (filterComboBox->currentText() == "Type") 
    {
      filter->setFilterKeyColumn(3);
    }
  else if (filterComboBox->currentText() == "Coder") 
    {
      filter->setFilterKeyColumn(4);
    }
  else if (filterComboBox->currentText() == "Source") 
    {
      filter->setFilterKeyColumn(5);
    }
  else if (filterComboBox->currentText() == "Source text") 
    {
      filter->setFilterKeyColumn(6);
    }
}


void RawLinkagesTable::exportTable() 
{
  updateTable();
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"",
						  tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if(!fileName.endsWith(".csv")) 
	{
	  fileName.append(".csv");
	}
      // And we create a file outstream.  
      std::ofstream fileOut(fileName.toStdString().c_str());
      // We first write the header.
      fileOut << "Tail" << ","
	      << "Head" << ","
	      << "Type" << ","
	      << "Coder" << ","
	      << "Source" << ","
	      << "Source Text" << "\n";
      // Then we iterate through the visible table.
      for (int i = 0; i != tableView->verticalHeader()->count(); i++) 
	{
	  QString tail = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
	  QString head = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
	  QString type = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
	  QString coder = tableView->model()->index(i, 4).data(Qt::DisplayRole).toString();
	  QString source = tableView->model()->index(i, 5).data(Qt::DisplayRole).toString();
	  QString sourceText = tableView->model()->index(i, 6).data(Qt::DisplayRole).toString();
	  fileOut << tail.toStdString() << ","
		  << head.toStdString() << ","
		  << "\"" << doubleQuote(type).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(coder).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(source).toStdString() << "\"" << ","
		  << "\"" << doubleQuote(sourceText).toStdString() << "\"" << "\n";
	}
      // And that should be it!
      fileOut.close();
    }
}

