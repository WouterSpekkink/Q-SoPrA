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

#ifndef VISUALLINKAGEWIDGET_H
#define VISUALLINKAGEWIDGET_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSlider>
#include <QVector>
#include <QPointer>
#include <QtSql>
#include <QMessageBox>
#include <QColorDialog>
#include <QListWidget>
#include <QWheelEvent>
#include <math.h>
#include <vector>
#include "IncidentNodeLabel.h"
#include "BandlessGraphicsView.h"
#include "IncidentNode.h"
#include "Linkage.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "LargeTextDialog.h"
#include "DeselectableListWidget.h"
#include "SortFunctions.h"
#include "SupportingFunctions.h"


class VisualLinkageWidget : public QWidget
{
  Q_OBJECT
  // MainWindow needs access to this class
  friend class MainWindow;
  
public:
  // Constructor and deconstructor
  VisualLinkageWidget(QWidget *parent = 0);
  ~VisualLinkageWidget();

private:
  // Interface elements
  QPointer<Scene> scene;
  QPointer<BandlessGraphicsView> view;

  
};
  
#endif
