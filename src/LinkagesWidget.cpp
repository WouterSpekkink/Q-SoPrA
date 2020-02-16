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

#include "../include/LinkagesWidget.h"

LinkagesWidget::LinkagesWidget(QWidget *parent) : QWidget(parent) 
{
  _codingType = MANUAL;
  _selectedType = "";
  _selectedDirection = "";
  _selectedCoder = "";
  _tailDescriptionFilter = "";
  _tailRawFilter = "";
  _tailCommentFilter = "";
  _headDescriptionFilter = "";
  _headRawFilter = "";
  _headCommentFilter = "";
  _commentBool = false;
  _linkageCommentBool = false;
  
  typesModel = new QSqlTableModel(this);
  typesModel->setTable("linkage_types");
  typesModel->select();

  linkagesModel = new QSqlTableModel(this);
  linkagesModel->setTable("linkages");
  linkagesModel->select();

  incidentsModel = new QSqlTableModel(this);
  incidentsModel->setTable("incidents");
  incidentsModel->select();
  
  settingsLabel = new QLabel(tr("<h2>Settings:</h2>"), this);
  selectCoderLabel = new QLabel(tr("<b>Select coder:</b>"), this);
  selectTypeLabel =  new QLabel(tr("<b>Select linkage type:</b>"), this);
  tailIndexLabel = new QLabel(tr("<b>Tail ( / ) - Incident:</b>"), this);
  tailMarkedLabel = new QLabel("");
  tailMarkedLabel->setStyleSheet("QLabel {color: red}");
  tailTimeStampLabel = new QLabel(tr("<b>Timing:</b>"), this);
  tailSourceLabel = new QLabel(tr("<b>Source:</b>"), this);
  tailDescriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  tailRawLabel = new QLabel(tr("<b>Raw:</b>"), this);
  tailCommentLabel = new QLabel(tr("<b>Comments:</b>"), this);
  tailDescriptionFilterLabel = new QLabel(tr("<i>Search descriptions:</i>"), this);
  tailRawFilterLabel = new QLabel(tr("<i>Search raw texts:</i>"), this);
  tailCommentFilterLabel = new QLabel(tr("<i>Search comments:</i>"), this);
  headIndexLabel = new QLabel(tr("<b>Head ( / ) - Incident:</b>"), this);
  headMarkedLabel = new QLabel("");
  headMarkedLabel->setStyleSheet("QLabel {color: red}");
  headTimeStampLabel = new QLabel(tr("<b>Timing:</b>"), this);
  headSourceLabel = new QLabel(tr("<b>Source:</b>"), this);
  headDescriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  headRawLabel = new QLabel(tr("<b>Raw:</b>"), this);
  headCommentLabel = new QLabel(tr("<b>Comments:</b>"), this);
  headDescriptionFilterLabel = new QLabel(tr("<i>Search descriptions:</i>"), this);
  headRawFilterLabel = new QLabel(tr("<i>Search raw texts:</i>"), this);
  headCommentFilterLabel = new QLabel(tr("<i>Search comments:</i>"), this);
  coderLabel = new QLabel(tr("<b>Coder:</b>"), this);
  coderFeedbackLabel = new QLabel(tr(""));
  linkageTypeLabel = new QLabel(tr("<b>Linkage type:</b>"), this);
  linkageTypeFeedbackLabel = new QLabel(tr(""), this);
  linkageQuestionLabel = new QLabel(tr("<b>Linkage question:</b>"), this);
  linkageQuestionFeedbackLabel = new QLabel(tr(""), this);
  linkageQuestionFeedbackLabel->setWordWrap(true);
  linkageQuestionFeedbackLabel->setMaximumWidth(200);
  linkageFeedbackLabel = new QLabel(tr(""), this);
  linkageCommentLabel = new QLabel(tr("<b>Linkage comments:</b>"), this);

  tailTimeStampField = new QLineEdit(this);
  tailTimeStampField->setReadOnly(true);
  tailSourceField = new QLineEdit(this);
  tailSourceField->setReadOnly(true);
  tailDescriptionFilterField = new QLineEdit(this);
  tailRawFilterField = new QLineEdit(this);
  tailCommentFilterField = new QLineEdit(this);
  headTimeStampField = new QLineEdit(this);
  headSourceField = new QLineEdit(this);
  headDescriptionFilterField = new QLineEdit(this);
  headRawFilterField = new QLineEdit(this);
  headCommentFilterField = new QLineEdit(this);

  tailDescriptionField = new QTextEdit(this);
  tailDescriptionField->setReadOnly(true);
  tailRawField = new QTextEdit(this);
  tailRawField->setReadOnly(true);
  tailCommentField = new QTextEdit(this);
  headDescriptionField = new QTextEdit(this);
  headDescriptionField->setReadOnly(true);
  headRawField = new QTextEdit(this);
  headRawField->setReadOnly(true);
  headCommentField = new QTextEdit(this);
  linkageCommentField = new QTextEdit(this);

  coderComboBox = new QComboBox(this);
  coderComboBox->addItem(DEFAULT);
  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);

  createCoderButton = new QPushButton(tr("New coder"), this);
  editCoderButton = new QPushButton(tr("Edit selected coder"), this);
  removeCoderButton = new QPushButton(tr("Remove selected coder"), this);
  createTypeButton = new QPushButton(tr("New linkage type"), this);
  editTypeButton = new QPushButton(tr("Edit selected type"), this);
  removeTypeButton = new QPushButton(tr("Remove selected type"), this);
  selectTypeButton = new QPushButton(tr("Load selected linkage"), this);
  selectTypeButton->setStyleSheet("QPushButton {font-weight: bold}");
  selectTypeButton->setEnabled(false);
  switchLinkageTypeButton = new QPushButton(tr("Switch linkage type"), this);
  switchLinkageTypeButton->setEnabled(false);
  manualCodingButton = new QPushButton(tr("Manual coding"), this);
  manualCodingButton->setCheckable(true);
  manualCodingButton->setChecked(true);
  manualCodingButton->setStyleSheet("QPushButton {font-weight: bold}");
  assistedCodingButton = new QPushButton(tr("Assisted coding"), this);
  assistedCodingButton->setCheckable(true);
  assistedCodingButton->setChecked(false);
  assistedCodingButton->setStyleSheet("QPushButton {font-weight: bold}");
  previousTailButton = new QPushButton(tr("Previous tail"), this);
  previousTailButton->setStyleSheet("QPushButton {font-weight: bold}");
  previousTailButton->setEnabled(false);
  nextTailButton = new QPushButton(tr("Next tail"), this);
  nextTailButton->setStyleSheet("QPushButton {font-weight: bold}");
  markTailButton = new QPushButton(tr("Toggle mark"), this);
  previousMarkedTailButton = new QPushButton(tr("Previous marked"), this);
  nextMarkedTailButton = new QPushButton(tr("Next marked"), this);
  tailDescriptionPreviousButton = new QPushButton(tr("Previous"), this);
  tailDescriptionNextButton = new QPushButton(tr("Next"), this);
  tailRawPreviousButton = new QPushButton(tr("Previous"), this);
  tailRawNextButton = new QPushButton(tr("Next"), this);
  tailCommentPreviousButton = new QPushButton(tr("Previous"), this);
  tailCommentNextButton = new QPushButton(tr("Next"), this);  
  previousHeadButton = new QPushButton(tr("Previous head"), this);
  previousHeadButton->setStyleSheet("QPushButton {font-weight: bold}");
  nextHeadButton = new QPushButton(tr("Next head"), this);
  nextHeadButton->setStyleSheet("QPushButton {font-weight: bold}");
  markHeadButton = new QPushButton(tr("Toggle mark"), this);
  previousMarkedHeadButton = new QPushButton(tr("Previous marked"), this);
  nextMarkedHeadButton = new QPushButton(tr("Next marked"), this);
  headDescriptionPreviousButton = new QPushButton(tr("Previous"), this);
  headDescriptionNextButton = new QPushButton(tr("Next"), this);  
  headRawPreviousButton = new QPushButton(tr("Previous"), this);
  headRawNextButton = new QPushButton(tr("Next"), this);  
  headCommentPreviousButton = new QPushButton(tr("Previous"), this);
  headCommentNextButton = new QPushButton(tr("Next"), this);  
  jumpButton = new QPushButton(tr("Jump to"), this);
  setLinkButton = new QPushButton(tr("Linked"), this);
  setLinkButton->setEnabled(false);
  unsetLinkButton =new QPushButton(tr("Not linked"), this);
  unsetLinkButton->setEnabled(false);
  markEvidenceButton = new QPushButton(tr("Mark evidence"), this);
  markEvidenceButton->setEnabled(false);
  clearEvidenceButton = new QPushButton(tr("Clear evidence"), this);
  clearEvidenceButton->setEnabled(false);
  
  linkageCommentField->installEventFilter(this);
  headTimeStampField->installEventFilter(this);
  tailTimeStampField->installEventFilter(this);
  headSourceField->installEventFilter(this);
  tailSourceField->installEventFilter(this);
  headDescriptionField->viewport()->installEventFilter(this);
  tailDescriptionField->viewport()->installEventFilter(this);
  headCommentField->installEventFilter(this);
  tailCommentField->installEventFilter(this);
  tailRawField->viewport()->installEventFilter(this);
  headRawField->viewport()->installEventFilter(this);
  setButtons(false);
  
  connect(coderComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setTypeButton()));
  connect(createCoderButton, SIGNAL(clicked()), this, SLOT(addCoder()));
  connect(editCoderButton, SIGNAL(clicked()), this, SLOT(editCoder()));
  connect(removeCoderButton, SIGNAL(clicked()), this, SLOT(removeCoder()));
  connect(typeComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(setTypeButton()));
  connect(createTypeButton, SIGNAL(clicked()), this, SLOT(addLinkageType()));
  connect(editTypeButton, SIGNAL(clicked()), this, SLOT(editLinkageType()));
  connect(removeTypeButton, SIGNAL(clicked()), this, SLOT(removeLinkageType()));
  connect(selectTypeButton, SIGNAL(clicked()), this, SLOT(setLinkageType()));
  connect(switchLinkageTypeButton, SIGNAL(clicked()), this, SLOT(switchLinkageType()));
  connect(manualCodingButton, SIGNAL(clicked()), this, SLOT(checkManualButton()));
  connect(assistedCodingButton, SIGNAL(clicked()), this, SLOT(checkAssistedButton()));
  connect(tailDescriptionFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setTailDescriptionFilter(const QString &)));
  connect(tailRawFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setTailRawFilter(const QString &)));
  connect(tailCommentFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setTailCommentFilter(const QString &)));
  connect(headDescriptionFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setHeadDescriptionFilter(const QString &)));
  connect(headRawFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setHeadRawFilter(const QString &)));
  connect(headCommentFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setHeadCommentFilter(const QString &)));
  connect(tailDescriptionPreviousButton, SIGNAL(clicked()), this, SLOT(previousTailDescription()));
  connect(tailDescriptionNextButton, SIGNAL(clicked()), this, SLOT(nextTailDescription()));
  connect(tailRawPreviousButton, SIGNAL(clicked()), this, SLOT(previousTailRaw()));
  connect(tailRawNextButton, SIGNAL(clicked()), this, SLOT(nextTailRaw()));
  connect(tailCommentPreviousButton, SIGNAL(clicked()), this, SLOT(previousTailComment()));
  connect(tailCommentNextButton, SIGNAL(clicked()), this, SLOT(nextTailComment()));
  connect(headDescriptionPreviousButton, SIGNAL(clicked()), this, SLOT(previousHeadDescription()));
  connect(headDescriptionNextButton, SIGNAL(clicked()), this, SLOT(nextHeadDescription()));
  connect(headRawPreviousButton, SIGNAL(clicked()), this, SLOT(previousHeadRaw()));
  connect(headRawNextButton, SIGNAL(clicked()), this, SLOT(nextHeadRaw()));
  connect(headCommentPreviousButton, SIGNAL(clicked()), this, SLOT(previousHeadComment()));
  connect(headCommentNextButton, SIGNAL(clicked()), this, SLOT(nextHeadComment()));
  connect(previousTailButton, SIGNAL(clicked()), this, SLOT(previousTail()));
  connect(nextTailButton, SIGNAL(clicked()), this, SLOT(nextTail()));
  connect(markTailButton, SIGNAL(clicked()), this, SLOT(markTail()));
  connect(previousMarkedTailButton, SIGNAL(clicked()), this, SLOT(previousMarkedTail()));
  connect(nextMarkedTailButton, SIGNAL(clicked()), this, SLOT(nextMarkedTail()));
  connect(jumpButton, SIGNAL(clicked()), this, SLOT(jumpTo()));
  connect(previousHeadButton, SIGNAL(clicked()), this, SLOT(previousHead()));
  connect(nextHeadButton, SIGNAL(clicked()), this, SLOT(nextHead()));
  connect(markHeadButton, SIGNAL(clicked()), this, SLOT(markHead()));
  connect(previousMarkedHeadButton, SIGNAL(clicked()), this, SLOT(previousMarkedHead()));
  connect(nextMarkedHeadButton, SIGNAL(clicked()), this, SLOT(nextMarkedHead()));
  connect(tailCommentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(headCommentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(setLinkButton, SIGNAL(clicked()), this, SLOT(setLink()));
  connect(unsetLinkButton, SIGNAL(clicked()), this, SLOT(unsetLink()));
  connect(markEvidenceButton, SIGNAL(clicked()), this, SLOT(markEvidence()));
  connect(clearEvidenceButton, SIGNAL(clicked()), this, SLOT(clearEvidence()));
  connect(linkageCommentField, SIGNAL(textChanged()), this, SLOT(setLinkageCommentBool()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));

  // I want to set the size of some widgets based on the availabe screen width
  QRect rect = QApplication::primaryScreen()->availableGeometry();
  int width = rect.width();
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> settingsLayout = new QHBoxLayout;
  settingsLayout->addWidget(settingsLabel);
  settingsLabel->setAlignment(Qt::AlignCenter);
  settingsLabel->setMaximumWidth(200);
  settingsLayout->addSpacerItem(new QSpacerItem(40, 0));
  QPointer<QVBoxLayout> linkageOptionsLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> coderLayout = new QHBoxLayout;
  coderLayout->addWidget(selectCoderLabel);
  coderLayout->addWidget(coderComboBox);
  coderComboBox->setMaximumWidth(200);
  coderLayout->addWidget(createCoderButton);
  createCoderButton->setMaximumWidth(createCoderButton->sizeHint().width());
  coderLayout->addWidget(editCoderButton);
  editCoderButton->setMaximumWidth(editCoderButton->sizeHint().width());
  coderLayout->addWidget(removeCoderButton);
  removeCoderButton->setMaximumWidth(removeCoderButton->sizeHint().width());
  coderLayout->addWidget(manualCodingButton);
  manualCodingButton->setMaximumWidth(manualCodingButton->sizeHint().width());
  coderLayout->addWidget(assistedCodingButton);
  assistedCodingButton->setMaximumWidth(assistedCodingButton->sizeHint().width());
  linkageOptionsLayout->addLayout(coderLayout);
  coderLayout->setAlignment(Qt::AlignLeft);
  QPointer<QHBoxLayout> typeLayout = new QHBoxLayout;
  typeLayout->addWidget(selectTypeLabel);
  typeLayout->addWidget(typeComboBox);
  typeComboBox->setMaximumWidth(200);
  typeLayout->addWidget(createTypeButton);
  createTypeButton->setMaximumWidth(createTypeButton->sizeHint().width());
  typeLayout->addWidget(editTypeButton);
  editTypeButton->setMaximumWidth(editTypeButton->sizeHint().width());
  typeLayout->addWidget(removeTypeButton);
  removeTypeButton->setMaximumWidth(removeTypeButton->sizeHint().width());
  linkageOptionsLayout->addLayout(typeLayout);
  linkageOptionsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  typeLayout->addWidget(selectTypeButton);
  selectTypeButton->setMaximumWidth(selectTypeButton->sizeHint().width());
  settingsLayout->addLayout(linkageOptionsLayout);
  mainLayout->addLayout(settingsLayout);
  settingsLayout->setAlignment(Qt::AlignCenter);

  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);
  
  QPointer<QHBoxLayout> fieldsLayout = new QHBoxLayout;
  
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> tailIndexLayout = new QHBoxLayout;
  tailIndexLayout->addWidget(tailIndexLabel);
  tailIndexLayout->addWidget(tailMarkedLabel);
  tailIndexLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  leftLayout->addLayout(tailIndexLayout);
  QPointer<QHBoxLayout> tailTimeLayout = new QHBoxLayout;
  tailTimeLayout->addWidget(tailTimeStampLabel);
  tailTimeLayout->addWidget(tailTimeStampField);
  tailTimeLayout->addWidget(tailSourceLabel);
  tailTimeLayout->addWidget(tailSourceField);
  leftLayout->addLayout(tailTimeLayout);
  QPointer<QHBoxLayout> tailDescriptionLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> tailDescriptionLayoutLeft = new QHBoxLayout;
  tailDescriptionLayoutLeft->addWidget(tailDescriptionLabel);
  tailDescriptionLayout->addLayout(tailDescriptionLayoutLeft);
  QPointer<QHBoxLayout> tailDescriptionLayoutRight = new QHBoxLayout;
  tailDescriptionLayoutRight->addWidget(tailDescriptionFilterLabel);
  tailDescriptionFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  tailDescriptionLayoutRight->addWidget(tailDescriptionPreviousButton);
  tailDescriptionPreviousButton->setMaximumWidth(tailDescriptionPreviousButton->sizeHint().width());
  tailDescriptionLayoutRight->addWidget(tailDescriptionFilterField);
  tailDescriptionLayoutRight->addWidget(tailDescriptionNextButton);
  tailDescriptionNextButton->setMaximumWidth(tailDescriptionNextButton->sizeHint().width());
  tailDescriptionLayout->addLayout(tailDescriptionLayoutRight);
  leftLayout->addLayout(tailDescriptionLayout);
  leftLayout->addWidget(tailDescriptionField);
  QPointer<QHBoxLayout> tailRawLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> tailRawLayoutLeft = new QHBoxLayout;
  tailRawLayoutLeft->addWidget(tailRawLabel);
  tailRawLayout->addLayout(tailRawLayoutLeft);
  QPointer<QHBoxLayout> tailRawLayoutRight = new QHBoxLayout;
  tailRawLayoutRight->addWidget(tailRawFilterLabel);
  tailRawFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  tailRawLayoutRight->addWidget(tailRawPreviousButton);
  tailRawPreviousButton->setMaximumWidth(tailRawPreviousButton->sizeHint().width());
  tailRawLayoutRight->addWidget(tailRawFilterField);  
  tailRawLayoutRight->addWidget(tailRawNextButton);
  tailRawNextButton->setMaximumWidth(tailRawNextButton->sizeHint().width());
  tailRawLayout->addLayout(tailRawLayoutRight);
  leftLayout->addLayout(tailRawLayout);
  leftLayout->addWidget(tailRawField);
  QPointer<QHBoxLayout> tailCommentLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> tailCommentLayoutLeft = new QHBoxLayout;
  tailCommentLayoutLeft->addWidget(tailCommentLabel);
  tailCommentLayout->addLayout(tailCommentLayoutLeft);
  QPointer<QHBoxLayout> tailCommentLayoutRight = new QHBoxLayout;
  tailCommentLayoutRight->addWidget(tailCommentFilterLabel);
  tailCommentFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  tailCommentLayoutRight->addWidget(tailCommentPreviousButton);
  tailCommentPreviousButton->setMaximumWidth(tailCommentPreviousButton->sizeHint().width());
  tailCommentLayoutRight->addWidget(tailCommentFilterField);
  tailCommentLayoutRight->addWidget(tailCommentNextButton);
  tailCommentNextButton->setMaximumWidth(tailCommentNextButton->sizeHint().width());
  tailCommentLayout->addLayout(tailCommentLayoutRight);
  leftLayout->addLayout(tailCommentLayout);
  leftLayout->addWidget(tailCommentField);
  fieldsLayout->addLayout(leftLayout);
  QPointer<QFrame> sepLineLeft = new QFrame();
  sepLineLeft->setFrameShape(QFrame::VLine);
  fieldsLayout->addWidget(sepLineLeft);
  if (width <= 1280) 
    {
      tailDescriptionFilterField->setMaximumWidth(90);
      tailRawFilterField->setMaximumWidth(90);
      tailCommentFilterField->setMaximumWidth(90);
    }
  else 
    {
      tailDescriptionFilterField->setMaximumWidth(200);
      tailRawFilterField->setMaximumWidth(200);
      tailCommentFilterField->setMaximumWidth(200);
    }
  QPointer<QVBoxLayout> middleLayout = new QVBoxLayout;
  middleLayout->setAlignment(Qt::AlignVCenter);
  middleLayout->addWidget(coderLabel);
  coderLabel->setAlignment(Qt::AlignHCenter);
  middleLayout->addWidget(coderFeedbackLabel);
  coderFeedbackLabel->setAlignment(Qt::AlignHCenter);
  middleLayout->addWidget(linkageTypeLabel);
  linkageTypeLabel->setAlignment(Qt::AlignHCenter);
  middleLayout->addWidget(linkageTypeFeedbackLabel);
  linkageTypeFeedbackLabel->setAlignment(Qt::AlignHCenter);
  linkageTypeFeedbackLabel->setStyleSheet("color: red");
  middleLayout->addSpacerItem(new QSpacerItem(200, 40));
  middleLayout->addSpacerItem(new QSpacerItem(200, 40));
  middleLayout->addWidget(linkageQuestionLabel);
  linkageQuestionLabel->setAlignment(Qt::AlignHCenter);
  middleLayout->addWidget(linkageQuestionFeedbackLabel);
  linkageQuestionFeedbackLabel->setMinimumWidth(180);
  middleLayout->setAlignment(linkageQuestionFeedbackLabel, Qt::AlignHCenter | Qt::AlignVCenter);
  middleLayout->addSpacerItem(new QSpacerItem(200, 40));
  middleLayout->addWidget(switchLinkageTypeButton);
  middleLayout->setAlignment(switchLinkageTypeButton, Qt::AlignHCenter | Qt::AlignVCenter);
  middleLayout->addSpacerItem(new QSpacerItem(240, 40));
  middleLayout->addWidget(linkageFeedbackLabel);
  linkageFeedbackLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  linkageFeedbackLabel->setStyleSheet("color: blue");
  linkageFeedbackLabel->setMinimumHeight(40);
  middleLayout->addWidget(setLinkButton);
  setLinkButton->setMaximumWidth(setLinkButton->sizeHint().width());
  middleLayout->setAlignment(setLinkButton, Qt::AlignHCenter | Qt::AlignVCenter);
  middleLayout->addWidget(unsetLinkButton);
  unsetLinkButton->setMaximumWidth(unsetLinkButton->sizeHint().width());
  middleLayout->setAlignment(unsetLinkButton, Qt::AlignHCenter | Qt::AlignVCenter);
  middleLayout->addWidget(markEvidenceButton);
  markEvidenceButton->setMaximumWidth(markEvidenceButton->sizeHint().width());
  middleLayout->setAlignment(markEvidenceButton, Qt::AlignHCenter | Qt::AlignVCenter);
  middleLayout->addWidget(clearEvidenceButton);
  clearEvidenceButton->setMaximumWidth(clearEvidenceButton->sizeHint().width());
  middleLayout->setAlignment(clearEvidenceButton, Qt::AlignHCenter | Qt::AlignVCenter);
  fieldsLayout->addLayout(middleLayout);
  QPointer<QFrame> sepLineRight = new QFrame();
  sepLineRight->setFrameShape(QFrame::VLine);
  fieldsLayout->addWidget(sepLineRight);

  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> headIndexLayout = new QHBoxLayout;
  headIndexLayout->addWidget(headIndexLabel);
  headIndexLayout->addWidget(headMarkedLabel);
  headIndexLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  rightLayout->addLayout(headIndexLayout);
  QPointer<QHBoxLayout> headTimeLayout = new QHBoxLayout;
  headTimeLayout->addWidget(headTimeStampLabel);
  headTimeLayout->addWidget(headTimeStampField);
  headTimeLayout->addWidget(headSourceLabel);
  headTimeLayout->addWidget(headSourceField);
  rightLayout->addLayout(headTimeLayout);
  QPointer<QHBoxLayout> headDescriptionLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> headDescriptionLayoutLeft = new QHBoxLayout;
  headDescriptionLayoutLeft->addWidget(headDescriptionLabel);
  headDescriptionLayout->addLayout(headDescriptionLayoutLeft);
  QPointer<QHBoxLayout> headDescriptionLayoutRight = new QHBoxLayout;
  headDescriptionLayoutRight->addWidget(headDescriptionFilterLabel);
  headDescriptionFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  headDescriptionLayoutRight->addWidget(headDescriptionPreviousButton);
  headDescriptionPreviousButton->setMaximumWidth(headDescriptionPreviousButton->sizeHint().width());
  headDescriptionLayoutRight->addWidget(headDescriptionFilterField);
  headDescriptionLayoutRight->addWidget(headDescriptionNextButton);
  headDescriptionNextButton->setMaximumWidth(headDescriptionNextButton->sizeHint().width());
  headDescriptionLayout->addLayout(headDescriptionLayoutRight);
  rightLayout->addLayout(headDescriptionLayout);
  rightLayout->addWidget(headDescriptionField);
  QPointer<QHBoxLayout> headRawLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> headRawLayoutLeft = new QHBoxLayout;
  headRawLayoutLeft->addWidget(headRawLabel);
  headRawLayout->addLayout(headRawLayoutLeft);
  QPointer<QHBoxLayout> headRawLayoutRight = new QHBoxLayout;
  headRawLayoutRight->addWidget(headRawFilterLabel);
  headRawFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  headRawLayoutRight->addWidget(headRawPreviousButton);
  headRawPreviousButton->setMaximumWidth(headRawPreviousButton->sizeHint().width());
  headRawLayoutRight->addWidget(headRawFilterField);
  headRawLayoutRight->addWidget(headRawNextButton);
  headRawNextButton->setMaximumWidth(headRawNextButton->sizeHint().width());
  headRawLayout->addLayout(headRawLayoutRight);
  rightLayout->addLayout(headRawLayout);
  rightLayout->addWidget(headRawField);
  QPointer<QHBoxLayout> headCommentLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> headCommentLayoutLeft = new QHBoxLayout;
  headCommentLayoutLeft->addWidget(headCommentLabel);
  headCommentLayout->addLayout(headCommentLayoutLeft);
  QPointer<QHBoxLayout> headCommentLayoutRight = new QHBoxLayout;
  headCommentLayoutRight->addWidget(headCommentFilterLabel);
  headCommentFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  headCommentLayoutRight->addWidget(headCommentPreviousButton);
  headCommentPreviousButton->setMaximumWidth(headCommentPreviousButton->sizeHint().width());
  headCommentLayoutRight->addWidget(headCommentFilterField);
  headCommentLayoutRight->addWidget(headCommentNextButton);
  headCommentNextButton->setMaximumWidth(headCommentNextButton->sizeHint().width());
  headCommentLayout->addLayout(headCommentLayoutRight);
  rightLayout->addLayout(headCommentLayout);
  rightLayout->addWidget(headCommentField);
  fieldsLayout->addLayout(rightLayout);
  if (width <= 1280) 
    {
      headDescriptionFilterField->setMaximumWidth(90);
      headRawFilterField->setMaximumWidth(90);
      headCommentFilterField->setMaximumWidth(90);
    }
  else 
    {
      headDescriptionFilterField->setMaximumWidth(200);
      headRawFilterField->setMaximumWidth(200);
      headCommentFilterField->setMaximumWidth(200);
    }  
  mainLayout->addLayout(fieldsLayout);
  QPointer<QFrame> bottomLine = new QFrame;
  bottomLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(bottomLine);
  mainLayout->addWidget(linkageCommentLabel);
  linkageCommentLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  mainLayout->addWidget(linkageCommentField);

  QPointer<QHBoxLayout> buttonsLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> tailButtonsLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> tailTopButtonsLayout = new QHBoxLayout;
  tailTopButtonsLayout->addWidget(previousTailButton);
  tailTopButtonsLayout->addWidget(nextTailButton);
  tailButtonsLayout->addLayout(tailTopButtonsLayout);
  QPointer<QHBoxLayout> tailBottomButtonsLayout = new QHBoxLayout;
  tailBottomButtonsLayout->addWidget(previousMarkedTailButton);
  tailBottomButtonsLayout->addWidget(markTailButton);
  tailBottomButtonsLayout->addWidget(nextMarkedTailButton);
  tailButtonsLayout->addLayout(tailBottomButtonsLayout);
  QPointer<QVBoxLayout> headButtonsLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> headTopButtonsLayout = new QHBoxLayout;
  headTopButtonsLayout->addWidget(previousHeadButton);
  headTopButtonsLayout->addWidget(nextHeadButton);
  headButtonsLayout->addLayout(headTopButtonsLayout);
  QPointer<QHBoxLayout> headBottomButtonsLayout = new QHBoxLayout;
  headBottomButtonsLayout->addWidget(previousMarkedHeadButton);
  headBottomButtonsLayout->addWidget(markHeadButton);
  headBottomButtonsLayout->addWidget(nextMarkedHeadButton);
  headButtonsLayout->addLayout(headBottomButtonsLayout);
  buttonsLayout->addLayout(tailButtonsLayout);
  buttonsLayout->addWidget(jumpButton);
  jumpButton->setMaximumWidth(jumpButton->sizeHint().width());
  buttonsLayout->addLayout(headButtonsLayout);
  mainLayout->addLayout(buttonsLayout);
  
  setLayout(mainLayout);

  retrieveCoders();
  retrieveLinkages();
}

