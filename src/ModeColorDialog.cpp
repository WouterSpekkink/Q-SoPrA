#include "../include/ModeColorDialog.h"

ModeColorDialog::ModeColorDialog(QWidget *parent,
				 QColor submittedFillColor,
				 QColor submittedTextColor) : QDialog(parent)
{
  exitStatus = 1;
  fillColor = submittedFillColor;
  textColor = submittedTextColor;

  infoLabel = new QLabel(tr("<b>Choose new colors:</b>"), this);
  fillColorButton = new QPushButton(tr("Choose fill color"), this);
  QPixmap pixmapOne(fillColorButton->width(), fillColorButton->height());
  pixmapOne.fill(fillColor);
  fillColorLabel = new QLabel(this);
  fillColorLabel->setMaximumWidth(50);
  fillColorLabel->setMaximumHeight(fillColorLabel->sizeHint().height());
  fillColorLabel->setPixmap(pixmapOne);
  textColorButton = new QPushButton(tr("Choose text color"), this);
  QPixmap pixmapTwo(textColorButton->width(), textColorButton->height());
  pixmapTwo.fill(textColor);
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
      fillColor = colorDialog->selectedColor();
    }
  QPixmap pixmapOne(fillColorButton->width(), fillColorButton->height());
  pixmapOne.fill(fillColor);
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
      textColor = colorDialog->selectedColor();
    }
  QPixmap pixmapTwo(textColorButton->width(), textColorButton->height());
  pixmapTwo.fill(textColor);
  textColorLabel->setPixmap(pixmapTwo);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void ModeColorDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void ModeColorDialog::saveAndClose() 
{
  exitStatus = 0;
  this->close();
}

QColor ModeColorDialog::getFillColor() 
{
  return fillColor;
}


QColor ModeColorDialog::getTextColor() 
{
  return textColor;
}

int ModeColorDialog::getExitStatus()
{
  return exitStatus;
}


