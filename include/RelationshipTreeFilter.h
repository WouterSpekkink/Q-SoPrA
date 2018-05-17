#ifndef RELATIONSHIPTREEFILTER_H
#define RELATIONSHIPTREEFILTER_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class RelationshipTreeFilter : public QSortFilterProxyModel {
  Q_OBJECT
  
public:
  RelationshipTreeFilter(QObject *parent=0);
  ~RelationshipTreeFilter() {};

protected:
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
  
};

#endif
