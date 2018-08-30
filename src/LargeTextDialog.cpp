#include "../include/LargeTextDialog.h"

LargeTextDialog::LargeTextDialog(QWidget *parent) : QDialog(parent) 
{
  text = "";
  subText = "";
  exitStatus = 1;
  emptyAllowed = false;
  textLabel = new QLabel(tr("<b>Text:</b>"), this);
  textField = new QTextEdit(this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(textLabel);
  mainLayout->addWidget(textField);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionsLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Add / Edit Coder");
}


QString LargeTextDialog::getText() 
{
  return text;
}

void LargeTextDialog::submitText(const QString &submittedText) 
{
  text = submittedText;
  subText = submittedText;
  textField->setText(submittedText);
}

void LargeTextDialog::setLabel(const QString &submittedLabel) 
{
  textLabel->setText(submittedLabel);
}

int LargeTextDialog::getExitStatus() 
{
  return exitStatus;
}

void LargeTextDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void LargeTextDialog::setEmptyAllowed(bool status) 
{
  emptyAllowed = status;
}

void LargeTextDialog::saveAndClose() 
{
  text = textField->toPlainText().trimmed();
  if (text == "" && !emptyAllowed) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Text required.");
      warningBox->setInformativeText("You need to enter text to save.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  exitStatus = 0;
  this->close();
}