void LinkagesWidget::addCoder() 
{
  SimpleTextDialog *coderDialog = new SimpleTextDialog(this);
  coderDialog->setLabel("Coder:");
  coderDialog->setWindowTitle("Add new coder");
  coderDialog->exec();
  if (coderDialog->getExitStatus() == 0) 
    {
      QString name = coderDialog->getText();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("INSERT INTO coders (name) "
		     "VALUES (:name)");
      query->bindValue(":name", name);
      query->exec();
      delete query;
      coderComboBox->addItem(name);
    }
  delete coderDialog;
}

void LinkagesWidget::editCoder() 
{
  if (coderComboBox->currentText() != DEFAULT) 
    {
      SimpleTextDialog *coderDialog = new SimpleTextDialog(this);
      coderDialog->setLabel("Coder:");
      coderDialog->setWindowTitle("Edit coder:");
      QString oldName = coderComboBox->currentText();
      coderDialog->submitText(oldName);
      coderDialog->exec();
      if (coderDialog->getExitStatus() == 0) 
	{
	  QString name = coderDialog->getText();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE coders "
			 "SET name = :name "
			 "WHERE name = :oldName");
	  query->bindValue(":name", name);
	  query->bindValue(":oldName", oldName);
	  query->exec();
	  delete query;
	  int current = typeComboBox->currentIndex();
	  coderComboBox->setItemText(current, name);
	}
      delete coderDialog;
    }
}

