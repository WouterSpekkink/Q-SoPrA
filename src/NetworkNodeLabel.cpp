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
    
#include "../include/NetworkNodeLabel.h"
#include <QGraphicsScene>
#include <QtCore>
#include "../include/Scene.h"

NetworkNodeLabel::NetworkNodeLabel(NetworkNode *networkNodePtr) 
{
  setFlag(QGraphicsItem::ItemIsMovable);
  _networkNodePtr = networkNodePtr;
  _xOffset = 20;
  _yOffset = -15;
  _fontSize = 12;
  QFont newFont = this->font();
  newFont.setPointSizeF(_fontSize);
  this->setFont(newFont);
}

NetworkNode* NetworkNodeLabel::getNode() 
{
  return _networkNodePtr;
}

void NetworkNodeLabel::setNewPos(QPointF nodePos) 
{
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() + _xOffset);
  newPos.setY(newPos.y() + _yOffset);
  this->setPos(newPos);
}

void NetworkNodeLabel::setOriginalPos(QPointF nodePos) 
{
  QPointF newPos = nodePos;
  newPos.setX(newPos.x() + _xOffset);
  newPos.setY(newPos.y() + _yOffset);
  this->setPos(newPos);
}

QPointF NetworkNodeLabel::getOffset() 
{
  return QPointF(_xOffset, _yOffset);
}

void NetworkNodeLabel::setOffset(QPointF offset) 
{
  _xOffset = offset.x();
  _yOffset = offset.y();
}

int NetworkNodeLabel::type() const 
{
  return Type;
}

void NetworkNodeLabel::increaseFontSize() 
{
  if (_fontSize < 100) 
    {
      _fontSize++;
      QFont newFont = this->font();
      newFont.setPointSizeF(_fontSize);
      _yOffset -= 1;
      this->setFont(newFont);
      this->setNewPos(this->getNode()->scenePos());
    }
}

void NetworkNodeLabel::decreaseFontSize() 
{
  if (_fontSize > 5) 
    {
      _fontSize--;
      QFont newFont = this->font();
      newFont.setPointSizeF(_fontSize);
      _yOffset += 1; 
      this->setFont(newFont);
      this->setNewPos(this->getNode()->scenePos());
    }
}

void NetworkNodeLabel::setFontSize(int size) 
{
  _fontSize = size;
  QFont newFont = this->font();
  newFont.setPointSizeF(_fontSize);
  this->setFont(newFont);
}

int NetworkNodeLabel::getFontSize() 
{
  return _fontSize;
}
