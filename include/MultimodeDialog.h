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

#ifndef MULTIMODEDIALOG_H
#define MULTIMODEDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"
#include "DirectedEdge.h"
#include "UndirectedEdge.h"
#include "SupportingFunctions.h"
#include "SortFunctions.h"

class MultimodeDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  MultimodeDialog(QWidget *parent = 0, 
		  QVector<DirectedEdge*> *directed = new QVector<DirectedEdge*>,
		  QVector<UndirectedEdge*> *undirected = new QVector<UndirectedEdge*>);
  ~MultimodeDialog() {};

  // Setters
  void setModes(QVector<QString> modes);

  // Getters
  QString getModeOne();
  QString getModeTwo();
  QString getRelationshipOne();
  QString getRelationshipTwo();
  QString getName();
  QString getDescription();
  QString getDirectedness();
  int getExitStatus();
		     
private slots:
  // Private member functions
  void setModeOne(const QString &mode);
  void setModeTwo(const QString &mode);
  void setRelationshipOne(const QString &relationship);
  void setRelationshipTwo(const QString &relationship);
  void checkDirectedButton();
  void checkUndirectedButton();
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements  
  QPointer<QLabel> modeOneLabel; 
  QPointer<QLabel> modeTwoLabel;
  QPointer<QLabel> relationshipOneLabel;
  QPointer<QLabel> relationshipTwoLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> directednessLabel;
  QPointer<QComboBox> modeOneComboBox;
  QPointer<QComboBox> modeTwoComboBox;
  QPointer<QComboBox> relationshipOneComboBox;
  QPointer<QComboBox> relationshipTwoComboBox;
  QPointer<QPushButton> directedButton;
  QPointer<QPushButton> undirectedButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QLineEdit> nameField;
  QPointer<QTextEdit> descriptionField;

  // Private variables
  QVector<QString> _modes;
  QString _name;
  QString _description;
  QString _modeOne;
  QString _modeTwo;
  QString _relationshipOne;
  QString _relationshipTwo;
  QString _directedness;
  int _exitStatus;

  // Private data vectors
  // Contents created by other classes
  // Do not delete.
  QVector<DirectedEdge*> *_directed;
  QVector<UndirectedEdge*> *_undirected;


};

#endif
