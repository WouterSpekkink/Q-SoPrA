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

#ifndef EVENTNODESETTINGSDIALOG_H
#define EVENTNODESETTINGSDIALOG_H

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
#include "AbstractNode.h"
#include "AbstractNodeLabel.h"
#include "Constants.h"

class EventNodeSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructors
  EventNodeSettingsDialog(QWidget *parent = 0,
			  QVector<QString> ids = QVector<QString>(),
			  QVector<QString> labels = QVector<QString>(),
			  QVector<QString> timings = QVector<QString>(),
			  QVector<QString> descriptions = QVector<QString>(),
			  QVector<QString> comments = QVector<QString>(),
			  QVector<QString> types = QVector<QString>(),
			  QVector<QString> modes = QVector<QString>(),
			  QVector<QString> xCoords = QVector<QString>(),
			  QVector<QString> yCoords = QVector<QString>(),
			  QVector<AbstractNode*> abstractNodes = QVector<AbstractNode*>());
  ~EventNodeSettingsDialog() {};
  
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
  QVector<QString> _ids;
  QVector<QString> _labels;
  QVector<QString> _timings;
  QVector<QString> _descriptions;
  QVector<QString> _comments;
  QVector<QString> _types;
  QVector<QString> _modes;
  QVector<QString> _xCoords;
  QVector<QString> _yCoords;
  QVector<AbstractNode*> _abstractNodes;
};

#endif
