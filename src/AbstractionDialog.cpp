#include "../include/AbstractionDialog.h"

AbstractionDialog::AbstractionDialog(QWidget *parent,
				     QVector<EventItem*> submittedEvents,
				     QVector<MacroEvent*> submittedMacros,
				     QVector<QGraphicsItem*> submittedData,
				     QVector<QString> submittedTypes,
				     QString submittedCoder) : QDialog(parent)
{
  exitStatus = 1;
  eventVector = submittedEvents;
  macroVector = submittedMacros;
  currentData = submittedData;
  presentTypes = submittedTypes;
  selectedAttribute = DEFAULT2;
  selectedCoder = submittedCoder;
  attributeIsEntity = false;
  chosenConstraint = "";
  eventDescription = "";

  constraintsLabel = new QLabel(tr("<b>Available constraints:</b>"), this);
  attributeOptionsLabel = new QLabel(tr("<b>Group on attribute:</b>"), this);
  attributeLabel = new QLabel(tr("<b>Selected attribute:</b>"), this);
  chosenAttributeLabel = new QLabel(DEFAULT2, this);
  chosenAttributeLabel->setStyleSheet("color: red");
  descriptionLabel = new QLabel(tr("<b>Event description:</b>"), this);

  pathsBasedCheckBox = new QCheckBox("Paths-based constraints", this);
  pathsBasedCheckBox->setEnabled(false);
  semiPathsBasedCheckBox = new QCheckBox("Semipaths-based constraints", this);
  semiPathsBasedCheckBox->setEnabled(false);
  noConstraintsCheckBox = new QCheckBox("No constraints", this);
  
  eventDescriptionField = new QTextEdit(this);

  setAttributeButton = new QPushButton(tr("Set attribute"), this);
  clearAttributeButton = new QPushButton(tr("Clear attribute"), this);
  clearAttributeButton->setEnabled(false);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Create abstract event"), this);
  if (currentData.size() == 1)
    {
      setAttributeButton->setEnabled(false);
    }
  connect(pathsBasedCheckBox, SIGNAL(clicked()), this, SLOT(processPathsCheck()));
  connect(semiPathsBasedCheckBox, SIGNAL(clicked()), this, SLOT(processSemipathsCheck()));
  connect(noConstraintsCheckBox, SIGNAL(clicked()), this, SLOT(processNoPathsCheck()));
  connect(setAttributeButton, SIGNAL(clicked()), this, SLOT(selectAttribute()));
  connect(clearAttributeButton, SIGNAL(clicked()), this, SLOT( clearAttribute()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> dualLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QVBoxLayout> constraintsLayout = new QVBoxLayout;
  constraintsLayout->addWidget(constraintsLabel);
  constraintsLayout->addWidget(pathsBasedCheckBox);
  constraintsLayout->addWidget(semiPathsBasedCheckBox);
  constraintsLayout->addWidget(noConstraintsCheckBox);
  leftLayout->addLayout(constraintsLayout);

  QPointer<QFrame> sepLine = new QFrame;
  sepLine->setFrameShape(QFrame::HLine);
  leftLayout->addWidget(sepLine);

  leftLayout->addWidget(attributeOptionsLabel);
  QPointer<QHBoxLayout> attributeLayoutTop = new QHBoxLayout;
  attributeLayoutTop->addWidget(setAttributeButton);
  attributeLayoutTop->addWidget(clearAttributeButton);
  leftLayout->addLayout(attributeLayoutTop);
  QPointer<QHBoxLayout> attributeLayoutBottom = new QHBoxLayout;
  attributeLayoutBottom->addWidget(attributeLabel);
  attributeLayoutBottom->addWidget(chosenAttributeLabel);
  chosenAttributeLabel->setFixedWidth(200);
  leftLayout->addLayout(attributeLayoutBottom);
  dualLayout->addLayout(leftLayout);

  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  rightLayout->addWidget(descriptionLabel);
  rightLayout->addWidget(eventDescriptionField);
  dualLayout->addLayout(rightLayout);
  mainLayout->addLayout(dualLayout);
  
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(cancelCloseButton);
  buttonLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle("Create abstract event");
  
  prepareEvents();
}

void AbstractionDialog::selectAttribute()
{
  QPointer<SimpleAttributeSelectionDialog> attributeSelection =
    new SimpleAttributeSelectionDialog(this, INCIDENT);
  attributeSelection->exec();
  if (attributeSelection->getExitStatus() == 0) 
    {
      selectedAttribute = attributeSelection->getAttribute();
      chosenAttributeLabel->setText(selectedAttribute);
      attributeIsEntity = attributeSelection->isEntity();
      QVectorIterator<QGraphicsItem*> it(currentData);
      prepareEvents();
      clearAttributeButton->setEnabled(true);
      return;
    }
  else 
    {
      selectedAttribute = DEFAULT2;
      chosenAttributeLabel->setText(DEFAULT2);
      attributeIsEntity = false;
      prepareEvents();
      clearAttributeButton->setEnabled(false);
      return;
    }
}

void AbstractionDialog::clearAttribute()
{
  selectedAttribute = DEFAULT2;
  chosenAttributeLabel->setText(DEFAULT2);
  clearAttributeButton->setEnabled(false);
}

void AbstractionDialog::prepareEvents() 
{
  if (currentData.size() > 0) 
    {
      collectedIncidents.clear();
      QVector<EventItem*> allIncidents;
      semiPathsAllowed = true;      
      pathsAllowed = true;
      QVectorIterator<QGraphicsItem*> it(currentData);
      while (it.hasNext()) 
	{
	  EventItem *event = qgraphicsitem_cast<EventItem*>(it.peekNext());
	  MacroEvent *macro = qgraphicsitem_cast<MacroEvent*>(it.peekNext());
	  if (event) 
	    {
	      EventItem *currentEvent = qgraphicsitem_cast<EventItem*>(it.next());
	      allIncidents.push_back(currentEvent);
	      int id = currentEvent->getId();
	      if (selectedAttribute != DEFAULT2) 
		{
		  QVector<QString> attributes;
		  attributes.push_back(selectedAttribute);
		  findChildren(selectedAttribute, &attributes, attributeIsEntity);
		  bool hasAttribute = false;
		  QVectorIterator<QString> it2(attributes);
		  while (it2.hasNext()) 
		    {
		      QString currentAttribute = it2.next();
		      QSqlQuery *query = new QSqlQuery;
		      query->prepare("SELECT incident FROM attributes_to_incidents "
				     "WHERE attribute = :attribute AND incident = :id");
		      query->bindValue(":attribute", currentAttribute);
		      query->bindValue(":id", id);
		      query->exec();
		      query->first();
		      if (!query->isNull(0)) 
			{
			  hasAttribute = true;
			  break;
			}
		      delete query;
		    }
		  if (hasAttribute) 
		    {
		      collectedIncidents.push_back(currentEvent);
		    }
		}
	      else 
		{
		  collectedIncidents.push_back(currentEvent);
		}
	    }
	  if (macro) 
	    {
	      MacroEvent *currentMacro = qgraphicsitem_cast<MacroEvent*>(it.next());
	      if (selectedAttribute != DEFAULT2)
		{
		  QSet<QString> macroAttributes = currentMacro->getAttributes();
		  QVector<QString> attributes;
		  attributes.push_back(selectedAttribute);
		  findChildren(selectedAttribute, &attributes, attributeIsEntity);
		  bool hasAttribute = false;
		  QVectorIterator<QString> it2(attributes);
		  while (it2.hasNext())
		    {
		      QString currentAttribute = it2.next();
		      if (macroAttributes.contains(currentAttribute))
			{
			  hasAttribute = true;
			  break;
			}
		    }
		  QVectorIterator<EventItem*> it3(currentMacro->getIncidents());
		  while (it3.hasNext())
		    {
		      EventItem* currentEvent = it3.next();
		      if (hasAttribute) 
			{
			  allIncidents.push_back(currentEvent);
			  collectedIncidents.push_back(currentEvent);			  
			}
		      else
			{
			  allIncidents.push_back(currentEvent);
			}
		    }
		}
	      else 
		{
		  QVectorIterator<EventItem*> it3(currentMacro->getIncidents());
		  while (it3.hasNext())
		    {
		      EventItem *currentEvent = it3.next();
		      allIncidents.push_back(currentEvent);
		      collectedIncidents.push_back(currentEvent);			  
		    }
		}
	    }
	}
      if (collectedIncidents.size() > 0)
	{
	  std::sort(collectedIncidents.begin(), collectedIncidents.end(), eventLessThan);
	  std::sort(allIncidents.begin(), allIncidents.end(), eventLessThan);
	  QApplication::setOverrideCursor(Qt::WaitCursor);
	  QVector<QGraphicsItem*> allEvents;
	  QVectorIterator<EventItem*> evIt(eventVector);
	  while (evIt.hasNext()) 
	    {
	      allEvents.push_back(evIt.next());
	    }
	  QVectorIterator<MacroEvent*> maIt(macroVector);
	  while (maIt.hasNext()) 
	    {
	      allEvents.push_back(maIt.next());
	    }
	  std::sort(allEvents.begin(), allEvents.end(), componentsSort);
	  EventItem *first = allIncidents.first();
	  EventItem *last = allIncidents.last();
	  QSet<int> markOne;
	  QSet<int> markTwo;
	  QVectorIterator<QString> lit(presentTypes);
	  while (lit.hasNext()) 
	    {
	      QString currentLinkage = lit.next();
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT direction FROM linkage_types WHERE name = :name");
	      query->bindValue(":name", currentLinkage);
	      query->exec();
	      query->first();
	      QString direction = query->value(0).toString();
	      delete query;
	      if (direction == PAST) 
		{
		  findTailsUpperBound(&markOne, first->getId(), last->getOrder(), currentLinkage);
		  findHeadsLowerBound(&markTwo, last->getId(), first->getOrder(), currentLinkage);
		}
	      else if (direction == FUTURE) 
		{
		  findTailsLowerBound(&markOne, first->getId(), first->getOrder(), currentLinkage);
		  findHeadsUpperBound(&markTwo, last->getId(), last->getOrder(), currentLinkage);
		}
	      QVectorIterator<QGraphicsItem*> it4(allEvents);
	      while (it4.hasNext()) 
		{
		  QGraphicsItem *temp = it4.next();
		  EventItem *eventTemp = qgraphicsitem_cast<EventItem*>(temp);
		  MacroEvent *macroTemp = qgraphicsitem_cast<MacroEvent*>(temp);
		  if (eventTemp)
		    {
		      if (markOne.contains(eventTemp->getId()) &&
			  markTwo.contains(eventTemp->getId()) &&
			  !allIncidents.contains(eventTemp)) 
			{
			  semiPathsAllowed = false;
			}
		    }
		  else if (macroTemp) 
		    {
		      QVector<EventItem*> macroIncidents = macroTemp->getIncidents();
		      QVectorIterator<EventItem*> it5(macroIncidents);
		      while (it5.hasNext()) 
			{
			  eventTemp = it5.next();
			  if (markOne.contains(eventTemp->getId()) &&
			      markTwo.contains(eventTemp->getId()) &&
			      !allIncidents.contains(eventTemp)) 
			    {
			      semiPathsAllowed = false;
			    }
			}
		    }
		}
	    }
	  QApplication::restoreOverrideCursor();
	  qApp->processEvents();
	  std::sort(allIncidents.begin(), allIncidents.end(), componentsSort);
	  checkConstraints(allIncidents);
	}
      else 
	{
	  QPointer <QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->addButton(QMessageBox::Ok);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<b>No incidents selected</b>");
	  warningBox->setInformativeText("None of the selected incidents have the selected "
					 "attribute.");
	  warningBox->exec();
	  delete warningBox;
	  selectedAttribute = DEFAULT2;
	  chosenAttributeLabel->setText(DEFAULT2);
	  prepareEvents();
	}
    }
  else 
    {
      semiPathsAllowed = false;
      pathsAllowed = false;
    }
  evaluateConstraints();
}

void AbstractionDialog::checkConstraints(QVector<EventItem*> incidents) 
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  QVector<int> incidentId;
  QVectorIterator<EventItem*> it(incidents);
  while (it.hasNext()) 
    {
      EventItem *event = qgraphicsitem_cast<EventItem*>(it.next());
      if (event) 
	{
	  incidentId.push_back(event->getId());
	}
    }
  QVector<bool> linkagePresence = checkLinkagePresence(incidentId);
  for (QVector<QString>::size_type lit = 0; lit != presentTypes.size(); lit++) 
    {
      QString currentType = presentTypes[lit];
      if (linkagePresence[lit] == true) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	  query->bindValue(":type", currentType);
	  query->exec();
	  query->first();
	  QString direction = query->value(0).toString();
	  QVectorIterator<EventItem*> dit(incidents);
	  while (dit.hasNext()) 
	    {
	      QSet<int> markPaths;
	      QSet<int> markSemiPaths;
	      EventItem *departure = dit.next();
	      // First we check the internal consistency;
	      if (direction == PAST) 
		{
		  findHeadsLowerBound(&markPaths, departure->getId(), incidents.first()->getOrder(),
				      currentType);
		}
	      else if (direction == FUTURE) 
		{
		  findHeadsUpperBound(&markPaths, departure->getId(), incidents.last()->getOrder(),
				      currentType);
		}
	      QSet<int> items;
	      items.insert(departure->getId());
	      int lowerLimit = incidents.first()->getOrder();
	      int upperLimit = incidents.last()->getOrder();
	      findUndirectedPaths(&markSemiPaths, &items, lowerLimit, upperLimit, currentType);
	      QVectorIterator<EventItem*> cit(incidents);
	      while (cit.hasNext()) 
		{
		  EventItem *current = cit.next();
		  bool pathsFound = false;
		  bool semiPathsFound = false;
		  if (current != departure) 
		    {
		      if (markPaths.contains(current->getId())) 
			{
			  pathsFound = true;
			}
		      if (markSemiPaths.contains(current->getId()))
			{
			  semiPathsFound = true;
			}
		      if (direction == PAST) 
			{
			  if (!pathsFound && current->getOrder() < departure->getOrder()) 
			    {
			      pathsAllowed = false;
			    }
			}
		      else if (direction == FUTURE) 
			{
			  if (!pathsFound && current->getOrder() > departure->getOrder()) 
			    {
			      pathsAllowed = false;
			    }
			}
		      if (!semiPathsFound) 
			{
			  semiPathsAllowed = false;
			}
		    }
		}
	      // Then we check the external consistency.
	      query->prepare("SELECT tail FROM linkages "
			     "WHERE head = :head AND type = :type AND coder = :coder");
	      query->bindValue(":head", departure->getId());
	      query->bindValue(":type", currentType);
	      query->bindValue(":coder", selectedCoder);
	      query->exec();
	      while (query->next()) 
		{
		  QSet<int> markPathsTwo;
		  int currentTail = query->value(0).toInt();
		  if (!(incidentId.contains(currentTail))) 
		    {
		      if (direction == PAST) 
			{
			  findHeadsLowerBound(&markPathsTwo, currentTail,
					      incidents.first()->getOrder(), currentType);
			}
		      else if (direction ==  FUTURE) 
			{
			  findHeadsUpperBound(&markPathsTwo, currentTail,
					      incidents.last()->getOrder(), currentType);
			}
		      QVectorIterator<EventItem*> kit(incidents);
		      while (kit.hasNext()) 
			{
			  EventItem *current = kit.next();
			  bool pathsFoundTwo = false;
			  if (markPathsTwo.contains(current->getId())) 
			    {
			      pathsFoundTwo = true;
			    }
			  if (!pathsFoundTwo) 
			    {
			      pathsAllowed = false;
			    }
			}
		    }
		}
	      query->prepare("SELECT head FROM linkages "
			     "WHERE tail = :tail AND type = :type AND coder = :coder");
	      query->bindValue(":tail", departure->getId());
	      query->bindValue(":type", currentType);
	      query->bindValue(":coder", selectedCoder);
	      query->exec();
	      while (query->next()) 
		{
		  int currentHead = query->value(0).toInt();
		  if (!(incidentId.contains(currentHead))) 
		    {
		      QVectorIterator<EventItem*> lit(incidents);
		      while (lit.hasNext()) 
			{
			  EventItem *current = lit.next();
			  if (current != departure) 
			    {
			      QSet<int> markPathsThree;
			      QSqlQuery *query2 = new QSqlQuery;
			      query2->prepare("SELECT ch_order FROM incidents "
					      "WHERE id = :currentHead");
			      query2->bindValue(":currentHead", currentHead);
			      query2->exec();
			      query2->first();
			      int headOrder = query2->value(0).toInt();
			      if (direction == PAST) 
				{
				  findHeadsLowerBound(&markPathsThree, current->getId(),
						      headOrder, currentType);
				}
			      else if (direction == FUTURE) 
				{
				  findHeadsUpperBound(&markPathsThree, current->getId(),
						      headOrder, currentType);
				}
			      bool pathsFoundThree = false;
			      if (markPathsThree.contains(currentHead)) 
				{
				  pathsFoundThree = true;
				}
			      if (!pathsFoundThree) 
				{
				  pathsAllowed = false;
				}
			    }
			}
		    }
		}
	    }
	  delete query;
	}
    }
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void AbstractionDialog::evaluateConstraints()
{
  if (pathsAllowed)
    {
      pathsBasedCheckBox->setEnabled(true);
    }
  else
    {
      pathsBasedCheckBox->setEnabled(false);
      if (pathsBasedCheckBox->checkState() == Qt::Checked)
	{
	  pathsBasedCheckBox->setCheckState(Qt::Unchecked);
	}
    }
  if (semiPathsAllowed)
    {
      semiPathsBasedCheckBox->setEnabled(true);
    }
  else
    {
      semiPathsBasedCheckBox->setEnabled(false);
      if (semiPathsBasedCheckBox->checkState() == Qt::Checked)
	{
	  semiPathsBasedCheckBox->setCheckState(Qt::Unchecked);
	}
    }
}

void AbstractionDialog::processPathsCheck()
{
  semiPathsBasedCheckBox->setCheckState(Qt::Unchecked);
  noConstraintsCheckBox->setChecked(Qt::Unchecked);
}

void AbstractionDialog::processSemipathsCheck()
{
  pathsBasedCheckBox->setCheckState(Qt::Unchecked);
  noConstraintsCheckBox->setCheckState(Qt::Unchecked);
}


void AbstractionDialog::processNoPathsCheck()
{
  pathsBasedCheckBox->setCheckState(Qt::Unchecked);
  semiPathsBasedCheckBox->setCheckState(Qt::Unchecked);
}

void AbstractionDialog::findChildren(QString father,
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


void AbstractionDialog::findHeadsLowerBound(QSet<int> *pMark, int currentIncident,
					    int lowerLimit, QString type) 
{
  int currentTail = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM linkages "
		 "WHERE tail = :tail AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :head");
  query->bindValue(":tail", currentTail);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentHead = query->value(0).toInt();
      query2->bindValue(":head", currentHead);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order >= lowerLimit && !pMark->contains(currentHead)) 
	{
	  results.push_back(currentHead);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findHeadsLowerBound(pMark, *it, lowerLimit, type);
    }
}

