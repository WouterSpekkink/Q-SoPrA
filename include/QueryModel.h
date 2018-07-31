#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include <QtSql>
#include "SupportingFunctions.h"

class QueryModel : public QSqlQueryModel {
  Q_OBJECT

public:
  QueryModel(QWidget *parent=0);
  ~QueryModel() {};
  
protected:
  QVariant data(const QModelIndex &index, int role) const;

};

#endif
