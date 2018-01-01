#include "../include/EntityTableModel.h"

EntityTableModel::EntityTableModel(QWidget *parent) : QSqlTableModel(parent) {};

QVariant EntityTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::ToolTipRole) {
    int row = index.row();
    int col = index.column();
    QModelIndex newIndex = this->createIndex(row, col + 1);
    const QString original = QSqlTableModel::data(newIndex, Qt::DisplayRole).toString();
    QString toolTip = breakString(original);
    return toolTip;
  } else {
    return QSqlTableModel::data(index, role);
  }
  return QVariant();
}

