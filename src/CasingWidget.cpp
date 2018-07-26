#include "../include/CasingWidget.h"

CasingWidget::CasingWidget(QWidget *parent) : QWidget(parent) {
  tableWidget = new QTableWidget(this);
  createTable();

  addCaseButton = new QPushButton(tr("Add case"));
  editCaseButton = new QPushButton(tr("Edit case"));
  removeCaseButton = new QPushButton(tr("Remove case"));
  selectAllButton = new QPushButton(tr("Set all selected"));
  deselectAllButton = new QPushButton(tr("Set all deselected"));
  attributeSelectButton = new QPushButton(tr("Select based on attribute"));
  relationshipSelectButton = new QPushButton(tr("Select based on relationship"));
  
  connect(tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
	    this, SLOT(setCellState(QTableWidgetItem*)));
  connect(addCaseButton, SIGNAL(clicked()), this, SLOT(addCase()));
  connect(editCaseButton, SIGNAL(clicked()), this, SLOT(editCase()));
  connect(removeCaseButton, SIGNAL(clicked()), this, SLOT(removeCase()));
  connect(selectAllButton, SIGNAL(clicked()), this, SLOT(selectAll()));
  connect(deselectAllButton, SIGNAL(clicked()), this, SLOT(deselectAll()));
  connect(attributeSelectButton, SIGNAL(clicked()), this, SLOT(attributeSelect()));
  connect(relationshipSelectButton, SIGNAL(clicked()), this, SLOT(relationshipSelect()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableWidget);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(addCaseButton);
  buttonLayout->addWidget(editCaseButton);
  buttonLayout->addWidget(removeCaseButton);
  buttonLayout->addWidget(selectAllButton);
  buttonLayout->addWidget(deselectAllButton);
  buttonLayout->addWidget(attributeSelectButton);
  buttonLayout->addWidget(relationshipSelectButton);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);
}

void CasingWidget::createTable() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT COUNT(id) FROM incidents");
  query->first();
  int count = query->value(0).toInt();
  tableWidget->setRowCount(count);
  query->exec("SELECT COUNT(id) FROM cases");
  query->first();
  count = query->value(0).toInt();
  tableWidget->setColumnCount(count + 3);
  // We have a few columns that are always there
  QTableWidgetItem *headerOne = new QTableWidgetItem("Timing", 0);
  QTableWidgetItem *headerTwo = new QTableWidgetItem("Description", 1);
  QTableWidgetItem *headerThree = new QTableWidgetItem("Source", 2);
  tableWidget->setHorizontalHeaderItem(0, headerOne);
  tableWidget->setHorizontalHeaderItem(1, headerTwo);
  tableWidget->setHorizontalHeaderItem(2, headerThree);
  // Let us fill these standard columns first.
  int rowCount = 0;
  query->exec("SELECT timestamp, description, source "
	      "FROM incidents "
	      "ORDER BY ch_order ASC");
  while (query->next()) {
    QString timing = query->value(0).toString();
    QString description = query->value(1).toString();
    QString source = query->value(2).toString();
    QTableWidgetItem *newTiming = new QTableWidgetItem(timing, 0);
    newTiming->setFlags(newTiming->flags() ^ Qt::ItemIsEditable);
    QTableWidgetItem *newDescription = new QTableWidgetItem(description, 0);
    newDescription->setFlags(newDescription->flags() ^ Qt::ItemIsEditable);
    QTableWidgetItem *newSource = new QTableWidgetItem(source, 0);
    newSource->setFlags(newSource->flags() ^ Qt::ItemIsEditable);
    tableWidget->setItem(rowCount, 0, newTiming);
    tableWidget->setItem(rowCount, 1, newDescription);
    tableWidget->setItem(rowCount, 2, newSource);
    rowCount++;
  }
  // Now let us fill the additional columns (check boxes)
  int colCount = 3;
  query->exec("SELECT name, description FROM cases");
  QSqlQuery *query2 = new QSqlQuery;
  while (query->next()) {
    QString currentCase = query->value(0).toString();
    QString currentDescription = query->value(1).toString();
    if (currentCase != COMPLETEDATASET) {
      QTableWidgetItem *caseHeader = new QTableWidgetItem(currentCase, 0);
      tableWidget->setHorizontalHeaderItem(colCount, caseHeader);
      tableWidget->setItemDelegateForColumn(colCount, new CheckBoxDelegate(tableWidget));
      caseHeader->setToolTip(breakString(currentDescription));
      for (int i = 0; i != tableWidget->rowCount(); i++) {
	int order = i + 1;
	query2->prepare("SELECT id FROM incidents WHERE ch_order = :order");
	query2->bindValue(":order", order);
	query2->exec();
	query2->first();
	int incident = query2->value(0).toInt();
	bool checked = false;
	query2->prepare("SELECT incident FROM incidents_to_cases "
			"WHERE incident = :incident AND casename = :case");
	query2->bindValue(":incident", incident);
	query2->bindValue(":case", currentCase);
	query2->exec();
	query2->first();
	if (!query2->isNull(0)) {
	  checked = true;
	}
	QTableWidgetItem *newCheck = new QTableWidgetItem(QString::number(incident), 1);
	tableWidget->blockSignals(true);
	if (checked) {
	  newCheck->setCheckState(Qt::Checked);
	} else {
	  newCheck->setCheckState(Qt::Unchecked);
	}
	tableWidget->setItem(i, colCount, newCheck);
	tableWidget->blockSignals(false);
      }
      colCount++;
    }
  }
  delete query;
  delete query2;
}

