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


#ifndef BANDLESSGRAPHICSVIEW_H
#define BANDLESSGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QGraphicsSceneMouseEvent>
#include "SimpleTextDialog.h"


class BandlessGraphicsView : public QGraphicsView
{
  Q_OBJECT
  
public:
  // Constructor and destructor
  BandlessGraphicsView(QGraphicsScene *scene);
  ~BandlessGraphicsView() {};

  // Getter
  bool isPanning();
  
signals:
  void HierarchyGraphContextMenuAction(const QString&, const QPoint&);
  
protected:
  // Override s
  void resizeEvent(QResizeEvent *) override;
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  // Private variables
  bool _pan;
  qreal _scaleFact;
  qreal _panSpeed;
  QPoint _lastMousePos;  
  
};


#endif
