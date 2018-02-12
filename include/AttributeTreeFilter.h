#ifndef ATTRIBUTETREEFILTER_H
#define ATTRIBUTETREEFILTER_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class AttributeTreeFilter : public QSortFilterProxyModel {
  Q_OBJECT

  friend class EntityDialog;

public:
  AttributeTreeFilter(QObject *parent=0);
  ~AttributeTreeFilter() {};

protected:
  bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
  
};

#endif
  
