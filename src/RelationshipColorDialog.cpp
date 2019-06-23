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

#include "../include/RelationshipColorDialog.h"

RelationshipColorDialog::RelationshipColorDialog(QWidget *parent) : QDialog(parent) 
{
  _exitStatus = 1;
  _chosenColor = QColor(Qt::darkBlue);
  _chosenTextColor = QColor(Qt::black);
  _chosenRelationship = DEFAULT;

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
  
  colorButton = new QPushButton(tr("Choose node color"), this);
  colorButton->setMinimumWidth(200);
  QPixmap pixmapOne(colorButton->width(), colorButton->height());
  pixmapOne.fill(_chosenColor);
  colorOneLabel = new QLabel(this);
  colorOneLabel->setMaximumWidth(50);
  colorOneLabel->setMaximumHeight(colorOneLabel->sizeHint().height());
  colorOneLabel->setPixmap(pixmapOne);
  textColorButton = new QPushButton(tr("Choose text color"), this);
  textColorButton->setMinimumWidth(200);
  QPixmap pixmapTwo(textColorButton->width(), textColorButton->height());
  pixmapTwo.fill(_chosenTextColor);
  colorTwoLabel = new QLabel(this);
  colorTwoLabel->setMaximumWidth(50);
  colorTwoLabel->setMaximumHeight(colorTwoLabel->sizeHint().height());
  colorTwoLabel->setPixmap(pixmapTwo);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);
  
  connect(colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
  connect(textColorButton, SIGNAL(clicked()), this, SLOT(setTextColor()));
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
  QPointer<QHBoxLayout> colorOneLayout = new QHBoxLayout;
  colorOneLayout->addWidget(colorButton);
  colorOneLayout->addWidget(colorOneLabel);
  mainLayout->addLayout(colorOneLayout);
  QPointer<QHBoxLayout> colorTwoLayout = new QHBoxLayout;
  colorTwoLayout->addWidget(textColorButton);
  colorTwoLayout->addWidget(colorTwoLabel);
  mainLayout->addLayout(colorTwoLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setMinimumWidth(400);
  setMinimumHeight(600);
  setWindowTitle("Create mode");
}

void RelationshipColorDialog::setTree() 
{
  relationshipsTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, directedness, description "
	      "FROM relationship_types");
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
      query2->prepare("SELECT name, comment FROM entity_relationships WHERE type = :type");
      query2->bindValue(":type", currentType);
      query2->exec();
      int children = 0;
      while (query2->next()) 
	{
	  QString name = query2->value(0).toString();
	  QString comment = query2->value(1).toString();
	  QStandardItem *relationship = new QStandardItem(name);
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


void RelationshipColorDialog::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void RelationshipColorDialog::setColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _chosenColor = colorDialog->selectedColor();
    }
  QPixmap pixmapOne(100, colorButton->height());
  pixmapOne.fill(_chosenColor);
  colorOneLabel->setPixmap(pixmapOne);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void RelationshipColorDialog::setTextColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _chosenTextColor = colorDialog->selectedColor();
    }
  QPixmap pixmapTwo(100, textColorButton->height());
  pixmapTwo.fill(_chosenTextColor);
  colorTwoLabel->setPixmap(pixmapTwo);
  saveCloseButton->setFocus();
  delete colorDialog;
}
 
void RelationshipColorDialog::setRelationship() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentRelationship = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentRelationship->parent()) 
	{
	  QStandardItem *typeItem = currentRelationship->parent();
	  _chosenRelationship = relationshipsTreeView->currentIndex().data().toString();
	  _relationshipType = typeItem->data(Qt::DisplayRole).toString();
	}
      else 
	{
	  _chosenRelationship = DEFAULT;
	  _relationshipType = DEFAULT;
	}
    }
}

void RelationshipColorDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void RelationshipColorDialog::saveAndClose() 
{
  if (_chosenRelationship == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Saving"); 
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>No relationship chosen.</b>");
      warningBox->setInformativeText("You have to choose a relationship to proceed.");
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

QColor RelationshipColorDialog::getColor() 
{
  return _chosenColor;
}

QColor RelationshipColorDialog::getTextColor() 
{
  return _chosenTextColor;
}

QString RelationshipColorDialog::getRelationship() 
{
  return _chosenRelationship;
}

QString RelationshipColorDialog::getType() 
{
  return _relationshipType;
}

int RelationshipColorDialog::getExitStatus() 
{
  return _exitStatus;
}

