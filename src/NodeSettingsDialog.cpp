#include "../include/NodeSettingsDialog.h"

NodeSettingsDialog::NodeSettingsDialog(QWidget *parent,
				       QVector<QString> submittedEntities,
				       QVector<QString> submittedDescriptions,
				       QVector<QString> submittedModes)
  : QDialog(parent) 
{
  entities = submittedEntities;
  descriptions = submittedDescriptions;
  modes = submittedModes;
  tableWidget = new QTableWidget(this);
  tableWidget->setRowCount(entities.length());
  tableWidget->setColumnCount(4); // default size
  QTableWidgetItem *headerOne = new QTableWidgetItem("Id", 0);
  QTableWidgetItem *headerTwo = new QTableWidgetItem("Label", 1);
  QTableWidgetItem *headerThree = new QTableWidgetItem("Description", 2);
  QTableWidgetItem *headerFour = new QTableWidgetItem("Mode", 3);
  tableWidget->setHorizontalHeaderItem(0, headerOne);
  tableWidget->setHorizontalHeaderItem(1, headerTwo);
  tableWidget->setHorizontalHeaderItem(2, headerThree);
  tableWidget->setHorizontalHeaderItem(3, headerFour);
  
  // Now let's fill the table
  for (QVector<QString>::size_type i = 0; i != entities.length(); i++) 
    {
      QTableWidgetItem *newId = new QTableWidgetItem(entities[i], 0);
      QTableWidgetItem *newLabel = new QTableWidgetItem(entities[i], 0);
      QTableWidgetItem *newDescription = new QTableWidgetItem(descriptions[i], 0);
      QTableWidgetItem *newMode = new QTableWidgetItem(modes[i], 0);
      tableWidget->setItem(i, 0, newId);
      tableWidget->setItem(i, 1, newLabel);
      tableWidget->setItem(i, 2, newDescription);
      tableWidget->setItem(i, 3, newMode);
    }

  // Let's create the other objects now.
  addAttributeButton = new QPushButton(tr("Add attribute"), this);
  removeAttributeButton = new QPushButton(tr("Remove attribute"), this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  exportCloseButton = new QPushButton(tr("Export"), this);

  // Let's connect the signals now.
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  connect(removeAttributeButton, SIGNAL(clicked()), this, SLOT(removeAttribute()));
  connect(exportCloseButton, SIGNAL(clicked()), this, SLOT(exportAndClose()));
  
  // And let's now create the layout
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableWidget);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(addAttributeButton);
  buttonLayout->addWidget(removeAttributeButton);
  buttonLayout->addWidget(cancelCloseButton);
  buttonLayout->addWidget(exportCloseButton);
  mainLayout->addLayout(buttonLayout);
  
  setLayout(mainLayout);
  setWindowTitle("Settings for node list");

  QSize availableSize = qApp->desktop()->availableGeometry().size();
  int width = availableSize.width();
  int height = availableSize.height();
  this->resize(width, height);
}

void NodeSettingsDialog::cancelAndClose() 
{
  this->close();
}

void NodeSettingsDialog::exportAndClose() 
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
      // Let us first create the header
      for (int i = 0; i != tableWidget->columnCount(); i++) 
	{
	  QString currentHeader = tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString();
	  fileOut << "\"" << doubleQuote(currentHeader).toStdString() << "\"";
	  if (i != tableWidget ->columnCount() - 1) 
	    {
	      fileOut << ",";
	    }
	  else 
	    {
	      fileOut << "\n";
	    }
	}
      // Then we write the table contents.
      for (int i = 0; i != tableWidget->rowCount(); i++) 
	{
	  for (int j = 0; j != tableWidget->columnCount(); j++) 
	    {
	      QString currentCell = tableWidget->item(i, j)->data(Qt::DisplayRole).toString();
	      fileOut << "\"" << doubleQuote(currentCell).toStdString() << "\"";
	      if (j != tableWidget->columnCount() - 1) 
		{
		  fileOut << ",";
		}
	      else 
		{
		  fileOut << "\n";
		}
	    }
	}
      // And then we are finished.
      fileOut.close();
    }
  this->close();
}

