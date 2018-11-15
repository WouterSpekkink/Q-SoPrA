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

#include "../include/EditEntityDialog.h"

EditEntityDialog::EditEntityDialog(QWidget *parent) : QDialog(parent) 
{
  // First we declare the entities of this dialog.
  _entityEdited = 0;
  _fresh = true;
  _relationshipsWidget = qobject_cast<RelationshipsWidget*>(parent);
  
  filterLabel = new QLabel(tr("<b>Filter:</b>"), this);
  entitiesTable = new EntityTableModel(this);
  entitiesTable->setTable("entities");
  entitiesTable->setSort(1, Qt::AscendingOrder);
  entitiesTable->select();
  updateTable();
  entitiesFilter = new EntitiesFilter(this);
  entitiesFilter->setSourceModel(entitiesTable);
  entitiesFilter->setFilterKeyColumn(1);
  entitiesFilter->setDynamicSortFilter(true);
  entitiesView = new ZoomableListView(this);
  entitiesView->setModel(entitiesFilter);
  entitiesView->setModelColumn(1);
  entitiesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
  entityFilterField = new QLineEdit();
  newEntityButton = new QPushButton(tr("Define new entity"), this);
  editEntityButton = new QPushButton(tr("Edit highlighted entity"), this);
  editEntityButton->setEnabled(false);
  removeEntitiesButton = new QPushButton(tr("Remove unused entities"), this);
  closeButton = new QPushButton(tr("Close dialog"), this);
  
  connect(entityFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(filterEntity(const QString &)));
  connect(entitiesView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(newEntityButton, SIGNAL(clicked()), this, SLOT(addEntity()));
  connect(editEntityButton, SIGNAL(clicked()), this, SLOT(editEntity()));
  connect(removeEntitiesButton, SIGNAL(clicked()), this, SLOT(removeEntities()));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(closeThis()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> labelLayout = new QHBoxLayout;
  labelLayout->addWidget(filterLabel);
  labelLayout->addWidget(entityFilterField);
  mainLayout->addLayout(labelLayout);
  QPointer<QHBoxLayout> listsLayout = new QHBoxLayout;
  listsLayout->addWidget(entitiesView);
  mainLayout->addLayout(listsLayout);

  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  QPointer<QVBoxLayout> buttonsLayout = new QVBoxLayout;
  buttonsLayout->addWidget(newEntityButton);
  buttonsLayout->addWidget(editEntityButton);
  buttonsLayout->addWidget(removeEntitiesButton);
  mainLayout->addLayout(buttonsLayout);
  
  QPointer<QFrame> bottomLine = new QFrame;
  bottomLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(bottomLine);

  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(closeButton);
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Entities overview");
  resize(600, 600);
}

void EditEntityDialog::filterEntity(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);
  entitiesFilter->setFilterRegExp(regExp);
}

void EditEntityDialog::addEntity() 
{
  EntityDialog *entityDialog = new EntityDialog(this);
  entityDialog->setRelationshipsWidget(_relationshipsWidget);
  entityDialog->setNew();
  entityDialog->exec();
  if (entityDialog->getExitStatus() == 0) 
    {
      QString name = entityDialog->getName();
      QString description = entityDialog->getDescription();
      QString father = "NONE";
      QSqlQuery *query = new QSqlQuery;
      query->prepare("INSERT INTO entities (name, description, father) "
		     "VALUES (:name, :description, :father)");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":father", father);
      query->exec();
      delete query;
    }
  delete entityDialog;
  entitiesTable->select();
  updateTable();
  filterEntity(entityFilterField->text());
  entitiesFilter->sort(1, Qt::AscendingOrder);
}

void EditEntityDialog::editEntity() 
{
  if (entitiesView->currentIndex().isValid()) 
    {
      QString selected = entitiesView->currentIndex().data(Qt::DisplayRole).toString();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT description FROM entities WHERE name = :name");
      query->bindValue(":name", selected);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      EntityDialog *entityDialog = new EntityDialog(this);
      entityDialog->setRelationshipsWidget(_relationshipsWidget);
      entityDialog->submitName(selected);
      entityDialog->submitDescription(description);
      entityDialog->exec();
      if (entityDialog->getExitStatus() == 0) 
	{
	  QString name = entityDialog->getName();
	  QString description = entityDialog->getDescription();
	  updateAfterEdit(name, description, selected);
	}
      delete query;
      delete entityDialog;
      entitiesTable->select();
      updateTable();
      entitiesFilter->sort(1, Qt::AscendingOrder);
    }
}