void LinkagesWidget::removeCoder() 
{
  if (coderComboBox->currentText() != DEFAULT) 
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Removing coder"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("This will remove the coder and all linkages "
				     "(s)he created. This action cannot be undone. "
				     "Are you sure you want to remove this coder?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("DELETE FROM coders WHERE name = :name");
	  query->bindValue(":name", coderComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM linkages WHERE coder = :name");
	  query->bindValue(":name", coderComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM linkages_sources WHERE coder = :name");
	  query->bindValue(":name", coderComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM coders_to_linkage_types WHERE coder = :name");
	  query->bindValue(":name", coderComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM linkage_comments WHERE coder = :name");
	  query->bindValue(":name", coderComboBox->currentText());
	  query->exec();
	  delete query;
	  coderComboBox->removeItem(coderComboBox->currentIndex());
	}
      delete warningBox;
    }
}

void LinkagesWidget::addLinkageType() 
{
  LinkageTypeDialog *linkageTypeDialog = new LinkageTypeDialog(this);
  linkageTypeDialog->exec();
  if (linkageTypeDialog->getExitStatus() == 0) 
    {
      QString name = linkageTypeDialog->getName();
      QString description = linkageTypeDialog->getDescription();
      QString question = linkageTypeDialog->getQuestion();
      QString direction = linkageTypeDialog->getDirection();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("INSERT INTO linkage_types (name, description, question, direction) "
		     "VALUES (:name, :description, :question, :direction)");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":question", question);
      query->bindValue(":direction", direction);
      query->exec();
      delete query;
      typeComboBox->addItem(name);
      int index = typeComboBox->count() - 1;
      typeComboBox->setItemData(index, description, Qt::ToolTipRole);
    }
  delete linkageTypeDialog;
}

void LinkagesWidget::editLinkageType() 
{
  if (typeComboBox->currentText() != DEFAULT) 
    {
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2 = new QSqlQuery;
      QSqlQuery *query3 = new QSqlQuery;
      QSqlQuery *query4 = new QSqlQuery;
      QSqlQuery *query5 = new QSqlQuery;
      query2->prepare("UPDATE linkages "
		      "SET tail = :newTail, head = :newHead "
		      "WHERE type = :type "
		      "AND tail = :oldTail AND head = :oldHead");
      query3->prepare("UPDATE linkages_sources "
		      "SET tail = :newTail, head = :newHead, "
		      "istail = :newIsTail "
		      "WHERE type = :type "
		      "AND tail = :oldTail AND head = :oldHead");
      query4->prepare("UPDATE coders_to_linkage_types "
		      "SET tail = :newTail, head = :newHead "
		      "WHERE type =: type "
		      "AND tail = :oldTail AND head = :oldHead");
      query5->prepare("UPDATE linkage_comments "
		      "SET tail = :newTail, head = :newHead "
		      "WHERE type = :type "
		      "AND tail = :oldTail AND head = :oldHead");
      QString oldName = typeComboBox->currentText();
      query->prepare("SELECT description, question, "
		     "direction FROM linkage_types "
		     "WHERE name = :name");
      query->bindValue(":name", oldName);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QString question = query->value(1).toString();
      QString direction = query->value(2).toString();
      LinkageTypeDialog *linkageTypeDialog = new LinkageTypeDialog(this);
      linkageTypeDialog->submitName(oldName);
      linkageTypeDialog->submitDescription(description);
      linkageTypeDialog->submitQuestion(question);
      linkageTypeDialog->submitDirection(direction);
      linkageTypeDialog->exec();
      if (linkageTypeDialog->getExitStatus() == 0) 
	{
	  bool changedDirection = false;
	  QString name = linkageTypeDialog->getName();
	  description = linkageTypeDialog->getDescription();
	  question = linkageTypeDialog->getQuestion();
	  if (direction != linkageTypeDialog->getDirection())
	    {
	      changedDirection = true;
	    }
	  direction = linkageTypeDialog->getDirection();
	  query->prepare("UPDATE linkage_types "
			 "SET name = :name, "
			 "description = :description, "
			 "question = :question, "
			 "direction = :direction WHERE name = :oldName");
	  query->bindValue(":name", name);
	  query->bindValue(":description", description);
	  query->bindValue(":question", question);
	  query->bindValue(":direction", direction);
	  query->bindValue(":oldName", oldName);
	  query->exec();
	  query->prepare("UPDATE linkages "
			 "SET type = :newType "
			 "WHERE type = :oldType");
	  query->bindValue(":newType", name);
	  query->bindValue(":oldType", oldName);
	  query->exec();
	  if (changedDirection)
	    {
	      query->prepare("SELECT tail, head FROM linkages "
			     "WHERE type = :type");
	      query->bindValue(":type", name);
	      query->exec();
	      while (query->next())
		{
		  int oldTail = query->value(0).toInt();
		  int oldHead = query->value(1).toInt();
		  query2->bindValue(":newTail", oldHead);
		  query2->bindValue(":newHead", oldTail);
		  query2->bindValue(":type", name);
		  query2->bindValue(":oldTail", oldTail);
		  query2->bindValue(":oldHead", oldHead);
		  query2->exec();
		}
	    }
	  query->prepare("UPDATE linkages_sources "
			 "SET type = :newType "
			 "WHERE type = :oldType");
	  query->bindValue(":newType", name);
	  query->bindValue(":oldType", oldName);
	  query->exec();
	  if (changedDirection)
	    {
	      query->prepare("SELECT tail, head, istail "
			     "FROM linkages_sources "
			     "WHERE type = :type");
	      query->bindValue(":type", name);
	      query->exec();
	      while (query->next())
		{
		  int oldTail = query->value(0).toInt();
		  int oldHead = query->value(1).toInt();
		  int oldIsTail = query->value(2).toInt();
		  int newIsTail = 0;
		  if (oldIsTail == 0)
		    {
		      newIsTail = 1;
		    }
		  query3->bindValue(":newTail", oldHead);
		  query3->bindValue(":newHead", oldTail);
		  query3->bindValue(":newIsTail", newIsTail);
		  query3->bindValue(":type", name);
		  query3->bindValue(":oldTail", oldTail);
		  query3->bindValue(":oldHead", oldHead);
		  query3->exec();
		}
	    }
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET type = :newType "
			 "WHERE type = :oldType");
	  query->bindValue(":newType", name);
	  query->bindValue(":oldType", oldName);
	  query->exec();
	  if (changedDirection)
	    {
	      query->prepare("SELECT tail, head "
			     "FROM coders_to_linkage_types "
			     "WHERE type = :type");
	      query->bindValue(":type", name);
	      query->exec();
	      while (query->next())
		{
		  int oldTail = query->value(0).toInt();
		  int oldHead = query->value(1).toInt();
		  query4->bindValue(":newTail", oldHead);
		  query4->bindValue(":newHead", oldTail);
		  query4->bindValue(":type", name);
		  query4->bindValue(":oldTail", oldTail);
		  query4->bindValue(":oldHead", oldHead);
		  query4->exec();
		}
	    }
	  query->prepare("UPDATE linkage_comments "
			 "SET type = :newType "
			 "WHERE type = :oldType");
	  query->bindValue(":newType", name);
	  query->bindValue(":oldType", oldName);
	  query->exec();
	  if (changedDirection)
	    {
	      query->prepare("SELECT tail, head "
			     "FROM linkage_comments "
			     "WHERE type = :type");
	      query->bindValue(":type", name);
	      query->exec();
	      while (query->next())
		{
		  int oldTail = query->value(0).toInt();
		  int oldHead = query->value(1).toInt();
		  query5->bindValue(":newTail", oldHead);
		  query5->bindValue(":newHead", oldTail);
		  query5->bindValue(":type", name);
		  query5->bindValue(":oldTail", oldTail);
		  query5->bindValue(":oldHead", oldHead);
		  query5->exec();
		}
	    }
	  delete query;
	  delete query2;
	  delete query3;
	  delete query4;
	  delete query5;
	  int current = typeComboBox->currentIndex();
	  typeComboBox->setItemText(current, name);
	  typeComboBox->setItemData(current, description, 
				    Qt::ToolTipRole);
	  QString label  = "<FONT SIZE = 3>--[" +
	    typeComboBox->currentText() + "]--></FONT>";
	  linkageTypeFeedbackLabel->setText(label);
	  QString toolTip = breakString(description);
	  linkageTypeFeedbackLabel->setToolTip(toolTip);
	  linkageQuestionFeedbackLabel->setText(question);
	  linkageQuestionFeedbackLabel->setMinimumHeight(linkageQuestionFeedbackLabel->
							 sizeHint().height());      
	}
      else 
	{
	  delete query;
	  delete query2;
	  delete query3;
	  delete query4;
	  delete query5;
	}
      delete linkageTypeDialog;
    }
}

