#ifndef EVENTQUERYMODEL_H
#define EVENTQUERYMODEL_H

#include <QtSql>
#include "SupportingFunctions.h"

class EventQueryModel : public QSqlQueryModel {
  Q_OBJECT

public:
  EventQueryModel(QWidget *parent=0);
  ~EventQueryModel() {};  

  Qt::ItemFlags flags(const QModelIndex & index) const;
  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
  
};

#endif
