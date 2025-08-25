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

#ifndef ZOOMABLETABLEVIEW_H
#define ZOOMABLETABLEVIEW_H

#include <QTableView>
#include <QWheelEvent>

class ZoomableTableView : public QTableView
{
  Q_OBJECT

public:
  // Constructor and destructor
  ZoomableTableView(QWidget *parent = 0);
  ~ZoomableTableView() {};
  
protected:
  // Override of event
  void wheelEvent(QWheelEvent *event) override;
};

#endif