void CasingWidget::updateTable() {
  tableWidget->setRowCount(0);
  tableWidget->setColumnCount(0);
  createTable();
}

void CasingWidget::addCase() {
  QPointer<CaseDialog> caseDialog = new CaseDialog(this);
  caseDialog->exec();
  if (caseDialog->getExitStatus() == 0) {
    QString name = caseDialog->getName();
    QString description = caseDialog->getDescription();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("INSERT INTO cases (name, description) "
		   "VALUES (:name, :description)");
    query->bindValue(":name", name);
    query->bindValue(":description", description);
    query->exec();
    delete query;
    updateTable();
  }
}

void CasingWidget::editCase() {
  QVector<QString> cases;
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM cases");
  while (query->next()) {
    QString current = query->value(0).toString();
    if (current != COMPLETEDATASET) {
      cases.push_back(current);
    }
  }
  QPointer<ComboBoxDialog> comboDialog = new ComboBoxDialog(this, cases);
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) {
    QString caseName = comboDialog->getSelection();
    QVector<QString> cases;
    query->prepare("SELECT description FROM cases "
		   "WHERE name = :name");
    query->bindValue(":name", caseName);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QPointer<CaseDialog> caseDialog = new CaseDialog(this);
    caseDialog->submitName(caseName);
    caseDialog->setDescription(description);
    caseDialog->exec();
    if (caseDialog->getExitStatus() == 0) {
      QString newName = caseDialog->getName();
      QString newDescription = caseDialog->getDescription();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE cases SET name = :newname, description = :newdescription "
		     "WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdescription", newDescription);
      query->bindValue(":oldname", caseName);
      query->exec();
      updateTable();
      query->prepare("UPDATE incidents_to_cases SET casename = :newname "
		     "WHERE casename = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":oldname", caseName);
      query->exec();
    }
  }
  delete query;
  updateTable();
}

void CasingWidget::removeCase() {
  QSqlQuery *query = new QSqlQuery;
  QVector<QString> cases;
  query->exec("SELECT name FROM cases");
  while (query->next()) {
    QString current = query->value(0).toString();
    if (current != COMPLETEDATASET) {
      cases.push_back(current);
    }
  }
  QPointer<ComboBoxDialog> comboDialog = new ComboBoxDialog(this, cases);
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) {
    QString selection = comboDialog->getSelection();
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing a case cannot be undone. "
				   "Are you sure you want to continue?");
    if (warningBox->exec() == QMessageBox::Yes) {
      query->prepare("DELETE FROM cases "
		     "WHERE name = :selection");
      query->bindValue(":selection", selection);
      query->exec();
      query->prepare("DELETE FROM incidents_to_cases "
		     "WHERE casename = :selection");
      query->bindValue(":selection", selection);
      query->exec();
      updateTable();
    }
  }
  delete query;
}

void CasingWidget::selectAll() {
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  QVector<QString> cases;
  query->exec("SELECT name FROM cases");
  while (query->next()) {
    QString current = query->value(0).toString();
    if (current != COMPLETEDATASET) {
      cases.push_back(current);
    }
  }
  QPointer<ComboBoxDialog> comboDialog = new ComboBoxDialog(this, cases);
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) {
    QString selection = comboDialog->getSelection();
    query->prepare("DELETE FROM incidents_to_cases "
		   "WHERE casename = :case");
    query->bindValue(":case", selection);
    query->exec();
    query->exec("SELECT id FROM incidents");
    while (query->next()) {
      int incident = query->value(0).toInt();
      query2->prepare("INSERT INTO incidents_to_cases (incident, casename) "
		      "VALUES (:incident, :case)");
      query2->bindValue(":incident", incident);
      query2->bindValue(":case", selection);
      query2->exec();
    }
    updateTable();
  }
  delete query;
  delete query2;
}

void CasingWidget::deselectAll() {
  QSqlQuery *query = new QSqlQuery;
  QVector<QString> cases;
  query->exec("SELECT name FROM cases");
  while (query->next()) {
    QString current = query->value(0).toString();
    if (current != COMPLETEDATASET) {
      cases.push_back(current);
    }
  }
  QPointer<ComboBoxDialog> comboDialog = new ComboBoxDialog(this, cases);
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) {
    QString selection = comboDialog->getSelection();
    query->prepare("DELETE FROM incidents_to_cases "
		   "WHERE casename = :case");
    query->bindValue(":case", selection);
    query->exec();
    updateTable();
  }
  delete query;
}

void CasingWidget::attributeSelect() {

}

void CasingWidget::relationshipSelect() {

}

void CasingWidget::setCellState(QTableWidgetItem *item) {
  if (item->column() > 2) {
    QSqlQuery *query = new QSqlQuery;
    int order = item->row() + 1;
    QTableWidgetItem *currentHeader = tableWidget->horizontalHeaderItem(item->column());
    QString currentCase = currentHeader->data(Qt::DisplayRole).toString();
    query->prepare("SELECT id FROM incidents "
		   "WHERE ch_order = :order");
    query->bindValue(":order", order);
    query->exec();
    query->first();
    int incident = query->value(0).toInt();
    if (item->checkState() == Qt::Unchecked) {
      query->prepare("DELETE FROM incidents_to_cases "
		     "WHERE incident = :incident AND casename = :case");
      query->bindValue(":incident", incident);
      query->bindValue(":case", currentCase);
      query->exec();
    } else {
      query->prepare("INSERT INTO incidents_to_cases (incident, casename) "
		     "VALUES (:incident, :case)");
      query->bindValue(":incident", incident);
      query->bindValue(":case", currentCase);
      query->exec();
    }
    delete query;
  }
}
