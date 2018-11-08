#include "../include/InheritanceDialog.h"

InheritanceDialog::InheritanceDialog(QWidget *parent, QSet<QString> attributes) : QDialog(parent)
{
  // First initialise some variables
  exitStatus = 1;
  
  // Let's then create our check boxes
  QList<QString> attributeList = attributes.toList();
  std::sort(attributeList.begin(), attributeList.end());
  QListIterator<QString> it(attributeList);
  while (it.hasNext())
    {
      QString currentAttribute = it.next();
      QCheckBox *currentCheckBox = new QCheckBox(currentAttribute);
      checkBoxVector.push_back(currentCheckBox);
    }

  // Then we can create our other interface elements
  titleLabel = new QLabel(tr("<b>Select attributes to inherit:</b>"), this);

  selectAllButton = new QPushButton(tr("Set all selected"), this);
  deselectAllButton = new QPushButton(tr("Set all unselected"), this);
  saveCloseButton = new QPushButton(tr("Inherit selected attributes"), this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);

  connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));
  connect(deselectAllButton, SIGNAL(clicked()), this, SLOT(deselectAll()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));

  // Lets' build the interface now
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(titleLabel);
  QVectorIterator<QCheckBox*> it2(checkBoxVector);
  while (it2.hasNext())
    {
      QCheckBox* current = it2.next();
      mainLayout->addWidget(current);
    }

  QPointer<QFrame> sepLine = new QFrame;
  sepLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(sepLine);
  
  QPointer<QHBoxLayout> selectionLayout = new QHBoxLayout;
  selectionLayout->addWidget(selectAllButton);
  selectionLayout->addWidget(deselectAllButton);
  mainLayout->addLayout(selectionLayout);

  QPointer<QFrame> sepLineTwo = new QFrame;
  sepLineTwo->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(sepLineTwo);

  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(saveCloseButton);
  optionsLayout->addWidget(cancelCloseButton);
  mainLayout->addLayout(optionsLayout);

  // Final steps
  setLayout(mainLayout);
  setWindowTitle("Set attribute inheritance");

}

InheritanceDialog::~InheritanceDialog()
{
  qDeleteAll(checkBoxVector.begin(), checkBoxVector.end());
  checkBoxVector.clear();
}

void InheritanceDialog::selectAll()
{
  QVectorIterator<QCheckBox*> it(checkBoxVector);
  while (it.hasNext())
    {
      QCheckBox *current = it.next();
      current->setCheckState(Qt::Checked);
    }
}

void InheritanceDialog::deselectAll()
{
  QVectorIterator<QCheckBox*> it(checkBoxVector);
  while (it.hasNext())
    {
      QCheckBox *current = it.next();
      current->setCheckState(Qt::Unchecked);
    }
}

void InheritanceDialog::saveAndClose()
{
  QVectorIterator<QCheckBox*> it(checkBoxVector);
  while (it.hasNext())
    {
      QCheckBox *current = it.next();
      if (current->checkState() == Qt::Checked)
	{
	  selectedAttributes.push_back(current->text());
	}
    }
  exitStatus = 0;
  this->close();
}

void InheritanceDialog::cancelAndClose()
{
  selectedAttributes.clear();
  exitStatus = 1;
  this->close();
}

int InheritanceDialog::getExitStatus()
{
  return exitStatus;
}

QVector<QString> InheritanceDialog::getSelected()
{
  return selectedAttributes;
}
