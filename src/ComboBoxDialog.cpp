#include "../include/ComboBoxDialog.h"

ComboBoxDialog::ComboBoxDialog(QWidget *parent, QVector<QString> contents) : QDialog(parent) {
  exitStatus = 1;

  selectionLabel = new QLabel(tr("<b>Choose:</b>"), this);

  selectionComboBox = new QComboBox(this);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Select"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(selectionLabel);
  mainLayout->addWidget(selectionComboBox);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  QVectorIterator<QString> it(contents);
  while (it.hasNext()) {
    QString current = it.next();
    selectionComboBox->addItem(current);
  }
}

void ComboBoxDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void ComboBoxDialog::saveAndClose() {
  selection = selectionComboBox->currentText();
  if (selection == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("No selection made.");
    warningBox->setInformativeText("You have to make a selection to proceed.");
    warningBox->exec();
    delete warningBox;
    return;
  } else {
    exitStatus = 0;
    this->close();
  }
}

QString ComboBoxDialog::getSelection() {
  return selection;
}

int ComboBoxDialog::getExitStatus() {
  return exitStatus;
}

				    
