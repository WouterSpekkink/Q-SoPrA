#include "../include/RecordDialog.h"

RecordDialog::RecordDialog(QWidget *parent)
  : QDialog(parent) 
{
  _exitStatus = 1;
  _timeStamp = "";
  _description = "";
  _raw = "";
  _comment = "";
  _source = "";

  titleLabel = new QLabel("<h1>Set incident details<h1>", this);
  timeStampLabel = new QLabel("<b>Timing:</b>", this);
  descriptionLabel = new QLabel("<b>Description:</b>", this);
  rawLabel = new QLabel("<b>Raw text:</b>", this);
  commentLabel = new QLabel("<b>Comments:</b>", this);
  sourceLabel = new QLabel("<b>Source:</b>", this);
  
  timeStampField = new QLineEdit(this);
  descriptionField = new TextEdit(this);
  descriptionField->installEventFilter(this);
  rawField = new TextEdit(this);
  rawField->setAcceptRichText(false);
  rawField->installEventFilter(this);
  commentField = new TextEdit(this);
  commentField->installEventFilter(this);
  sourceField = new QLineEdit(this);

  saveRecordButton = new QPushButton("Save incident", this);
  cancelButton = new QPushButton("Cancel", this);

  // We connect all the signals.
  connect(saveRecordButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  
  // Let's set the layout next.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topLayout = new QHBoxLayout;
  mainLayout->addWidget(titleLabel);
  topLayout->addWidget(timeStampLabel);
  topLayout->addWidget(timeStampField);
  topLayout->addWidget(sourceLabel);
  topLayout->addWidget(sourceField);
  mainLayout->addLayout(topLayout);
  QPointer<QHBoxLayout> fieldLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> descriptionLayout = new QVBoxLayout;
  descriptionLayout->addWidget(descriptionLabel);
  descriptionLayout->addWidget(descriptionField);
  fieldLayout->addLayout(descriptionLayout);
  QPointer<QVBoxLayout> rawLayout = new QVBoxLayout;
  rawLayout->addWidget(rawLabel);
  rawLayout->addWidget(rawField);
  fieldLayout->addLayout(rawLayout);
  mainLayout->addLayout(fieldLayout);
  mainLayout->addWidget(commentLabel);
  mainLayout->addWidget(commentField);

  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(saveRecordButton);
  buttonLayout->addWidget(cancelButton);

  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  
  // Set tab order
  setTabOrder(timeStampField, sourceField);
  setTabOrder(sourceField, descriptionField);
  setTabOrder(descriptionField, rawField);
  setTabOrder(rawField, commentField);
  
  this->setMinimumWidth(1000);
  setWindowTitle("Set incident data"); 
}

void RecordDialog::initialize() 
{
  timeStampField->setText(_timeStamp);
  sourceField->setText(_source);
  descriptionField->setText(_description);
  rawField->setText(_raw);
  commentField->setText(_comment);
}

QString RecordDialog::getTimeStamp() 
{
  return _timeStamp;
}

QString RecordDialog::getSource() 
{
  return _source;
}

QString RecordDialog::getDescription() 
{
  return _description;
}

QString RecordDialog::getRaw() 
{
  return _raw;
}

QString RecordDialog::getComment() 
{
  return _comment;
}

int RecordDialog::getExitStatus() 
{
  return _exitStatus;
}

void RecordDialog::cancelAndClose() 
{
  _exitStatus = 1;
  this->close();
}

void RecordDialog::setTimeStamp(QString &timeStamp) 
{
  _timeStamp = timeStamp;
}

void RecordDialog::setSource(QString &source) 
{
  _source = source;
}

void RecordDialog::setDescription(QString &description) 
{
  _description = description;
}

void RecordDialog::setRaw(QString &raw) 
{
  _raw = raw;
}

void RecordDialog::setComment(QString &comment) 
{
  _comment = comment;
}

void RecordDialog::saveAndClose() 
{
  _timeStamp = timeStampField->text();
  _source = sourceField->text();
  _description = descriptionField->toPlainText();
  _raw = rawField->toPlainText();
  while (_raw.endsWith("\n")) 
    {
      _raw = _raw.left(_raw.length() - 1);
    }
  _comment = commentField->toPlainText();
  if (_description == "") 
    {
      QPointer<QMessageBox> errorBox = new QMessageBox(this);
      errorBox->setText(tr("<b>ERROR</b>"));
      errorBox->setInformativeText("A description for the incident is required.");
      errorBox->exec();
      return;
    }
  else if (_timeStamp == "") 
    {
      QPointer<QMessageBox> errorBox = new QMessageBox(this);
      errorBox->setText(tr("<b>ERROR</b>"));
      errorBox->setInformativeText("A time stamp for the incident is required.");
      errorBox->exec();
      return;
    }
  else if (_source == "") 
    {
      QPointer<QMessageBox> errorBox = new QMessageBox(this);
      errorBox->setText(tr("<b>ERROR</b>"));
      errorBox->setInformativeText("A source for the incident is required.");
      errorBox->exec();
      return;
    }
  else 
    {
      _exitStatus = 0;
      this->close();
    }
}

bool RecordDialog::eventFilter(QObject *object, QEvent *event) 
{
  if (event->type() == QEvent::Wheel) 
    {
      QWheelEvent *wheelEvent = (QWheelEvent*) event;
      TextEdit *textEdit = qobject_cast<TextEdit*>(object);
      if (textEdit) 
	{
	  if(wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->angleDelta().y() > 0) 
		{
		  textEdit->zoomIn(1);
		}
	      else if (wheelEvent->angleDelta().y() < 0) 
		{
		  textEdit->zoomOut(1);
		}
	    }
	}
    }
  return false;
}
