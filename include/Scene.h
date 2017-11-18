#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "EventItem.h"
#include "Arrow.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>

class Scene : public QGraphicsScene {
  Q_OBJECT

public:
  Scene(QObject *parent = 0);
  QRectF itemsBoundingRect() const;

signals:
  void widthIncreased(EventItem *);
  void widthDecreased(EventItem *);
  void posIncreased(EventItem *);
  void posDecreased(EventItem *);
  void relevantChange();
  
protected:
  void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  bool resizeOn;
  QPointF lastMousePos;
  EventItem *selectedEvent;
  
};

#endif 
