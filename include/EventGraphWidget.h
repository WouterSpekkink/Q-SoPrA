#ifndef EVENTGRAPHWIDGET
#define EVENTGRAPHWIDGET

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

class EventGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;
  
public:
  EventGraphWidget(QWidget *parent = 0);
  ~EventGraphWidget() {};

private slots:
  void toggleDetails();
  void toggleGraphicsControls();
  void retrieveData();
  void previousDataItem();
  void nextDataItem();
  void getEvents();
  void plotEvents();
  void getEdges(QString coder, QString type);
  void plotEdges();
  void getLabels();
  void addLabels();
  void cleanUp();	     
  void increaseWidth(EventItem *item);
  void decreaseWidth(EventItem *item);
  void increasePos(EventItem *item);
  void decreasePos(EventItem *item);
  void increaseDistance();
  void decreaseDistance();
  void setPlotButton();
  void getLinkageDetails();
  void plotGraph();
  void setCompareButton();
  void compare();
  void getCompareEdges(QString coder, QString type);
  void plotCompareEdges();
  void saveCurrentPlot();
  void seePlots();
  void setChangeLabel();
  void plotLabels();
  void processLowerRange(int value);
  void processUpperRange(int value);
  void setRangeControls();
  void exportSvg();
  void colorByAttribute();
  void findChildren(QString father, QVector<QString> *children);
  void setEventColor();
  void setLabelColor();
  void setBackgroundColor();
  void processEventItemContextMenu(const QString &action);
  void colligateEvents();
  void recolorEvents();
  void recolorLabels();
  void processArrowContextMenu(const QString &action);
  void removeLinkage();
  void keepLinkage();
  void acceptLinkage();
  void rejectLinkage();
  void ignoreLinkage();
  void removeNormalLinkage();
  void findPastPaths(QVector<int> *mark, int currentIncident);
  void findFuturePaths(QVector<int> *mark, int currentIncident);
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> graphicsWidget;
  QVector<EventItem*> eventVector;
  QVector<EventItem*> currentData;
  QVector<Arrow*> edgeVector;
  QVector<Arrow*> compareVector;
  QVector<NodeLabel*> nodeLabelVector;
  QVector<QGraphicsTextItem*> textVector;
  
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
  
  QPointer<QPushButton> plotButton;
  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> compareButton;
  QPointer<QPushButton> increaseDistanceButton;
  QPointer<QPushButton> decreaseDistanceButton;
  QPointer<QPushButton> toggleDetailsButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> previousEventButton;
  QPointer<QPushButton> nextEventButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> plotLabelsButton;
  QPointer<QPushButton> colorByAttributeButton;
  QPointer<QPushButton> eventColorButton;
  QPointer<QPushButton> labelColorButton;
  QPointer<QPushButton> backgroundColorButton;
  
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;

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
  
  QString selectedCoder;
  QString selectedCompare;
  QString selectedType;
  
  int distance;
  int vectorPos;

  bool labelsVisible;
};

#endif
