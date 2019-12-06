
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

#include "../include/AttributesWidget.h"

AttributesWidget::AttributesWidget(QWidget *parent) : QWidget(parent) 
{
  _descriptionFilter = "";
  _rawFilter = "";
  _commentFilter = "";
  _commentBool = false;
  
  incidentsModel = new QSqlTableModel(this);  
  incidentsModel->setTable("incidents");
  incidentsModel->setSort(1, Qt::AscendingOrder);
  incidentsModel->select();

  attributesModel = new QSqlTableModel(this);
  attributesModel->setTable("incident_attributes");
  attributesModel->select();
  assignedModel = new QSqlTableModel(this);
  assignedModel->setTable("attributes_to_incidents");
  assignedModel->select();
  
  attributesTreeView = new DeselectableTreeView(this);
  attributesTreeView->setHeaderHidden(true);
  attributesTreeView->setDragEnabled(true);
  attributesTreeView->viewport()->setAcceptDrops(true);
  attributesTreeView->setDropIndicatorShown(true);
  attributesTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  attributesTreeView->header()->setStretchLastSection(false);
  attributesTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  
  treeFilter = new AttributeTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);

  indexLabel = new QLabel("<b>Incident ( / )</b>", this);
  markLabel = new QLabel("", this);
  markLabel->setStyleSheet("QLabel {color: red}");
  timeStampLabel = new QLabel("<b>Timing:</b>", this);
  sourceLabel = new QLabel("<b>Source:</b>", this);
  descriptionLabel = new QLabel("<b>Description:</b>", this);
  rawLabel = new QLabel("<b>Raw:</b>", this);
  commentLabel = new QLabel("<b>Comments:</b>", this);
  attributesLabel = new QLabel("<h2>Attributes</h2>", this);
  descriptionFilterLabel = new QLabel("<i>Search descriptions:</i>", this);
  rawFilterLabel = new QLabel("<i>Search raw texts:</i>", this);
  commentFilterLabel = new QLabel("<i>Search comments:</i>", this);
  attributeFilterLabel = new QLabel("<b>Filter attributes:</b>", this);
  valueLabel = new QLabel("<b>Value:</b>", this);
  
  timeStampField = new QLineEdit(this);
  timeStampField->setReadOnly(true);
  sourceField = new QLineEdit(this);
  sourceField->setReadOnly(true);
  descriptionField = new QTextEdit(this);
  descriptionField->setReadOnly(true);
  rawField = new QTextEdit(this);
  rawField->setReadOnly(true);
  commentField = new QTextEdit(this);
  descriptionFilterField = new QLineEdit(this);
  rawFilterField = new QLineEdit(this);
  commentFilterField = new QLineEdit(this);
  attributeFilterField = new QLineEdit(this);
  valueField = new QLineEdit(this);
  valueField->setEnabled(false);

  previousIncidentButton = new QPushButton("Previous incident", this);
  previousIncidentButton->setStyleSheet("QPushButton {font-weight: bold}");
  nextIncidentButton = new QPushButton("Next incident", this);
  nextIncidentButton->setStyleSheet("QPushButton {font-weight: bold}");
  jumpButton = new QPushButton("Jump to", this);
  markButton = new QPushButton("Toggle mark", this);
  previousMarkedButton = new QPushButton("Previous marked", this);
  nextMarkedButton = new QPushButton("Next marked", this);
  descriptionPreviousButton = new QPushButton("Previous", this);
  descriptionNextButton = new QPushButton("Next", this);
  rawPreviousButton = new QPushButton("Previous", this);
  rawNextButton = new QPushButton("Next", this);
  commentPreviousButton = new QPushButton("Previous", this);
  commentNextButton = new QPushButton("Next", this);
  newAttributeButton = new QPushButton("New attribute", this);
  editAttributeButton = new QPushButton("Edit attribute", this);
  editAttributeButton->setEnabled(false);
  assignAttributeButton = new QPushButton("Assign attribute", this);
  assignAttributeButton->setEnabled(false);
  unassignAttributeButton = new QPushButton("Unassign attribute", this);
  unassignAttributeButton->setEnabled(false);
  removeUnusedAttributesButton = new QPushButton("Remove unused attributes", this);
  valueButton = new QPushButton("Store value", this);
  valueButton->setEnabled(false);
  expandTreeButton = new QPushButton("Expand all", this);
  collapseTreeButton = new QPushButton("Collapse all", this);
  previousCodedButton = new QPushButton("Previous coded", this);
  previousCodedButton->setEnabled(false);
  nextCodedButton = new QPushButton("Next coded", this);
  nextCodedButton->setEnabled(false);
  removeTextButton = new QPushButton("Remove text", this);
  removeTextButton->setEnabled(false);
  resetTextsButton = new QPushButton("Reset texts", this);
  resetTextsButton->setEnabled(false);

  attributesTreeView->installEventFilter(this);
  timeStampField->installEventFilter(this);
  sourceField->installEventFilter(this);
  descriptionField->viewport()->installEventFilter(this);  
  rawField->viewport()->installEventFilter(this);
  commentField->installEventFilter(this);
  /*
    To make possible context menu actions. This also requires the connection of a signal,
    which I do somewhere in the below.
  */
  attributesTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
 
  connect(commentField, SIGNAL(textChanged()), this, SLOT(setCommentBool()));
  connect(previousIncidentButton, SIGNAL(clicked()), this, SLOT(previousIncident()));
  connect(nextIncidentButton, SIGNAL(clicked()), this, SLOT(nextIncident()));
  connect(jumpButton, SIGNAL(clicked()), this, SLOT(jumpIncident()));
  connect(markButton, SIGNAL(clicked()), this, SLOT(toggleMark()));
  connect(previousMarkedButton, SIGNAL(clicked()), this, SLOT(previousMarked()));
  connect(nextMarkedButton, SIGNAL(clicked()), this, SLOT(nextMarked()));
  connect(descriptionFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setDescriptionFilter(const QString &)));
  connect(descriptionPreviousButton, SIGNAL(clicked()), this, SLOT(previousDescription()));
  connect(descriptionNextButton, SIGNAL(clicked()), this, SLOT(nextDescription()));
  connect(rawFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setRawFilter(const QString &)));
  connect(rawPreviousButton, SIGNAL(clicked()), this, SLOT(previousRaw()));
  connect(rawNextButton, SIGNAL(clicked()), this, SLOT(nextRaw()));
  connect(commentFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setCommentFilter(const QString &)));
  connect(commentPreviousButton, SIGNAL(clicked()), this, SLOT(previousComment()));
  connect(commentNextButton, SIGNAL(clicked()), this, SLOT(nextComment()));
  connect(newAttributeButton, SIGNAL(clicked()), this, SLOT(newAttribute()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(assignAttributeButton, SIGNAL(clicked()), this, SLOT(assignAttribute()));
  connect(unassignAttributeButton, SIGNAL(clicked()), this, SLOT(unassignAttribute()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(attributeFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(removeUnusedAttributesButton, SIGNAL(clicked()), this, SLOT(removeUnusedAttributes()));
  connect(valueField, SIGNAL(textChanged(const QString &)), this, SLOT(setValueButton()));
  connect(valueButton, SIGNAL(clicked()), this, SLOT(setValue()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(getValue()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(highlightText()));
  connect(attributesTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(attributesTreeView, SIGNAL(noneSelected()), this, SLOT(setButtons()));
  connect(attributesTreeView, SIGNAL(customContextMenuRequested(QPoint)),
	  this, SLOT(treeContextMenu(QPoint)));
  connect(expandTreeButton, SIGNAL(clicked()), this, SLOT(expandTree()));
  connect(collapseTreeButton, SIGNAL(clicked()), this, SLOT(collapseTree()));
  connect(previousCodedButton, SIGNAL(clicked()), this, SLOT(previousCoded()));
  connect(nextCodedButton, SIGNAL(clicked()), this, SLOT(nextCoded()));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(finalBusiness()));
                                                                               
  QPointer<QHBoxLayout> mainLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> indexLayout = new QHBoxLayout;
  indexLayout->addWidget(indexLabel);
  indexLabel->setAlignment(Qt::AlignHCenter);
  indexLayout->addWidget(markLabel);
  markLabel->setAlignment(Qt::AlignHCenter);
  leftLayout->addLayout(indexLayout);
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  topFieldsLayout->addWidget(timeStampLabel);
  topFieldsLayout->addWidget(timeStampField);
  topFieldsLayout->addSpacerItem(new QSpacerItem(20, 0));
  topFieldsLayout->addWidget(sourceLabel);
  topFieldsLayout->addWidget(sourceField);
  leftLayout->addLayout(topFieldsLayout);
  QPointer<QHBoxLayout> descriptionLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> descriptionLayoutLeft = new QHBoxLayout;
  descriptionLayoutLeft->addWidget(descriptionLabel);
  descriptionLayout->addLayout(descriptionLayoutLeft);
  QPointer<QHBoxLayout> descriptionLayoutRight = new QHBoxLayout;
  descriptionLayoutRight->addWidget(descriptionFilterLabel);
  descriptionFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  descriptionLayoutRight->addWidget(descriptionPreviousButton);
  descriptionPreviousButton->setMaximumWidth(descriptionPreviousButton->sizeHint().width());
  descriptionLayoutRight->addWidget(descriptionFilterField);
  descriptionLayoutRight->addWidget(descriptionNextButton);
  descriptionNextButton->setMaximumWidth(descriptionNextButton->sizeHint().width());
  descriptionLayout->addLayout(descriptionLayoutRight);
  leftLayout->addLayout(descriptionLayout);
  leftLayout->addWidget(descriptionField);
  QPointer<QHBoxLayout> rawLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> rawLayoutLeft = new QHBoxLayout;
  rawLayoutLeft->addWidget(rawLabel);
  rawLayout->addLayout(rawLayoutLeft);
  QPointer<QHBoxLayout> rawLayoutRight = new QHBoxLayout;
  rawLayoutRight->addWidget(rawFilterLabel);
  rawFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  rawLayoutRight->addWidget(rawPreviousButton);
  rawPreviousButton->setMaximumWidth(rawPreviousButton->sizeHint().width());
  rawLayoutRight->addWidget(rawFilterField);
  rawLayoutRight->addWidget(rawNextButton);
  rawNextButton->setMaximumWidth(rawNextButton->sizeHint().width());
  rawLayout->addLayout(rawLayoutRight);
  leftLayout->addLayout(rawLayout);
  leftLayout->addWidget(rawField);
  QPointer<QHBoxLayout> commentLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> commentLayoutLeft = new QHBoxLayout;
  commentLayoutLeft->addWidget(commentLabel);
  commentLayout->addLayout(commentLayoutLeft);
  QPointer<QHBoxLayout> commentLayoutRight = new QHBoxLayout;
  commentLayoutRight->addWidget(commentFilterLabel);
  commentFilterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
  commentLayoutRight->addWidget(commentPreviousButton);
  commentPreviousButton->setMaximumWidth(commentPreviousButton->sizeHint().width());
  commentLayoutRight->addWidget(commentFilterField);
  commentLayoutRight->addWidget(commentNextButton);
  commentNextButton->setMaximumWidth(commentNextButton->sizeHint().width());
  commentLayout->addLayout(commentLayoutRight);
  leftLayout->addLayout(commentLayout);
  leftLayout->addWidget(commentField);
  QPointer<QHBoxLayout> leftButtonTopLayout = new QHBoxLayout;
  leftButtonTopLayout->addWidget(previousIncidentButton);
  leftButtonTopLayout->addWidget(jumpButton);
  leftButtonTopLayout->addWidget(nextIncidentButton);
  leftLayout->addLayout(leftButtonTopLayout);
  QPointer<QHBoxLayout> leftButtonBottomLayout = new QHBoxLayout;
  leftButtonBottomLayout->addWidget(previousMarkedButton);
  leftButtonBottomLayout->addWidget(markButton);
  leftButtonBottomLayout->addWidget(nextMarkedButton);
  leftLayout->addLayout(leftButtonBottomLayout);
  mainLayout->addLayout(leftLayout);
  QPointer<QVBoxLayout> rightLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> titleLayout = new QHBoxLayout;
  titleLayout->addSpacerItem(new QSpacerItem(20, 0));
  titleLayout->addWidget(attributesLabel);
  QPointer<QHBoxLayout> collapseLayout = new QHBoxLayout;
  collapseLayout->addWidget(expandTreeButton);
  expandTreeButton->setMaximumWidth(expandTreeButton->sizeHint().width());
  collapseLayout->addWidget(collapseTreeButton);
  collapseTreeButton->setMaximumWidth(collapseTreeButton->sizeHint().width());
  titleLayout->addLayout(collapseLayout);
  rightLayout->addLayout(titleLayout);
  rightLayout->addWidget(attributesTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(attributeFilterLabel);
  filterLayout->addWidget(attributeFilterField);
  rightLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> valueLayout = new QHBoxLayout;
  valueLayout->addWidget(valueLabel);
  valueLayout->addWidget(valueField);
  valueLayout->addWidget(valueButton);
  rightLayout->addLayout(valueLayout);
  QPointer<QHBoxLayout> rightButtonTopLayout = new QHBoxLayout;
  rightButtonTopLayout->addWidget(assignAttributeButton);
  rightButtonTopLayout->addWidget(unassignAttributeButton);
  rightButtonTopLayout->addWidget(removeTextButton);
  rightButtonTopLayout->addWidget(resetTextsButton);
  rightLayout->addLayout(rightButtonTopLayout);
  QPointer<QHBoxLayout> rightButtonMiddleLayout = new QHBoxLayout;
  rightButtonMiddleLayout->addWidget(previousCodedButton);
  rightButtonMiddleLayout->addWidget(nextCodedButton);
  rightLayout->addLayout(rightButtonMiddleLayout);
  QPointer<QHBoxLayout> rightButtonBottomLayout = new QHBoxLayout;
  rightButtonBottomLayout->addWidget(newAttributeButton);
  rightButtonBottomLayout->addWidget(editAttributeButton);
  rightButtonBottomLayout->addWidget(removeUnusedAttributesButton);
  rightLayout->addLayout(rightButtonBottomLayout);
  mainLayout->addLayout(rightLayout);
  // I want to set the size of some widgets based on the availabe screen width
  QRect rect = QApplication::primaryScreen()->availableGeometry();
  int width = rect.width();
  if (width <= 1280) 
    {
      descriptionFilterField->setMaximumWidth(90);
      rawFilterField->setMaximumWidth(90);
      commentFilterField->setMaximumWidth(90);
    }
  else 
    {
      descriptionFilterField->setMaximumWidth(200);
      rawFilterField->setMaximumWidth(200);
      commentFilterField->setMaximumWidth(200);
    }
  setLayout(mainLayout);
}

void AttributesWidget::setCommentBool() 
{
  _commentBool = true;
}

void AttributesWidget::setComment() 
{
  if (_commentBool) 
    {
      QString comment = commentField->toPlainText();
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      int order = 0;
      query->first();
      order = query->value(0).toInt();
      query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :order");
      query->bindValue(":comment", comment);
      query->bindValue(":order", order);
      query->exec();
      _commentBool = false;
      delete query;
    }
}

void AttributesWidget::previousIncident() 
{
  setComment();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  if (order > 1) 
    {
      order--;
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  delete query;
}

void AttributesWidget::jumpIncident() 
{
  setComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  AttributeIndexDialog *indexDialog = new AttributeIndexDialog(this, incidentsModel->rowCount());
  indexDialog->exec();
  int order = 0;
  if (indexDialog->getExitStatus() == 0) 
    {
      order = indexDialog->getIndex();
      QSqlQuery *query = new QSqlQuery;
      if (order > 0) 
	{
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
  delete indexDialog;
}

void AttributesWidget::nextIncident() 
{
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order < incidentsModel->rowCount()) 
    {
      order++;
      query->prepare("UPDATE save_data "
		     "SET attributes_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  delete query;
}

void AttributesWidget::toggleMark() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  query->first();
  int order = 0;
  order = query->value(0).toInt();
  query->prepare("SELECT mark FROM incidents WHERE ch_order = :order");
  query->bindValue(":order", order);
  query->exec();
  query->first();
  int currentMark = 0;
  currentMark = query->value(0).toInt();
  if (currentMark == 0) 
    {
      int newMark = 1;
      query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->bindValue(":newMark", newMark);
      query->exec();
      markLabel->setText("MARKED");
    }
  else if (currentMark == 1) 
    {
      int newMark = 0;
      query->prepare("UPDATE incidents SET mark = :newMark WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->bindValue(":newMark", newMark);
      query->exec();
      markLabel->setText("");
    }
  delete query;
}

void AttributesWidget::previousMarked() 
{
  setComment();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  if (order != 1) 
    {
      query->prepare("SELECT ch_order FROM incidents WHERE ch_order < :order "
		     "AND mark = 1 ORDER BY ch_order DESC");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record = :new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	} 
    }
  delete query;
}

void AttributesWidget::nextMarked() 
{
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  int order = 0;
  query->first();
  order = query->value(0).toInt();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order != incidentsModel->rowCount()) 
    {
      query->prepare("SELECT ch_order FROM incidents WHERE ch_order > :order "
		     "AND mark = 1 ORDER BY ch_order ASC");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record = :new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	} 
    }
  delete query;
}


void AttributesWidget::setDescriptionFilter(const QString &text) 
{
  _descriptionFilter = text;
}

void AttributesWidget::previousDescription() 
{
  setComment();
  if (_descriptionFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      int order = 0;
      query->first();
      order = query->value(0).toInt();
      QString searchText = "%" + _descriptionFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order < :order "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
}
void AttributesWidget::nextDescription() 
{
  setComment();
  if (_descriptionFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      int order = 0;
      query->first();
      order = query->value(0).toInt();
      QString searchText = "%" + _descriptionFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE description LIKE :text "
		     "AND ch_order > :order "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
}

void AttributesWidget::editIncident()
{
  QPointer<RecordDialog> dialog = new RecordDialog(this);
  QString timeStamp = timeStampField->text();
  QString source  = sourceField->text();
  QString description = descriptionField->toPlainText();
  QString raw = rawField->toPlainText();
  QString comment = commentField->toPlainText();
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
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0;
      if (!query->isNull(0))
	{
	  order = query->value(0).toInt();
	}
      query->prepare("UPDATE incidents "
		     "SET timestamp = :timestamp, description = :description, "
		     "raw = :raw, comment = :comment, source = :source "
		     "WHERE ch_order = :order");
      query->bindValue(":timestamp", dialog->getTimeStamp());
      query->bindValue(":description", dialog->getDescription());
      query->bindValue(":raw", dialog->getRaw());
      query->bindValue(":comment", dialog->getComment());
      query->bindValue(":source", dialog->getSource());
      query->bindValue(":order", order);
      query->exec();
      delete query;
      retrieveData();
    }
  delete dialog;
}

void AttributesWidget::setRawFilter(const QString &text) 
{
  _rawFilter = text;
}

void AttributesWidget::previousRaw() 
{
  setComment();
  if (_rawFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      int order = 0;
      query->first();
      order = query->value(0).toInt();
      QString searchText = "%" + _rawFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order < :order "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
}

void AttributesWidget::nextRaw() 
{
  setComment();
  if (_rawFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      int order = 0;
      query->first();
      order = query->value(0).toInt();
      QString searchText = "%" + _rawFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE raw LIKE :text "
		     "AND ch_order > :order "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
}

void AttributesWidget::setCommentFilter(const QString &text) 
{
  _commentFilter = text;
}

void AttributesWidget::previousComment() 
{
  setComment();
  if (_commentFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      int order = 0;
      query->first();
      order = query->value(0).toInt();
      QString searchText = "%" + _commentFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order < :order "
		     "ORDER BY ch_order desc");
      query->bindValue(":text", searchText);
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
}

void AttributesWidget::nextComment() 
{
  setComment();
  if (_commentFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      int order = 0;
      query->first();
      order = query->value(0).toInt();
      QString searchText = "%" + _commentFilter + "%";
      query->prepare("SELECT ch_order FROM incidents "
		     "WHERE comment LIKE :text "
		     "AND ch_order > :order "
		     "ORDER BY ch_order asc");
      query->bindValue(":text", searchText);
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
}

void AttributesWidget::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void AttributesWidget::newAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString currentParent = treeFilter->
	mapToSource(attributesTreeView->currentIndex()).data().toString();
      QString name = "";
      QString description = "";
      QModelIndex tempIndex = attributesTreeView->currentIndex();
      while (tempIndex.parent().isValid()) 
	{
	  tempIndex = tempIndex.parent();
	}
      QString top = tempIndex.data().toString();
      if (top == ENTITIES) 
	{
	  EntityDialog *entityDialog = new EntityDialog(this);
	  entityDialog->setRelationshipsWidget(_relationshipsWidgetPtr);
	  entityDialog->setNew();
	  entityDialog->exec();
	  if (entityDialog->getExitStatus() == 0) 
	    {
	      QString name = entityDialog->getName();
	      QString description = entityDialog->getDescription();
	      QStandardItem *attribute = new QStandardItem(name);    
	      attribute->setToolTip(description);
	      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
	      QStandardItem *father = attributesTree->
		itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
	      QFont fatherFont = father->font();
	      int fontSize = fatherFont.pointSize();
	      QFont attributeFont = attribute->font();
	      attributeFont.setPointSize(fontSize);
	      attribute->setFont(attributeFont);
	      father->appendRow(attribute);
	      attribute->setToolTip(hint);
	      attribute->setEditable(false);
	      QString fatherName = currentParent;
	      if (fatherName == ENTITIES) 
		{
		  fatherName = "NONE";
		}
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("INSERT INTO entities (name, description, father) "
			     "VALUES (:name, :description, :father)");
	      query->bindValue(":name", name);
	      query->bindValue(":description", description);
	      query->bindValue(":father", fatherName);
	      query->exec();
	      delete query;
	    }
	  delete entityDialog;
	  _eventGraphWidgetPtr->resetTree();
	}
      else 
	{
	  QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
	  attributeDialog->exec();
	  if (attributeDialog->getExitStatus() == 0) 
	    {
	      name = attributeDialog->getName();
	      description = attributeDialog->getDescription();
	      QStandardItem *attribute = new QStandardItem(name);    
	      attribute->setToolTip(description);
	      QString hint = "<FONT SIZE = 3>" + description + "</FONT>";
	      QStandardItem *father = attributesTree->
		itemFromIndex(treeFilter->mapToSource((attributesTreeView->currentIndex())));
	      QFont fatherFont = father->font();
	      int fontSize = fatherFont.pointSize();
	      QFont attributeFont = attribute->font();
	      attributeFont.setPointSize(fontSize);
	      father->appendRow(attribute);
	      attribute->setToolTip(hint);
	      attribute->setEditable(false);
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("INSERT INTO incident_attributes (name, description, father) "
			     "VALUES (:name, :description, :father)");
	      query->bindValue(":name", name);
	      query->bindValue(":description", description);
	      query->bindValue(":father", currentParent);
	      query->exec();
	      delete query;
	      _eventGraphWidgetPtr->resetTree();
	    }
	  delete attributeDialog;
	}
    }
  else 
    {
      QString name = "";
      QString description = "";
      QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
      attributeDialog->exec();
      if (attributeDialog->getExitStatus() == 0) 
	{
	  name = attributeDialog->getName();
	  description = attributeDialog->getDescription();
	  QString currentParent = "NONE";
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("INSERT INTO incident_attributes (name, description, father) "
			 "VALUES (:name, :description, :father)");
	  query->bindValue(":name", name);
	  query->bindValue(":description", description);
	  query->bindValue(":father", currentParent);
	  query->exec();
	  QStandardItem *attribute = new QStandardItem(name);
	  QFont currentFont = attributesTree->item(0)->font();
	  int fontSize = currentFont.pointSize();
	  QFont attributeFont = attribute->font();
	  attributeFont.setPointSize(fontSize);
	  attribute->setFont(attributeFont);
	  attributesTree->appendRow(attribute);
	  QString hint = breakString(description);
	  attribute->setToolTip(hint);
	  attribute->setEditable(false);
	  _eventGraphWidgetPtr->resetTree();
	}
      delete attributeDialog;
    }
  attributesTree->sort(0, Qt::AscendingOrder);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void AttributesWidget::editAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString name = attributesTreeView->currentIndex().data().toString();
      if (name != ENTITIES) 
	{
	  QModelIndex tempIndex = attributesTreeView->currentIndex();
	  while (tempIndex.parent().isValid()) 
	    {
	      tempIndex = tempIndex.parent();
	    }
	  QString top = tempIndex.data().toString();
	  if (top == ENTITIES) 
	    {
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT description FROM entities WHERE name = :name");
	      query->bindValue(":name", name);
	      query->exec();
	      query->first();
	      QString description = query->value(0).toString();
	      EntityDialog *entityDialog = new EntityDialog(this);
	      entityDialog->setRelationshipsWidget(_relationshipsWidgetPtr);
	      entityDialog->submitName(name);
	      entityDialog->submitDescription(description);
	      entityDialog->exec();
	      if (entityDialog->getExitStatus() == 0) 
		{
		  QString newName = entityDialog->getName();
		  description = entityDialog->getDescription();
		  QStandardItem *currentAttribute = attributesTree->
		    itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
		  currentAttribute->setData(newName);
		  currentAttribute->setData(newName, Qt::DisplayRole);
		  QString hint = breakString(description);
		  currentAttribute->setToolTip(hint);
		  updateEntityAfterEdit(newName, description, name);
		}
	      delete query;
	      delete entityDialog;
	    }
	  else 
	    {
	      QSqlQuery *query = new QSqlQuery;
	      query->prepare("SELECT description FROM incident_attributes WHERE name = :name");
	      query->bindValue(":name", name);
	      query->exec();
	      query->first();
	      QString description = query->value(0).toString();
	      QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
	      attributeDialog->submitName(name);
	      attributeDialog->submitDescription(description);
	      attributeDialog->exec();
	      if (attributeDialog->getExitStatus() == 0) 
		{
		  QString newName = attributeDialog->getName();
		  description = attributeDialog->getDescription();
		  QStandardItem *currentAttribute = attributesTree->
		    itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
		  currentAttribute->setData(newName);
		  currentAttribute->setData(newName, Qt::DisplayRole);
		  QString hint = breakString(description);
		  currentAttribute->setToolTip(hint);
		  query->prepare("UPDATE incident_attributes "
				 "SET name = :newname, description = :newdescription "
				 "WHERE name = :oldname");
		  query->bindValue(":newname", newName);
		  query->bindValue(":newdescription", description);
		  query->bindValue(":oldname", name);
		  query->exec();
		  query->prepare("UPDATE incident_attributes "
				 "SET father = :newname "
				 "WHERE father = :oldname");
		  query->bindValue(":newname", newName);
		  query->bindValue(":oldname", name);
		  query->exec();
		  query->prepare("UPDATE attributes_to_incidents "
				 "SET attribute = :newname "
				 "WHERE attribute = :oldname");
		  query->bindValue(":newname", newName);
		  query->bindValue(":oldname", name);
		  query->exec();
		  query->prepare("UPDATE attributes_to_incidents_sources "
				 "SET attribute = :newname "
				 "WHERE attribute = :oldname");
		  query->bindValue(":newname", newName);
		  query->bindValue(":oldname", name);
		  query->exec();
		  query->prepare("UPDATE saved_eg_plots_attributes_to_abstract_nodes "
				 "SET attribute = :newname "
				 "WHERE attribute = :oldname");
		  query->bindValue(":newname", newName);
		  query->bindValue(":oldname", name);
		  query->exec();
		  this->setCursor(Qt::WaitCursor);
		  retrieveData();
		  this->setCursor(Qt::ArrowCursor);
		  _eventGraphWidgetPtr->resetTree();
		}
	      delete query;
	      delete attributeDialog;
	    }
	}
      attributesTree->sort(0, Qt::AscendingOrder);
      attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
    }
}  


void AttributesWidget::updateEntityAfterEdit(const QString name,
					     const QString description,
					     const QString former) 
{
  QSqlQuery *query = new QSqlQuery;
  // Update the entity itself.
  query->prepare("UPDATE entities "
		 "SET name = :name, description = :description "
		 "WHERE name = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();
  // Update the parent entities
  query->prepare("UPDATE entities "
		 "SET father = :name, description = :description "
		 "WHERE father = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();
  // Update attributes.
  query->prepare("UPDATE attributes_to_entities "
		 "SET entity = :new "
		 "WHERE entity = :old");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();
  // Update entities assigned as attributes
  query->prepare("UPDATE attributes_to_incidents "
		 "SET attribute = :new "
		 "WHERE attribute = :old ");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();
  /*
    Next up are the relationships in which the entity already participates.
    First, let us update all the relationships where the current entity is a source.
  */
  if (name != former) 
    {
      query->prepare("SELECT name, target, type "
		     "FROM entity_relationships WHERE source = :current");
      query->bindValue(":current", former);
      query->exec();
      while (query->next()) 
	{
	  QString oldRelationship = query->value(0).toString();
	  QString target = query->value(1).toString();
	  QString type = query->value(2).toString();
	  QSqlQuery *query2 = new QSqlQuery();
	  query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
	  query2->bindValue(":type", type);
	  query2->exec();
	  query2->first();
	  QString directedness = query2->value(0).toString();
	  QString arrow = "";
	  if (directedness == UNDIRECTED) 
	    {
	      arrow = "<-->";
	    }
	  else if (directedness == DIRECTED) 
	    {
	      arrow = "--->";
	    }
	  QString newRelationship = name + arrow + target;
	  query2->prepare("UPDATE entity_relationships "
			  "SET source = :source, name = :name "
			  "WHERE source = :oldSource AND name = :oldRelationship");
	  query2->bindValue(":source", name);
	  query2->bindValue(":name", newRelationship);
	  query2->bindValue(":oldSource", former);
	  query2->bindValue(":oldRelationship", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents_sources "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  delete query2;
	}
      // And then the relationships where the entity is a target.
      query->prepare("SELECT name, source, type "
		     "FROM entity_relationships WHERE target = :current");
      query->bindValue(":current", former);
      query->exec();
      while (query->next()) 
	{
	  QString oldRelationship = query->value(0).toString();
	  QString source = query->value(1).toString();
	  QString type = query->value(2).toString();
	  QSqlQuery *query2 = new QSqlQuery();
	  query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
	  query2->bindValue(":type", type);
	  query2->exec();
	  query2->first();
	  QString directedness = query2->value(0).toString();
	  QString arrow = "";
	  if (directedness == UNDIRECTED) 
	    {
	      arrow = "<-->";
	    }
	  else if (directedness == DIRECTED) 
	    {
	      arrow = "--->";
	    }
	  QString newRelationship = source + arrow + name;
	  query2->prepare("UPDATE entity_relationships "
			  "SET target = :target, name = :name "
			  "WHERE target = :oldTarget AND name = :oldRelationship");
	  query2->bindValue(":target", name);
	  query2->bindValue(":name", newRelationship);
	  query2->bindValue(":oldTarget", former);
	  query2->bindValue(":oldRelationship", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  query2->prepare("UPDATE relationships_to_incidents_sources "
			  "SET relationship = :new "
			  "WHERE relationship = :old");
	  query2->bindValue(":new", newRelationship);
	  query2->bindValue(":old", oldRelationship);
	  query2->exec();
	  delete query2;
	}
      delete query;
    }
  /* 
     If an entity is edited, we should communicate this change both to the
     relationships widget and the event graph. The tree of the Hierachy Graph Widget
     is reconstructed every time it is switched to, so we do not need to reset it
     explicitly.
  */
  _relationshipsWidgetPtr->resetTree();
  _eventGraphWidgetPtr->resetTree();
}

void AttributesWidget::selectText() 
{
  if (rawField->textCursor().selectedText().trimmed() != "") 
    {
      int end = 0;
      int begin = 0;
      QTextCursor selectCursor = rawField->textCursor();
      if (rawField->textCursor().anchor() >= rawField->textCursor().position()) 
	{
	  begin = rawField->textCursor().position();
	  end = rawField->textCursor().anchor();
	}
      else 
	{
	  begin = rawField->textCursor().anchor();
	  end = rawField->textCursor().position();
	}
      selectCursor.setPosition(begin);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      rawField->setTextCursor(selectCursor);
      while (rawField->textCursor().selectedText()[0].isSpace()) 
	{
	  begin++;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  rawField->setTextCursor(selectCursor);
	}
      while (rawField->textCursor().selectedText()[rawField->textCursor().
						   selectedText().length() - 1].isSpace()) 
	{
	  end--;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  rawField->setTextCursor(selectCursor);
	}
      selectCursor.setPosition(begin);
      selectCursor.movePosition(QTextCursor::StartOfWord);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      if (!rawField->toPlainText()[end].isPunct()) 
	{
	  selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	}
      rawField->setTextCursor(selectCursor);
    }
}

void AttributesWidget::sourceAttributeText(const QString &attribute, const int &incident) 
{
  if (rawField->textCursor().selectedText().trimmed() != "") 
    {
      QString sourceText = rawField->textCursor().selectedText().trimmed();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT attribute FROM attributes_to_incidents_sources "
		     "WHERE attribute = :att AND inc = :incident AND source_text = :text");
      query->bindValue(":att", attribute);
      query->bindValue(":inc", incident);
      query->bindValue(":text", sourceText);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO attributes_to_incidents_sources "
			 "(attribute, incident, source_text) "
			 "VALUES (:att, :inc, :text)");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", incident);
	  query->bindValue(":text", sourceText);
	  query->exec();
	}
      delete query;
    }
}
  
void AttributesWidget::highlightText() 
{
  int barPos = rawField->verticalScrollBar()->value();
  QTextCursor currentPos = rawField->textCursor();
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentAttribute = attributesTree->
	itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
      QString currentName = attributesTreeView->currentIndex().data().toString();
      QSqlQuery *query = new QSqlQuery;
      if (currentAttribute->font().bold()) 
	{
	  query->exec("SELECT attributes_record FROM save_data");
	  query->first();
	  int order = 0;
	  if (!query->isNull(0))
	    {
	      order = query->value(0).toInt();
	    }
	  query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
	  query->bindValue(":order", order);
	  query->exec();
	  query->first();
	  int id = 0;
	  id = query->value(0).toInt();
	  QTextCharFormat format;
	  format.setFontWeight(QFont::Normal);
	  format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	  rawField->selectAll();
	  rawField->textCursor().mergeCharFormat(format);
	  QTextCursor cursor = rawField->textCursor();
	  cursor.movePosition(QTextCursor::Start);
	  rawField->setTextCursor(cursor);
	  query->prepare("SELECT source_text "
			  "FROM attributes_to_incidents_sources "
			  "WHERE attribute = :attribute AND incident = :id");
	  query->bindValue(":attribute", currentName);
	  query->bindValue(":id", id);
	  query->exec();
	  while (query->next()) 
	    {
	      QString currentText = query->value(0).toString();
	      QVector<QString> blocks = splitLines(currentText);
	      QVectorIterator<QString> it(blocks);
	      while (it.hasNext()) 
		{
		  QString currentLine = it.next();
		  while (rawField->find(currentLine, QTextDocument::FindWholeWords)) 
		    {
		      format.setFontWeight(QFont::Bold);
		      format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
		      format.setUnderlineColor(Qt::blue);
		      rawField->textCursor().mergeCharFormat(format);
		    }
		}
	      cursor = rawField->textCursor();
	      cursor.movePosition(QTextCursor::Start);
	      rawField->setTextCursor(cursor);
	    }
	  rawField->setTextCursor(currentPos);
	}
      else 
	{
	  QString currentSelected = rawField->textCursor().selectedText();
	  QTextCharFormat format;
	  format.setFontWeight(QFont::Normal);
	  format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	  rawField->selectAll();
	  rawField->textCursor().mergeCharFormat(format);
	  QTextCursor cursor = rawField->textCursor();
	  cursor.movePosition(QTextCursor::Start);
	  rawField->setTextCursor(cursor);
	  rawField->find(currentSelected);
	  rawField->setTextCursor(currentPos);
	}
      delete query;
    }
  else 
    {
      QTextCharFormat format;
      format.setFontWeight(QFont::Normal);
      format.setUnderlineStyle(QTextCharFormat::NoUnderline);
      rawField->selectAll();
      rawField->textCursor().mergeCharFormat(format);
      QTextCursor cursor = rawField->textCursor();
      cursor.movePosition(QTextCursor::Start);
      rawField->setTextCursor(cursor);
      rawField->setTextCursor(currentPos);
    }
  rawField->verticalScrollBar()->setValue(barPos);
}

void AttributesWidget::assignAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      int barPos = rawField->verticalScrollBar()->value();
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0; 
      order = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!(query->isNull(0))) 
	{
	  int id = 0;
	  id = query->value(0).toInt();
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  query->prepare("SELECT attribute, incident "
			 "FROM attributes_to_incidents "
			 "WHERE attribute = :att AND incident = :inc");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", id);
	  query->exec();
	  query->first();
	  QTextCursor cursPos = rawField->textCursor();
	  if (query->isNull(0)) 
	    {
	      query->prepare("INSERT INTO attributes_to_incidents "
			     "(attribute, incident) "
			     "VALUES (:attribute, :incident)");
	      query->bindValue(":attribute", attribute);
	      query->bindValue(":incident", id);
	      query->exec();
	      sourceAttributeText(attribute, id);
	      boldSelected(attributesTree, attribute);
	      highlightText();
	      rawField->setTextCursor(cursPos);
	      valueField->setEnabled(true);
	    }
	  else 
	    {
	      sourceAttributeText(attribute, id);
	      highlightText();
	      rawField->setTextCursor(cursPos);
	    }
	  setButtons();
	}
      delete query;
      rawField->verticalScrollBar()->setValue(barPos);
    }
}

void AttributesWidget::unassignAttribute() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      int barPos = rawField->verticalScrollBar()->value();
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0;
      if (!query->isNull(0))
	{
	  order = query->value(0).toInt();
	}
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int id = 0;
      if (!(query->isNull(0))) 
	{
	  id = query->value(0).toInt();
	  QString attribute = attributesTreeView->currentIndex().data().toString();
      
	  assignedModel->select();
	  query->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			  "WHERE attribute = :att AND incident = :inc  ");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", id);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      query->prepare("DELETE FROM attributes_to_incidents "
			      "WHERE attribute = :att AND incident = :inc");
	      query->bindValue(":att", attribute);
	      query->bindValue(":inc", id);
	      query->exec();
	      query->prepare("DELETE FROM attributes_to_incidents_sources "
			      "WHERE attribute = :att AND incident = :inc");
	      query->bindValue(":att", attribute);
	      query->bindValue(":inc", id);
	      query->exec();
	      assignedModel->select();
	      resetFont(attributesTree);
	      query->exec("SELECT attribute, incident FROM attributes_to_incidents");
	      while (query->next()) 
		{
		  QString attribute = query->value(0).toString();
		  int incident = query->value(1).toInt();
		  if (incident == id) 
		    {
		      boldSelected(attributesTree, attribute);
		    }
		}
	      valueField->setText("");
	      valueField->setEnabled(false);
	      valueButton->setEnabled(false);
	      QTextCharFormat format;
	      format.setFontWeight(QFont::Normal);
	      format.setUnderlineStyle(QTextCharFormat::NoUnderline);
	      rawField->selectAll();
	      rawField->textCursor().mergeCharFormat(format);
	      rawField->setFontWeight(QFont::Normal);
	      QTextCursor cursor = rawField->textCursor();
	      cursor.movePosition(QTextCursor::Start);
	      rawField->setTextCursor(cursor);
	    }
	}
      delete query;
      rawField->verticalScrollBar()->setValue(barPos);
    }
  setButtons();
}

void AttributesWidget::removeText() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0; 
      order = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int id = 0;
      id = query->value(0).toInt();
      QString attribute = attributesTreeView->currentIndex().data().toString();
      if (rawField->textCursor().selectedText().trimmed() != "") 
	{
	  QString sourceText = rawField->textCursor().selectedText().trimmed();
	  query->prepare("DELETE FROM attributes_to_incidents_sources "
			 "WHERE attribute = :att AND incident = :inc AND source_text = :text");
	  query->bindValue(":att", attribute);
	  query->bindValue(":inc", id);
	  query->bindValue(":text", sourceText);
	  query->exec();
	}
      setButtons();
      highlightText();
      delete query;
    }
}

