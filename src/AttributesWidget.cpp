#include "../include/AttributesWidget.h"

AttributesWidget::AttributesWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  tableModel = new QSqlTableModel;

  tableModel->setTable("incident_attributes");
  tableModel->select();
  
  timeStampLabel = new QLabel("Timing:");
  sourceLabel = new QLabel("Source");
  descriptionLabel = new QLabel("Description:");
  rawLabel = new QLabel("Raw:");
  commentLabel = new QLabel("Comments:");

  timeStampField = new QLineEdit();
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit();
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit();
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit();
  rawField->setReadOnly(true);
  commentField = new QTextEdit();
  commentField->setReadOnly(true);

  newAttributeButton = new QPushButton("New attribute");

  connect(newAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  topFieldsLayout->addWidget(timeStampLabel);
  topFieldsLayout->addWidget(timeStampField);
  topFieldsLayout->addWidget(sourceLabel);
  topFieldsLayout->addWidget(sourceField);
  mainLayout->addLayout(topFieldsLayout);
  mainLayout->addWidget(descriptionLabel);
  mainLayout->addWidget(descriptionField);
  mainLayout->addWidget(rawLabel);
  mainLayout->addWidget(rawField);
  mainLayout->addWidget(commentLabel);
  mainLayout->addWidget(commentField);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(newAttributeButton);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  // I should think of a way to store this at some point.
  // Probably just in a separate table.
  retrieveData(1);
}

void AttributesWidget::newAttribute() {
  attributeDialog = new AttributeDialog(this, esd);
  attributeDialog->exec();

  if (attributeDialog->getExitStatus() == 0) {
    QString name = attributeDialog->getName();
    QString description = attributeDialog->getDescription();
    
    int newIndex = tableModel->rowCount();
    
    tableModel->insertRow(newIndex);
    tableModel->setData(tableModel->index(newIndex, 1), name);
    tableModel->setData(tableModel->index(newIndex, 2), description);
    tableModel->submitAll();
  }
  delete attributeDialog;
}

void AttributesWidget::retrieveData(int order) {
  QSqlQueryModel *query = new QSqlQueryModel;
  query->setQuery("SELECT * FROM incidents");
  QString timeStamp = query->record(order - 1).value("timestamp").toString();
  QString source = query->record(order - 1).value("source").toString();
  QString description = query->record(order - 1).value("description").toString();
  QString raw = query->record(order - 1).value("raw").toString();
  QString comment = query->record(order - 1).value("comment").toString();
  timeStampField->setText(timeStamp);
  sourceField->setText(source);
  descriptionField->setText(description);
  rawField->setText(raw);
  commentField->setText(comment);
}
