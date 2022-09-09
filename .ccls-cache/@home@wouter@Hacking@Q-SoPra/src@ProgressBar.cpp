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

#include "../include/ProgressBar.h"
#include <QtWidgets/QHBoxLayout>

ProgressBar::ProgressBar(QWidget *parent, int minimum, int maximum) : QDialog(parent)
{
  _minimum = minimum;
  _progressPtr = new QProgressBar(this);
  _progressPtr->setMinimum(minimum);
  _progressPtr->setMaximum(maximum);
  QHBoxLayout *barLayout = new QHBoxLayout;
  barLayout->addWidget(_progressPtr);
  setLayout(barLayout);
  setWindowTitle(tr("Importing data"));
  setFixedHeight(sizeHint().height());
  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
  setFixedWidth(300);
}

ProgressBar::~ProgressBar()
{
  delete _progressPtr;
}

void ProgressBar::setProgress(int value) 
{
  _progressPtr->setValue(value);
}