void AttributesWidget::resetTexts() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Resetting texts");
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("Resetting source texts cannot be undone. "
				     "Are you sure you want to proceed?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->exec("SELECT attributes_record FROM save_data");
	  query->first();
	  int order = 0; 
	  order = query->value(0).toInt();
	  query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
	  query->bindValue(":order", order);
	  query->exec();
	  query->first();
	  int id = 0;
	  if (!(query->isNull(0))) 
	    {
	      id = query->value(0).toInt();
	      QString attribute = attributesTreeView->currentIndex().data().toString();
	      query->prepare("DELETE FROM attributes_to_incidents_sources "
			     "WHERE attribute = :att AND incident = :inc");
	      query->bindValue(":att", attribute);
	      query->bindValue(":inc", id);
	      query->exec();
	    }
	  highlightText();
	  delete query;
	}
      setButtons();
      delete warningBox;
    }
}

void AttributesWidget::removeUnusedAttributes() 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  bool unfinished = true;
  QVector<AbstractNode*> abstractNodeVector = _eventGraphWidgetPtr->getAbstractNodes();
  QSet<QString> takenAttributes;
  QVectorIterator<AbstractNode*> it(abstractNodeVector);
  while (it.hasNext()) 
    {
      AbstractNode* current = it.next();
      QSet<QString> attributes = current->getAttributes();
      QSet<QString>::iterator it2;
      for (it2 = attributes.begin(); it2 != attributes.end(); it2++) 
	{
	  takenAttributes.insert(*it2);
	}
    }
  while (unfinished) 
    {
      query->exec("SELECT name FROM incident_attributes "
		  "EXCEPT SELECT attribute FROM attributes_to_incidents "
		  "EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_abstract_nodes "
		  "EXCEPT SELECT father FROM incident_attributes");
      QSet<QString> temp;
      while (query->next()) 
	{
	  QString current = query->value(0).toString();			   
	  temp.insert(current);
	}
      QSet<QString>::iterator it3;
      bool found = false;
      for (it3 = temp.begin(); it3 != temp.end(); it3++) 
	{
	  if (!takenAttributes.contains(*it3)) 
	    {
	      found = true;
	      query2->prepare("DELETE FROM incident_attributes WHERE name = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	    }
	}
      if (!found) 
	{
	  unfinished = false;
	}
    }
  unfinished =  true;
  while (unfinished) 
    {
      query->exec("SELECT name FROM entities "
		  "EXCEPT SELECT source FROM entity_relationships "
		  "EXCEPT SELECT target FROM entity_relationships "
		  "EXCEPT SELECT attribute FROM attributes_to_incidents "
		  "EXCEPT SELECT attribute FROM saved_eg_plots_attributes_to_abstract_nodes "
		  "EXCEPT SELECT attribute FROM saved_og_plots_occurrence_items "
		  "EXCEPT SELECT father FROM entities");
      QSet<QString> temp;
      while (query->next()) 
	{
	  QString current = query->value(0).toString();
	  temp.insert(current);
	}
      QSet<QString>::iterator it3;
      bool found = false;
      for (it3 = temp.begin(); it3 != temp.end(); it3++) 
	{
	  if (!takenAttributes.contains(*it3)) 
	    {
	      found = true;
	      query2->prepare("DELETE FROM entities WHERE name = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_entities WHERE entity = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents WHERE attribute = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	      query2->prepare("DELETE FROM attributes_to_incidents_sources WHERE attribute = :current");
	      query2->bindValue(":current", *it3);
	      query2->exec();
	    }
	}
      if (!found) 
	{
	  unfinished = false;
	}
    }
  this->setCursor(Qt::WaitCursor);
  attributesTreeView->setSortingEnabled(false);
  resetTree();
  _eventGraphWidgetPtr->resetTree();
  attributesTreeView->setSortingEnabled(true);
  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
  delete query;  
  delete query2;
}

void AttributesWidget::setValueButton() 
{
  valueButton->setEnabled(true);
}

void AttributesWidget::setValue() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0;
      if (!query->isNull(0))
	{
	  order = query->value(0).toInt();
	}
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!(query->isNull(0))) 
	{
	  int id = 0;
	  id = query->value(0).toInt();
	  QString attribute = attributesTreeView->currentIndex().data().toString();
	  query->prepare("UPDATE attributes_to_incidents "
			  "SET value = :val WHERE attribute = :attribute "
			  "AND incident = :id");
	  query->bindValue(":val", valueField->text());
	  query->bindValue(":attribute", attribute);
	  query->bindValue(":id", id);
	  query->exec();
	}
      delete query;
      valueButton->setEnabled(false);
    }
}

