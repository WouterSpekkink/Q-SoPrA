#ifndef RELATIONALTABLE_H
#define RELATIONALTABLE_H

#include <QtSql>
#include "SupportingFunctions.h"

class RelationalTable : public QSqlRelationalTableModel {
  Q_OBJECT

public:
  RelationalTable(QWidget *parent=0);
  ~RelationalTable() {};
  
protected:
  QVariant data(const QModelIndex &index, int role) const;

};

#endif
