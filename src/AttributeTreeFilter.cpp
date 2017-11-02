#include "../include/AttributeTreeFilter.h"

AttributeTreeFilter::AttributeTreeFilter(QObject *parent) : QSortFilterProxyModel(parent) {

}

bool AttributeTreeFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
  QModelIndex sourceIndex = sourceModel()->index(sourceRow, this->filterKeyColumn(), sourceParent);
  if (sourceIndex.isValid()) {
    for (int i = 0; i < sourceModel()->rowCount(sourceIndex); ++i) {
      if (filterAcceptsRow(i, sourceIndex)) {
	return true;
      }
    }
    QString key = sourceModel()->data(sourceIndex, Qt::DisplayRole).toString();
    return key.contains(filterRegExp());
  }
  QStandardItemModel *currentModel = qobject_cast<QStandardItemModel*>(sourceModel());
  QStandardItem *currentItem = currentModel->itemFromIndex(sourceModel()->index(sourceRow, 0, sourceParent));
  return (!currentItem->parent() || (currentItem->data(Qt::DisplayRole).toString().contains(filterRegExp())));
}
