/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
  Modifications to the code below were made by Wouter Spekkink
*/


#include "../include/Arrow.h"
#include <math.h>

#include <QPen>
#include <QPainter>
#include "../include/Scene.h"


const qreal Pi = 3.14;

Arrow::Arrow(QGraphicsItem *startItem, QGraphicsItem *endItem, QString subType, QString subCoder,
	     QGraphicsItem *parent)
  : QGraphicsLineItem(parent) {
  start = startItem;
  end = endItem;
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  color = Qt::black;
  setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  typeInd = subType;
  coder = subCoder;
  copy = false;
}

QRectF Arrow::boundingRect() const {
  qreal extra = (pen().width() + 20) / 2.0;
  
  return QRectF(start->pos(), QSizeF(end->pos().x() - start->pos().x(),
				     end->pos().y() - start->pos().y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Arrow::shape() const {
  QPainterPath path = QGraphicsLineItem::shape();
  path.addPolygon(arrowHead);
  return path;
}

void Arrow::updatePosition() {
  QLineF line(mapFromItem(start, 0, 0), mapFromItem(end, 0, 0));
  setLine(line);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  calculate();
  QPen myPen = pen();
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);

  arrowHead.clear();
  arrowHead << line().p2() << arrowP1 << arrowP2;
  
  painter->drawPolygon(arrowHead);
  painter->drawLine(line());
  if (isSelected()) {
    painter->setPen(QPen(color, 1, Qt::DashLine));
    QLineF myLine = line();
    myLine.translate(0, 4.0);
    painter->drawLine(myLine);
    myLine.translate(0,-8.0);
    painter->drawLine(myLine);
  }
}

void Arrow::calculate() {
  prepareGeometryChange();
  qreal arrowSize = 10;
  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(start);
  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(end);
  MacroEvent *startMacro = qgraphicsitem_cast<MacroEvent*>(start);
  MacroEvent *endMacro = qgraphicsitem_cast<MacroEvent*>(end);
  OccurrenceItem *occurrenceStart = qgraphicsitem_cast<OccurrenceItem*>(start);
  OccurrenceItem *occurrenceEnd = qgraphicsitem_cast<OccurrenceItem*>(end);
  if (startEvent && endEvent) {
    // startX is middle of start node
    qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
    // endX is middle of end node
    qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
    // endStart is the left-most edge of the end node
    qreal endStart = endEvent->pos().x() - 20;
    if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endEvent->getWidth()) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(startX, endEvent->pos().y())));
      newLine.setLength(newLine.length() - 28);
    } else if (startX < endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
      newLine.setLength(newLine.length() - 28);
    } else if (startX > endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				endEvent->pos().y())));
      newLine.setLength(newLine.length() - 28);
    }
  } else if (startEvent && endMacro) {
    // startX is middle of start node
    qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
    // endX is middle of end node;
    qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
    // endStart is the left-most edge of the end node.
    qreal endStart = endMacro->pos().x() - 20;
    if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endMacro->getWidth()) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(startX, endMacro->pos().y())));
      newLine.setLength(newLine.length() - 28);

    } else if (startX < endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
      newLine.setLength(newLine.length() - 28);
    } else if (startX > endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				endMacro->pos().y())));
      newLine.setLength(newLine.length() - 28);
    }
  } else if (startMacro && endMacro) {
    // startX is middle of start node
    qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
    // endX is middle of end node;
    qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
    // endStart is the left-most edge of the end node.
    qreal endStart = endMacro->pos().x() - 20;
    if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endMacro->getWidth()) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(startX, endMacro->pos().y())));
      newLine.setLength(newLine.length() - 28);

    } else if (startX < endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
      newLine.setLength(newLine.length() - 28);
    } else if (startX > endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				endMacro->pos().y())));
      newLine.setLength(newLine.length() - 28);
    }
  } else if (startMacro && endEvent) {
    // startX is middle of start node
    qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
    // endX is middle of end node;
    qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
    // endStart is the left-most edge of the end node.
    qreal endStart = endEvent->pos().x() - 20;
    if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 + endEvent->getWidth()) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(startX, endEvent->pos().y())));
      newLine.setLength(newLine.length() - 28);
      
    } else if (startX < endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
      newLine.setLength(newLine.length() - 28);
    } else if (startX > endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				endEvent->pos().y())));
      newLine.setLength(newLine.length() - 28);
    }
  } else if (occurrenceStart && occurrenceEnd) {
    // startX is middle of start node
    qreal startX = occurrenceStart->pos().x() + (occurrenceStart->getWidth() / 2) - 20;
    // endX is middle of end node
    qreal endX = occurrenceEnd->pos().x() + (occurrenceEnd->getWidth() / 2) - 20;
    // endStart is the left-most edge of the end node
    qreal endStart = occurrenceEnd->pos().x() - 20;
    if (startX >= end->pos().x() - 20 && startX <= end->pos().x() - 20 +
	occurrenceEnd->getWidth()) {
      newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
		       (QPointF(startX, occurrenceEnd->pos().y())));
      newLine.setLength(newLine.length() - 28);
    } else if (startX < endStart) {
      newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
		       (QPointF(endX - (occurrenceEnd->getWidth() / 2) + 20,
				occurrenceEnd->pos().y())));
      newLine.setLength(newLine.length() - 28);
    } else if (startX > endStart) {
      newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
		       (QPointF(endX + (occurrenceEnd->getWidth() / 2) - 20,
				occurrenceEnd->pos().y())));
      newLine.setLength(newLine.length() - 28);
    }
  }
  
  setLine(newLine);
  
  double angle = ::acos(line().dx() / line().length());
  if (line().dy() >= 0)
    angle = (Pi * 2) - angle;


  arrowP1 = line().p2() - QPointF(sin(angle + Pi / 3) * arrowSize,
					  cos(angle + Pi / 3) * arrowSize);
  arrowP2 = line().p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
					  cos(angle + Pi - Pi / 3) * arrowSize);
  prepareGeometryChange();
}

void Arrow::setColor(const QColor &subColor) {
  color = subColor;
}

QColor Arrow::getColor() {
  return color;
}

QGraphicsItem* Arrow::startItem() const {
  return start;
}

QGraphicsItem* Arrow::endItem() const {
  return end;
}

int Arrow::type() const {
  return Type;
}

bool Arrow::isCopy() {
  return copy;
}

void Arrow::setCopy(bool status) {
  copy = status;
}

