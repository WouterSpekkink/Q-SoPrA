#ifndef ENTITIESFILTER_H
#define ENTITIESFILTER_H

#include <QSortFilterProxyModel>
#include "RelationshipsDialog.h"

class EntitiesFilter : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  EntitiesFilter(QObject *parent=0);
  ~EntitiesFilter() {};

protected:
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
  
};

#endif