void AttributesWidget::getValue() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0;
      if (!query->isNull(0))
	{
	  order = query->value(0).toInt();
	}
      QString attribute = attributesTreeView->currentIndex().data().toString();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int id = 0;
      if (!query->isNull(0))
	{
	  id = query->value(0).toInt();
	}
      query->prepare("SELECT attribute, value FROM attributes_to_incidents "
		      "WHERE incident = :id AND attribute = :att");
      query->bindValue(":id", id);
      query->bindValue(":att", attribute);
      query->exec();
      query->first();
      if (!(query->isNull(0))) 
	{
	  valueField->setEnabled(true);
	}
      else 
	{
	  valueField->setEnabled(false);
	}
      if (!(query->isNull(1))) 
	{
	  QString value = query->value(1).toString();
	  valueField->setText(value);
	}
      else 
	{
	  valueField->setText("");
	}
      valueButton->setEnabled(false);
      delete query;
    }
  else 
    {
      valueField->setText("");
      valueButton->setEnabled(false);
    }
}

void AttributesWidget::retrieveData() 
{
  valueField->blockSignals(true);
  valueField->setText("");
  valueField->setEnabled(false);
  valueField->blockSignals(false);
  valueButton->setEnabled(false);
  attributesTreeView->clearSelection();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  query->first();
  int order = 0;
  if (!query->isNull(0))
    {
      order = query->value(0).toInt();
    }
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  int total = incidentsModel->rowCount();
  QString indexText = "<b>Incident (" + QString::number(order) + " / " +
    QString::number(total) + ")<b>";
  indexLabel->setText(indexText);
  query->prepare("SELECT id, timestamp, source, description, raw, comment, mark FROM "
		  "incidents WHERE ch_order = :order");
  query->bindValue(":order", order);
  query->exec();
  query->first();
  if (!query->isNull(0)) 
    {
      timeStampField->setEnabled(true);
      sourceField->setEnabled(true);
      descriptionField->setEnabled(true);
      rawField->setEnabled(true);
      commentField->setEnabled(true);
      int id = query->value(0).toInt();
      QString timeStamp = query->value(1).toString();
      QString source = query->value(2).toString();
      QString description = query->value(3).toString();
      QString raw = query->value(4).toString();
      QString comment = query->value(5).toString();
      int mark = query->value(6).toInt();
      timeStampField->setText(timeStamp);
      sourceField->setText(source);
      descriptionField->setText(description);
      rawField->setText(raw);
      commentField->blockSignals(true);
      commentField->setText(comment);
      commentField->blockSignals(false);
      if (mark == 0) 
	{
	  markLabel->setText("");
	}
      else 
	{
	  markLabel->setText("MARKED");
	}
      resetFont(attributesTree);
      query->exec("SELECT attribute, incident FROM attributes_to_incidents");
      while (query->next()) 
	{
	  QString attribute = query->value(0).toString();
	  int incident = query->value(1).toInt();
	  if (incident == id) 
	    {
	      boldSelected(attributesTree, attribute);
	    }
	}
      attributesTreeView->setModel(treeFilter);
      attributesTreeView->resetSelection();
      attributesTree->sort(0, Qt::AscendingOrder);
      attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
      QTextCharFormat format;
      format.setFontWeight(QFont::Normal);
      format.setUnderlineStyle(QTextCharFormat::NoUnderline);
      rawField->selectAll();
      rawField->textCursor().mergeCharFormat(format);
      QTextCursor cursor = rawField->textCursor();
      cursor.movePosition(QTextCursor::Start);
      rawField->setTextCursor(cursor);
      delete query;
    }
  else
    {
      timeStampField->clear();
      timeStampField->setEnabled(false);
      sourceField->clear();
      sourceField->setEnabled(false);
      descriptionField->clear();
      descriptionField->setEnabled(false);
      rawField->clear();
      rawField->setEnabled(false);
      commentField->clear();
      commentField->setEnabled(false);
    }
}

