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

#include "../include/ZoomableListView.h"

ZoomableListView::ZoomableListView(QWidget *parent) : QListView(parent)
{
} 

void ZoomableListView::wheelEvent(QWheelEvent *event)
{
  if (event->modifiers() & Qt::ControlModifier)
    {
      if (event->angleDelta().y() > 0)
	{
	  QFont font = this->font();
	  int currentSize = font.pointSize();
	  if (currentSize <= 50)
	    {
	      int newSize = currentSize + 1;
	      font.setPointSize(newSize);
	      setFont(font);
	    }
	  return;
	}
      else if (event->angleDelta().y() < 0)
	{
	  QFont font = this->font();
	  int currentSize = font.pointSize();
	  if (currentSize >= 10)
	    {
	      int newSize = currentSize - 1;
	      font.setPointSize(newSize);
	      setFont(font);
	    }
	  return;
	}
    }
  else
    {
      QListView::wheelEvent(event);
    }
}
