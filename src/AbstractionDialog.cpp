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

#include "../include/AbstractionDialog.h"

AbstractionDialog::AbstractionDialog(QWidget *parent,
				     QVector<IncidentNode*> eventVector,
				     QVector<AbstractNode*> abstractNodeVector,
				     QVector<QGraphicsItem*> currentData,
				     QVector<QString> presentTypes,
				     QString selectedCoder) : QDialog(parent)
{
  _exitStatus = 1;
  _eventVector = eventVector;
  _abstractNodeVector = abstractNodeVector;
  _currentData = currentData;
  _presentTypes = presentTypes;
  _selectedAttribute = DEFAULT2;
  _selectedCoder = selectedCoder;
  _attributeIsEntity = false;
  _chosenConstraint = "";
  _eventDescription = "";
  _inheritance = false;
  _eventTiming = "";
  
  constraintsLabel = new QLabel(tr("<b>Available constraints:</b>"), this);
  attributeOptionsLabel = new QLabel(tr("<b>Group on attribute:</b>"), this);
  attributeLabel = new QLabel(tr("<b>Selected attribute:</b>"), this);
  attributeInheritanceLabel = new QLabel(tr("<b>Inherit attributes:</b>"), this);
  chosenAttributeLabel = new QLabel(DEFAULT2, this);
  chosenAttributeLabel->setStyleSheet("color: red");
  descriptionLabel = new QLabel(tr("<b>Event description:</b>"), this);
  timingLabel = new QLabel(tr("<b>Timing:</b>"), this);

  pathsBasedRadioButton = new QRadioButton("Paths-based constraints", this);
  pathsBasedRadioButton->setEnabled(false);
  semiPathsBasedRadioButton = new QRadioButton("Semipaths-based constraints", this);
  semiPathsBasedRadioButton->setEnabled(false);
  noConstraintsRadioButton = new QRadioButton("No constraints", this);
  noConstraintsRadioButton->setChecked(true);
  
  eventDescriptionField = new QTextEdit(this);

  timingField = new QLineEdit(this);

  setAttributeButton = new QPushButton(tr("Set attribute"), this);
  clearAttributeButton = new QPushButton(tr("Clear attribute"), this);
  clearAttributeButton->setEnabled(false);
  inheritAttributesButton = new QPushButton(tr("Inherit attributes any"));
  inheritSharedAttributesButton = new QPushButton(tr("Inherit attributes shared"));
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Create abstract event"), this);
  if (_currentData.size() == 1)
    {
      setAttributeButton->setEnabled(false);
    }
  connect(setAttributeButton, SIGNAL(clicked()), this, SLOT(selectAttribute()));
  connect(clearAttributeButton, SIGNAL(clicked()), this, SLOT(clearAttribute()));
  connect(inheritAttributesButton, SIGNAL(clicked()), this, SLOT(inheritAttributes()));
  connect(inheritSharedAttributesButton, SIGNAL(clicked()), this, SLOT(inheritSharedAttributes()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> dualLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QVBoxLayout> constraintsLayout = new QVBoxLayout;
  constraintsLayout->addWidget(constraintsLabel);
  constraintsLayout->addWidget(pathsBasedRadioButton);
  constraintsLayout->addWidget(semiPathsBasedRadioButton);
  constraintsLayout->addWidget(noConstraintsRadioButton);
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

  QPointer<QFrame> sepLineTwo = new QFrame;
  sepLineTwo->setFrameShape(QFrame::HLine);
  leftLayout->addWidget(sepLineTwo);

  QPointer<QVBoxLayout> inheritanceLayout = new QVBoxLayout;
  inheritanceLayout->addWidget(attributeInheritanceLabel);
  inheritanceLayout->addWidget(inheritAttributesButton);
  inheritanceLayout->addWidget(inheritSharedAttributesButton);
  leftLayout->addLayout(inheritanceLayout);
  
  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> timingLayout = new QHBoxLayout;
  timingLayout->addWidget(timingLabel);
  timingLayout->addWidget(timingField);
  rightLayout->addLayout(timingLayout);
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

AbstractionDialog::~AbstractionDialog()
{
  // We want to clear these vectors, but we don't want to
  // delete the pointers they contain.
  _eventVector.clear();
  _abstractNodeVector.clear();
  _collectedIncidents.clear();
  _currentData.clear();
}

void AbstractionDialog::selectAttribute()
{
  QPointer<SimpleAttributeSelectionDialog> attributeSelection =
    new SimpleAttributeSelectionDialog(this, INCIDENT);
  attributeSelection->exec();
  if (attributeSelection->getExitStatus() == 0) 
    {
      _selectedAttribute = attributeSelection->getAttribute();
      chosenAttributeLabel->setText(_selectedAttribute);
      _attributeIsEntity = attributeSelection->isEntity();
      QVectorIterator<QGraphicsItem*> it(_currentData);
      prepareEvents();
      clearAttributeButton->setEnabled(true);
      _inheritance = false;
      _inheritedAttributes.clear();
      return;
    }
  else 
    {
      _selectedAttribute = DEFAULT2;
      chosenAttributeLabel->setText(DEFAULT2);
      _attributeIsEntity = false;
      prepareEvents();
      clearAttributeButton->setEnabled(false);
      return;
    }
}

void AbstractionDialog::clearAttribute()
{
  _selectedAttribute = DEFAULT2;
  chosenAttributeLabel->setText(DEFAULT2);
  clearAttributeButton->setEnabled(false);
  _inheritance = false;
  _inheritedAttributes.clear();
}

void AbstractionDialog::inheritAttributes()
{
  // identify attributes assigned to collected incidents and their abstractNode parents
  if (_collectedIncidents.size() != 0)
    {
      QSet<QString> collectedAttributes;
      QVectorIterator<IncidentNode*> it(_collectedIncidents);
      while (it.hasNext())
	{
	  IncidentNode *currentIncident = it.next();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :incident");
	  query->bindValue(":incident", currentIncident->getId());
	  query->exec();
	  while (query->next())
	    {
	      QString currentAttribute = query->value(0).toString();
	      collectedAttributes.insert(currentAttribute);
	    }
	  delete query;
	  AbstractNode *abstractNode = currentIncident->getAbstractNode();
	  if (abstractNode != NULL)
	    {
	      QSet<QString> abstractNodeAttributes = abstractNode->getAttributes();
	      QSetIterator<QString> it2(abstractNodeAttributes);
	      while(it2.hasNext())
		{
		  collectedAttributes.insert(it2.next());
		}
	    }
	}
      // Now we have a set of attributes that we could inherit
      // Let's now give the user the option to select the ones that must be inherited
      if (collectedAttributes.size() > 0)
	{
	  QPointer<InheritanceDialog> inheritanceDialog = new InheritanceDialog(this,
										collectedAttributes);
	  inheritanceDialog->exec();
	  if (inheritanceDialog->getExitStatus() == 0)
	    {
	      QVector<QString> selectedAttributes = inheritanceDialog->getSelected();
	      if (selectedAttributes.size() != 0)
		{
		  _inheritance = true;
		  _inheritedAttributes = selectedAttributes;
		}
	    }
	}
      else
	{
	  QPointer<QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->setWindowTitle("Inheriting attributes");
	  warningBox->addButton(QMessageBox::Ok);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<b>No attributes detected</b>");
	  warningBox->setInformativeText("No attributes to inherit.");
	  warningBox->exec();
	  delete warningBox;
	}
    }
}

void AbstractionDialog::inheritSharedAttributes()
{
  // identify attributes assigned to collected incidents and their abstractNode parents
  if (_collectedIncidents.size() != 0)
    {
      QSet<QString> previousAttributes;
      QSet<QString> sharedAttributes;
      QVectorIterator<IncidentNode*> it(_collectedIncidents);
      while (it.hasNext())
	{
	  sharedAttributes.clear();
	  IncidentNode *currentIncident = it.next();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE incident = :incident");
	  query->bindValue(":incident", currentIncident->getId());
	  query->exec();
	  while (query->next())
	    {
	      QString currentAttribute = query->value(0).toString();
	      if (currentIncident == _collectedIncidents.first())
		{
		  previousAttributes.insert(currentAttribute);		  
		}
	      else
		{
		  if (previousAttributes.contains(currentAttribute))
		    {
		      sharedAttributes.insert(currentAttribute);
		    }
		}
	    }
	  delete query;
	  AbstractNode *abstractNode = currentIncident->getAbstractNode();
	  if (abstractNode != NULL)
	    {
	      QSet<QString> abstractNodeAttributes = abstractNode->getAttributes();
	      QSetIterator<QString> it2(abstractNodeAttributes);
	      while(it2.hasNext())
		{
		  QString currentAttribute = it2.next();
		  if (previousAttributes.contains(currentAttribute))
		    {
		      sharedAttributes.insert(currentAttribute);
		    }
		}
	    }
	  if (currentIncident != _collectedIncidents.first())
	    {
	      previousAttributes = sharedAttributes;
	    }
	}
      // Now we have a set of attributes that we could inherit
      // Let's now give the user the option to select the ones that must be inherited
      if (sharedAttributes.size() > 0)
	{
	  QPointer<InheritanceDialog> inheritanceDialog = new InheritanceDialog(this, sharedAttributes);
	  inheritanceDialog->exec();
	  if (inheritanceDialog->getExitStatus() == 0)
	    {
	      QVector<QString> selectedAttributes = inheritanceDialog->getSelected();
	      if (selectedAttributes.size() != 0)
		{
		  _inheritance = true;
		  _inheritedAttributes = selectedAttributes;
		}
	    }
	}
      else
	{
	  QPointer<QMessageBox> warningBox = new QMessageBox(this);
	  warningBox->setWindowTitle("Inheriting attributes");
	  warningBox->addButton(QMessageBox::Ok);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<b>No attributes detected</b>");
	  warningBox->setInformativeText("No shared attributes to inherit.");
	  warningBox->exec();
	  delete warningBox;
	 }
    }
}

void AbstractionDialog::prepareEvents() 
{
  if (_currentData.size() > 0) 
    {
      _collectedIncidents.clear();
      QVector<IncidentNode*> allIncidents;
      _semiPathsAllowed = true;      
      _pathsAllowed = true;
      QVectorIterator<QGraphicsItem*> it(_currentData);
      while (it.hasNext()) 
	{
	  IncidentNode *event = qgraphicsitem_cast<IncidentNode*>(it.peekNext());
	  AbstractNode *abstractNode = qgraphicsitem_cast<AbstractNode*>(it.peekNext());
	  if (event) 
	    {
	      IncidentNode *currentIncidentNode = qgraphicsitem_cast<IncidentNode*>(it.next());
	      allIncidents.push_back(currentIncidentNode);
	      int id = currentIncidentNode->getId();
	      if (_selectedAttribute != DEFAULT2) 
		{
		  QVector<QString> attributes;
		  attributes.push_back(_selectedAttribute);
		  findChildren(_selectedAttribute, &attributes, _attributeIsEntity);
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
		      _collectedIncidents.push_back(currentIncidentNode);
		    }
		}
	      else 
		{
		  _collectedIncidents.push_back(currentIncidentNode);
		}
	    }
	  if (abstractNode) 
	    {
	      AbstractNode *currentAbstractNode = qgraphicsitem_cast<AbstractNode*>(it.next());
	      if (_selectedAttribute != DEFAULT2)
		{
		  QSet<QString> abstractNodeAttributes = currentAbstractNode->getAttributes();
		  QVector<QString> attributes;
		  attributes.push_back(_selectedAttribute);
		  findChildren(_selectedAttribute, &attributes, _attributeIsEntity);
		  bool hasAttribute = false;
		  QVectorIterator<QString> it2(attributes);
		  while (it2.hasNext())
		    {
		      QString currentAttribute = it2.next();
		      if (abstractNodeAttributes.contains(currentAttribute))
			{
			  hasAttribute = true;
			  break;
			}
		    }
		  QVectorIterator<IncidentNode*> it3(currentAbstractNode->getIncidents());
		  while (it3.hasNext())
		    {
		      IncidentNode* currentIncidentNode = it3.next();
		      if (hasAttribute) 
			{
			  allIncidents.push_back(currentIncidentNode);
			  _collectedIncidents.push_back(currentIncidentNode);			  
			}
		      else
			{
			  allIncidents.push_back(currentIncidentNode);
			}
		    }
		}
	      else 
		{
		  QVectorIterator<IncidentNode*> it3(currentAbstractNode->getIncidents());
		  while (it3.hasNext())
		    {
		      IncidentNode *currentIncidentNode = it3.next();
		      allIncidents.push_back(currentIncidentNode);
		      _collectedIncidents.push_back(currentIncidentNode);			  
		    }
		}
	    }
	}
      if (_collectedIncidents.size() > 0)
	{
	  std::sort(_collectedIncidents.begin(), _collectedIncidents.end(), eventLessThan);
	  std::sort(allIncidents.begin(), allIncidents.end(), eventLessThan);
	  QApplication::setOverrideCursor(Qt::WaitCursor);
	  QVector<QGraphicsItem*> allEvents;
	  QVectorIterator<IncidentNode*> evIt(_eventVector);
	  while (evIt.hasNext()) 
	    {
	      allEvents.push_back(evIt.next());
	    }
	  QVectorIterator<AbstractNode*> maIt(_abstractNodeVector);
	  while (maIt.hasNext()) 
	    {
	      allEvents.push_back(maIt.next());
	    }
	  std::sort(allEvents.begin(), allEvents.end(), componentsSort);
	  IncidentNode *first = allIncidents.first();
	  IncidentNode *last = allIncidents.last();
	  QSet<int> markOne;
	  QSet<int> markTwo;
	  QVectorIterator<QString> lit(_presentTypes);
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
		  IncidentNode *eventTemp = qgraphicsitem_cast<IncidentNode*>(temp);
		  AbstractNode *abstractNodeTemp = qgraphicsitem_cast<AbstractNode*>(temp);
		  if (eventTemp)
		    {
		      if (markOne.contains(eventTemp->getId()) &&
			  markTwo.contains(eventTemp->getId()) &&
			  !allIncidents.contains(eventTemp)) 
			{
			  _semiPathsAllowed = false;
			}
		    }
		  else if (abstractNodeTemp) 
		    {
		      QVector<IncidentNode*> abstractNodeIncidents = abstractNodeTemp->getIncidents();
		      QVectorIterator<IncidentNode*> it5(abstractNodeIncidents);
		      while (it5.hasNext()) 
			{
			  eventTemp = it5.next();
			  if (markOne.contains(eventTemp->getId()) &&
			      markTwo.contains(eventTemp->getId()) &&
			      !allIncidents.contains(eventTemp)) 
			    {
			      _semiPathsAllowed = false;
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
	  warningBox->setWindowTitle("Selecting incidents");
	  warningBox->addButton(QMessageBox::Ok);
	  warningBox->setIcon(QMessageBox::Warning);
	  warningBox->setText("<b>No incidents selected</b>");
	  warningBox->setInformativeText("None of the selected incidents have the selected "
					 "attribute.");
	  warningBox->exec();
	  delete warningBox;
	  _selectedAttribute = DEFAULT2;
	  chosenAttributeLabel->setText(DEFAULT2);
	  prepareEvents();
	}
    }
  else 
    {
      _semiPathsAllowed = false;
      _pathsAllowed = false;
    }
  std::sort(_collectedIncidents.begin(), _collectedIncidents.end(), componentsSort);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT timestamp FROM incidents WHERE id = :id");
  query->bindValue(":id", _collectedIncidents.first()->getId());
  query->exec();
  query->first();
  QString timingBegin = query->value(0).toString();
  query->bindValue(":id", _collectedIncidents.last()->getId());
  query->exec();
  query->first();
  QString timingEnd = query->value(0).toString();
  delete query;
  timingField->setText("From " + timingBegin + " to " + timingEnd);
  evaluateConstraints();
}

void AbstractionDialog::checkConstraints(QVector<IncidentNode*> incidents) 
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  QVector<int> incidentId;
  QVectorIterator<IncidentNode*> it(incidents);
  while (it.hasNext()) 
    {
      IncidentNode *event = qgraphicsitem_cast<IncidentNode*>(it.next());
      if (event) 
	{
	  incidentId.push_back(event->getId());
	}
    }
  QVector<bool> linkagePresence = checkLinkagePresence(incidentId);
  if (linkagePresence.contains(true))
    {
      for (QVector<QString>::size_type lit = 0; lit != _presentTypes.size(); lit++) 
	{
	  QString currentType = _presentTypes[lit];
	  if (linkagePresence[lit] == true) 
	    {
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	      query->bindValue(":type", currentType);
	      query->exec();
	      query->first();
	      QString direction = query->value(0).toString();
	      QVectorIterator<IncidentNode*> dit(incidents);
	      while (dit.hasNext()) 
		{
		  QSet<int> markPaths;
		  QSet<int> markSemiPaths;
		  IncidentNode *departure = dit.next();
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
		  QVectorIterator<IncidentNode*> cit(incidents);
		  while (cit.hasNext()) 
		    {
		      IncidentNode *current = cit.next();
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
				  _pathsAllowed = false;
				}
			    }
			  else if (direction == FUTURE) 
			    {
			      if (!pathsFound && current->getOrder() > departure->getOrder()) 
				{
				  _pathsAllowed = false;
				}
			    }
			  if (!semiPathsFound) 
			    {
			      _semiPathsAllowed = false;
			    }
			}
		    }
		  // Then we check the external consistency.
		  query->prepare("SELECT tail FROM linkages "
				 "WHERE head = :head AND type = :type AND coder = :coder");
		  query->bindValue(":head", departure->getId());
		  query->bindValue(":type", currentType);
		  query->bindValue(":coder", _selectedCoder);
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
			  QVectorIterator<IncidentNode*> kit(incidents);
			  while (kit.hasNext()) 
			    {
			      IncidentNode *current = kit.next();
			      bool pathsFoundTwo = false;
			      if (markPathsTwo.contains(current->getId())) 
				{
				  pathsFoundTwo = true;
				}
			      if (!pathsFoundTwo) 
				{
				  _pathsAllowed = false;
				}
			    }
			}
		    }
		  query->prepare("SELECT head FROM linkages "
				 "WHERE tail = :tail AND type = :type AND coder = :coder");
		  query->bindValue(":tail", departure->getId());
		  query->bindValue(":type", currentType);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();
		  while (query->next()) 
		    {
		      int currentHead = query->value(0).toInt();
		      if (!(incidentId.contains(currentHead))) 
			{
			  QVectorIterator<IncidentNode*> lit(incidents);
			  while (lit.hasNext()) 
			    {
			      IncidentNode *current = lit.next();
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
				      _pathsAllowed = false;
				    }
				}
			    }
			}
		    }
		}
	      delete query;
	    }
	}
    }
  else
    {
      _pathsAllowed = false;
      _semiPathsAllowed = false;
    }
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void AbstractionDialog::evaluateConstraints()
{
  if (_pathsAllowed)
    {
      pathsBasedRadioButton->setEnabled(true);
    }
  else
    {
      pathsBasedRadioButton->setEnabled(false);
    }
  if (_semiPathsAllowed)
    {
      semiPathsBasedRadioButton->setEnabled(true);
    }
  else
    {
      semiPathsBasedRadioButton->setEnabled(false);
    }
  noConstraintsRadioButton->setChecked(true);
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
  query->bindValue(":coder", _selectedCoder);
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
  query->bindValue(":coder", _selectedCoder);
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
      query->bindValue(":coder", _selectedCoder);
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
      query3->bindValue(":coder", _selectedCoder);
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
  query->bindValue(":coder", _selectedCoder);
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
  query->bindValue(":coder", _selectedCoder);
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
  // We create a vector that indicates for each type of linkage whether it is present
  // in the current selection.
  QVector<bool> result;
  QVectorIterator<QString> it(_presentTypes);
  while (it.hasNext()) 
    {
      QString currentType = it.next();
      // 'observed' is a vector that holds all incidents that are connected by
      // a linkage of this type.
      QVector<int> observed;
      // We also need a vector to store our linkages, which we will do as pairs.
      QVector<QPair<int, int>> edges;
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail, head FROM linkages "
		     "WHERE type = :type");
      query->bindValue(":type", currentType);
      query->exec();
      while (query->next()) 
	{
	  int tail = query->value(0).toInt();
	  int head = query->value(1).toInt();
	  observed.push_back(tail);
	  observed.push_back(head);
	  edges.push_back(QPair<int,int>(tail, head));
	}
      delete query;
      // We iterate through the 'observed' vector to see if our current selection
      // has incidents included in them. We put these in a vector called 'included'.
      QVectorIterator<int> it2(observed);
      QVector<int> included;
      // Status will record the presence of linkages. We need to make exceptions for
      // some cases.
      bool status = false;
      while (it2.hasNext()) 
	{
	  int currentObserved = it2.next();
	  if (incidentIds.contains(currentObserved)) 
	    {
	      included.push_back(currentObserved);
	    }
	}
      // If we have more than two incidents connected by a linkage of this type
      // then we should always consider the linkage as present.
      if (included.size() > 2)
	{
	  status = true;
	}
      // If there are only one or two linkages connected by this kind of linkage, then
      // there might be situations that should count as exceptions. 
      else if (included.size() <= 2 && included.size() > 0)
	{
	  // We need to see if any of the incidents in our selection are connected by
	  // the current type of linkage. It might just be the first and the last incidents
	  // that are both connected to an 'outsider', in which case we don't want to count
	  // this linkage type as present.
	  QVectorIterator<int> it3(incidentIds);
	  while (it3.hasNext())
	    {
	      int first = it3.next();
	      QVectorIterator<int> it4(incidentIds);
	      while (it4.hasNext())
		{
		  int second = it4.next();
		  if (first != second)
		    {
		      // Depending on the direction of the linkage, the order of
		      // tail and head might be different. We therefore consider
		      // two cases for each pair of incidents.
		      QPair<int, int> caseOne = QPair<int, int>(first, second);
		      QPair<int, int> caseTwo = QPair<int, int>(second, first);
		      if (edges.contains(caseOne) || edges.contains(caseTwo))
			{
			  status = true;
			}
		    }
		}
	    }
	  // Now we still need to make sure that if there is a linkage with 'outsiders'
	  // This linkage does only exist with either the first or the last member of
	  // the current selection.	 
	  if (included[0] != incidentIds.first() &&
	      included[0] != incidentIds.last())
	    {
	      status = true;
	    }
	  if (included.size() == 2)
	    {
	      if (included[1] != incidentIds.first() &&
		  included[1] != incidentIds.last())
		{
		  status = true;
		}
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
  if (eventDescriptionField->toPlainText() == "")
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving event");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>No description given</b>");
      warningBox->setInformativeText("A description of the event must be given.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else if (timingField->text() == "")
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving event");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>No timing given</b>");
      warningBox->setInformativeText("A timing for the event must be given.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else
    {
      _exitStatus = 0;
      if (pathsBasedRadioButton->isChecked())
	{
	  _chosenConstraint = PATHS;
	}
      else if (semiPathsBasedRadioButton->isChecked())
	{
	  _chosenConstraint = SEMIPATHS;
	}
      else if (noConstraintsRadioButton->isChecked())
	{
	  _chosenConstraint = NOCONSTRAINT;
	}
      _eventDescription = eventDescriptionField->toPlainText();
      _eventTiming = timingField->text();
      this->close();
    }
}

int AbstractionDialog::getExitStatus()
{
  return _exitStatus;
}

QString AbstractionDialog::getConstraint()
{
  return _chosenConstraint;
}

QVector<IncidentNode*> AbstractionDialog::getCollectedIncidents()
{
  return _collectedIncidents;
}

QString AbstractionDialog::getDescription()
{
  return _eventDescription;
}

QString AbstractionDialog::getTiming()
{
  return _eventTiming;
}

QString AbstractionDialog::getAttribute()
{
  return _selectedAttribute;
}

bool AbstractionDialog::isEntity()
{
  return _attributeIsEntity;
}

bool AbstractionDialog::isInheriting()
{
  return _inheritance;
}

QVector<QString> AbstractionDialog::getInheritance()
{
  return _inheritedAttributes;
}
