#include "../include/ComboBoxDialog.h"

ComboBoxDialog::ComboBoxDialog(QWidget *parent) : QDialog(parent) {
  exitStatus = 1;

  typesLabel = new QLabel(tr("<b>Choose type:</b>"), this);

  typesComboBox = new QComboBox(this);
  getTypes();

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(typesLabel);
  mainLayout->addWidget(typesComboBox);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);
  setWindowTitle("Switch relationship type");
}

void ComboBoxDialog::getTypes() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM relationship_types ORDER BY name ASC");
  while (query->next()) {
    QString current = query->value(0).toString();
    typesComboBox->addItem(current);
  }
  delete query;
}

void ComboBoxDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void ComboBoxDialog::saveAndClose() {
  chosenType = typesComboBox->currentText();
  if (chosenType == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("No type chosen.");
    warningBox->setInformativeText("You have to choose a type to proceed.");
    warningBox->exec();
    delete warningBox;
    return;
  } else {
    exitStatus = 0;
    this->close();
  }
}

QString ComboBoxDialog::getType() {
  return chosenType;
}

int ComboBoxDialog::getExitStatus() {
  return exitStatus;
}

				    
