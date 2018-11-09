#include "../include/EventNodeSettingsDialog.h"

EventNodeSettingsDialog::EventNodeSettingsDialog(QWidget *parent,
						 QVector<QString> submittedIds,
						 QVector<QString> submittedLabels,
						 QVector<QString> submittedTimings,
						 QVector<QString> submittedDescriptions,
						 QVector<QString> submittedComments,
						 QVector<QString> submittedTypes,
						 QVector<QString> submittedModes,
						 QVector<QString> submittedX,
						 QVector<QString> submittedY,
						 QVector<MacroEvent*> submittedMacros)
  : QDialog(parent) 
{

  ids = submittedIds;
  labels = submittedLabels;
  timings = submittedTimings;
  descriptions = submittedDescriptions;
  comments = submittedComments;
  types = submittedTypes;
  modes = submittedModes;
  xCoords = submittedX;
  yCoords = submittedY;
  macros = submittedMacros;
  tableWidget = new QTableWidget(this);
  tableWidget->setRowCount(ids.length());
  tableWidget->setColumnCount(8); // default size
  QTableWidgetItem *headerOne = new QTableWidgetItem("Id", 0);
  QTableWidgetItem *headerTwo = new QTableWidgetItem("Label", 1);
  QTableWidgetItem *headerThree = new QTableWidgetItem("Timing", 2);
  QTableWidgetItem *headerFour = new QTableWidgetItem("Description", 3);
  QTableWidgetItem *headerFive = new QTableWidgetItem("Comment", 4);
  QTableWidgetItem *headerSix = new QTableWidgetItem("Type", 5);
  QTableWidgetItem *headerSeven = new QTableWidgetItem("Mode", 6);
  QTableWidgetItem *headerEight = new QTableWidgetItem("X", 7);
  QTableWidgetItem *headerNine = new QTableWidgetItem("Y", 8);
  tableWidget->setHorizontalHeaderItem(0, headerOne);
  tableWidget->setHorizontalHeaderItem(1, headerTwo);
  tableWidget->setHorizontalHeaderItem(2, headerThree);
  tableWidget->setHorizontalHeaderItem(3, headerFour);
  tableWidget->setHorizontalHeaderItem(4, headerFive);
  tableWidget->setHorizontalHeaderItem(5, headerSix);
  tableWidget->setHorizontalHeaderItem(6, headerSeven);
  tableWidget->setHorizontalHeaderItem(7, headerEight);
  tableWidget->setHorizontalHeaderItem(8, headerNine);
  
  // Now let's fill the table
  for (QVector<QString>::size_type i = 0; i != ids.length(); i++) 
    {
      QTableWidgetItem *newId = new QTableWidgetItem(ids[i], 0);
      QTableWidgetItem *newLabel = new QTableWidgetItem(labels[i], 0);
      QTableWidgetItem *newDescription = new QTableWidgetItem(descriptions[i], 0);
      QTableWidgetItem *newTiming = new QTableWidgetItem(timings[i], 0);
      QTableWidgetItem *newComment = new QTableWidgetItem(comments[i], 0);
      QTableWidgetItem *newType = new QTableWidgetItem(types[i], 0);
      QTableWidgetItem *newMode = new QTableWidgetItem(modes[i], 0);
      QTableWidgetItem *newX = new QTableWidgetItem(xCoords[i], 0);
      QTableWidgetItem *newY = new QTableWidgetItem(yCoords[i], 0);
      tableWidget->setItem(i, 0, newId);
      tableWidget->setItem(i, 1, newLabel);
      tableWidget->setItem(i, 2, newTiming);
      tableWidget->setItem(i, 3, newDescription);
      tableWidget->setItem(i, 4, newComment);
      tableWidget->setItem(i, 5, newType);
      tableWidget->setItem(i, 6, newMode);
      tableWidget->setItem(i, 7, newX);
      tableWidget->setItem(i, 8, newY);
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

void EventNodeSettingsDialog::cancelAndClose() 
{
  this->close();
}

void EventNodeSettingsDialog::exportAndClose() 
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

void EventNodeSettingsDialog::addAttribute() 
{
  // We first ask the user to select the attribute that is to be added
  QPointer<AttributeSelectionDialog> attributeSelectionDialog = new AttributeSelectionDialog(this, INCIDENT);
  attributeSelectionDialog->exec();
  if (attributeSelectionDialog->getExitStatus() == 0) 
    {
      // We retrieve the settings from the dialog and set some variables based on the settings
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
      // We create a header for the new column
      QTableWidgetItem *newHeader = new QTableWidgetItem(headerText, 0);
      tableWidget->setColumnCount(tableWidget->columnCount() + 1);
      tableWidget->setHorizontalHeaderItem(tableWidget->columnCount() - 1, newHeader);
      // We create a vector to hold our selected attribute and its children
      // (if the valued option was selected)
      QVector<QString> attributesVec;
      attributesVec.push_back(attribute);
      if (!valued)
	{
	  findChildren(attribute, &attributesVec);
	}
      // We create a map to pair incidents with values
      QMap<QString, QString> valuesMap;
      // Let us then iterate through the attribute vector to fill the map
      QVectorIterator<QString> it(attributesVec);
      while (it.hasNext()) 
	{
	  QString currentAttribute = it.next();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT incident, value FROM attributes_to_incidents "
			 "WHERE attribute = :attribute");
	  query->bindValue(":attribute", currentAttribute);
	  query->exec();
	  while (query->next()) 
	    {
	      int id = query->value(0).toInt();
	      QSqlQuery *query2 = new QSqlQuery;
	      query2->prepare("SELECT ch_order FROM incidents WHERE id = :id");
	      query2->bindValue(":id", id);
	      query2->exec();
	      query2->first();
	      QString currentIncident = query2->value(0).toString();
	      delete query2;
	      QString currentValue = "1";
	      if (valued)
		{
		  currentValue = query->value(1).toString();
		}
	      valuesMap[currentIncident] = currentValue;
	    }
	  delete query;
	}
      // Then we can walk through the table to add the new data
      qApp->setOverrideCursor(Qt::WaitCursor);
      for (int i = 0; i != tableWidget->rowCount(); i++) 
	{
	  // If we are dealing with an incident, then we can try to retrieve the value from our map.
	  if (tableWidget->item(i, 5)->data(Qt::DisplayRole).toString() == INCIDENT) 
	    {
	      if (valuesMap.keys().length() > 0) 
		{
		  bool found = false;
		  for (QList<QString>::size_type j = 0; j != valuesMap.keys().length(); j++) 
		    {
		      QString currentIncident = valuesMap.keys()[j];
		      QString currentValue = valuesMap[currentIncident];
		      if (tableWidget->item(i, 1)->data(Qt::DisplayRole).toString() == currentIncident) 
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
	  // If we are not dealing with an incident, we have a special procedure for (macro) events
	  else 
	    {
	      QString currentName = tableWidget->item(i, 1)->data(Qt::DisplayRole).toString();
	      QVectorIterator<MacroEvent*> it2(macros);
	      while (it2.hasNext()) 
		{
		  MacroEvent *currentMacro = it2.next();
		  if (currentMacro->getLabel()->toPlainText() == currentName) 
		    {
		      QSet<QString> attributes = currentMacro->getAttributes();
		      QVectorIterator<QString> it3(attributesVec);
		      QString currentAtt = it3.next();
		      if (attributes.contains(currentAtt) && !valued) 
			{
			  QTableWidgetItem *newEntry = new QTableWidgetItem("1", 0);
			  tableWidget->setItem(i, tableWidget->columnCount() - 1, newEntry);
			  break;
			}
		      else if (attributes.contains(currentAtt) && valued) 
			{
			  QMap<QString, QString> values = currentMacro->getValues();
			  QString currentValue = values[attribute];
			  if (currentValue != "") 
			    {
			      QTableWidgetItem *newEntry = new QTableWidgetItem(currentValue, 0);
			      tableWidget->setItem(i, tableWidget->columnCount() - 1, newEntry);
			      break;
			    }
			  else 
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
		}
	    }
	}
      qApp->restoreOverrideCursor();
      qApp->processEvents(); 
    }
  delete attributeSelectionDialog;
}
  
void EventNodeSettingsDialog::removeAttribute() 
{
  // Only do this if we actually have attributes added.
  if (tableWidget->columnCount() > 9) 
    {
      QVector<QString> attributes;
      for (int i = 9; i != tableWidget->columnCount(); i++) 
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
	  for (int i = 9; i != tableWidget->columnCount();) 
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

void EventNodeSettingsDialog::findChildren(QString father, QVector<QString> *children) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM incident_attributes WHERE father = :father");
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
