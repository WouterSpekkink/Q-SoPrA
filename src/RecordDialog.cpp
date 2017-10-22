#include "../include/RecordDialog.h"

RecordDialog::RecordDialog(QWidget *parent, EventSequenceDatabase *submittedEsd) : QDialog(parent) {
  esd = submittedEsd;
  timeStamp = "";
  description = "";
  raw = "";
  comment = "";
  source = "";
  exitStatus = 0;
  
  titleLabel = new QLabel("<h1>Set incident details<h1>");
  timeStampLabel = new QLabel("Timing:");
  descriptionLabel = new QLabel("Description:");
  rawLabel = new QLabel("Raw text:");
  commentLabel = new QLabel("Comments:");
  sourceLabel = new QLabel("Source:");
  
  timeStampField = new QLineEdit();
  descriptionField = new QPlainTextEdit();
  rawField = new QPlainTextEdit();
  commentField = new QPlainTextEdit();
  sourceField = new QLineEdit();

  saveRecordButton = new QPushButton("Save incident");
  cancelButton = new QPushButton("Cancel");

  // We connect all the signals.
  connect(timeStampField, SIGNAL(textChanged(const QString &)), this, SLOT(setTimeStamp(const QString &)));
  connect(sourceField, SIGNAL(textChanged(const QString &)), this, SLOT(setSource(const QString &)));
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
  this->setMinimumWidth(800);
  setWindowTitle("Set incident data"); 
}

void RecordDialog::setTimeStamp(const QString &text) {
  timeStamp = text;
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

void RecordDialog::saveAndClose() {
  description = descriptionField->toPlainText();
  raw = rawField->toPlainText();
  comment = commentField->toPlainText();
  this->close();
}
