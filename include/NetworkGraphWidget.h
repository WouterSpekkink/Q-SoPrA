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

#ifndef NETWORKGRAPHWIDGET
#define NETWORKGRAPHWIDGET

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
#include "UndirectedEdge.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "SavedPlotsDialog.h"
#include "NetworkNodeLabel.h"
#include "AttributeColorDialog.h"
#include "MultimodeDialog.h"
#include "DeselectableListWidget.h"
#include "AttributeTreeFilter.h"
#include "AttributeDialog.h"
#include "MergeRelationshipsDialog.h"
#include "SortFunctions.h"
#include "SupportingFunctions.h"
#include "NodeSettingsDialog.h"
#include "LargeTextDialog.h"
#include "LineObject.h"
#include "EllipseObject.h"
#include "RectObject.h"
#include "TextObject.h"
#include "ModeColorDialog.h"

class NetworkGraphWidget : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  NetworkGraphWidget(QWidget *parent = 0);
  ~NetworkGraphWidget();

  // Setters
  void setOpenGL(bool state);
  void setAntialiasing(bool state);
  
  // Getters
  void getTypes();
  
  // Some functions that need to be exposed
  void resetTree();
  bool typesPresent();
  void updateCases();

private slots:
  // Private member functions
  void checkCongruency();
  void toggleDetails();
  void toggleGraphicsControls();
  void toggleLegend();
  void rescale();
  void checkCases();
  void processZoomSliderChange(int value);
  void resetZoomSlider();
  void setGraphControls(bool state);
  void retrieveData();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void boldSelected(QAbstractItemModel *model, QString name = "",
		    QString entity = "", QModelIndex parent = QModelIndex());
  void fixTree(QString entity);
  void setFilter(const QString &text);
  void previousDataItem();
  void nextDataItem();
  void setValueButton();
  void setValue();
  void assignAttribute();
  void unassignAttribute();
  void addAttribute();
  void editAttribute();
  void removeUnusedAttributes();
  void multimodeTransformation();
  void removeMode();
  void setModeButtons(QTableWidgetItem *item);
  void disableModeButtons();
  void restoreModeColors();
  void hideMode();
  void showMode();
  void moveModeUp();
  void moveModeDown();
  void mergeRelationships();
  void setFilterButtons(QTableWidgetItem *item);
  void disableFilterButtons();
  void activateFilter();
  void deactivateFilter();
  void hideType();
  void showType();
  void setVisibility();
  void processHeights();
  void getEntities();
  void plotEntities();
  void getDirectedEdges();
  void plotDirectedEdges(QString type, QColor color);
  void getUndirectedEdges();
  void plotUndirectedEdges(QString type, QColor color);
  void colorByAttribute();
  void setNodeColor();
  void setLabelColor();
  void setBackgroundColor();
  void changeEdgeColor(QTableWidgetItem *item);
  void changeModeColor(QTableWidgetItem *item);
  void toggleLabels();
  void increaseLabelSize();
  void decreaseLabelSize();
  void findChildren(QString father, QVector<QString> *children);
  void processLowerRange(int value);
  void processUpperRange(int value);
  void setRangeControls();
  void setWeightControls();
  void updateRangeControls();
  void updateWeightControls();
  void exportSvg();
  void exportNodes();
  void exportEdges();
  void saveCurrentPlot();
  void seePlots();
  void setChangeLabel();
  void updateEdges();
  void springLayout();
  void circularLayout();
  void expandLayout();
  void contractLayout();
  void processMoveItems(QGraphicsItem *item, QPointF pos);
  void processNetworkNodeContextMenu(const QString &action);
  void setNodePersistence(bool state);
  void recolorNodes();
  void recolorLabels();
  void addLineObject(const QPointF &start, const QPointF &end);
  void addSingleArrowObject(const QPointF &start, const QPointF &end);
  void addDoubleArrowObject(const QPointF &start, const QPointF &end);
  void addEllipseObject(const QRectF &area);
  void addRectObject(const QRectF &area);
  void addTextObject(const QRectF &area, const qreal &size);
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
  void duplicateLine();
  void duplicateText();
  void duplicateEllipse();
  void duplicateRect();
  void objectOneForward();
  void objectOneBackward();
  void objectToFront();
  void objectToBack();
  void fixZValues();
  void setPlotButton();
  void plotNewGraph();
  void addRelationshipType();
  void removeRelationshipType();
  void cleanUp();
  void setButtons();
  bool eventFilter(QObject *object, QEvent *event);
  void finalBusiness();
  
