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

#ifndef OCCURRENCEGRAPHWIDGET
#define OCCURRENCEGRAPHWIDGET

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QVector>
#include <QPointer>
#include <QtSql>
#include <QMessageBox>
#include <QSvgGenerator>
#include <QFileDialog>
#include <math.h>
#include <QGraphicsTextItem>
#include <QColorDialog>
#include <vector>
#include "OccurrenceLabel.h"
#include "GraphicsView.h"
#include "OccurrenceItem.h"
#include "Linkage.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "SavedPlotsDialog.h"
#include "AttributeColorDialog.h"
#include "LargeTextDialog.h"
#include "DeselectableTreeView.h"
#include "AttributeTreeFilter.h"
#include "AttributeDialog.h"
#include "DeselectableListWidget.h"
#include "EventGraphWidget.h"
#include "IncidentNode.h"
#include "SortFunctions.h"
#include "SupportingFunctions.h"
#include "LargeTextDialog.h"
#include "LineObject.h"
#include "EllipseObject.h"
#include "RectObject.h"
#include "TextObject.h"
#include "RelationshipColorDialog.h"
#include "ModeColorDialog.h"

class EventGraphWidget;

class OccurrenceGraphWidget : public QWidget
{
  Q_OBJECT
  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  OccurrenceGraphWidget(QWidget *parent = 0);
  ~OccurrenceGraphWidget();

  // Setters
  void setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr);
  void setOpenGL(bool state);
  void setAntialiasing(bool state);

  // Other functions that need to be exposed
  bool attributesPresent();
  bool relationshipsPresent();
  void updateCases();
		    
private slots:
  // Private member functions
  void checkCongruency();
  void toggleLegend();
  void toggleGraphicsControls();
  void toggleTimeLine();
  void rescale();
  void checkCases();
  void addAttribute();
  void addRelationship();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  void setAttributeModeButton(QTableWidgetItem *item);
  void disableAttributeModeButton();
  void removeAttributeMode();
  void changeAttributeModeColor(QTableWidgetItem *item);
  void setRelationshipModeButton(QTableWidgetItem *item);
  void disableRelationshipModeButton();
  void removeRelationshipMode();
  void changeRelationshipModeColor(QTableWidgetItem *item);
  void wireLinkages();
  void groupOccurrences();
  void reset();
  void matchEventGraph();
  void restore();
  void plotLabels();
  void changeLabels();
  void setBackgroundColor();
  void increaseDistance();
  void decreaseDistance();
  void processZoomSliderChange(int value);
  void resetZoomSlider();
  void setGraphControls(bool status);
  void processLowerRange(int value);
  void processUpperRange(int value);
  void setVisibility();
  void setRangeControls();
  void exportSvg();
  void exportMatrix();
  void setChangeLabel();
  void updateLinkages();
  void processMoveItems(QGraphicsItem *item, QPointF pos);
  void processMoveLine(QGraphicsItem *item, QPointF pos);
  void addLineObject(const QPointF &start, const QPointF &end);
  void addSingleArrowObject(const QPointF &start, const QPointF &end);
  void addDoubleArrowObject(const QPointF &start, const QPointF &end);
  void addEllipseObject(const QRectF &area);
  void addRectObject(const QRectF &area);
  void addTextObject(const QRectF &area, const qreal &size);
  void addTimeLineObject(const qreal &startX, const qreal &endX, const qreal &y);
  void setPenStyle();
  void setPenWidth();
  void setLineColor();
  void setFillColor();
  void setMajorInterval();
  void setMinorDivision();
  void setMajorTickSize();
  void setMinorTickSize();
  void setTimeLineWidth();
  void setTimeLineColor();
  void processShapeSelection();
  void processLineContextMenu(const QString &action);
  void changeLineColor();
  void toggleArrow1();
  void toggleArrow2();
  void deleteLine();
  void processTextContextMenu(const QString &action);
  void changeText();
  void changeTextColor();
  void deleteText();
  void processEllipseContextMenu(const QString &action);
  void changeEllipseColor();
  void changeEllipseFillColor();
  void deleteEllipse();
  void processRectContextMenu(const QString &action);
  void changeRectColor();
  void changeRectFillColor();
  void deleteRect();
  void processTimeLineContextMenu(const QString &action);
  void changeTimelineColor();
  void deleteTimeLine();
  void duplicateTimeLine();
  void duplicateLine();
  void duplicateText();
  void duplicateEllipse();
  void duplicateRect();
  void objectOneForward();
  void objectOneBackward();
  void objectToFront();
  void objectToBack();
  void fixZValues();
  void saveCurrentPlot();
  void seePlots();
  void cleanUp();
  void finalBusiness();
  bool eventFilter(QObject *object, QEvent *event);

