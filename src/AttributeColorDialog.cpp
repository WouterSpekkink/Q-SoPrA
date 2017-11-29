#include "../include/AttributeColorDialog.h"

AttributeColorDialog::AttributeColorDialog(QWidget *parent, QString submittedType) : QDialog(parent) {
  type = submittedType;
  exitStatus = 1;
  chosenColor = QColor(Qt::darkGreen);
  chosenTextColor = QColor(Qt::black);
  chosenAttribute = DEFAULT;

  attributeLabel = new QLabel(tr("Choose attribute:"), this);

  attributeListWidget = new QListWidget(this);

  colorButton = new QPushButton(tr("Choose node color"), this);
  textColorButton = new QPushButton(tr("Choose text color"), this);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  QPalette pal = colorButton->palette();
  pal.setColor(QPalette::Button, chosenColor);
  colorButton->setAutoFillBackground(true);
  colorButton->setPalette(pal);
  colorButton->update();
  pal = textColorButton->palette();
  pal.setColor(QPalette::Button, chosenTextColor);
  textColorButton->setAutoFillBackground(true);
  textColorButton->setPalette(pal);
  textColorButton->update();
  
  connect(colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
  connect(textColorButton, SIGNAL(clicked()), this, SLOT(setTextColor()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(attributeListWidget, SIGNAL(currentTextChanged(const QString &)),
	  this, SLOT(setAttribute(const QString &)));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(attributeLabel);
  mainLayout->addWidget(attributeListWidget);
  mainLayout->addWidget(colorButton);
  mainLayout->addWidget(textColorButton);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setWindowTitle("Color by attribute");

  getAttributes();
}

void AttributeColorDialog::getAttributes() {
  QSqlQuery *query = new QSqlQuery;
  if (type == ENTITY) {
    query->exec("SELECT name FROM entity_attributes "
		"WHERE father = 'NONE' "
		"ORDER BY name asc");
    while (query->next()) {
      QString currentAttribute = query->value(0).toString();
      QListWidgetItem *item = new QListWidgetItem(currentAttribute);
      attributeListWidget->addItem(item);
      addChildren(currentAttribute, 1);
    }
  } else if (type == INCIDENT) {
    query->exec("SELECT DISTINCT name FROM incident_attributes "
		"ORDER BY name asc");
    while (query->next()) {
      QString currentAttribute = query->value(0).toString();
      QListWidgetItem *item = new QListWidgetItem(currentAttribute);
      attributeListWidget->addItem(item);
      addChildren(currentAttribute, 1);
    }
  }
  delete query;
}

void AttributeColorDialog::addChildren(QString father, int indent) {
  QSqlQuery *query = new QSqlQuery;
  if (type == ENTITY) {
    query->prepare("SELECT name FROM entity_attributes "
		   "WHERE father = :father "
		   "ORDER BY name asc");
    query->bindValue(":father", father);
    query->exec();
    while (query->next()) {
      QString tab = "   ";
      QString currentAttribute = query->value(0).toString();
      QString currentItem = tab.repeated(indent) + currentAttribute;
      QListWidgetItem *item = new QListWidgetItem(currentItem);
      attributeListWidget->addItem(item);
      addChildren(currentAttribute, indent + 1);
    }
  } else if (type == INCIDENT) {
    query->prepare("SELECT name FROM incident_attributes "
		   "WHERE father = :father "
		   "ORDER BY name asc");
    query->bindValue(":father", father);
    query->exec();
    while (query->next()) {
      QString tab = "   ";
      QString currentAttribute = query->value(0).toString();
      QString currentItem = tab.repeated(indent) + currentAttribute;
      QListWidgetItem *item = new QListWidgetItem(currentItem);
      attributeListWidget->addItem(item);
      addChildren(currentAttribute, indent + 1);
    }
  }
  delete query;
}

void AttributeColorDialog::setColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    chosenColor = colorDialog->selectedColor();
  }
  QPalette pal = textColorButton->palette();
  pal.setColor(QPalette::Button, chosenColor);
  colorButton->setAutoFillBackground(true);
  colorButton->setPalette(pal);
  colorButton->update();
  saveCloseButton->setFocus();
  delete colorDialog;
}

void AttributeColorDialog::setTextColor() {
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  if (colorDialog->exec()) {
    chosenTextColor = colorDialog->selectedColor();
  }
  QPalette pal = colorButton->palette();
  pal.setColor(QPalette::Button, chosenTextColor);
  textColorButton->setAutoFillBackground(true);
  textColorButton->setPalette(pal);
  textColorButton->update();
  saveCloseButton->setFocus();
  delete colorDialog;
}


void AttributeColorDialog::setAttribute(const QString &newAttribute) {
  QString temp = newAttribute.trimmed();
  //  temp.replace(" ", "");
  chosenAttribute = temp;
}

void AttributeColorDialog::cancelAndClose() {
  exitStatus = 1;
  this->close();
}

void AttributeColorDialog::saveAndClose() {
  if (chosenAttribute == DEFAULT) {
    QPointer <QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Ok);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("No attribute chosen.");
    warningBox->setInformativeText("You have to choose an attribute to proceed.");
    warningBox->exec();
    delete warningBox;
    return;
  } else {
    exitStatus = 0;
    this->close();
  }
}

QColor AttributeColorDialog::getColor() {
  return chosenColor;
}

QColor AttributeColorDialog::getTextColor() {
  return chosenTextColor;
}

QString AttributeColorDialog::getAttribute() {
    return chosenAttribute;
}

int AttributeColorDialog::getExitStatus() {
  return exitStatus;
}

