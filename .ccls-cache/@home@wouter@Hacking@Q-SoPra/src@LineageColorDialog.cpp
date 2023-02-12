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

#include "../include/LineageColorDialog.h"

LineageColorDialog::LineageColorDialog(QWidget *parent) : QDialog(parent) 
{
  _exitStatus = 1;
  _originFillColor = QColor(255, 165, 0);
  _ancestorFillColor = QColor(179, 225, 172);
  _descendantFillColor = QColor(0, 75, 255);
  _unrelatedFillColor = QColor(255, 255, 255);
  _originTextColor = QColor(0, 0, 0);
  _ancestorTextColor = QColor(0, 0, 0);
  _descendantTextColor = QColor(0, 0, 0);
  _unrelatedTextColor = QColor(0, 0, 0);
  
  lineageLabel = new QLabel(tr("<b>Choose lineage colors:</b>"), this);

  originFillButton = new QPushButton(tr("Choose origin fill color"), this);
  QPixmap pixmapOneA(originFillButton->width(), originFillButton->height());
  pixmapOneA.fill(_originFillColor);
  fillColorOneLabel = new QLabel(this);
  fillColorOneLabel->setMaximumWidth(50);
  fillColorOneLabel->setMaximumHeight(fillColorOneLabel->sizeHint().height());
  fillColorOneLabel->setPixmap(pixmapOneA);
  originTextButton = new QPushButton(tr("Choose origin text color"), this);
  QPixmap pixmapOneB(originTextButton->width(), originTextButton->height());
  pixmapOneB.fill(_originTextColor);
  textColorOneLabel = new QLabel(this);
  textColorOneLabel->setMaximumWidth(50);
  textColorOneLabel->setMaximumHeight(textColorOneLabel->sizeHint().height());
  textColorOneLabel->setPixmap(pixmapOneB);
  ancestorFillButton = new QPushButton(tr("Choose ancestors fill color"), this);
  QPixmap pixmapTwoA(ancestorFillButton->width(), ancestorFillButton->height());
  pixmapTwoA.fill(_ancestorFillColor);
  fillColorTwoLabel = new QLabel(this);
  fillColorTwoLabel->setMaximumWidth(50);
  fillColorTwoLabel->setMaximumHeight(fillColorTwoLabel->sizeHint().height());
  fillColorTwoLabel->setPixmap(pixmapTwoA);
  ancestorTextButton = new QPushButton(tr("Choose ancestors text color"), this);
  QPixmap pixmapTwoB(ancestorTextButton->width(), ancestorTextButton->height());
  pixmapTwoB.fill(_ancestorTextColor);
  textColorTwoLabel = new QLabel(this);
  textColorTwoLabel->setMaximumWidth(50);
  textColorTwoLabel->setMaximumHeight(textColorTwoLabel->sizeHint().height());
  textColorTwoLabel->setPixmap(pixmapTwoB);
  descendantFillButton = new QPushButton(tr("Choose descendants fill color"), this);
  QPixmap pixmapThreeA(descendantFillButton->width(), descendantFillButton->height());
  pixmapThreeA.fill(_descendantFillColor);
  fillColorThreeLabel = new QLabel(this);
  fillColorThreeLabel->setMaximumWidth(50);
  fillColorThreeLabel->setMaximumHeight(fillColorThreeLabel->sizeHint().height());
  fillColorThreeLabel->setPixmap(pixmapThreeA);
  descendantTextButton = new QPushButton(tr("Choose descendants text color"), this);
  QPixmap pixmapThreeB(descendantTextButton->width(), descendantTextButton->height());
  pixmapThreeB.fill(_descendantTextColor);
  textColorThreeLabel = new QLabel(this);
  textColorThreeLabel->setMaximumWidth(50);
  textColorThreeLabel->setMaximumHeight(textColorThreeLabel->sizeHint().height());
  textColorThreeLabel->setPixmap(pixmapThreeB);
  unrelatedFillButton = new QPushButton(tr("Choose unrelated fill color"), this);
  QPixmap pixmapFourA(unrelatedFillButton->width(), unrelatedFillButton->height());
  pixmapFourA.fill(_unrelatedFillColor);
  fillColorFourLabel = new QLabel(this);
  fillColorFourLabel->setMaximumWidth(50);
  fillColorFourLabel->setMaximumHeight(fillColorFourLabel->sizeHint().height());
  fillColorFourLabel->setPixmap(pixmapFourA);
  unrelatedTextButton = new QPushButton(tr("Choose unrelated text color"), this);
  QPixmap pixmapFourB(unrelatedTextButton->width(), unrelatedTextButton->height());
  pixmapFourB.fill(_unrelatedTextColor);
  textColorFourLabel = new QLabel(this);
  textColorFourLabel->setMaximumWidth(50);
  textColorFourLabel->setMaximumHeight(textColorFourLabel->sizeHint().height());
  textColorFourLabel->setPixmap(pixmapFourB);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  modesCheckBox = new QCheckBox(tr("Save as modes"), this);
  
  connect(originFillButton, SIGNAL(clicked()), this, SLOT(setOriginFillColor()));
  connect(ancestorFillButton, SIGNAL(clicked()), this, SLOT(setAncestorFillColor()));
  connect(descendantFillButton, SIGNAL(clicked()), this, SLOT(setDescendantFillColor()));
  connect(unrelatedFillButton, SIGNAL(clicked()), this, SLOT(setUnrelatedFillColor()));
  connect(originTextButton, SIGNAL(clicked()), this, SLOT(setOriginTextColor()));
  connect(ancestorTextButton, SIGNAL(clicked()), this, SLOT(setAncestorTextColor()));
  connect(descendantTextButton, SIGNAL(clicked()), this, SLOT(setDescendantTextColor()));
  connect(unrelatedTextButton, SIGNAL(clicked()), this, SLOT(setUnrelatedTextColor()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(lineageLabel);
  QPointer<QHBoxLayout> dualLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> colorOneLeftLayout = new QHBoxLayout;
  colorOneLeftLayout->addWidget(originFillButton);
  colorOneLeftLayout->addWidget(fillColorOneLabel);
  leftLayout->addLayout(colorOneLeftLayout);
  QPointer<QHBoxLayout> colorTwoLeftLayout = new QHBoxLayout;
  colorTwoLeftLayout->addWidget(ancestorFillButton);
  colorTwoLeftLayout->addWidget(fillColorTwoLabel);
  leftLayout->addLayout(colorTwoLeftLayout);
  QPointer<QHBoxLayout> colorThreeLeftLayout = new QHBoxLayout;
  colorThreeLeftLayout->addWidget(descendantFillButton);
  colorThreeLeftLayout->addWidget(fillColorThreeLabel);
  leftLayout->addLayout(colorThreeLeftLayout);
  QPointer<QHBoxLayout> colorFourLeftLayout = new QHBoxLayout;
  colorFourLeftLayout->addWidget(unrelatedFillButton);
  colorFourLeftLayout->addWidget(fillColorFourLabel);
  leftLayout->addLayout(colorFourLeftLayout);
  dualLayout->addLayout(leftLayout);
  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> colorOneRightLayout = new QHBoxLayout;
  colorOneRightLayout->addWidget(originTextButton);
  colorOneRightLayout->addWidget(textColorOneLabel);
  rightLayout->addLayout(colorOneRightLayout);
  QPointer<QHBoxLayout> colorTwoRightLayout = new QHBoxLayout;
  colorTwoRightLayout->addWidget(ancestorTextButton);
  colorTwoRightLayout->addWidget(textColorTwoLabel);
  rightLayout->addLayout(colorTwoRightLayout);
  QPointer<QHBoxLayout> colorThreeRightLayout = new QHBoxLayout;
  colorThreeRightLayout->addWidget(descendantTextButton);
  colorThreeRightLayout->addWidget(textColorThreeLabel);
  rightLayout->addLayout(colorThreeRightLayout);
  QPointer<QHBoxLayout> colorFourRightLayout = new QHBoxLayout;
  colorFourRightLayout->addWidget(unrelatedTextButton);
  colorFourRightLayout->addWidget(textColorFourLabel);
  rightLayout->addLayout(colorFourRightLayout);
  dualLayout->addLayout(rightLayout);
  mainLayout->addLayout(dualLayout);
  mainLayout->addWidget(modesCheckBox);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setWindowTitle("Create lineage");
}

void LineageColorDialog::setOriginFillColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _originFillColor = colorDialog->selectedColor();
    }
  QPixmap pixmapOneA(originFillButton->width(), originFillButton->height());
  pixmapOneA.fill(_originFillColor);
  fillColorOneLabel->setPixmap(pixmapOneA);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setOriginTextColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _originTextColor = colorDialog->selectedColor();
    }
  QPixmap pixmapOneB(originTextButton->width(), originTextButton->height());
  pixmapOneB.fill(_originTextColor);
  textColorOneLabel->setPixmap(pixmapOneB);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setAncestorFillColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _ancestorFillColor = colorDialog->selectedColor();
    }
  QPixmap pixmapTwoA(ancestorFillButton->width(), ancestorFillButton->height());
  pixmapTwoA.fill(_ancestorFillColor);
  fillColorTwoLabel->setPixmap(pixmapTwoA);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setAncestorTextColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _ancestorTextColor = colorDialog->selectedColor();
    }
  QPixmap pixmapTwoB(ancestorTextButton->width(), ancestorTextButton->height());
  pixmapTwoB.fill(_ancestorTextColor);
  textColorTwoLabel->setPixmap(pixmapTwoB);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setDescendantFillColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _descendantFillColor = colorDialog->selectedColor();
    }
  QPixmap pixmapThreeA(descendantFillButton->width(), descendantFillButton->height());
  pixmapThreeA.fill(_descendantFillColor);
  fillColorThreeLabel->setPixmap(pixmapThreeA);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setDescendantTextColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _descendantTextColor = colorDialog->selectedColor();
    }
  QPixmap pixmapThreeB(descendantTextButton->width(), descendantTextButton->height());
  pixmapThreeB.fill(_descendantTextColor);
  textColorThreeLabel->setPixmap(pixmapThreeB);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setUnrelatedFillColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _unrelatedFillColor = colorDialog->selectedColor();
    }
  QPixmap pixmapFourA(unrelatedFillButton->width(), unrelatedFillButton->height());
  pixmapFourA.fill(_unrelatedFillColor);
  fillColorFourLabel->setPixmap(pixmapFourA);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setUnrelatedTextColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      _unrelatedTextColor = colorDialog->selectedColor();
    }
  QPixmap pixmapFourB(unrelatedTextButton->width(), unrelatedTextButton->height());
  pixmapFourB.fill(_unrelatedTextColor);
  textColorFourLabel->setPixmap(pixmapFourB);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void LineageColorDialog::saveAndClose() 
{
  _exitStatus = 0;
  this->close();
}

QColor LineageColorDialog::getOriginFillColor() 
{
  return _originFillColor;
}


QColor LineageColorDialog::getOriginTextColor() 
{
  return _originTextColor;
}

QColor LineageColorDialog::getAncestorFillColor() 
{
  return _ancestorFillColor;
}

QColor LineageColorDialog::getAncestorTextColor() 
{
  return _ancestorTextColor;
}


QColor LineageColorDialog::getDescendantFillColor() 
{
  return _descendantFillColor;
}

QColor LineageColorDialog::getDescendantTextColor() 
{
  return _descendantTextColor;
}

QColor LineageColorDialog::getUnrelatedFillColor() 
{
  return _unrelatedFillColor;
}

QColor LineageColorDialog::getUnrelatedTextColor() 
{
  return _unrelatedTextColor;
}

bool LineageColorDialog::modesOn()
{
  return modesCheckBox->isChecked();
}

int LineageColorDialog::getExitStatus() 
{
  return _exitStatus;
}