void LinkagesWidget::removeLinkageType() 
{
  if (typeComboBox->currentText() != DEFAULT) 
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Removing linkage type"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("This will remove the linkage type and all its linkages. "
				     "This action cannot be undone. Are you sure you want to  "
				     "remove this linkage type?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("DELETE FROM linkage_types WHERE name = :name");
	  query->bindValue(":name", typeComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM linkages WHERE type = :name");
	  query->bindValue(":name", typeComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM linkages_sources WHERE type = :name");
	  query->bindValue(":name", typeComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM coders_to_linkage_types WHERE type = :name");
	  query->bindValue(":name", typeComboBox->currentText());
	  query->exec();
	  query->prepare("DELETE FROM linkage_comments WHERE type = :name");
	  query->bindValue(":name", typeComboBox->currentText());
	  query->exec();

	  delete query;
	  typeComboBox->removeItem(typeComboBox->currentIndex());

	  tailIndexLabel->setText("<b>Tail ( / ) - Incident:</b>");
	  headIndexLabel->setText("<b>Head ( / ) - Incident:</b>");
	  tailTimeStampField->setText("");
	  tailSourceField->setText("");
	  tailDescriptionField->setText("");
	  tailRawField->setText("");
	  tailCommentField->blockSignals(true);
	  tailCommentField->setText("");
	  tailCommentField->blockSignals(false);
	  headTimeStampField->setText("");
	  headSourceField->setText("");
	  headDescriptionField->setText("");
	  headRawField->setText("");
	  headCommentField->blockSignals(true);
	  headCommentField->setText("");
	  headCommentField->blockSignals(false);
	  tailMarkedLabel->setText("");
	  headMarkedLabel->setText("");
	  linkageFeedbackLabel->setText("");
	  setLinkButton->setEnabled(false);
	  unsetLinkButton->setEnabled(false);
	  linkageCommentField->blockSignals(true);
	  linkageCommentField->setText("");
	  linkageCommentField->blockSignals(false);
	  _selectedType = "";
	  _selectedDirection = "";
	  _selectedCoder = "";
	}
      delete warningBox;
    }
}

void LinkagesWidget::retrieveCoders() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM coders");
  while (query->next()) 
    {
      QString name = query->value(0).toString();
      coderComboBox->addItem(name);
    }
  delete query;
}

void LinkagesWidget::retrieveLinkages() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM linkage_types ORDER BY name ASC");
  while (query->next()) 
    {
      QString name = query->value(0).toString();
      QString description = query->value(1).toString();
      typeComboBox->addItem(name);
      int index = typeComboBox->count() - 1;
      typeComboBox->setItemData(index, description, Qt::ToolTipRole);
    }
  delete query;
}

void LinkagesWidget::setTypeButton() 
{
  if (typeComboBox->currentText() != DEFAULT && coderComboBox->currentText() != DEFAULT) 
    {
      selectTypeButton->setEnabled(true);
    }
  else 
    {
      selectTypeButton->setEnabled(false);
    }
  if (typeComboBox->currentText() != DEFAULT) 
    {
      editTypeButton->setEnabled(true);
      removeTypeButton->setEnabled(true);
    }
  else 
    {
      editTypeButton->setEnabled(false);
      removeTypeButton->setEnabled(false);
    }
  if (coderComboBox->currentText() != DEFAULT) 
    {
      editCoderButton->setEnabled(true);
      removeCoderButton->setEnabled(true);
    }
  else 
    {
      editCoderButton->setEnabled(false);
      removeCoderButton->setEnabled(false);
    }
}

void LinkagesWidget::setLinkageType() 
{
  setComments();
  setLinkageComment();
  if (typeComboBox->currentText() != DEFAULT && coderComboBox->currentText() != DEFAULT) 
    {
      _selectedType = typeComboBox->currentText();
      _selectedCoder = coderComboBox->currentText();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT coder, type FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	  query2->bindValue(":type", _selectedType);
	  query2->exec();
	  query2->first();
	  QString direction = query2->value(0).toString();
	  query2->prepare("INSERT INTO coders_to_linkage_types (coder, type, tail, head) "
			  "VALUES (:coder, :type, :tail, :head)");
	  query2->bindValue(":coder", _selectedCoder);
	  query2->bindValue(":type", _selectedType);
	  if (direction == PAST) 
	    {
	      query2->bindValue(":tail", 2);
	      query2->bindValue(":head", 1);
	    }
	  else if (direction == FUTURE) 
	    {
	      query2->bindValue(":tail", 1);
	      query2->bindValue(":head", 2);
	    }
	  query2->exec();
	  delete query2;
	}
      QString coderText = "<i>" + _selectedCoder + "</i>";
      coderFeedbackLabel->setText(coderText);
      query->prepare("SELECT description, question FROM linkage_types WHERE name = :name");
      query->bindValue(":name", _selectedType);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QString question = query->value(1).toString();
      QString label = "<FONT SIZE = 3>--[" + typeComboBox->currentText() + "]--></FONT>";
      linkageTypeFeedbackLabel->setText(label);
      QString toolTip = breakString(description);
      linkageTypeFeedbackLabel->setToolTip(toolTip);
      linkageQuestionFeedbackLabel->setText(question);
      linkageQuestionFeedbackLabel->setMinimumHeight(linkageQuestionFeedbackLabel->
						     sizeHint().height());
      retrieveData();
      delete query;
      setButtons(true);
      checkManualButton();
    }
}

void LinkagesWidget::switchLinkageType()
{
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  QVector<QString> types;
  query->exec("SELECT name, description FROM linkage_types ORDER BY name ASC");
  while (query->next()) 
    {
      QString currentType = query->value(0).toString();
      if (currentType != _selectedType)
	{
	  types.push_back(currentType);
	}
    }
  QPointer<ComboBoxDialog> dialog = new ComboBoxDialog(this, types);
  dialog->setWindowTitle("Switch linkage type");
  dialog->exec();
  if (dialog->getExitStatus() == 0)
    {
      QString newType = dialog->getSelection();
      query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      int tailIndex = query->value(0).toInt();
      int headIndex = query->value(1).toInt();
      query->prepare("SELECT coder, type FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", newType);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	  query2->bindValue(":type", newType);
	  query2->exec();
	  query2->first();
	  QString direction = query2->value(0).toString();
	  query2->prepare("INSERT INTO coders_to_linkage_types (coder, type, tail, head) "
			  "VALUES (:coder, :type, :tail, :head)");
	  query2->bindValue(":coder", _selectedCoder);
	  query2->bindValue(":type", newType);
	  if (direction == PAST) 
	    {
	      if (_selectedDirection == PAST)
		{
		  query2->bindValue(":tail", tailIndex);
		  query2->bindValue(":head", headIndex);
		}
	      else
		{
		  query2->bindValue(":tail", headIndex);
		  query2->bindValue(":head", tailIndex);
		}
	    }
	  else if (direction == FUTURE) 
	    {
	      if (_selectedDirection == FUTURE)
		{
		  query2->bindValue(":tail", tailIndex);
		  query2->bindValue(":head", headIndex);
		}
	      else
		{
		  query2->bindValue(":tail", headIndex);
		  query2->bindValue(":head", tailIndex);
		}
	    }
	  query2->exec();
	  delete query2;
	}
      else
	{
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("SELECT direction FROM linkage_types WHERE name = :type");
	  query2->bindValue(":type", newType);
	  query2->exec();
	  query2->first();
	  QString direction = query2->value(0).toString();
	  query2->prepare("UPDATE coders_to_linkage_types "
			  "SET tail = :tail, head = :head "
			  "WHERE coder = :coder AND type = :type");
	  query2->bindValue(":coder", _selectedCoder);
	  query2->bindValue(":type", newType);
	  if (direction == PAST) 
	    {
	      if (_selectedDirection == PAST)
		{
		  query2->bindValue(":tail", tailIndex);
		  query2->bindValue(":head", headIndex);
		}
	      else
		{
		  query2->bindValue(":tail", headIndex);
		  query2->bindValue(":head", tailIndex);
		}
	    }
	  else if (direction == FUTURE) 
	    {
	      if (_selectedDirection == FUTURE)
		{
		  query2->bindValue(":tail", tailIndex);
		  query2->bindValue(":head", headIndex);
		}
	      else
		{
		  query2->bindValue(":tail", headIndex);
		  query2->bindValue(":head", tailIndex);
		}
	    }
	  query2->exec();
	  delete query2;
	}
      _selectedType =  newType;
      query->prepare("SELECT description, question "
		     "FROM linkage_types WHERE name = :name");
      query->bindValue(":name", _selectedType);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QString question = query->value(1).toString();
      QString label = "<FONT SIZE = 3>--[" + _selectedType + "]--></FONT>";
      linkageTypeFeedbackLabel->setText(label);
      QString toolTip = breakString(description);
      linkageTypeFeedbackLabel->setToolTip(toolTip);
      linkageQuestionFeedbackLabel->setText(question);
      linkageQuestionFeedbackLabel->setMinimumHeight(linkageQuestionFeedbackLabel->
						     sizeHint().height());
      setButtons(true);
      retrieveData();
    }
  delete dialog;
  delete query;
}

void LinkagesWidget::retrieveData() 
{
  _markedTailEvidence.clear();
  _markedHeadEvidence.clear();
  markEvidenceButton->setEnabled(false);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;  
  if (!(query->isNull(0))) 
    {
      tailIndex = query->value(0).toInt();
      headIndex = query->value(1).toInt();
    }
  else 
    {
      delete query;
      return;
    }
  query->prepare("SELECT direction FROM linkage_types WHERE name = :name");
  query->bindValue(":name", _selectedType);
  query->exec();
  query->first();
  _selectedDirection = query->value(0).toString();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  int total = incidentsModel->rowCount();
  QString tailIndexText = "";
  QString headIndexText = "";
  if (_selectedDirection == PAST) 
    {
      tailIndexText = "<b>Tail (" + QString::number(tailIndex - 1) +
	" / " + QString::number(total - 1) + ") - Incident: " + QString::number(tailIndex) + "<b>";
      int headShown = tailIndex - headIndex;
      int totalShown = tailIndex - 1;
      headIndexText = "<b>Head (" + QString::number(headShown) +
	" / " + QString::number(totalShown) + ") - Incident: " + QString::number(headIndex) + "<b>";
    }
  else if (_selectedDirection == FUTURE) 
    {
      tailIndexText = "<b>Tail (" + QString::number(tailIndex) +
	" / " + QString::number(total - 1) + ") - Incident: " + QString::number(tailIndex) + "<b>";
      int headShown = headIndex - tailIndex;
      int totalShown = total - tailIndex;
      headIndexText = "<b>Head (" + QString::number(headShown) +
	" / " + QString::number(totalShown) + ") - Incident: "+ QString::number(headIndex) + "<b>";
    }
  tailIndexLabel->setText(tailIndexText);
  headIndexLabel->setText(headIndexText);
  query->prepare("SELECT id, timestamp, source, description, raw, comment, mark "
		 "FROM incidents WHERE ch_order = :tail");
  query->bindValue(":tail", tailIndex);
  query->exec();
  query->first();
  int tailId = query->value(0).toInt();
  QString tailTimeStamp = query->value(1).toString();
  QString tailSource = query->value(2).toString();
  QString tailDescription = query->value(3).toString();
  QString tailRaw = query->value(4).toString();
  QString tailComment = query->value(5).toString();
  int tailMark = query->value(6).toInt();
  tailTimeStampField->setText(tailTimeStamp);
  tailSourceField->setText(tailSource);
  tailDescriptionField->setText(tailDescription);
  tailRawField->setText(tailRaw);
  tailCommentField->blockSignals(true);
  tailCommentField->setText(tailComment);
  tailCommentField->blockSignals(false);
  if (tailMark == 0) 
    {
      tailMarkedLabel->setText("");
    }
  else 
    {
      tailMarkedLabel->setText("MARKED");
    }
  query->prepare("SELECT id, timestamp, source, description, raw, comment, mark "
		 "FROM incidents WHERE ch_order = :head");
  query->bindValue(":head", headIndex);
  query->exec();
  query->first();
  int headId = query->value(0).toInt();
  QString headTimeStamp = query->value(1).toString();
  QString headSource = query->value(2).toString();
  QString headDescription = query->value(3).toString();
  QString headRaw = query->value(4).toString();
  QString headComment = query->value(5).toString();
  int headMark = query->value(6).toInt();
  headTimeStampField->setText(headTimeStamp);
  headSourceField->setText(headSource);
  headDescriptionField->setText(headDescription);
  headRawField->setText(headRaw);
  headCommentField->blockSignals(true);
  headCommentField->setText(headComment);
  headCommentField->blockSignals(false);
  if (headMark == 0) 
    {
      headMarkedLabel->setText("");
    }
  else 
    {
      headMarkedLabel->setText("MARKED");
    }
  query->prepare("SELECT tail, head "
		 "FROM linkages "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  query->first();
  if (!(query->isNull(0))) 
    {
      linkageFeedbackLabel->setText("LINKED");
      if (_codingType == MANUAL) 
	{
	  unsetLinkButton->setEnabled(true);
	  setLinkButton->setEnabled(false);
	}
    }
  else 
    {
      linkageFeedbackLabel->setText("");
      if (_codingType == MANUAL) 
	{
	  unsetLinkButton->setEnabled(false);
	  setLinkButton->setEnabled(true);
	}
    }
  query->prepare("SELECT comment FROM linkage_comments "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailIndex);
  query->bindValue(":head", headIndex);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  query->first();
  if (!(query->isNull(0))) 
    {
      QString comment = query->value(0).toString();
      linkageCommentField->blockSignals(true);
      linkageCommentField->setText(comment);
      linkageCommentField->blockSignals(false);
    }
  query->prepare("SELECT istail, source_text FROM linkages_sources "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  while (query->next())
    {
      if (query->value(0).toInt() == 1)
	{
	  _markedTailEvidence.push_back(query->value(1).toString());
	}
      else
	{
	  _markedHeadEvidence.push_back(query->value(1).toString());
	}
    }
  highlightText();
  delete query;
}

void LinkagesWidget::editLeftIncident()
{
  QPointer<RecordDialog> dialog = new RecordDialog(this);
  QString timeStamp = tailTimeStampField->text();
  QString source  = tailSourceField->text();
  QString description = tailDescriptionField->toPlainText();
  QString raw = tailRawField->toPlainText();
  QString comment = tailCommentField->toPlainText();
  dialog->setTimeStamp(timeStamp);
  dialog->setSource(source);
  dialog->setDescription(description);
  dialog->setRaw(raw);
  dialog->setComment(comment);
  dialog->initialize();
  dialog->exec();
  if (dialog->getExitStatus() == 0)
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      int tailIndex = 0;
      if (!(query->isNull(0))) 
	{
	  tailIndex = query->value(0).toInt();
	}
      query->prepare("UPDATE incidents "
		     "SET timestamp = :timestamp, description = :description, "
		     "raw = :raw, comment = :comment, source = :source "
		     "WHERE ch_order = :incident");
      query->bindValue(":timestamp", dialog->getTimeStamp());
      query->bindValue(":description", dialog->getDescription());
      query->bindValue(":raw", dialog->getRaw());
      query->bindValue(":comment", dialog->getComment());
      query->bindValue(":source", dialog->getSource());
      query->bindValue(":incident", tailIndex);
      query->exec();
      delete query;
      retrieveData();
    }
}

void LinkagesWidget::editRightIncident()
{
  QPointer<RecordDialog> dialog = new RecordDialog(this);
  QString timeStamp = headTimeStampField->text();
  QString source  = headSourceField->text();
  QString description = headDescriptionField->toPlainText();
  QString raw = headRawField->toPlainText();
  QString comment = headCommentField->toPlainText();
  dialog->setTimeStamp(timeStamp);
  dialog->setSource(source);
  dialog->setDescription(description);
  dialog->setRaw(raw);
  dialog->setComment(comment);
  dialog->initialize();
  dialog->exec();
  if (dialog->getExitStatus() == 0)
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      int headIndex = 0;
      if (!(query->isNull(0))) 
	{
	  headIndex = query->value(0).toInt();
	}
      query->prepare("UPDATE incidents "
		     "SET timestamp = :timestamp, description = :description, "
		     "raw = :raw, comment = :comment, source = :source "
		     "WHERE ch_order = :incident");
      query->bindValue(":timestamp", dialog->getTimeStamp());
      query->bindValue(":description", dialog->getDescription());
      query->bindValue(":raw", dialog->getRaw());
      query->bindValue(":comment", dialog->getComment());
      query->bindValue(":source", dialog->getSource());
      query->bindValue(":incident", headIndex);
      query->exec();
      delete query;
      retrieveData();
    }
}

