#include "../include/RelationalTable.h"

RelationalTable::RelationalTable(QWidget *parent) : QSqlRelationalTableModel(parent) {};

/*
  I figured that a tool tip that simply shows the incident number (or its order variable, to 
  be more precise), is not very helpful. Instead, it would be more helpful to show the 
  description of the incident. Therefore, I have decided to fetch the description data
  from the incidents table, and to show that description whenever the user is hovering
  the mouse over a cell that belongs to a column with the header called "Incident".

  I later implemented the same principle for entities.
*/

QVariant RelationalTable::data(const QModelIndex &index, int role) const {
  if (role == Qt::ToolTipRole) {
    int col = index.column();
    const QString original = QSqlRelationalTableModel::data(index, Qt::DisplayRole).toString();
    if (this->headerData(col, Qt::Horizontal, Qt::DisplayRole) == "Incident") {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT description FROM incidents "
		     "WHERE ch_order = :order");
      query->bindValue(":order", original);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QString toolTip = breakString(description);
      return toolTip;
      delete query;
    } else if (this->headerData(col, Qt::Horizontal, Qt::DisplayRole).toString() == "Entity") {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT description FROM entities "
		     "WHERE name = :name");
      query->bindValue(":name", original);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QString toolTip = breakString(description);
      return toolTip;
      delete query;      
    } else {
      QString toolTip = breakString(original);
      return toolTip;
    }
  } else {
    return QSqlRelationalTableModel::data(index, role);
  }
  return QVariant();
}

