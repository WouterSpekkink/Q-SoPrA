#ifndef NETWORKNODELABEL_H
#define NETWORKNODELABEL_H

#include <QGraphicsTextItem>
#include "NetworkNode.h"
#include <QGraphicsSceneMouseEvent>

class NetworkNode;

class NetworkNodeLabel : public QGraphicsTextItem
{
  
public:
  NetworkNodeLabel(NetworkNode *submittedNode = new NetworkNode("", ""));
  ~NetworkNodeLabel() {};

  NetworkNode* getNode();
  void setNewPos(QPointF nodePos);
  void setOriginalPos(QPointF nodePos);

  enum {Type = UserType + 7};
  int type() const;

  QPointF getOffset();
  void setOffset(QPointF offset);
  void increaseFontSize();
  void decreaseFontSize();
  int getFontSize();
  void setFontSize(int size);
    
private:
  NetworkNode *node;
  qreal xOffset;
  qreal yOffset;
  int fontSize;
};

#endif
