#include "../include/EventTableModel.h"

EventTableModel::EventTableModel(QWidget *parent) : QSqlTableModel(parent) {};

QVariant EventTableModel::data(const QModelIndex &index, int role) const {
  
  if (role == Qt::ToolTipRole) {
    QString toolTip = QString("<FONT SIZE = 5>");
    toolTip += QSqlTableModel::data(index, Qt::DisplayRole).toString();
    toolTip += QString("</FONT>");
    return toolTip;
  } else {
    return QSqlTableModel::data(index, role).toString();
  }
				      
  return QVariant();
}

