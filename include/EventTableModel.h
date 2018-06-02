#ifndef EVENTTABLEMODEL_H
#define EVENTTABLEMODEL_H

o#include <QtSql>
#include "SupportingFunctions.h"

class EventTableModel : public QSqlTableModel {
  Q_OBJECT

public:
  EventTableModel(QWidget *parent=0);
  ~EventTableModel() {};  

  Qt::ItemFlags flags(const QModelIndex & index) const;
  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
  
};

#endif
