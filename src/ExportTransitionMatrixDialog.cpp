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

#include "../include/ExportTransitionMatrixDialog.h"

ExportTransitionMatrixDialog::ExportTransitionMatrixDialog(QWidget *parent) : QDialog(parent) 
{
  _exitStatus = 1;

  typeLabel = new QLabel(tr("<b>Matrix type:</b>"), this);
  probLabel = new QLabel(tr("<b>Values type:</b>"), this);

  modesButton = new QPushButton(tr("Modes based"), this);
  modesButton->setCheckable(true);
  modesButton->setChecked(true);
  modesButton->setToolTip(breakString("The values of the transition matrix will be based only on "
				      "how often transitions between events of a given mode "
				      "are observed. "));
  attributesButton = new QPushButton(tr("Attributes based"), this);
  attributesButton->setCheckable(true);
  attributesButton->setChecked(false);
  attributesButton->setToolTip(breakString("The values of the transition matrix will be based "
					   "on the number of times transitions are observed "
					   "between events that have one of the attributes in the "
					   "list of modes assigned to them."));
  rawButton = new QPushButton(tr("Raw values"), this);
  rawButton->setCheckable(true);
  rawButton->setChecked(true);
  rawButton->setToolTip(breakString("The values of the transition matrix will be raw counts."));
  probButton = new QPushButton(tr("Probabilities"), this);
  probButton->setCheckable(true);
  probButton->setChecked(false);
  probButton->setToolTip(breakString("The values of the transition matrix will be raw counts "
				     "divided by the row marginals."));
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(modesButton, SIGNAL(clicked()), this, SLOT(setModes()));
  connect(attributesButton, SIGNAL(clicked()), this, SLOT(setAttributes()));
  connect(rawButton, SIGNAL(clicked()), this, SLOT(setRaw()));
  connect(probButton, SIGNAL(clicked()), this, SLOT(setProb()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> typeLayout = new QHBoxLayout;
  typeLayout->addWidget(typeLabel);
  typeLayout->addWidget(modesButton);
  typeLayout->addWidget(attributesButton);
  mainLayout->addLayout(typeLayout);
  QPointer<QHBoxLayout> valueLayout = new QHBoxLayout;
  valueLayout->addWidget(probLabel);
  valueLayout->addWidget(rawButton);
  valueLayout->addWidget(probButton);
  mainLayout->addLayout(valueLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Set transition matrix parameters");
}

void ExportTransitionMatrixDialog::setModes() 
{
  modesButton->setChecked(true);
  attributesButton->setChecked(false);
}

void ExportTransitionMatrixDialog::setAttributes() 
{
  attributesButton->setChecked(true);
  modesButton->setChecked(false);
}

void ExportTransitionMatrixDialog::setRaw() 
{
  rawButton->setChecked(true);
  probButton->setChecked(false);
}

void ExportTransitionMatrixDialog::setProb() 
{
  probButton->setChecked(true);
  rawButton->setChecked(false);
}

bool ExportTransitionMatrixDialog::isMode() 
{
  return modesButton->isChecked();
}

bool ExportTransitionMatrixDialog::isProbability() 
{
  return probButton->isChecked();
}

int ExportTransitionMatrixDialog::getExitStatus() 
{
  return _exitStatus;
}

void ExportTransitionMatrixDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void ExportTransitionMatrixDialog::saveAndClose() 
{
  _exitStatus = 0;
  this->close();
}