private:
  // Interface elements
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> legendWidget;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeViewEntities> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;
  QPointer<QLabel> weightLabel;
  QPointer<QLabel> nodeLegendLabel;
  QPointer<QLabel> edgeLegendLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QLabel> plotLabel;
  QPointer<QLabel> changeLabel;
  QPointer<QLabel> incongruencyLabel;
  QPointer<QLabel> casesLabel;
  QPointer<QLabel> zoomLabel;
  QPointer<QComboBox> typeComboBox;
  QPointer<DeselectableListWidget> nodeListWidget;
  QPointer<DeselectableListWidget> edgeListWidget;
  QPointer<QListWidget> caseListWidget;
  QPointer<QLineEdit> nameField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QLineEdit> valueField;
  QPointer<QTextEdit> descriptionField;
  QPointer<QPushButton> plotButton;
  QPointer<QPushButton> addButton;
  QPointer<QPushButton> toggleLabelsButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> toggleDetailsButton;
  QPointer<QPushButton> previousNodeButton;
  QPointer<QPushButton> nextNodeButton;
  QPointer<QPushButton> valueButton; 
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;  
  QPointer<QPushButton> colorByAttributeButton;
  QPointer<QPushButton> nodeColorButton;
  QPointer<QPushButton> labelColorButton;
  QPointer<QPushButton> backgroundColorButton;
  QPointer<QPushButton> increaseFontSizeButton;
  QPointer<QPushButton> decreaseFontSizeButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> exportNodesButton;
  QPointer<QPushButton> exportEdgesButton;
  QPointer<QPushButton> setFilteredButton;
  QPointer<QPushButton> unsetFilteredButton;
  QPointer<QPushButton> hideTypeButton;
  QPointer<QPushButton> showTypeButton;
  QPointer<QPushButton> hideModeButton;
  QPointer<QPushButton> showModeButton;
  QPointer<QPushButton> multimodeButton;
  QPointer<QPushButton> removeModeButton;
  QPointer<QPushButton> mergeButton;
  QPointer<QPushButton> removeTypeButton;
  QPointer<QPushButton> springLayoutButton;
  QPointer<QPushButton> circularLayoutButton;
  QPointer<QPushButton> expandLayoutButton;
  QPointer<QPushButton> contractLayoutButton;
  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> restoreModeColorsButton;
  QPointer<QPushButton> moveModeUpButton;
  QPointer<QPushButton> moveModeDownButton;
  QPointer<QPushButton> addLineButton;
  QPointer<QPushButton> addSingleArrowButton;
  QPointer<QPushButton> addDoubleArrowButton;
  QPointer<QPushButton> addEllipseButton;
  QPointer<QPushButton> addRectangleButton;
  QPointer<QPushButton> addTextButton;
  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;
  QPointer<QSpinBox> weightSpinBox;
  QPointer<QSlider> zoomSlider;
  QPointer<QCheckBox> weightCheckBox;
  
  // Private variables    
  QString _selectedType;
  QString _selectedEntityName;
  int _minOrder;
  int _maxOrder;
  int _maxWeight;
  int _vectorPos;
  bool _labelsShown;
  bool _massMove;

  // Private data vectors
  QVector<QString> _presentTypes; 
  QVector<QString> _checkedCases;
  QVector<NetworkNode*> _networkNodeVector;
  QVector<NetworkNode*> _currentData;
  QVector<DirectedEdge*> _directedVector;
  QVector<UndirectedEdge*> _undirectedVector;
  QVector<NetworkNodeLabel*> _networkNodeLabelVector;
  QVector<LineObject*> _lineVector;
  QVector<TextObject*> _textVector;
  QVector<EllipseObject*> _ellipseVector;
  QVector<RectObject*> _rectVector;
};


#endif
