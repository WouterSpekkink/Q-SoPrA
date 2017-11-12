
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
  commentBool = false;
  
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
  selectTypeLabel =  new QLabel(tr("select linkage type:"), this);
  tailIndexLabel = new QLabel(tr("<b>Tail ( / )<b>"), this);
  tailMarkedLabel = new QLabel("");
  tailMarkedLabel->setStyleSheet("QLabel {color: red}");
  tailTimeStampLabel = new QLabel(tr("<b>Timing:</b>"), this);
  tailSourceLabel = new QLabel(tr("<b>Source:</b>"), this);
  tailDescriptionLabel = new QLabel(tr("<b>Description:</b>"), this);
  tailRawLabel = new QLabel(tr("<b>Raw</b>"), this);
  tailCommentLabel = new QLabel(tr("<b>Comments:</b>"), this);
  tailDescriptionFilterLabel = new QLabel(tr("<i>Search descriptions:</i>"), this);
  tailRawFilterLabel = new QLabel(tr("<i>Search raw texts:</i>"), this);
  tailCommentFilterLabel = new QLabel(tr("<i>Search comments:</i>"), this);
  headIndexLabel = new QLabel(tr("<b>Head ( / )</b>"), this);
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
  linkageTypeLabel = new QLabel(tr("<b>Linkage type:</b>"), this);
  linkageTypeFeedbackLabel = new QLabel(tr(""), this);
  tailMarkedLabel->setStyleSheet("QLabel {font-weight: bold, color: red}");
  linkageQuestionLabel = new QLabel(tr("<b>Linkage question:</b>"), this);
  linkageQuestionFeedbackLabel = new QLabel(tr(""), this);
  linkageQuestionFeedbackLabel->setWordWrap(true);
  linkageQuestionFeedbackLabel->setMaximumWidth(200);
  linkageFeedbackLabel = new QLabel(tr(""), this);
  linkageCommentLabel = new QLabel(tr("<b>Linkage comments:</b>"), this);

  tailTimeStampField = new QLineEdit(this);
  tailSourceField = new QLineEdit(this);
  tailDescriptionFilterField = new QLineEdit(this);
  tailRawFilterField = new QLineEdit(this);
  tailCommentFilterField = new QLineEdit(this);
  headTimeStampField = new QLineEdit(this);
  headSourceField = new QLineEdit(this);
  headDescriptionFilterField = new QLineEdit(this);
  headRawFilterField = new QLineEdit(this);
  headCommentFilterField = new QLineEdit(this);

  tailDescriptionField = new QTextEdit(this);
  tailRawField = new QTextEdit(this);
  tailCommentField = new QTextEdit(this);
  headDescriptionField = new QTextEdit(this);
  headRawField = new QTextEdit(this);
  headCommentField = new QTextEdit(this);
  linkageCommentField = new QTextEdit(this);

  typeComboBox = new QComboBox(this);
  typeComboBox->addItem(DEFAULT);

  createTypeButton = new QPushButton(tr("New linkage type"), this);
  editTypeButton = new QPushButton(tr("Edit selected type"), this);
  removeTypeButton = new QPushButton(tr("Remove selected type"), this);
  selectTypeButton = new QPushButton(tr("Load selected linkage"), this);
  selectTypeButton->setStyleSheet("QPushButton {font-weight: bold}");
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
  unsetLinkButton =new QPushButton(tr("Not linked"), this);
  
  connect(createTypeButton, SIGNAL(clicked()), this, SLOT(addLinkageType()));
  connect(editTypeButton, SIGNAL(clicked()), this, SLOT(editLinkageType()));
  connect(removeTypeButton, SIGNAL(clicked()), this, SLOT(removeLinkageType()));
  connect(selectTypeButton, SIGNAL(clicked()), this, SLOT(setLinkageType()));
  connect(manualCodingButton, SIGNAL(clicked()), this, SLOT(checkManualButton()));
  connect(assistedCodingButton, SIGNAL(clicked()), this, SLOT(checkAssistedButton()));
  connect(previousTailButton, SIGNAL(clicked()), this, SLOT(previousTail()));
  connect(nextTailButton, SIGNAL(clicked()), this, SLOT(nextTail()));
  connect(previousHeadButton, SIGNAL(clicked()), this, SLOT(previousHead()));
  connect(nextHeadButton, SIGNAL(clicked()), this, SLOT(nextHead()));
  connect(tailCommentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(setLinkButton, SIGNAL(clicked()), this, SLOT(setLink()));
  connect(unsetLinkButton, SIGNAL(clicked()), this, SLOT(unsetLink()));
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(settingsLabel);
  settingsLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  QPointer<QHBoxLayout> linkageOptionsLayout = new QHBoxLayout;
  linkageOptionsLayout->addWidget(selectTypeLabel);
  linkageOptionsLayout->addWidget(typeComboBox);
  linkageOptionsLayout->addWidget(selectTypeButton);
  selectTypeButton->setMaximumWidth(selectTypeButton->sizeHint().width());
  linkageOptionsLayout->addWidget(createTypeButton);
  createTypeButton->setMaximumWidth(createTypeButton->sizeHint().width());
  linkageOptionsLayout->addWidget(editTypeButton);
  editTypeButton->setMaximumWidth(editTypeButton->sizeHint().width());
  linkageOptionsLayout->addWidget(removeTypeButton);
  removeTypeButton->setMaximumWidth(removeTypeButton->sizeHint().width());
  linkageOptionsLayout->addWidget(manualCodingButton);
  manualCodingButton->setMaximumWidth(manualCodingButton->sizeHint().width());
  linkageOptionsLayout->addWidget(assistedCodingButton);
  assistedCodingButton->setMaximumWidth(assistedCodingButton->sizeHint().width());
  linkageOptionsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  mainLayout->addLayout(linkageOptionsLayout);

  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);
  
  QPointer<QHBoxLayout> fieldsLayout = new QHBoxLayout;
  
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> tailIndexLayout = new QHBoxLayout;
  tailIndexLayout->addWidget(tailIndexLabel);
  tailIndexLayout->addWidget(tailMarkedLabel);
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
  tailDescriptionFilterField->setFixedWidth(250);
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
  tailRawFilterField->setFixedWidth(250);
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
  tailCommentFilterField->setFixedWidth(250);
  tailCommentLayoutRight->addWidget(tailCommentNextButton);
  tailCommentNextButton->setMaximumWidth(tailCommentNextButton->sizeHint().width());
  tailCommentLayout->addLayout(tailCommentLayoutRight);
  leftLayout->addLayout(tailCommentLayout);
  leftLayout->addWidget(tailCommentField);
  fieldsLayout->addLayout(leftLayout);

  QPointer<QVBoxLayout> middleLayout = new QVBoxLayout;
  middleLayout->setAlignment(Qt::AlignVCenter);
  middleLayout->addWidget(linkageTypeLabel);
  linkageTypeLabel->setAlignment(Qt::AlignHCenter);
  middleLayout->addWidget(linkageTypeFeedbackLabel);
  linkageTypeFeedbackLabel->setAlignment(Qt::AlignHCenter);
  linkageTypeFeedbackLabel->setStyleSheet("color: red");
  middleLayout->addSpacerItem(new QSpacerItem(250, 50));
  middleLayout->addWidget(linkageQuestionLabel);
  linkageQuestionLabel->setAlignment(Qt::AlignHCenter);
  middleLayout->addWidget(linkageQuestionFeedbackLabel);
  middleLayout->setAlignment(linkageQuestionFeedbackLabel, Qt::AlignHCenter | Qt::AlignVCenter);
  middleLayout->addSpacerItem(new QSpacerItem(250, 50));
  middleLayout->addWidget(linkageFeedbackLabel);
  linkageFeedbackLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  linkageFeedbackLabel->setStyleSheet("color: blue");
  linkageFeedbackLabel->setMinimumHeight(40);
  middleLayout->addWidget(setLinkButton);
  middleLayout->addWidget(unsetLinkButton);
  fieldsLayout->addLayout(middleLayout);

  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> headIndexLayout = new QHBoxLayout;
  headIndexLayout->addWidget(headIndexLabel);
  headIndexLayout->addWidget(headMarkedLabel);
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
  headDescriptionFilterField->setFixedWidth(250);
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
  headRawFilterField->setFixedWidth(250);
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
  headCommentFilterField->setFixedWidth(250);
  headCommentLayoutRight->addWidget(headCommentNextButton);
  headCommentNextButton->setMaximumWidth(headCommentNextButton->sizeHint().width());
  headCommentLayout->addLayout(headCommentLayoutRight);
  rightLayout->addLayout(headCommentLayout);
  rightLayout->addWidget(headCommentField);
  fieldsLayout->addLayout(rightLayout);

  mainLayout->addLayout(fieldsLayout);
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

  retrieveLinkages();
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
    query->prepare("INSERT INTO linkage_types (name, description, question, direction, tail, head) "
		   "VALUES (:name, :description, :question, :direction, :tail, :head)");
    query->bindValue(":name", name);
    query->bindValue(":description", description);
    query->bindValue(":question", question);
    query->bindValue(":direction", direction);
    if (direction == PAST) {
      query->bindValue(":tail", 1);
      query->bindValue(":head", 2);

    } else if (direction == FUTURE) {
      query->bindValue(":tail", 2);
      query->bindValue(":head", 1);
    }
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
    warningBox->setInformativeText("This will remove the linkage type and all its linkages. This action cannot be undone. Are you sure you want to remove this linkage type?");
    if (warningBox->exec() == QMessageBox::Yes) {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("DELETE FROM linkage_types WHERE name = :name");
      query->bindValue(":name", typeComboBox->currentText());
      query->exec();
      query->prepare("DELETE FROM linkages WHERE type = :name");
      query->bindValue(":name", typeComboBox->currentText());
      query->exec();
      delete query;
      typeComboBox->removeItem(typeComboBox->currentIndex());
    }
    delete warningBox;
  }
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