void NodeSettingsDialog::addAttribute() 
{
  QPointer<AttributeSelectionDialog> attributeSelectionDialog = new AttributeSelectionDialog(this, ENTITY);
  attributeSelectionDialog->exec();
  if (attributeSelectionDialog->getExitStatus() == 0) 
    {
      bool valued = attributeSelectionDialog->getChecked();
      QString attribute = attributeSelectionDialog->getAttribute();
      QString headerText = "";
      if (valued) 
	{
	  headerText = attribute + " (values)";
	}
      else 
	{
	  headerText = attribute + " (boolean)";
	}
      QTableWidgetItem *newHeader = new QTableWidgetItem(headerText, 0);
      tableWidget->setColumnCount(tableWidget->columnCount() + 1);
      tableWidget->setHorizontalHeaderItem(tableWidget->columnCount() - 1, newHeader);
      QMap<QString, QString> valuesMap;
      QVector<QString> attributesVec;
      attributesVec.push_back(attribute);
      if (!valued)
	{
	  findChildren(attribute, &attributesVec);
	}
      QVectorIterator<QString> it(attributesVec);
      while (it.hasNext()) 
	{
	  QString currentAttribute = it.next();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT entity, value FROM attributes_to_entities "
			 "WHERE attribute = :attribute");
	  query->bindValue(":attribute", currentAttribute);
	  query->exec();
	  while (query->next()) 
	    {
	      QString currentEntity = query->value(0).toString();
	      QString currentValue = "1";
	      if (valued)
		{
		  currentValue = query->value(1).toString();
		}
	      valuesMap[currentEntity] = currentValue;
	    }
	  delete query;
	}
      qApp->setOverrideCursor(Qt::WaitCursor);
      for (int i = 0; i != tableWidget->rowCount(); i++) 
	{
	  if (valuesMap.keys().length() > 0) 
	    {
	      bool found = false;
	      for (QVector<QString>::size_type j = 0; j != valuesMap.keys().length(); j++) 
		{
		  QString currentEntity = valuesMap.keys()[j];
		  QString currentValue = valuesMap[currentEntity];
		  if (tableWidget->item(i, 0)->data(Qt::DisplayRole).toString() == currentEntity) 
		    {
		      QTableWidgetItem *newEntry = new QTableWidgetItem(currentValue, 0);
		      tableWidget->setItem(i, tableWidget->columnCount() - 1, newEntry);
		      found = true;
		      break;
		    }
		}
	      if (!found)
		{
		  QTableWidgetItem *newEntry = new QTableWidgetItem("0", 0);
		  tableWidget->setItem(i, tableWidget->columnCount() - 1, newEntry);
		}
	    }
	  else 
	    {
	      QTableWidgetItem *newEntry = new QTableWidgetItem("0", 0);
	      tableWidget->setItem(i, tableWidget->columnCount() - 1, newEntry);
	    }
	}
      qApp->restoreOverrideCursor();
      qApp->processEvents();
    }
}

void NodeSettingsDialog::removeAttribute() 
{
  // Only do this if we actually have attributes added.
  if (tableWidget->columnCount() > 4) 
    {
      QVector<QString> attributes;
      for (int i = 4; i != tableWidget->columnCount(); i++) 
	{
	  QString current = tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString();
	  attributes.push_back(current);
	}
      QPointer<ComboBoxDialog> attributeDialog = new ComboBoxDialog(this, attributes);
      attributeDialog->setWindowTitle("Select attribute to remove");
      attributeDialog->exec();
      if (attributeDialog->getExitStatus() == 0) 
	{
	  QString attribute = attributeDialog->getSelection();
	  for (int i = 4; i != tableWidget->columnCount();) 
	    {
	      QString current = tableWidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString();
	      if (current == attribute) 
		{
		  tableWidget->removeColumn(i);
		  break;
		}
	      else 
		{
		  i++;
		}
	    } 
	}
    }    
}


void NodeSettingsDialog::findChildren(QString father, QVector<QString> *children) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM entity_attributes WHERE father = :father");
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) 
    {
      QString currentChild = query->value(0).toString();
      children->push_back(currentChild);
      findChildren(currentChild, children);
    }
  delete query;
}
