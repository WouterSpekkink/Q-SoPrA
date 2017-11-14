#include "../include/EntitiesFilter.h"

EntitiesFilter::EntitiesFilter(QObject *parent) : QSortFilterProxyModel(parent) {}

bool EntitiesFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
  RelationshipsDialog *dialog = qobject_cast<RelationshipsDialog*>(this->parent());
  if (dialog) {
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, this->filterKeyColumn(), sourceParent);
    QString source = dialog->selectedSourceLabel->text();
    QString target = dialog->selectedTargetLabel->text();
    QString key = sourceModel()->data(sourceIndex, Qt::DisplayRole).toString();
    if (key == source || key == target) {
      return false;
    }
  }
  return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
