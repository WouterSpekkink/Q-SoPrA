#include "../include/LineageColorDialog.h"

LineageColorDialog::LineageColorDialog(QWidget *parent) : QDialog(parent) 
{
  exitStatus = 1;
  originColor = QColor(255, 165, 0);
  ancestorColor = QColor(179, 225, 172);
  descendantColor = QColor(0, 75, 255);
  unrelatedColor = QColor(255, 255, 255);
  
  lineageLabel = new QLabel(tr("<b>Choose lineage colors:</b>"), this);

  originButton = new QPushButton(tr("Choose origin color"), this);
  QPixmap pixmapOne(originButton->width(), originButton->height());
  pixmapOne.fill(originColor);
  colorOneLabel = new QLabel(this);
  colorOneLabel->setMaximumWidth(50);
  colorOneLabel->setPixmap(pixmapOne);
  ancestorButton = new QPushButton(tr("Choose ancestors color"), this);
  QPixmap pixmapTwo(ancestorButton->width(), ancestorButton->height());
  pixmapTwo.fill(ancestorColor);
  colorTwoLabel = new QLabel(this);
  colorTwoLabel->setMaximumWidth(50);
  colorTwoLabel->setPixmap(pixmapTwo);
  descendantButton = new QPushButton(tr("Choose descendants color"), this);
  QPixmap pixmapThree(descendantButton->width(), descendantButton->height());
  pixmapThree.fill(descendantColor);
  colorThreeLabel = new QLabel(this);
  colorThreeLabel->setMaximumWidth(50);
  colorThreeLabel->setPixmap(pixmapThree);
  unrelatedButton = new QPushButton(tr("Choose unrelated color"), this);
  QPixmap pixmapFour(unrelatedButton->width(), unrelatedButton->height());
  pixmapFour.fill(unrelatedColor);
  colorFourLabel = new QLabel(this);
  colorFourLabel->setMaximumWidth(50);
  colorFourLabel->setPixmap(pixmapFour);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);
  
  connect(originButton, SIGNAL(clicked()), this, SLOT(setOriginColor()));
  connect(ancestorButton, SIGNAL(clicked()), this, SLOT(setAncestorColor()));
  connect(descendantButton, SIGNAL(clicked()), this, SLOT(setDescendantColor()));
  connect(unrelatedButton, SIGNAL(clicked()), this, SLOT(setUnrelatedColor()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(lineageLabel);
  QPointer<QHBoxLayout> colorOneLayout = new QHBoxLayout;
  colorOneLayout->addWidget(originButton);
  colorOneLayout->addWidget(colorOneLabel);
  mainLayout->addLayout(colorOneLayout);
  QPointer<QHBoxLayout> colorTwoLayout = new QHBoxLayout;
  colorTwoLayout->addWidget(ancestorButton);
  colorTwoLayout->addWidget(colorTwoLabel);
  mainLayout->addLayout(colorTwoLayout);
  QPointer<QHBoxLayout> colorThreeLayout = new QHBoxLayout;
  colorThreeLayout->addWidget(descendantButton);
  colorThreeLayout->addWidget(colorThreeLabel);
  mainLayout->addLayout(colorThreeLayout);
  QPointer<QHBoxLayout> colorFourLayout = new QHBoxLayout;
  colorFourLayout->addWidget(unrelatedButton);
  colorFourLayout->addWidget(colorFourLabel);
  mainLayout->addLayout(colorFourLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setWindowTitle("Create lineage");
}

void LineageColorDialog::setOriginColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      originColor = colorDialog->selectedColor();
    }
  QPixmap pixmapOne(originButton->width(), originButton->height());
  pixmapOne.fill(originColor);
  colorOneLabel->setPixmap(pixmapOne);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setAncestorColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      ancestorColor = colorDialog->selectedColor();
    }
  QPixmap pixmapTwo(ancestorButton->width(), ancestorButton->height());
  pixmapTwo.fill(ancestorColor);
  colorTwoLabel->setPixmap(pixmapTwo);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setDescendantColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      descendantColor = colorDialog->selectedColor();
    }
  QPixmap pixmapThree(descendantButton->width(), descendantButton->height());
  pixmapThree.fill(descendantColor);
  colorThreeLabel->setPixmap(pixmapThree);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setUnrelatedColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      unrelatedColor = colorDialog->selectedColor();
    }
  QPixmap pixmapFour(unrelatedButton->width(), unrelatedButton->height());
  pixmapFour.fill(unrelatedColor);
  colorFourLabel->setPixmap(pixmapFour);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void LineageColorDialog::saveAndClose() 
{
  exitStatus = 0;
  this->close();
}

QColor LineageColorDialog::getOriginColor() 
{
  return originColor;
}

QColor LineageColorDialog::getAncestorColor() 
{
  return ancestorColor;
}

QColor LineageColorDialog::getDescendantColor() 
{
  return descendantColor;
}

QColor LineageColorDialog::getUnrelatedColor() 
{
  return unrelatedColor;
}

int LineageColorDialog::getExitStatus() 
{
  return exitStatus;
}

