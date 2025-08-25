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

#ifndef RELATIONALTABLE_H
#define RELATIONALTABLE_H

#include <QtSql>
#include "SupportingFunctions.h"

class RelationalTable : public QSqlRelationalTableModel
{
  Q_OBJECT

public:
  // Constructor and destructor
  RelationalTable(QWidget *parent=0);
  ~RelationalTable() {};
  
protected:
  // Override of data function
  QVariant data(const QModelIndex &index, int role) const override;

};

#endif