void LinkagesWidget::checkManualButton() 
{
  _codingType = MANUAL;
  manualCodingButton->setChecked(true);
  assistedCodingButton->setChecked(false);
  switchLinkageTypeButton->setEnabled(true);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :tail");
  query->bindValue(":tail", tailIndex);
  query->exec();
  query->first();
  int tailId = 0;
  tailId = query->value(0).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :head");
  query->bindValue(":head", headIndex);
  query->exec();
  query->first();
  int headId = 0;
  headId = query->value(0).toInt();
  query->prepare("SELECT tail, head "
		 "FROM linkages "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  query->first();
  if (!(query->isNull(0))) 
    {
      setLinkButton->setEnabled(false);
      unsetLinkButton->setEnabled(true);
    }
  else 
    {
      setLinkButton->setEnabled(true);
      unsetLinkButton->setEnabled(false);
    }
  delete query;
}

void LinkagesWidget::checkAssistedButton() 
{
  _codingType = ASSISTED;
  switchLinkageTypeButton->setEnabled(false);
  assistedCodingButton->setChecked(true);
  manualCodingButton->setChecked(false);
  unsetLinkButton->setEnabled(true);
  setLinkButton->setEnabled(true);
}


void LinkagesWidget::highlightSearch(QTextEdit *field, QString text)
{
  int barPos = field->verticalScrollBar()->value(); 
  QTextCursor currentPos = field->textCursor();
  field->selectAll();
  QTextCharFormat originalFormat = field->textCursor().charFormat();
  originalFormat.setBackground(Qt::transparent);
  field->textCursor().mergeCharFormat(originalFormat);      
  QTextCharFormat tempFormat;
  tempFormat.setBackground(Qt::yellow);
  QTextCursor cursor = field->textCursor();
  cursor.movePosition(QTextCursor::Start);
  field->setTextCursor(cursor);
  while (field->find(text))
    {
      field->textCursor().mergeCharFormat(tempFormat);      
    }
  field->setTextCursor(currentPos);
  field->verticalScrollBar()->setValue(barPos); 
}

void LinkagesWidget::setTailDescriptionFilter(const QString &text) 
{
  _tailDescriptionFilter = text;
  highlightSearch(tailDescriptionField, text);
}

void LinkagesWidget::setTailRawFilter(const QString &text) 
{
  _tailRawFilter = text;
  highlightSearch(tailRawField, text);
}

void LinkagesWidget::setTailCommentFilter(const QString &text) 
{
  _tailCommentFilter = text;
  highlightSearch(tailCommentField, text);
}

void LinkagesWidget::setHeadDescriptionFilter(const QString &text) 
{
  _headDescriptionFilter = text;
  highlightSearch(headDescriptionField, text);
}

void LinkagesWidget::setHeadRawFilter(const QString &text) 
{
  _headRawFilter = text;
  highlightSearch(headRawField, text);
}

void LinkagesWidget::setHeadCommentFilter(const QString &text) 
{
  _headCommentFilter = text;
  highlightSearch(headCommentField, text);
}

void LinkagesWidget::previousTailDescription() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_tailDescriptionFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int tail = 0;
      query->first();
      tail = query->value(0).toInt();
      QString searchText = "%" + _tailDescriptionFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order < :tail "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":tail", tail);
      query->exec();
      query->first();
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (_selectedDirection == PAST) 
	    {
	      if (tail >= 2) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailDescriptionField, _tailDescriptionFilter);
		}
	    }
	  else if (_selectedDirection ==  FUTURE) 
	    {
	      if (tail >= 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailDescriptionField, _tailDescriptionFilter);
		}
	    }
	}
      delete query;
    }  
}

void LinkagesWidget::nextTailDescription() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_tailDescriptionFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int tail = 0;
      query->first();
      tail = query->value(0).toInt();
      QString searchText = "%" + _tailDescriptionFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order > :tail "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":tail", tail);
      query->exec();
      query->first();
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (_selectedDirection == PAST) 
	    {
	      if (tail <= incidentsModel->rowCount()) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailDescriptionField, _tailDescriptionFilter);
		}
	    }
	  else if (_selectedDirection == FUTURE) 
	    {
	      if (tail <= incidentsModel->rowCount() - 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailDescriptionField, _tailDescriptionFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::previousTailRaw() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_tailRawFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int tail = 0;
      query->first();
      tail = query->value(0).toInt();
      QString searchText = "%" + _tailRawFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order < :tail "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":tail", tail);
      query->exec();
      query->first();
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (_selectedDirection == PAST) 
	    {
	      if (tail >= 2) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailRawField, _tailRawFilter);
		}
	    }
	  else if (_selectedDirection ==  FUTURE) 
	    {
	      if (tail >= 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailRawField, _tailRawFilter);
		}
	    }
	}
      delete query;
    }
}
  
void LinkagesWidget::nextTailRaw() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
    if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_tailRawFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int tail = 0;
      query->first();
      tail = query->value(0).toInt();
      QString searchText = "%" + _tailRawFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order > :tail "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":tail", tail);
      query->exec();
      query->first();
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (_selectedDirection == PAST) 
	    {
	      if (tail <= incidentsModel->rowCount()) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailRawField, _tailRawFilter);
		}
	    }
	  else if (_selectedDirection == FUTURE) 
	    {
	      if (tail <= incidentsModel->rowCount() - 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailRawField, _tailRawFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::previousTailComment() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_tailCommentFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int tail = 0;
      query->first();
      tail = query->value(0).toInt();
      QString searchText = "%" + _tailCommentFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order < :tail "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":tail", tail);
      query->exec();
      query->first();
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (_selectedDirection == PAST) 
	    {
	      if (tail >= 2) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailCommentField, _tailCommentFilter);
		}
	    }
	  else if (_selectedDirection ==  FUTURE) 
	    {
	      if (tail >= 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailCommentField, _tailCommentFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::nextTailComment() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_tailCommentFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int tail = 0;
      query->first();
      tail = query->value(0).toInt();
      QString searchText = "%" + _tailCommentFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order > :tail "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":tail", tail);
      query->exec();
      query->first();
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (_selectedDirection == PAST) 
	    {
	      if (tail <= incidentsModel->rowCount()) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailCommentField, _tailCommentFilter);
		}
	    }
	  else if (_selectedDirection == FUTURE) 
	    {
	      if (tail <= incidentsModel->rowCount() - 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(tailCommentField, _tailCommentFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::previousHeadDescription() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_headDescriptionFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int head = 0;
      int tail = 0;
      query->first();
      head = query->value(0).toInt();
      tail = query->value(1).toInt();
      QString searchText = "%" + _headDescriptionFilter + "%";
      if (_selectedDirection == PAST) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE description LIKE :text "
			 "AND ch_order > :head "
			 "ORDER BY ch_order asc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head <= tail - 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headDescriptionField, _headDescriptionFilter);
		}
	    }
	}
      else if (_selectedDirection ==  FUTURE) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE description LIKE :text "
			 "AND ch_order < :head "
			 "ORDER BY ch_order desc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head >= tail + 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headDescriptionField, _headDescriptionFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::nextHeadDescription() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_headDescriptionFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int head = 0;
      query->first();
      head = query->value(0).toInt();
      QString searchText = "%" + _headDescriptionFilter + "%";
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (_selectedDirection == PAST) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE description LIKE :text "
			 "AND ch_order < :head "
			 "ORDER BY ch_order desc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head >=  1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headDescriptionField, _headDescriptionFilter);
		}
	    }
	}
      else if (_selectedDirection ==  FUTURE) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE description LIKE :text "
			 "AND ch_order > :head "
			 "ORDER BY ch_order asc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head <= incidentsModel->rowCount()) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headDescriptionField, _headDescriptionFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::previousHeadRaw() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_headRawFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int head = 0;
      int tail = 0;
      query->first();
      head = query->value(0).toInt();
      tail = query->value(1).toInt();
      QString searchText = "%" + _headRawFilter + "%";
      if (_selectedDirection == PAST) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE raw LIKE :text "
			 "AND ch_order > :head "
			 "ORDER BY ch_order asc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head <= tail - 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headRawField, _headRawFilter);
		}
	    }
	}
      else if (_selectedDirection ==  FUTURE) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE raw LIKE :text "
			 "AND ch_order < :head "
			 "ORDER BY ch_order desc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head >= tail + 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headRawField, _headRawFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::nextHeadRaw() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_headRawFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int head = 0;
      query->first();
      head = query->value(0).toInt();
      QString searchText = "%" + _headRawFilter + "%";
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (_selectedDirection == PAST) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE raw LIKE :text "
			 "AND ch_order < :head "
			 "ORDER BY ch_order desc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head >=  1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headRawField, _headRawFilter);
		}
	    }
	}
      else if (_selectedDirection ==  FUTURE) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE raw LIKE :text "
			 "AND ch_order > :head "
			 "ORDER BY ch_order asc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head <= incidentsModel->rowCount()) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headRawField, _headRawFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::previousHeadComment() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_headCommentFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int head = 0;
      int tail = 0;
      query->first();
      head = query->value(0).toInt();
      tail = query->value(1).toInt();
      QString searchText = "%" + _headCommentFilter + "%";
      if (_selectedDirection == PAST) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE comment LIKE :text "
			 "AND ch_order > :head "
			 "ORDER BY ch_order asc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head <= tail - 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headCommentField, _headCommentFilter);
		}
	    }
	}
      else if (_selectedDirection ==  FUTURE) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE comment LIKE :text "
			 "AND ch_order < :head "
			 "ORDER BY ch_order desc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head >= tail + 1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headCommentField, _headCommentFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::nextHeadComment() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  if (_headCommentFilter != "") 
    {
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      int head = 0;
      query->first();
      head = query->value(0).toInt();
      QString searchText = "%" + _headCommentFilter + "%";
      while(incidentsModel->canFetchMore())
	incidentsModel->fetchMore();
      if (_selectedDirection == PAST) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE comment LIKE :text "
			 "AND ch_order < :head "
			 "ORDER BY ch_order desc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head >=  1) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headCommentField, _headCommentFilter);
		}
	    }
	}
      else if (_selectedDirection ==  FUTURE) 
	{
	  query->prepare("SELECT ch_order FROM incidents "
			 "WHERE comment LIKE :text "
			 "AND ch_order > :head "
			 "ORDER BY ch_order asc");
	  query->bindValue(":text", searchText);
	  query->bindValue(":head", head);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      head = query->value(0).toInt();
	      if (head <= incidentsModel->rowCount()) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  highlightSearch(headCommentField, _headCommentFilter);
		}
	    }
	}
      delete query;
    }
}

