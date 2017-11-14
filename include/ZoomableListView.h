#ifndef ZOOMABLELISTVIEW_H
#define ZOOMABLELISTVIEW_H

#include <QListView>
#include <QWheelEvent>

class ZoomableListView : public QListView {
  Q_OBJECT

public:
  ZoomableListView(QWidget *parent = 0);
  ~ZoomableListView() {};

protected:
  void wheelEvent(QWheelEvent *event);
  
};

#endif