void AttributesWidget::expandTree() 
{
  attributesTreeView->expandAll();
}

void AttributesWidget::collapseTree() 
{
  attributesTreeView->collapseAll();
}

void AttributesWidget::previousCoded() 
{
  setComment();
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QPersistentModelIndex currentIndex = attributesTreeView->currentIndex();
      QString attribute = attributesTreeView->currentIndex().data().toString();
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int currentOrder = query->value(0).toInt();
      QVector<QString> attributeVector;
      attributeVector.push_back(attribute);
      QModelIndex oldIndex = attributesTreeView->currentIndex();
      while (oldIndex.parent().isValid()) 
	{
	  oldIndex = oldIndex.parent();
	}
      bool entity = false;
      if (oldIndex.data().toString() == "Entities") 
	{
	  entity = true;
	}
      if (attribute == ENTITIES) 
	{
	  attribute = "NONE";
	}
      findChildren(attribute, &attributeVector, entity);
      QVectorIterator<QString> it(attributeVector);
      int order = -1;
      while (it.hasNext()) 
	{
	  QString currentAttribute = it.next();
	  query->prepare("SELECT ch_order FROM "
			 "(SELECT incident, ch_order, attribute FROM attributes_to_incidents "
			 "LEFT JOIN incidents ON attributes_to_incidents.incident = incidents.id "
			 "WHERE ch_order < :order AND attribute = :attribute)"
			 "ORDER BY ch_order desc");
	  query->bindValue(":order", currentOrder);
	  query->bindValue(":attribute", currentAttribute);
	  query->exec();
	  query->first();
	  if (order == -1 && !(query->isNull(0))) 
	    {
	      order = query->value(0).toInt();
	    }
	  else if (!(query->isNull(0)) && query->value(0).toInt() > order) 
	    {
	      order = query->value(0).toInt();
	    }
	}
      if (order != -1) 
	{
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
      attributesTreeView->setCurrentIndex(currentIndex);
      attributesTreeView->selectionModel()->select(currentIndex,
						   QItemSelectionModel::SelectCurrent);
    }
}

