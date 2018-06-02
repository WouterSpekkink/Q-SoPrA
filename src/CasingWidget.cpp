#include "../include/CasingWidget.h"

CasingWidget::CasingWidget(parent) : QWidget(parent) {
  createTable();

  QPointer<QVBoxLayout> mainLayout;
  mainLayout->addWidget(tableWidget);

  setLayout(mainLayout);
}

void CasingWidget::createTable() {
  tableWidget = new QTableWidget(this);
  QSqlQuery *query;
  query->exec("SELECT COUNT(id) FROM incidents");
  int count = query->value(0).toInt();
  tableWidget->setRowCount(count);
  query->exec("SELECT COUNT(id) FROM cases");
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
  query->exec("SELECT timing, description, source "
	      "FROM incidents "
	      "ORDER BY ch_order ASC");
  while (query->next()) {
    QString timing = query->value(0).toString();
    QString description = query->value(1).toString();
    QString source = query->value(2).toString();
    QTableWidgetItem *newTiming = new QTableWidgetItem(timing, 0);
    QTableWidgetItem *newDescription = new QTableWidgetItem(description, 0);
    QTableWidgetItem *newSource = new QTableWidgetItem(source, 0);
    tableWidget->setItem(rowCount, 0, newTiming);
    tableWidget->setItem(rowCount, 1, newDescription);
    tableWidget->setItem(rowCount, 2, newSource);
    rowCount++;
  }
  // Now let us fill the additional columns (check boxes)
  colCount = 3;
  query->exec("SELECT name, description FROM cases");
  QSqlQuery *query2 = new QSqlQuery;
  while (query->next()) {
    QString currentCase = query->value(0).toString();
    QString currentDescription = query->value(1).toString();
    QTableWidgetItem *caseHeader = new QTableWidgetItem(currentCase, 0);
    tableWidget->setHorizontalHeaderItem(colCount, caseHeader);
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
		      "WHERE incident = :incident AND case = :case");
      query2->bindValue(":incident", incident);
      query2->bindValue(":case", currentCase);
      query2->exec();
      query2->first();
      if (!query2->isNull(0)) {
	checked = true;
      }
      QTableWidgetItem *newCheck = new QTableWidgetItem(incident, 1);
      if (checked) {
	newCheck->setCheckState(Qt::Checked);
      } else {
	newCheck->setCheckState(Qt::Unchecked);
      }
      tableWidget->setItem(i, colCount, newCheck);
    }
    colCount++;
  }  
  delete query;
  delete query2;
}

void CasingWidget::updateTable() {
  delete tableWidget;
  createTable();
}
