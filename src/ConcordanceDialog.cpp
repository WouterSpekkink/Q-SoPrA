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

#include "../include/ConcordanceDialog.h"

ConcordanceDialog::ConcordanceDialog(QWidget *parent,
				     QVector<QGraphicsItem*> drawItems,
				     bool linePlot)
: QDialog(parent), _drawItems(drawItems)
{
  _colorsOn = linePlot;
  scene = new Scene(this);
  QVectorIterator<QGraphicsItem*> it(_drawItems);
  while (it.hasNext())
    {
      scene->addItem(it.next());
    }
  view = new GraphicsView(scene);
  view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  QRectF currentRect = this->scene->itemsBoundingRect();
  currentRect.setX(currentRect.x() - 50);
  currentRect.setY(currentRect.y() - 50);
  currentRect.setWidth(currentRect.width() + 100);
  currentRect.setHeight(currentRect.height() + 100);
  scene->setSceneRect(currentRect);
  
  zoomLabel = new QLabel(tr("<b>Zoom slider:</b>"), this);

  QString sliderSheet = QString("QSlider::groove:horizontal { "
				"border: 1px solid #999999; "
				"height: 6px; "
				"background: white; "
				"margin 2px 0; "
				"border-radius: 3px;} "
				"QSlider::handle:horizontal { "
				"background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
				"stop:0 #b4b4b4, stop:1 #8f8f8f);"
				"border: 1px solid #5c5c5c; "
				"width: 18px; "
				"margin: -2px 0; "
				"border-radius: 3px;} "
				"QSlider::groove:horizontal:disabled { "
				"background: #b2b1b1;} "
				"QSlider::handle:horizontal:disabled { "
				"background: #b2b1b1; "
				"border: 1px solid #787676}");
  setStyleSheet(sliderSheet);
  
  zoomSlider = new QSlider(Qt::Horizontal, this);
  zoomSlider->installEventFilter(this);
  zoomSlider->setMinimum(-9);
  zoomSlider->setMaximum(9);
  zoomSlider->setValue(0);
  
  exportButton = new QPushButton(tr("Export plot"), this);
  closeButton = new QPushButton(tr("Close dialog"), this);
  if (linePlot)
    {
      toggleColorsButton = new QPushButton(tr("Toggle colors"), this);
    }

  connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(processZoomSliderChange(int)));
  connect(zoomSlider, SIGNAL(sliderReleased()), this, SLOT(resetZoomSlider()));
  if (linePlot)
    {
      connect(toggleColorsButton, SIGNAL(clicked()), this, SLOT(toggleColors()));
    }
  connect(exportButton, SIGNAL(clicked()), this, SLOT(exportPlot()));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(view);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(zoomLabel);
  zoomLabel->setMaximumWidth(zoomLabel->sizeHint().width());
  buttonLayout->addWidget(zoomSlider);
  zoomSlider->setMaximumWidth(100);
  if (linePlot)
    {
      buttonLayout->addWidget(toggleColorsButton);
      toggleColorsButton->setMaximumWidth(toggleColorsButton->sizeHint().width());
    }
  buttonLayout->addWidget(exportButton);
  exportButton->setMaximumWidth(exportButton->sizeHint().width());
  buttonLayout->addWidget(closeButton);
  closeButton->setMaximumWidth(closeButton->sizeHint().width());
  buttonLayout->setAlignment(Qt::AlignRight);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  if (_colorsOn)
    {
      QVectorIterator<QGraphicsItem*> it(_drawItems);
      while (it.hasNext())
	{
	  QGraphicsItem *current = it.next();
	  QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem*>(current);
	  if (line)
	    {
	      originalColors.insert(line, line->pen().color());
	    }
	}
    }
}

ConcordanceDialog::~ConcordanceDialog()
{
  qDeleteAll(_drawItems);
  _drawItems.clear();
  delete scene;
  delete view;
}

void ConcordanceDialog::processZoomSliderChange(int value)
{
  while (zoomSlider->sliderPosition() != 0)
    {
      view->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
      if (value < 0)
	{
	  double scale = (value * -1) / 250.0;
	  view->scale(1.0 / (1 + scale), 1.0 / (1 + scale));
	}
      else
	{
	  double scale = value / 250.0;
	  view->scale(1 + scale, 1 + scale);
	}
      qApp->processEvents();
      if (!zoomSlider->isSliderDown()) {
	break;
      }
    }
  resetZoomSlider();
}

void ConcordanceDialog::resetZoomSlider()
{
  zoomSlider->setValue(0);
  zoomSlider->setSliderPosition(0);
}

void ConcordanceDialog::toggleColors()
{
  QVectorIterator<QGraphicsItem*> it(_drawItems);
  if (_colorsOn)
    {
      while (it.hasNext())
	{
	  QGraphicsItem *current = it.next();
	  QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem*>(current);
	  if (line)
	    {
	      line->setPen(QPen(Qt::black, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
	    }
	}
    }
  else
    {
      while (it.hasNext())
	{
	  QGraphicsItem *current = it.next();
	  QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem*>(current);
	  QColor color = originalColors.value(line);
	  if (line)
	    {
	      line->setPen(QPen(color, 1, Qt::PenStyle(1), Qt::SquareCap, Qt::MiterJoin));
	    }
	}
    }
  _colorsOn = !_colorsOn;
}

void ConcordanceDialog::exportPlot()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("New svg file"),""
						  , tr("svg files (*.svg)"));
  if (!fileName.trimmed().isEmpty()) 
    {
      if (!fileName.endsWith(".svg")) 
	{
	  fileName.append(".svg");
	}
      QSvgGenerator gen;
      gen.setFileName(fileName);
      QRectF currentRect = scene->itemsBoundingRect();
      currentRect.setX(currentRect.x());
      currentRect.setY(currentRect.y());
      currentRect.setWidth(currentRect.width());
      currentRect.setHeight(currentRect.height());
      gen.setSize(QSize(currentRect.width(), currentRect.height()));
      gen.setViewBox(QRect(0, 0, currentRect.width(), currentRect.height()));
      int dpiX = view->logicalDpiX();
      gen.setResolution(dpiX);
      QPainter painter;
      painter.begin(&gen);
      scene->render(&painter);
      painter.end();
    }
}

bool ConcordanceDialog::eventFilter(QObject *object, QEvent *event) 
{
  if (event->type() == QEvent::MouseButtonRelease)
    {
      resetZoomSlider();
    }
  else if (object == zoomSlider)
    {
      if (event->type() == QEvent::Wheel ||
	  event->type() == QEvent::MouseButtonDblClick)
	{
	  event->ignore();
	  return true;
	}
      else if (event->type() == QEvent::MouseButtonPress)
	{
	  if (zoomSlider->sliderPosition() == 0)
	    {
	      return false;
	    }
	  else
	    {
	      event->ignore();
	      return true;
	    }
	}
    }
  return false;
}
