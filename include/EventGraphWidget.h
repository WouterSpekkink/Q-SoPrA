#ifndef EVENTGRAPHWIDGET
#define EVENTGRAPHWIDGET

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QVector>
#include <QPointer>
#include <QtSql>
#include "GraphicsView.h"
#include "EventItem.h"
#include "Arrow.h"
#include "Scene.h"
#include <math.h>
#include "Constants.h"

class EventGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;
  
public:
  EventGraphWidget(QWidget *parent = 0);
  ~EventGraphWidget() {};

private slots:
  void getEvents();
  void plotEvents();
  void getEdges(QString coder, QString type);
  void plotEdges();
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
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QVector<EventItem*> eventVector;
  QVector<Arrow*> edgeVector;
  QVector<Arrow*> compareVector;

  QPointer<QLabel> coderLabel;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> compareLabel;

  QPointer<QPushButton> plotButton;
  QPointer<QPushButton> compareButton;
  QPointer<QPushButton> increaseDistanceButton;
  QPointer<QPushButton> decreaseDistanceButton;

  QPointer<QComboBox> coderComboBox;
  QPointer<QComboBox> typeComboBox;
  QPointer<QComboBox> compareComboBox;
  
  QString selectedCoder;
  QString selectedCompare;
  QString selectedType;
  
  int distance;
  
};

#endif
