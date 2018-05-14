#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsTextItem>
#include <QPointer>
#include <QGraphicsSceneMouseEvent>
#include "SimpleTextDialog.h"


class GraphicsView : public QGraphicsView {
  Q_OBJECT
  
public:
  GraphicsView(QGraphicsScene *scene);
  qreal panSpeed;
  bool isPanning();

signals:
  void EventGraphContextMenuAction(const QString&, const QPoint&);
  void NetworkGraphContextMenuAction(const QString&, const QPoint&);
  void OccurrenceGraphContextMenuAction(const QString&, const QPoint&);
  void changedView();
  
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
