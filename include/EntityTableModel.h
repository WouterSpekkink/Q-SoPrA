#ifndef ENTITYTABLEMODEL_H
#define ENTITYTABLEMODEL_H

#include <QtSql>
#include "SupportingFunctions.h"

class EntityTableModel : public QSqlTableModel
{
  Q_OBJECT
  friend class RelationshipsDialog;
  
public:
  EntityTableModel(QWidget *parent=0);
  ~EntityTableModel() {};

protected:
  QVariant data(const QModelIndex &index, int role) const;
  
};

#endif
