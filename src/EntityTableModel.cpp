/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

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

#include "../include/EntityTableModel.h"

EntityTableModel::EntityTableModel(QWidget *parent) : QSqlTableModel(parent)
{
}

QVariant EntityTableModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::ToolTipRole)
    {
      int row = index.row();
      int col = index.column();
      QModelIndex newIndex = this->createIndex(row, col + 1);
      const QString original = QSqlTableModel::data(newIndex, Qt::DisplayRole).toString();
      QString toolTip = breakString(original);
      return toolTip;
    }
  else
    {
      return QSqlTableModel::data(index, role);
    }
  return QVariant();
}

