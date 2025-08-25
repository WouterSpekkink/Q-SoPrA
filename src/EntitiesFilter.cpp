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

#include "../include/EntitiesFilter.h"

EntitiesFilter::EntitiesFilter(QObject *parent) : QSortFilterProxyModel(parent)
{
}

bool EntitiesFilter::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
  RelationshipsDialog *dialog = qobject_cast<RelationshipsDialog*>(this->parent());
  if (dialog)
    {
      QModelIndex sourceIndex = sourceModel()->index(sourceRow, this->filterKeyColumn(), sourceParent);
      QString source = dialog->selectedSourceLabel->text();
      QString target = dialog->selectedTargetLabel->text();
      QString key = sourceModel()->data(sourceIndex, Qt::DisplayRole).toString();
      if (key == source || key == target)
	{
	  return false;
	}
    }
  return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
