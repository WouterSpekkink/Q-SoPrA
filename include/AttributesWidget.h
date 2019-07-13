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


#ifndef ATTRIBUTESWIDGET_H
#define ATTRIBUTESWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QDropEvent>
#include <QHeaderView>
#include <QMessageBox>
#include "AttributeDialog.h"
#include "DeselectableTreeView.h"
#include "AttributeIndexDialog.h"
#include "AttributeTreeFilter.h"
#include "EventGraphWidget.h"
#include "MainWindow.h"
#include "AbstractNode.h"
#include "EventGraphWidget.h"
#include "OccurrenceGraphWidget.h"
#include "SupportingFunctions.h"
#include "MergeAttributesDialog.h"
#include "RelationshipComboBoxDialog.h"
#include "RelationshipsWidget.h"

class EventGraphWidget;

class AttributesWidget : public QWidget
{
  Q_OBJECT
  // MainWindow needs access to some private member functions
  friend class MainWindow;

public:
  // Constructor and desconstructor
  AttributesWidget(QWidget *parent=0);
  ~AttributesWidget() {};

  // Other widgets need to be able to reset the tree
  void resetTree();
  
  // Setters
  void setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr);
  void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);

private slots:
  // Private member functions
  void setCommentBool();
  void setComment();
  void previousIncident();
  void nextIncident();
  void jumpIncident();
  void toggleMark();
  void previousMarked();
  void nextMarked();
  void setDescriptionFilter(const QString &text);
  void previousDescription();
  void nextDescription();
  void setRawFilter(const QString &text);
  void previousRaw();
  void nextRaw();
  void setCommentFilter(const QString &text);
  void previousComment();
  void nextComment();
  void changeFilter(const QString &text);
  void retrieveData();
  void newAttribute();
  void editAttribute();
  void updateEntityAfterEdit(const QString name, const QString description, const QString oldName);
  void selectText();
  void sourceAttributeText(const QString &attribute, const int &incident);
  void highlightText();
  void assignAttribute();
  void unassignAttribute();
  void removeText();
  void resetTexts();
  void removeUnusedAttributes();
  void setTree();
  void expandTree();
  void collapseTree();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  void previousCoded();
  void nextCoded();
  void setButtons();
  void buildHierarchy(QStandardItem *top, QString name);
  void buildEntities(QStandardItem *top, QString name);
  void treeContextMenu(const QPoint &);
  void autoAssignAll();
  void autoAssignEntityAt(QModelIndex &index);
  void unassignAllEntities();
  void unassignAllAttribute(QModelIndex &index);
  void mergeAttributes(QModelIndex &index);
  void boldSelected(QAbstractItemModel *model, QString name = "", QModelIndex parent = QModelIndex());
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void changeTreeFontSize(QAbstractItemModel *model, QModelIndex parent = QModelIndex(),
			  int size = 0);
  void finalBusiness();
  void setValueButton();
  void setValue();
  void getValue();
  void fixTree();
  bool eventFilter(QObject *object, QEvent *event);
  
private:
  // Interface elements
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QSqlTableModel> attributesModel;
  QPointer<QSqlTableModel> assignedModel;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<QLabel> indexLabel;
  QPointer<QLabel> markLabel;
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> descriptionFilterLabel;
  QPointer<QLabel> rawFilterLabel;
  QPointer<QLabel> commentFilterLabel;
  QPointer<QLabel> attributeFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> descriptionFilterField;
  QPointer<QLineEdit> rawFilterField;
  QPointer<QLineEdit> commentFilterField;
  QPointer<QLineEdit> attributeFilterField;
  QPointer<QLineEdit> valueField;
  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;
  QPointer<QPushButton> previousIncidentButton;
  QPointer<QPushButton> nextIncidentButton;
  QPointer<QPushButton> jumpButton;
  QPointer<QPushButton> markButton;
  QPointer<QPushButton> previousMarkedButton;
  QPointer<QPushButton> nextMarkedButton;
  QPointer<QPushButton> descriptionPreviousButton;
  QPointer<QPushButton> descriptionNextButton;
  QPointer<QPushButton> rawPreviousButton;
  QPointer<QPushButton> rawNextButton;
  QPointer<QPushButton> commentPreviousButton;
  QPointer<QPushButton> commentNextButton;
  QPointer<QPushButton> newAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;
  QPointer<QPushButton> valueButton;
  QPointer<QPushButton> expandTreeButton;
  QPointer<QPushButton> collapseTreeButton;
  QPointer<QPushButton> previousCodedButton;
  QPointer<QPushButton> nextCodedButton;
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> resetTextsButton;

  // Private variables
  QString _descriptionFilter;
  QString _rawFilter;
  QString _commentFilter;
  bool _commentBool;

  // Pointers to other widgets
  // Do not delete
  QPointer<EventGraphWidget> _eventGraphWidgetPtr;
  QPointer<RelationshipsWidget> _relationshipsWidgetPtr;
};


#endif
