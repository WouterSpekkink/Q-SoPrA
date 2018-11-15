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

#ifndef EVENTTABLEMODEL_H
#define EVENTTABLEMODEL_H

#include <QtSql>
#include "SupportingFunctions.h"

class EventTableModel : public QSqlTableModel
{
  Q_OBJECT

public:
  // Constructor and destructor
  EventTableModel(QWidget *parent=0);
  ~EventTableModel() {};  

  // Override of publif functions
  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex & index) const;
};

#endif