void LinkagesWidget::setLinkageType() {
  if (typeComboBox->currentText() != DEFAULT) {
    QString currentType = typeComboBox->currentText();
    selectedType = currentType;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT description, question FROM linkage_types WHERE name = :name");
    query->bindValue(":name", currentType);
    query->exec();
    query->first();
    QString description = query->value(0).toString();
    QString question = query->value(1).toString();
    QString label = "<FONT SIZE = 3>--[" + typeComboBox->currentText() + "]--></FONT>";
    linkageTypeFeedbackLabel->setText(label);
    QString toolTip = "<FONT SIZE = 3>" + description + "</FONT>";
    linkageTypeFeedbackLabel->setToolTip(toolTip);
    linkageQuestionFeedbackLabel->setText(question);
    linkageQuestionFeedbackLabel->setMinimumHeight(linkageQuestionFeedbackLabel->sizeHint().height());
    retrieveData();
    delete query;
  }
}

void LinkagesWidget::retrieveData() {
  QString currentType = selectedType;
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head, direction FROM linkage_types WHERE name = :name");
  query->bindValue(":name", currentType);
  query->exec();
  query->first();
  int tailIndex = query->value(0).toInt();
  int headIndex = query->value(1).toInt();
  selectedDirection = query->value(2).toString();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  int total = incidentsModel->rowCount();
  QString tailIndexText = "";
  QString headIndexText = "";
  if (selectedDirection == PAST) {
    tailIndexText = "<b>Tail (" + QString::number(tailIndex - 1) +
      " / " + QString::number(total - 1) + ")<b>";
    int headShown = tailIndex - headIndex;
    int totalShown = tailIndex - 1;
    headIndexText = "<b>Head (" + QString::number(headShown) +
      " / " + QString::number(totalShown) + ")<b>";
  } else if (selectedDirection == FUTURE) {
    tailIndexText = "<b>Tail (" + QString::number(tailIndex) +
      " / " + QString::number(total - 1) + ")<b>";
    int headShown = headIndex - tailIndex;
    int totalShown = total - tailIndex;
    headIndexText = "<b>Head (" + QString::number(headShown) +
      " / " + QString::number(totalShown) + ")<b>";
  }
  tailIndexLabel->setText(tailIndexText);
  headIndexLabel->setText(headIndexText);
  query->prepare("SELECT id, timestamp, source, description, raw, comment, mark FROM incidents WHERE ch_order = :tail");
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
  query->prepare("SELECT id, timestamp, source, description, raw, comment, mark FROM incidents WHERE ch_order = :head");
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
  query->prepare("SELECT tail, head FROM linkages WHERE tail = :tail AND head = :head AND type = :type");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", selectedType);
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
  delete query;
}

