#include "../include/AttributeIndexDialog.h"

AttributeIndexDialog::AttributeIndexDialog(QWidget *parent, int max) : QDialog(parent) 
{
  // First we construct our dialog's entities.
  exitStatus = 1;
  indexLabel = new QLabel(tr("Incident index:"), this);
  indexField = new QLineEdit(this);
  index = 1;
  indexField->setValidator(new QIntValidator(1, max, this)); 
  goButton = new QPushButton(tr("Go"), this);
  goButton->setEnabled(false); // We only activate this button if there is something in the fields.
  cancelButton = new QPushButton(tr("Cancel"), this);

  // We connect the signals to the appropriate slots.
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
  connect(indexField, SIGNAL(textChanged(const QString &)), this, SLOT(setIndexText(const QString &)));
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> fieldLayout = new QHBoxLayout;
  fieldLayout->addWidget(indexLabel);
  fieldLayout->addWidget(indexField);

  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(goButton);
  buttonLayout->addWidget(cancelButton);
  
  mainLayout->addLayout(fieldLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("Jump to index"));
  setFixedHeight(sizeHint().height());
  // And this finished the constructor.
}

void AttributeIndexDialog::setIndexText(const QString newIndex) 
{
  QString indexText = newIndex.trimmed();
  // Let us immediately check if we should active the go Button.
  if (indexText != "") 
    {
      goButton->setEnabled(true); 
    }
  else 
    {
      goButton->setEnabled(false);
    }
  index = indexText.toInt();
}

void AttributeIndexDialog::go() 
{
  exitStatus = 0;
  this->close();
}

void AttributeIndexDialog::cancel() 
{
  exitStatus = 1;
  this->close();
}

int AttributeIndexDialog::getIndex() 
{
  return index;
}

int AttributeIndexDialog::getExitStatus() 
{
  return exitStatus;
}

