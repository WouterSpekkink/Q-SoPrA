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

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtWidgets/QTextEdit>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMimeData>

class TextEdit : public QTextEdit
{
  Q_OBJECT

public:
  TextEdit(QWidget *parent = 0);
  
private slots:
  void simplifyText();
  
private:
  void contextMenuEvent(QContextMenuEvent *event);
  void insertFromMimeData(const QMimeData *source);
};

#endif

