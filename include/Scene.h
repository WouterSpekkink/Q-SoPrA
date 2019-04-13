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

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "IncidentNode.h"
#include "Linkage.h"
#include "NetworkNode.h"
#include "Constants.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>
#include <QMenu>
#include "../include/NetworkNode.h"
#include "../include/NetworkNodeLabel.h"
#include "../include/IncidentNodeLabel.h"
#include "../include/OccurrenceItem.h"
#include "../include/LineObject.h"
#include "../include/TextObject.h"
#include "../include/EllipseObject.h"
#include "../include/AbstractNodeLabel.h"
#include "../include/OccurrenceLabel.h"
#include "../include/RectObject.h"
#include "../include/TimeLineObject.h"
#include "../include/GuideLine.h"

class Scene : public QGraphicsScene
{
  Q_OBJECT

public:
  // Constructor and destructor
  Scene(QObject *parent = 0);
  ~Scene() {};
  
  // This needs to be exposed;
  QRectF itemsBoundingRect() const;
  OccurrenceItem* getSelectedOccurrence(); // Used by occurrence item
  void resetAreas();
  void stopRotation();

  // Getters
  bool isPreparingArea();
  bool isRotating();

signals:
  void posChanged(IncidentNode *, qreal &dist);
  void posChanged(AbstractNode *, qreal &dist);
  void moveItems(QGraphicsItem *, QPointF);
  void moveLine(QGraphicsItem *, QPointF);
  void relevantChange();
  void resetItemSelection();
  void IncidentNodeContextMenuAction(const QString&);
  void LinkageContextMenuAction(const QString&);
  void NetworkNodeContextMenuAction(const QString&);
  void LineContextMenuAction(const QString&);
  void TextContextMenuAction(const QString&);
  void EllipseContextMenuAction(const QString&);
  void RectContextMenuAction(const QString&);
  void TimeLineContextMenuAction(const QString&);
  void GuideLineContextMenuAction(const QString&);
  void sendLinePoints(const QPointF&, const QPointF&);
  void sendTimeLinePoints(const qreal&, const qreal&, const qreal&);
  void sendSingleArrowPoints(const QPointF&, const QPointF&);
  void sendDoubleArrowPoints(const QPointF&, const QPointF&);
  void sendEllipseArea(const QRectF&);
  void sendRectArea(const QRectF&);
  void sendTextArea(const QRectF&, const qreal&);
  void sendHorizontalGuideLinePos(const QPointF&);
  void sendVerticalGuideLinePos(const QPointF&);
						
private slots:
  // Private member functions
  void modEventWidth(QGraphicsItem* item);
  void prepLinePoints();
  void prepSingleArrowPoints();
  void prepDoubleArrowPoints();
  void prepTimeLinePoints();
  void prepEllipseArea();
  void prepRectArea();
  void prepTextArea();
  void prepHorizontalGuideLine();
  void prepVerticalGuideLine();
  void setPenStyle(int style);
  void setPenWidth(int width);
  void setLineColor(QColor &color);
  void setFillColor(QColor &color);
  void setMajorInterval(qreal &majorInterval);
  void setMinorDivision(qreal &minorDivision);
  void setMajorTickSize(qreal &majorTickSize);
  void setMinorTickSize(qreal &minorTickSize);
  void setTimeLineWidth(int width);
  void setTimeLineColor(QColor &color);
  
protected:
  // Overrides of events
  void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private:
  // Private variables
  bool _resizeOnIncidentNode;
  bool _resizeOnAbstractNode;
  bool _moveOn;
  bool _lineMoveOn;
  bool _moveLineObject;
  bool _manipulateEllipse;
  bool _moveEllipse;
  bool _manipulateRect;
  bool _rotateEllipse;
  bool _moveRect;
  bool _rotateRect;
  bool _moveText;
  bool _manipulateText;
  bool _rotateText;
  bool _moveTimeLine;
  bool _manipulateTimeLine;
  bool _moveGuideLine;
  bool _hierarchyMove;
  bool _eventWidthChange;
  bool _moveNetworkNodeLabel;
  bool _gettingLinePoints;
  bool _linePointsStarted;
  bool _gettingSingleArrowPoints;
  bool _singleArrowPointsStarted;
  bool _gettingDoubleArrowPoints;
  bool _doubleArrowPointsStarted;
  bool _gettingTimeLinePoints;
  bool _timeLinePointsStarted;
  bool _gettingEllipseArea;
  bool _ellipseAreaStarted;
  bool _gettingRectArea;
  bool _rectAreaStarted;
  bool _gettingTextArea;
  bool _textAreaStarted;
  bool _gettingHorizontalGuideLine;
  bool _gettingVerticalGuideLine;
  QPointF _lastMousePos;
  QPointF _initPos;
  QRectF _drawArea;
  QPointF _lineStart;
  QPointF _lineEnd;
  int _currentPenStyle;
  int _currentPenWidth;
  QColor _currentLineColor;
  QColor _currentFillColor;
  qreal _currentMajorInterval;
  qreal _currentMinorDivision;
  qreal _currentMajorTickSize;
  qreal _currentMinorTickSize;
  int _currentTimeLineWidth;
  QColor _currentTimeLineColor;
  
  // Drawing aids
  LineObject * _tempLinePtr;
  EllipseObject *_tempEllipsePtr;
  RectObject *_tempRectPtr;
  TextObject *_tempTextPtr;
  TimeLineObject *_tempTimeLinePtr;
  GuideLine * _tempGuideLinePtr;
  
  // Pointers to objects created by other classes
  // Do not delete
  IncidentNode *_selectedIncidentNodePtr;
  AbstractNode *_selectedAbstractNodePtr;
  NetworkNode *_selectedNetworkNodePtr;
  NetworkNodeLabel *_selectedNetworkNodeLabelPtr;
  OccurrenceItem *_selectedOccurrencePtr;
  LineObject *_selectedLinePtr;
  EllipseObject *_selectedEllipsePtr;
  RectObject *_selectedRectPtr;
  TextObject *_selectedTextPtr;
  TimeLineObject *_selectedTimeLinePtr;
  GuideLine* _selectedGuideLine;
};

#endif 
