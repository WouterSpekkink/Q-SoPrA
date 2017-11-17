#include "../include/GraphTextDialog.h"

GraphTextDialog::GraphTextDialog(QWidget *parent) : QDialog(parent) {
  text = "";
  subText = "";
  exitStatus = 1;
  textLabel = new QLabel(tr("Text:"), this);
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


QString GraphTextDialog::getText() {
  return text;
}

void GraphTextDialog::submitText(const QString &submittedText) {
  text = submittedText;
  subText = submittedText;
  textField->setText(submittedText);
}

int GraphTextDialog::getExitStatus() {
  return exitStatus;
}

void GraphTextDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void GraphTextDialog::saveAndClose() {
  text = textField->text().trimmed();
  if (text == "") {
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
