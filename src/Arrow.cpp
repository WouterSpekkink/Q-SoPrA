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
#include <QtCore>
#include <QPen>
#include <QPainter>
#include "../include/Scene.h"

Arrow::Arrow(QString subType, QString subCoder, QGraphicsItem *parent)
  : QGraphicsLineItem(parent) {
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  color = Qt::black;
  setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  typeInd = subType;
  coder = subCoder;
  copy = false;
  theta = 0.0;
  height = 20;
}

QRectF Arrow::boundingRect() const {
  qreal extra = (pen().width() + 20) / 2.0;
  return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
				    line().p2().y() - line().p1().y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

void Arrow::updatePosition() {
  newLine = QLineF(mapFromItem(start, 0, 0), mapFromItem(end, 0, 0));
  setLine(newLine);
}

QPainterPath Arrow::shape() const {
  static const qreal clickTolerance = 8;
  QLineF myLine = line();
  myLine.setLength(myLine.length());
  QPointF vec = myLine.p2() - myLine.p1();
  vec = vec*(clickTolerance / sqrt(QPointF::dotProduct(vec, vec)));
  QPointF orthogonal(vec.y(), -vec.x());
  QPainterPath result(myLine.p1() - vec + orthogonal);
  result.lineTo(myLine.p1() - vec - orthogonal);
  result.lineTo(myLine.p2() + vec - orthogonal);
  result.lineTo(myLine.p2() + vec + orthogonal);
  result.closeSubpath();
  return result;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  calculate();
  QPen myPen = pen();
  arrowHead.clear();
  arrowHead << ghostLine.p2() << arrowP1 << arrowP2;
  QPainterPath myPath;
  myPath.moveTo(QPointF(0,0));
  myPath.quadTo(controlPoint, ghostLine.p2());
  strokePath = myPath;
  if (isSelected()) {
    painter->setPen(QPen(QColor(169, 169, 169, 255), 1, Qt::DashLine));
    painter->setBrush(Qt::transparent);
    painter->drawPath(shape());
  }
  myPen.setColor(color);
  painter->setPen(myPen);
  painter->setBrush(color);
  painter->translate(start->pos() - QPointF(0, 0));
  painter->rotate(theta);
  painter->drawPolygon(arrowHead);
  painter->strokePath(myPath, QPen(color));
}

void Arrow::calculate() {
  prepareGeometryChange();
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
    } else if (startX <= endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
    } else if (startX >= endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				endEvent->pos().y())));
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
    } else if (startX <= endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
    } else if (startX >= endStart) {
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				endMacro->pos().y())));
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
    } else if (startX <= endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX - (endMacro->getWidth() / 2) + 20, endMacro->pos().y())));
    } else if (startX >= endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX + (endMacro->getWidth() / 2) - 20,
				endMacro->pos().y())));
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
    } else if (startX <= endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX - (endEvent->getWidth() / 2) + 20, endEvent->pos().y())));
    } else if (startX >= endStart) {
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX + (endEvent->getWidth() / 2) - 20,
				endEvent->pos().y())));
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
    } else if (startX <= endStart) {
      newLine = QLineF(QPointF(startX, occurrenceStart->pos().y()),
		       (QPointF(endX - (occurrenceEnd->getWidth() / 2) + 20,
				occurrenceEnd->pos().y())));
    } else if (startX >= endStart) {
      newLine = QLineF(QPointF(startX, occurrenceStart->pos().y() - 20),
		       (QPointF(endX + (occurrenceEnd->getWidth() / 2) - 20,
				occurrenceEnd->pos().y())));
    }
  }
   // Let us first calculate the distance between our two points.
  qreal xDiff = newLine.p2().x() - newLine.p1().x();
  qreal yDiff = newLine.p2().y() - newLine.p1().y();
  qreal distance = sqrt(pow(xDiff, 2) + pow(yDiff, 2));
  qreal controlX = (0.0 + distance) / 2;
  controlPoint = QPointF(controlX, height);
  departure = controlPoint;
  departure.setY(departure.y() - (departure.y() / 3));
  theta = atan2(yDiff, xDiff);
  theta = qRadiansToDegrees(theta);
  ghostLine = QLineF(departure, QPointF(distance, 0));
  ghostLine.setLength(ghostLine.length() - 28);
  // Then we do some calculations to determine how the arrow is drawn.
  double angle = ::acos(ghostLine.dx() / ghostLine.length());
  if (ghostLine.dy() >= 0)
    angle = (Pi * 2) - angle;
  qreal arrowSize = 10;
  arrowP1 = ghostLine.p2() - QPointF(sin(angle + Pi /3) * arrowSize,
				cos(angle + Pi / 3) * arrowSize);
  arrowP2 = ghostLine.p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
				cos(angle + Pi - Pi / 3) * arrowSize);
  setLine(newLine);
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

QString Arrow::getType() {
  return typeInd;
}

void Arrow::setStartItem(QGraphicsItem *subStart) {
  start = subStart;
}

void Arrow::setEndItem(QGraphicsItem *subEnd) {
  end = subEnd;
}

