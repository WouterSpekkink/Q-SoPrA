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

#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QStyledItemDelegate>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>

class CheckBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  // Constructor and destructor
  CheckBoxDelegate(QObject *parent = 0);
  ~CheckBoxDelegate() {};

private:
  // Overrides
  void paint (QPainter * painter, const QStyleOptionViewItem & option,
	      const QModelIndex & index ) const;

  virtual bool editorEvent(QEvent *event, QAbstractItemModel *model,
			   const QStyleOptionViewItem &option,
			   const QModelIndex &index);
};
 

#endif
