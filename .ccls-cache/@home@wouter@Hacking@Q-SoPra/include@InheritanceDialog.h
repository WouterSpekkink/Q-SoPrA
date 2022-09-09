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

#ifndef INHERITANCEDIALOG_H
#define INHERITANCEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QPointer>
#include <QSet>
#include <QList>
#include <QScrollArea>

class InheritanceDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  InheritanceDialog(QWidget *parent = 0,
		    QSet<QString> attributes = QSet<QString>());
  ~InheritanceDialog();

  // Getters
  int getExitStatus();
  QVector<QString> getSelected();
		     
private slots:
  // Private member functions
  void selectAll();
  void deselectAll();
  void saveAndClose();
  void cancelAndClose();
  
private:
  // Interface elements
  QPointer<QWidget> checkBoxWidget;
  QPointer<QScrollArea> scrollArea;
  QPointer<QLabel> titleLabel;
  QPointer<QPushButton> selectAllButton;
  QPointer<QPushButton> deselectAllButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QPushButton> cancelCloseButton;
  QVector<QString> _selectedAttributes;
  // This one we need to delete and clear
  QVector<QCheckBox*> checkBoxVector;

  // Private data vector
  QVector<QString> _selection;
  
  // Private variables
  int _exitStatus;


};

#endif
