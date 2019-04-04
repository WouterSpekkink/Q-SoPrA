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

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QGraphicsSceneMouseEvent>
#include "SimpleTextDialog.h"

class GraphicsView : public QGraphicsView
{
  Q_OBJECT
  
public:
  // Constructor and deconstructor
  GraphicsView(QGraphicsScene *scene);
  ~GraphicsView() {};
  
  qreal panSpeed;
  bool isPanning();
  void centerMe();

signals:
  void changedView();
  
protected:
  // Overrides of events
  void resizeEvent(QResizeEvent *) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
    
private:
  // Private variables
  bool _pan;
  qreal _scaleFact;
  qreal _panSpeed;
  QPoint _lastMousePos;
  
};


#endif