void AttributesWidget::nextCoded() 
{
  setComment();
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QPersistentModelIndex currentIndex = attributesTreeView->currentIndex();
      QString attribute = attributesTreeView->currentIndex().data().toString();
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int currentOrder = query->value(0).toInt();
      QVector<QString> attributeVector;
      attributeVector.push_back(attribute);
      QModelIndex oldIndex = attributesTreeView->currentIndex();
      while (oldIndex.parent().isValid()) 
	{
	  oldIndex = oldIndex.parent();
	}
      bool entity = false;
      if (oldIndex.data().toString() == "Entities") 
	{
	  entity = true;
	}
      if (attribute == ENTITIES) 
	{
	  attribute = "NONE";
	}
      findChildren(attribute, &attributeVector, entity);
      QVectorIterator<QString> it(attributeVector);
      int order = -1;
      while (it.hasNext()) 
	{
	  QString currentAttribute = it.next();
	  query->prepare("SELECT ch_order FROM "
			 "(SELECT incident, ch_order, attribute FROM attributes_to_incidents "
			 "LEFT JOIN incidents ON attributes_to_incidents.incident = incidents.id "
			 "WHERE ch_order > :order AND attribute = :attribute)"
			 "ORDER BY ch_order asc");
	  query->bindValue(":order", currentOrder);
	  query->bindValue(":attribute", currentAttribute);
	  query->exec();
	  query->first();
	  if (order == -1 && !(query->isNull(0))) 
	    {
	      order = query->value(0).toInt();
	    }
	  else if (!(query->isNull(0)) && query->value(0).toInt() < order) 
	    {
	      order = query->value(0).toInt();
	    }
	}
      if (order != -1) 
	{
	  query->prepare("UPDATE save_data "
			 "SET attributes_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
      attributesTreeView->setCurrentIndex(currentIndex);
      attributesTreeView->selectionModel()->select(currentIndex,
						   QItemSelectionModel::SelectCurrent);
    }
}

void AttributesWidget::findChildren(QString father, QVector<QString> *children, bool entity) 
{
  QSqlQuery *query = new QSqlQuery;
  if (entity) 
    {
      query->prepare("SELECT name FROM entities WHERE father = :father");
    }
  else 
    {
      query->prepare("SELECT name FROM incident_attributes WHERE father = :father");
    }
  query->bindValue(":father", father);
  query->exec();
  while (query->next()) 
    {
      QString currentChild = query->value(0).toString();
      children->push_back(currentChild);
      findChildren(currentChild, children, entity);
    }  
  delete query;
}

void AttributesWidget::setButtons() 
{
  if (attributesTreeView->currentIndex().isValid()) 
    {
      QString currentAttribute = attributesTreeView->currentIndex().data().toString();
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int order = 0;
      order = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      if (!(query->isNull(0))) 
	{
	  int id = query->value(0).toInt();
	  assignedModel->select();
	  query->prepare("SELECT attribute, incident FROM "
			 "attributes_to_incidents "
			 "WHERE attribute = :att AND incident = :inc  ");
	  query->bindValue(":att", currentAttribute);
	  query->bindValue(":inc", id);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      unassignAttributeButton->setEnabled(true);
	    }
	  else 
	    {
	      unassignAttributeButton->setEnabled(false);
	    }
	  query->prepare("SELECT attribute, incident FROM "
			 "attributes_to_incidents_sources "
			 "WHERE attribute = :att AND incident = :inc");
	  query->bindValue(":att", currentAttribute);
	  query->bindValue(":inc", id);
	  query->exec();
	  query->first();
	  if (!query->isNull(0)) 
	    {
	      removeTextButton->setEnabled(true);
	      resetTextsButton->setEnabled(true);
	    }
	  else 
	    {
	      removeTextButton->setEnabled(false);
	      resetTextsButton->setEnabled(false);
	    }
	  if (currentAttribute != ENTITIES) 
	    {
	      assignAttributeButton->setEnabled(true);
	    }
	  else 
	    {
	      assignAttributeButton->setEnabled(false);
	    }
	  previousCodedButton->setEnabled(true);
	  nextCodedButton->setEnabled(true);
	}
      editAttributeButton->setEnabled(true);
      delete query;
    }
  else 
    {
      assignAttributeButton->setEnabled(false);
      previousCodedButton->setEnabled(false);
      nextCodedButton->setEnabled(false);
      editAttributeButton->setEnabled(false);
      unassignAttributeButton->setEnabled(false);
      removeTextButton->setEnabled(false);
      resetTextsButton->setEnabled(false);
    }
}