void LinkagesWidget::checkManualButton() {
  codingType = MANUAL;
  manualCodingButton->setChecked(true);
  assistedCodingButton->setChecked(false);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  int headIndex = 0;
  tailIndex = query->value(0).toInt();
  headIndex = query->value(1).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :tail");
  query->bindValue(":tail", tailIndex);
  query->exec();
  int tailId = 0;
  tailId = query->value(0).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :head");
  query->bindValue(":head", headIndex);
  query->exec();
  int headId = 0;
  headId = query->value(0).toInt();
  query->prepare("SELECT tail, head FROM linkages WHERE tail = :tail AND head = :head AND type = :type");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  if (!(query->isNull(0))) {
    setLinkButton->setEnabled(false);
    unsetLinkButton->setEnabled(true);
  } else {
    setLinkButton->setEnabled(true);
    unsetLinkButton->setEnabled(false);
  }
}

void LinkagesWidget::checkAssistedButton() {
  codingType = ASSISTED;
  assistedCodingButton->setChecked(true);
  manualCodingButton->setChecked(false);
  unsetLinkButton->setEnabled(true);
  setLinkButton->setEnabled(true);
}

void LinkagesWidget::previousTail() {
  setComments();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  tailIndex = query->value(0).toInt();
  if (selectedDirection == PAST) {
    if (tailIndex != 2) {
      tailIndex--;
      query->prepare("UPDATE linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE name = :name");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", tailIndex - 1);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
    }
  } else if (selectedDirection == FUTURE) {
    if (tailIndex != 1) {
      tailIndex--;
      query->prepare("UPDATE linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE name = :name");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", tailIndex + 1);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
    }
  }
  delete query;
}