signals:
  void sendLineColor(QColor&);
  void sendFillColor(QColor&);
  void sendMajorInterval(qreal&);
  void sendMinorDivision(qreal&);
  void sendMajorTickSize(qreal&);
  void sendMinorTickSize(qreal&);
  void sendTimeLineWidth(int);
  void sendTimeLineColor(QColor&);
  
private:
  // Interface elements
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> legendWidget;
  QPointer<QWidget> timeLineWidget;
  QPointer<QLabel> attributeLegendLabel;
  QPointer<QLabel> relationshipLegendLabel;
  QPointer<QLabel> plotLabel;
  QPointer<QLabel> changeLabel;
  QPointer<QLabel> incongruencyLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;
  QPointer<QLabel> casesLabel;
  QPointer<QLabel> zoomLabel;
  QPointer<QLabel> shapesLabel;
  QPointer<QLabel> penStyleLabel;
  QPointer<QLabel> penWidthLabel;
  QPointer<QLabel> lineColorLabel;
  QPointer<QLabel> fillColorLabel;
  QPointer<QLabel> timeLineLabel;
  QPointer<QLabel> majorIntervalLabel;
  QPointer<QLabel> minorDivisionLabel;
  QPointer<QLabel> majorTickSizeLabel;
  QPointer<QLabel> minorTickSizeLabel;
  QPointer<QLabel> timeLineWidthLabel;
  QPointer<QLabel> timeLineColorLabel;
  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> toggleTimeLineButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> addRelationshipButton;
  QPointer<QPushButton> matchEventGraphButton;
  QPointer<QPushButton> restoreButton;
  QPointer<QPushButton> plotLabelsButton;
  QPointer<QPushButton> changeLabelsButton;
  QPointer<QPushButton> backgroundColorButton;
  QPointer<QPushButton> increaseDistanceButton;
  QPointer<QPushButton> decreaseDistanceButton;
  QPointer<QPushButton> removeAttributeModeButton;
  QPointer<QPushButton> removeRelationshipModeButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> exportMatrixButton;
  QPointer<QPushButton> addLineButton;
  QPointer<QPushButton> addSingleArrowButton;
  QPointer<QPushButton> addDoubleArrowButton;
  QPointer<QPushButton> addEllipseButton;
  QPointer<QPushButton> addRectangleButton;
  QPointer<QPushButton> addTextButton;
  QPointer<QPushButton> changeLineColorButton;
  QPointer<QPushButton> changeFillColorButton;
  QPointer<QPushButton> addTimeLineButton;
  QPointer<QPushButton> changeTimeLineColorButton;
  QPointer<DeselectableListWidget> attributeListWidget;
  QPointer<DeselectableListWidget> relationshipListWidget;
  QPointer<QListWidget> caseListWidget;
  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;
  QPointer<QSpinBox> penWidthSpinBox;
  QPointer<QSpinBox> timeLineWidthSpinBox;
  QPointer<QSlider> zoomSlider;
  QPointer<QSlider> majorIntervalSlider;
  QPointer<QSlider> minorDivisionSlider;
  QPointer<QSlider> majorTickSizeSlider;
  QPointer<QSlider> minorTickSizeSlider;
  QPointer<QComboBox> penStyleComboBox;

  // Private variables
  int _distance;
  bool _labelsVisible;
  bool _shortLabels;
  bool _matched;
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
  
  // Pointer to other widget
  // Do not delete
  QPointer<EventGraphWidget> _eventGraphWidgetPtr;  
  
  // Private data vectors
  QVector<QString> _presentAttributes;
  QVector<QString> _presentRelationships;
  QVector<QString> _checkedCases;
  QVector<OccurrenceItem*> _attributeOccurrenceVector;
  QVector<OccurrenceItem*> _relationshipOccurrenceVector;
  QVector<Linkage*> _edgeVector;
  QVector<OccurrenceLabel*> _attributeLabelVector;
  QVector<OccurrenceLabel*> _relationshipLabelVector;
  QVector<LineObject*> _lineVector;
  QVector<TextObject*> _textVector;
  QVector<EllipseObject*> _ellipseVector;
  QVector<RectObject*> _rectVector;
  QVector<TimeLineObject*> _timeLineVector;

};

#endif
