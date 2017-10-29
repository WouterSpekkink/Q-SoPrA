#include "../include/AttributesWidget.h"

AttributesWidget::AttributesWidget(QWidget *parent, EventSequenceDatabase *submittedEsd) : QWidget(parent) {
  esd = submittedEsd;

  incidentsModel = new QSqlTableModel;  
  incidentsModel->setTable("incidents");
  incidentsModel->select();

  attributesModel = new QSqlTableModel;
  attributesModel->setTable("incident_attributes");
  attributesModel->select();
  
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

  previousIncidentButton = new QPushButton("Previous incident");
  nextIncidentButton = new QPushButton("Next incident");
  newAttributeButton = new QPushButton("New attribute");

  connect(previousIncidentButton, SIGNAL(clicked()), this, SLOT(previousIncident()));
  connect(nextIncidentButton, SIGNAL(clicked()), this, SLOT(nextIncident()));
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
  buttonLayout->addWidget(previousIncidentButton);
  buttonLayout->addWidget(nextIncidentButton);
  buttonLayout->addWidget(newAttributeButton);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  retrieveData();
}

void AttributesWidget::previousIncident() {
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  if (order != 1) {
    query->prepare("UPDATE save_data "
		   "SET attributes_record=:new");
    query->bindValue(":new", order - 1);
    query->exec();
    retrieveData();
  }
}

void AttributesWidget::nextIncident() {
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order != incidentsModel->rowCount()) {
    query->prepare("UPDATE save_data "
		   "SET attributes_record=:new");
    query->bindValue(":new", order + 1);
    query->exec();
    retrieveData();
  }
}

void AttributesWidget::newAttribute() {
  attributeDialog = new AttributeDialog(this, esd);
  attributeDialog->exec();

  if (attributeDialog->getExitStatus() == 0) {
    QString name = attributeDialog->getName();
    QString description = attributeDialog->getDescription();
    
    int newIndex = attributesModel->rowCount();
    
    attributesModel->insertRow(newIndex);
    attributesModel->setData(attributesModel->index(newIndex, 1), name);
    attributesModel->setData(attributesModel->index(newIndex, 2), description);
    attributesModel->submitAll();
  }
  delete attributeDialog;
}

void AttributesWidget::retrieveData() {
  QSqlQueryModel *query = new QSqlQueryModel;
  query->setQuery("SELECT * FROM save_data");
  int order = 0;
  order = query->record(order).value("attributes_record").toInt();
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
