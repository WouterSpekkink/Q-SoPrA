#ifndef BANDLESSGRAPHICSVIEW_H
#define BANDLESSGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QGraphicsSceneMouseEvent>
#include "SimpleTextDialog.h"


class BandlessGraphicsView : public QGraphicsView {
  
public:
  BandlessGraphicsView(QGraphicsScene *scene);
  qreal panSpeed;
  
protected:
  void resizeEvent(QResizeEvent *) override;
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  bool pan;
  qreal scaleFact;
  QPoint lastMousePos;  
  
};


#endif
