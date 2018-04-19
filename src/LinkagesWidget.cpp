
/*
  Linkage types can be added via a separate dialog (to be created). 
  The user should also be asked to formulate a question corresponding
  to the linkage there.
*/

#include "../include/LinkagesWidget.h"

LinkagesWidget::LinkagesWidget(QWidget *parent) : QWidget(parent) {
  codingType = MANUAL;
  selectedType = "";
  selectedDirection = "";
  selectedCoder == "";
  tailDescriptionFilter = "";
  tailRawFilter = "";
  tailCommentFilter = "";
  headDescriptionFilter = "";
  headRawFilter = "";
  headCommentFilter = "";

  commentBool = false;
  linkageCommentBool = false;
  
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

  linkageCommentField->installEventFilter(this);
  headCommentField->installEventFilter(this);
  tailCommentField->installEventFilter(this);
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
  connect(linkageCommentField, SIGNAL(textChanged()), this, SLOT(setLinkageCommentBool()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));

  // I want to set the size of some widgets based on the availabe screen width
  QRect rect = QApplication::desktop()->screenGeometry();
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
  if (width <= 1280) {
    tailDescriptionFilterField->setMaximumWidth(90);
    tailRawFilterField->setMaximumWidth(90);
    tailCommentFilterField->setMaximumWidth(90);
  } else {
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
  if (width <= 1280) {
    headDescriptionFilterField->setMaximumWidth(90);
    headRawFilterField->setMaximumWidth(90);
    headCommentFilterField->setMaximumWidth(90);
  } else {
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

void LinkagesWidget::addCoder() {
  SimpleTextDialog *coderDialog = new SimpleTextDialog(this);
  coderDialog->setLabel("Coder:");
  coderDialog->setWindowTitle("Add new coder");
  coderDialog->exec();
  if (coderDialog->getExitStatus() == 0) {
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

void LinkagesWidget::editCoder() {
  if (coderComboBox->currentText() != DEFAULT) {
    SimpleTextDialog *coderDialog = new SimpleTextDialog(this);
    coderDialog->setLabel("Coder:");
    coderDialog->setWindowTitle("Edit coder:");
    QString oldName = coderComboBox->currentText();
    coderDialog->submitText(oldName);
    coderDialog->exec();
    if (coderDialog->getExitStatus() == 0) {
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

void LinkagesWidget::removeCoder() {
  if (coderComboBox->currentText() != DEFAULT) {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("This will remove the coder and all linkages (s)he created. This action cannot be undone. Are you sure you want to remove this coder?");
    if (warningBox->exec() == QMessageBox::Yes) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("DELETE FROM coders WHERE name = :name");
      query->bindValue(":name", coderComboBox->currentText());
      query->exec();
      query->prepare("DELETE FROM linkages WHERE coder = :name");
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

void LinkagesWidget::addLinkageType() {
  LinkageTypeDialog *linkageTypeDialog = new LinkageTypeDialog(this);
  linkageTypeDialog->exec();
  if (linkageTypeDialog->getExitStatus() == 0) {
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

void LinkagesWidget::editLinkageType() {
  if (typeComboBox->currentText() != DEFAULT) {
    QSqlQuery *query = new QSqlQuery;
    QString oldName = typeComboBox->currentText();
    query->prepare("SELECT description, question, direction FROM linkage_types "
		   "WHERE name = :name");
    query->bindValue(":name", oldName);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QString question = query->value(1).toString();
    QString direction = query->value(2).toString();
    LinkageTypeDialog *linkageTypeDialog = new LinkageTypeDialog(this);
    linkageTypeDialog->fixDirection();
    linkageTypeDialog->submitName(oldName);
    linkageTypeDialog->submitDescription(description);
    linkageTypeDialog->submitQuestion(question);
    linkageTypeDialog->submitDirection(direction);
    linkageTypeDialog->exec();
    if (linkageTypeDialog->getExitStatus() == 0) {
      QString name = linkageTypeDialog->getName();
      description = linkageTypeDialog->getDescription();
      question = linkageTypeDialog->getQuestion();
      direction = linkageTypeDialog->getDirection();
      query->prepare("UPDATE linkage_types "
		     "SET name = :name, description = :description, question = :question, "
		     "direction = :direction WHERE name = :oldName");
      query->bindValue(":name", name);
      query->bindValue(":description", description);
      query->bindValue(":question", question);
      query->bindValue(":direction", direction);
      query->bindValue(":oldName", oldName);
      query->exec();
      delete query;
      int current = typeComboBox->currentIndex();
      typeComboBox->setItemText(current, name);
      typeComboBox->setItemData(current, description, Qt::ToolTipRole);
      QString label  = "<FONT SIZE = 3>--[" + typeComboBox->currentText() + "]--></FONT>";
      linkageTypeFeedbackLabel->setText(label);
      QString toolTip = breakString(description);
      linkageTypeFeedbackLabel->setToolTip(toolTip);
      linkageQuestionFeedbackLabel->setText(question);
      linkageQuestionFeedbackLabel->setMinimumHeight(linkageQuestionFeedbackLabel->
						     sizeHint().height());
      
    } else {
      delete query;
    }
    delete linkageTypeDialog;
  }
}

void LinkagesWidget::removeLinkageType() {
  if (typeComboBox->currentText() != DEFAULT) {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("This will remove the linkage type and all its linkages. "
				   "This action cannot be undone. Are you sure you want to  "
				   "remove this linkage type?");
    if (warningBox->exec() == QMessageBox::Yes) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("DELETE FROM linkage_types WHERE name = :name");
      query->bindValue(":name", typeComboBox->currentText());
      query->exec();
      query->prepare("DELETE FROM linkages WHERE type = :name");
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
      selectedType = "";
      selectedDirection = "";
      selectedCoder = "";
    }
    delete warningBox;
  }
}

void LinkagesWidget::retrieveCoders() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name FROM coders");
  while (query->next()) {
    QString name = query->value(0).toString();
    coderComboBox->addItem(name);
  }
  delete query;
}

void LinkagesWidget::retrieveLinkages() {
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, description FROM linkage_types");
  while (query->next()) {
    QString name = query->value(0).toString();
    QString description = query->value(1).toString();
    typeComboBox->addItem(name);
    int index = typeComboBox->count() - 1;
    typeComboBox->setItemData(index, description, Qt::ToolTipRole);
  }
  delete query;
}

void LinkagesWidget::setTypeButton() {
  if (typeComboBox->currentText() != DEFAULT && coderComboBox->currentText() != DEFAULT) {
    selectTypeButton->setEnabled(true);
  } else {
    selectTypeButton->setEnabled(false);
  }
  if (typeComboBox->currentText() != DEFAULT) {
    editTypeButton->setEnabled(true);
    removeTypeButton->setEnabled(true);
  } else {
    editTypeButton->setEnabled(false);
    removeTypeButton->setEnabled(false);
  }
  if (coderComboBox->currentText() != DEFAULT) {
    editCoderButton->setEnabled(true);
    removeCoderButton->setEnabled(true);
  } else {
    editCoderButton->setEnabled(false);
    removeCoderButton->setEnabled(false);
  }
}

void LinkagesWidget::setLinkageType() {
  setComments();
  setLinkageComment();
  if (typeComboBox->currentText() != DEFAULT && coderComboBox->currentText() != DEFAULT) {
    selectedType = typeComboBox->currentText();
    selectedCoder = coderComboBox->currentText();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT coder, type FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    query->first();
    if (query->isNull(0)) {
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT direction FROM linkage_types WHERE name = :type");
      query2->bindValue(":type", selectedType);
      query2->exec();
      query2->first();
      QString direction = query2->value(0).toString();
      query2->prepare("INSERT INTO coders_to_linkage_types (coder, type, tail, head) "
		     "VALUES (:coder, :type, :tail, :head)");
      query2->bindValue(":coder", selectedCoder);
      query2->bindValue(":type", selectedType);
      if (direction == PAST) {
	query2->bindValue(":tail", 2);
	query2->bindValue(":head", 1);
      } else if (direction == FUTURE) {
	query2->bindValue(":tail", 1);
	query2->bindValue(":head", 2);
      }
      query2->exec();
      delete query2;
    }
    QString coderText = "<i>" + selectedCoder + "</i>";
    coderFeedbackLabel->setText(coderText);
    query->prepare("SELECT description, question FROM linkage_types WHERE name = :name");
    query->bindValue(":name", selectedType);
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
  }
}

void LinkagesWidget::retrieveData() {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  if (!(query->isNull(0))) {
    tailIndex = query->value(0).toInt();
    headIndex = query->value(1).toInt();
  } else {
    delete query;
    return;
  }
  query->prepare("SELECT direction FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  selectedDirection = query->value(0).toString();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  int total = incidentsModel->rowCount();
  QString tailIndexText = "";
  QString headIndexText = "";
  if (selectedDirection == PAST) {
    tailIndexText = "<b>Tail (" + QString::number(tailIndex - 1) +
      " / " + QString::number(total - 1) + ") - Incident: " + QString::number(tailIndex) + "<b>";
    int headShown = tailIndex - headIndex;
    int totalShown = tailIndex - 1;
    headIndexText = "<b>Head (" + QString::number(headShown) +
      " / " + QString::number(totalShown) + ") - Incident: " + QString::number(headIndex) + "<b>";
  } else if (selectedDirection == FUTURE) {
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
  if (tailMark == 0) {
    tailMarkedLabel->setText("");
  } else {
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
  if (headMark == 0) {
    headMarkedLabel->setText("");
  } else {
    headMarkedLabel->setText("MARKED");
  }
  query->prepare("SELECT tail, head "
		 "FROM linkages "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  query->first();
  if (!(query->isNull(0))) {
    linkageFeedbackLabel->setText("LINKED");
    if (codingType == MANUAL) {
      unsetLinkButton->setEnabled(true);
      setLinkButton->setEnabled(false);
    }
  } else {
    linkageFeedbackLabel->setText("");
    if (codingType == MANUAL) {
      unsetLinkButton->setEnabled(false);
      setLinkButton->setEnabled(true);
    }
  }
  query->prepare("SELECT comment, coder FROM linkage_comments "
		 "WHERE tail = :tail AND head = :head AND type = :type");
  query->bindValue(":tail", tailIndex);
  query->bindValue(":head", headIndex);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  if (!(query->isNull(0))) {
    QString comment = query->value(0).toString();
    QString coder = query->value(1).toString();
    linkageCommentField->blockSignals(true);
    linkageCommentField->setText(comment);
    linkageCommentField->blockSignals(false);
    QString toolTip = "Last author: " + coder;
    linkageCommentField->setToolTip(toolTip);
  } else {
    linkageCommentField->blockSignals(true);
    linkageCommentField->setText("");
    linkageCommentField->blockSignals(false);
    linkageCommentField->setToolTip("No comment available");
  }
  delete query;
}

void LinkagesWidget::checkManualButton() {
  codingType = MANUAL;
  manualCodingButton->setChecked(true);
  assistedCodingButton->setChecked(false);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
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
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  query->first();
  if (!(query->isNull(0))) {
    setLinkButton->setEnabled(false);
    unsetLinkButton->setEnabled(true);
  } else {
    setLinkButton->setEnabled(true);
    unsetLinkButton->setEnabled(false);
  }
  delete query;
}

void LinkagesWidget::checkAssistedButton() {
  codingType = ASSISTED;
  assistedCodingButton->setChecked(true);
  manualCodingButton->setChecked(false);
  unsetLinkButton->setEnabled(true);
  setLinkButton->setEnabled(true);
}

void LinkagesWidget::setTailDescriptionFilter(const QString &text) {
  tailDescriptionFilter = text;
}

void LinkagesWidget::setTailRawFilter(const QString &text) {
  tailRawFilter = text;
}

void LinkagesWidget::setTailCommentFilter(const QString &text) {
  tailCommentFilter = text;
}

void LinkagesWidget::setHeadDescriptionFilter(const QString &text) {
  headDescriptionFilter = text;
}

void LinkagesWidget::setHeadRawFilter(const QString &text) {
  headRawFilter = text;
}

void LinkagesWidget::setHeadCommentFilter(const QString &text) {
  headCommentFilter = text;
}

void LinkagesWidget::previousTailDescription() {
  setComments();
  setLinkageComment();
  if (tailDescriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int tail = 0;
    query->first();
    tail = query->value(0).toInt();
    QString searchText = "%" + tailDescriptionFilter + "%";
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
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (selectedDirection == PAST) {
	if (tail >= 2) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      } else if (selectedDirection ==  FUTURE) {
	if (tail >= 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }  
}

void LinkagesWidget::nextTailDescription() {
  setComments();
  setLinkageComment();
  if (tailDescriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int tail = 0;
    query->first();
    tail = query->value(0).toInt();
    QString searchText = "%" + tailDescriptionFilter + "%";
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
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (selectedDirection == PAST) {
	if (tail <= incidentsModel->rowCount()) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      } else if (selectedDirection == FUTURE) {
	if (tail <= incidentsModel->rowCount() - 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::previousTailRaw() {
  setComments();
  setLinkageComment();
  if (tailRawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int tail = 0;
    query->first();
    tail = query->value(0).toInt();
    QString searchText = "%" + tailRawFilter + "%";
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
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (selectedDirection == PAST) {
	if (tail >= 2) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      } else if (selectedDirection ==  FUTURE) {
	if (tail >= 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}
  
void LinkagesWidget::nextTailRaw() {
  setComments();
  setLinkageComment();
  if (tailRawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int tail = 0;
    query->first();
    tail = query->value(0).toInt();
    QString searchText = "%" + tailRawFilter + "%";
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
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (selectedDirection == PAST) {
	if (tail <= incidentsModel->rowCount()) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      } else if (selectedDirection == FUTURE) {
	if (tail <= incidentsModel->rowCount() - 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::previousTailComment() {
  setComments();
  setLinkageComment();
  if (tailCommentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int tail = 0;
    query->first();
    tail = query->value(0).toInt();
    QString searchText = "%" + tailCommentFilter + "%";
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
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (selectedDirection == PAST) {
	if (tail >= 2) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      } else if (selectedDirection ==  FUTURE) {
	if (tail >= 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::nextTailComment() {
  setComments();
  setLinkageComment();
  if (tailCommentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int tail = 0;
    query->first();
    tail = query->value(0).toInt();
    QString searchText = "%" + tailCommentFilter + "%";
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
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (selectedDirection == PAST) {
	if (tail <= incidentsModel->rowCount()) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      } else if (selectedDirection == FUTURE) {
	if (tail <= incidentsModel->rowCount() - 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::previousHeadDescription() {
  setComments();
  setLinkageComment();
  if (headDescriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int head = 0;
    int tail = 0;
    query->first();
    head = query->value(0).toInt();
    tail = query->value(1).toInt();
    QString searchText = "%" + headDescriptionFilter + "%";
    if (selectedDirection == PAST) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order > :head "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head <= tail - 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    } else if (selectedDirection ==  FUTURE) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order < :head "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head >= tail + 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::nextHeadDescription() {
  setComments();
  setLinkageComment();
  if (headDescriptionFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT head FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int head = 0;
    query->first();
    head = query->value(0).toInt();
    QString searchText = "%" + headDescriptionFilter + "%";
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (selectedDirection == PAST) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order < :head "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head >=  1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    } else if (selectedDirection ==  FUTURE) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order > :head "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head <= incidentsModel->rowCount()) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::previousHeadRaw() {
  setComments();
  setLinkageComment();
  if (headRawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int head = 0;
    int tail = 0;
    query->first();
    head = query->value(0).toInt();
    tail = query->value(1).toInt();
    QString searchText = "%" + headRawFilter + "%";
    if (selectedDirection == PAST) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order > :head "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head <= tail - 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    } else if (selectedDirection ==  FUTURE) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order < :head "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head >= tail + 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::nextHeadRaw() {
  setComments();
  setLinkageComment();
  if (headRawFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT head FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int head = 0;
    query->first();
    head = query->value(0).toInt();
    QString searchText = "%" + headRawFilter + "%";
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (selectedDirection == PAST) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order < :head "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head >=  1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    } else if (selectedDirection ==  FUTURE) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order > :head "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head <= incidentsModel->rowCount()) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::previousHeadComment() {
  setComments();
  setLinkageComment();
  if (headCommentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int head = 0;
    int tail = 0;
    query->first();
    head = query->value(0).toInt();
    tail = query->value(1).toInt();
    QString searchText = "%" + headCommentFilter + "%";
    if (selectedDirection == PAST) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order > :head "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head <= tail - 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    } else if (selectedDirection ==  FUTURE) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order < :head "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head >= tail + 1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::nextHeadComment() {
  setComments();
  setLinkageComment();
  if (headCommentFilter != "") {
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT head FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    int head = 0;
    query->first();
    head = query->value(0).toInt();
    QString searchText = "%" + headCommentFilter + "%";
    while(incidentsModel->canFetchMore())
      incidentsModel->fetchMore();
    if (selectedDirection == PAST) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order < :head "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head >=  1) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    } else if (selectedDirection ==  FUTURE) {
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order > :head "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":head", head);
      query->exec();
      query->first();
      if (!query->isNull(0)) {
	head = query->value(0).toInt();
	if (head <= incidentsModel->rowCount()) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	}
      }
    }
    delete query;
  }
}

void LinkagesWidget::previousTail() {
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  if (selectedDirection == PAST) {
    if (tailIndex != 2) {
      tailIndex--;
      query->prepare("UPDATE coders_to_linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE coder =:coder and type = :type ");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", tailIndex - 1);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":type", selectedType);
      query->exec();
      retrieveData();
    }
  } else if (selectedDirection == FUTURE) {
    if (tailIndex != 1) {
      tailIndex--;
      if (codingType == ASSISTED && tailIndex < headIndex) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      } else {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", tailIndex + 1);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      }
    }
  }
  delete query;
}

void LinkagesWidget::nextTail() {
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  QSqlQuery  *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  if (selectedDirection == PAST) {
    if (tailIndex != incidentsModel->rowCount()) {
      tailIndex++;
      query->prepare("UPDATE coders_to_linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", tailIndex - 1);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":type", selectedType);
      query->exec();
      retrieveData();
    }
  } else if (selectedDirection == FUTURE) {
    if (tailIndex != incidentsModel->rowCount() - 1) {
      tailIndex++;
      if (codingType == ASSISTED && tailIndex < headIndex) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      } else {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", tailIndex + 1);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      }
    }
  }
  delete query;  
}

void LinkagesWidget::markTail() {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM coders_to_linkage_types "
	      "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
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
  if (currentMark == 0) {
    int newMark = 1;
    query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
    query->bindValue(":newMark", newMark);
    query->bindValue(":order", tail);
    query->exec();
    tailMarkedLabel->setText("MARKED");
  } else if (currentMark == 1) {
    int newMark = 0;
    query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
    query->bindValue(":newMark", newMark);
    query->bindValue(":order", tail);
    query->exec();
    tailMarkedLabel->setText("");
  }
  delete query;
}

void LinkagesWidget::previousMarkedTail() {
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
	      "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
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
  if (selectedDirection == PAST) {
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (tail >= 2) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", tail);
	query->bindValue(":head", tail - 1);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);	
	query->exec();
	retrieveData();
      }
    }
  } else if (selectedDirection == FUTURE) {
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (tail >= 1) {
	if (codingType == ASSISTED && tail < head) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);	
	  query->exec();
	  retrieveData();
	} else {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", tail + 1);	
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);	
	  query->exec();
	  retrieveData();
	}
      }
    }
  }
  delete query;
}

void LinkagesWidget::nextMarkedTail() {
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
	      "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
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
  if (incidentsModel->canFetchMore()) {
    incidentsModel->fetchMore();
  }
  if (selectedDirection == PAST) {
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (tail <= incidentsModel->rowCount()) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", tail);
	query->bindValue(":head", tail - 1);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);	
	query->exec();
	retrieveData();
      }
    }
  } else if (selectedDirection == FUTURE) {
    if (!query->isNull(0)) {
      tail = query->value(0).toInt();
      if (tail <= incidentsModel->rowCount() - 1) {
	if (codingType == ASSISTED && tail < head) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);	
	  query->exec();
	  retrieveData();
	} else {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tail);
	  query->bindValue(":head", tail + 1);	
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);	
	  query->exec();
	  retrieveData();
	}
      }
    }
  }
  delete query;
}

void LinkagesWidget::jumpTo() {
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  LinkagesIndexDialog *indexDialog = new LinkagesIndexDialog(this, incidentsModel->rowCount(), selectedDirection);
  indexDialog->exec();
  int tailIndex = 0;
  int headIndex = 0;
  if (indexDialog->getExitStatus() == 0) {
    if (selectedDirection == PAST) {
      tailIndex = indexDialog->getTailIndex() + 1;
      headIndex = (indexDialog->getHeadIndex() - tailIndex) * -1;
    } else if (selectedDirection == FUTURE) {
      tailIndex = indexDialog->getTailIndex();
      headIndex = indexDialog->getHeadIndex() + tailIndex;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("UPDATE coders_to_linkage_types "
		   "SET tail = :tail, head = :head "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":tail", tailIndex);
    query->bindValue(":head", headIndex);
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    retrieveData();
    delete query;
  }
  delete indexDialog;
}

void LinkagesWidget::previousHead() {
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head, tail FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  int headIndex = 0;
  int tailIndex = 0;
  headIndex = query->value(0).toInt();
  tailIndex = query->value(1).toInt();
  if (selectedDirection == PAST) {
    if (headIndex != tailIndex - 1) {
      headIndex++;
      query->prepare("UPDATE coders_to_linkage_types "
		     "SET head = :head "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":head", headIndex);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":type", selectedType);
      query->exec();
      retrieveData();
    } 
  } else if (selectedDirection == FUTURE) {
    if (headIndex != tailIndex + 1) {
      headIndex--;
      if (codingType == ASSISTED) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", headIndex - 1);
	query->bindValue(":head", headIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      } else {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":head", headIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      }
    } else {
      if (tailIndex != 1 && codingType == ASSISTED) {
	headIndex--;
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", headIndex - 1);
	query->bindValue(":head", headIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      }
    }
  }
  delete query;
}

void LinkagesWidget::nextHead() {
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  int headIndex = 0;
  headIndex = query->value(0).toInt();
  if (selectedDirection == PAST) {
    if (headIndex != 1) {
      headIndex--;
      query->prepare("UPDATE coders_to_linkage_types "
		     "SET head = :head "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":head", headIndex);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":type", selectedType);
      query->exec();
      retrieveData();
    } 
  } else if (selectedDirection == FUTURE) {
    if (headIndex != incidentsModel->rowCount()) {
      headIndex++;
      if (codingType == ASSISTED) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", headIndex - 1);
	query->bindValue(":head", headIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      } else {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET head = :head "
		     "WHERE coder = :coder AND type = :type");
	query->bindValue(":head", headIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
      }
    }
  }
  delete query;
}

void LinkagesWidget::markHead() {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM coders_to_linkage_types "
	      "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->first();
  int head = 0;
  head = query->value(0).toInt();
  query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
  query->bindValue(":order", head);
  query->exec();
  query->first();
  int currentMark = 0;
  currentMark = query->value(0).toInt();
  if (currentMark == 0) {
    int newMark = 1;
    query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
    query->bindValue(":order", head);
    query->bindValue(":newMark", newMark);
    query->exec();
    headMarkedLabel->setText("MARKED");
  } else if (currentMark == 1) {
    int newMark = 0;
    query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
    query->bindValue(":order", head);
    query->bindValue(":newMark", newMark);
    query->exec();
    headMarkedLabel->setText("");
  }
  delete query;
}

void LinkagesWidget::previousMarkedHead() {
  setComments();
  setLinkageComment();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head, tail FROM coders_to_linkage_types "
	      "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->exec();
  int head = 0;
  int tail = 0;
  query->first();
  head = query->value(0).toInt();
  tail = query->value(1).toInt();
  if (selectedDirection == PAST) {
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
    query->bindValue(":order", head);
    query->exec();
    query->first();
    if (!query->isNull(0)) {
      head = query->value(0).toInt();
      if (head <= tail - 1) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":head", head);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);	
	query->exec();
	retrieveData();
      }
    }
  } else if (selectedDirection == FUTURE) {
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE ch_order < :order AND mark = 1 ORDER BY ch_order desc");
    query->bindValue(":order", head);
    query->exec();
    query->first();
    if (!query->isNull(0)) {
      head = query->value(0).toInt();
      if (head >= tail + 1) {
	if (codingType == ASSISTED) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", head - 1);
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	} else {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);	
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);	
	  query->exec();
	  retrieveData();
	}
      }
    }
  }
  delete query;
}

void LinkagesWidget::nextMarkedHead() {
  setComments();
  setLinkageComment();
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) {
    incidentsModel->fetchMore();
  }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM coders_to_linkage_types "
	      "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
  query->exec();
  int head = 0;
  query->first();
  head = query->value(0).toInt();
  if (selectedDirection == PAST) {
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE ch_order < :order AND mark = 1 ORDER BY ch_order desc");
    query->bindValue(":order", head);
    query->exec();
    query->first();
    if (!query->isNull(0)) {
      head = query->value(0).toInt();
      if (head >= 1) {
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":head", head);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);	
	query->exec();
	retrieveData();
      }
    }
  } else if (selectedDirection == FUTURE) {
    query->prepare("SELECT ch_order FROM incidents "
		   "WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
    query->bindValue(":order", head);
    query->exec();
    query->first();
    if (!query->isNull(0)) {
      head = query->value(0).toInt();
      if (head <= incidentsModel->rowCount()) {
	if (codingType == ASSISTED) {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail, head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", head - 1);
	  query->bindValue(":head", head);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	} else {
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET head = :head "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":head", head);	
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);	
	  query->exec();
	  retrieveData();
	}
      }
    }
  }
  delete query;
}

void LinkagesWidget::setCommentBool() {
  commentBool = true;
}

void LinkagesWidget::setComments() {
  if (commentBool) {
    QString tailComment = tailCommentField->toPlainText();
    QString headComment = headCommentField->toPlainText();
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
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
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    query->first();
    int headIndex = 0;
    headIndex = query->value(0).toInt();
    query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :head");
    query->bindValue(":comment", headComment);
    query->bindValue(":head", headIndex);
    query->exec();
    commentBool = false;
    delete query;
  }
}

void LinkagesWidget::setLinkageCommentBool() {
  linkageCommentBool = true;
}

void LinkagesWidget::setLinkageComment() {
  if (linkageCommentBool) {
    QString comment = linkageCommentField->toPlainText();
    incidentsModel->select();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		   "WHERE coder = :coder AND type = :type");
    query->bindValue(":coder", selectedCoder);
    query->bindValue(":type", selectedType);
    query->exec();
    query->first();
    int tailIndex = 0;
    int headIndex = 0;
    tailIndex = query->value(0).toInt();
    headIndex = query->value(1).toInt();
    query->prepare("SELECT tail FROM linkage_comments "
		   "WHERE tail = :tail AND head = :head AND type = :type");
    query->bindValue(":tail", tailIndex);
    query->bindValue(":head", headIndex);
    query->bindValue(":type", selectedType);
    query->exec();
    query->first();
    if (!(query->isNull(0))) {
      if (comment != "") {
	query->prepare("UPDATE linkage_comments "
		       "SET comment = :comment, coder = :coder "
		       "WHERE tail = :tail AND head = :head AND type = :type");
	query->bindValue(":comment", comment);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", headIndex);
	query->bindValue(":type", selectedType);
	query->exec();
      } else {
	query->prepare("DELETE FROM linkage_comments "
		       "WHERE tail = :tail and head = :head AND type = :type");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", headIndex);
	query->bindValue(":type", selectedType);
	query->exec();
      }
    } else {
      query->prepare("INSERT INTO linkage_comments (tail, head, comment, coder, type)"
		     "VALUES (:tail, :head, :comment, :coder, :type)");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", headIndex);
      query->bindValue(":comment", comment);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":type", selectedType);
      query->exec();
    }
    linkageCommentBool = false;
    delete query;
  }
}

void LinkagesWidget::pause(int time) {
#ifdef __linux__ 
  std::chrono::milliseconds timespan(time); 
  std::this_thread::sleep_for(timespan);
#elif _WIN32
  Sleep(time);
#endif
}

void LinkagesWidget::setLink() {
  setComments();
  setLinkageComment();
  if (codingType == MANUAL) {
    setLinkButton->setEnabled(false);
    unsetLinkButton->setEnabled(true);
    unsetLinkButton->setFocus();
  }
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) {
    incidentsModel->fetchMore();
  }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
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
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  query->first();
  if (query->isNull(0)) {
    query->prepare("INSERT INTO linkages (tail, head, type, coder) "
		   "VALUES (:tail, :head, :type, :coder)");
    query->bindValue(":tail", tailId);
    query->bindValue(":head", headId);
    query->bindValue(":type", selectedType);
    query->bindValue(":coder", selectedCoder);
    query->exec();
  }
  if (linkageFeedbackLabel->text() != "LINKED") {
    linkageFeedbackLabel->setText("LINKED");
    linkageFeedbackLabel->adjustSize();
    linkageFeedbackLabel->repaint();
  }
  qApp->processEvents();
  eventGraph->checkCongruency();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (codingType == ASSISTED && selectedDirection == PAST) {
    QSet<int> ignore;
    QSet<int> done;
    if (headIndex != 1) {
      findPastPaths(&ignore, &done, tailIndex);
      for (int i = headIndex - 1; i != 0; i--) {
	bool found = false;
	if (ignore.contains(i)) {
	  found = true;
	}
	if (!found) {
	  if (headIndex != 1) {
	    headIndex = i;
	    query->prepare("UPDATE coders_to_linkage_types "
			   "SET head = :head "
			   "WHERE coder = :coder AND type = :type");
	    query->bindValue(":head", headIndex);
	    query->bindValue(":coder", selectedCoder);
	    query->bindValue(":type", selectedType);
	    query->exec();
	    pause(500);
	    retrieveData();
	    delete query;
	    QApplication::restoreOverrideCursor();
	    qApp->processEvents();
	    return;
	  } else {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", selectedCoder);
	      query->bindValue(":type", selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      delete query;
	      QApplication::restoreOverrideCursor();
	      qApp->processEvents();
	      return; 
	    }
	  }
	} else {
	  if (i == 1) {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", selectedCoder);
	      query->bindValue(":type", selectedType);
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
    } else {
      if (tailIndex != incidentsModel->rowCount()) {
	tailIndex++;
	headIndex = tailIndex - 1;
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type = :type");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", headIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	pause(500);
	retrieveData();
	QApplication::restoreOverrideCursor();
	qApp->processEvents();
	delete query;
	return; 
      }
    }
  } else if (codingType == ASSISTED && selectedDirection == FUTURE) {
    QSet<int> ignore;
    QSet<int> done;
    if (tailIndex != 1) {
      findFuturePaths(&ignore, &done, tailIndex);
      for (int i = tailIndex - 1; i != 0; i--) {
	bool found = false;
	if (ignore.contains(i)) {
	  found = true;
	}
	if (!found) {
	  tailIndex = i;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  pause(500);
	  retrieveData();
	  delete query;
	  QApplication::restoreOverrideCursor();
	  qApp->processEvents();
	  return;
	} else {
	  if (i == 1) {
	    if (headIndex != incidentsModel->rowCount()) {
	      headIndex++;
	      tailIndex = headIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", selectedCoder);
	      query->bindValue(":type", selectedType);
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
    } else {
      headIndex++;
      tailIndex = headIndex - 1;
      query->prepare("UPDATE coders_to_linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", headIndex);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":type", selectedType);
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

void LinkagesWidget::unsetLink() {
  setComments();
  setLinkageComment();
  if (codingType == MANUAL) {
    setLinkButton->setEnabled(true);
    unsetLinkButton->setEnabled(false);
    setLinkButton->setFocus();
  }
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) {
    incidentsModel->fetchMore();
  }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", selectedCoder);
  query->bindValue(":type", selectedType);
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
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  query->first();
  if (!(query->isNull(0))) {
    query->prepare("DELETE FROM linkages "
		   "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
    query->bindValue(":tail", tailId);
    query->bindValue(":head", headId);
    query->bindValue(":type", selectedType);
    query->bindValue(":coder", selectedCoder);
    query->exec();
  }
  linkageFeedbackLabel->setText("");
  linkageFeedbackLabel->adjustSize();
  linkageFeedbackLabel->repaint();
  qApp->processEvents();
  eventGraph->checkCongruency();
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (codingType == ASSISTED && selectedDirection == PAST) {
    QSet<int> ignore;
    QSet<int> done;
    if (headIndex != 1) {
      findPastPaths(&ignore, &done, tailIndex);
      for (int i = headIndex - 1; i != 0; i--) {
	bool found = false;
	if (ignore.contains(i)) {
	  found = true;
	}
	if (!found) {
	  if (headIndex != 1) {
	    headIndex = i;
	    query->prepare("UPDATE coders_to_linkage_types "
			   "SET head = :head "
			   "WHERE coder = :coder AND type = :type");
	    query->bindValue(":head", headIndex);
	    query->bindValue(":coder", selectedCoder);
	    query->bindValue(":type", selectedType);
	    query->exec();
	    retrieveData();
	    delete query;
	    QApplication::restoreOverrideCursor();
	    qApp->processEvents();
	    return;
	  } else {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", selectedCoder);
	      query->bindValue(":type", selectedType);
	      query->exec();
	      retrieveData();
	      QApplication::restoreOverrideCursor();
	      qApp->processEvents();
	      delete query;
	      return; 
	    }
	  }
	} else {
	  if (i == 1) {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", selectedCoder);
	      query->bindValue(":type", selectedType);
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
    } else {
      if (tailIndex != incidentsModel->rowCount()) {
	tailIndex++;
	headIndex = tailIndex - 1;
	query->prepare("UPDATE coders_to_linkage_types "
		       "SET tail = :tail, head = :head "
		       "WHERE coder = :coder AND type  = :type");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", headIndex);
	query->bindValue(":coder", selectedCoder);
	query->bindValue(":type", selectedType);
	query->exec();
	retrieveData();
	QApplication::restoreOverrideCursor();
	qApp->processEvents();
	delete query;
	return; 
      }
    }
  } else if (codingType == ASSISTED && selectedDirection == FUTURE) {
    QSet<int> ignore;
    QSet<int> done;
    if (tailIndex != 1) {
      findFuturePaths(&ignore, &done, headIndex);
      for (int i = tailIndex - 1; i != 0; i--) {
	bool found = false;
	if (ignore.contains(i)) {
	  found = true;
	}
	if (!found) {
	  tailIndex = i;
	  query->prepare("UPDATE coders_to_linkage_types "
			 "SET tail = :tail "
			 "WHERE coder = :coder AND type = :type");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":coder", selectedCoder);
	  query->bindValue(":type", selectedType);
	  query->exec();
	  retrieveData();
	  QApplication::restoreOverrideCursor();
	  qApp->processEvents();
	  delete query;
	  return;
	} else {
	  if (i == 1) {
	    if (headIndex != incidentsModel->rowCount()) {
	      headIndex++;
	      tailIndex = headIndex - 1;
	      query->prepare("UPDATE coders_to_linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE coder = :coder AND type = :type");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":coder", selectedCoder);
	      query->bindValue(":type", selectedType);
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
    } else {
      headIndex++;
      tailIndex = headIndex - 1;
      query->prepare("UPDATE coders_to_linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE coder = :coder AND type = :type");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", headIndex);
      query->bindValue(":coder", selectedCoder);
      query->bindValue(":type", selectedType);
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

void LinkagesWidget::findPastPaths(QSet<int> *pIgnore, QSet<int> *pDone, int currentIncident) {
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
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  QSet<int> results;
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :current");
  while (query->next()) {
    int currentHead = 0;
    currentHead = query->value(0).toInt();
    query2->bindValue(":current", currentHead);
    query2->exec();
    query2->first();
    int newIndex = query2->value(0).toInt();
    if (!pDone->contains(newIndex)) {
      results.insert(newIndex);
    }
  }
  delete query2;
  delete query;
  QSqlDatabase::database().commit();
  QList<int> tempList = results.toList();
  std::sort(tempList.begin(), tempList.end());
  QList<int>::iterator it;
  for (it = tempList.begin(); it != tempList.end(); it++) {
    pIgnore->insert(*it);
    pDone->insert(*it);
    findPastPaths(pIgnore, pDone, *it);
  }
}

void LinkagesWidget::findFuturePaths(QSet<int> *pIgnore, QSet<int> *pDone, int currentIncident) {
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
  query->bindValue(":type", selectedType);
  query->bindValue(":coder", selectedCoder);
  query->exec();
  QSet<int> results;
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT ch_order FROM incidents WHERE id = :current");
  while (query->next()) {
    int currentTail = 0;
    currentTail = query->value(0).toInt();
    query2->bindValue(":current", currentTail);
    query2->exec();
    query2->first();
    int newIndex = query2->value(0).toInt();
    if (!pDone->contains(newIndex)) {
      results.insert(newIndex);
    }
  }
  delete query;
  delete query2;
  QSqlDatabase::database().commit();
  QList<int> tempList = results.toList();
  std::sort(tempList.begin(), tempList.end());
  QList<int>::iterator it;
  for (it = tempList.begin(); it != tempList.end(); it++) {
    pIgnore->insert(*it);
    pDone->insert(*it);
    findFuturePaths(pIgnore, pDone, *it);
  }
}

void LinkagesWidget::setButtons(bool status) {
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
}

bool LinkagesWidget::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = (QWheelEvent*) event;
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
    if (textEdit) {
      if(wheelEvent->modifiers() & Qt::ControlModifier) {
        if (wheelEvent->angleDelta().y() > 0) {
	  textEdit->zoomIn(1);
	} else if (wheelEvent->angleDelta().y() < 0) {
	  textEdit->zoomOut(1);
	}
      }
    }
  }
  return false;
}

void LinkagesWidget::setEventGraph(EventGraphWidget *egw) {
  eventGraph = egw;
}

void LinkagesWidget::finalBusiness() {
  setComments();
  setLinkageComment();
}
