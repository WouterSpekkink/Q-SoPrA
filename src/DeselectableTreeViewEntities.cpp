/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2025 Wouter Spekkink  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

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
  QModelIndex targetIndex = indexAt(event->position().toPoint());
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



