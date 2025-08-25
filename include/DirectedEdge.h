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

#ifndef DIRECTEDEDGE_H
#define DIRECTEDEDGE_H

#include <QGraphicsLineItem>
#include <QSet>
#include "NetworkNode.h"
#include "SupportingFunctions.h"
#include "Constants.h"

class DirectedEdge : public QGraphicsLineItem
{

  public:
    // Constructor and destructor
    DirectedEdge(NetworkNode *start = NULL, NetworkNode *end = NULL,
                 QString type = QString(), QString name = QString(),
                 QGraphicsItem *parent = 0);
    ~DirectedEdge() {};

    // Overrides of public functions
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    // Setters
    void setColor(const QColor &color);
    void setType(const QString &type);
    void setHeight(const int &height);
    void setName(const QString &name);
    void setFiltered(bool state);
    void setMassHidden(bool state);
    void setComment(const QString &comment);
    void setPenWidth(const qreal &width);
    void setIncidents(const QSet<int> &incidents);
    void insertIncidents(const QSet<int> &incidents);
    void insertIncident(const int &incident);
    void setAntialiasing(bool state);
  
    // Getters;
    QColor getColor();
    NetworkNode* getStart() const;
    NetworkNode* getEnd() const;
    QString getType();
    int getHeight();
    QString getName();
    bool isFiltered();
    bool isMassHidden();
    QString getComment();
    QSet<int> getIncidents();
  
    // New function for correct drawing
    void updatePosition();

    // Type check
    enum {Type = UserType + 4};
    int type() const;

  private slots:
    // Calculate function to support paint function
    void calculate();
  
  protected:
    // Override of the pain function
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

  
  private:
    // Private variables
    QColor _color;
    QPolygonF _arrowHead;
    QPainterPath _strokePath;
    int _height;
    QLineF _ghostLine;
    QPointF _arrowP1;
    QPointF _arrowP2;
    QPointF _controlPoint;
    QPointF _controlPoint2;
    QString _relType;
    QString _name;
    QString _comment;
    bool _filtered;
    bool _massHidden;
    qreal _penWidth;
    bool _antialiasing;

    // Private data set
    QSet<int> _incidents;

    // Pointers to objects managed by other class.
    // Do not delete.
    NetworkNode *_start;
    NetworkNode *_end;
};

#endif
