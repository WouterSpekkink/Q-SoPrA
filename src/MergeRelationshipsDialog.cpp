#include "../include/MergeRelationshipsDialog.h"

MergeRelationshipsDialog::MergeRelationshipsDialog(QWidget *parent,
						   QVector<DirectedEdge*> *directed,
						   QVector<UndirectedEdge*> *undirected,
						   QVector<QString> submittedRelationships,
						   QVector<QString> submittedDirections)
  : QDialog(parent) 
{
  
  pDirected = directed;
  pUndirected = undirected;
  directions = submittedDirections;
  name = "";
  description = "";
  directedness = "";
  exitStatus = 1;  

  relationshipsLabel = new QLabel(tr("<b>Relationships:</b>"), this);
  nameLabel = new QLabel(tr("<b>Merged relationship name:</b>"), this);
  descriptionLabel = new QLabel(tr("<b>Merged relationship description:</b>"), this);
  
  nameField = new QLineEdit();

  descriptionField = new QTextEdit();

  QVectorIterator<QString> it(submittedRelationships);
  while (it.hasNext()) 
    {
      QString currentRel = it.next();
      QPointer<QCheckBox> checkBox = new QCheckBox(currentRel, this);
      relationships.push_back(checkBox);
    }
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save new relationship"), this);
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(relationshipsLabel);
  QVectorIterator<QPointer<QCheckBox>> it2(relationships);
  while (it2.hasNext()) 
    {
      mainLayout->addWidget(it2.next()); 
    }
  
  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);

  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  mainLayout->addWidget(descriptionLabel);
  mainLayout->addWidget(descriptionField);

  QPointer<QFrame> middleLine = new QFrame;
  middleLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(middleLine);

  QPointer<QHBoxLayout> optionLayout = new QHBoxLayout;
  optionLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionLayout);

  setLayout(mainLayout);
  setWindowTitle("Merge relationships");
  setFixedHeight(sizeHint().height());
}

QVector<QString> MergeRelationshipsDialog::getTypes() 
{
  return types;
}

QString MergeRelationshipsDialog::getName() 
{
  return name;
}

QString MergeRelationshipsDialog::getDescription() 
{
  return description;
}

QString MergeRelationshipsDialog::getDirectedness() 
{
  return directedness;
}

int MergeRelationshipsDialog::getExitStatus() 
{
  return exitStatus;
}

void MergeRelationshipsDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

// TO DO: Check for attributes with same name.
void MergeRelationshipsDialog::saveAndClose() 
{
  description =  descriptionField->toPlainText().trimmed();
  name = nameField->text().trimmed();
  QVectorIterator<QPointer<QCheckBox>> it(relationships);
  int checked = 0;
  while (it.hasNext()) 
    {
      QPointer<QCheckBox> current = it.next();
      if (current->isChecked()) 
	{
	  checked++;
	}
    }
  if (checked < 2) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Less than 2 relationships checked.");
      warningBox->setInformativeText("You have to check at least 2 relationships.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  bool directedFound = false;
  bool undirectedFound = false;
  for (int i = 0; i != relationships.size(); i++) 
    {
      QPointer<QCheckBox> current = relationships[i];
      if (current->isChecked()) 
	{
	  if (directions[i] == DIRECTED) 
	    {
	      directedFound = true;
	    }
	  else if (directions[i] == UNDIRECTED) 
	    {
	      undirectedFound = true;
	    }
	}
    }
  if (directedFound && undirectedFound) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Directedness is different.");
      warningBox->setInformativeText("You cannot merge relationships with a different directedness.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else if (directedFound) 
    {
      directedness = DIRECTED;
    }
  else if (undirectedFound) 
    {
      directedness = UNDIRECTED;
    }
  if (description == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Description required.");
      warningBox->setInformativeText("A relationship type requires a description.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (name == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name required.");
      warningBox->setInformativeText("A relationship type requires a name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  bool found = false;
  QVectorIterator<DirectedEdge*> it2(*pDirected);
  while (it2.hasNext()) 
    {
      DirectedEdge* directed = it2.next();
      if (directed->getType() == name) 
	{
	  found = true;
	}
    }
  QVectorIterator<UndirectedEdge*> it3(*pUndirected);
  while (it3.hasNext()) 
    {
      UndirectedEdge* undirected = it3.next();
      if (undirected->getType() == name) 
	{
	  found = true;
	}
    }
  if (found) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Name already exists.");
      warningBox->setInformativeText("You already have a relationship type with that name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  QVectorIterator<QPointer<QCheckBox>> it4(relationships);
  while (it4.hasNext()) 
    {
      QPointer<QCheckBox> current = it4.next();
      if (current->isChecked()) 
	{
	  QString currentText = removeChar(current->text(), '&');
	  types.push_back(currentText);
	}
    }
  exitStatus = 0;
  this->close();
}



