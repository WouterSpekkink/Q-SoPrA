#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "EventItem.h"
#include "Arrow.h"
#include "NetworkNode.h"
#include "Constants.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>
#include <QMenu>
#include "../include/NetworkNode.h"
#include "../include/OccurrenceItem.h"

class Scene : public QGraphicsScene {
  Q_OBJECT

public:
  Scene(QObject *parent = 0);
  QRectF itemsBoundingRect() const;
  OccurrenceItem* getSelectedOccurrence();

signals:
  void widthIncreased(EventItem *);
  void widthDecreased(EventItem *);
  void posIncreased(EventItem *);
  void posDecreased(EventItem *);
  void widthIncreased(MacroEvent *);
  void widthDecreased(MacroEvent *);
  void posIncreased(MacroEvent *);
  void posDecreased(MacroEvent *);
  void moveItems(QGraphicsItem *, QPointF);
  
  void relevantChange();
  void EventItemContextMenuAction(const QString&);
  void ArrowContextMenuAction(const QString&);
  
protected:
  void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
  
  bool resizeOnEvent;
  bool resizeOnMacro;
  bool moveOn;
  QPointF lastMousePos;
  EventItem *selectedEvent;
  MacroEvent *selectedMacro;
  NetworkNode *selectedNode;
  OccurrenceItem *selectedOccurrence;
};

#endif 
