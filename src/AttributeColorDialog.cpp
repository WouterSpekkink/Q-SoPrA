#include "../include/AttributeColorDialog.h"

AttributeColorDialog::AttributeColorDialog(QWidget *parent, QString submittedType) : QDialog(parent) 
{
  type = submittedType;
  exitStatus = 1;
  chosenColor = QColor(Qt::darkGreen);
  chosenTextColor = QColor(Qt::black);
  chosenAttribute = DEFAULT;
  entity = false;

  attributeLabel = new QLabel(tr("<b>Choose attribute:</b>"), this);
  attributesFilterLabel = new QLabel(tr("<b>Filter:</b>"), this);

  attributesTreeView = new DeselectableTreeView(this);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  attributesTreeView->installEventFilter(this);

  attributesFilterField = new QLineEdit(this);
  
  colorButton = new QPushButton(tr("Choose node color"), this);
  colorButton->setMinimumWidth(200);
  QPixmap pixmapOne(colorButton->width(), colorButton->height());
  pixmapOne.fill(chosenColor);
  colorOneLabel = new QLabel(this);
  colorOneLabel->setMaximumWidth(50);
  colorOneLabel->setPixmap(pixmapOne);
  textColorButton = new QPushButton(tr("Choose text color"), this);
  textColorButton->setMinimumWidth(200);
  QPixmap pixmapTwo(textColorButton->width(), textColorButton->height());
  pixmapTwo.fill(chosenTextColor);
  colorTwoLabel = new QLabel(this);
  colorTwoLabel->setMaximumWidth(50);
  colorTwoLabel->setPixmap(pixmapTwo);

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);
  
  connect(colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
  connect(textColorButton, SIGNAL(clicked()), this, SLOT(setTextColor()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  connect(attributesFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setAttribute()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(attributeLabel);
  mainLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributesFilterLabel);
  filterLayout->addWidget(attributesFilterField);
  mainLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> colorOneLayout = new QHBoxLayout;
  colorOneLayout->addWidget(colorButton);
  colorOneLayout->addWidget(colorOneLabel);
  mainLayout->addLayout(colorOneLayout);
  QPointer<QHBoxLayout> colorTwoLayout = new QHBoxLayout;
  colorTwoLayout->addWidget(textColorButton);
  colorTwoLayout->addWidget(colorTwoLabel);
  mainLayout->addLayout(colorTwoLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(saveCloseButton);
  mainLayout->addLayout(optionsLayout);
  setLayout(mainLayout);

  setMinimumWidth(400);
  setMinimumHeight(600);
  setWindowTitle("Create mode");
}

void AttributeColorDialog::setTree() 
{
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  if (type == ENTITY) 
    {
      query->exec("SELECT name, description FROM entity_attributes WHERE father = 'NONE'");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *father = new QStandardItem(name);    
	  attributesTree->appendRow(father);
	  QString hint = breakString(description);
	  father->setToolTip(hint);
	  father->setEditable(false);
	  buildHierarchy(father, name);
	}
    }
  else if (type == INCIDENT) 
    {
      // First we will fetch the 'normal' attributes.
      query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE'");
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *father = new QStandardItem(name);    
	  attributesTree->appendRow(father);
	  QString hint = breakString(description);
	  father->setToolTip(hint);
	  father->setEditable(false);
	  buildHierarchy(father, name);
	}
      // And then we will also fetch the entities.
      QStandardItem *entities = new QStandardItem(ENTITIES);
      QString entitiesHint = breakString("You can also assign entities that you have created "
					 "in the relationships widget as attributes.");
      entities->setToolTip(entitiesHint);
      QFont font;
      font.setItalic(true);
      attributesTree->appendRow(entities);
      entities->setFont(font);
      query->exec("SELECT name, description FROM entities WHERE father = 'NONE'");
      int children = 0;
      while (query->next()) 
	{
	  QString name = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *father = new QStandardItem(name);
	  entities->setChild(children, father);
	  children++;
	  QString hint = breakString(description);
	  father->setToolTip(hint);
	  father->setEditable(false);
	  buildEntities(father, name);
	}
    }
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void AttributeColorDialog::buildHierarchy(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  if (type == ENTITY) 
    {
      query->prepare("SELECT name, description FROM entity_attributes WHERE  father = :father");
      query->bindValue(":father", name);
      query->exec();
      int children = 0;
      while (query->next()) 
	{
	  QString childName = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *child = new QStandardItem(childName);
	  top->setChild(children, child);
	  QString hint = breakString(description);
	  child->setToolTip(hint);
	  child->setEditable(false);
	  children++;
	  buildHierarchy(child, childName);
	}
    }
  else if (type == INCIDENT) 
    {
      query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father");
      query->bindValue(":father", name);
      query->exec();
      int children = 0;
      while (query->next()) 
	{
	  QString childName = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QStandardItem *child = new QStandardItem(childName);
	  top->setChild(children, child);
	  QString hint = breakString(description);
	  child->setToolTip(hint);
	  child->setEditable(false);
	  children++;
	  buildHierarchy(child, childName);
	}
    }
  delete query;
}

void AttributeColorDialog::buildEntities(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entities WHERE father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) 
    {
      QString childName = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *child = new QStandardItem(childName);
      top->setChild(children, child);
      QString hint = breakString(description);
      child->setToolTip(hint);
      child->setEditable(false);
      children++;
      buildEntities(child, childName);
    }
  delete query;
}

void AttributeColorDialog::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void AttributeColorDialog::setColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      chosenColor = colorDialog->selectedColor();
    }
  QPixmap pixmapOne(100, colorButton->height());
  pixmapOne.fill(chosenColor);
  colorOneLabel->setPixmap(pixmapOne);
  saveCloseButton->setFocus();
  delete colorDialog;
}

void AttributeColorDialog::setTextColor() 
{
  QPointer<QColorDialog> colorDialog = new QColorDialog(this);
  colorDialog->setOption(QColorDialog::DontUseNativeDialog, true);
  if (colorDialog->exec()) 
    {
      chosenTextColor = colorDialog->selectedColor();
    }
  QPixmap pixmapTwo(100, textColorButton->height());
  pixmapTwo.fill(chosenTextColor);
  colorTwoLabel->setPixmap(pixmapTwo);
  saveCloseButton->setFocus();
  delete colorDialog;
}
 
void AttributeColorDialog::setAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      chosenAttribute = attributesTreeView->currentIndex().data().toString();
    }
  QModelIndex currentIndex = attributesTreeView->currentIndex();
  while (currentIndex.parent().isValid()) 
    {
      currentIndex = currentIndex.parent();
    }
  if (currentIndex.data().toString() == ENTITIES) 
    {
      entity = true;
    }
}

void AttributeColorDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void AttributeColorDialog::saveAndClose() 
{
  if (chosenAttribute == DEFAULT) 
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("No attribute chosen.");
      warningBox->setInformativeText("You have to choose an attribute to proceed.");
      warningBox->exec();
      delete warningBox;
      return;
    }
  else 
    {
      exitStatus = 0;
      this->close();
    }
}

QColor AttributeColorDialog::getColor() 
{
  return chosenColor;
}

QColor AttributeColorDialog::getTextColor() 
{
  return chosenTextColor;
}

QString AttributeColorDialog::getAttribute() 
{
  return chosenAttribute;
}

bool AttributeColorDialog::isEntity() 
{
  return entity;
}

int AttributeColorDialog::getExitStatus() 
{  
  return exitStatus;
}

