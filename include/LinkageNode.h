/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef LINKAGENODE_H
#define LINKAGENODE_H

#include <QGraphicsItem>
#include <QWheelEvent>

class LinkageNodeLabel;
class AbstractNode;
class Scene;

class LinkageNode : public QGraphicsItem
{

public:
  // Constructor and destructor
  LinkageNode(QPointF originalPosition = QPointF(),
	      QString toolTip = QString(),
	      int id = -1,
	      int order = -1,
	      QGraphicsItem *parent = 0);
  // Although this class does have pointers
  // we don't want to delete these, because they may still be
  // in use by others.
  // The memory addressed they point to are not allocated
  // by this class.
  ~LinkageNode() {};

  // Overrides
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  QPainterPath shape() const override;
  
  // Setters
  void setOriginalPos(QPointF originalPos);
  void setLabel(LinkageNodeLabel *labelPtr);
  void setFirst();
  void setLast();
  void setDirection(QString direction);
  void setMiddle();
  void setTail();
  void setHead();
  void setUnselected();
  
  // Getters
  QPointF getOriginalPos() const;
  int getId() const;
  int getOrder() const;
  bool isFirst() const;
  bool isLast() const;
  LinkageNodeLabel* getLabel();
  QString getDirection();

  // Type recognition
  enum {Type = UserType + 18};
  int type() const;

protected:
  // Override of events
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  // Private variables
  QColor _color;
  QPointF _originalPos;
  int _id;
  int _width;
  int _order;
  bool _first;
  bool _last;
  bool _tail;
  bool _head;
  QString _direction;
  
  // Pointers to memory allocated by other classes
  // Do not delete.
  LinkageNodeLabel *_labelPtr;
};

#endif
