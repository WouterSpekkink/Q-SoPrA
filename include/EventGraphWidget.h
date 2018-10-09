#ifndef EVENTGRAPHWIDGET
#define EVENTGRAPHWIDGET

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
#include "NodeLabel.h"
#include "GraphicsView.h"
#include "EventItem.h"
#include "Arrow.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "SavedPlotsDialog.h"
#include "AttributeColorDialog.h"
#include "MacroEvent.h"
#include "MacroLabel.h"
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

class AttributesWidget;

class EventGraphWidget : public QWidget
{
  Q_OBJECT
  friend class MainWindow;
  
public:
  EventGraphWidget(QWidget *parent = 0);
  ~EventGraphWidget() {};

  QVector<MacroEvent*> getMacros();
  QVector<EventItem*> getEventItems();
  QVector<Arrow*> getEdgeVector();
  void resetTree();
  void checkCongruency();
  void setAttributesWidget(AttributesWidget* aw);
  void setOccurrenceGraph(OccurrenceGraphWidget* ogw);
  void setRelationshipsWidget(RelationshipsWidget *rw);
  void updateCases();

private slots:
  void setCommentBool();
  void setComment();
  void toggleDetails();
  void toggleGraphicsControls();
  void toggleLegend();
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
  void getEvents();
  void plotEvents();
  void getEdges(QString coder, QString type, QColor color);
  void plotEdges(QString type);
  void layoutGraph();
  void getLabels();
  void addLabels();
  void cleanUp();	     
  void changePos(EventItem *item, qreal &dist);
  void changePos(MacroEvent *item, qreal &dist);
  void increaseDistance();
  void decreaseDistance();
  void expandGraph();
  void contractGraph();
  void processMoveItems(QGraphicsItem *item, QPointF pos);
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
  void processEventItemContextMenu(const QString &action);
  void colligateEvents(bool single);
  void disaggregateEvent();
  void updateMacroIds(MacroEvent *macro);
  void updateMacroOrder();
  void rewireLinkages(MacroEvent *macro, QVector<EventItem*> incidents);
  void recolorEvents();
  void recolorLabels();
  void colorLineage();
  void exportTransitionMatrix();
  void findAncestors(QColor ancestor,
		     QGraphicsItem *origin,
		     QSet<QGraphicsItem*> *pFinished,
		     QString type);
  void findDescendants(QColor descendant,
		       QGraphicsItem *origin,
		       QSet<QGraphicsItem*> *pFinished,
		       QString type);
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
  void processArrowContextMenu(const QString &action);
  void removeLinkage();
  void keepLinkage();
  void acceptLinkage();
  void rejectLinkage();
  void ignoreLinkage();
  void removeNormalLinkage();
  void changeLinkageComment();
  void processEventGraphContextMenu(const QString &action, const QPoint &pos);
  void addLineObject(bool arrow1, bool arrow2, const QPointF &pos);
  void addTextObject(const QPointF &pos);
  void addEllipseObject(const QPointF &pos);
  void addRectObject(const QPointF &pos);
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
  void seeHierarchy(MacroEvent *);
  void changeEventWidth(QGraphicsItem *);
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> attWidget;
  QPointer<QWidget> commentWidget;
  QPointer<QWidget> legendWidget;
  QVector<EventItem*> eventVector;
  QVector<MacroEvent*> macroVector;
  QVector<QGraphicsItem*> currentData;
  QVector<Arrow*> edgeVector;
  QVector<Arrow*> compareVector;
  QVector<NodeLabel*> nodeLabelVector;
  QVector<MacroLabel*> macroLabelVector;
  QVector<LineObject*> lineVector;
  QVector<TextObject*> textVector;
  QVector<EllipseObject*> ellipseVector;
  QVector<RectObject*> rectVector;
  AttributesWidget *attributesWidget;
  OccurrenceGraphWidget *occurrenceGraph;
  RelationshipsWidget *relationshipsWidget;
  
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
  
  QVector<QString> presentTypes;
  QVector<QString> checkedCases;
  
  QString selectedCoder;
  QString selectedCompare;
  MacroEvent *selectedMacro;
  
  qreal distance;
  int vectorPos;
  int selectedIncident;

  bool labelsVisible;
  bool commentBool;
};

#endif
