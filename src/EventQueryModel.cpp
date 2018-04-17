#include "../include/EventQueryModel.h"

EventQueryModel::EventQueryModel(QWidget *parent) : QSqlQueryModel(parent) {};

QVariant EventQueryModel::data(const QModelIndex &index, int role) const {
  if (index.column() == 7) {
    if (role == Qt::CheckStateRole) {
      int row = index.row();
      QSqlRecord record = this->record(row);
      int order = record.value(1).toInt();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int mark = query->value(0).toInt();
      if (mark == 1) {
	delete query;
	return Qt::Checked;
      } else if (mark == 0) {
	delete query;
	return Qt::Unchecked;
      }
    } else {
      return QVariant();
    }
  } else if (role == Qt::ToolTipRole) {
    const QString original = QSqlQueryModel::data(index, Qt::DisplayRole).toString();
    QString toolTip = breakString(original);
    return toolTip;
  } else {
    return QSqlQueryModel::data(index, role);
  }
  return QVariant();
}

bool EventQueryModel::setData(const QModelIndex & index,
			      const QVariant & value, int role) {
  if (index.column() == 7 && role == Qt::CheckStateRole) {
    if (value == Qt::Checked) {
      int row = index.row();
      QSqlRecord record = this->record(row);
      int order = record.value(1).toInt();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET mark = 1 WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      delete query;
      return true;
    } else if (value == Qt::Unchecked) {
      int row = index.row();
      QSqlRecord record = this->record(row);
      int order = record.value(1).toInt();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE incidents SET mark = 0 WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      delete query;
      return true;
    }
  }
  return QSqlQueryModel::setData(index, value, role);
}

Qt::ItemFlags EventQueryModel::flags(const QModelIndex & index) const {
  if (index.column() == 7) {
    return QSqlQueryModel::flags(index) | Qt::ItemIsUserCheckable;
  }
  return QSqlQueryModel::flags(index);
}
