#include "../include/SimpleTextDialog.h"

SimpleTextDialog::SimpleTextDialog(QWidget *parent) : QDialog(parent) 
{
  text = "";
  subText = "";
  exitStatus = 1;
  textLabel = new QLabel(tr("<b>Text:</b>"), this);
  textField = new QLineEdit(this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> textLayout = new QHBoxLayout;
  textLayout->addWidget(textLabel);
  textLayout->addWidget(textField);
  mainLayout->addLayout(textLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionsLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Add / Edit Coder");
}


QString SimpleTextDialog::getText() 
{
  return text;
}

void SimpleTextDialog::submitText(const QString &submittedText) 
{
  text = submittedText;
  subText = submittedText;
  textField->setText(submittedText);
}

void SimpleTextDialog::setLabel(const QString &submittedLabel) 
{
  textLabel->setText(submittedLabel);
}

int SimpleTextDialog::getExitStatus() 
{
  return exitStatus;
}

void SimpleTextDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void SimpleTextDialog::saveAndClose() 
{
  text = textField->text().trimmed();
  if (text == "") 
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

