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

#ifndef LINKAGESWIDGET_H
#define LINKAGESWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include <QWheelEvent>
#include <vector>
#include <thread>
#include <chrono>
#include "Constants.h"
#include "LinkageTypeDialog.h"
#include "SimpleTextDialog.h"
#include "LinkagesIndexDialog.h"
#include "EventGraphWidget.h"
#include "SupportingFunctions.h"

class EventGraphWidget;

class LinkagesWidget : public QWidget
{
  Q_OBJECT
  
  // MainWindow needs access to this class
  friend class MainWindow;

public:
  // Constructor and destructor
  LinkagesWidget(QWidget *parent=0);
  ~LinkagesWidget() {};

private slots:
  // Private member functions
  void addCoder();
  void editCoder();
  void removeCoder();
  void addLinkageType();
  void editLinkageType();
  void removeLinkageType();
  void setTypeButton();
  void setButtons(bool status);
  void setLinkageType();
  void switchLinkageType();
  void checkManualButton();
  void checkAssistedButton();
  void retrieveCoders();
  void retrieveLinkages();
  void retrieveData();
  void setTailDescriptionFilter(const QString &text);
  void setTailRawFilter(const QString &text);
  void setTailCommentFilter(const QString &text);
  void setHeadDescriptionFilter(const QString &text);
  void setHeadRawFilter(const QString &text);
  void setHeadCommentFilter(const QString &text);
  void previousTailDescription();
  void nextTailDescription();
  void previousTailRaw();
  void nextTailRaw();
  void previousTailComment();
  void nextTailComment();
  void previousHeadDescription();
  void nextHeadDescription();
  void previousHeadRaw();
  void nextHeadRaw();
  void previousHeadComment();
  void nextHeadComment();
  void previousTail();
  void nextTail();
  void markTail();
  void previousMarkedTail();
  void nextMarkedTail();
  void jumpTo();
  void previousHead();
  void nextHead();
  void markHead();
  void previousMarkedHead();
  void nextMarkedHead();
  void setCommentBool();
  void setComments();
  void setLinkageCommentBool();
  void setLinkageComment();
  void pause(int time);
  void setLink();
  void unsetLink();
  void markEvidence();
  void clearEvidence();
  void storeEvidence(const int tail, const int head);
  void highlightText();
  void selectTailText();
  void selectHeadText();
  void findPastPaths(QSet<int> *pIgnore, int currentIncident);
  void findFuturePaths(QSet<int> *pIgnore, int currentIncident);
  bool eventFilter(QObject *object, QEvent *event);
  void finalBusiness();
  
private:
  // Interface elements
  QPointer<QSqlTableModel> typesModel;
  QPointer<QSqlTableModel> linkagesModel;
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QLabel> settingsLabel;
  QPointer<QLabel> selectCoderLabel;
  QPointer<QLabel> selectTypeLabel;
  QPointer<QLabel> tailIndexLabel;
  QPointer<QLabel> tailMarkedLabel;
  QPointer<QLabel> tailTimeStampLabel;
  QPointer<QLabel> tailSourceLabel;
  QPointer<QLabel> tailDescriptionLabel;
  QPointer<QLabel> tailRawLabel;
  QPointer<QLabel> tailCommentLabel;
  QPointer<QLabel> tailDescriptionFilterLabel;
  QPointer<QLabel> tailRawFilterLabel;
  QPointer<QLabel> tailCommentFilterLabel;
  QPointer<QLabel> headIndexLabel;
  QPointer<QLabel> headMarkedLabel;
  QPointer<QLabel> headTimeStampLabel;
  QPointer<QLabel> headSourceLabel;
  QPointer<QLabel> headDescriptionLabel;
  QPointer<QLabel> headRawLabel;
  QPointer<QLabel> headCommentLabel;
  QPointer<QLabel> headDescriptionFilterLabel;
  QPointer<QLabel> headRawFilterLabel;
  QPointer<QLabel> headCommentFilterLabel;
  QPointer<QLabel> coderLabel;
  QPointer<QLabel> coderFeedbackLabel;
  QPointer<QLabel> linkageTypeLabel;
  QPointer<QLabel> linkageTypeFeedbackLabel;
  QPointer<QLabel> linkageQuestionLabel;
  QPointer<QLabel> linkageQuestionFeedbackLabel;
  QPointer<QLabel> linkageFeedbackLabel;
  QPointer<QLabel> linkageCommentLabel;
  QPointer<QLineEdit> tailTimeStampField;
  QPointer<QLineEdit> tailSourceField;
  QPointer<QLineEdit> tailDescriptionFilterField;
  QPointer<QLineEdit> tailRawFilterField;
  QPointer<QLineEdit> tailCommentFilterField;
  QPointer<QLineEdit> headTimeStampField;
  QPointer<QLineEdit> headSourceField;
  QPointer<QLineEdit> headDescriptionFilterField;
  QPointer<QLineEdit> headRawFilterField;
  QPointer<QLineEdit> headCommentFilterField;
  QPointer<QTextEdit> tailDescriptionField;
  QPointer<QTextEdit> tailRawField;
  QPointer<QTextEdit> tailCommentField;
  QPointer<QTextEdit> headDescriptionField;
  QPointer<QTextEdit> headRawField;
  QPointer<QTextEdit> headCommentField;
  QPointer<QTextEdit> linkageCommentField;
  QPointer<QComboBox> coderComboBox;
  QPointer<QComboBox> typeComboBox;
  QPointer<QPushButton> createCoderButton;
  QPointer<QPushButton> editCoderButton;
  QPointer<QPushButton> removeCoderButton;
  QPointer<QPushButton> createTypeButton;
  QPointer<QPushButton> editTypeButton;
  QPointer<QPushButton> removeTypeButton;
  QPointer<QPushButton> selectTypeButton;
  QPointer<QPushButton> manualCodingButton;
  QPointer<QPushButton> assistedCodingButton;
  QPointer<QPushButton> previousTailButton;
  QPointer<QPushButton> nextTailButton;
  QPointer<QPushButton> markTailButton;
  QPointer<QPushButton> previousMarkedTailButton;
  QPointer<QPushButton> nextMarkedTailButton;
  QPointer<QPushButton> tailDescriptionPreviousButton;
  QPointer<QPushButton> tailDescriptionNextButton;
  QPointer<QPushButton> tailRawPreviousButton;
  QPointer<QPushButton> tailRawNextButton;
  QPointer<QPushButton> tailCommentPreviousButton;
  QPointer<QPushButton> tailCommentNextButton;  
  QPointer<QPushButton> previousHeadButton;
  QPointer<QPushButton> nextHeadButton;
  QPointer<QPushButton> markHeadButton;
  QPointer<QPushButton> previousMarkedHeadButton;
  QPointer<QPushButton> nextMarkedHeadButton;
  QPointer<QPushButton> headDescriptionPreviousButton;
  QPointer<QPushButton> headDescriptionNextButton;
  QPointer<QPushButton> headRawPreviousButton;
  QPointer<QPushButton> headRawNextButton;
  QPointer<QPushButton> headCommentPreviousButton;
  QPointer<QPushButton> headCommentNextButton;
  QPointer<QPushButton> jumpButton;
  QPointer<QPushButton> setLinkButton;
  QPointer<QPushButton> unsetLinkButton;
  QPointer<QPushButton> switchLinkageTypeButton;
  QPointer<QPushButton> markEvidenceButton;
  QPointer<QPushButton> clearEvidenceButton;

  // Private data vectors
  QVector<QString> _markedTailEvidence;
  QVector<QString> _markedHeadEvidence;
  
  // Private variables
  QString _codingType;
  QString _selectedType;
  QString _selectedDirection;
  QString _selectedCoder;
  QString _tailDescriptionFilter;
  QString _tailRawFilter;
  QString _tailCommentFilter;
  QString _headDescriptionFilter;
  QString _headRawFilter;
  QString _headCommentFilter;
  QString _originalLinkageType;
  QString _originalCodingMode;
  bool _commentBool;
  bool _linkageCommentBool;
};

#endif