void AbstractionDialog::findHeadsUpperBound(QSet<int> *pMark, int currentIncident,
					    int upperLimit, QString type) 
{
  int currentTail = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM linkages "
		 "WHERE tail = :tail AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :head");
  query->bindValue(":tail", currentTail);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentHead = query->value(0).toInt();
      query2->bindValue(":head", currentHead);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order <= upperLimit && !pMark->contains(currentHead)) 
	{
	  results.push_back(currentHead);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findHeadsUpperBound(pMark, *it, upperLimit, type);
    }
}

void AbstractionDialog::findUndirectedPaths(QSet<int> *pMark, QSet<int> *submittedItems,
					    int lowerLimit, int upperLimit, QString type) 
{
  QSet<int> temp = *submittedItems;
  QSetIterator<int> it(temp);
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM linkages "
		 "WHERE tail = :tail AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :head");
  QSqlQuery *query3 = new QSqlQuery;
  query3->prepare("SELECT tail FROM linkages "
		  "WHERE head = :head AND type = :type AND coder = :coder");
  QSqlQuery *query4 = new QSqlQuery;
  query4->prepare("SELECT ch_order FROM incidents WHERE id = :tail");
  while (it.hasNext()) 
    {
      int current = it.next();
      pMark->insert(current);  
      query->bindValue(":tail", current);
      query->bindValue(":type", type);
      query->bindValue(":coder", selectedCoder);
      query->exec();
      while (query->next()) 
	{
	  int currentHead = query->value(0).toInt();
	  query2->bindValue(":head", currentHead);
	  query2->exec();
	  query2->first();
	  int order = query2->value(0).toInt();
	  if (order >= lowerLimit && order <= upperLimit) 
	    {
	      submittedItems->insert(currentHead);
	    }
	}
      query3->bindValue(":head", current);
      query3->bindValue(":type", type);
      query3->bindValue(":coder", selectedCoder);
      query3->exec();
      while (query3->next()) 
	{
	  int currentTail = query3->value(0).toInt();
	  query4->bindValue(":tail", currentTail);
	  query4->exec();
	  query4->first();
	  int order = query4->value(0).toInt();
	  if (order >= lowerLimit && order <= upperLimit && !pMark->contains(currentTail)) 
	    {
	      submittedItems->insert(currentTail);
	    }
	}
    }
  delete query;
  delete query2;
  delete query3;
  delete query4;
  QSqlDatabase::database().commit();
  if (submittedItems->size() > temp.size()) 
    {
      findUndirectedPaths(pMark, submittedItems, lowerLimit, upperLimit, type);
    }
  else 
    {
      return;
    }
}

