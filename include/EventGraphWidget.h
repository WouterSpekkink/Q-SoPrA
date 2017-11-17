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

class EventGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;
  
public:
  EventGraphWidget(QWidget *parent = 0);
  ~EventGraphWidget() {};

private slots:
  void getEvents();
  void plotEvents();
  void getEdges();
  void plotEdges();
  void cleanUp();	     
  void increaseWidth(EventItem *item);
  void decreaseWidth(EventItem *item);
  void increasePos(EventItem *item);
  void decreasePos(EventItem *item);
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QVector<EventItem*> eventVector;
  QVector<Arrow*> edgeVector;
   
};

#endif
