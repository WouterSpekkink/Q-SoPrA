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

class EventGraphWidget;

class OccurrenceGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  OccurrenceGraphWidget(QWidget *parent = 0);
  ~OccurrenceGraphWidget() {};

  void setEventGraph(EventGraphWidget *egw);			     
  void checkCongruency();
			
private slots:
  void toggleLegend();
  void toggleGraphicsControls();
  
  void addAttribute();
  void findChildren(QString father, QVector<QString> *children);
  void setModeButton(QTableWidgetItem *item);
  void disableModeButton();
  void removeMode();
  void changeModeColor(QTableWidgetItem *item);
  
  void wireLinkages();
  void groupOccurrences();
  void reset();
  void getEvents();
  void restore();
  void plotLabels();
  void setBackgroundColor();
  void increaseDistance();
  void decreaseDistance();

  void processLowerRange(int value);
  void processUpperRange(int value);
  void setVisibility();
  void setRangeControls();
  void exportSvg();
  void setChangeLabel();
  
  void processMoveItems(QGraphicsItem *item, QPointF pos);
  void processMoveLine(QGraphicsItem *item, QPointF pos);
  
  void saveCurrentPlot();
  void seePlots();
  
  void cleanUp();
  void finalBusiness();
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> legendWidget;
  QVector<OccurrenceItem*> occurrenceVector;
  QVector<Arrow*> edgeVector;
  QVector<OccurrenceLabel*> labelVector;
  EventGraphWidget *eventGraph;

  QPointer<QLabel> legendLabel;
  QPointer<QLabel> plotLabel;
  QPointer<QLabel> changeLabel;
  QPointer<QLabel> incongruencyLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;

  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> getEventsButton;
  QPointer<QPushButton> restoreButton;
  QPointer<QPushButton> plotLabelsButton;
  QPointer<QPushButton> backgroundColorButton;
  QPointer<QPushButton> increaseDistanceButton;
  QPointer<QPushButton> decreaseDistanceButton;
  QPointer<QPushButton> removeModeButton;
  QPointer<QPushButton> exportSvgButton;
  
  QPointer<DeselectableListWidget> attributeListWidget;

  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;

  QVector<QString> presentAttributes;
  
  int distance;
  bool labelsVisible;
};

#endif
