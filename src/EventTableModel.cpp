#include "../include/EventTableModel.h"

EventTableModel::EventTableModel(QWidget *parent) : QSqlTableModel(parent) {};

QVariant EventTableModel::data(const QModelIndex &index, int role) const {
  if (index.column() == 7) { // This is always the column with the boolean variable
    if (role == Qt::CheckStateRole) { // Only do the below when we are setting the checkbox.
      // We want to fetch the state of the boolean from the sql table.
      QSqlQuery *query = new QSqlQuery;
      int order = index.row() + 1;
      query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int mark = query->value(0).toInt();
      // Return the appropriate check state based on the state of mar.
      if (mark == 1) {
	return Qt::Checked;
      } else if (mark == 0) {
	return Qt::Unchecked;
      }
    } else {
      /*
	We return an empty variant in all other cases. This is to prevent, for example,
	that we also see a '0' or '1' in the same column.
      */
      return QVariant();
    }
    // Only do the below if we want to fetch a tool tip.
  } else if (role == Qt::ToolTipRole) {
    // I just want the tool tip to show the data in the column.
    const QString original = QSqlTableModel::data(index, Qt::DisplayRole).toString();
    QString toolTip = breakString(original); // breakString() breaks the text in smaller lines.
    return toolTip;
  } else {
    /* 
       In all other cases, we want the default behaviour of this function. 
       This can be done easily by returning the default version of the function,
       rather than the re-implemented version we have here.
    */
    return QSqlTableModel::data(index, role);
  }
  return QVariant();
}

bool EventTableModel::setData(const QModelIndex & index,
			      const QVariant & value, int role) {
  if (index.column() == 7 && role == Qt::CheckStateRole) {
    if (value == Qt::Checked) {
      QSqlQuery *query = new QSqlQuery;
      int order = index.row() + 1;
      query->prepare("UPDATE incidents SET mark = 1 WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      delete query;
      return true;
    } else if (value == Qt::Unchecked) {
      QSqlQuery *query = new QSqlQuery;
      int order = index.row() + 1;
      query->prepare("UPDATE incidents SET mark = 0 WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      delete query;
      return true;
    }
  }
  return QSqlTableModel::setData(index, value, role);
}

Qt::ItemFlags EventTableModel::flags(const QModelIndex & index) const {
  if (index.column() == 7) {
    return QSqlTableModel::flags(index) | Qt::ItemIsUserCheckable;
  }
  return QSqlTableModel::flags(index);
}