void LinkagesWidget::nextTail() {
  setComments();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  QSqlQuery  *query = new QSqlQuery;
  query->prepare("SELECT tail FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  int tailIndex = 0;
  tailIndex = query->value(0).toInt();
  if (selectedDirection == PAST) {
    if (tailIndex != incidentsModel->rowCount()) {
      tailIndex++;
      query->prepare("UPDATE linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE name = :name");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", tailIndex - 1);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
    }
  } else if (selectedDirection == FUTURE) {
    if (tailIndex != incidentsModel->rowCount() - 1) {
      tailIndex++;
      query->prepare("UPDATE linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE name = :name");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", tailIndex + 1);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
    }
  }
  delete query;  
}

void LinkagesWidget::previousHead() {
  setComments();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head, tail FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  int headIndex = 0;
  int tailIndex = 0;
  headIndex = query->value(0).toInt();
  tailIndex = query->value(1).toInt();
  if (selectedDirection == PAST) {
    if (headIndex != tailIndex - 1) {
      headIndex++;
      query->prepare("UPDATE linkage_types "
		     "SET head = :head "
		     "WHERE name = :name");
      query->bindValue(":head", headIndex);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
    } 
  } else if (selectedDirection == FUTURE) {
    if (headIndex != tailIndex + 1) {
      headIndex--;
      query->prepare("UPDATE linkage_types "
		     "SET head = :head "
		     "WHERE name = :name");
      query->bindValue(":head", headIndex);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
    }
  }
  delete query;
}

void LinkagesWidget::nextHead() {
  setComments();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT head FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
  query->exec();
  query->first();
  int headIndex = 0;
  headIndex = query->value(0).toInt();
  if (selectedDirection == PAST) {
    if (headIndex != 1) {
      headIndex--;
      query->prepare("UPDATE linkage_types "
		     "SET head = :head "
		     "WHERE name = :name");
      query->bindValue(":head", headIndex);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
    } 
  } else if (selectedDirection == FUTURE) {
    if (headIndex != incidentsModel->rowCount()) {
      headIndex++;
      query->prepare("UPDATE linkage_types "
		     "SET head = :head "
		     "WHERE name = :name");
      query->bindValue(":head", headIndex);
      query->bindValue(":name", selectedType);
      query->exec();
      retrieveData();
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
    query->prepare("SELECT tail FROM linkage_types WHERE name = :name");
    query->bindValue(":name", selectedType);
    query->exec();
    query->first();
    int tailIndex = 0;
    tailIndex = query->value(0).toInt();
    query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :tail");
    query->bindValue(":comment", tailComment);
    query->bindValue(":order", tailIndex);
    query->exec();
    query->prepare("SELECT head FROM linkage_types WHERE name = :name");
    query->bindValue(":name", selectedType);
    query->exec();
    query->first();
    int headIndex = 0;
    headIndex = query->value(0).toInt();
    query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :head");
    query->bindValue(":comment", headComment);
    query->bindValue(":order", headIndex);
    query->exec();
    commentBool = false;
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
  if (codingType == MANUAL) {
    setLinkButton->setEnabled(false);
    unsetLinkButton->setEnabled(true);
  }
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) {
    incidentsModel->fetchMore();
  }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
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
  query->prepare("SELECT tail, head FROM linkages WHERE tail = :tail AND head = :head");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->exec();
  query->first();
  if (query->isNull(0)) {
    query->prepare("INSERT INTO linkages (tail, head, type) "
		   "VALUES (:tail, :head, :type)");
    query->bindValue(":tail", tailId);
    query->bindValue(":head", headId);
    query->bindValue(":type", selectedType);
    query->exec();
  } 
  linkageFeedbackLabel->setText("LINKED");
  qApp->processEvents();
  if (codingType == ASSISTED && selectedDirection == PAST) {
    std::vector<int> ignore;
    if (headIndex != 1) {
      findPastPaths(&ignore, headIndex);
      std::vector<int>::iterator it;
      for (int i = headIndex - 1; i != 0; i--) {
	bool found = false;
	for (it = ignore.begin(); it != ignore.end(); it++) {
	  if (*it == i) {
	    found = true;
	  }
	}
	if (!found) {
	  if (headIndex != 1) {
	    headIndex = i;
	    query->prepare("UPDATE linkage_types SET head = :head WHERE name = :name");
	    query->bindValue(":head", headIndex);
	    query->bindValue(":name", selectedType);
	    query->exec();
	    pause(500);
	    retrieveData();
	    return;
	  } else {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":name", selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      return; 
	    }
	  }
	} else {
	  if (i == 1) {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":name", selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      return; 
	    }
	  }
	}
      }
    } else {
      if (tailIndex != incidentsModel->rowCount()) {
	tailIndex++;
	headIndex = tailIndex - 1;
	query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", headIndex);
	query->bindValue(":name", selectedType);
	query->exec();
	pause(500);
	retrieveData();
	return; 
      }
    }
  } else if (codingType == ASSISTED && selectedDirection == FUTURE) {
    std::vector<int> ignore;
    if (tailIndex != 1) {
      findFuturePaths(&ignore, tailIndex);
      std::vector<int>::iterator it;
      for (int i = tailIndex - 1; i != 0; i--) {
	bool found = false;
	for (it = ignore.begin(); it != ignore.end(); it++) {
	  if (*it == i) {
	    found = true;
	  }
	}
	if (!found) {
	  tailIndex = i;
	  query->prepare("UPDATE linkage_types SET tail = :tail WHERE name = :name");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":name", selectedType);
	  query->exec();
	  pause(500);
	  retrieveData();
	  return;
	} else {
	  if (i == 1) {
	    if (headIndex != incidentsModel->rowCount()) {
	      headIndex++;
	      tailIndex = headIndex - 1;
	      query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":name", selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      return;
	    }
	  }
	}
      }
    } else {
      headIndex++;
      tailIndex = headIndex - 1;
      query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", headIndex);
      query->bindValue(":name", selectedType);
      query->exec();
      pause(500);
      retrieveData();
      return;
    }
  }
}

