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


#ifndef SYSTEMGRAPHWIDGET_H
#define SYSTEMGRAPHWIDGET_H
#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSlider>
#include <QVector>
#include <QPointer>
#include <QtSql>
#include <QMessageBox>
#include <QSvgGenerator>
#include <QFileDialog>
#include <math.h>
#include <QGraphicsTextItem>
#include <QColorDialog>
#include <QHeaderView>
#include <QStandardItemModel>
#include <fstream>
#include "DeselectableTreeViewEntities.h"
#include "IncidentNodeLabel.h"
#include "GraphicsView.h"
#include "NetworkNode.h"
#include "DirectedEdge.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "NetworkNodeLabel.h"
#include "AttributeColorDialog.h"
#include "DeselectableListWidget.h"
#include "SortFunctions.h"
#include "SupportingFunctions.h"
#include "LargeTextDialog.h"
#include "LineObject.h"
#include "EllipseObject.h"
#include "RectObject.h"
#include "TextObject.h"
#include "ModeColorDialog.h"
#include "GuideLine.h"

class SystemGraphWidget : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;

  public:
    // Constructor and destructor
    SystemGraphWidget(QWidget *parent = 0);
    ~SystemGraphWidget();

    // Setters
    void setSystem(QMap<QVector<QString>, int> system);
    void setEvents(QMap<QVector<QString>, QColor> eventsMap);
    void setEdges(QMap<QVector<QString>, QColor> edgesMap);
    void setOpenGL(bool state);
    void setAntialiasing(bool state);
    void cleanUp();

  private slots:
    // Private member functions
    void toggleLegend();
    void toggleDetails();
    void toggleGraphicsControls();
    void processZoomSliderChange(int value);
    void resetZoomSlider();
    void setGraphControls(bool state);
    void hideMode();
    void showMode();
    void hideType();
    void showType();
    void setVisibility();
    void processHeights();
    void setBackgroundColor();
    void changeEdgeColor(QTableWidgetItem *item);
    void changeModeColor(QTableWidgetItem *item);
    void toggleLabels();
    void increaseLabelSize();
    void decreaseLabelSize();
    void setWeightControls();
    void exportSvg();
    void exportNodes();
    void exportEdges();
    void updateEdges();
    void makeLayout();
    void springLayout();
    void circularLayout();
    void expandLayout();
    void contractLayout();
    void processMoveItems(QGraphicsItem *item, QPointF pos);
    void addLineObject(const QPointF &start, const QPointF &end);
    void addSingleArrowObject(const QPointF &start, const QPointF &end);
    void addDoubleArrowObject(const QPointF &start, const QPointF &end);
    void addEllipseObject(const QRectF &area);
    void addRectObject(const QRectF &area);
    void addTextObject(const QRectF &area, const qreal &size);
    void hideAnnotations();
    void addHorizontalGuideLine(const QPointF &pos);
    void addVerticalGuideLine(const QPointF &pos);
    void toggleSnapGuides();
    void setPenStyle();
    void setPenWidth();
    void setLineColor();
    void setFillColor();
    void setFillOpacity(int value);
    void processShapeSelection();
    void processLineContextMenu(const QString &action);
    void toggleArrow1();
    void toggleArrow2();
    void deleteLine();
    void processTextContextMenu(const QString &action);
    void changeText();
    void deleteText();
    void processEllipseContextMenu(const QString &action);
    void deleteEllipse();
    void processRectContextMenu(const QString &action);
    void deleteRect();
    void duplicateLine();
    void duplicateText();
    void duplicateEllipse();
    void duplicateRect();
    void processGuideLineContextMenu(const QString &action);
    void deleteGuideLine();
    void objectOneForward();
    void objectOneBackward();
    void objectToFront();
    void objectToBack();
    void fixZValues();
    void rescale();
    void retrieveData();
    void previousDataItem();
    void nextDataItem();
    void updateWeightControls();
    void switchBack();
    void setModeButtons(QTableWidgetItem *item);
    void setTypeButtons(QTableWidgetItem *item);
    void disableModeButtons();
    void disableTypeButtons();
    bool eventFilter(QObject *object, QEvent *event);
    void finalBusiness();

  signals:
    void goToEventGraph();
    void sendLineColor(QColor&);
    void sendFillColor(QColor&);

  private:
    // Interface elements
    QPointer<Scene> scene;
    QPointer<GraphicsView> view;
    QPointer<QWidget> infoWidget;
    QPointer<QWidget> graphicsWidget;
    QPointer<QWidget> legendWidget;
    QPointer<QLabel> weightLabel;
    QPointer<QLabel> nodeLegendLabel;
    QPointer<QLabel> edgeLegendLabel;
    QPointer<QLabel> nameLabel;
    QPointer<QLabel> descriptionLabel;
    QPointer<QLabel> zoomLabel;
    QPointer<QLabel> shapesLabel;
    QPointer<QLabel> penStyleLabel;
    QPointer<QLabel> penWidthLabel;
    QPointer<QLabel> lineColorLabel;
    QPointer<QLabel> fillColorLabel;
    QPointer<QLabel> fillOpacityLabel;
    QPointer<QLabel> guideLinesLabel;
    QPointer<QLabel> labelSizeLabel;
    QPointer<QLabel> layoutLabel;
    QPointer<QComboBox> penStyleComboBox;
    QPointer<QComboBox> layoutComboBox;
    QPointer<DeselectableListWidget> nodeListWidget;
    QPointer<DeselectableListWidget> edgeListWidget;
    QPointer<QLineEdit> nameField;
    QPointer<QTextEdit> descriptionField;
    QPointer<QPushButton> exitButton;
    QPointer<QPushButton> toggleLabelsButton;
    QPointer<QPushButton> toggleGraphicsControlsButton;
    QPointer<QPushButton> toggleLegendButton;
    QPointer<QPushButton> toggleDetailsButton;
    QPointer<QPushButton> previousNodeButton;
    QPointer<QPushButton> nextNodeButton;
    QPointer<QPushButton> backgroundColorButton;
    QPointer<QPushButton> increaseFontSizeButton;
    QPointer<QPushButton> decreaseFontSizeButton;
    QPointer<QPushButton> exportSvgButton;
    QPointer<QPushButton> exportNodesButton;
    QPointer<QPushButton> exportEdgesButton;
    QPointer<QPushButton> hideModeButton;
    QPointer<QPushButton> showModeButton;
    QPointer<QPushButton> hideTypeButton;
    QPointer<QPushButton> showTypeButton;
    QPointer<QPushButton> layoutButton;
    QPointer<QPushButton> expandLayoutButton;
    QPointer<QPushButton> contractLayoutButton;
    QPointer<QPushButton> savePlotButton;
    QPointer<QPushButton> seePlotsButton;
    QPointer<QPushButton> addLineButton;
    QPointer<QPushButton> addSingleArrowButton;
    QPointer<QPushButton> addDoubleArrowButton;
    QPointer<QPushButton> addEllipseButton;
    QPointer<QPushButton> addRectangleButton;
    QPointer<QPushButton> addTextButton;
    QPointer<QPushButton> hideAnnotationsButton;
    QPointer<QPushButton> changeLineColorButton;
    QPointer<QPushButton> changeFillColorButton;
    QPointer<QPushButton> addHorizontalGuideLineButton;
    QPointer<QPushButton> addVerticalGuideLineButton;
    QPointer<QPushButton> snapGuidesButton;
    QPointer<QSpinBox> weightSpinBox;
    QPointer<QSpinBox> penWidthSpinBox;
    QPointer<QSlider> zoomSlider;
    QPointer<QSlider> fillOpacitySlider;
    QPointer<QCheckBox> weightCheckBox;

    // Private variables
    QString _selectedType;
    QString _selectedEntityName;
    int _maxWeight;
    int _vectorPos;
    bool _labelsShown;
    bool _massMove;
    int _currentPenStyle;
    int _currentPenWidth;
    QColor _currentLineColor;
    QColor _currentFillColor;

    // Private data vectors
    QVector<QString> _edgeTypes;
    QVector<NetworkNode*> _nodeVector;
    QVector<NetworkNode*> _currentData;
    QVector<DirectedEdge*> _edgeVector;
    QMap<DirectedEdge*, int> _edgeWeights;
    QVector<NetworkNodeLabel*> _labelVector;
    QVector<LineObject*> _lineVector;
    QVector<TextObject*> _textVector;
    QVector<EllipseObject*> _ellipseVector;
    QVector<RectObject*> _rectVector;
    QVector<GuideLine*> _guidesVector;
};

#endif
