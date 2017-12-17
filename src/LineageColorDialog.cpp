#include "../include/LineageColorDialog.h"

LineageColorDialog::LineageColorDialog(QWidget *parent) : QDialog(parent) {
  exitStatus = 1;
  originColor = QColor(255, 165, 0);
  ancestorColor = QColor(179, 225, 172);
  descendantColor = QColor(0, 75, 255);
  unrelatedColor = QColor(255, 255, 255);
  
  lineageLabel = new QLabel(tr("Choose lineage colors:"), this);

  originButton = new QPushButton(tr("Choose origin color"), this);
  ancestorButton = new QPushButton(tr("Choose ancestors color"), this);
  descendantButton = new QPushButton(tr("Choose descendants color"), this);
  unrelatedButton = new QPushButton(tr("Choose unrelated color"), this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  QPalette pal = originButton->palette();
  pal.setColor(QPalette::Button, originColor);
  originButton->setAutoFillBackground(true);
  originButton->setPalette(pal);
  originButton->update();
  pal = ancestorButton->palette();
  pal.setColor(QPalette::Button, ancestorColor);
  ancestorButton->setAutoFillBackground(true);
  ancestorButton->setPalette(pal);
  ancestorButton->update();
  pal = descendantButton->palette();
  pal.setColor(QPalette::Button, descendantColor);
  descendantButton->setAutoFillBackground(true);
  descendantButton->setPalette(pal);
  descendantButton->update();
  pal = unrelatedButton->palette();
  pal.setColor(QPalette::Button, unrelatedColor);
  unrelatedButton->setAutoFillBackground(true);
  unrelatedButton->setPalette(pal);
  unrelatedButton->update();
  
  connect(originButton, SIGNAL(clicked()), this, SLOT(setOriginColor()));
  connect(ancestorButton, SIGNAL(clicked()), this, SLOT(setAncestorColor()));
  connect(descendantButton, SIGNAL(clicked()), this, SLOT(setDescendantColor()));
  connect(unrelatedButton, SIGNAL(clicked()), this, SLOT(setUnrelatedColor()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(lineageLabel);
  mainLayout->addWidget(originButton);
  mainLayout->addWidget(ancestorButton);
  mainLayout->addWidget(descendantButton);
  mainLayout->addWidget(unrelatedButton);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setWindowTitle("Create lineage");
}

void LineageColorDialog::setOriginColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    originColor = colorDialog->selectedColor();
  }
  QPalette pal = originButton->palette();
  pal.setColor(QPalette::Button, originColor);
  originButton->setAutoFillBackground(true);
  originButton->setPalette(pal);
  originButton->update();
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setAncestorColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    ancestorColor = colorDialog->selectedColor();
  }
  QPalette pal = ancestorButton->palette();
  pal.setColor(QPalette::Button, ancestorColor);
  ancestorButton->setAutoFillBackground(true);
  ancestorButton->setPalette(pal);
  ancestorButton->update();
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setDescendantColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    descendantColor = colorDialog->selectedColor();
  }
  QPalette pal = descendantButton->palette();
  pal.setColor(QPalette::Button, descendantColor);
  descendantButton->setAutoFillBackground(true);
  descendantButton->setPalette(pal);
  descendantButton->update();
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::setUnrelatedColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    unrelatedColor = colorDialog->selectedColor();
  }
  QPalette pal = unrelatedButton->palette();
  pal.setColor(QPalette::Button, unrelatedColor);
  unrelatedButton->setAutoFillBackground(true);
  unrelatedButton->setPalette(pal);
  unrelatedButton->update();
  saveCloseButton->setFocus();
  delete colorDialog;
}

void LineageColorDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void LineageColorDialog::saveAndClose() {
  exitStatus = 0;
  this->close();
}

QColor LineageColorDialog::getOriginColor() {
  return originColor;
}

QColor LineageColorDialog::getAncestorColor() {
  return ancestorColor;
}

QColor LineageColorDialog::getDescendantColor() {
  return descendantColor;
}

QColor LineageColorDialog::getUnrelatedColor() {
  return unrelatedColor;
}

int LineageColorDialog::getExitStatus() {
  return exitStatus;
}

