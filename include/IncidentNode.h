/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2025 Wouter Spekkink  

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

#ifndef INCIDENTNODE_H
#define INCIDENTNODE_H

#include <QGraphicsItem>
#include <QWheelEvent>

class IncidentNodeLabel;
class AbstractNode;
class Scene;

class IncidentNode : public QGraphicsItem
{

public:
  // Constructor and destructor
  IncidentNode(int width = 40,
	    QString toolTip = QString(),
	    QPointF originalPosition = QPointF(),
	    int id = -1,
	    int order = -1,
	    QGraphicsItem *parent = 0);
  // Although this class does have pointers
  // we don't want to delete these, because they may still be
  // in use by others.
  // The memory addressed they point to are not allocated
  // by this class.
  ~IncidentNode() {};

  // Overrides
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  QPainterPath shape() const override;
  
  // Setters
  void setColor(const QColor &color); 
  void setSelectionColor(const QColor &color);
  void setOriginalPos(QPointF originalPos);
  void setWidth(int width);
  void setDislodged(bool state);
  void setLabel(IncidentNodeLabel *labelPtr);
  void setAbstractNode(AbstractNode* eventPtr);
  void setMode(const QString mode);
  void setCopy(bool state);
  void setMassHidden(bool state);
  
  // Getters
  QColor getColor();
  int getCorrection();
  QPointF getOriginalPos() const;
  int getId() const;
  int getOrder() const;
  int getWidth() const;
  bool isDislodged();
  IncidentNodeLabel* getLabel();
  AbstractNode* getAbstractNode();
  QString getMode() const;
  bool isCopy();
  bool isMassHidden();

  // Type recognition
  enum {Type = UserType + 1};
  int type() const;
  
protected:
  // Override of events
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  // Private variables
  QColor _color;
  QColor _selectionColor;
  QPointF _originalPos;
  QPointF _lastPos;
  int _id;
  int _width;
  bool _dislodged;
  int _order;
  QString _mode;
  bool _copy;
  bool _massHidden;

  // Pointers to memory allocated by other classes
  // Do not delete.
  AbstractNode *_abstractNodePtr;
  IncidentNodeLabel *_labelPtr;
};

#endif
