#include "../include/EventTableModel.h"

EventTableModel::EventTableModel(QWidget *parent) : QSqlTableModel(parent) {};

QVariant EventTableModel::data(const QModelIndex &index, int role) const {
  if (index.column() == 7) {
    if (role == Qt::CheckStateRole) {
      QSqlQuery *query = new QSqlQuery;
      int order = index.row() + 1;
      query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int mark = query->value(0).toInt();
      if (mark == 1) {
	return Qt::Checked;
      } else if (mark == 0) {
	return Qt::Unchecked;
      }
    } else {
      return QVariant();
    }
  } else if (role == Qt::ToolTipRole) {
    const QString original = QSqlTableModel::data(index, Qt::DisplayRole).toString();
    QString toolTip = breakString(original);
    return toolTip;
  } else {
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
