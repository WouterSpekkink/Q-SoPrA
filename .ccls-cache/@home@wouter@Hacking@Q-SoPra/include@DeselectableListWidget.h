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

#ifndef DESELECTABLELISTWIDGET_H
#define DESELECTABLELISTWIDGET_H

#include <QTableWidget>
#include <QMouseEvent>

class DeselectableListWidget : public QTableWidget
{
  Q_OBJECT

public:
  // Constructor and destructor
  DeselectableListWidget(QWidget *parent);
  ~DeselectableListWidget() {};

signals:
  void noneSelected();

private slots:
  // Private member functions
  void resetSelection();

protected:
  // Event overrides
  void mousePressEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
};

  
#endif