void LinkagesWidget::unsetLink() {
  if (codingType == MANUAL) {
    setLinkButton->setEnabled(true);
    unsetLinkButton->setEnabled(false);
  }
  incidentsModel->select();
  while (incidentsModel->canFetchMore()) {
    incidentsModel->fetchMore();
  }
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM linkage_types WHERE name = :name");
  query->bindValue(":name", selectedType);
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
  query->prepare("SELECT tail, head FROM linkages WHERE tail = :tail AND head = :head AND type = :type");
  query->bindValue(":tail", tailId);
  query->bindValue(":head", headId);
  query->bindValue(":type", selectedType);
  query->exec();
  query->first();
  if (!(query->isNull(0))) {
    query->prepare("DELETE FROM linkages WHERE tail = :tail AND head = :head AND type = :type");
    query->bindValue(":tail", tailId);
    query->bindValue(":head", headId);
    query->bindValue(":type", selectedType);
    query->exec();
  }
  linkageFeedbackLabel->setText("");
  if (codingType == ASSISTED && selectedDirection == PAST) {
    std::vector<int> ignore;
    if (headIndex != 1) {
      findPastPaths(&ignore, tailIndex);
      std::vector<int>::iterator it;
      for (int i = headIndex - 1; i != 0; i--) {
	bool found = false;
	for (it = ignore.begin(); it != ignore.end(); it++) {
	  if (*it == i) {
	    found = true;
	  }
	}
	if (!found) {
	  if (headIndex != 1) {
	    headIndex = i;
	    query->prepare("UPDATE linkage_types SET head = :head WHERE name = :name");
	    query->bindValue(":head", headIndex);
	    query->bindValue(":name", selectedType);
	    query->exec();
	    pause(500);
	    retrieveData();
	    return;
	  } else {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":name", selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      return; 
	    }
	  }
	} else {
	  if (i == 1) {
	    if (tailIndex != incidentsModel->rowCount()) {
	      tailIndex++;
	      headIndex = tailIndex - 1;
	      query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":name", selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      return; 
	    }
	  }
	}
      }
    } else {
      if (tailIndex != incidentsModel->rowCount()) {
	tailIndex++;
	headIndex = tailIndex - 1;
	query->prepare("UPDATE linkage_types SET tail = :tail, head = :head WHERE name = :name");
	query->bindValue(":tail", tailIndex);
	query->bindValue(":head", headIndex);
	query->bindValue(":name", selectedType);
	query->exec();
	pause(500);
	retrieveData();
	return; 
      }
    }
  } else if (codingType == ASSISTED && selectedDirection == FUTURE) {
    std::vector<int> ignore;
    if (tailIndex != 1) {
      findFuturePaths(&ignore, headIndex);
      std::vector<int>::iterator it;
      for (int i = tailIndex - 1; i != 0; i--) {
	bool found = false;
	for (it = ignore.begin(); it != ignore.end(); it++) {
	  if (*it == i) {
	    found = true;
	  }
	}
	if (!found) {
	  tailIndex = i;
	  query->prepare("UPDATE linkage_types "
			 "SET tail = :tail "
			 "WHERE name = :name");
	  query->bindValue(":tail", tailIndex);
	  query->bindValue(":name", selectedType);
	  query->exec();
	  pause(500);
	  retrieveData();
	  return;
	} else {
	  if (i == 1) {
	    if (headIndex != incidentsModel->rowCount()) {
	      headIndex++;
	      tailIndex = headIndex - 1;
	      query->prepare("UPDATE linkage_types "
			     "SET tail = :tail, head = :head "
			     "WHERE name = :name");
	      query->bindValue(":tail", tailIndex);
	      query->bindValue(":head", headIndex);
	      query->bindValue(":name", selectedType);
	      query->exec();
	      pause(500);
	      retrieveData();
	      return;
	    }
	  }
	}
      }
    } else {
      headIndex++;
      tailIndex = headIndex - 1;
      query->prepare("UPDATE linkage_types "
		     "SET tail = :tail, head = :head "
		     "WHERE name = :name");
      query->bindValue(":tail", tailIndex);
      query->bindValue(":head", headIndex);
      query->bindValue(":name", selectedType);
      query->exec();
      pause(500);
      retrieveData();
      return;
    }
  }
}

