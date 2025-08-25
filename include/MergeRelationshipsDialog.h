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

#ifndef MERGERELATIONSHIPSDIALOG_H
#define MERGERELATIONSHIPSDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"
#include "DirectedEdge.h"
#include "UndirectedEdge.h"
#include "SupportingFunctions.h"

class MergeRelationshipsDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  MergeRelationshipsDialog(QWidget *parent = 0,
			   QVector<DirectedEdge*> *directed = new QVector<DirectedEdge*>,
			   QVector<UndirectedEdge*> *undirected = new QVector<UndirectedEdge*>,
			   QVector<QString> relationships = QVector<QString>(),
			   QVector<QString> directedness = QVector<QString>());
  ~MergeRelationshipsDialog() {};

  // Getters
  QVector<QString> getTypes();
  QString getName();
  QString getDescription();
  QString getDirectedness();
  int getExitStatus();
  
private slots:
  // Private member functions
  void cancelAndClose();
  void saveAndClose();

private:
  // Interface elements  
  QPointer<QLabel> relationshipsLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QVector<QPointer<QCheckBox>> relationshipsCheckBoxVector;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QLineEdit> nameField;
  QPointer<QTextEdit> descriptionField;

  // Private attributes
  QString _name;
  QString _description;
  QString _directedness;
  int _exitStatus;

  // Private data vectors
  QVector<QString> _types;
  QVector<QString> _directednessVector;
  // Contents created by other class
  // Do not delete
  QVector<DirectedEdge*> *_directed;
  QVector<UndirectedEdge*> *_undirected;

};

#endif