void LinkagesWidget::previousTail() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  if (_selectedDirection == PAST) 
    {
      if (tailIndex != 2) 
	{
	  tailIndex--;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder =:coder and type = :type ");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":head", tailIndex - 1);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", _selectedType);
	  query->exec();
	  retrieveData();
	}
    }
  else if (_selectedDirection == FUTURE) 
    {
      if (tailIndex != 1) 
	{
	  tailIndex--;
	  if (_codingType == ASSISTED && tailIndex < headIndex) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	  else 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", tailIndex + 1);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	}
    }
  delete query;
}

void LinkagesWidget::nextTail() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  QSqlQuery  *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  if (_selectedDirection == PAST) 
    {
      if (tailIndex != incidentsModel->rowCount()) 
	{
	  tailIndex++;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":head", tailIndex - 1);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", _selectedType);
	  query->exec();
	  retrieveData();
	}
    }
  else if (_selectedDirection == FUTURE) 
    {
      if (tailIndex != incidentsModel->rowCount() - 1) 
	{
	  tailIndex++;
	  if (_codingType == ASSISTED && tailIndex < headIndex) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	  else 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", tailIndex + 1);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	}
    }
  delete query;  
}

void LinkagesWidget::markTail() 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tail = 0;
  tail = query->value(0).toInt();
  query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
  query->bindValue(":order", tail);
  query->exec();
  query->first();
  int currentMark = 0;
  currentMark = query->value(0).toInt();
  if (currentMark == 0) 
    {
      int newMark = 1;
      query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
      query->bindValue(":newMark", newMark);
      query->bindValue(":order", tail);
      query->exec();
      tailMarkedLabel->setText("MARKED");
    }
  else if (currentMark == 1) 
    {
      int newMark = 0;
      query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
      query->bindValue(":newMark", newMark);
      query->bindValue(":order", tail);
      query->exec();
      tailMarkedLabel->setText("");
    }
  delete query;
}

void LinkagesWidget::previousMarkedTail() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  int tail = 0;
  int head = 0;
  query->first();
  tail = query->value(0).toInt();
  head = query->value(1).toInt();
  query->prepare("SELECT ch_order FROM incidents "
		 "WHERE ch_order < :order AND mark = 1 ORDER BY ch_order desc");
  query->bindValue(":order", tail);
  query->exec();
  query->first();
  if (_selectedDirection == PAST) 
    {
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (tail >= 2) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tail);
	      query->bindValue(":head", tail - 1);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);	
	      query->exec();
	      retrieveData();
	    }
	}
    }
  else if (_selectedDirection == FUTURE) 
    {
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (tail >= 1) 
	    {
	      if (_codingType == ASSISTED && tail < head) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);	
		  query->exec();
		  retrieveData();
		}
	      else 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail, head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":head", tail + 1);	
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);	
		  query->exec();
		  retrieveData();
		}
	    }
	}
    }
  delete query;
}

void LinkagesWidget::nextMarkedTail() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  int tail = 0;
  int head = 0;
  query->first();
  tail = query->value(0).toInt();
  head = query->value(1).toInt();
  query->prepare("SELECT ch_order FROM incidents "
		 "WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
  query->bindValue(":order", tail);
  query->exec();
  query->first();
  incidentsModel->select();
  if (incidentsModel->canFetchMore()) 
    {
      incidentsModel->fetchMore();
    }
  if (_selectedDirection == PAST) 
    {
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (tail <= incidentsModel->rowCount()) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tail);
	      query->bindValue(":head", tail - 1);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);	
	      query->exec();
	      retrieveData();
	    }
	}
    }
  else if (_selectedDirection == FUTURE) 
    {
      if (!query->isNull(0)) 
	{
	  tail = query->value(0).toInt();
	  if (tail <= incidentsModel->rowCount() - 1) 
	    {
	      if (_codingType == ASSISTED && tail < head) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);	
		  query->exec();
		  retrieveData();
		}
	      else 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail, head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tail);
		  query->bindValue(":head", tail + 1);	
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);	
		  query->exec();
		  retrieveData();
		}
	    }
	}
    }
  delete query;
}

void LinkagesWidget::jumpTo() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  LinkagesIndexDialog *indexDialog = new LinkagesIndexDialog(this, incidentsModel->rowCount(), _selectedDirection);
  indexDialog->exec();
  int tailIndex = 0;
  int headIndex = 0;
  if (indexDialog->getExitStatus() == 0) 
    {
      if (_selectedDirection == PAST) 
	{
	  tailIndex = indexDialog->getTailIndex() + 1;
	  headIndex = (indexDialog->getHeadIndex() - tailIndex) * -1;
	}
      else if (_selectedDirection == FUTURE) 
	{
	  tailIndex = indexDialog->getTailIndex();
	  headIndex = indexDialog->getHeadIndex() + tailIndex;
	}
      QSqlQuery *query = new QSqlQuery;
      query->prepare("UPDATE coders_to_linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", headIndex);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      retrieveData();
      delete query;
    }
  delete indexDialog;
}

void LinkagesWidget::previousHead() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int headIndex = 0;
  int tailIndex = 0;
  headIndex = query->value(0).toInt();
  tailIndex = query->value(1).toInt();
  if (_selectedDirection == PAST) 
    {
      if (headIndex != tailIndex - 1) 
	{
	  headIndex++;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", headIndex);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", _selectedType);
	  query->exec();
	  retrieveData();
	} 
    }
  else if (_selectedDirection == FUTURE) 
    {
      if (headIndex != tailIndex + 1) 
	{
	  headIndex--;
	  if (_codingType == ASSISTED) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", headIndex - 1);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	  else 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	}
      else 
	{
	  if (tailIndex != 1 && _codingType == ASSISTED) 
	    {
	      headIndex--;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", headIndex - 1);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	}
    }
  delete query;
}

void LinkagesWidget::nextHead() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int headIndex = 0;
  headIndex = query->value(0).toInt();
  if (_selectedDirection == PAST) 
    {
      if (headIndex != 1) 
	{
	  headIndex--;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", headIndex);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", _selectedType);
	  query->exec();
	  retrieveData();
	} 
    }
  else if (_selectedDirection == FUTURE) 
    {
      if (headIndex != incidentsModel->rowCount()) 
	{
	  headIndex++;
	  if (_codingType == ASSISTED) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", headIndex - 1);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	  else 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	    }
	}
    }
  delete query;
}

void LinkagesWidget::markHead() 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->first();
  int head = 0;
  head = query->value(0).toInt();
  query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
  query->bindValue(":order", head);
  query->exec();
  query->first();
  int currentMark = 0;
  currentMark = query->value(0).toInt();
  if (currentMark == 0) 
    {
      int newMark = 1;
      query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
      query->bindValue(":order", head);
      query->bindValue(":newMark", newMark);
      query->exec();
      headMarkedLabel->setText("MARKED");
    }
  else if (currentMark == 1) 
    {
      int newMark = 0;
      query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
      query->bindValue(":order", head);
      query->bindValue(":newMark", newMark);
      query->exec();
      headMarkedLabel->setText("");
    }
  delete query;
}

void LinkagesWidget::previousMarkedHead() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  int head = 0;
  int tail = 0;
  query->first();
  head = query->value(0).toInt();
  tail = query->value(1).toInt();
  if (_selectedDirection == PAST) 
    {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
      query->bindValue(":order", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  head = query->value(0).toInt();
	  if (head <= tail - 1) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":head", head);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);	
	      query->exec();
	      retrieveData();
	    }
	}
    }
  else if (_selectedDirection == FUTURE) 
    {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE ch_order < :order AND mark = 1 ORDER BY ch_order desc");
      query->bindValue(":order", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  head = query->value(0).toInt();
	  if (head >= tail + 1) 
	    {
	      if (_codingType == ASSISTED) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail, head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", head - 1);
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		}
	      else 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);	
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);	
		  query->exec();
		  retrieveData();
		}
	    }
	}
    }
  delete query;
}

void LinkagesWidget::nextMarkedHead() 
{
  if (linkageFeedbackLabel->text() == "" &&
      (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty()))
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Jumping to incident"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but a linkage was not set. "
				     "Evidence is not stored for unset linkages.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) 
    {
      incidentsModel->fetchMore();
    }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  int head = 0;
  query->first();
  head = query->value(0).toInt();
  if (_selectedDirection == PAST) 
    {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE ch_order < :order AND mark = 1 ORDER BY ch_order desc");
      query->bindValue(":order", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  head = query->value(0).toInt();
	  if (head >= 1) 
	    {
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":head", head);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);	
	      query->exec();
	      retrieveData();
	    }
	}
    }
  else if (_selectedDirection == FUTURE) 
    {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
      query->bindValue(":order", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  head = query->value(0).toInt();
	  if (head <= incidentsModel->rowCount()) 
	    {
	      if (_codingType == ASSISTED) 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail, head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", head - 1);
		  query->bindValue(":head", head);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		}
	      else 
		{
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET head = :head "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":head", head);	
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);	
		  query->exec();
		  retrieveData();
		}
	    }
	}
    }
  delete query;
}

void LinkagesWidget::setCommentBool() 
{
  _commentBool = true;
}

