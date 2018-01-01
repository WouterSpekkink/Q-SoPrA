#include "../include/LinkagesIndexDialog.h"

LinkagesIndexDialog::LinkagesIndexDialog(QWidget *parent, int size, const QString &direction) : QDialog(parent) {
  max =  1;
  submittedSize = size - 1;
  submittedDirection = direction;
  tailIndexLabel = new QLabel(tr("Tail index:"), this);
  headIndexLabel = new QLabel(tr("Head index:"), this);
  tailIndexText = "";
  headIndexText = "";
  if (submittedDirection == PAST) {
    tailIndex = 2;
    headIndex = 1;
  } else {
    tailIndex = 1;
    headIndex = 2;
  }
  exitStatus = 1;
  tailIndexField = new QLineEdit(this);
  tailIndexField->setValidator(new QIntValidator(1, submittedSize, this));
  headIndexField = new QLineEdit(this);
  headIndexField->setValidator(new QIntValidator(1, 1, this));

  goButton = new QPushButton(tr("Go"), this);
  goButton->setEnabled(false); // We only activate this button if there is something in the fields.
  cancelButton = new QPushButton(tr("Cancel"), this);

  // We connect the signals to the appropriate slots.
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
  connect(tailIndexField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setTailIndexText(const QString &)));
  connect(headIndexField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setHeadIndexText(const QString &)));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> fieldLayout = new QHBoxLayout;
  fieldLayout->addWidget(tailIndexLabel);
  fieldLayout->addWidget(tailIndexField);
  fieldLayout->addWidget(headIndexLabel);
  fieldLayout->addWidget(headIndexField);
  
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(goButton);
  buttonLayout->addWidget(cancelButton);
  
  mainLayout->addLayout(fieldLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("Jump to index"));
  setFixedHeight(sizeHint().height());
}

void LinkagesIndexDialog::setTailIndexText(const QString &newIndex) {
  tailIndexText = newIndex.trimmed();
  if (submittedDirection == PAST) {
    std::istringstream ss(tailIndexText.toStdString());
    ss >> max;
    headIndexField->setValidator(new QIntValidator(1, max, this));
  } else if (submittedDirection == FUTURE) {
    std::istringstream ss(tailIndexText.toStdString());
    ss >> max;
    headIndexField->setValidator(new QIntValidator(1, submittedSize - max + 1, this));
  }
  tailIndex = tailIndexText.toInt();
  evaluateIndexes();
}

void LinkagesIndexDialog::setHeadIndexText(const QString &newIndex) {
  headIndexText = newIndex;
  headIndex = headIndexText.toInt();
  evaluateIndexes();
}
void LinkagesIndexDialog::evaluateIndexes() {
  if (tailIndexText != "" && tailIndex > 0 && headIndexText != "" && headIndex > 0) {
    goButton->setEnabled(true);
  } else {
    goButton->setEnabled(false);
  }
}

int LinkagesIndexDialog::getTailIndex() {
  return tailIndex;
}

int LinkagesIndexDialog::getHeadIndex() {
  return headIndex;
}

void LinkagesIndexDialog::go() {
  exitStatus = 0;
  this->close();
}

void LinkagesIndexDialog::cancel() {
  exitStatus = 1;
  this->close();
}

int LinkagesIndexDialog::getExitStatus() {
  return exitStatus;
}