void AbstractionDialog::findTailsUpperBound(QSet<int> *pMark, int currentIncident,
					    int upperLimit, QString type) 
{
  int currentHead = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM linkages "
		 "WHERE head = :head AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :tail");  
  query->bindValue(":head", currentHead);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentTail = query->value(0).toInt();
      query2->bindValue(":tail", currentTail);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order <=  upperLimit && !pMark->contains(currentTail)) 
	{
	  results.push_back(currentTail);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findTailsUpperBound(pMark, *it, upperLimit, type);
    }
}

void AbstractionDialog::findTailsLowerBound(QSet<int> *pMark, int currentIncident,
					    int lowerLimit, QString type) 
{
  int currentHead = currentIncident;
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM linkages "
		 "WHERE head = :head AND type = :type AND coder = :coder");
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :tail");  
  query->bindValue(":head", currentHead);
  query->bindValue(":type", type);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  std::vector<int> results;
  while (query->next()) 
    {
      int currentTail = query->value(0).toInt();
      query2->bindValue(":tail", currentTail);
      query2->exec();
      query2->first();
      int order = query2->value(0).toInt();
      if (order >=  lowerLimit && !pMark->contains(currentTail)) 
	{
	  results.push_back(currentTail);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) 
    {
      pMark->insert(*it);
      findTailsLowerBound(pMark, *it, lowerLimit, type);
    }
}

