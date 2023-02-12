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

#include "../include/RelationshipCheckBoxDialog.h"

RelationshipCheckBoxDialog::RelationshipCheckBoxDialog(QWidget *parent) : QDialog(parent) 
{
  _exitStatus = 1;

  relationshipLabel = new QLabel(tr("<b>Choose relationship:</b>"), this);
  relationshipsFilterLabel = new QLabel(tr("<b>Filter:</b>"), this);

  relationshipsTreeView = new DeselectableTreeView(this);
  relationshipsTreeView->setHeaderHidden(true);
  relationshipsTreeView->header()->setStretchLastSection(false);
  relationshipsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  treeFilter = new RelationshipTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  relationshipsTreeView->setSortingEnabled(true);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
  relationshipsTreeView->installEventFilter(this);

  relationshipsFilterField = new QLineEdit(this);
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);
  
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(relationshipsFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(relationshipsTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setRelationship()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(relationshipLabel);
  mainLayout->addWidget(relationshipsTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(relationshipsFilterLabel);
  filterLayout->addWidget(relationshipsFilterField);
  mainLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setMinimumWidth(400);
  setMinimumHeight(600);
  setWindowTitle("Create mode");
}

void RelationshipCheckBoxDialog::setTree() 
{
  relationshipsTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, directedness, description "
	      "FROM relationship_types "
	      "ORDER BY name ASC");
  while (query->next()) 
    {
      QString currentType = query->value(0).toString();
      QString currentDirection = query->value(1).toString();
      QString typeDescription = query->value(2).toString();
      QStandardItem *type = new QStandardItem(currentType);
      relationshipsTree->appendRow(type);
      QString hint = breakString(currentDirection + " - " + typeDescription);
      type->setToolTip(hint);
      type->setEditable(false);
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT name, comment FROM entity_relationships WHERE type = :type "
		      "ORDER BY name ASC");
      query2->bindValue(":type", currentType);
      query2->exec();
      int children = 0;
      while (query2->next()) 
	{
	  QString name = query2->value(0).toString();
	  QString comment = query2->value(1).toString();
	  QStandardItem *relationship = new QStandardItem(name);
	  relationship->setCheckable(true);
	  type->setChild(children, relationship);
	  relationship->setToolTip(hint);
	  relationship->setEditable(false);
	  children++;
	}
      delete query2;
    }
  delete query;
  treeFilter->setSourceModel(relationshipsTree);
  relationshipsTreeView->setModel(treeFilter);
}


void RelationshipCheckBoxDialog::changeFilter(const QString &text) 
{
  QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
  treeFilter->setFilterRegularExpression(regExp);
}

void RelationshipCheckBoxDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void RelationshipCheckBoxDialog::saveAndClose() 
{
  getChecks(relationshipsTree);
  if (_chosenRelationships.size() > 0)
    {
      _exitStatus = 0;
    }
  else
    {
      _exitStatus = 1;
    }
  this->close();
}

void RelationshipCheckBoxDialog::getChecks(QAbstractItemModel *model)
{
  for (int i = 0; i != model->rowCount(QModelIndex()); i++)
    {
      QModelIndex typeIndex = model->index(i, 0, QModelIndex());
      QString currentType = typeIndex.data().toString();
      if (model->hasChildren(typeIndex))
	{
	  for (int j = 0; j != model->rowCount(typeIndex); j++)
	    {
	      QModelIndex relIndex = model->index(j, 0, typeIndex);
	      QVariant checkState = relIndex.data(Qt::CheckStateRole);
	      QString relName = relIndex.data().toString();
	      if (checkState == Qt::Checked)
		{
		  QPair<QString, QString> currentPair(currentType, relName);
		  _chosenRelationships.push_back(currentPair);
		}
	    }
	}
    }
}

QVector<QPair<QString, QString> > RelationshipCheckBoxDialog::getRelationships() 
{
  return _chosenRelationships;
}

int RelationshipCheckBoxDialog::getExitStatus() 
{
  return _exitStatus;
}