void AttributesWidget::setTree() 
{
  attributesTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  // First we will fetch the 'normal' attributes.
  query->exec("SELECT name, description FROM incident_attributes WHERE father = 'NONE'");
  while (query->next()) 
    {
      QString name = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *father = new QStandardItem(name);    
      attributesTree->appendRow(father);
      QString hint = breakString(description);
      father->setToolTip(hint);
      father->setEditable(false);
      buildHierarchy(father, name);
    }
  // And then we will also fetch the entities.
  QStandardItem *entities = new QStandardItem(ENTITIES);
  QString entitiesHint = breakString("You can also assign entities that you have created "
				     "in the relationships widget as attributes.");
  entities->setToolTip(entitiesHint);
  QFont font;
  font.setItalic(true);
  attributesTree->appendRow(entities);
  entities->setFont(font);
  query->exec("SELECT name, description FROM entities WHERE father = 'NONE'");
  int children = 0;
  while (query->next()) 
    {
      QString name = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *father = new QStandardItem(name);
      entities->setChild(children, father);
      children++;
      QString hint = breakString(description);
      father->setToolTip(hint);
      father->setEditable(false);
      buildEntities(father, name);
    }    
  treeFilter->setSourceModel(attributesTree);
  attributesTreeView->setModel(treeFilter);
  delete query;
}

void AttributesWidget::buildHierarchy(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM incident_attributes WHERE  father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) 
    {
      QString childName = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *child = new QStandardItem(childName);
      top->setChild(children, child);
      QString hint = breakString(description);
      child->setToolTip(hint);
      child->setEditable(false);
      children++;
      buildHierarchy(child, childName);
    }
  delete query;
}

void AttributesWidget::buildEntities(QStandardItem *top, QString name) 
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT name, description FROM entities WHERE father = :father");
  query->bindValue(":father", name);
  query->exec();
  int children = 0;
  while (query->next()) 
    {
      QString childName = query->value(0).toString();
      QString description = query->value(1).toString();
      QStandardItem *child = new QStandardItem(childName);
      top->setChild(children, child);
      QString hint = breakString(description);
      child->setToolTip(hint);
      child->setEditable(false);
      children++;
      buildEntities(child, childName);
    }
  delete query;
}

void AttributesWidget::treeContextMenu(const QPoint &pos) 
{
  QPoint globalPos = attributesTreeView->mapToGlobal(pos);
  QModelIndex targetIndex = attributesTreeView->indexAt(pos);
  QString selected = targetIndex.data().toString();
  if (!targetIndex.isValid())
    {
      QMenu menu;
      QAction *action1 = new QAction(ADDATTRIBUTEACTION, this);
      menu.addAction(action1);
      if (QAction *action = menu.exec(globalPos)) 
	{
	  if (action->text() == ADDATTRIBUTEACTION) 
	    {
	      attributesTreeView->clearSelection();
	      const QModelIndex index;
	      attributesTreeView->selectionModel()->
		setCurrentIndex(index, QItemSelectionModel::Select);
	      newAttribute();
	    }
	}
      return;
    }
  if (selected == ENTITIES) 
    {
      QMenu menu;
      QAction *action1 = new QAction(ADDATTRIBUTEACTION, this);
      QAction *action2 = new QAction(EDITATTRIBUTEACTION, this);
      QAction *action3 = new QAction(AUTOASSIGNALLACTION, this);
      QAction *action4 = new QAction(UNASSIGNALLACTION, this);
      menu.addAction(action1);
      menu.addAction(action2);
      menu.addAction(action3);
      menu.addAction(action4);
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2 = new QSqlQuery;
      query->exec("SELECT name FROM entities");
      query2->prepare("SELECT attribute FROM attributes_to_incidents "
		      "WHERE attribute = :entity");
      bool found = false;
      while (query->next()) 
	{
	  QString currentEntity = query->value(0).toString();
	  query2->bindValue(":entity", currentEntity);
	  query2->exec();
	  query2->first();
	  if (!query2->isNull(0)) 
	    {
	      found = true;
	    }
	}
      if (!found) 
	{
	  action4->setEnabled(false);
	}
      delete query;
      delete query2;
      if (QAction *action = menu.exec(globalPos)) 
	{
	  if (action->text() == ADDATTRIBUTEACTION) 
	    {
	      newAttribute();
	    }
	  else if (action->text() == EDITATTRIBUTEACTION)
	    {
	      editAttribute();
	    }
	  else if (action->text() == AUTOASSIGNALLACTION) 
	    {
	      autoAssignAll();
	    }
	  else if (action->text() == UNASSIGNALLACTION) 
	    {
	      unassignAllEntities();
	    }
	}
    }
  else 
    {
      QModelIndex tempIndex = attributesTreeView->selectionModel()->currentIndex();
      while (tempIndex.parent().isValid()) 
	{
	  tempIndex = tempIndex.parent();
	}
      QString topName = tempIndex.data().toString();
      if (topName == ENTITIES) 
	{
	  QMenu menu;
	  QAction *action1 = new QAction(ADDATTRIBUTEACTION, this);
	  QAction *action2 = new QAction(EDITATTRIBUTEACTION, this);
	  QAction *action3 = new QAction(ASSIGNATTRIBUTEACTION, this);
	  QAction *action4 = new QAction(UNASSIGNATTRIBUTEACTION, this);
	  QAction *action5 = new QAction(AUTOASSIGNSPECIFICACTION, this);
	  QAction *action6 = new QAction(UNASSIGNALLACTION, this);
	  menu.addAction(action1);
	  menu.addAction(action2);
	  menu.addAction(action3);
	  menu.addAction(action4);
	  menu.addAction(action5);
	  menu.addAction(action6);
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE attribute = :selected");
	  query->bindValue(":selected", selected);
	  query->exec();
	  query->first();
	  if (query->isNull(0)) 
	    {
	      action6->setEnabled(false);
	    }
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->exec("SELECT attributes_record FROM save_data");
	  query2->first();
	  int order = query2->value(0).toInt();
	  query2->prepare("SELECT id FROM incidents WHERE ch_order = :order");
	  query2->bindValue(":order", order);
	  query2->exec();
	  query2->first();
	  int id = 0;
	  if (!query2->isNull(0))
	    {
	      id = query2->value(0).toInt();
	    }
	  query2->prepare("SELECT attribute FROM attributes_to_incidents "
			  "WHERE attribute = :entity AND incident = :id");
	  query2->bindValue(":entity", selected);
	  query2->bindValue(":id", id);
	  query2->exec();
	  query2->first();
	  if (query2->isNull(0)) 
	    {
	      action4->setEnabled(false);
	    }
	  delete query;
	  delete query2;
	  if (QAction *action = menu.exec(globalPos)) 
	    {
	      if (action->text() == ADDATTRIBUTEACTION) 
		{
		  newAttribute();
		}
	      else if (action->text() == EDITATTRIBUTEACTION)
		{
		  editAttribute();
		}
	      else if (action->text() == ASSIGNATTRIBUTEACTION) 
		{
		  assignAttribute();
		}
	      else if (action->text() == UNASSIGNATTRIBUTEACTION) 
		{
		  unassignAttribute();
		}
	      else if (action->text() == AUTOASSIGNSPECIFICACTION) 
		{
		  autoAssignEntityAt(targetIndex);
		}
	      else if (action->text() == UNASSIGNALLACTION) 
		{
		  unassignAllAttribute(targetIndex);
		}
	    }
	}
      else 
	{
	  QMenu menu;
	  QAction *action1 = new QAction(ADDATTRIBUTEACTION, this);
	  QAction *action2 = new QAction(EDITATTRIBUTEACTION, this);
	  QAction *action3 = new QAction(ASSIGNATTRIBUTEACTION, this);
	  QAction *action4 = new QAction(UNASSIGNATTRIBUTEACTION, this);
	  QAction *action5 = new QAction(UNASSIGNALLACTION, this);
	  QAction *action6 = new QAction(MERGEATTRIBUTESACTION, this);
	  menu.addAction(action1);
	  menu.addAction(action2);
	  menu.addAction(action3);
	  menu.addAction(action4);
	  menu.addAction(action5);
	  menu.addAction(action6);
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT attribute FROM attributes_to_incidents "
			 "WHERE attribute = :selected");
	  query->bindValue(":selected", selected);
	  query->exec();
	  query->first();
	  if (query->isNull(0)) 
	    {
	      action5->setEnabled(false);
	    }
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->exec("SELECT attributes_record FROM save_data");
	  query2->first();
	  int order = query2->value(0).toInt();
	  query2->prepare("SELECT id FROM incidents WHERE ch_order = :order");
	  query2->bindValue(":order", order);
	  query2->exec();
	  query2->first();
	  int id = 0;
	  if (!query2->isNull(0))
	    {
	      id = query2->value(0).toInt();
	    }
	  query2->prepare("SELECT attribute FROM attributes_to_incidents "
			  "WHERE attribute = :attribute AND incident = :id");
	  query2->bindValue(":attribute", selected);
	  query2->bindValue(":id", id);
	  query2->exec();
	  query2->first();
	  if (query2->isNull(0)) 
	    {
	      action4->setEnabled(false);
	    }
	  delete query;
	  delete query2;
	  if (QAction *action = menu.exec(globalPos)) 
	    {
	      if (action->text() == ADDATTRIBUTEACTION) 
		{
		  newAttribute();
		}
	      else if (action->text() == EDITATTRIBUTEACTION)
		{
		  editAttribute();
		}
	      else if (action->text() == ASSIGNATTRIBUTEACTION) 
		{
		  assignAttribute();
		}
	      else if (action->text() == UNASSIGNATTRIBUTEACTION) 
		{
		  unassignAttribute();
		}
	      else if (action->text() == UNASSIGNALLACTION) 
		{
		  unassignAllAttribute(targetIndex);
		}
	      else if (action->text() == MERGEATTRIBUTESACTION) 
		{
		  mergeAttributes(targetIndex);
		}
	    }
	} 
    }
}