QVector<bool> AbstractionDialog::checkLinkagePresence(QVector<int> incidentIds) 
{
  QVector<bool> result;
  QVectorIterator<QString> it(presentTypes);
  while (it.hasNext()) 
    {
      QString currentType = it.next();
      QVector<int> observed;
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail, head FROM linkages "
		     "WHERE type = :type");
      query->bindValue(":type", currentType);
      query->exec();
      while (query->next()) 
	{
	  observed.push_back(query->value(0).toInt());
	  observed.push_back(query->value(1).toInt());
	}
      delete query;
      QVectorIterator<int> it2(observed);
      bool status = false;
      while (it2.hasNext()) 
	{
	  int currentObserved = it2.next();
	  if (incidentIds.contains(currentObserved)) 
	    {
	      status = true;
	    }
	}
      result.push_back(status);
    }
  return(result);
}

void AbstractionDialog::cancelAndClose()
{
  this->close();
}

void AbstractionDialog::saveAndClose()
{
  if (pathsBasedCheckBox->checkState() == Qt::Unchecked &&
      semiPathsBasedCheckBox->checkState() == Qt::Unchecked &&
      noConstraintsCheckBox->checkState() == Qt::Unchecked)
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>No constraint selected</b>");
      warningBox->setInformativeText("One of the options for constraints on the creation "
				     "of the abstract event must be selected");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else if (eventDescriptionField->toPlainText() == "")
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>No description given</b>");
      warningBox->setInformativeText("A description of the event must be given.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else
    {
      exitStatus = 0;
      if (pathsBasedCheckBox->checkState() == Qt::Checked)
	{
	  chosenConstraint = PATHS;
	}
      else if (semiPathsBasedCheckBox->checkState() == Qt::Checked)
	{
	  chosenConstraint = SEMIPATHS;
	}
      else if (noConstraintsCheckBox->checkState() == Qt::Checked)
	{
	  chosenConstraint = NOCONSTRAINT;
	}
      eventDescription = eventDescriptionField->toPlainText();
      this->close();
    }
}

int AbstractionDialog::getExitStatus()
{
  return exitStatus;
}

QString AbstractionDialog::getConstraint()
{
  return chosenConstraint;
}

QVector<EventItem*> AbstractionDialog::getCollectedIncidents()
{
  return collectedIncidents;
}

QString AbstractionDialog::getDescription()
{
  return eventDescription;
}

QString AbstractionDialog::getAttribute()
{
  return selectedAttribute;
}

bool AbstractionDialog::isEntity()
{
  return attributeIsEntity;
}
