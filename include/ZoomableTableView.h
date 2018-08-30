#ifndef ZOOMABLETABLEVIEW_H
#define ZOOMABLETABLEVIEW_H

#include <QTableView>
#include <QWheelEvent>

class ZoomableTableView : public QTableView
{
  Q_OBJECT

public:
  ZoomableTableView(QWidget *parent = 0);
  ~ZoomableTableView() {};
  
protected:
  void wheelEvent(QWheelEvent *event);
};

#endif
