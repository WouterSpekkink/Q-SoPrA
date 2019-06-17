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

#ifndef CONCORDANCEDIALOG_H
#define CONCORDANCEDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QSvgGenerator>
#include "GraphicsView.h"
#include "Scene.h"

class ConcordanceDialog : public QDialog
{
  Q_OBJECT
  
public:
  ConcordanceDialog(QWidget *parent = 0,
		    QVector<QGraphicsItem*> drawItems = QVector<QGraphicsItem*>());
    ~ConcordanceDialog();
private slots:
  void exportPlot();
  void processZoomSliderChange(int value);
  void resetZoomSlider();
  bool eventFilter(QObject *object, QEvent *event);
  
private:
  // Interface elements
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QLabel> zoomLabel;
  QPointer<QPushButton> exportButton;
  QPointer<QPushButton> closeButton;
  QPointer<QSlider> zoomSlider;
  
  // Private data vector
  QVector<QGraphicsItem*> _drawItems;
};

#endif