void AttributesWidget::autoAssignAll() 
{
  QSqlQuery *query = new QSqlQuery;
  QPointer<RelationshipComboBoxDialog> comboDialog = new RelationshipComboBoxDialog(this);
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) 
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);
      QString selectedRelationship = comboDialog->getSelection();
      bool tail = comboDialog->tailSelected();
      bool head = comboDialog->headSelected();
      QVector<QString> entities;
      query->exec("SELECT name FROM entities");
      while (query->next()) 
	{
	  QString currentEntity = query->value(0).toString();
	  entities.push_back(currentEntity);
	}
      QVectorIterator<QString> it(entities);
      while (it.hasNext()) 
	{
	  QString selected = it.next();
	  QVector<QString> valid;
	  if (tail) 
	    {
	      query->prepare("SELECT name FROM entity_relationships "
			     "WHERE type = :type AND source = :entity");
	      query->bindValue(":type", selectedRelationship);
	      query->bindValue(":entity", selected);
	      query->exec();
	      while (query->next()) 
		{
		  QString current = query->value(0).toString();
		  valid.push_back(current);
		}
	    }
	  if (head) 
	    {
	      query->prepare("SELECT name FROM entity_relationships "
			     "WHERE type = :type AND target = :entity");
	      query->bindValue(":type", selectedRelationship);
	      query->bindValue(":entity", selected);
	      query->exec();
	      while (query->next()) 
		{
		  QString current = query->value(0).toString();
		  valid.push_back(current);
		}
	    }
	  QVectorIterator<QString> it2(valid);
	  while (it2.hasNext()) 
	    {
	      QString current = it2.next();
	      query->prepare("SELECT incident FROM relationships_to_incidents "
			     "WHERE type = :type AND relationship = :name");
	      query->bindValue(":type", selectedRelationship);
	      query->bindValue(":name", current);
	      query->exec();
	      while (query->next()) 
		{
		  int incident = query->value(0).toInt();
		  QSqlQuery *query2 = new QSqlQuery;
		  query2->prepare("SELECT attribute, incident FROM attributes_to_incidents "
				  "WHERE attribute = :attribute AND incident = :incident");
		  query2->bindValue(":attribute", selected);
		  query2->bindValue(":incident", incident);
		  query2->exec();
		  query2->first();
		  bool found = false;
		  if (!query2->isNull(0)) 
		    {
		      found = true;
		    }
		  if (!found) 
		    {
		      query2->prepare("INSERT INTO attributes_to_incidents (attribute, incident) "
				      "VALUES (:attribute, :incident)");
		      query2->bindValue(":attribute", selected);
		      query2->bindValue(":incident", incident);
		      query2->exec();
		    }
		  delete query2;
		}
	      query->prepare("SELECT incident, source_text FROM relationships_to_incidents_sources "
			     "WHERE type = :type AND relationship = :name");
	      query->bindValue(":type", selectedRelationship);
	      query->bindValue(":name", current);
	      query->exec();
	      while (query->next()) 
		{
		  int incident = query->value(0).toInt();
		  QString sourceText = query->value(1).toString();
		  QSqlQuery *query2 = new QSqlQuery;
		  query2->prepare("SELECT attribute, incident FROM attributes_to_incidents_sources "
				  "WHERE attribute = :attribute AND incident = :incident "
				  "AND source_text = :sourceText");
		  query2->bindValue(":attribute", selected);
		  query2->bindValue(":incident", incident);
		  query2->bindValue(":sourceText", sourceText);
		  query2->exec();
		  query2->first();
		  bool found = false;
		  if (!query2->isNull(0)) 
		    {
		      found = true;
		    }
		  if (!found) 
		    {
		      query2->prepare("INSERT INTO attributes_to_incidents_sources "
				      "(attribute, incident, source_text) "
				      "VALUES (:attribute, :incident, :source_text)");
		      query2->bindValue(":attribute", selected);
		      query2->bindValue(":incident", incident);
		      query2->bindValue(":source_text", sourceText);
		      query2->exec();
		    }
		  delete query2;
		}
	    }
	}
    }
  resetFont(attributesTree);
  query->exec("SELECT attributes_record FROM save_data");
  query->first();
  int currentOrder = query->value(0).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :currentOrder");
  query->bindValue(":currentOrder", currentOrder);
  query->exec();
  query->first();
  int currentIncident = query->value(0).toInt();
  resetFont(attributesTree);
  query->prepare("SELECT attribute FROM attributes_to_incidents "
		 "WHERE incident = :incident");
  query->bindValue(":incident", currentIncident);
  query->exec();
  while (query->next()) 
    {
      QString currentAttribute = query->value(0).toString();
      boldSelected(attributesTree, currentAttribute);
    }
  delete query;
  setButtons();
  highlightText();
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void AttributesWidget::autoAssignEntityAt(QModelIndex &index) 
{
  QString selected = index.data().toString();
  QSqlQuery *query = new QSqlQuery;
  QPointer<RelationshipComboBoxDialog> comboDialog = new RelationshipComboBoxDialog(this);
  comboDialog->exec();
  if (comboDialog->getExitStatus() == 0) 
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);
      QString selectedRelationship = comboDialog->getSelection();
      bool tail = comboDialog->tailSelected();
      bool head = comboDialog->headSelected();
      QVector<QString> valid;
      if (tail) 
	{
	  query->prepare("SELECT name FROM entity_relationships "
			 "WHERE type = :type AND source = :entity");
	  query->bindValue(":type", selectedRelationship);
	  query->bindValue(":entity", selected);
	  query->exec();
	  while (query->next()) 
	    {
	      QString current = query->value(0).toString();
	      valid.push_back(current);
	    }
	}
      if (head) 
	{
	  query->prepare("SELECT name FROM entity_relationships "
			 "WHERE type = :type AND target = :entity");
	  query->bindValue(":type", selectedRelationship);
	  query->bindValue(":entity", selected);
	  query->exec();
	  while (query->next()) 
	    {
	      QString current = query->value(0).toString();
	      valid.push_back(current);
	    }
	}
      QVectorIterator<QString> it(valid);
      while (it.hasNext()) 
	{
	  QString current = it.next();
	  query->prepare("SELECT incident FROM relationships_to_incidents "
			 "WHERE type = :type AND relationship = :name");
	  query->bindValue(":type", selectedRelationship);
	  query->bindValue(":name", current);
	  query->exec();
	  while (query->next()) 
	    {
	      int incident = query->value(0).toInt();
	      QSqlQuery *query2 = new QSqlQuery;
	      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents "
			      "WHERE attribute = :attribute AND incident = :incident");
	      query2->bindValue(":attribute", selected);
	      query2->bindValue(":incident", incident);
	      query2->exec();
	      query2->first();
	      bool found = false;
	      if (!query2->isNull(0)) 
		{
		  found = true;
		}
	      if (!found) 
		{
		  query2->prepare("INSERT INTO attributes_to_incidents (attribute, incident) "
				  "VALUES (:attribute, :incident)");
		  query2->bindValue(":attribute", selected);
		  query2->bindValue(":incident", incident);
		  query2->exec();
		}
	      delete query2;
	    }
	  query->prepare("SELECT incident, source_text FROM relationships_to_incidents_sources "
			 "WHERE type = :type AND relationship = :name");
	  query->bindValue(":type", selectedRelationship);
	  query->bindValue(":name", current);
	  query->exec();
	  while (query->next()) 
	    {
	      int incident = query->value(0).toInt();
	      QString sourceText = query->value(1).toString();
	      QSqlQuery *query2 = new QSqlQuery;
	      query2->prepare("SELECT attribute, incident FROM attributes_to_incidents_sources"
			      "WHERE attribute = :attribute AND incident = :incident "
			      "AND source_text = :sourceText");
	      query2->bindValue(":attribute", selected);
	      query2->bindValue(":incident", incident);
	      query2->bindValue(":sourceText", sourceText);
	      query2->exec();
	      query2->first();
	      bool found = false;
	      if (!query2->isNull(0)) 
		{
		  found = true;
		}
	      if (!found) 
		{
		  query2->prepare("INSERT INTO attributes_to_incidents_sources "
				  "(attribute, incident, source_text) "
				  "VALUES (:attribute, :incident, :source_text)");
		  query2->bindValue(":attribute", selected);
		  query2->bindValue(":incident", incident);
		  query2->bindValue(":source_text", sourceText);
		  query2->exec();
		}
	      delete query2;
	    }
	}
    }
  resetFont(attributesTree);
  query->exec("SELECT attributes_record FROM save_data");
  query->first();
  int currentOrder = query->value(0).toInt();
  query->prepare("SELECT id FROM incidents WHERE ch_order = :currentOrder");
  query->bindValue(":currentOrder", currentOrder);
  query->exec();
  query->first();
  int currentIncident = query->value(0).toInt();
  resetFont(attributesTree);
  query->prepare("SELECT attribute FROM attributes_to_incidents "
		 "WHERE incident = :incident");
  query->bindValue(":incident", currentIncident);
  query->exec();
  while (query->next()) 
    {
      QString currentAttribute = query->value(0).toString();
      boldSelected(attributesTree, currentAttribute);
    }
  delete query;
  setButtons();
  highlightText();
  QApplication::restoreOverrideCursor();
  qApp->processEvents();
}

void AttributesWidget::unassignAllEntities() 
{
  QPointer<QMessageBox> warningBox = new QMessageBox(this);
  warningBox->setWindowTitle("Unassiging entities");
  warningBox->addButton(QMessageBox::Yes);
  warningBox->addButton(QMessageBox::No);
  warningBox->setIcon(QMessageBox::Warning);
  warningBox->setText("<h2>Are you sure?</h2>");
  warningBox->setInformativeText("This will unassign all entities from all incidents. "
				 "Are you sure you want to proceed?");
  if (warningBox->exec() == QMessageBox::Yes) 
    {
      QSqlQuery *query = new QSqlQuery;
      QSqlQuery *query2 = new QSqlQuery;
      QSqlQuery *query3 = new QSqlQuery;
      query->exec("SELECT name FROM entities");
      query2->prepare("DELETE FROM attributes_to_incidents "
		      "WHERE attribute = :entity");
      query3->prepare("DELETE FROM attributes_to_incidents_sources "
		      "WHERE attribute = :entity");
      while (query->next()) 
	{
	  QString currentEntity = query->value(0).toString();
	  query2->bindValue(":entity", currentEntity);
	  query2->exec();
	  query3->bindValue(":entity", currentEntity);
	  query3->exec();
	}
      query->exec();
      resetFont(attributesTree);
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int currentOrder = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :currentOrder");
      query->bindValue(":currentOrder", currentOrder);
      query->exec();
      query->first();
      int currentIncident = query->value(0).toInt();
      resetFont(attributesTree);
      query->prepare("SELECT attribute FROM attributes_to_incidents "
		     "WHERE incident = :incident");
      query->bindValue(":incident", currentIncident);
      query->exec();
      while (query->next()) 
	{
	  QString currentAttribute = query->value(0).toString();
	  boldSelected(attributesTree, currentAttribute);
	}
      delete query;
      delete query2;
      delete query3;
    }
}

void AttributesWidget::unassignAllAttribute(QModelIndex &index) 
{
  QString selected = index.data().toString();
  QPointer<QMessageBox> warningBox = new QMessageBox(this);
  warningBox->setWindowTitle("Unassigning attributes");
  warningBox->addButton(QMessageBox::Yes);
  warningBox->addButton(QMessageBox::No);
  warningBox->setIcon(QMessageBox::Warning);
  warningBox->setText("<h2>Are you sure?</h2>");
  warningBox->setInformativeText("This will unassign the selected attribute from all incidents. "
				 "Are you sure you want to proceed?");
  if (warningBox->exec() == QMessageBox::Yes) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("DELETE FROM attributes_to_incidents "
		     "WHERE attribute = :attribute");
      query->bindValue(":attribute", selected);
      query->exec();
      query->prepare("DELETE FROM attributes_to_incidents_sources "
		     "WHERE attribute = :attribute");
      query->bindValue(":attribute", selected);
      query->exec("SELECT attributes_record FROM save_data");
      query->first();
      int currentOrder = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :currentOrder");
      query->bindValue(":currentOrder", currentOrder);
      query->exec();
      query->first();
      int currentIncident = query->value(0).toInt();
      resetFont(attributesTree);
      query->prepare("SELECT attribute FROM attributes_to_incidents "
		     "WHERE incident = :incident");
      query->bindValue(":incident", currentIncident);
      query->exec();
      while (query->next()) 
	{
	  QString currentAttribute = query->value(0).toString();
	  boldSelected(attributesTree, currentAttribute);
	}
      delete query;
    }
}

