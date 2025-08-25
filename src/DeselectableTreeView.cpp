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

#include "../include/DeselectableTreeView.h"

DeselectableTreeView::DeselectableTreeView(QWidget *parent) : QTreeView(parent) 
{
}

void DeselectableTreeView::mousePressEvent(QMouseEvent *event) 
{
  if (event->button() == Qt::RightButton) 
    {
      QTreeView::mousePressEvent(event);
    }
  else 
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
}

void DeselectableTreeView::dropEvent(QDropEvent *event) 
{
  setSortingEnabled(false);
  QModelIndex targetIndex = indexAt(event->position().toPoint());
  QString childName = selectionModel()->currentIndex().data().toString();
  bool entity = false;
  QModelIndex tempIndex = selectionModel()->currentIndex();
  while (tempIndex.parent().isValid()) 
    {
      tempIndex = tempIndex.parent();
    }
  QString topName = tempIndex.data().toString();
  if (topName == ENTITIES) 
    {
      entity = true;
    }
  QString targetName = "";
  if (dropIndicatorPosition() == QAbstractItemView::OnItem) 
    {
      targetName = model()->data(targetIndex).toString();
    }
  else if (dropIndicatorPosition() == QAbstractItemView::AboveItem ||
	   dropIndicatorPosition() == QAbstractItemView::BelowItem) 
    {
      if ((model()->data(targetIndex).toString() == ENTITIES) & entity)
	{
	  event->ignore();
	  return;
	}
      else
	{
	  targetName = model()->data(targetIndex.parent()).toString();
	}
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
  if (targetName == ENTITIES) 
    {
      targetName = "NONE";
    }
  if (entity) 
    {
      QModelIndex fatherIndex = targetIndex;
      while (fatherIndex.parent().isValid()) 
	{
	  fatherIndex = fatherIndex.parent();
	}
      QString topName = fatherIndex.data().toString();
      if (topName != ENTITIES) 
	{
	  event->ignore();
	}
      else 
	{
	  QSqlQuery* query = new QSqlQuery;  
	  query->prepare("UPDATE entities SET father = :father WHERE name = :child");
	  query->bindValue(":father", targetName);
	  query->bindValue(":child", childName);
	  query->exec();
	  delete query;
	  QTreeView::dropEvent(event);
	  setSortingEnabled(true);
	  model()->sort(0, Qt::AscendingOrder);
	  sortByColumn(0, Qt::AscendingOrder);
	}
    }
  else 
    {
      QModelIndex fatherIndex = targetIndex;
      while (fatherIndex.parent().isValid()) 
	{
	  fatherIndex = fatherIndex.parent();
	}
      QString topName = fatherIndex.data().toString();
      if (topName == ENTITIES) 
	{
	  event->ignore();
	}
      else 
	{
	  QSqlQuery* query = new QSqlQuery;  
	  query->prepare("UPDATE incident_attributes SET father = :father WHERE name = :child");
	  query->bindValue(":father", targetName);
	  query->bindValue(":child", childName);
	  query->exec();
	  delete query;
	  QTreeView::dropEvent(event);
	  setSortingEnabled(true);
	  model()->sort(0, Qt::AscendingOrder);
	  sortByColumn(0, Qt::AscendingOrder);
	}    
    }
}

void DeselectableTreeView::resetSelection() 
{
  clearSelection();
  const QModelIndex index;
  selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
}



