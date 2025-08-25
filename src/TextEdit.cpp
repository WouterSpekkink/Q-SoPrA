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

#include "../include/TextEdit.h"

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void TextEdit::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu *menu = createStandardContextMenu();
  QAction *simplifyAction = new QAction(tr("Simplify"), this);
  connect(simplifyAction, SIGNAL(triggered()), this, SLOT(simplifyText()));
  simplifyAction->setShortcut(tr("CTRL+S"));

  menu->addAction(simplifyAction);
  if (this->textCursor().selectedText().length() == 0)
    {
      simplifyAction->setEnabled(false);
    }
  menu->exec(event->globalPos());
  delete simplifyAction;
  delete menu;
}

void TextEdit::simplifyText()
{
  QString replacement = this->textCursor().selectedText().simplified();
  this->textCursor().deleteChar(); 
  this->textCursor().insertText(replacement);
}

void TextEdit::insertFromMimeData(const QMimeData *source)
{
  QTextEdit::insertPlainText(source->text());
}
