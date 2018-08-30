#include "../include/LinkageTypeDialog.h"

LinkageTypeDialog::LinkageTypeDialog(QWidget *parent) : QDialog(parent) 
{
  name = "";
  subName = "";
  description = "";
  question = "";
  direction = PAST;
  exitStatus = 1;

  nameLabel = new QLabel(tr("<b>Name:</b>"), this);
  descriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  questionLabel = new QLabel(tr("<b>Question:</b>"), this);
  questionLabel->setToolTip(breakString("Formulate a question that needs to be answered "
					"with 'yes' for a linkage between a given pair of "
					"incidents to exist"));
  directionLabel = new QLabel(tr("<b>Linkage direction:</b>"), this);
  pastButton = new QPushButton(PAST, this);
  pastButton->setCheckable(true);
  pastButton->setChecked(true);
  futureButton = new QPushButton(FUTURE, this);
  futureButton->setCheckable(true);
  futureButton->setChecked(false);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  nameField = new QLineEdit(this);

  descriptionField = new QTextEdit(this);
  questionField = new QTextEdit(this);

  connect(nameField, SIGNAL(textChanged(const QString &)), this, SLOT(setName(const QString &)));
  connect(pastButton, SIGNAL(clicked()), this, SLOT(checkPastButton()));
  connect(futureButton, SIGNAL(clicked()), this, SLOT(checkFutureButton()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> nameLayout = new QHBoxLayout;
  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(nameField);
  mainLayout->addLayout(nameLayout);
  QPointer<QHBoxLayout> descriptionLayout = new QHBoxLayout;
  descriptionLayout->addWidget(descriptionLabel);
  descriptionLayout->addWidget(descriptionField);
  mainLayout->addLayout(descriptionLayout);
  QPointer<QHBoxLayout> questionLayout = new QHBoxLayout;
  descriptionLayout->addWidget(questionLabel);
  descriptionLayout->addWidget(questionField);
  mainLayout->addLayout(questionLayout);
  QPointer<QHBoxLayout> directionLayout = new QHBoxLayout;
  directionLayout->addWidget(directionLabel);
  directionLayout->addWidget(pastButton);
  directionLayout->addWidget(futureButton);
  mainLayout->addLayout(directionLayout);

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
  setWindowTitle("Add / Edit Linkage Type");
  setFixedHeight(sizeHint().height());
  
}

void LinkageTypeDialog::setName(const QString &newName) 
{
  name = newName;
}

void LinkageTypeDialog::checkPastButton() 
{
  futureButton->setChecked(false);
  direction = PAST;
}

void LinkageTypeDialog::checkFutureButton() 
{
  pastButton->setChecked(false);
  direction = FUTURE;
}

void LinkageTypeDialog::submitName(const QString &submittedName) 
{
  name = submittedName;
  subName = submittedName;
  nameField->setText(name);
}

void LinkageTypeDialog::submitDescription(const QString &submittedDescription) 
{
  description = submittedDescription;
  descriptionField->setText(description);
}

void LinkageTypeDialog::submitQuestion(const QString &submittedQuestion) 
{
  question = submittedQuestion;
  questionField->setText(question);
}

void LinkageTypeDialog::submitDirection(const QString &submittedDirection) 
{
  direction = submittedDirection;
  if (direction == PAST) 
    {
      pastButton->setChecked(true);
      futureButton->setChecked(false);
    }
  else if (direction == FUTURE) 
    {
      pastButton->setChecked(false);
      futureButton->setChecked(true);
    }
}

void LinkageTypeDialog::fixDirection() 
{
  pastButton->setEnabled(false);
  futureButton->setEnabled(false);
}

QString LinkageTypeDialog::getName() 
{
  return name;
}

QString LinkageTypeDialog::getDescription() 
{
  return description;
}


QString LinkageTypeDialog::getQuestion() 
{
  return question;
}

QString LinkageTypeDialog::getDirection() 
{
  return direction;
}

int LinkageTypeDialog::getExitStatus() 
{
  return exitStatus;
}

void LinkageTypeDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void LinkageTypeDialog::saveAndClose() 
{
  description = descriptionField->toPlainText().trimmed();
  question = questionField->toPlainText().trimmed();
  name = name.trimmed();
  if (description == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Description required.");
      warningBox->setInformativeText("A linkage type requires a description.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  if (question == "") 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Question required.");
      warningBox->setInformativeText("A linkage type requires a question.");
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
      warningBox->setInformativeText("A linkage type requires a name.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  bool empty = false;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name FROM linkage_types WHERE name = :name");
  query->bindValue(":name", name);
  query->exec();
  query->first();
  empty = query->isNull(0);
  if (!empty && name != subName) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("Duplicate name.");
      warningBox->setInformativeText("You cannot create linkage types with identical names.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  delete query;
  exitStatus = 0;
  this->close();
}
