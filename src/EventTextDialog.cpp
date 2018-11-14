#include "../include/EventTextDialog.h"

EventTextDialog::EventTextDialog(QWidget *parent) : QDialog(parent) 
{
  text = "";
  subText = "";
  timing = "";
  subTiming = "";
  exitStatus = 1;

  timingLabel = new QLabel(tr("<b>Timing:</b>"), this);
  textLabel = new QLabel(tr("<b>Event description:</b>"), this);
			   
  textField = new QTextEdit(this);

  timingField = new QLineEdit(this);
  
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> timingLayout = new QHBoxLayout;
  timingLayout->addWidget(timingLabel);
  timingLayout->addWidget(timingField);
  mainLayout->addLayout(timingLayout);
  mainLayout->addWidget(textLabel);
  mainLayout->addWidget(textField);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  optionsLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Event description");
}

QString EventTextDialog::getText() 
{
  return text;
}

QString EventTextDialog::getTiming() 
{
  return timing;
}

void EventTextDialog::submitText(const QString &submittedText) 
{
  text = submittedText;
  subText = submittedText;
  textField->setText(submittedText);
}

void EventTextDialog::submitTiming(const QString &submittedTiming) 
{
  timing = submittedTiming;
  subTiming = submittedTiming;
  timingField->setText(submittedTiming);
}

int EventTextDialog::getExitStatus() 
{
  return exitStatus;
}

void EventTextDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void EventTextDialog::saveAndClose() 
{
  text = textField->toPlainText().trimmed();
  timing = timingField->text();
  if (text == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Event description required.");
      warningBox->setInformativeText("Events cannot be saved without descriptions.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (timing == "")
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Timing required.");
      warningBox->setInformativeText("Events cannot be saved without "
				     "an indication of their timing.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  exitStatus = 0;
  this->close();
}