void LinkagesWidget::setComments() 
{
  if (_commentBool) 
    {
      QString tailComment = tailCommentField->toPlainText();
      QString headComment = headCommentField->toPlainText();
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      int tailIndex = 0;
      tailIndex = query->value(0).toInt();
      query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :tail");
      query->bindValue(":comment", tailComment);
      query->bindValue(":tail", tailIndex);
      query->exec();
      query->prepare("SELECT head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      int headIndex = 0;
      headIndex = query->value(0).toInt();
      query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :head");
      query->bindValue(":comment", headComment);
      query->bindValue(":head", headIndex);
      query->exec();
      _commentBool = false;
      delete query;
    }
}

void LinkagesWidget::setLinkageCommentBool() 
{
  _linkageCommentBool = true;
}

void LinkagesWidget::setLinkageComment() 
{
  if (_linkageCommentBool) 
    {
      QString comment = linkageCommentField->toPlainText();
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      int tailIndex = query->value(0).toInt();
      int headIndex = query->value(1).toInt();
      query->prepare("SELECT tail FROM linkage_comments "
		     "WHERE tail = :tail AND head = :head AND type = :type");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", headIndex);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      if (!(query->isNull(0))) 
	{
	  if (comment != "") 
	    {
	      query->prepare("UPDATE linkage_comments "
			     "SET comment = :comment, coder = :coder "
			     "WHERE tail = :tail AND head = :head AND type = :type");
	      query->bindValue(":comment", comment);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	    }
	  else 
	    {
	      query->prepare("DELETE FROM linkage_comments "
			     "WHERE tail = :tail and head = :head AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	    }
	}
      else 
	{
	  if (comment != "")
	    {
	      query->prepare("INSERT INTO linkage_comments (tail, head, comment, coder, type)"
			     "VALUES (:tail, :head, :comment, :coder, :type)");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":comment", comment);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	    }
	}
      _linkageCommentBool = false;
      delete query;
    }
}

void LinkagesWidget::pause(int time) 
{
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void LinkagesWidget::setLink() 
{
  setComments();
  setLinkageComment();
  if (_codingType == MANUAL) 
    {
      setLinkButton->setEnabled(false);
      unsetLinkButton->setEnabled(true);
      unsetLinkButton->setFocus();
    }
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) 
    {
      incidentsModel->fetchMore();
    }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :tail");
  query->bindValue(":tail", tailIndex);
  query->exec();
  query->first();
  int tailId = 0;
  tailId = query->value(0).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :head");
  query->bindValue(":head", headIndex);
  query->exec();
  query->first();
  int headId = 0;
  headId = query->value(0).toInt();
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  query->first();
  if (query->isNull(0)) 
    {
      query->prepare("INSERT INTO linkages (tail, head, type, coder) "
		     "VALUES (:tail, :head, :type, :coder)");
      query->bindValue(":tail", tailId);
      query->bindValue(":head", headId);
      query->bindValue(":type", _selectedType);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      storeEvidence(tailId, headId);
      highlightText();
    }
  if (linkageFeedbackLabel->text() != "LINKED") 
    {
      linkageFeedbackLabel->setText("LINKED");
      linkageFeedbackLabel->adjustSize();
      linkageFeedbackLabel->repaint();
    }
  qApp->processEvents();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (_codingType == ASSISTED && _selectedDirection == PAST) 
    {
      QSet<int> ignore;
      if (headIndex != 1) 
	{
	  findPastPaths(&ignore, tailIndex);
	  for (int i = headIndex - 1; i != 0; i--) 
	    {
	      bool found = false;
	      if (ignore.contains(i)) 
		{
		  found = true;
		}
	      if (!found) 
		{
		  if (headIndex != 1) 
		    {
		      headIndex = i;
		      query->prepare("UPDATE coders_to_linkage_types "
				     "SET head = :head "
				     "WHERE coder = :coder AND type = :type");
		      query->bindValue(":head", headIndex);
		      query->bindValue(":coder", _selectedCoder);
		      query->bindValue(":type", _selectedType);
		      query->exec();
		      pause(500);
		      retrieveData();
		      delete query;
		      QApplication::restoreOverrideCursor();
		      qApp->processEvents();
		      return;
		    }
		  else 
		    {
		      if (tailIndex != incidentsModel->rowCount()) 
			{
			  tailIndex++;
			  headIndex = tailIndex - 1;
			  query->prepare("UPDATE coders_to_linkage_types "
					 "SET tail = :tail, head = :head "
					 "WHERE coder = :coder AND type = :type");
			  query->bindValue(":tail", tailIndex);
			  query->bindValue(":head", headIndex);
			  query->bindValue(":coder", _selectedCoder);
			  query->bindValue(":type", _selectedType);
			  query->exec();
			  pause(500);
			  retrieveData();
			  delete query;
			  QApplication::restoreOverrideCursor();
			  qApp->processEvents();
			  return; 
			}
		    }
		}
	      else 
		{
		  if (i == 1) 
		    {
		      if (tailIndex != incidentsModel->rowCount()) 
			{
			  tailIndex++;
			  headIndex = tailIndex - 1;
			  query->prepare("UPDATE coders_to_linkage_types "
					 "SET tail = :tail, head = :head "
					 "WHERE coder = :coder AND type = :type");
			  query->bindValue(":tail", tailIndex);
			  query->bindValue(":head", headIndex);
			  query->bindValue(":coder", _selectedCoder);
			  query->bindValue(":type", _selectedType);
			  query->exec();
			  pause(500);
			  retrieveData();
			  QApplication::restoreOverrideCursor();
			  qApp->processEvents();
			  delete query;
			  return; 
			}
		    }
		}
	    }
	}
      else 
	{
	  if (tailIndex != incidentsModel->rowCount()) 
	    {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      QApplication::restoreOverrideCursor();
	      qApp->processEvents();
	      delete query;
	      return; 
	    }
	}
    }
  else if (_codingType == ASSISTED && _selectedDirection == FUTURE) 
    {
      QSet<int> ignore;
      if (tailIndex != 1) 
	{
	  findFuturePaths(&ignore, tailIndex);
	  for (int i = tailIndex - 1; i != 0; i--) 
	    {
	      bool found = false;
	      if (ignore.contains(i)) 
		{
		  found = true;
		}
	      if (!found) 
		{
		  tailIndex = i;
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tailIndex);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  pause(500);
		  retrieveData();
		  delete query;
		  QApplication::restoreOverrideCursor();
		  qApp->processEvents();
		  return;
		}
	      else 
		{
		  if (i == 1) 
		    {
		      if (headIndex != incidentsModel->rowCount()) 
			{
			  headIndex++;
			  tailIndex = headIndex - 1;
			  query->prepare("UPDATE coders_to_linkage_types "
					 "SET tail = :tail, head = :head "
					 "WHERE coder = :coder AND type = :type");
			  query->bindValue(":tail", tailIndex);
			  query->bindValue(":head", headIndex);
			  query->bindValue(":coder", _selectedCoder);
			  query->bindValue(":type", _selectedType);
			  query->exec();
			  pause(500);
			  retrieveData();
			  delete query;
			  QApplication::restoreOverrideCursor();
			  qApp->processEvents();
			  return;
			}
		    }
		}
	    }
	}
      else 
	{
	  headIndex++;
	  tailIndex = headIndex - 1;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":head", headIndex);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", _selectedType);
	  query->exec();
	  pause(500);
	  retrieveData();
	  delete query;
	  QApplication::restoreOverrideCursor();
	  qApp->processEvents();
	  return;
	}
    }
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
  delete query;
}

void LinkagesWidget::unsetLink() 
{
  if (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty())
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Unsetting linkage"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("You have marked evidence, but are removing a linkage. "
				     "Evidence is not stored for unset linkages. Any evidence "
				     "associated with this linkage will be lost when the linkage "
				     "is removed.");
      if (warningBox->exec() == QMessageBox::No) 
	{
	  return;
	}
    }
  clearEvidence();
  setComments();
  setLinkageComment();
  if (_codingType == MANUAL) 
    {
      setLinkButton->setEnabled(true);
      unsetLinkButton->setEnabled(false);
      setLinkButton->setFocus();
    }
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) 
    {
      incidentsModel->fetchMore();
    }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :tail");
  query->bindValue(":tail", tailIndex);
  query->exec();
  query->first();
  int tailId = 0;
  tailId = query->value(0).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :head");
  query->bindValue(":head", headIndex);
  query->exec();
  query->first();
  int headId = 0;
  headId = query->value(0).toInt();
  query->prepare("SELECT tail, head FROM linkages "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  query->first();
  if (!(query->isNull(0))) 
    {
      query->prepare("DELETE FROM linkages "
		     "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
      query->bindValue(":tail", tailId);
      query->bindValue(":head", headId);
      query->bindValue(":type", _selectedType);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      query->prepare("DELETE FROM linkages_sources "
		     "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
      query->bindValue(":tail", tailId);
      query->bindValue(":head", headId);
      query->bindValue(":type", _selectedType);
      query->bindValue(":coder", _selectedCoder);
    }
  linkageFeedbackLabel->setText("");
  linkageFeedbackLabel->adjustSize();
  linkageFeedbackLabel->repaint();
  highlightText();
  qApp->processEvents();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (_codingType == ASSISTED && _selectedDirection == PAST) 
    {
      QSet<int> ignore;
      if (headIndex != 1) 
	{
	  findPastPaths(&ignore, tailIndex);
	  for (int i = headIndex - 1; i != 0; i--) 
	    {
	      bool found = false;
	      if (ignore.contains(i)) 
		{
		  found = true;
		}
	      if (!found) 
		{
		  if (headIndex != 1) 
		    {
		      headIndex = i;
		      query->prepare("UPDATE coders_to_linkage_types "
				     "SET head = :head "
				     "WHERE coder = :coder AND type = :type");
		      query->bindValue(":head", headIndex);
		      query->bindValue(":coder", _selectedCoder);
		      query->bindValue(":type", _selectedType);
		      query->exec();
		      retrieveData();
		      delete query;
		      QApplication::restoreOverrideCursor();
		      qApp->processEvents();
		      return;
		    }
		  else 
		    {
		      if (tailIndex != incidentsModel->rowCount()) 
			{
			  tailIndex++;
			  headIndex = tailIndex - 1;
			  query->prepare("UPDATE coders_to_linkage_types "
					 "SET tail = :tail, head = :head "
					 "WHERE coder = :coder AND type = :type");
			  query->bindValue(":tail", tailIndex);
			  query->bindValue(":head", headIndex);
			  query->bindValue(":coder", _selectedCoder);
			  query->bindValue(":type", _selectedType);
			  query->exec();
			  retrieveData();
			  QApplication::restoreOverrideCursor();
			  qApp->processEvents();
			  delete query;
			  return; 
			}
		    }
		}
	      else 
		{
		  if (i == 1) 
		    {
		      if (tailIndex != incidentsModel->rowCount()) 
			{
			  tailIndex++;
			  headIndex = tailIndex - 1;
			  query->prepare("UPDATE coders_to_linkage_types "
					 "SET tail = :tail, head = :head "
					 "WHERE coder = :coder AND type = :type");
			  query->bindValue(":tail", tailIndex);
			  query->bindValue(":head", headIndex);
			  query->bindValue(":coder", _selectedCoder);
			  query->bindValue(":type", _selectedType);
			  query->exec();
			  retrieveData();
			  QApplication::restoreOverrideCursor();
			  qApp->processEvents();
			  delete query;
			  return; 
			}
		    }
		}
	    }
	}
      else 
	{
	  if (tailIndex != incidentsModel->rowCount()) 
	    {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type  = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", _selectedCoder);
	      query->bindValue(":type", _selectedType);
	      query->exec();
	      retrieveData();
	      QApplication::restoreOverrideCursor();
	      qApp->processEvents();
	      delete query;
	      return; 
	    }
	}
    }
  else if (_codingType == ASSISTED && _selectedDirection == FUTURE) 
    {
      QSet<int> ignore;
      if (tailIndex != 1) 
	{
	  findFuturePaths(&ignore, headIndex);
	  for (int i = tailIndex - 1; i != 0; i--) 
	    {
	      bool found = false;
	      if (ignore.contains(i)) 
		{
		  found = true;
		}
	      if (!found) 
		{
		  tailIndex = i;
		  query->prepare("UPDATE coders_to_linkage_types "
				 "SET tail = :tail "
				 "WHERE coder = :coder AND type = :type");
		  query->bindValue(":tail", tailIndex);
		  query->bindValue(":coder", _selectedCoder);
		  query->bindValue(":type", _selectedType);
		  query->exec();
		  retrieveData();
		  QApplication::restoreOverrideCursor();
		  qApp->processEvents();
		  delete query;
		  return;
		}
	      else 
		{
		  if (i == 1) 
		    {
		      if (headIndex != incidentsModel->rowCount()) 
			{
			  headIndex++;
			  tailIndex = headIndex - 1;
			  query->prepare("UPDATE coders_to_linkage_types "
					 "SET tail = :tail, head = :head "
					 "WHERE coder = :coder AND type = :type");
			  query->bindValue(":tail", tailIndex);
			  query->bindValue(":head", headIndex);
			  query->bindValue(":coder", _selectedCoder);
			  query->bindValue(":type", _selectedType);
			  query->exec();
			  retrieveData();
			  QApplication::restoreOverrideCursor();
			  qApp->processEvents();
			  delete query;
			  return;
			}
		    }
		}
	    }
	}
      else 
	{
	  headIndex++;
	  tailIndex = headIndex - 1;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":head", headIndex);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":type", _selectedType);
	  query->exec();
	  retrieveData();
	  QApplication::restoreOverrideCursor();
	  qApp->processEvents();
	  delete query;
	  return;
	}
    }
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
  delete query;
}

void LinkagesWidget::markEvidence()
{
  if (_codingType == MANUAL)
    {
      setLink();
    }
  QString tailText = tailRawField->textCursor().selectedText().trimmed();
  QString headText = headRawField->textCursor().selectedText().trimmed();
  if (tailText != "")
    {
      _markedTailEvidence.push_back(tailText);
    }
  if (headText != "")
    {
      _markedHeadEvidence.push_back(headText);
    }
  if (linkageFeedbackLabel->text() == "LINKED")
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":type", _selectedType);
      query->exec();
      query->first();
      int tailIndex = 0;
      int headIndex = 0;
      tailIndex = query->value(0).toInt();
      headIndex = query->value(1).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :tail");
      query->bindValue(":tail", tailIndex);
      query->exec();
      query->first();
      int tailId = 0;
      tailId = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :head");
      query->bindValue(":head", headIndex);
      query->exec();
      query->first();
      int headId = 0;
      headId = query->value(0).toInt(); 
      storeEvidence(tailId, headId);
    }
  QTextCursor tailCursor = tailRawField->textCursor();
  tailCursor.clearSelection();
  tailRawField->setTextCursor(tailCursor);
  QTextCursor headCursor = headRawField->textCursor();
  headCursor.clearSelection();
  headRawField->setTextCursor(headCursor);
  highlightText();
}

void LinkagesWidget::clearEvidence()
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _selectedCoder);
  query->bindValue(":type", _selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :tail");
  query->bindValue(":tail", tailIndex);
  query->exec();
  query->first();
  int tailId = 0;
  tailId = query->value(0).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :head");
  query->bindValue(":head", headIndex);
  query->exec();
  query->first();
  int headId = 0;
  headId = query->value(0).toInt(); 
  query->prepare("DELETE FROM linkages_sources "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  _markedTailEvidence.clear();
  _markedHeadEvidence.clear();
  delete query;
  highlightText();
}

void LinkagesWidget::storeEvidence(const int tail, const int head)
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("DELETE FROM linkages_sources "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tail);
  query->bindValue(":head", head);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  QVectorIterator<QString> it(_markedTailEvidence);
  while (it.hasNext())
    {
      QString currentEvidence = it.next();
      query->prepare("INSERT INTO linkages_sources "
		     "(tail, head, type, coder, istail, source_text) "
		     "VALUES (:tail, :head, :type, :coder, :istail, :source_text)");
      query->bindValue(":tail", tail);
      query->bindValue(":head", head);
      query->bindValue(":type", _selectedType);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":istail", 1);
      query->bindValue(":source_text", currentEvidence);
      query->exec();
    }
  QVectorIterator<QString> it2(_markedHeadEvidence);
  while (it2.hasNext())
    {
      QString currentEvidence = it2.next();
      query->prepare("INSERT INTO linkages_sources "
		     "(tail, head, type, coder, istail, source_text) "
		     "VALUES (:tail, :head, :type, :coder, :istail, :source_text)");
      query->bindValue(":tail", tail);
      query->bindValue(":head", head);
      query->bindValue(":type", _selectedType);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":istail", 0);
      query->bindValue(":source_text", currentEvidence);
      query->exec();
    }
  delete query;
}