void AttributesWidget::mergeAttributes(QModelIndex &index) 
{
  QString origin = index.data().toString();
  if (origin != ENTITIES) 
    {
      QModelIndex tempIndex = attributesTreeView->currentIndex();
      while (tempIndex.parent().isValid()) 
	{
	  tempIndex = tempIndex.parent();
	}
      QString top = tempIndex.data().toString();
      if (top != ENTITIES) 
	{
	  QPointer<MergeAttributesDialog> mergeDialog =
	    new MergeAttributesDialog(this, origin, INCIDENT);
	  mergeDialog->setWindowTitle("Select attribute to merge with");
	  mergeDialog->exec();
	  if (mergeDialog->getExitStatus() == 0) 
	    {
	      QPointer<QMessageBox> warningBox = new QMessageBox(this);
	      warningBox->setWindowTitle("Merging attributes");
	      warningBox->addButton(QMessageBox::Yes);
	      warningBox->addButton(QMessageBox::No);
	      warningBox->setIcon(QMessageBox::Warning);
	      warningBox->setText("<h2>Are you sure?</h2>");
	      warningBox->setInformativeText("Merging attributes cannot be undone. "
					     "Are you sure you want to proceed?");
	      if (warningBox->exec() == QMessageBox::Yes) 
		{
		  QString partner = mergeDialog->getAttribute();
		  QSqlQuery *query = new QSqlQuery;
		  query->prepare("UPDATE incident_attributes SET father = :new "
				 "WHERE father = :old");
		  query->bindValue(":new", partner);
		  query->bindValue(":old", origin);
		  query->exec();
		  query->prepare("DELETE FROM incident_attributes WHERE name = :old");
		  query->bindValue(":old", origin);
		  query->exec();
		  query->prepare("UPDATE attributes_to_incidents SET attribute = :new "
				 "WHERE attribute = :old");
		  query->bindValue(":new", partner);
		  query->bindValue(":old", origin);
		  query->exec();
		  query->prepare("UPDATE attributes_to_incidents_sources SET attribute = :new "
				 "WHERE attribute = :old");
		  query->bindValue(":new", partner);
		  query->bindValue(":old", origin);
		  query->exec();
		  query->prepare("UPDATE saved_eg_plots_attributes_to_abstract_nodes "
				 "SET attribute = :new WHERE attribute = :old");
		  query->bindValue(":new", partner);
		  query->bindValue(":old", origin);
		  query->exec();
		  delete mergeDialog;
		  query->prepare("SELECT description FROM incident_attributes WHERE name = :name");
		  query->bindValue(":name", partner);
		  query->exec();
		  query->first();
		  QString description = query->value(0).toString();
		  QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
		  attributeDialog->submitName(partner);
		  attributeDialog->submitDescription(description);
		  attributeDialog->exec();
		  if (attributeDialog->getExitStatus() == 0) 
		    {
		      QString newName = attributeDialog->getName();
		      description = attributeDialog->getDescription();
		      QStandardItem *currentAttribute = attributesTree->
			itemFromIndex(treeFilter->mapToSource(attributesTreeView->currentIndex()));
		      currentAttribute->setData(newName);
		      currentAttribute->setData(newName, Qt::DisplayRole);
		      QString hint = breakString(description);
		      currentAttribute->setToolTip(hint);
		      query->prepare("UPDATE incident_attributes "
				     "SET name = :newname, description = :newdescription "
				     "WHERE name = :oldname");
		      query->bindValue(":newname", newName);
		      query->bindValue(":newdescription", description);
		      query->bindValue(":oldname", partner);
		      query->exec();
		      query->prepare("UPDATE incident_attributes "
				     "SET father = :newname "
				     "WHERE father = :oldname");
		      query->bindValue(":newname", newName);
		      query->bindValue(":oldname", partner);
		      query->exec();
		      query->prepare("UPDATE attributes_to_incidents "
				     "SET attribute = :newname "
				     "WHERE attribute = :oldname");
		      query->bindValue(":newname", newName);
		      query->bindValue(":oldname", partner);
		      query->exec();
		      query->prepare("UPDATE attributes_to_incidents_sources "
				     "SET attribute = :newname "
				     "WHERE attribute = :oldname");
		      query->bindValue(":newname", newName);
		      query->bindValue(":oldname", partner);
		      query->exec();
		      query->prepare("UPDATE saved_eg_plots_attributes_to_abstract_nodes "
				     "SET attribute = :newname "
				     "WHERE attribute = :oldname");
		      query->bindValue(":newname", newName);
		      query->bindValue(":oldname", partner);
		      query->exec();
		      this->setCursor(Qt::WaitCursor);
		      retrieveData();
		      this->setCursor(Qt::ArrowCursor);
		      _eventGraphWidgetPtr->resetTree();
		    }
		  delete query;
		  delete attributeDialog;
		  attributesTree->sort(0, Qt::AscendingOrder);
		  attributesTreeView->sortByColumn(0, Qt::AscendingOrder);
		  resetTree();
		  fixTree();
		  _eventGraphWidgetPtr->resetTree();
		}
	    }
	}
    }
}

void AttributesWidget::boldSelected(QAbstractItemModel *model, QString name, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QString currentName = model->data(index).toString();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
      int fontSize = currentAttribute->font().pointSize();
      QFont font;
      font.setBold(true);
      font.setPointSize(fontSize);
      QFont font2;
      font2.setUnderline(true);
      font2.setPointSize(fontSize);
      QFont font3;
      font3.setBold(true);
      font3.setUnderline(true);
      font3.setPointSize(fontSize);
      QFont font4;
      font4.setItalic(true);
      font4.setPointSize(fontSize);
      QFont font5;
      font5.setItalic(true);
      font5.setUnderline(true);
      font5.setPointSize(fontSize);
      if (name != ENTITIES) 
	{
	  if (name == currentName) 
	    {
	      if (currentAttribute->font().underline()) 
		{
		  currentAttribute->setFont(font3);
		}
	      else 
		{
		  currentAttribute->setFont(font);
		}
	      if (currentAttribute->parent()) 
		{
		  while (currentAttribute->parent()) 
		    {
		      currentAttribute = currentAttribute->parent();
		      QString parentName = currentAttribute->data(Qt::DisplayRole).toString();
		      if (parentName != ENTITIES) 
			{
			  QSqlQuery *query = new QSqlQuery;
			  query->exec("SELECT attributes_record FROM save_data");
			  query->first();
			  int order = query->value(0).toInt();
			  query->prepare("SELECT id FROM incidents WHERE ch_order = :order");
			  query->bindValue(":order", order);
			  query->exec();
			  query->first();
			  int incident = query->value(0).toInt();
			  query->prepare("SELECT attribute, incident FROM attributes_to_incidents "
					 "WHERE attribute = :attribute AND incident = :incident");
			  query->bindValue(":attribute", parentName);
			  query->bindValue(":incident", incident);
			  query->exec();
			  query->first();
			  if (query->isNull(0)) 
			    {
			      currentAttribute->setFont(font2);      
			    }
			  else 
			    {
			      currentAttribute->setFont(font3);
			    }
			  delete query;
			}
		      else 
			{
			  currentAttribute->setFont(font5);
			}
		    }
		}
	    } 
	}
      else 
	{
	  currentAttribute->setFont(font4);
	}
      if (model->hasChildren(index)) 
	{
	  boldSelected(model, name, index);
	}
    }
}

void AttributesWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QString currentName = model->data(index).toString();
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
      int fontSize = currentAttribute->font().pointSize();
      QFont font;
      font.setBold(false);
      font.setUnderline(false);
      font.setPointSize(fontSize);
      QFont font2;
      font2.setItalic(true);
      font2.setBold(false);
      font2.setUnderline(false);
      font2.setPointSize(fontSize);
      if (currentName != ENTITIES) 
	{
	  currentAttribute->setFont(font);
	}
      else 
	{
	  currentAttribute->setFont(font2);
	}
      if (model->hasChildren(index)) 
	{
	  resetFont(model, index);
	}
    }
}

void AttributesWidget::changeTreeFontSize(QAbstractItemModel *model, QModelIndex parent, int size)
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QStandardItem *currentAttribute = attributesTree->itemFromIndex(index);
      QFont font = currentAttribute->font();
      int fontSize = font.pointSize();
      if ((size == -1 && fontSize >= 10) ||
	  (size == 1 && fontSize <= 50))
	{
	  font.setPointSize(fontSize + size);
	  currentAttribute->setFont(font);
	  if (model->hasChildren(index)) 
	    {
	      changeTreeFontSize(model, index, size);
	    }
	}
    }
}

void AttributesWidget::fixTree() 
{
  resetFont(attributesTree);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT attributes_record FROM save_data");
  query->first();
  int order = 0; 
  if (!query->isNull(0))
    {
      order = query->value(0).toInt();
    }
  QSqlQuery *query2 = new QSqlQuery;
  query2->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
  query2->bindValue(":order", order);
  query2->exec();
  query2->first();
  int id = 0;
  if (!query2->isNull(0))
    {
      id = query2->value(0).toInt();
    }
  query2->exec("SELECT attribute, incident FROM attributes_to_incidents");
  while (query2->next()) 
    {
      QString attribute = query2->value(0).toString();
      int incident = query2->value(1).toInt();
      if (incident == id) 
	{
	  boldSelected(attributesTree, attribute);
	}
    }
  delete query;
  delete query2;
}

void AttributesWidget::finalBusiness() 
{
  setComment();
}

void AttributesWidget::setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr) 
{
  _eventGraphWidgetPtr = eventGraphWidgetPtr;
}

void AttributesWidget::setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr) 
{
  _relationshipsWidgetPtr = relationshipsWidgetPtr;
}

void AttributesWidget::resetTree() 
{
  delete attributesTree;
  setTree();
}

bool AttributesWidget::eventFilter(QObject *object, QEvent *event) 
{
  if (object == attributesTreeView)
    {
      if (event->type() == QEvent::ChildRemoved) 
	{
	  fixTree();
	}
      else if (event->type() == QEvent::Wheel)
	{
	  QWheelEvent *wheelEvent = (QWheelEvent*) event;
	  if(wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->angleDelta().y() > 0) 
		{
		  changeTreeFontSize(attributesTree, QModelIndex(), 1);
		}
	      else if (wheelEvent->angleDelta().y() < 0) 
		{
		  changeTreeFontSize(attributesTree, QModelIndex(), -1);
		}
	    }
	} 
    }
  else if (((object == descriptionField->viewport() && descriptionField->isEnabled()) ||
	    (object == rawField->viewport() && rawField->isEnabled()) ||
	    (object == timeStampField && timeStampField->isEnabled()) ||
	    (object == sourceField && sourceField->isEnabled())) &&
	   event->type() == QEvent::ContextMenu)
    {
      QContextMenuEvent *context = (QContextMenuEvent*) event;
      QMenu *menu = new QMenu;
      QAction *editAction = new QAction(tr("Edit text"), this);
      connect(editAction, SIGNAL(triggered()), this, SLOT(editIncident()));
      menu->addAction(editAction);
      menu->exec(context->globalPos());
      delete editAction;
      delete menu;
      return true;
    }
  else if (object == rawField->viewport() && event->type() == QEvent::MouseButtonRelease) 
    {
      selectText();
    }
  else if (event->type() == QEvent::Wheel) 
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
  return false;
}
