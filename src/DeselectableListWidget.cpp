#include "../include/DeselectableListWidget.h"

DeselectableListWidget::DeselectableListWidget(QWidget *parent) : QTableWidget(parent) {}

void DeselectableListWidget::mousePressEvent(QMouseEvent *event) {
  QModelIndex item = indexAt(event->pos());
  QTableWidget::mousePressEvent(event);
  if ((item.row() == -1 && item.column() == -1)) {
    clearSelection();
    const QModelIndex index;
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    emit noneSelected();
  } 
}

void DeselectableListWidget::resetSelection() {
  clearSelection();
  const QModelIndex index;
  selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
}

void DeselectableListWidget::paintEvent(QPaintEvent *event) {
  QTableWidget::paintEvent(event);
}


 
