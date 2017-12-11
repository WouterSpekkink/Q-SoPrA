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
  typeInd = subType;
  coder = subCoder;
}

QRectF Arrow::boundingRect() const {
  qreal extra = (pen().width() + 20) / 2.0;
  
  return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
				    line().p2().y() - line().p1().y()))
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
  Scene *myScene = qobject_cast<Scene*>(scene());
  myScene->relevantChange();
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
  QPen myPen = pen();
  myPen.setColor(color);
  qreal arrowSize = 10;
  painter->setPen(myPen);
  painter->setBrush(color);

  QLineF newLine = QLineF();

  EventItem *startEvent = qgraphicsitem_cast<EventItem*>(start);
  EventItem *endEvent = qgraphicsitem_cast<EventItem*>(end);
  MacroEvent *startMacro = qgraphicsitem_cast<MacroEvent*>(start);
  MacroEvent *endMacro = qgraphicsitem_cast<MacroEvent*>(end);
  if (start->pos().x() < end->pos().x()) {
    if (startEvent && endEvent) {
      if(endEvent->pos().x() < startEvent->pos().x() +
	 startEvent->getWidth() - endEvent->getWidth() / 2 - 20) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 (QPointF(startX, endEvent->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (endEvent->pos().x() < startEvent->pos().x() +
		 startEvent->getWidth()) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 endEvent->pos());
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2 - 20);
	newLine = QLineF(QPointF(startX, start->pos().y()), end->pos());
	newLine.setLength(newLine.length() - 23);
      }
    } else if (startEvent && endMacro) {
      if(endMacro->pos().x() < startEvent->pos().x() +
	 startEvent->getWidth() - endMacro->getWidth() / 2 - 20) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 (QPointF(startX, endMacro->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (endMacro->pos().x() < startEvent->pos().x() +
		 startEvent->getWidth()) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 endMacro->pos());
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2 - 20);
	newLine = QLineF(QPointF(startX, start->pos().y()), end->pos());
	newLine.setLength(newLine.length() - 23);
      }
    } else if (startMacro && endMacro) {
      if(endMacro->pos().x() < startMacro->pos().x() +
	 startMacro->getWidth() - endMacro->getWidth() / 2 - 20) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
	qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 2) - 20;
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 (QPointF(startX, endMacro->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (endMacro->pos().x() < startMacro->pos().x() +
		 startMacro->getWidth()) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 endMacro->pos());
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2 - 20);
	newLine = QLineF(QPointF(startX, start->pos().y()), end->pos());
	newLine.setLength(newLine.length() - 23);
      }
    } else if (startMacro && endEvent) {
      if (endEvent->pos().x() < startMacro->pos().x() +
	 startMacro->getWidth() - endEvent->getWidth() / 2 - 20) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
	qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 (QPointF(startX, endEvent->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (endEvent->pos().x() < startMacro->pos().x() +
		 startMacro->getWidth()) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 endEvent->pos());
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2 - 20);
	newLine = QLineF(QPointF(startX, start->pos().y()), end->pos());
	newLine.setLength(newLine.length() - 23);
      }
    }
  } else if (start->pos().x() > end->pos().x()) {
    if (startEvent && endEvent) {
      if(startEvent->pos().x() < endEvent->pos().x() +
	 endEvent->getWidth() - startEvent->getWidth() / 2 - 20) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 (QPointF(startX, endEvent->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (startEvent->pos().x() < endEvent->pos().x() +
		 endEvent->getWidth()) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	QPointF correction = QPointF(endEvent->getCorrection(), 0);
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 endEvent->pos() + correction);
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	QPointF correction = QPointF(endEvent->getCorrection(), 0);
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 endEvent->pos() + correction);
	newLine.setLength(newLine.length() - 23);
      }
    } else if (startEvent && endMacro) {
      if (startEvent->pos().x() < endMacro->pos().x() +
	  endMacro->getWidth() - startEvent->getWidth() / 2 - 20) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 4);
	qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 4);
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 (QPointF(endX, endMacro->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (startEvent->pos().x() < endMacro->pos().x() +
		 endMacro->getWidth()) {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
	QPointF correction = QPointF(endMacro->getCorrection(), 0);
	newLine = QLineF(QPointF(startX, startEvent->pos().y()),
			 endMacro->pos() + correction);
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2 - 20);
	qreal endX = endMacro->pos().x() + (endMacro->getWidth() - 20);
	newLine = QLineF(QPointF(startX, start->pos().y()),
			 QPointF(endX, end->pos().y()));
	newLine.setLength(newLine.length() - 23);
      }
    } else if (startMacro && endMacro) {
      if (startMacro->pos().x() < endMacro->pos().x() +
	  endMacro->getWidth() - startMacro->getWidth() / 2 - 20) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 4);
	qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 4);
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 (QPointF(endX, endMacro->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (startMacro->pos().x() < endMacro->pos().x() +
		 endMacro->getWidth()) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
	QPointF correction = QPointF(endMacro->getCorrection(), 0);
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 endMacro->pos() + correction);
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2 - 20);
	qreal endX = endMacro->pos().x() + (endMacro->getWidth() - 20);
	newLine = QLineF(QPointF(startX, start->pos().y()),
			 QPointF(endX, end->pos().y()));
	newLine.setLength(newLine.length() - 23);
      }
    } else if (startMacro && endEvent) {
      if (startMacro->pos().x() < endEvent->pos().x() +
	  endEvent->getWidth() - startMacro->getWidth() / 2 - 20) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 4);
	qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 4);
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 (QPointF(endX, endEvent->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
      } else if (startMacro->pos().x() < endEvent->pos().x() +
		 endEvent->getWidth()) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2) - 20;
	QPointF correction = QPointF(endEvent->getCorrection(), 0);
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 endEvent->pos() + correction);
	newLine.setLength(newLine.length() - 23);
      } else {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 2 - 20);
	qreal endX = endEvent->pos().x() + (endEvent->getWidth() - 20);
	newLine = QLineF(QPointF(startX, start->pos().y()),
			 QPointF(endX, end->pos().y()));
	newLine.setLength(newLine.length() - 23);
      }
    }
  } else if (start->pos().x() == end->pos().x()) {
    if (startEvent && endEvent) {
      qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 2) - 20;
      qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 2) - 20;
      qreal dist = abs(endX - startX);
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(startX, endEvent->pos().y())));
      newLine.setLength(newLine.length() + dist/8 - 28);
    } else if (startEvent && endMacro) {
      qreal startX = startEvent->pos().x() + (startEvent->getWidth() / 4);
      qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 4);
      qreal dist = abs(endX - startX);
      newLine = QLineF(QPointF(startX, startEvent->pos().y()),
		       (QPointF(endX, endMacro->pos().y())));
      newLine.setLength(newLine.length() + dist/8 - 28);
    } else if (startMacro && endMacro) {
      qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 4);
      qreal endX = endMacro->pos().x() + (endMacro->getWidth() / 4);
      qreal dist = abs(endX - startX);
      newLine = QLineF(QPointF(startX, startMacro->pos().y()),
		       (QPointF(endX, endMacro->pos().y())));
      newLine.setLength(newLine.length() + dist/8 - 28);
    } else if (startMacro && endEvent) {
	qreal startX = startMacro->pos().x() + (startMacro->getWidth() / 4);
	qreal endX = endEvent->pos().x() + (endEvent->getWidth() / 4);
	qreal dist = abs(endX - startX);
	newLine = QLineF(QPointF(startX, startMacro->pos().y()),
			 (QPointF(endX, endEvent->pos().y())));
	newLine.setLength(newLine.length() + dist/8 - 28);
    }
  }
  setLine(newLine);
  
  double angle = ::acos(line().dx() / line().length());
  if (line().dy() >= 0)
    angle = (Pi * 2) - angle;


  QPointF arrowP1 = line().p2() - QPointF(sin(angle + Pi / 3) * arrowSize,
					  cos(angle + Pi / 3) * arrowSize);
  QPointF arrowP2 = line().p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
					  cos(angle + Pi - Pi / 3) * arrowSize);

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

