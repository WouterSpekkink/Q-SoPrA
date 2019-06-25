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

#include "../include/InheritanceDialog.h"
#include <QDebug>

InheritanceDialog::InheritanceDialog(QWidget *parent, QSet<QString> attributes) : QDialog(parent)
{
  // First initialise some variables
  _exitStatus = 1;
  
  // Let's then create our check boxes
  QList<QString> attributeList = attributes.toList();
  std::sort(attributeList.begin(), attributeList.end());
  QListIterator<QString> it(attributeList);

  checkBoxWidget = new QWidget(this);
  
  while (it.hasNext())
    {
      QString currentAttribute = it.next();
      QCheckBox *currentCheckBox = new QCheckBox(currentAttribute, checkBoxWidget);
      _selection.push_back(currentAttribute);
      checkBoxVector.push_back(currentCheckBox);
    }

  // Then we can create our other interface elements
  titleLabel = new QLabel(tr("<b>Select attributes to inherit:</b>"), this);

  scrollArea = new QScrollArea(this);
  
  selectAllButton = new QPushButton(tr("Set all selected"), this);
  deselectAllButton = new QPushButton(tr("Set all unselected"), this);
  saveCloseButton = new QPushButton(tr("Inherit selected attributes"), this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);

  connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));
  connect(deselectAllButton, SIGNAL(clicked()), this, SLOT(deselectAll()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));

  // Lets' build the interface now
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  QPointer<QVBoxLayout> checkBoxLayout = new QVBoxLayout;
  QVectorIterator<QCheckBox*> it2(checkBoxVector);
  while (it2.hasNext())
    {
      QCheckBox* current = it2.next();
      checkBoxLayout->addWidget(current);
    }
  checkBoxWidget->setLayout(checkBoxLayout);
  scrollArea->setWidget(checkBoxWidget);
  mainLayout->addWidget(scrollArea);

  QPointer<QFrame> sepLine = new QFrame;
  sepLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(sepLine);
  
  QPointer<QHBoxLayout> selectionLayout = new QHBoxLayout;
  selectionLayout->addWidget(selectAllButton);
  selectionLayout->addWidget(deselectAllButton);
  mainLayout->addLayout(selectionLayout);

  QPointer<QFrame> sepLineTwo = new QFrame;
  sepLineTwo->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(sepLineTwo);

  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(saveCloseButton);
  optionsLayout->addWidget(cancelCloseButton);
  mainLayout->addLayout(optionsLayout);

  setMaximumHeight(600);
  setMinimumWidth(500);
  
  // Final steps
  setLayout(mainLayout);
  setWindowTitle("Set attribute inheritance");

}

InheritanceDialog::~InheritanceDialog()
{
  qDeleteAll(checkBoxVector);
  checkBoxVector.clear();
}

void InheritanceDialog::selectAll()
{
  QVectorIterator<QCheckBox*> it(checkBoxVector);
  while (it.hasNext())
    {
      QCheckBox *current = it.next();
      current->setCheckState(Qt::Checked);
    }
}

void InheritanceDialog::deselectAll()
{
  QVectorIterator<QCheckBox*> it(checkBoxVector);
  while (it.hasNext())
    {
      QCheckBox *current = it.next();
      current->setCheckState(Qt::Unchecked);
    }
}

void InheritanceDialog::saveAndClose()
{
  // I had to develop a somewhat messy way to fetch the results
  // because Qt automatically adds ampersands to the texts of
  // check boxes.
  for (QVector<QCheckBox*>::size_type i = 0; i != checkBoxVector.size(); i++)
    {
      QCheckBox *current = checkBoxVector[i];
      if (current->checkState() == Qt::Checked)
	{

	  _selectedAttributes.push_back(_selection[i]);
	}
    }
  _exitStatus = 0;
  this->close();
}

void InheritanceDialog::cancelAndClose()
{
  _selectedAttributes.clear();
  _exitStatus = 1;
  this->close();
}

int InheritanceDialog::getExitStatus()
{
  return _exitStatus;
}

QVector<QString> InheritanceDialog::getSelected()
{
  return _selectedAttributes;
}
