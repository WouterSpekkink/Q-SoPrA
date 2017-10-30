#include "../include/DeselectableTreeView.h"

DeselectableTreeView::DeselectableTreeView(QWidget *parent) : QTreeView(parent) {}

void DeselectableTreeView::mousePressEvent(QMouseEvent *event) {
  QModelIndex item = indexAt(event->pos());
  bool selected = selectionModel()->isSelected(indexAt(event->pos()));
  QTreeView::mousePressEvent(event);
  if ((item.row() == -1 && item.column() == -1) || selected) {
    clearSelection();
    const QModelIndex index;
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
  }
}

void DeselectableTreeView::dropEvent(QDropEvent *event) {
  QModelIndex targetIndex = indexAt(event->pos());
  QModelIndex childIndex = this->selectionModel()->currentIndex();
  QString childName = this->model()->data(childIndex).toString();
  QString targetName = "";
  if (this->dropIndicatorPosition() == QAbstractItemView::OnItem) {
    targetName = this->model()->data(targetIndex).toString();
  } else if (this->dropIndicatorPosition() == QAbstractItemView::AboveItem ||
	     this->dropIndicatorPosition() == QAbstractItemView::BelowItem) {
    targetName = this->model()->data(targetIndex.parent()).toString();
    if (targetName == "") {
      targetName = "NONE";
    }
  } else if (this->dropIndicatorPosition() == QAbstractItemView::OnViewport) {
    targetName = "NONE";
  }
  QSqlQuery* query = new QSqlQuery;  
  query->prepare("UPDATE incident_attributes SET father = :father WHERE name = :child");
  query->bindValue(":father", targetName);
  query->bindValue(":child", childName);
  query->exec();
  QTreeView::dropEvent(event);
}

void DeselectableTreeView::resetSelection() {
  clearSelection();
  const QModelIndex index;
  selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
}




