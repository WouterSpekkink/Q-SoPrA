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

#ifndef EVENTGRAPHWIDGET_H
#define EVENTGRAPHWIDGET_H

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
#include <QGraphicsTextItem>
#include <QColorDialog>
#include <QListWidget>
#include <math.h>
#include <vector>
#include <fstream>
#include <QSvgRenderer>
#include "IncidentNodeLabel.h"
#include "GraphicsView.h"
#include "IncidentNode.h"
#include "Linkage.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "SavedPlotsDialog.h"
#include "AttributeColorDialog.h"
#include "AbstractNode.h"
#include "AbstractNodeLabel.h"
#include "LargeTextDialog.h"
#include "DeselectableTreeView.h"
#include "AttributeTreeFilter.h"
#include "AttributesWidget.h"
#include "AttributeDialog.h"
#include "DeselectableListWidget.h"
#include "LineageColorDialog.h"
#include "OccurrenceGraphWidget.h"
#include "HierarchyGraphWidget.h"
#include "SortFunctions.h"
#include "SupportingFunctions.h"
#include "ExportTransitionMatrixDialog.h"
#include "EventNodeSettingsDialog.h"
#include "SimpleAttributeSelectionDialog.h"
#include "LineObject.h"
#include "TextObject.h"
#include "EllipseObject.h"
#include "RectObject.h"
#include "AbstractionDialog.h"
#include "ModeColorDialog.h"
#include "EventTextDialog.h"

// Need a forward declaration here
class AttributesWidget;
class OccurrenceGraphWidget;

class EventGraphWidget : public QWidget
{
  Q_OBJECT
  // MainWindow needs access to this class
  friend class MainWindow;
  
public:
  // Constructor and deconstructor
  EventGraphWidget(QWidget *parent = 0);
  ~EventGraphWidget();

  // Setters
  void setAttributesWidget(AttributesWidget* attributesWidgetPtr);
  void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);
  void setOpenGL(bool state);
  void setAntialiasing(bool state);
  
  // Getters
  QVector<AbstractNode*> getAbstractNodes();
  QVector<IncidentNode*> getIncidentNodes();
  QVector<Linkage*> getEdgeVector();
  QVector<QString> getCheckedCases();

  // Functions that need to be exposed
  void resetTree();
  void checkCongruency();
  void updateCases();

