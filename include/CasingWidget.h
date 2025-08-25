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

#ifndef CASINGWIDGET_H
#define CASINGWIDGET_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtSql>
#include <QHeaderView>
#include <QTableWidget>
#include "Constants.h"
#include "SupportingFunctions.h"
#include "CaseDialog.h"
#include "CheckBoxDelegate.h"
#include "ComboBoxDialog.h"
#include "Constants.h"
#include "SimpleAttributeSelectionDialog.h"
#include "EventGraphWidget.h"
#include "NetworkGraphWidget.h"
#include "OccurrenceGraphWidget.h"
#include "LinkagesWidget.h"

class OccurrenceGraphWidget;
class LinkagesWidget;

class CasingWidget : public QWidget
{
  Q_OBJECT

  // MainWindow needs access to private member functions
    friend class MainWindow;

  public:
    // Constructor and destructor
    CasingWidget(QWidget *parent = 0);
    ~CasingWidget() {};

    // Setters
    void setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr);
    void setNetworkGraphWidget(NetworkGraphWidget *networkGraphWidgetPtr);
    void setOccurrenceGraphWidget(OccurrenceGraphWidget *occurrenceGraphWidgetPtr);
    void setLinkagesWidget(LinkagesWidget *linkagesWidgetPtr);
    void setAttributesWidget(AttributesWidget *attributesWidgetPtr);
    void setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr);

  private slots:
    void createTable();
    void addCase();
    void editCase();
    void removeCase();
    void selectAll();
  
    void deselectAll();
    void attributeSelect();
    void findChildren(QString father, QVector<QString> *children, bool entity);
    void updateTable();
    void setCellState(QTableWidgetItem *item);
  
  private:
    // Interface elements
    QPointer<QTableWidget> tableWidget;
    QPointer<QPushButton> addCaseButton;
    QPointer<QPushButton> editCaseButton;
    QPointer<QPushButton> removeCaseButton;
    QPointer<QPushButton> selectAllButton;
    QPointer<QPushButton> deselectAllButton;
    QPointer<QPushButton> attributeSelectButton;

    // Pointers to other widgets
    // Do not delete.
    QPointer<EventGraphWidget> _eventGraphWidgetPtr;
    QPointer<NetworkGraphWidget> _networkGraphWidgetPtr;
    QPointer<OccurrenceGraphWidget> _occurrenceGraphWidgetPtr;
    QPointer<LinkagesWidget> _linkagesWidgetPtr;
    QPointer<AttributesWidget> _attributesWidgetPtr;
    QPointer<RelationshipsWidget> _relationshipsWidgetPtr;
};

#endif
