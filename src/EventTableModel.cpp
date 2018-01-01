#include "../include/EventTableModel.h"

EventTableModel::EventTableModel(QWidget *parent) : QSqlTableModel(parent) {};

QVariant EventTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::ToolTipRole) {
    const QString original = QSqlTableModel::data(index, Qt::DisplayRole).toString();
    QString toolTip = breakString(original);
    return toolTip;
  } else {
    return QSqlTableModel::data(index, role);
  }
  return QVariant();
}

