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

#ifndef RELATIONSHIPSWIDGET_H
#define RELATIONSHIPSWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QDropEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QWheelEvent>
#include "AttributeDialog.h"
#include "DeselectableTreeView.h"
#include "RelationshipTypeDialog.h"
#include "RelationshipTreeFilter.h"
#include "RelationshipsDialog.h"
#include "AttributeIndexDialog.h"
#include "NetworkGraphWidget.h"
#include "EventGraphWidget.h"
#include "SupportingFunctions.h"
#include "EditEntityDialog.h"
#include "AttributesWidget.h"

class AttributesWidget;

class RelationshipsWidget : public QWidget
{
  Q_OBJECT
  // Some other classes need access to this class
  friend class MainWindow;
  friend class RelationshipsDialog;
  friend class EntityDialog;

  public:
    // Constructor and destructor
    RelationshipsWidget(QWidget *parent = 0);
    ~RelationshipsWidget() {};

    // Setters
    void setCurrentCoder(QString coder);
    void setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr);
    void setAttributesWidget(AttributesWidget *attributesWidgetPtr);
    void setNetworkGraphWidget(NetworkGraphWidget *networkGraphWidgetPtr);

    // This needs to be exposed
    void resetTree();
    void retrieveCases();
    void collectCase();

  private slots:
    // Private member functions
    void retrieveData();
    void setCase();
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
    void highlightSearch(QTextEdit *field, QString text);
    void editIncident();
    void setRawFilter(const QString &text);
    void previousRaw();
    void nextRaw();
    void setCommentFilter(const QString &text);
    void previousComment();
    void nextComment();
    void previousCoded();
    void nextCoded();
    void changeFilter(const QString &text);
    void setCommentButton();
    void submitRelationshipComment();
    void getComment();
    void newType();
    void editType();
    void entitiesOverview();
    void newRelationship();
    void editRelationship();
    void removeRelationship();
    void removeType();
    void assignRelationship();
    void unassignRelationship();
    void removeText();
    void resetTexts();
    void setButtons();
    void selectText();
    void sourceRelationshipText(const QString &relationship, const QString &type, const int &incident);
    void highlightText();
    void setTree();
    void treeContextMenu(const QPoint &pos);
    void unassignAll();
    void boldSelected(QAbstractItemModel *model, QString name = "",
                      QString type = "", QModelIndex parent = QModelIndex());
    void resetFont(QAbstractItemModel *model,
                   QModelIndex parent = QModelIndex());
    void expandTree();
    void collapseTree();
    void finalBusiness();
    bool eventFilter(QObject *object, QEvent *event);
  
  private:
    // Interface elements
    QPointer<QSqlTableModel> incidentsModel;
    QPointer<QSqlTableModel> typeModel;
    QPointer<QSqlTableModel> relationshipsModel;
    QPointer<QSqlTableModel> assignedModel;
    QPointer<QStandardItemModel> relationshipsTree;
    QPointer<DeselectableTreeView> relationshipsTreeView;
    QPointer<RelationshipTypeDialog> typeDialog;
    QPointer<RelationshipTreeFilter> treeFilter;
    QPointer<QLabel> indexLabel;
    QPointer<QLabel> markLabel;
    QPointer<QLabel> timeStampLabel;
    QPointer<QLabel> sourceLabel;
    QPointer<QLabel> descriptionLabel;
    QPointer<QLabel> rawLabel;
    QPointer<QLabel> commentLabel;
    QPointer<QLabel> relationshipsLabel;
    QPointer<QLabel> descriptionFilterLabel;
    QPointer<QLabel> rawFilterLabel;
    QPointer<QLabel> commentFilterLabel;
    QPointer<QLabel> relationshipFilterLabel;
    QPointer<QLabel> relationshipCommentLabel;
    QPointer<QLabel> caseLabel;
    QPointer<QLineEdit> timeStampField;
    QPointer<QLineEdit> sourceField;
    QPointer<QLineEdit> descriptionFilterField;
    QPointer<QLineEdit> rawFilterField;
    QPointer<QLineEdit> commentFilterField;
    QPointer<QLineEdit> relationshipFilterField;
    QPointer<QLineEdit> relationshipCommentField;
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
    QPointer<QPushButton> previousCodedButton;
    QPointer<QPushButton> nextCodedButton;
    QPointer<QPushButton> submitRelationshipCommentButton;
    QPointer<QPushButton> newTypeButton;
    QPointer<QPushButton> editTypeButton;
    QPointer<QPushButton> entitiesOverviewButton;
    QPointer<QPushButton> newRelationshipButton;
    QPointer<QPushButton> editRelationshipButton;
    QPointer<QPushButton> removeRelationshipButton;
    QPointer<QPushButton> removeTypeButton;
    QPointer<QPushButton> assignRelationshipButton;
    QPointer<QPushButton> unassignRelationshipButton;
    QPointer<QPushButton> expandTreeButton;
    QPointer<QPushButton> collapseTreeButton;
    QPointer<QPushButton> removeTextButton;
    QPointer<QPushButton> resetTextsButton;
    QPointer<QComboBox> caseComboBox;

    // Private variables
    QString _selectedCoder;
    QString _descriptionFilter;
    QString _rawFilter;
    QString _commentFilter;
    QString _selectedCase;
    bool _commentBool;

    // Private data vectors
    QSet<int> _caseIncidents;

    // Pointers to other widgets
    // Do not delete
    QPointer<EventGraphWidget> _eventGraphWidgetPtr;
    QPointer<AttributesWidget> _attributesWidgetPtr;
    QPointer<NetworkGraphWidget> _networkGraphWidgetPtr; // Needed forget entity dialog
};

#endif
