#include "../include/RecordDialog.h"

RecordDialog::RecordDialog(QWidget *parent, EventSequenceDatabase *submittedEsd, const QString type) : QDialog(parent) {
  esd = submittedEsd;
  exitStatus = 1;
  if (type == NEW) {
    timeStamp = "";
    description = "";
    raw = "";
    comment = "";
    source = "";
  }
  
  titleLabel = new QLabel("<h1>Set incident details<h1>", this);
  timeStampLabel = new QLabel("Timing:", this);
  descriptionLabel = new QLabel("Description:", this);
  rawLabel = new QLabel("Raw text:", this);
  commentLabel = new QLabel("Comments:", this);
  sourceLabel = new QLabel("Source:", this);
  
  timeStampField = new QLineEdit(this);
  descriptionField = new QTextEdit(this);
  descriptionField->installEventFilter(this);
  rawField = new QTextEdit(this);
  rawField->installEventFilter(this);
  commentField = new QTextEdit(this);
  commentField->installEventFilter(this);
  sourceField = new QLineEdit(this);

  saveRecordButton = new QPushButton("Save incident", this);
  cancelButton = new QPushButton("Cancel", this);

  // We connect all the signals.
  connect(timeStampField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setTimeStamp(const QString &)));
  connect(sourceField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setSource(const QString &)));
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
  this->setMinimumWidth(1000);
  setWindowTitle("Set incident data"); 
}

void RecordDialog::setTimeStamp(const QString &text) {
  timeStamp = text;
}

void RecordDialog::initialize() {
  timeStampField->setText(timeStamp);
  sourceField->setText(source);
  descriptionField->setText(description);
  rawField->setText(raw);
  commentField->setText(comment);
}

void RecordDialog::setSource(const QString &text) {
  source = text;
}

QString RecordDialog::getTimeStamp() {
  return timeStamp;
}

QString RecordDialog::getSource() {
  return source;
}

QString RecordDialog::getDescription() {
  return description;
}

QString RecordDialog::getRaw() {
  return raw;
}

QString RecordDialog::getComment() {
  return comment;
}

int RecordDialog::getExitStatus() {
  return exitStatus;
}

void RecordDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void RecordDialog::setTimeStamp(QString &text) {
  timeStamp = text;
}

void RecordDialog::setSource(QString &text) {
  source = text;
}

void RecordDialog::setDescription(QString &text) {
  description = text;
}

void RecordDialog::setRaw(QString &text) {
  raw = text;
}

void RecordDialog::setComment(QString &text) {
  comment = text;
}

void RecordDialog::saveAndClose() {
  description = descriptionField->toPlainText();
  raw = rawField->toPlainText();
  comment = commentField->toPlainText();
  if (description == "") {
    QPointer<QMessageBox> errorBox = new QMessageBox(this);
    errorBox->setText(tr("<b>ERROR</b>"));
    errorBox->setInformativeText("A description for the incident is required.");
    errorBox->exec();
    return;
  } else if (timeStamp == "") {
      QPointer<QMessageBox> errorBox = new QMessageBox(this);
    errorBox->setText(tr("<b>ERROR</b>"));
    errorBox->setInformativeText("A time stamp for the incident is required.");
    errorBox->exec();
    return;
  } else if (source == "") {
    QPointer<QMessageBox> errorBox = new QMessageBox(this);
    errorBox->setText(tr("<b>ERROR</b>"));
    errorBox->setInformativeText("A source for the incident is required.");
    errorBox->exec();
    return;
  } else {
    exitStatus = 0;
    this->close();
  }
}

bool RecordDialog::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = (QWheelEvent*) event;
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
    if (textEdit) {
      if(wheelEvent->modifiers() & Qt::ControlModifier) {
        if (wheelEvent->angleDelta().y() > 0) {
	  textEdit->zoomIn(1);
	} else if (wheelEvent->angleDelta().y() < 0) {
	  textEdit->zoomOut(1);
	}
      }
    }
  }
  return false;
}