private slots:
  // Private member functions
  void setCommentBool();
  void setComment();
  void toggleDetails();
  void toggleGraphicsControls();
  void toggleLegend();
  void rescale();
  void checkCases();
  void removeMode();
  void setModeButtons(QTableWidgetItem *item);
  void disableModeButtons();
  void setLinkageButtons(QTableWidgetItem *item);
  void disableLinkageButtons();
  void restoreModeColors();
  void moveModeUp();
  void moveModeDown();
  void retrieveData();
  void showAttributes();
  void showComments();
  void setValueButton();
  void setValue();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void buildEntities(QStandardItem *top, QString name);
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void boldSelected(QAbstractItemModel *model, QString name = "", int event = -1, 
		    QString type = "", QModelIndex parent = QModelIndex());
  void selectText();
  void sourceAttributeText(const QString &attribute, const int &incident);
  void removeText();
  void resetTexts();
  void setButtons();
  void fixTree();
  void changeFilter(const QString &text);
  void assignAttribute();
  void unassignAttribute();
  void highlightText();
  void newAttribute();
  void editAttribute();
  void updateEntityAfterEdit(const QString name, const QString description, const QString oldName);
  void removeUnusedAttributes();  
  void seeComponents();
  void previousDataItem();
  void nextDataItem();
  void getIncidents();
  void plotIncidents();
  void getEdges(QString coder, QString type, QColor color);
  void plotEdges(QString type);
  void layoutGraph();
  void getLabels();
  void addLabels();
  void cleanUp();	     
  void changePos(IncidentNode* item, qreal &dist);
  void changePos(AbstractNode* item, qreal &dist);
  void increaseDistance();
  void decreaseDistance();
  void expandGraph();
  void contractGraph();
  void contractCurrentGraph();
  void restorePositions();
  void processMoveItems(QGraphicsItem* item, QPointF pos);
  void setPlotButtons();
  void getLinkageDetails();
  void plotGraph();
  void addLinkageType();
  void removeLinkageType();
  void setCompareButton();
  void compare();
  void getCompareEdges(QString coder, QString type);
  void plotCompareEdges();
  void saveCurrentPlot();
  void seePlots();
  void setChangeLabel();
  void updateLinkages();
  void plotLabels();
  void processLowerRange(int value);
  void processUpperRange(int value);
  void setVisibility();
  void setHeights();
  void setRangeControls();
  void exportSvg();
  void exportTable();
  void exportNodes();
  void exportEdges();
  void colorByAttribute();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  void setEventColor();
  void setEdgeColor();
  void setLabelColor();
  void setBackgroundColor();
  void changeModeColor(QTableWidgetItem *item);
  void changeLinkageColor(QTableWidgetItem *item);
  void hideLinkageType();
  void showLinkageType();
  void processIncidentNodeContextMenu(const QString &action);
  void colligateEvents();
  void disaggregateEvent();
  void updateAbstractNodeIds(AbstractNode* abstractNode);
  void updateAbstractNodeOrder();
  void rewireLinkages(AbstractNode* abstractNode, QVector<IncidentNode*> incidents);
  void recolorEvents();
  void recolorLabels();
  void colorLineage();
  void exportTransitionMatrix();
  void findAncestors(QColor ancestorFill,
		     QColor ancestorText,
		     QGraphicsItem *origin,
		     QSet<QGraphicsItem*> *pFinished,
		     QString type);
  void findDescendants(QColor descendantFill,
		       QColor descendantText,
		       QGraphicsItem *origin,
		       QSet<QGraphicsItem*> *pFinished,
		       QString type);
  void setEventOriginalPosition();
  void settleEvent();
  void makeParallel();
  void normalizeDistance();
  void closeGap();
  void changeEventDescription();
  void addLinkage();
  void selectFollowers();
  void selectPredecessors();
  void selectAncestors(QGraphicsItem *origin, QSet<QGraphicsItem*> *pFinished, QString type);
  void selectDescendants(QGraphicsItem *origin, QSet<QGraphicsItem*> *pFinished, QString type);
  void setEventWidth();
  void processLinkageContextMenu(const QString &action);
  void removeLinkage();
  void keepLinkage();
  void acceptLinkage();
  void rejectLinkage();
  void ignoreLinkage();
  void removeNormalLinkage();
  void changeLinkageComment();
  void addLineObject(bool arrow1, bool arrow2, const QPointF &pos);
  void addTextObject(const QPointF &pos);
  void prepEllipseArea();
  void prepRectArea();
  void addEllipseObject(const QRectF &area);
  void addRectObject(const QRectF &area);
  void processLineContextMenu(const QString &action);
  void changeLineColor();
  void toggleArrow1();
  void toggleArrow2();
  void deleteLine();
  void processTextContextMenu(const QString &action);
  void changeText();
  void changeTextColor();
  void deleteText();
  void copyDescriptionToText();
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
  void processZoomSliderChange(int value);
  void resetZoomSlider();
  void setGraphControls(bool status);
  void findHeadsLowerBound(QSet<int> *mark, int currentIncident, int lowerLimit, QString type);
  void findHeadsUpperBound(QSet<int> *mark, int currentIncident, int upperLimit, QString type);
  void findTailsUpperBound(QSet<int> *mark, int currentIncident, int upperLimit, QString type);
  void findTailsLowerBound(QSet<int> *mark, int currentIncident, int lowerLimit, QString type);
  bool eventFilter(QObject *object, QEvent *event);
  void finalBusiness();
  
signals:
  void seeHierarchy(AbstractNode*);
  void changeEventWidth(QGraphicsItem*);
  