void LinkagesWidget::findPastPaths(std::vector<int> *pIgnore, int currentIncident) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT id FROM incidents WHERE ch_order = :current");
  query->bindValue(":current", currentIncident);
  query->exec();
  query->first();
  int currentTail = 0;
  currentTail = query->value(0).toInt();
  query->prepare("SELECT head FROM linkages WHERE tail = :tail AND type = :type");
  query->bindValue(":tail", currentTail);
  query->bindValue(":type", selectedType);
  query->exec();
  std::vector<int> results;
  while (query->next()) {
    int currentHead = 0;
    currentHead = query->value(0).toInt();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT ch_order FROM incidents WHERE id = :current");
    query2->bindValue(":current", currentHead);
    query2->exec();
    query2->first();
    int newIndex = 0;
    newIndex = query->value(0).toInt();
    results.push_back(newIndex);
    delete query2;
  }
  delete query;
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) {
    pIgnore->push_back(*it);
    findPastPaths(pIgnore, *it);
  }
}

void LinkagesWidget::findFuturePaths(std::vector<int> *pIgnore, int currentIncident) {
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT id FROM incidents WHERE ch_order = :current");
  query->bindValue(":current", currentIncident);
  query->exec();
  query->first();
  int currentHead = 0;
  currentHead = query->value(0).toInt();
  query->prepare("SELECT tail FROM linkages WHERE head = :head AND type = :type");
  query->bindValue(":head", currentHead);
  query->bindValue(":type", selectedType);
  query->exec();
  std::vector<int> results;
  while (query->next()) {
    int currentTail = 0;
    currentTail = query->value(0).toInt();
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("SELECT ch_order FROM incidents WHERE id = :current");
    query2->bindValue(":current", currentTail);
    query2->exec();
    query2->first();
    int newIndex = 0;
    newIndex = query->value(0).toInt();
    results.push_back(newIndex);
    delete query2;
  }
  delete query;
  std::sort(results.begin(), results.end());
  std::vector<int>::iterator it;
  for (it = results.begin(); it != results.end(); it++) {
    pIgnore->push_back(*it);
    findFuturePaths(pIgnore, *it);
  }
}
