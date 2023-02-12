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

#include "../include/AttributeCheckBoxDialog.h"

AttributeCheckBoxDialog::AttributeCheckBoxDialog(QWidget *parent, QString type)
  : QDialog(parent) 
{
  _type = type;
  _exitStatus = 1;

  attributeLabel = new QLabel(tr("<b>Choose attribute:</b>"), this);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), this);

  attributesTreeView = new DeselectableTreeView(this);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  attributesTreeView->installEventFilter(this);
  attributesTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

  attributesFilterField = new QLineEdit(this);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(attributesTreeView, SIGNAL(customContextMenuRequested(QPoint)),
	  this, SLOT(customContextMenu(QPoint)));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(attributeLabel);
  mainLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributesFilterLabel);
  filterLayout->addWidget(attributesFilterField);
  mainLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setMinimumWidth(400);
  setMinimumHeight(600);
  setWindowTitle("Select attribute");
}

void AttributeCheckBoxDialog::setTree() 
{
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  if (_type == ENTITY) 
    {
      query->exec("SELECT name, description FROM entity_attributes WHERE father = 'NONE' "
		  "ORDER BY name ASC");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *father = new QStandardItem(name);
	  father->setCheckable(true);
	  attributesTree->appendRow(father);
	  QString hint = breakString(description);
	  father->setToolTip(hint);
	  father->setEditable(false);
	  buildHierarchy(father, name);
	}
    }
  else if (_type == INCIDENT) 
    {
      // First we will fetch the 'normal' attributes.
      {
	query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE' "
		    "ORDER BY name ASC");
	while (query->next()) 
	  {
	    QString name = query->value(0).toString();
	    QString description = query->value(1).toString();
	    QStandardItem *father = new QStandardItem(name);
	    father->setCheckable(true);
	    attributesTree->appendRow(father);
	    QString hint = breakString(description);
	    father->setToolTip(hint);
	    father->setEditable(false);
	    buildHierarchy(father, name);
	  }
	// And then we will also fetch the entities.
	QStandardItem *entities = new QStandardItem(ENTITIES);
	entities->setCheckable(true);
	QString entitiesHint = breakString("You can also assign entities that you have created "
					   "in the relationships widget as attributes.");
	entities->setToolTip(entitiesHint);
	QFont font;
	font.setItalic(true);
	attributesTree->appendRow(entities);
	entities->setFont(font);
	query->exec("SELECT name, description FROM entities WHERE father = 'NONE' "
		    "ORDER BY name ASC");
	int children = 0;
	while (query->next()) 
	  {
	    QString name = query->value(0).toString();
	    QString description = query->value(1).toString();
	    QStandardItem *father = new QStandardItem(name);
	    father->setCheckable(true);
	    entities->setChild(children, father);
	    children++;
	    QString hint = breakString(description);
	    father->setToolTip(hint);
	    father->setEditable(false);
	    buildEntities(father, name);
	  }
      }
    }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void AttributeCheckBoxDialog::buildHierarchy(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  if (_type == ENTITY) 
    {
      query->prepare("SELECT name, description FROM entity_attributes WHERE  father = :father "
		     "ORDER BY name ASC");
      query->bindValue(":father", name);
      query->exec();
      int children = 0;
      while (query->next()) 
	{
	  QString childName = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *child = new QStandardItem(childName);
	  child->setCheckable(true);
	  top->setChild(children, child);
	  QString hint = breakString(description);
	  child->setToolTip(hint);
	  child->setEditable(false);
	  children++;
	  buildHierarchy(child, childName);
	}
    }
  else 
    {
      query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father "
		     "ORDER BY name ASC");
      query->bindValue(":father", name);
      query->exec();
      int children = 0;
      while (query->next()) 
	{
	  QString childName = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *child = new QStandardItem(childName);
	  child->setCheckable(true);
	  top->setChild(children, child);
	  QString hint = breakString(description);
	  child->setToolTip(hint);
	  child->setEditable(false);
	  children++;
	  buildHierarchy(child, childName);
	}
    }
  delete query;
}