void EditEntityDialog::updateAfterEdit(const QString name,
				       const QString description,
				       const QString former) 
{
  _entityEdited = 1;
  QSqlQuery *query = new QSqlQuery;
  // Update the entity itself.
  query->prepare("UPDATE entities "
		 "SET name = :name, description = :description "
		 "WHERE name = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();

  // Update attributes.
  query->prepare("UPDATE attributes_to_entities "
		 "SET entity = :new "
		 "WHERE entity = :old");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();

  /*
    Next up are the relationships in which the entity already participates.
    First, let us update all the relationships where the current entity is a source.
  */
  if (name != former) 
    {
      query->prepare("SELECT name, target, type "
		     "FROM entity_relationships WHERE source = :current");
      query->bindValue(":current", former);
      query->exec();
      while (query->next()) 
	{
	  QString oldRelationship = query->value(0).toString();
	  QString target = query->value(1).toString();
	  QString type = query->value(2).toString();
	  QSqlQuery *query2 = new QSqlQuery();
	  query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
	  query2->bindValue(":type", type);
	  query2->exec();
	  query2->first();
	  QString directedness = query2->value(0).toString();
	  QString arrow = "";
	  if (directedness == UNDIRECTED) 
	    {
	      arrow = "<-->";
	    }
	  else if (directedness == DIRECTED) 
	    {
	      arrow = "--->";
	    }
	  QString newRelationship = name + arrow + target;
	  query2->prepare("UPDATE entity_relationships "
			  "SET source = :source, name = :name "
			  "WHERE source = :oldSource AND name = :oldRelationship");
	  query2->bindValue(":source", name);
	  query2->bindValue(":name", newRelationship);
	  query2->bindValue(":oldSource", former);
	  query2->bindValue(":oldRelationship", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents_sources "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  delete query2;
	}
      // And then the relationships where the entity is a target.
      query->prepare("SELECT name, source, type "
		     "FROM entity_relationships WHERE target = :current");
      query->bindValue(":current", former);
      query->exec();
      while (query->next()) 
	{
	  QString oldRelationship = query->value(0).toString();
	  QString source = query->value(1).toString();
	  QString type = query->value(2).toString();
	  QSqlQuery *query2 = new QSqlQuery();
	  query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
	  query2->bindValue(":type", type);
	  query2->exec();
	  query2->first();
	  QString directedness = query2->value(0).toString();
	  QString arrow = "";
	  if (directedness == UNDIRECTED) 
	    {
	      arrow = "<-->";
	    }
	  else if (directedness == DIRECTED) 
	    {
	      arrow = "--->";
	    }
	  QString newRelationship = source + arrow + name;
	  query2->prepare("UPDATE entity_relationships "
			  "SET target = :target, name = :name "
			  "WHERE target = :oldTarget AND name = :oldRelationship");
	  query2->bindValue(":target", name);
	  query2->bindValue(":name", newRelationship);
	  query2->bindValue(":oldTarget", former);
	  query2->bindValue(":oldRelationship", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents_sources "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  delete query2;
	}
      delete query;
    }
}  

void EditEntityDialog::removeEntities() 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  QVector<MacroEvent*> macroVector = _eventGraph->getMacros();
  QSet<QString> takenAttributes;
  QVectorIterator<MacroEvent*> it(macroVector);
  while (it.hasNext()) 
    {
      MacroEvent* current = it.next();
      QSet<QString> attributes = current->getAttributes();
      QSet<QString>::iterator it2;
      for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
	{
	  takenAttributes.insert(*it2);
	}
    }
  while (unfinished) 
    {
      query->exec("SELECT name FROM entities "
		  "EXCEPT SELECT source FROM entity_relationships "
		  "EXCEPT SELECT target FROM entity_relationships "
		  "EXCEPT SELECT attribute FROM attributes_to_incidents "
		  "EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_macro_events "
		  "EXCEPT SELECT attribute FROM saved_og_plots_occurrence_items "
		  "EXCEPT SELECT father FROM entities");
      QSet<QString> temp;
      while (query->next()) 
	{
	  QString current = query->value(0).toString();			   
	  temp.insert(current);
	}
      QSet<QString>::iterator it3;
      bool found = false;
      for (it3 = temp.begin(); it3 != temp.end(); it3++) 
	{
	  if (!takenAttributes.contains(*it3)) 
	    {
	      found = true;
	      query2->prepare("DELETE FROM entities WHERE name = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_entities WHERE entity = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents WHERE attribute = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents_sources WHERE attribute = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	    }
	}
      if (!found) 
	{
	  unfinished = false;
	}
    }
  delete query;  
  delete query2;
  //  occurrenceGraph->checkCongruency(); // Does not seem necessary
  entitiesTable->select();
  updateTable();
  filterEntity(entityFilterField->text());
  entitiesFilter->sort(1, Qt::AscendingOrder);
  // Also remove the entities from attribute trees.
  _eventGraph->resetTree();
  _attributesWidget->resetTree();
}

void EditEntityDialog::closeThis() 
{
  this->close();
}

void EditEntityDialog::setButtons() 
{
  if (_fresh) 
    {
      _fresh = false;
    }
  if (entitiesView->currentIndex().isValid()) 
    {
      editEntityButton->setEnabled(true);
    }
  else 
    {
      editEntityButton->setEnabled(false);
    }
}

void EditEntityDialog::updateTable() 
{
  while (entitiesTable->canFetchMore()) 
    {
      entitiesTable->fetchMore();
    }
}

int EditEntityDialog::getEntityEdited() 
{
  return _entityEdited;
}

void EditEntityDialog::setEventGraph(EventGraphWidget *eventGraphWidget)
{
  _eventGraph = eventGraphWidget;
}

void EditEntityDialog::setAttributesWidget(AttributesWidget *attributesWidget)
{
  _attributesWidget = attributesWidget;
}

