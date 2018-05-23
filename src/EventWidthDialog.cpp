#include "../include/EventWidthDialog.h"

EventWidthDialog::EventWidthDialog(QWidget *parent, int max) : QDialog(parent) {
  // First we construct our dialog's entities.
  exitStatus = 1;
  widthLabel = new QLabel(tr("Width:"), this);
  widthField = new QLineEdit(this);
  width = 1;
  widthField->setValidator(new QIntValidator(40, max, this)); 
  goButton = new QPushButton(tr("Set"), this);
  goButton->setEnabled(false); // We only activate this button if there is something in the fields.
  cancelButton = new QPushButton(tr("Cancel"), this);

  // We connect the signals to the appropriate slots.
  connect(goButton, SIGNAL(clicked()), this, SLOT(go()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
  connect(widthField, SIGNAL(textChanged(const QString &)), this, SLOT(setWidthText(const QString &)));
  
  // Then we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;

  QPointer<QHBoxLayout> fieldLayout = new QHBoxLayout;
  fieldLayout->addWidget(widthLabel);
  fieldLayout->addWidget(widthField);

  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(goButton);
  buttonLayout->addWidget(cancelButton);
  
  mainLayout->addLayout(fieldLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("Set event width"));
  setFixedHeight(sizeHint().height());
  // And this finished the constructor.
}

void EventWidthDialog::setWidthText(const QString newWidth) {
  QString widthText = newWidth.trimmed();
  // Let us immediately check if we should active the go Button.
  if (widthText != "") {
    width = widthText.toInt();
    if (width >= 40) {
      goButton->setEnabled(true);
    }
  } else {
    goButton->setEnabled(false);
  }
}

void EventWidthDialog::go() {
  exitStatus = 0;
  this->close();
}

void EventWidthDialog::cancel() {
  exitStatus = 1;
  this->close();
}

int EventWidthDialog::getWidth() {
  return width;
}

int EventWidthDialog::getExitStatus() {
  return exitStatus;
}