void LinkagesWidget::highlightText() 
{
  QTextCursor currentTailPos = tailRawField->textCursor();
  QTextCursor currentHeadPos = headRawField->textCursor();
  int tailBarPos = tailRawField->verticalScrollBar()->value();
  int headBarPos = headRawField->verticalScrollBar()->value();
  QTextCharFormat format;
  format.setFontWeight(QFont::Normal);
  format.setUnderlineStyle(QTextCharFormat::NoUnderline);
  tailRawField->selectAll();
  tailRawField->textCursor().mergeCharFormat(format);
  QTextCursor tailCursor = tailRawField->textCursor();
  tailCursor.movePosition(QTextCursor::Start);
  tailRawField->setTextCursor(tailCursor);
  headRawField->selectAll();
  headRawField->textCursor().mergeCharFormat(format);
  QTextCursor headCursor = headRawField->textCursor();
  headCursor.movePosition(QTextCursor::Start);
  headRawField->setTextCursor(headCursor);
  QVectorIterator<QString> it(_markedTailEvidence);
  while (it.hasNext())
  {
    QString tailText = it.next();
    QVector<QString> tailBlocks = splitLines(tailText);
    QVectorIterator<QString> it2(tailBlocks);
    while (it2.hasNext()) 
      {
	QString currentLine = it2.next();
	while (tailRawField->find(currentLine, QTextDocument::FindWholeWords)) 
	  {
	    format.setFontWeight(QFont::Bold);
	    format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
	    format.setUnderlineColor(Qt::blue);
	    tailRawField->textCursor().mergeCharFormat(format);
	  }
      }
    tailCursor = tailRawField->textCursor();
    tailCursor.movePosition(QTextCursor::Start);
    tailRawField->setTextCursor(tailCursor);
  }
  tailRawField->setTextCursor(currentTailPos);
  QVectorIterator<QString> it3(_markedHeadEvidence);
  while (it3.hasNext())
    {
      QString headText = it3.next();
      QVector<QString> headBlocks = splitLines(headText);
      QVectorIterator<QString> it4(headBlocks);
      while (it4.hasNext()) 
	{
	  QString currentLine = it4.next();
	  while (headRawField->find(currentLine, QTextDocument::FindWholeWords)) 
	    {
	      format.setFontWeight(QFont::Bold);
	      format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
	      format.setUnderlineColor(Qt::blue);
	      headRawField->textCursor().mergeCharFormat(format);
	    }
	}
      headCursor = headRawField->textCursor();
      headCursor.movePosition(QTextCursor::Start);
      headRawField->setTextCursor(headCursor);
    }
  headRawField->setTextCursor(currentHeadPos);
  tailRawField->verticalScrollBar()->setValue(tailBarPos);
  headRawField->verticalScrollBar()->setValue(headBarPos);
  if (_markedTailEvidence.empty() && _markedHeadEvidence.empty())
    {
      clearEvidenceButton->setEnabled(false);
    }
  else
    {
      clearEvidenceButton->setEnabled(true);
    }
}

void LinkagesWidget::selectTailText() 
{
  if (tailRawField->textCursor().selectedText().trimmed() != "") 
    {
      int end = 0;
      int begin = 0;
      QTextCursor selectCursor = tailRawField->textCursor();
      if (tailRawField->textCursor().anchor() >= tailRawField->textCursor().position()) 
	{
	  begin = tailRawField->textCursor().position();
	  end = tailRawField->textCursor().anchor();
	}
      else 
	{
	  begin = tailRawField->textCursor().anchor();
	  end = tailRawField->textCursor().position();
	}
      selectCursor.setPosition(begin);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      tailRawField->setTextCursor(selectCursor);
      while (tailRawField->textCursor().selectedText()[0].isSpace()) 
	{
	  begin++;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  tailRawField->setTextCursor(selectCursor);
	}
      while (tailRawField->textCursor().selectedText()[tailRawField->textCursor().
						       selectedText().length() - 1].isSpace()) 
	{
	  end--;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  tailRawField->setTextCursor(selectCursor);
	}
      selectCursor.setPosition(begin);
      selectCursor.movePosition(QTextCursor::StartOfWord);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      if (!tailRawField->toPlainText()[end].isPunct()) 
	{
	  selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	}
      tailRawField->setTextCursor(selectCursor);
    }
  markEvidenceButton->setEnabled(false);
  if (tailRawField->textCursor().selectedText() != "" ||
      headRawField->textCursor().selectedText() != "")
    {
      markEvidenceButton->setEnabled(true);
    }
}

void LinkagesWidget::selectHeadText() 
{
  if (headRawField->textCursor().selectedText().trimmed() != "") 
    {
      int end = 0;
      int begin = 0;
      QTextCursor selectCursor = headRawField->textCursor();
      if (headRawField->textCursor().anchor() >= headRawField->textCursor().position()) 
	{
	  begin = headRawField->textCursor().position();
	  end = headRawField->textCursor().anchor();
	}
      else 
	{
	  begin = headRawField->textCursor().anchor();
	  end = headRawField->textCursor().position();
	}
      selectCursor.setPosition(begin);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      headRawField->setTextCursor(selectCursor);
      while (headRawField->textCursor().selectedText()[0].isSpace()) 
	{
	  begin++;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  headRawField->setTextCursor(selectCursor);
	}
      while (headRawField->textCursor().selectedText()[headRawField->textCursor().
						       selectedText().length() - 1].isSpace()) 
	{
	  end--;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  headRawField->setTextCursor(selectCursor);
	}
      selectCursor.setPosition(begin);
      selectCursor.movePosition(QTextCursor::StartOfWord);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      if (!headRawField->toPlainText()[end].isPunct()) 
	{
	  selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	}
      headRawField->setTextCursor(selectCursor);
    }
  markEvidenceButton->setEnabled(false);
  if (tailRawField->textCursor().selectedText() != "" ||
      headRawField->textCursor().selectedText() != "")
    {
      markEvidenceButton->setEnabled(true);
    }
}

void LinkagesWidget::findPastPaths(QSet<int> *pIgnore, int currentIncident) 
{
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT id FROM incidents WHERE ch_order = :current");
  query->bindValue(":current", currentIncident);
  query->exec();
  query->first();
  int currentTail = 0;
  currentTail = query->value(0).toInt();
  query->prepare("SELECT head FROM linkages "
		 "WHERE tail = :tail AND type = :type AND coder = :coder");
  query->bindValue(":tail", currentTail);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  QSet<int> results;
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :current");
  while (query->next()) 
    {
      int currentHead = 0;
      currentHead = query->value(0).toInt();
      query2->bindValue(":current", currentHead);
      query2->exec();
      query2->first();
      int newIndex = query2->value(0).toInt();
      if (!pIgnore->contains(newIndex)) 
	{
	  results.insert(newIndex);
	}
    }
  delete query2;
  delete query;
  QSqlDatabase::database().commit();
  QList<int> tempList = results.toList();
  std::sort(tempList.begin(), tempList.end());
  QList<int>::iterator it;
  for (it = tempList.begin(); it != tempList.end(); it++) 
    {
      pIgnore->insert(*it);
      findPastPaths(pIgnore, *it);
    }
}

void LinkagesWidget::findFuturePaths(QSet<int> *pIgnore, int currentIncident) 
{
  QSqlDatabase::database().transaction();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT id FROM incidents WHERE ch_order = :current");
  query->bindValue(":current", currentIncident);
  query->exec();
  query->first();
  int currentHead = 0;
  currentHead = query->value(0).toInt();
  query->prepare("SELECT tail FROM linkages "
		 "WHERE head = :head AND type = :type AND coder = :coder");
  query->bindValue(":head", currentHead);
  query->bindValue(":type", _selectedType);
  query->bindValue(":coder", _selectedCoder);
  query->exec();
  QSet<int> results;
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :current");
  while (query->next()) 
    {
      int currentTail = 0;
      currentTail = query->value(0).toInt();
      query2->bindValue(":current", currentTail);
      query2->exec();
      query2->first();
      int newIndex = query2->value(0).toInt();
      if (!pIgnore->contains(newIndex)) 
	{
	  results.insert(newIndex);
	}
    }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  QList<int> tempList = results.toList();
  std::sort(tempList.begin(), tempList.end());
  QList<int>::iterator it;
  for (it = tempList.begin(); it != tempList.end(); it++) 
    {
      pIgnore->insert(*it);
      findFuturePaths(pIgnore, *it);
    }
}

void LinkagesWidget::setButtons(bool status) 
{
  manualCodingButton->setEnabled(status);
  assistedCodingButton->setEnabled(status);
  previousTailButton->setEnabled(status);
  nextTailButton->setEnabled(status);
  markTailButton->setEnabled(status);
  previousMarkedTailButton->setEnabled(status);
  nextMarkedTailButton->setEnabled(status);
  tailDescriptionPreviousButton->setEnabled(status);
  tailDescriptionNextButton->setEnabled(status); 
  tailRawPreviousButton->setEnabled(status); 
  tailRawNextButton->setEnabled(status); 
  tailCommentPreviousButton->setEnabled(status);
  tailCommentNextButton->setEnabled(status);
  previousHeadButton->setEnabled(status);
  nextHeadButton->setEnabled(status);
  markHeadButton->setEnabled(status);
  previousMarkedHeadButton->setEnabled(status);
  nextMarkedHeadButton->setEnabled(status);
  headDescriptionPreviousButton->setEnabled(status);
  headDescriptionNextButton->setEnabled(status);
  headRawPreviousButton->setEnabled(status);
  headRawNextButton->setEnabled(status);
  headCommentPreviousButton->setEnabled(status);
  headCommentNextButton->setEnabled(status);
  jumpButton->setEnabled(status);
  tailDescriptionFilterField->setEnabled(status);
  tailRawFilterField->setEnabled(status);
  tailCommentFilterField->setEnabled(status);
  headDescriptionFilterField->setEnabled(status);
  headRawFilterField->setEnabled(status);
  headCommentFilterField->setEnabled(status);
  tailTimeStampField->setEnabled(status);
  tailSourceField->setEnabled(status);
  tailDescriptionField->setEnabled(status);  
  tailRawField->setEnabled(status);
  tailCommentField->setEnabled(status);
  headTimeStampField->setEnabled(status);
  headSourceField->setEnabled(status);
  headDescriptionField->setEnabled(status);  
  headRawField->setEnabled(status);
  headCommentField->setEnabled(status);
  linkageCommentField->setEnabled(status);
}

bool LinkagesWidget::eventFilter(QObject *object, QEvent *event) 
{
  if (event->type() == QEvent::Wheel) 
    {
      QWheelEvent *wheelEvent = (QWheelEvent*) event;
      QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
      if (textEdit) 
	{
	  if(wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->angleDelta().y() > 0) 
		{
		  textEdit->zoomIn(1);
		}
	      else if (wheelEvent->angleDelta().y() < 0) 
		{
		  textEdit->zoomOut(1);
		}
	    }
	}
    }
  else if (((object == tailDescriptionField->viewport() &&
	     tailDescriptionField->isEnabled()) ||
	    (object == tailRawField->viewport() &&
	     tailRawField->isEnabled()) ||
	    (object == tailTimeStampField &&
	     tailTimeStampField->isEnabled()) ||
	    (object == tailSourceField &&
	     tailSourceField->isEnabled())) &&
	   event->type() == QEvent::ContextMenu)
    {
      QContextMenuEvent *context = (QContextMenuEvent*) event;
      QMenu *menu = new QMenu;
      QAction *editAction = new QAction(tr("Edit incident"), this);
      connect(editAction, SIGNAL(triggered()), this, SLOT(editLeftIncident()));
      menu->addAction(editAction);
      menu->exec(context->globalPos());
      delete editAction;
      delete menu;
      return true;
    }
  else if (((object == headDescriptionField->viewport() &&
	     headDescriptionField->isEnabled()) ||
	    (object == headRawField->viewport() &&
	     headRawField->isEnabled()) ||
	    (object == headTimeStampField &&
	     headTimeStampField->isEnabled()) ||
	    (object == headSourceField &&
	     headSourceField->isEnabled())) &&
	   event->type() == QEvent::ContextMenu)
    {
      QContextMenuEvent *context = (QContextMenuEvent*) event;
      QMenu *menu = new QMenu;
      QAction *editAction = new QAction(tr("Edit incident"), this);
      connect(editAction, SIGNAL(triggered()), this, SLOT(editRightIncident()));
      menu->addAction(editAction);
      menu->exec(context->globalPos());
      delete editAction;
      delete menu;
      return true;
    }
  else if (object == tailRawField->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
      selectTailText();	
    }
  else if (object == headRawField->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
      selectHeadText();
    }
  return false;
}

void LinkagesWidget::finalBusiness() 
{
  setComments();
  setLinkageComment();
}
