#include "../include/RelationshipTreeFilter.h"

RelationshipTreeFilter::RelationshipTreeFilter(QObject *parent) : QSortFilterProxyModel(parent) {

}

bool RelationshipTreeFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
  QStandardItemModel *currentModel = qobject_cast<QStandardItemModel*>(sourceModel());
  
  QStandardItem *currentItem = currentModel->itemFromIndex(sourceModel()->index(sourceRow, 0, sourceParent));
  return (!currentItem->parent() || (currentItem->data(Qt::DisplayRole).toString().contains(filterRegExp())));
}
