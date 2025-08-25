/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2025 Wouter Spekkink  

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

#include "../include/SimpleAttributeSelectionDialog.h"

SimpleAttributeSelectionDialog::SimpleAttributeSelectionDialog(QWidget *parent, QString type)
  : QDialog(parent) 
{
  _type = type;
  _exitStatus = 1;
  _chosenAttribute = DEFAULT;

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

  attributesFilterField = new QLineEdit(this);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setAttribute()));

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

void SimpleAttributeSelectionDialog::setTree() 
{
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  if (_type == ENTITY) 
    {
      query->exec("SELECT name, description FROM entity_attributes WHERE father = 'NONE'");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *father = new QStandardItem(name);    
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
	query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE'");
	while (query->next()) 
	  {
	    QString name = query->value(0).toString();
	    QString description = query->value(1).toString();
	    QStandardItem *father = new QStandardItem(name);    
	    attributesTree->appendRow(father);
	    QString hint = breakString(description);
	    father->setToolTip(hint);
	    father->setEditable(false);
	    buildHierarchy(father, name);
	  }
	// And then we will also fetch the entities.
	QStandardItem *entities = new QStandardItem(ENTITIES);
	QString entitiesHint = breakString("You can also assign entities that you have created "
					   "in the relationships widget as attributes.");
	entities->setToolTip(entitiesHint);
	QFont font;
	font.setItalic(true);
	attributesTree->appendRow(entities);
	entities->setFont(font);
	query->exec("SELECT name, description FROM entities WHERE father = 'NONE'");
	int children = 0;
	while (query->next()) 
	  {
	    QString name = query->value(0).toString();
	    QString description = query->value(1).toString();
	    QStandardItem *father = new QStandardItem(name);
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

void SimpleAttributeSelectionDialog::buildHierarchy(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  if (_type == ENTITY) 
    {
      query->prepare("SELECT name, description FROM entity_attributes WHERE  father = :father");
      query->bindValue(":father", name);
      query->exec();
      int children = 0;
      while (query->next()) 
	{
	  QString childName = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *child = new QStandardItem(childName);
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
      query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father");
      query->bindValue(":father", name);
      query->exec();
      int children = 0;
      while (query->next()) 
	{
	  QString childName = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *child = new QStandardItem(childName);
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

void SimpleAttributeSelectionDialog::buildEntities(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entities WHERE father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) 
    {
      QString childName = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *child = new QStandardItem(childName);
      top->setChild(children, child);
      QString hint = breakString(description);
      child->setToolTip(hint);
      child->setEditable(false);
      children++;
      buildEntities(child, childName);
    }
  delete query;
}

void SimpleAttributeSelectionDialog::changeFilter(const QString &text) 
{
  QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
  treeFilter->setFilterRegularExpression(regExp);
}
 
void SimpleAttributeSelectionDialog::setAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      _chosenAttribute = attributesTreeView->currentIndex().data().toString();
    }
  QModelIndex currentIndex = attributesTreeView->currentIndex();
  while (currentIndex.parent().isValid()) 
    {
      currentIndex = currentIndex.parent();
    }
  if (currentIndex.data().toString() == ENTITIES) 
    {
      _entity = true;
    }
  else 
    {
      _entity = false;
    }
}

void SimpleAttributeSelectionDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void SimpleAttributeSelectionDialog::saveAndClose() 
{
  if (_chosenAttribute == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Selecting attribute");
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No attribute chosen.");
      warningBox->setInformativeText("You have to choose an attribute to proceed.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else 
    {
      _exitStatus = 0;
      this->close();
    }
}

QString SimpleAttributeSelectionDialog::getAttribute() 
{
  return _chosenAttribute;
}

int SimpleAttributeSelectionDialog::getExitStatus() 
{
  return _exitStatus;
}

bool SimpleAttributeSelectionDialog::isEntity() 
{
  return _entity;
}
