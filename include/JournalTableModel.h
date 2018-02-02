#ifndef JOURNALTABLEMODEL_H
#define JOURNALTABLEMODEL_H

#include <QtSql>
#include "SupportingFunctions.h"

class JournalTableModel : public QSqlTableModel {
  Q_OBJECT

public:
  JournalTableModel(QWidget *parent=0);
  ~JournalTableModel() {};  

  Qt::ItemFlags flags(const QModelIndex & index) const;
  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
  
};

#endif
