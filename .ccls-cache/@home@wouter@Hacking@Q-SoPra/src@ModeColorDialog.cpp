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

#include "../include/ModeColorDialog.h"

ModeColorDialog::ModeColorDialog(QWidget *parent,
				 QColor fillColor,
				 QColor textColor) : QDialog(parent)
{
  _exitStatus = 1;
  _fillColor = fillColor;
  _textColor = textColor;

  infoLabel = new QLabel(tr("<b>Choose new colors:</b>"), this);
  fillColorButton = new QPushButton(tr("Choose fill color"), this);
  QPixmap pixmapOne(fillColorButton->width(), fillColorButton->height());
  pixmapOne.fill(_fillColor);
  fillColorLabel = new QLabel(this);
  fillColorLabel->setMaximumWidth(50);
  fillColorLabel->setMaximumHeight(fillColorLabel->sizeHint().height());
  fillColorLabel->setPixmap(pixmapOne);
  textColorButton = new QPushButton(tr("Choose text color"), this);
  QPixmap pixmapTwo(textColorButton->width(), textColorButton->height());
  pixmapTwo.fill(_textColor);
  textColorLabel = new QLabel(this);
  textColorLabel->setMaximumWidth(50);
  textColorLabel->setMaximumHeight(textColorLabel->sizeHint().height());
  textColorLabel->setPixmap(pixmapTwo);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(fillColorButton, SIGNAL(clicked()), this, SLOT(setFillColor()));
  connect(textColorButton, SIGNAL(clicked()), this, SLOT(setTextColor()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(infoLabel);
  QPointer<QHBoxLayout> fillColorLayout = new QHBoxLayout;
  fillColorLayout->addWidget(fillColorButton);
  fillColorLayout->addWidget(fillColorLabel);
  mainLayout->addLayout(fillColorLayout);
  QPointer<QHBoxLayout> textColorLayout = new QHBoxLayout;
  textColorLayout->addWidget(textColorButton);
  textColorLayout->addWidget(textColorLabel);
  mainLayout->addLayout(textColorLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout); 
  setWindowTitle("Set new mode colors");
}

void ModeColorDialog::setFillColor()
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _fillColor = colorDialog->selectedColor();
    }
  QPixmap pixmapOne(fillColorButton->width(), fillColorButton->height());
  pixmapOne.fill(_fillColor);
  fillColorLabel->setPixmap(pixmapOne);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void ModeColorDialog::setTextColor()
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _textColor = colorDialog->selectedColor();
    }
  QPixmap pixmapTwo(textColorButton->width(), textColorButton->height());
  pixmapTwo.fill(_textColor);
  textColorLabel->setPixmap(pixmapTwo);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void ModeColorDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void ModeColorDialog::saveAndClose() 
{
  _exitStatus = 0;
  this->close();
}

QColor ModeColorDialog::getFillColor() 
{
  return _fillColor;
}


QColor ModeColorDialog::getTextColor() 
{
  return _textColor;
}

int ModeColorDialog::getExitStatus()
{
  return _exitStatus;
}


