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

#ifndef HIERARCHYGRAPHWIDGET_H
#define HIERARCHYGRAPHWIDGET_H

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
#include "EventGraphWidget.h"
#include "IncidentNodeLabel.h"
#include "BandlessGraphicsView.h"
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
#include "SortFunctions.h"
#include "SupportingFunctions.h"
#include "LargeTextDialog.h"
#include "LineObject.h"
#include "EllipseObject.h"
#include "RectObject.h"
#include "TextObject.h"
#include "RelationshipsWidget.h"
#include "ModeColorDialog.h"

class AttributesWidget;
class RelationshipsWidget;

class HierarchyGraphWidget : public QDialog
{
  Q_OBJECT
  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  HierarchyGraphWidget(QWidget *parent = 0);
  ~HierarchyGraphWidget();

  // Setters
  void setOrigin(AbstractNode *origin);
  void setEvents(QVector<IncidentNode*> eventVector);
  void setAbstractNodes(QVector<AbstractNode*> abstractNodeVector);
  void setEdges(QVector<Linkage*> edgeVector);
  void setAttributesWidget(AttributesWidget *attributesWidgetPtr);
  void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);
  void setOpenGL(bool state);
  void setAntialiasing(bool state);

  // Getters
  void getEdges();
  
  // Some functions that need to be exposed
  void cleanUp();
  void resetTree();
					   
private slots:
  // Private member functions
  void setCommentBool();
  void setComment();
  void toggleDetails();
  void toggleLegend();
  void rescale();
  void changeModeColor(QTableWidgetItem *item);
  void colorByAttribute();
  void removeMode();
  void restoreModeColors();
  void moveModeUp();
  void moveModeDown();
  void setModeButtons(QTableWidgetItem *item);
  void disableModeButtons();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  void retrieveData();
  void showAttributes();
  void showComments();
  void setValueButton();
  void setValue();
  void buildComponents(AbstractNode *origin, int layer);
  void addLayer(QVector<AbstractNode*> presentLayer, QVector<AbstractNode*> partLayer, int layer);
  void processMoveItems(QGraphicsItem *item, QPointF pos);
  void exportSvg();
  void assignAttribute();
  void unassignAttribute();
  void newAttribute();
  void editAttribute();
  void updateEntityAfterEdit(const QString name, const QString description, const QString oldName);
  void removeUnusedAttributes();
  void changeFilter(const QString &text);
  void selectText();
  void sourceAttributeText(const QString &attribute, const int &incident);
  void highlightText();
  void removeText();
  void resetTexts();
  void processZoomSliderChange(int value);
  void resetZoomSlider();
  void addLineObject(const QPointF &start, const QPointF &end);
  void addSingleArrowObject(const QPointF &start, const QPointF &end);
  void addDoubleArrowObject(const QPointF &start, const QPointF &end);
  void addEllipseObject(const QRectF &area);
  void addRectObject(const QRectF &area);
  void addTextObject(const QRectF &area, const qreal &size);
  void setPenStyle();
  void setPenWidth();
  void setLineColor();
  void setFillColor();
  void setFillOpacity(int value);
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
  void duplicateLine();
  void duplicateText();
  void duplicateEllipse();
  void duplicateRect();
  void objectOneForward();
  void objectOneBackward();
  void objectToFront();
  void objectToBack();
  void fixZValues();
  void setLinkageButtons(QTableWidgetItem *item);
  void changeLinkageColor(QTableWidgetItem *item);
  void hideLinkage();
  void showLinkage();
  void disableLinkageButtons();
  void updateLinkages();
  void setHeights();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void buildEntities(QStandardItem *top, QString name);
  void fixTree();
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void boldSelected(QAbstractItemModel *model, QString name = "", int event = -1, 
		    QString type = "", QModelIndex parent = QModelIndex());
  void setButtons();
  bool eventFilter(QObject *object, QEvent *event);
  void switchBack();
  void finalBusiness();
  
signals:
  void goToEventGraph();
  void sendLineColor(QColor&);
  void sendFillColor(QColor&);
  
private:
  // Interface elements
  QPointer<Scene> scene;
  QPointer<BandlessGraphicsView> view;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> attWidget;
  QPointer<QWidget> commentWidget;
  QPointer<QWidget> legendWidget;
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QLabel> legendLabel;
  QPointer<QLabel> zoomLabel;
  QPointer<QLabel> shapesLabel;
  QPointer<QLabel> penStyleLabel;
  QPointer<QLabel> penWidthLabel;
  QPointer<QLabel> lineColorLabel;
  QPointer<QLabel> fillColorLabel;
  QPointer<QLabel> fillOpacityLabel;
  QPointer<QComboBox> penStyleComboBox;
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QLineEdit> valueField;
  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;
  QPointer<QPushButton> toggleDetailsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> seeAttributesButton;
  QPointer<QPushButton> seeCommentsButton;
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> valueButton; 
  QPointer<QPushButton> removeUnusedAttributesButton;  
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> resetTextsButton;
  QPointer<QPushButton> exitButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> colorByAttributeButton;
  QPointer<QPushButton> removeModeButton;
  QPointer<QPushButton> restoreModeColorsButton;
  QPointer<QPushButton> moveModeUpButton;
  QPointer<QPushButton> moveModeDownButton;
  QPointer<QPushButton> hideLinkageTypeButton;
  QPointer<QPushButton> showLinkageTypeButton;
  QPointer<QPushButton> addLineButton;
  QPointer<QPushButton> addSingleArrowButton;
  QPointer<QPushButton> addDoubleArrowButton;
  QPointer<QPushButton> addEllipseButton;
  QPointer<QPushButton> addRectangleButton;
  QPointer<QPushButton> addTextButton;
  QPointer<QPushButton> changeLineColorButton;
  QPointer<QPushButton> changeFillColorButton;
  QPointer<DeselectableListWidget> eventListWidget;
  QPointer<DeselectableListWidget> linkageListWidget;
  QPointer<QSlider> zoomSlider;
  QPointer<QSlider> fillOpacitySlider;
  QPointer<QSpinBox> penWidthSpinBox;

  // Private variables
  AbstractNode *_origin;
  AbstractNode *_selectedAbstractNode;
  int _selectedIncident;
  bool _commentBool;
  int _currentPenStyle;
  int _currentPenWidth;
  QColor _currentLineColor;
  QColor _currentFillColor;
  
  // Private data vectors
  QVector<IncidentNode*> _eventVector;
  QVector<AbstractNode*> _abstractNodeVector;
  QVector<Linkage*> _edgeVector;
  QVector<QString> _presentTypes;
  QVector<LineObject*> _lineVector;
  QVector<TextObject*> _textVector;
  QVector<EllipseObject*> _ellipseVector;
  QVector<RectObject*> _rectVector;
  QVector<QGraphicsItem*> _currentData;

  // Pointers to other widgets
  // Do not delete
  AttributesWidget *_attributesWidgetPtr;
  RelationshipsWidget *_relationshipsWidgetPtr;

};

#endif
