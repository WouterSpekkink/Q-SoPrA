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

#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <QGraphicsItem>
#include "SupportingFunctions.h"

class NetworkNodeLabel;

class NetworkNode : public QGraphicsItem
{

public:
  // Constructor and destructor
  NetworkNode(QString name, QString description);
  ~NetworkNode() {};

  // Some overrides of public functions
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  // Setters
  void setColor(const QColor &color);
  void setSelectionColor(const QColor &color);
  void setMode(const QString &mode);
  void setPersistent(bool state);
  void setMassHidden(bool state);
  void setLabel(NetworkNodeLabel *networkNodeLabelPtr);
  
  // Getters
  QColor getColor();
  int getCorrection();
  QString getName() const;
  QString getDescription() const;
  QString getMode() const;
  bool isPersistent();
  bool isMassHidden();
  NetworkNodeLabel* getLabel();

  // A function that enables moving of this objcet
  void move(QPointF newPos);

  // Type checking  
  enum {Type = UserType + 6};
  int type() const;
  
protected:
  // Overriding some events
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
  // Private variables
  QColor _color;
  QColor _selectionColor;
  QPointF _previousPos;
  QString _name;
  QString _description;
  QString _mode;
  bool _persistent;
  bool _massHidden;

  // Pointer to label, created by other class
  // Do not delete
  NetworkNodeLabel *_networkNodeLabelPtr;
};

#endif
