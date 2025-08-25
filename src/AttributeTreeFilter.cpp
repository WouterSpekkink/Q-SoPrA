
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

#include "../include/AttributeTreeFilter.h"

AttributeTreeFilter::AttributeTreeFilter(QObject *parent) : QSortFilterProxyModel(parent)
{
}

bool AttributeTreeFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
  QModelIndex sourceIndex = sourceModel()->index(sourceRow, this->filterKeyColumn(), sourceParent);
  if (sourceIndex.isValid())
    {
      for (int i = 0; i < sourceModel()->rowCount(sourceIndex); ++i)
	{
	  if (filterAcceptsRow(i, sourceIndex))
	    {
	      return true;
	    }
	}
      QString key = sourceModel()->data(sourceIndex, Qt::DisplayRole).toString();
      return key.contains(filterRegularExpression());
    }
  QStandardItemModel *currentModel = qobject_cast<QStandardItemModel*>(sourceModel());
  QStandardItem *currentItem = currentModel->itemFromIndex(sourceModel()->
							   index(sourceRow, 0, sourceParent));
  return (!currentItem->parent() || (currentItem->data(Qt::DisplayRole).
				     toString().contains(filterRegularExpression())));
}
