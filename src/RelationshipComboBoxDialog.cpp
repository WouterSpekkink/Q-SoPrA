#include "../include/RelationshipComboBoxDialog.h"

RelationshipComboBoxDialog::RelationshipComboBoxDialog(QWidget *parent) : QDialog(parent) 
{
  exitStatus = 1;
  tail = true;
  head = true;
  selectionLabel = new QLabel(tr("<b>Choose:</b>"), this);

  selectionComboBox = new QComboBox(this);

  includeTailCheckBox = new QCheckBox(tr("Include tails"), this);
  includeTailCheckBox->setChecked(true);
  includeHeadCheckBox = new QCheckBox(tr("Include heads"), this);
  includeHeadCheckBox->setChecked(true);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Select"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(selectionComboBox, SIGNAL(currentTextChanged(const QString&)),
	  this, SLOT(setControls(const QString&)));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(selectionLabel);
  mainLayout->addWidget(selectionComboBox);
  mainLayout->addWidget(includeTailCheckBox);
  mainLayout->addWidget(includeHeadCheckBox);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  getRelationships();
}

void RelationshipComboBoxDialog::getRelationships() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM relationship_types ORDER BY name");
  while (query->next()) 
    {
      QString relationship = query->value(0).toString();
      selectionComboBox->addItem(relationship);
    }
  delete query;
}

void RelationshipComboBoxDialog::setControls(const QString &selection) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT directedness FROM relationship_types "
		 "WHERE name = :selection");
  query->bindValue(":selection", selection);
  query->exec();
  query->first();
  QString direction = query->value(0).toString();
  if (direction == UNDIRECTED) 
    {
      includeTailCheckBox->setChecked(true);
      includeTailCheckBox->setEnabled(false);
      includeHeadCheckBox->setChecked(true);
      includeHeadCheckBox->setEnabled(false);
    }
  else if (direction == DIRECTED) 
    {
      includeTailCheckBox->setChecked(true);
      includeTailCheckBox->setEnabled(true);
      includeHeadCheckBox->setChecked(true);
      includeHeadCheckBox->setEnabled(true);
    }
  delete query;
}

void RelationshipComboBoxDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void RelationshipComboBoxDialog::saveAndClose() 
{
  selection = selectionComboBox->currentText();
  if (selection == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No relationship selected.");
      warningBox->setInformativeText("You have to select a relationship to proceed.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (!includeTailCheckBox->isChecked() && !includeHeadCheckBox->isChecked()) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Tail and head both unchecked.");
      warningBox->setInformativeText("The current settings will not have any effect.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  tail = includeTailCheckBox->isChecked();
  head = includeHeadCheckBox->isChecked();
  exitStatus = 0;
  this->close();
}

QString RelationshipComboBoxDialog::getSelection() 
{
  return selection;
}

int RelationshipComboBoxDialog::getExitStatus() 
{
  return exitStatus;
}

bool RelationshipComboBoxDialog::tailSelected() 
{
  return tail;
}

bool RelationshipComboBoxDialog::headSelected() 
{
  return head;
}
				    