private:
  // Interface elements
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> attWidget;
  QPointer<QWidget> commentWidget;
  QPointer<QWidget> legendWidget;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<DeselectableListWidget> linkageListWidget;
  QPointer<QLabel> coderLabel;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> plotLabel;
  QPointer<QLabel> changeLabel;
  QPointer<QLabel> compareLabel;
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;
  QPointer<QLabel> indexLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QLabel> incongruencyLabel;
  QPointer<QLabel> eventLegendLabel;
  QPointer<QLabel> linkageLegendLabel;
  QPointer<QLabel> casesLabel;
  QPointer<QLabel> zoomLabel;
  QPointer<QPushButton> plotButton;
  QPointer<QPushButton> addLinkageTypeButton;
  QPointer<QPushButton> removeLinkageTypeButton;
  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> compareButton;
  QPointer<QPushButton> increaseDistanceButton;
  QPointer<QPushButton> decreaseDistanceButton;
  QPointer<QPushButton> expandButton;
  QPointer<QPushButton> contractButton;
  QPointer<QPushButton> toggleDetailsButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> previousEventButton;
  QPointer<QPushButton> nextEventButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> exportTableButton;
  QPointer<QPushButton> exportNodesButton;
  QPointer<QPushButton> exportEdgesButton;
  QPointer<QPushButton> plotLabelsButton;
  QPointer<QPushButton> colorByAttributeButton;
  QPointer<QPushButton> eventColorButton;
  QPointer<QPushButton> labelColorButton;
  QPointer<QPushButton> backgroundColorButton;
  QPointer<QPushButton> seeComponentsButton;
  QPointer<QPushButton> seeAttributesButton;
  QPointer<QPushButton> seeCommentsButton;
  QPointer<QPushButton> valueButton; 
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;  
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> resetTextsButton;
  QPointer<QPushButton> removeModeButton;
  QPointer<QPushButton> restoreModeColorsButton;
  QPointer<QPushButton> exportTransitionMatrixButton;
  QPointer<QPushButton> moveModeUpButton;
  QPointer<QPushButton> moveModeDownButton;
  QPointer<QPushButton> hideLinkageTypeButton;
  QPointer<QPushButton> showLinkageTypeButton;
  QPointer<QPushButton> contractCurrentGraphButton;
  QPointer<QPushButton> addLineButton;
  QPointer<QPushButton> addSingleArrowButton;
  QPointer<QPushButton> addDoubleArrowButton;
  QPointer<QPushButton> addEllipseButton;
  QPointer<QPushButton> addRectangleButton;
  QPointer<QPushButton> addTextButton;
  QPointer<DeselectableListWidget> eventListWidget;
  QPointer<QListWidget> caseListWidget;
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QLineEdit> valueField;
  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;
  QPointer<QComboBox> coderComboBox;
  QPointer<QComboBox> typeComboBox;
  QPointer<QComboBox> compareComboBox;
  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;
  QPointer<QSlider> zoomSlider;

  // Private variables
  QString _selectedCoder;
  QString _selectedCompare;
  AbstractNode* _selectedAbstractNode;
  qreal _distance;
  int _vectorPos;
  int _selectedIncident;
  bool _labelsVisible;
  bool _commentBool;
  bool _contracted;

  // Private data vectors
  QVector<IncidentNode*> _incidentNodeVector;
  QVector<AbstractNode*> _abstractNodeVector;
  QVector<QGraphicsItem*> _currentData;
  QVector<Linkage*> _edgeVector;
  QVector<Linkage*> _compareVector;
  QVector<IncidentNodeLabel*> _incidentNodeLabelVector;
  QVector<AbstractNodeLabel*> _abstractNodeLabelVector;
  QVector<LineObject*> _lineVector;
  QVector<TextObject*> _textVector;
  QVector<EllipseObject*> _ellipseVector;
  QVector<RectObject*> _rectVector;
  QVector<QString> _presentTypes;
  QVector<QString> _checkedCases;

  // Private map
  QMap<QGraphicsItem*, QPointF> _contractedMap;
  
  // Pointers to other widgets
  // Do not delete
  QPointer<AttributesWidget> _attributesWidgetPtr;
  QPointer<RelationshipsWidget> _relationshipsWidgetPtr;
    
};

#endif
