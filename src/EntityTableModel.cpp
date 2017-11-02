#include "../include/EntityTableModel.h"

EntityTableModel::EntityTableModel(QWidget *parent) : QSqlTableModel(parent) {};

QVariant EntityTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::ToolTipRole) {
    QString toolTip = QString("<FONT SIZE = 3>");
    int row = index.row();
    int col = index.column();
    QModelIndex newIndex = this->createIndex(row, col + 1);
    toolTip += QSqlTableModel::data(newIndex, Qt::DisplayRole).toString();
    toolTip += QString("</FONT>");
    return toolTip;
  } else {
    return QSqlTableModel::data(index, role);
  }
  return QVariant();
}

