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

#ifndef ABSTRACTIONDIALOG_H
#define ABSTRACTIONDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include "SortFunctions.h"
#include "EventGraphWidget.h"
#include "InheritanceDialog.h"
#include "SupportingFunctions.h"

class AbstractionDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  AbstractionDialog(QWidget *parent = 0,
		    QVector<IncidentNode*> eventVector = QVector<IncidentNode*>(),
		    QVector<AbstractNode*> abstractNodeVector = QVector<AbstractNode*>(),
		    QVector<QGraphicsItem*> currentData = QVector<QGraphicsItem*>(),
		    QVector<QString> presentTypes = QVector<QString>(),
		    QString selectedCoder = QString());
  ~AbstractionDialog();

  // Getters
  QVector<IncidentNode*> getCollectedIncidents();
  QString getConstraint();
  QString getDescription();
  QString getTiming();
  QString getAttribute();
  bool isEntity();
  bool isInheriting();
  QVector<QString> getInheritance();
  int getExitStatus();
		     
private slots:
  // private member fuctions
  void selectAttribute();
  void clearAttribute();
  void inheritAttributes();
  void inheritSharedAttributes();
  void prepareEvents();
  void checkConstraints(QVector<IncidentNode*> submittedIncidents);
  void evaluateConstraints();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  QVector<bool> checkLinkagePresence(QVector<int> incidentIds);
  void cancelAndClose();
  void saveAndClose();
  
private:
  // Interface items
  QPointer<QLabel> constraintsLabel;
  QPointer<QLabel> attributeOptionsLabel;
  QPointer<QLabel> attributeLabel;
  QPointer<QLabel> attributeInheritanceLabel;
  QPointer<QLabel> chosenAttributeLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> timingLabel;
  QPointer<QRadioButton> pathsBasedRadioButton;
  QPointer<QRadioButton> semiPathsBasedRadioButton;
  QPointer<QRadioButton> compositeEventRadioButton;
  QPointer<QRadioButton> noConstraintsRadioButton;
  QPointer<QTextEdit> eventDescriptionField;
  QPointer<QLineEdit> timingField;
  QPointer<QPushButton> setAttributeButton;
  QPointer<QPushButton> clearAttributeButton;
  QPointer<QPushButton> inheritAttributesButton;
  QPointer<QPushButton> inheritSharedAttributesButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  // Private data vectors (these take their addresses from submitted vectors)
  QVector<IncidentNode*> _incidentNodeVector;
  QVector<AbstractNode*> _abstractNodeVector;
  QVector<IncidentNode*> _collectedIncidents;
  QVector<QGraphicsItem*> _currentData;
  QVector<QString> _presentTypes;
  QVector<QString> _inheritedAttributes;

  // Private variables  
  QString _selectedAttribute;
  QString _selectedCoder;
  QString _chosenConstraint;
  QString _eventDescription;
  QString _eventTiming;
  bool _attributeIsEntity;
  int _exitStatus;
  bool _semiPathsAllowed;
  bool _pathsAllowed;
  bool _compositeAllowed;
  bool _inheritance;
  
};

#endif
