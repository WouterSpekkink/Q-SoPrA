#include "../include/NodeSettingsDialog.h"

NodeSettingsDialog::NodeSettingsDialog(QWidget *parent,
				       QVector<QString> submittedEntities,
				       QVector<QString> submittedDescriptions,
				       QVector<QString> submittedModes)
  : QDialog(parent) {

  entities = submittedEntities;
  descriptions = submittedDescriptions;
  modes = submittedModes;
  tableWidget = new QTableWidget(this);
  tableWidget->setRowCount(entities.length());
  tableWidget->setColumnCount(4); // default size
  QTableWidgetItem *headerOne = new QTableWidgetItem("Id", 0);
  QTableWidgetItem *headerTwo = new QTableWidgetItem("Label", 1);
  QTableWidgetItem *headerThree = new QTableWidgetItem("Description", 2);
  QTableWidgetItem *headerFour = new QTableWidgetItem("Mode", 3);
  tableWidget->setHorizontalHeaderItem(0, headerOne);
  tableWidget->setHorizontalHeaderItem(1, headerTwo);
  tableWidget->setHorizontalHeaderItem(2, headerThree);
  tableWidget->setHorizontalHeaderItem(3, headerFour);
  
  // Now let's fill the table
  for (QVector<QString>::size_type i = 0; i != entities.length(); i++) {
    QTableWidgetItem *newId = new QTableWidgetItem(entities[i], 0);
    QTableWidgetItem *newLabel = new QTableWidgetItem(entities[i], 0);
    QTableWidgetItem *newDescription = new QTableWidgetItem(descriptions[i], 0);
    QTableWidgetItem *newMode = new QTableWidgetItem(modes[i], 0);
    tableWidget->setItem(i, 0, newId);
    tableWidget->setItem(i, 1, newLabel);
    tableWidget->setItem(i, 2, newDescription);
    tableWidget->setItem(i, 3, newMode);
  }

  // Let's create the other objects now.
  addAttributeButton = new QPushButton(tr("Add attribute"), this);
  removeAttributeButton = new QPushButton(tr("Remove attribute"), this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  exportCloseButton = new QPushButton(tr("Export"), this);

  // Let's connect the signals now.
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(addAttributeButton, SIGNAL(clicked()), this, SLOT(addAttribute()));
  
  // And let's now create the layout
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableWidget);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(addAttributeButton);
  buttonLayout->addWidget(removeAttributeButton);
  buttonLayout->addWidget(cancelCloseButton);
  buttonLayout->addWidget(exportCloseButton);
  mainLayout->addLayout(buttonLayout);
  
  setLayout(mainLayout);
  setWindowTitle("Settings for node list");

  QSize availableSize = qApp->desktop()->availableGeometry().size();
  int width = availableSize.width();
  int height = availableSize.height();
  this->resize(width, height);
}

void NodeSettingsDialog::cancelAndClose() {
  this->close();
}

void NodeSettingsDialog::exportAndClose() {
  // NEED TO CHANGE THIS TO ACTUALLY EXPORT SOMETHING
  this->close();
}

void NodeSettingsDialog::addAttribute() {
  QPointer<AttributeSelectionDialog> attributeSelectionDialog = new AttributeSelectionDialog(this, ENTITY);
  attributeSelectionDialog->exec();
  if (attributeSelectionDialog->getExitStatus() == 0) {
    QString attribute = attributeSelectionDialog->getAttribute();
    QTableWidgetItem *newHeader = new QTableWidgetItem(attribute, 0);
    tableWidget->setColumnCount(tableWidget->columnCount() + 1);
    tableWidget->setHorizontalHeaderItem(tableWidget->columnCount() - 1, newHeader);
    QVector<QString> entitiesVec;
    QVector<QString> attributesVec;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT entity, value FROM attributes_to_entities "
		  "WHERE attribute = :attribute");
    query->bindValue(":attribute", attribute);
    query->exec();
    while (query->next()) {
      QString currentEntity = query->value(0).toString();
      entitiesVec.push_back(currentEntity);
      QString currentValue = query->value(1).toString();
      qDebug() << currentValue;
      if (currentValue == "") {
	currentValue = "1";
      }
      attributesVec.push_back(currentValue);
    }
    for (int i = 0; i != tableWidget->rowCount(); i++) {
      for (QVector<QString>::size_type j = 0; j != entitiesVec.length(); j++) {
	QString currentEntity = entitiesVec[j];
	QString currentValue = attributesVec[j];
	if (tableWidget->item(i, 0)->data(Qt::DisplayRole).toString() == currentEntity) {
	  QTableWidgetItem *newEntry = new QTableWidgetItem(currentValue, 0);
	  tableWidget->setItem(i, tableWidget->columnCount() - 1, newEntry);
	  break;
	} else {
	  QTableWidgetItem *newEntry = new QTableWidgetItem("0", 0);
	  tableWidget->setItem(i, tableWidget->columnCount() - 1, newEntry);
	}
      }
    }
  }
  // TODO
}

void NodeSettingsDialog::removeAttribute() {
  // TODO
}
