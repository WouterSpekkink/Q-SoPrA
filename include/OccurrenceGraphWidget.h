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
#include "Arrow.h"
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
#include "EventItem.h"
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
  friend class MainWindow;

public:
  OccurrenceGraphWidget(QWidget *parent = 0);
  ~OccurrenceGraphWidget() {};

  void setEventGraph(EventGraphWidget *egw);			     
  void checkCongruency();
  bool attributesPresent();
  bool relationshipsPresent();
  void updateCases();
  bool eventFilter(QObject *object, QEvent *event);
		    
private slots:
  void toggleLegend();
  void toggleGraphicsControls();
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
  void getEvents();
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
  
  void processOccurrenceGraphContextMenu(const QString &action, const QPoint &pos);
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
  
  void saveCurrentPlot();
  void seePlots();
  
  void cleanUp();
  void finalBusiness();
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> legendWidget;
  QVector<OccurrenceItem*> attributeOccurrenceVector;
  QVector<OccurrenceItem*> relationshipOccurrenceVector;
  QVector<Arrow*> edgeVector;
  QVector<OccurrenceLabel*> attributeLabelVector;
  QVector<OccurrenceLabel*> relationshipLabelVector;
  QVector<LineObject*> lineVector;
  QVector<TextObject*> textVector;
  QVector<EllipseObject*> ellipseVector;
  QVector<RectObject*> rectVector;
  EventGraphWidget *eventGraph;

  QPointer<QLabel> attributeLegendLabel;
  QPointer<QLabel> relationshipLegendLabel;
  QPointer<QLabel> plotLabel;
  QPointer<QLabel> changeLabel;
  QPointer<QLabel> incongruencyLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;
  QPointer<QLabel> casesLabel;
  QPointer<QLabel> zoomLabel;
  
  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> addRelationshipButton;
  QPointer<QPushButton> getEventsButton;
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
  
  QPointer<DeselectableListWidget> attributeListWidget;
  QPointer<DeselectableListWidget> relationshipListWidget;

  QPointer<QListWidget> caseListWidget;
  
  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;

  QPointer<QSlider> zoomSlider;
  
  QVector<QString> presentAttributes;
  QVector<QString> presentRelationships;
  QVector<QString> checkedCases;
  
  int distance;
  bool labelsVisible;
  bool matched;
};

#endif
