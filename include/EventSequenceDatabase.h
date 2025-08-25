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


#ifndef EVENTSEQUENCEDATABASE_H
#define EVENTSEQUENCEDATABASE_H

#include <QtSql>
#include <QObject>
#include <QMessageBox>
#include <QPointer>

class EventSequenceDatabase : public QObject
{
  // WelcomeDialog needs access to some functions of this class
  friend class WelcomeDialog;
  Q_OBJECT

public:
  // Constructor and deconstructor
  EventSequenceDatabase();
  ~EventSequenceDatabase() {};

  // Override of public function
  void openDB(const QString &name);
  
private:
  // Private variable
  QSqlDatabase database;
  
};

#endif
