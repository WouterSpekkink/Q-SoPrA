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

class EventGraphWidget;

class OccurrenceGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  OccurrenceGraphWidget(QWidget *parent = 0);
  ~OccurrenceGraphWidget() {};

  void setEventGraph(EventGraphWidget *egw);			     
  void checkCongruency();
  bool attributesPresent();
			
private slots:
  void toggleLegend();
  void toggleGraphicsControls();
  
  void addAttribute();
  void findChildren(QString father, QVector<QString> *children, bool entity);
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
  void updateArrows();
  
  void processLowerRange(int value);
  void processUpperRange(int value);
  void setVisibility();
  void setRangeControls();
  void exportSvg();
  void exportMatrix();
  void setChangeLabel();
  
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
  void deleteEllipse();
  void processRectContextMenu(const QString &action);
  void changeRectColor();
  void deleteRect();

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
  QVector<LineObject*> lineVector;
  QVector<TextObject*> textVector;
  QVector<EllipseObject*> ellipseVector;
  QVector<RectObject*> rectVector;
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
  QPointer<QPushButton> exportMatrixButton;
  
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
