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

#ifndef LINEAGECOLORDIALOG_H
#define LINEAGECOLORDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QCheckBox>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"

class LineageColorDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  LineageColorDialog(QWidget *parent = 0);
  ~LineageColorDialog() {};

  // Getters
  QColor getOriginFillColor();
  QColor getAncestorFillColor();
  QColor getDescendantFillColor();
  QColor getUnrelatedFillColor();
  QColor getOriginTextColor();
  QColor getAncestorTextColor();
  QColor getDescendantTextColor();
  QColor getUnrelatedTextColor();
  bool modesOn();
  int getExitStatus();
			    
private slots:
  // Private member functions
  void setOriginFillColor();
  void setAncestorFillColor();
  void setDescendantFillColor();
  void setUnrelatedFillColor();
  void setOriginTextColor();
  void setAncestorTextColor();
  void setDescendantTextColor();
  void setUnrelatedTextColor();
  void cancelAndClose();
  void saveAndClose();
    
private:
  // Interface elements
  QPointer<QLabel> lineageLabel;
  QPointer<QLabel> fillColorOneLabel;
  QPointer<QLabel> fillColorTwoLabel;
  QPointer<QLabel> fillColorThreeLabel;
  QPointer<QLabel> fillColorFourLabel;
  QPointer<QLabel> textColorOneLabel;
  QPointer<QLabel> textColorTwoLabel;
  QPointer<QLabel> textColorThreeLabel;
  QPointer<QLabel> textColorFourLabel;
  QPointer<QPushButton> originFillButton;
  QPointer<QPushButton> ancestorFillButton;
  QPointer<QPushButton> descendantFillButton;
  QPointer<QPushButton> unrelatedFillButton;
  QPointer<QPushButton> originTextButton;
  QPointer<QPushButton> ancestorTextButton;
  QPointer<QPushButton> descendantTextButton;
  QPointer<QPushButton> unrelatedTextButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QCheckBox> modesCheckBox;

  // Private variables
  QColor _originFillColor;
  QColor _ancestorFillColor;
  QColor _descendantFillColor;
  QColor _unrelatedFillColor;
  QColor _originTextColor;
  QColor _ancestorTextColor;
  QColor _descendantTextColor;
  QColor _unrelatedTextColor;
  int _exitStatus;
};

#endif