void AttributeCheckBoxDialog::buildEntities(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entities WHERE father = :father "
		 "ORDER BY name ASC");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) 
    {
      QString childName = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *child = new QStandardItem(childName);
      child->setCheckable(true);
      top->setChild(children, child);
      QString hint = breakString(description);
      child->setToolTip(hint);
      child->setEditable(false);
      children++;
      buildEntities(child, childName);
    }
  delete query;
}

void AttributeCheckBoxDialog::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void AttributeCheckBoxDialog::customContextMenu(const QPoint &point)
{
  QPoint globalPos = attributesTreeView->mapToGlobal(point);
  QModelIndex index = attributesTreeView->indexAt(point);
  attributesTreeView->selectionModel()->select(index, QItemSelectionModel::Current);
  if (index.isValid())
    {
      QStandardItem *item = attributesTree->itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
      if (item->hasChildren())
	{
	  QMenu *menu = new QMenu;
	  QAction *action1 = new QAction(SELECTALLCHILDRENACTION, this);
	  QAction *action2 = new QAction(DESELECTALLCHILDRENACTION, this);
	  menu->addAction(action1);
	  menu->addAction(action2);
	  if (QAction *action = menu->exec(globalPos))
	    {
	      if (action->text() == SELECTALLCHILDRENACTION)
		{
		  attributesTreeView->expand(index);
		  selectAllChildren(attributesTree, attributesTree->indexFromItem(item));
		}
	      else if (action->text() == DESELECTALLCHILDRENACTION)
		{
		  attributesTreeView->expand(index);
		  deselectAllChildren(attributesTree, attributesTree->indexFromItem(item));
		}
	    }
	  delete menu;
	  delete action1;
	  delete action2;
	}
    }
}

void AttributeCheckBoxDialog::selectAllChildren(const QAbstractItemModel *model,
						const QModelIndex index)
{
  for (int i = 0; i != model->rowCount(index); i++)
    {
      QModelIndex currentIndex = model->index(i, 0, index);
      QStandardItem *currentItem = attributesTree->itemFromIndex(currentIndex);
      currentItem->setCheckState(Qt::Checked);
    }
}

  void AttributeCheckBoxDialog::deselectAllChildren(const QAbstractItemModel *model,
						    const QModelIndex index)
{
  for (int i = 0; i != model->rowCount(index); i++)
    {
      QModelIndex currentIndex = model->index(i, 0, index);
      QStandardItem *currentItem = attributesTree->itemFromIndex(currentIndex);
      currentItem->setCheckState(Qt::Unchecked);
    }
}

void AttributeCheckBoxDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void AttributeCheckBoxDialog::saveAndClose() 
{
  getChecks(attributesTree);
  if (_chosenAttributes.size() > 0)
    {
      _exitStatus = 0;
    }
  else
    {
      _exitStatus = 1;
    }
  this->close();
}

void AttributeCheckBoxDialog::getChecks(QAbstractItemModel *model, QModelIndex parent)
{
  for (int i = 0; i != model->rowCount(parent); i++)
    {
      bool isEntity = false;
      QModelIndex currentIndex = model->index(i, 0, parent);
      QVariant checkState = currentIndex.data(Qt::CheckStateRole);
      if (checkState == Qt::Checked)
	{
	  QString name = currentIndex.data().toString();
	  QModelIndex tempIndex = currentIndex;
	  while(tempIndex.parent().isValid())
	    {
	      tempIndex = tempIndex.parent();
	    }
	  QString top = tempIndex.data().toString();
	  if (top == ENTITIES)
	    {
	      isEntity = true;
	    }
	  QPair<QString, bool> currentPair = QPair<QString, bool>(name, isEntity);
	  _chosenAttributes.push_back(currentPair);
	}
      if (model->hasChildren(currentIndex))
	{
	  getChecks(model, currentIndex);
	}
    }
}

QVector<QPair<QString, bool> > AttributeCheckBoxDialog::getAttributes()
{
  return _chosenAttributes;
}

int AttributeCheckBoxDialog::getExitStatus() 
{
  return _exitStatus;
}

bool AttributeCheckBoxDialog::isEntity() 
{
  return _entity;
}
