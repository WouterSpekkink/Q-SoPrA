#include "../include/EntitiesAttributesTable.h"

EntitiesAttributesTable::EntitiesAttributesTable(QWidget *parent) : QWidget(parent) 
{
  // We first create our model, our table, the view and the filter of the view
  attributesModel = new RelationalTable(this);
  attributesModel->setTable("attributes_to_entities");
  attributesModel->select();
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(attributesModel);
  filter->setFilterKeyColumn(1);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Then we set how the data are displayed.
  attributesModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Attribute"));
  attributesModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Entity"));
  attributesModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Value"));
  tableView->setColumnHidden(0, true);
  tableView->setColumnHidden(4, true);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnWidth(1, 300);
  tableView->setColumnWidth(2, 300);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);
  
  // We add the controls.
  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Attributes");
  filterComboBox->addItem("Entities");
  filterComboBox->addItem("Values");

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

  // We first sort by attribute
  attributesModel->sort(1, Qt::AscendingOrder);

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

void EntitiesAttributesTable::updateTable() 
{
  while (attributesModel->canFetchMore()) 
    {
      attributesModel->fetchMore();
    }
}

void EntitiesAttributesTable::resetHeader(int header) 
{
  tableView->verticalHeader()->resizeSection(header, 30);
}

void EntitiesAttributesTable::sortHeader(int header) 
{
  attributesModel->sort(header, Qt::AscendingOrder);
  updateTable();
}

void EntitiesAttributesTable::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  filter->setFilterRegExp(regExp);
}

void EntitiesAttributesTable::setFilterColumn() 
{
  if (filterComboBox->currentText() == "Attributes") 
    {
      filter->setFilterKeyColumn(1);    
    }
  else  if (filterComboBox->currentText() == "Entities") 
    {
      filter->setFilterKeyColumn(2);
    }
  else  if (filterComboBox->currentText() == "Values") 
    {
      filter->setFilterKeyColumn(3);
    }
}

void EntitiesAttributesTable::editValue() 
{
  if (tableView->currentIndex().isValid()) 
    {
      int row = tableView->currentIndex().row();
      QString attribute = tableView->model()->index(row, 1).data(Qt::DisplayRole).toString();
      QString entity = tableView->model()->index(row, 2).data(Qt::DisplayRole).toString();
      QString value = tableView->model()->index(row, 3).data(Qt::DisplayRole).toString();
      QPointer<SimpleTextDialog> simpleTextDialog = new SimpleTextDialog(this);
      simpleTextDialog->submitText(value);
      simpleTextDialog->setLabel("<b>Change value:</b>");
      simpleTextDialog->setWindowTitle("Edit value");
      simpleTextDialog->exec();
      if (simpleTextDialog->getExitStatus() == 0) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  QString newValue = simpleTextDialog->getText();
	  query->prepare("UPDATE attributes_to_entities SET value = :newValue "
			 "WHERE value = :oldValue AND attribute = :attribute AND entity = :entity");
	  query->bindValue(":newValue", newValue);
	  query->bindValue(":oldValue", value);
	  query->bindValue(":attribute", attribute);
	  query->bindValue(":entity", entity);
	  query->exec();
	  updateTable();
	  delete query;
	}
    }
}

void EntitiesAttributesTable::exportTable()
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
      fileOut << "Attribute,Incident,Value\n";
      // And then we can write the rest of the table.
      for (int i = 0; i != tableView->verticalHeader()->count(); i++)
	{
	  QString attribute = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
	  QString entity = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
	  QString value = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
	  fileOut << doubleQuote(attribute).toStdString() << ","
		  << doubleQuote(entity).toStdString() << ","
		  << value.toStdString() << "\n";
	}
    }
}

void EntitiesAttributesTable::exportNormalMatrix() 
{
  exportMatrix(false);
}

void EntitiesAttributesTable::exportValuedMatrix() 
{
  exportMatrix(true);
}

void EntitiesAttributesTable::exportMatrix(bool valued) 
{
  updateTable();
  // We first need a list with unique names of attributes and a list with unique entities.
  QSet<QString> attributeSet;
  QSet<QString> entitySet;
  QMultiMap<QString, QVector<QString>> valueMap;
  for (int i = 0; i != tableView->verticalHeader()->count(); i++) 
    {
      QString attribute = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
      QString entity = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
      QString value = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
      attributeSet.insert(attribute);
      entitySet.insert(entity);
      QVector<QString> temp;
      temp.push_back(attribute);
      temp.push_back(value);
      valueMap.insert(entity, temp);
    }
  QList<QString> attributeList = attributeSet.toList();
  QList<QString> entityList = entitySet.toList();
  // Let's sort the attribute list alphabetically.
  std::sort(attributeList.begin(), attributeList.end(), stringSort);
  // And let's sort the entity list alphabetically.
  std::sort(entityList.begin(), entityList.end(), stringSort);
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
      // We first need to write the header row.
      QListIterator<QString> it(attributeList);
      while (it.hasNext()) 
	{
	  QString currentAttribute = it.next();
	  fileOut << "," << doubleQuote(currentAttribute).toStdString();
	}
      fileOut << "\n"; // we need a newline symbol at the end of the header.
      // Then we iterate through our lists and fetch values from the value map.
      QPointer<ProgressBar> saveProgress = new ProgressBar(0, 1, attributeList.size());
      saveProgress->setWindowTitle("Compiling matrix");
      saveProgress->setAttribute(Qt::WA_DeleteOnClose);
      saveProgress->setModal(true);
      int counter = 1;
      saveProgress->show();
      QListIterator<QString> it2(entityList);
      while (it2.hasNext()) 
	{
	  QString currentEntity = it2.next();
	  QListIterator<QString> it3(attributeList);
	  fileOut << "\"" << doubleQuote(currentEntity).toStdString() << "\"";
	  while (it3.hasNext()) 
	    {
	      QString currentAttribute = it3.next();
	      QList<QVector<QString>> values = valueMap.values(currentEntity);
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
			      fileOut << "," << "\"" << doubleQuote(currentPair[1]).toStdString() << "\"";
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
