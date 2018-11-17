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

#ifndef NODESETTINGSDIALOG_H
#define NODESETTINGSDIALOG_H

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QTableView>
#include <QPointer>
#include <fstream>
#include "ZoomableTableView.h"
#include "SortFunctions.h"
#include "ProgressBar.h"
#include "AttributeSelectionDialog.h"
#include "ComboBoxDialog.h"

class NodeSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  NodeSettingsDialog(QWidget *parent = 0,
		     QVector<QString> entities = QVector<QString>(),
		     QVector<QString> descriptions = QVector<QString>(),
		     QVector<QString> modes = QVector<QString>());
  ~NodeSettingsDialog() {};
  
private slots:
  // Private member functions
  void cancelAndClose();
  void exportAndClose();
  void addAttribute();
  void removeAttribute();
  void findChildren(QString father, QVector<QString> *children);
  
private:
  // Interface elements
  QPointer<QTableWidget> tableWidget;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> removeAttributeButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> exportCloseButton;

  // Private data vectors
  QVector<QString> _entities;
  QVector<QString> _descriptions;
  QVector<QString> _modes;
  
};


#endif
