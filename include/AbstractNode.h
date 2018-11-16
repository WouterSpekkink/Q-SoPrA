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

#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QGraphicsItem>
#include <QWheelEvent>
#include "SupportingFunctions.h"

class AbstractNodeLabel;
class IncidentNode;

class AbstractNode : public QGraphicsItem
{
  
public:
  // Constructor and destructor
  AbstractNode(int width = 40,
	       QString description = QString(),
	       QPointF originalPosition = QPointF(),
	       int id = -1,
	       QString constraint = QString(),
	       QVector<IncidentNode*> incidents = QVector<IncidentNode*>(),
	       QGraphicsItem *parent = 0);
  ~AbstractNode() {};

  // Overrides of public functions
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  
  // Setters
  void setColor(const QColor &color); 
  void setSelectionColor(const QColor &color);
  void setOriginalPos(const QPointF &originalPos);
  void setWidth(const int &width);
  void setDislodged(bool state);
  void setLabel(AbstractNodeLabel *labelPtr);
  void setIncidents(QVector<IncidentNode*> incidents);
  void setDescription(const QString description);  
  void setComment(const QString comment);
  void setAbstractNode(AbstractNode *abstractNodePtr);
  void setOrder(const int &order);
  void setAttributes(const QSet<QString> &attributes);
  void setValues(const QMap<QString, QString> &values);
  void setMode(const QString &mode);
  void setCopy(bool state);
  void setNewId(const int &id);
  void setTiming(const QString &timing);

  // Getters  
  int getCorrection();
  QPointF getOriginalPos();
  int getId();
  int getWidth() const;
  bool isDislodged();
  AbstractNodeLabel* getLabel();
  QVector<IncidentNode*> getIncidents() const;
  QColor getColor();
  QString getDescription();
  QString getComment();
  AbstractNode* getAbstractNode();
  int getOrder();
  const QSet<QString> getAttributes();
  QMap<QString, QString> getValues();
  QString getMode() const;
  QString getConstraint() const;
  bool isCopy();
  QString getTiming();
  
  // Functions for attribute management
  void insertAttribute(const QString &attribute);
  void removeAttribute(const QString &attribute);
  // Functions for value management
  void insertValue(const QString &attribute, const QString &value);
  void removeValue(const QString &attribute);

  // Type checking
  enum {Type = UserType + 8};
  int type() const;
  
protected:
  // Override of paint function
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

  // Overrides of events
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  
private:
  // Private variables
  QColor _color;
  QColor _selectionColor;
  QPointF _originalPos;
  int _id;
  int _width;
  int _order;
  bool _dislodged;
  QString _description;
  QString _comment;
  QString _mode;
  QString _constraint;
  QString _timing;
  bool _copy;

  // Private data vectors
  QSet<QString> _attributes;
  QMap<QString, QString> _values;
  // Do not delete these incidents
  QVector<IncidentNode*> _incidents;

  // Pointers to variables managed by other classes
  // Do not delete
  AbstractNodeLabel *_labelPtr;
  AbstractNode *_abstractNodePtr;

};

#endif
