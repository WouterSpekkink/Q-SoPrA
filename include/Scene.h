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
#include "../include/NodeLabel.h"
#include "../include/OccurrenceItem.h"
#include "../include/LineObject.h"
#include "../include/TextObject.h"
#include "../include/EllipseObject.h"
#include "../include/MacroLabel.h"
#include "../include/OccurrenceLabel.h"
#include "../include/RectObject.h"

class Scene : public QGraphicsScene
{
  Q_OBJECT

public:
  Scene(QObject *parent = 0);
  QRectF itemsBoundingRect() const;
  OccurrenceItem* getSelectedOccurrence();

signals:
  void posChanged(EventItem *, qreal &dist);
  void posChanged(MacroEvent *, qreal &dist);
  void moveItems(QGraphicsItem *, QPointF);
  void moveLine(QGraphicsItem *, QPointF);
  void relevantChange();
  void resetItemSelection();
  void EventItemContextMenuAction(const QString&);
  void ArrowContextMenuAction(const QString&);
  void NetworkNodeContextMenuAction(const QString&);
  void LineContextMenuAction(const QString&);
  void TextContextMenuAction(const QString&);
  void EllipseContextMenuAction(const QString&);
  void RectContextMenuAction(const QString&);

private slots:
  void modEventWidth(QGraphicsItem* item);
  
protected:
  void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
  
  bool resizeOnEvent;
  bool resizeOnMacro;
  bool moveOn;
  bool lineMoveOn;
  bool moveLineObject;
  bool manipulateEllipse;
  bool moveEllipse;
  bool manipulateRect;
  bool rotateEllipse;
  bool moveRect;
  bool rotateRect;
  bool moveText;
  bool rotateText;
  bool hierarchyMove;
  bool eventWidthChange;
  QPointF lastMousePos;
  EventItem *selectedEvent;
  MacroEvent *selectedMacro;
  NetworkNode *selectedNode;
  OccurrenceItem *selectedOccurrence;
  LineObject *selectedLine;
  EllipseObject *selectedEllipse;
  RectObject *selectedRect;
  TextObject *selectedText;

  QColor edgeColor;
};

#endif 
