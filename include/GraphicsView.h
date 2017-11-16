#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>

class GraphicsView : public QGraphicsView {
  
public:
  GraphicsView(QGraphicsScene *scene);
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
