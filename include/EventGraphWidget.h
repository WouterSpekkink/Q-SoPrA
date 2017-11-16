#ifndef EVENTGRAPHWIDGET
#define EVENTGRAPHWIDGET

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QVector>
#include <QPointer>
#include "GraphicsView.h"
#include "EventItem.h"
#include "Arrow.h"
#include <math.h>

class EventGraphWidget : public QWidget {
  Q_OBJECT

public:
  EventGraphWidget(QWidget *parent = 0);
  ~EventGraphWidget() {};

  void temp();			

private slots:
  void getEvents();
  void plotEvents();
  void getEdges();
  void plotEdges();

private:
  QPointer<QGraphicsScene> scene;
  QPointer<GraphicsView> view;
  QVector<QPointer<EventItem*>> eventVector;
  QVector<QPointer<Arrow*>> edgeVector;
   
};

#endif
