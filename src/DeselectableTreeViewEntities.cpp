#include "../include/DeselectableTreeViewEntities.h"

DeselectableTreeViewEntities::DeselectableTreeViewEntities(QWidget *parent) : QTreeView(parent) 
{}

void DeselectableTreeViewEntities::mousePressEvent(QMouseEvent *event) 
{
  QModelIndex item = indexAt(event->pos());
  bool selected = selectionModel()->isSelected(indexAt(event->pos()));
  QTreeView::mousePressEvent(event);
  if ((item.row() == -1 && item.column() == -1) || selected) 
    {
      clearSelection();
      const QModelIndex index;
      selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
      emit noneSelected();

    } 
}

void DeselectableTreeViewEntities::wheelEvent(QWheelEvent *event) 
{
  if (event->modifiers() & Qt::ControlModifier) 
    {
      if (event->angleDelta().y() > 0) 
	{
	  QFont font = this->font();
	  int currentSize = font.pointSize();
	  if (currentSize <= 50) 
	    {
	      int newSize = currentSize + 1;
	      font.setPointSize(newSize);
	      setFont(font);
	    }
	  return;
	}
      else if (event->angleDelta().y() < 0) 
	{
	  QFont font = this->font();
	  int currentSize = font.pointSize();
	  if (currentSize >= 10) 
	    {
	      int newSize = currentSize - 1;
	      font.setPointSize(newSize);
	      setFont(font);
	    }
	  return;
	}
    }
  else 
    {
      QTreeView::wheelEvent(event);
    }
}

void DeselectableTreeViewEntities::dropEvent(QDropEvent *event) 
{
  setSortingEnabled(false);
  QModelIndex targetIndex = indexAt(event->pos());
  QString childName = selectionModel()->currentIndex().data().toString();
  QString targetName = "";
  if (dropIndicatorPosition() == QAbstractItemView::OnItem) 
    {
      targetName = model()->data(targetIndex).toString();
    }
  else if (dropIndicatorPosition() == QAbstractItemView::AboveItem ||
	   dropIndicatorPosition() == QAbstractItemView::BelowItem) 
    {
      targetName = model()->data(targetIndex.parent()).toString();
    }
  else if (dropIndicatorPosition() == QAbstractItemView::OnViewport) 
    {
      targetName = "NONE";
    }
  if (targetName == "") 
    {
      targetName = "NONE";
    }
  if (targetName == childName) 
    {
      targetName = "NONE";
    }
  QSqlQuery* query = new QSqlQuery;  
  query->prepare("UPDATE entity_attributes SET father = :father WHERE name = :child");
  query->bindValue(":father", targetName);
  query->bindValue(":child", childName);
  query->exec();
  delete query;
  QTreeView::dropEvent(event);
  setSortingEnabled(true);
  model()->sort(0, Qt::AscendingOrder);
  sortByColumn(0, Qt::AscendingOrder);
}

void DeselectableTreeViewEntities::resetSelection() 
{
  clearSelection();
  const QModelIndex index;
  selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
}



