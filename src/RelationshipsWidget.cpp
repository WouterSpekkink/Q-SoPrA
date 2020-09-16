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

#include "../include/RelationshipsWidget.h"

RelationshipsWidget::RelationshipsWidget(QWidget *parent) : QWidget(parent) 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT coder FROM save_data");
  query->first();
  _selectedCoder = query->value(0).toString();
  delete query;
  _descriptionFilter = "";
  _rawFilter = "";
  _commentFilter = "";
  _commentBool = false;

  incidentsModel = new QSqlTableModel(this);  
  incidentsModel->setTable("incidents");
  incidentsModel->setSort(1, Qt::AscendingOrder);
  incidentsModel->select();

  typeModel = new QSqlTableModel(this);
  typeModel->setTable("relationship_types");
  typeModel->select();
 
  relationshipsModel = new QSqlTableModel(this);
  relationshipsModel->setTable("entity_relationships");
  relationshipsModel->select();

  assignedModel = new QSqlTableModel(this);
  assignedModel->setTable("relationships_to_incidents");
  assignedModel->select();

  relationshipsTreeView = new DeselectableTreeView(this);
  relationshipsTreeView->setHeaderHidden(true);
  relationshipsTreeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  relationshipsTreeView->header()->setStretchLastSection(false);
  relationshipsTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  treeFilter = new RelationshipTreeFilter(this);
  treeFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
  setTree();
  relationshipsTreeView->setSortingEnabled(true);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);

  indexLabel = new QLabel("<b>Incident ( / )</b>", this);
  markLabel = new QLabel("", this);
  markLabel->setStyleSheet("QLabel {color: red}");
  timeStampLabel = new QLabel("<b>Timing:</b>", this);
  sourceLabel = new QLabel("<b>Source:</b>", this);
  descriptionLabel = new QLabel("<b>Description:</b>", this);
  rawLabel = new QLabel("<b>Raw:</b>", this);
  commentLabel = new QLabel("<b>Comments:</b>", this);
  relationshipsLabel = new QLabel("<h2>Relationships</h2>", this);
  descriptionFilterLabel = new QLabel("<i>Search descriptions:</i>", this);
  rawFilterLabel = new QLabel("<i>Search raw texts:</i>", this);
  commentFilterLabel = new QLabel("<i>Search comments:</i>", this);
  relationshipCommentLabel = new QLabel("<b>Comment:</b>", this);
  relationshipFilterLabel = new QLabel("<b>Filter relationships:</b>", this);
  
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
  relationshipFilterField = new QLineEdit(this);
  relationshipCommentField = new QLineEdit(this);
  relationshipCommentField->setEnabled(false);

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
  previousCodedButton = new QPushButton("Previous coded", this);
  previousCodedButton->setEnabled(false);
  nextCodedButton = new QPushButton("Next coded", this);
  nextCodedButton->setEnabled(false);
  submitRelationshipCommentButton = new QPushButton("Set comment", this);
  submitRelationshipCommentButton->setEnabled(false);
  newTypeButton = new QPushButton("Add relationship type", this);
  editTypeButton = new QPushButton("Edit relationship type", this);
  editTypeButton->setEnabled(false);
  entitiesOverviewButton = new QPushButton("Entities overview", this);
  newRelationshipButton = new QPushButton("Add relationship", this);
  newRelationshipButton->setEnabled(false);
  editRelationshipButton = new QPushButton("Edit relationship", this);
  editRelationshipButton->setEnabled(false);
  removeUnusedRelationshipsButton = new QPushButton("Removed unused relationships", this);
  assignRelationshipButton = new QPushButton("Assign relationship", this);
  assignRelationshipButton->setEnabled(false);
  unassignRelationshipButton = new QPushButton("Unassign relationship", this);
  unassignRelationshipButton->setEnabled(false);
  expandTreeButton = new QPushButton("Expand all", this);
  collapseTreeButton = new QPushButton("Collapse all", this);
  removeTextButton = new QPushButton("Remove text", this);
  removeTextButton->setEnabled(false);
  resetTextsButton = new QPushButton("Reset texts", this);
  resetTextsButton->setEnabled(false);
  
  relationshipsTreeView->installEventFilter(this);
  timeStampField->installEventFilter(this);
  sourceField->installEventFilter(this);
  descriptionField->viewport()->installEventFilter(this);  
  rawField->viewport()->installEventFilter(this);
  commentField->installEventFilter(this);

  relationshipsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
   
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
  connect(previousCodedButton, SIGNAL(clicked()), this, SLOT(previousCoded()));
  connect(nextCodedButton, SIGNAL(clicked()), this, SLOT(nextCoded()));
  connect(relationshipFilterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(relationshipCommentField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(setCommentButton()));
  connect(submitRelationshipCommentButton, SIGNAL(clicked()),
	  this, SLOT(submitRelationshipComment()));
  connect(newTypeButton, SIGNAL(clicked()), this, SLOT(newType()));
  connect(editTypeButton, SIGNAL(clicked()), this, SLOT(editType()));
  connect(entitiesOverviewButton, SIGNAL(clicked()), this, SLOT(entitiesOverview()));
  connect(removeUnusedRelationshipsButton, SIGNAL(clicked()), this, SLOT(removeUnusedRelationships()));
  connect(assignRelationshipButton, SIGNAL(clicked()), this, SLOT(assignRelationship()));
  connect(unassignRelationshipButton, SIGNAL(clicked()), this, SLOT(unassignRelationship()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(resetTextsButton, SIGNAL(clicked()), this, SLOT(resetTexts()));
  connect(relationshipsTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(highlightText()));
  connect(relationshipsTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(getComment()));
  connect(relationshipsTreeView->selectionModel(),
	  SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	  this, SLOT(setButtons()));
  connect(relationshipsTreeView, SIGNAL(noneSelected()), this, SLOT(setButtons()));
  connect(relationshipsTreeView, SIGNAL(customContextMenuRequested(QPoint)),
	  this, SLOT(treeContextMenu(QPoint)));
  connect(newRelationshipButton, SIGNAL(clicked()), this, SLOT(newRelationship()));
  connect(editRelationshipButton, SIGNAL(clicked()), this, SLOT(editRelationship()));
  connect(expandTreeButton, SIGNAL(clicked()), this, SLOT(expandTree()));
  connect(collapseTreeButton, SIGNAL(clicked()), this, SLOT(collapseTree()));
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
  titleLayout->addWidget(relationshipsLabel);
  QPointer<QHBoxLayout> collapseLayout = new QHBoxLayout;
  collapseLayout->addWidget(expandTreeButton);
  expandTreeButton->setMaximumWidth(expandTreeButton->sizeHint().width());
  collapseLayout->addWidget(collapseTreeButton);
  collapseTreeButton->setMaximumWidth(collapseTreeButton->sizeHint().width());
  titleLayout->addLayout(collapseLayout);
  rightLayout->addLayout(titleLayout);
  rightLayout->addWidget(relationshipsTreeView);
  QPointer<QHBoxLayout> filterLayout = new QHBoxLayout;
  filterLayout->addWidget(relationshipFilterLabel);
  filterLayout->addWidget(relationshipFilterField);
  rightLayout->addLayout(filterLayout);
  QPointer<QHBoxLayout> relCommentLayout = new QHBoxLayout;
  relCommentLayout->addWidget(relationshipCommentLabel);
  relCommentLayout->addWidget(relationshipCommentField);
  relCommentLayout->addWidget(submitRelationshipCommentButton);
  rightLayout->addLayout(relCommentLayout);
  QPointer<QHBoxLayout> rightButtonTopLayout = new QHBoxLayout;
  rightButtonTopLayout->addWidget(assignRelationshipButton);
  rightButtonTopLayout->addWidget(unassignRelationshipButton);
  rightButtonTopLayout->addWidget(removeTextButton);
  rightButtonTopLayout->addWidget(resetTextsButton);
  rightLayout->addLayout(rightButtonTopLayout);
  QPointer<QHBoxLayout> rightButtonCodedLayout = new QHBoxLayout;
  rightButtonCodedLayout->addWidget(previousCodedButton);
  rightButtonCodedLayout->addWidget(nextCodedButton);
  rightLayout->addLayout(rightButtonCodedLayout);
  QPointer<QHBoxLayout> rightButtonMiddleLayout = new QHBoxLayout;
  rightButtonMiddleLayout->addWidget(newRelationshipButton);
  rightButtonMiddleLayout->addWidget(editRelationshipButton);
  rightButtonMiddleLayout->addWidget(removeUnusedRelationshipsButton);
  rightLayout->addLayout(rightButtonMiddleLayout);
  QPointer<QHBoxLayout> rightButtonBottomLayout = new QHBoxLayout;
  rightButtonBottomLayout->addWidget(newTypeButton);
  rightButtonBottomLayout->addWidget(editTypeButton);
  rightButtonBottomLayout->addWidget(entitiesOverviewButton);
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

void RelationshipsWidget::setCommentBool() 
{
  _commentBool = true;
}

void RelationshipsWidget::setComment() 
{
  if (_commentBool) 
    {
      QString comment = commentField->toPlainText();
      incidentsModel->select();
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
      query->prepare("UPDATE incidents SET comment = :comment WHERE ch_order = :order");
      query->bindValue(":comment", comment);
      query->bindValue(":order", order);
      query->exec();
      _commentBool = false;
      delete query;
    }
}

void RelationshipsWidget::retrieveData() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  query->first();
  int order = query->value(0).toInt();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  int total = incidentsModel->rowCount();
  QString indexText = "<b>Incident (" + QString::number(order)
    + " / " + QString::number(total) + ")<b>";
  indexLabel->setText(indexText);
  query->prepare("SELECT id, timestamp, source, description, raw, comment, mark "
		  "FROM incidents WHERE ch_order = :order");
  query->bindValue(":order", order);
  query->exec();
  query->first();
  if (!(query->isNull(0))) 
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
      resetFont(relationshipsTree);
      query->exec("SELECT relationship, type, incident, coder FROM relationships_to_incidents");
      while (query->next()) 
	{
	  QString relationship = query->value(0).toString();
	  QString type = query->value(1).toString();
	  int incident = query->value(2).toInt();
	  QString coder = query->value(3).toString();
	  if (incident == id && coder == _selectedCoder) 
	    {
	      boldSelected(relationshipsTree, relationship, type);
	    }
	}
      relationshipsTreeView->setModel(treeFilter);
      relationshipsTreeView->resetSelection();
      relationshipsTree->sort(0, Qt::AscendingOrder);
      relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
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

void RelationshipsWidget::selectText() 
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

void RelationshipsWidget::sourceRelationshipText(const QString &relationship,
						 const QString &type,
						 const int &incident) 
{
  if (rawField->textCursor().selectedText().trimmed() != "") 
    {
      QString sourceText = rawField->textCursor().selectedText().trimmed();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT relationship FROM relationships_to_incidents_sources "
		     "WHERE relationship = :relationship AND type = :type AND "
		     "incident = :incident AND coder = :coder AND source_text = :text)");
      query->bindValue(":relationship", relationship);
      query->bindValue(":type", type);
      query->bindValue(":incident", incident);
      query->bindValue(":coder", _selectedCoder);
      query->bindValue(":text", sourceText);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  query->prepare("INSERT INTO relationships_to_incidents_sources "
			 "(relationship, type, incident, coder, source_text)"
			 "VALUES (:relationship, :type, :incident, :coder, :text)");
	  query->bindValue(":relationship", relationship);
	  query->bindValue(":type", type);
	  query->bindValue(":incident", incident);
	  query->bindValue(":coder", _selectedCoder);
	  query->bindValue(":text", sourceText);
	  query->exec();
	}
      delete query;
    }
}

void RelationshipsWidget::highlightText() 
{
  int barPos = rawField->verticalScrollBar()->value();
  QTextCursor currentPos =  rawField->textCursor();
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentRelationship = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      QString currentName = relationshipsTreeView->currentIndex().data(Qt::UserRole).toString();
      if (currentRelationship->parent()) 
	{
	  QStandardItem *typeItem = currentRelationship->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  if (currentRelationship->font().bold()) 
	    {
	      query->exec("SELECT relationships_record FROM save_data");
	      query->first();
	      int order = query->value(0).toInt();
	      query->prepare("SELECT id FROM incidents WHERE ch_order = :order");
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
	      query->prepare("SELECT source_text FROM relationships_to_incidents_sources "
			     "WHERE relationship = :relationship "
			     "AND type = :type AND incident = :id AND coder = :coder");
	      query->bindValue(":relationship", currentName);
	      query->bindValue(":type", currentType);
	      query->bindValue(":id", id);
	      query->bindValue(":coder", _selectedCoder);
	      query->exec();
	      while(query->next()) 
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
    }
  rawField->verticalScrollBar()->setValue(barPos);
}

void RelationshipsWidget::changeFilter(const QString &text) 
{
  QRegExp regExp(text, Qt::CaseInsensitive);
  treeFilter->setFilterRegExp(regExp);
}

void RelationshipsWidget::setCommentButton() 
{
  submitRelationshipCommentButton->setEnabled(true);
}

void RelationshipsWidget::submitRelationshipComment() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  QString currentName = currentItem->data(Qt::UserRole).toString();
	  QString currentType = currentItem->parent()->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("UPDATE entity_relationships SET comment = :comment "
			 "WHERE name = :name AND type = :type");
	  query->bindValue(":comment", relationshipCommentField->text());
	  query->bindValue(":name", currentName);
	  query->bindValue(":type", currentType);
	  query->exec();
	  delete query;
	}
    }
  submitRelationshipCommentButton->setEnabled(false);
}

void RelationshipsWidget::getComment() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  relationshipCommentField->setEnabled(true);
	  QString currentName = currentItem->data(Qt::UserRole).toString();
	  QString currentType = currentItem->parent()->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT comment FROM entity_relationships "
			 "WHERE name = :name AND type = :type");
	  query->bindValue(":name", currentName);
	  query->bindValue(":type", currentType);
	  query->exec();
	  query->first();
	  QString comment = query->value(0).toString();
	  relationshipCommentField->setText(comment);
	  submitRelationshipCommentButton->setEnabled(false);\
	  delete query;
	}
      else 
	{
	  relationshipCommentField->setEnabled(false);
	  relationshipCommentField->setText("");
	  submitRelationshipCommentButton->setEnabled(false);
	}
    }
  else 
    {
      relationshipCommentField->setEnabled(false);
      relationshipCommentField->setText("");
      submitRelationshipCommentButton->setEnabled(false);
    }
}

void RelationshipsWidget::newType() 
{
  typeDialog = new RelationshipTypeDialog(this);
  typeDialog->exec();
  if (typeDialog->getExitStatus() == 0) 
    {
      QString name = typeDialog->getName();
      QString description = typeDialog->getDescription();
      QString directedness = typeDialog->getDirectedness();
      QStandardItem *type = new QStandardItem(name);
      QString hint =  breakString(directedness + " - " + description);
      type->setToolTip(hint);
      type->setEditable(false);
      relationshipsTree->appendRow(type);
      int newIndex = typeModel->rowCount();
      typeModel->insertRow(newIndex);
      typeModel->setData(typeModel->index(newIndex, 1), name);
      typeModel->setData(typeModel->index(newIndex, 2), directedness);
      typeModel->setData(typeModel->index(newIndex, 3), description);
      typeModel->submitAll();
    }
  delete typeDialog;
  relationshipsTree->sort(0, Qt::AscendingOrder);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
}

void RelationshipsWidget::editType() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      QString currentName = relationshipsTreeView->currentIndex().data().toString();
      // Check if this is a type, and not a relationship
      if (!currentItem->parent()) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT directedness, description "
			 "FROM relationship_types "
			 "WHERE name = :name");
	  query->bindValue(":name", currentName);
	  query->exec();
	  query->first();
	  QString oldDirectedness = query->value(0).toString();
	  QString description = query->value(1).toString();
	  typeDialog = new RelationshipTypeDialog(this);
	  typeDialog->submitName(currentName);
	  typeDialog->submitDescription(description);
	  typeDialog->submitDirectedness(oldDirectedness);
	  typeDialog->exec();
	  if (typeDialog->getExitStatus() == 0) 
	    {
	      QString newName = typeDialog->getName();
	      description = typeDialog->getDescription();
	      QString directedness = typeDialog->getDirectedness();
	      if (oldDirectedness == DIRECTED && directedness == UNDIRECTED)
		{
		  QPointer<QMessageBox> warningBox = new QMessageBox(this);
		  warningBox->setWindowTitle("Changing directedness"); 
		  warningBox->addButton(QMessageBox::Yes);
		  warningBox->addButton(QMessageBox::No);
		  warningBox->setIcon(QMessageBox::Warning);
		  warningBox->setText("<h2>Changing to undirected?</h2>");
		  warningBox->setInformativeText("You are changing a directed relationship "
						 "to an undirected one. This may create duplicate "
						 "relationships that will be removed. Do you wish "
						 "to continue?");
		  if (warningBox->exec() == QMessageBox::No) 
		    {
		      delete query;
		      delete typeDialog;
		      return;
		    }
		}
	      QVector<QPair<QString, QString>> finished;
	      QStandardItem *currentType = relationshipsTree->
		itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
	      currentType->setData(newName);
	      currentType->setData(newName, Qt::DisplayRole);
	      QString hint = breakString(directedness + " - " + description);
	      currentType->setToolTip(hint);
	      for (int i = 0; i != currentType->rowCount(); i++) 
		{
		  bool deleteCurrent = false;
		  QStandardItem *currentChild = currentType->takeChild(i);
		  QString baseName = currentChild->data(Qt::UserRole).toString();
		  QStringList relationshipParts = QStringList();
		  QString newChildName = QString();
		  QString newWriteName = QString();
		  if (oldDirectedness == DIRECTED)
		    {
		      relationshipParts = baseName.split("--->");
		      QPair<QString, QString> pairOne =
			QPair<QString, QString>(relationshipParts[0], relationshipParts[1]);
		      QPair<QString, QString> pairTwo =
			QPair<QString, QString>(relationshipParts[1], relationshipParts[0]);
		      if (finished.contains(pairOne) || finished.contains(pairTwo))
			{
			  deleteCurrent = true;
			}
		      else
			{
			  finished.push_back(pairOne);
			}
		      if (directedness == DIRECTED)
			{
			  newChildName = relationshipParts[0] + "-[" + newName +
			    "]->" + relationshipParts[1];
			  newWriteName = relationshipParts[0] + "--->" + relationshipParts[1];
			}
		      else if (directedness == UNDIRECTED)
			{
			  newChildName = relationshipParts[0] + "<-[" + newName +
			    "]->" + relationshipParts[1];
			  newWriteName = relationshipParts[0] + "<-->" + relationshipParts[1];
			}
		    }
		  else if (oldDirectedness == UNDIRECTED)
		    {
		      relationshipParts = baseName.split("<-->");
		      if (directedness == DIRECTED)
			{
			  newChildName = relationshipParts[0] + "-[" + newName +
			    "]->" + relationshipParts[1];
			  newWriteName = relationshipParts[0] + "--->" + relationshipParts[1];
			}
		      else if (directedness == UNDIRECTED)
			{
			  newChildName = relationshipParts[0] + "<-[" + newName +
			    "]->" + relationshipParts[1];
			  newWriteName = relationshipParts[0] + "<-->" + relationshipParts[1];
			}
		    }
		  if (deleteCurrent)
		    {
		      query->prepare("DELETE FROM relationships_to_incidents "
				     "WHERE relationship = :current "
				     "AND type = :type");
		      query->bindValue(":current", baseName);
		      query->bindValue(":type", currentName);
		      query->exec();
		      query->prepare("DELETE FROM entity_relationships "
				     "WHERE name = :current "
				     "AND type = :type");
		      query->bindValue(":current", baseName);
		      query->bindValue(":type", currentName);
		      query->exec();
		      delete currentChild;
		      currentType->removeRow(i);
		      i--;
		    }
		  else
		    {
		      currentChild->setData(newChildName, Qt::DisplayRole);
		      currentChild->setData(newWriteName, Qt::UserRole);
		      currentChild->setToolTip(hint);
		      currentType->setChild(i, currentChild);
		    }
		  query->prepare("UPDATE entity_relationships SET name = :newname, "
				 "type = :newtype "
				 "WHERE type = :oldtype AND name = :oldname");
		  query->bindValue(":newname", newWriteName);
		  query->bindValue(":newtype", newName);
		  query->bindValue(":oldname", baseName);
		  query->bindValue(":oldtype", currentName);
		  query->exec();
		  query->prepare("UPDATE relationships_to_incidents SET "
				 "relationship = :newname, type = :newtype "
				 "WHERE relationship = :oldname AND type = :oldtype");
		  query->bindValue(":newname", newWriteName);
		  query->bindValue(":newtype", newName);
		  query->bindValue(":oldname", baseName);
		  query->bindValue(":oldtype", currentName);
		  query->exec();
		  query->prepare("UPDATE relationships_to_incidents_sources "
				 "SET relationship = :newname, type = :newtype "
				 "WHERE relationship = :oldname AND type = :oldtype");
		  query->bindValue(":newname", newWriteName);
		  query->bindValue(":newtype", newName);
		  query->bindValue(":oldname", baseName);
		  query->bindValue(":oldtype", currentName);
		  query->exec();
		  query->exec("SELECT relationship, type FROM relationships_to_incidents");
		  while (query->next()) 
		    {
		      QString assignedRelationship = query->value(0).toString();
		      QString assignedType = query->value(1).toString();
		      if (assignedRelationship == newWriteName && assignedType == newName) 
			{
			  boldSelected(relationshipsTree, assignedRelationship, assignedType);
			}
		    }
		}
	      query->prepare("UPDATE relationship_types SET name = :newname, "
			     "directedness = :newdirectedness, description = :newdescription "
			     "WHERE name = :oldname");
	      query->bindValue(":newname", newName);
	      query->bindValue(":newdirectedness", directedness);
	      query->bindValue(":newdescription", description);
	      query->bindValue(":oldname", currentName);
	      query->exec();
	      this->setCursor(Qt::WaitCursor);
	      retrieveData();
	      this->setCursor(Qt::ArrowCursor);
	    }
	  delete query;
	}
      relationshipsTree->sort(0, Qt::AscendingOrder);
      relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
    }
}

void RelationshipsWidget::entitiesOverview() 
{
  QPointer<EditEntityDialog> entityDialog = new EditEntityDialog(this);
  entityDialog->setEventGraphWidget(_eventGraphWidgetPtr);
  entityDialog->setAttributesWidget(_attributesWidgetPtr);
  entityDialog->exec();
  if (entityDialog->entityEdited()) 
    {
      delete relationshipsTree;
      setTree();
      relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
      _eventGraphWidgetPtr->resetTree();
      _attributesWidgetPtr->resetTree();
      retrieveData();
    }
  if (entityDialog->entityAdded())
    {
      _eventGraphWidgetPtr->resetTree();
      _attributesWidgetPtr->resetTree();
    }
  delete entityDialog;
}

void RelationshipsWidget::assignRelationship() 
{
  int barPos = rawField->verticalScrollBar()->value();
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  QStandardItem *typeItem = currentItem->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  QString currentRelationship = relationshipsTreeView->
	    currentIndex().data(Qt::UserRole).toString();
	  query->exec("SELECT relationships_record FROM save_data");
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
	      assignedModel->select();
	      bool empty = false;
	      query->prepare("SELECT relationship, incident FROM relationships_to_incidents "
			     "WHERE relationship = :rel AND type = :type AND incident = :inc "
			     "AND coder = :coder");
	      query->bindValue(":rel", currentRelationship);
	      query->bindValue(":type", currentType);
	      query->bindValue(":inc", id);
	      query->bindValue(":coder", _selectedCoder);
	      query->exec();
	      query->first();
	      empty = query->isNull(0);
	      QTextCursor cursorPos = rawField->textCursor();
	      if (empty) 
		{
		  query->prepare("INSERT INTO relationships_to_incidents (relationship, type, "
				 "incident, coder) "
				  "VALUES (:relationship, :type, :incident, :coder)");
		  query->bindValue(":relationship", currentRelationship);
		  query->bindValue(":type", currentType);
		  query->bindValue(":incident", id);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();
		  sourceRelationshipText(currentRelationship, currentType, id);
		  rawField->setTextCursor(cursorPos);
		  boldSelected(relationshipsTree, currentRelationship, currentType);
		  highlightText();
		}
	      else 
		{
		  sourceRelationshipText(currentRelationship, currentType, id);
		  highlightText();
		  rawField->setTextCursor(cursorPos);
		}
	      setButtons();
	    }
	  delete query;
	}
    }
  rawField->verticalScrollBar()->setValue(barPos);
}

void RelationshipsWidget::unassignRelationship() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  QString currentRelationship = relationshipsTreeView->
	    currentIndex().data(Qt::UserRole).toString();
	  QStandardItem *typeItem = currentItem->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->exec("SELECT relationships_record FROM save_data");
	  query->first();
	  int order = query->value(0).toInt(); 
	  query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
	  query->bindValue(":order", order);
	  query->exec();
	  query->first();
	  int id = 0;
	  if (!(query->isNull(0))) 
	    {
	      id = query->value(0).toInt();
	      assignedModel->select();
	      bool empty = false;
	      query->prepare("SELECT relationship, incident "
			     "FROM relationships_to_incidents "
			     "WHERE relationship = :rel AND type = :type AND incident = :inc "
			     "AND coder = :coder");
	      query->bindValue(":rel", currentRelationship);
	      query->bindValue(":type", currentType);
	      query->bindValue(":inc", id);
	      query->bindValue(":coder", _selectedCoder);
	      query->exec();
	      query->first();
	      empty = query->isNull(0);
	      if (!empty) 
		{
		  query->prepare("DELETE FROM relationships_to_incidents "
				 "WHERE relationship = :rel AND type = :type "
				 "AND incident = :inc AND coder = :coder");
		  query->bindValue(":rel", currentRelationship);
		  query->bindValue(":type", currentType);	  
		  query->bindValue(":inc", id);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();
		  query->prepare("DELETE FROM relationships_to_incidents_sources "
				 "WHERE relationship = :rel AND type = :type "
				 "AND incident = :inc AND coder = :coder");
		  query->bindValue(":rel", currentRelationship);
		  query->bindValue(":type", currentType);
		  query->bindValue(":inc", id);
		  query->bindValue(":coder", _selectedCoder);
		  query->exec();
		  assignedModel->select();
		  resetFont(relationshipsTree);
		  query->exec("SELECT relationship, type, incident, coder "
			      "FROM relationships_to_incidents");
		  while (query->next()) 
		    {
		      QString relationship = query->value(0).toString();
		      QString type = query->value(1).toString();
		      int incident = query->value(2).toInt();
		      QString coder = query->value(3).toString();
		      if (incident == id && coder == _selectedCoder) 
			{
			  boldSelected(relationshipsTree, relationship, type);
			}
		    }
		  highlightText();
		  unassignRelationshipButton->setEnabled(false);
		}
	    }
	  delete query;
	}
    }
}

void RelationshipsWidget::removeText() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = 0; 
      order = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int id = 0;
      id = query->value(0).toInt();
      QString relationship = relationshipsTreeView->currentIndex().data(Qt::UserRole).toString();
      if (rawField->textCursor().selectedText().trimmed() != "") 
	{
	  QString sourceText = rawField->textCursor().selectedText().trimmed();
	  QSqlQuery *query2 = new QSqlQuery;
	  query2->prepare("DELETE FROM relationships_to_incidents_sources "
			  "WHERE relationship = :rel AND incident = :inc "
			  "AND coder = :code AND source_text = :text");
	  query2->bindValue(":rel", relationship);
	  query2->bindValue(":inc", id);
	  query2->bindValue(":coder", _selectedCoder);
	  query2->bindValue(":text", sourceText);
	  query2->exec();
	  delete query2;
	}
      highlightText();
      setButtons();
      delete query;
    }
}

void RelationshipsWidget::resetTexts() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->setWindowTitle("Reset texts"); 
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("Resetting source texts cannot be undone. "
				     "Are you sure you want to proceed?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
	  QSqlQuery *query = new QSqlQuery;
	  query->exec("SELECT relationships_record FROM save_data");
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
	      QString relationship = relationshipsTreeView->
		currentIndex().data(Qt::UserRole).toString();
	      QStandardItem *currentItem = relationshipsTree->
		itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
	      QStandardItem *typeItem = currentItem->parent();
	      QString currentType = typeItem->data(Qt::DisplayRole).toString();
	      QSqlQuery *query2 = new QSqlQuery;
	      query2->prepare("DELETE FROM relationships_to_incidents_sources "
			      "WHERE relationship = :rel AND type = :type AND incident = :inc "
			      "AND coder = :coder");
	      query2->bindValue(":rel", relationship);
	      query2->bindValue(":type", currentType);
	      query2->bindValue(":inc", id);
	      query2->bindValue(":coder", _selectedCoder);
	      query2->exec();
	      delete query2;
	    }
	  highlightText();
	  delete query;
	}
      setButtons();
      delete warningBox;
    }
}

void RelationshipsWidget::newRelationship() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (!currentItem->parent()) 
	{
	  QString currentType = relationshipsTreeView->currentIndex().data().toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT directedness, description FROM relationship_types WHERE name = :name");
	  query->bindValue(":name", currentType);
	  query->exec();
	  query->first();
	  QString directedness = query->value(0).toString();
	  QString description = query->value(1).toString();
	  QString hint = breakString(directedness + " - " + description);
	  RelationshipsDialog *relationshipsDialog = new RelationshipsDialog(this);
	  relationshipsDialog->setAttributesWidget(_attributesWidgetPtr);
	  relationshipsDialog->setEventGraphWidget(_eventGraphWidgetPtr);
	  relationshipsDialog->submitType(currentType);
	  relationshipsDialog->submitDescription(description);
	  relationshipsDialog->submitDirectedness(directedness);
	  relationshipsDialog->exec();
	  if (relationshipsDialog->getExitStatus() == 0) 
	    {
	      QString name = relationshipsDialog->getName();
	      QString leftEntity = relationshipsDialog->getLeftEntity();
	      QString rightEntity = relationshipsDialog->getRightEntity();
	      QString itemName = QString();
	      if (directedness == DIRECTED)
		{
		  itemName = leftEntity + "-[" + currentType + "]->" + rightEntity;
		}
	      else if (directedness == UNDIRECTED)
		{
		  itemName = leftEntity + "<-[" + currentType + "]->" + rightEntity;
		}
	      QStandardItem *newItem = new QStandardItem(itemName);
	      newItem->setData(name, Qt::UserRole);
	      newItem->setEditable(false);
	      newItem->setToolTip(hint);
	      currentItem->appendRow(newItem);
	      query->prepare("INSERT INTO entity_relationships (name, source, target, type)"
			     "VALUES (:name, :source, :target, :type)");
	      query->bindValue(":name", name);
	      query->bindValue(":source", leftEntity);
	      query->bindValue(":target", rightEntity);
	      query->bindValue(":type", currentType);
	      query->exec();
	    }
	  if (relationshipsDialog->getEntityEdited() == 1) 
	    {
	      delete relationshipsTree;
	      setTree();
	      relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
	      retrieveData();
	    }
	  delete relationshipsDialog;
	  delete query;
	}
    }
}

void RelationshipsWidget::editRelationship() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  QStandardItem *typeItem = currentItem->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QString currentRelationship = currentItem->data(Qt::UserRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->prepare("SELECT directedness, description FROM relationship_types "
			 "WHERE name = :name");
	  query->bindValue(":name", currentType);
	  query->exec();
	  query->first();
	  QString directedness = query->value(0).toString();
	  QString description = query->value(1).toString();
	  query->prepare("SELECT source, target FROM entity_relationships WHERE name = :name");
	  query->bindValue(":name", currentRelationship);
	  query->exec();
	  query->first();
	  QString leftEntity = query->value(0).toString();
	  QString rightEntity = query->value(1).toString();
	  RelationshipsDialog *relationshipsDialog = new RelationshipsDialog(this);
	  relationshipsDialog->setAttributesWidget(_attributesWidgetPtr);
	  relationshipsDialog->setEventGraphWidget(_eventGraphWidgetPtr);
	  relationshipsDialog->submitType(currentType);
	  relationshipsDialog->submitDescription(description);
	  relationshipsDialog->submitDirectedness(directedness);
	  relationshipsDialog->submitLeftEntity(leftEntity);
	  relationshipsDialog->submitRightEntity(rightEntity);
	  relationshipsDialog->submitName(currentRelationship);
	  relationshipsDialog->exec();
	  if (relationshipsDialog->getExitStatus() == 0) 
	    {
	      QString name = relationshipsDialog->getName();
	      QString leftEntity = relationshipsDialog->getLeftEntity();
	      QString rightEntity = relationshipsDialog->getRightEntity();
	      QString itemName = QString();
	      if (directedness == DIRECTED)
		{
		  itemName = leftEntity + "-[" + currentType + "]->" + rightEntity;
		}
	      else if (directedness == UNDIRECTED)
		{
		  itemName = leftEntity + "<-[" + currentType + "]->" + rightEntity;
		}
	      query->prepare("UPDATE entity_relationships "
			     "SET name = :name, source = :leftEntity, target = :rightEntity "
			     "WHERE name = :oldName AND type = :type");
	      query->bindValue(":name", name);
	      query->bindValue(":leftEntity", leftEntity);
	      query->bindValue(":rightEntity", rightEntity);
	      query->bindValue(":oldName", currentRelationship);
	      query->bindValue(":type", currentType);
	      query->exec();
	      query->prepare("UPDATE relationships_to_incidents "
			     "SET relationship = :newName "
			     "WHERE relationship = :oldName AND type = :type");
	      query->bindValue(":newName", name);
	      query->bindValue(":oldName", currentRelationship);
	      query->bindValue(":type", currentType);
	      query->exec();
	      query->prepare("UPDATE relationships_to_incidents_sources "
			     "SET relationship = :newName "
			     "WHERE relationship = :oldName AND type = :type");
	      query->bindValue(":newName", name);
	      query->bindValue(":oldName", currentRelationship);
	      query->bindValue(":type", currentType);
	      query->exec();
	      currentItem->setData(itemName, Qt::DisplayRole);
	    }
	  if (relationshipsDialog->getEntityEdited() == 1) 
	    {
	      delete relationshipsTree;
	      setTree();
	      relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
	      retrieveData();
	      _attributesWidgetPtr->resetTree();
	      _eventGraphWidgetPtr->resetTree();
	    }
	  delete relationshipsDialog;
	  delete query;
	}
    }
}

void RelationshipsWidget::removeUnusedRelationships() 
{
  QSqlQuery *query = new QSqlQuery;
  QSqlQuery *query2 = new QSqlQuery;
  query->exec("SELECT name, type FROM entity_relationships EXCEPT SELECT relationship, type "
	      "FROM relationships_to_incidents");
  while (query->next()) 
    {
      QString currentName = query->value(0).toString();
      QString currentType = query->value(1).toString();
      query2->prepare("DELETE FROM entity_relationships WHERE name = :current AND type = :type");
      query2->bindValue(":current", currentName);
      query2->bindValue(":type", currentType);
      query2->exec();
    }
  query->exec("SELECT name FROM relationship_types EXCEPT SELECT type "
  	      "FROM entity_relationships");
  while(query->next()) 
    {
      QString current = query->value(0).toString();
      query2->prepare("DELETE FROM relationship_types WHERE name = :current");
      query2->bindValue(":current", current);
      query2->exec();
    }
  this->setCursor(Qt::WaitCursor);
  relationshipsTreeView->setSortingEnabled(false);
  setTree();
  relationshipsTreeView->setSortingEnabled(true);
  relationshipsTreeView->sortByColumn(0, Qt::AscendingOrder);
  retrieveData();
  this->setCursor(Qt::ArrowCursor);
  delete query;
  delete query2;
}

void RelationshipsWidget::setTree() 
{
  relationshipsTree = new QStandardItemModel(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT name, directedness, description FROM relationship_types");
  while (query->next()) 
    {
      QString currentType = query->value(0).toString();
      QString currentDirection = query->value(1).toString();
      QString typeDescription = query->value(2).toString();
      QStandardItem *type = new QStandardItem(currentType);    
      relationshipsTree->appendRow(type);
      QString hint = breakString(currentDirection + " - " + typeDescription);
      type->setToolTip(hint);
      type->setEditable(false);
      QSqlQuery *query2 = new QSqlQuery;
      query2->prepare("SELECT name, comment FROM entity_relationships WHERE type = :type");
      query2->bindValue(":type", currentType);
      query2->exec();
      int children = 0;
      while (query2->next()) 
	{
	  QString name = query2->value(0).toString();
	  QString comment = query2->value(1).toString();
	  QStringList relationshipParts = QStringList();
	  QString itemName = QString();
	  if (currentDirection == DIRECTED)
	    {
	      relationshipParts = name.split("--->");
	      itemName = relationshipParts[0] + "-[" + currentType
		+ "]->" + relationshipParts[1]; 
	    }
	  else if (currentDirection == UNDIRECTED)
	    {
	      relationshipParts = name.split("<-->");
	      itemName = relationshipParts[0] + "<-[" + currentType
		+ "]->" + relationshipParts[1]; 
	    }
	  QStandardItem *relationship = new QStandardItem(itemName);
	  relationship->setData(name, Qt::UserRole);
	  type->setChild(children, relationship);
	  relationship->setToolTip(hint);
	  relationship->setEditable(false);
	  children++;
	}
      delete query2;
    }
  delete query;
  treeFilter->setSourceModel(relationshipsTree);
  relationshipsTreeView->setModel(treeFilter);
}

void RelationshipsWidget::treeContextMenu(const QPoint &pos) 
{
  QPoint globalPos = relationshipsTreeView->mapToGlobal(pos);
  QModelIndex targetIndex = relationshipsTreeView->indexAt(pos);
  relationshipsTreeView->selectionModel()->select(targetIndex, QItemSelectionModel::Current);
  QString relationship = targetIndex.data(Qt::UserRole).toString();
  QString type = targetIndex.parent().data(Qt::DisplayRole).toString();
  if (targetIndex.parent().isValid()) 
    {
      QMenu *menu = new QMenu();
      QAction *action1 = new QAction(EDITRELATIONSHIPACTION, this);
      QAction *action2 = new QAction(ASSIGNRELATIONSHIPACTION, this);
      QAction *action3 = new QAction(UNASSIGNRELATIONSHIPACTION, this);
      QAction *action4 = new QAction(UNASSIGNALLACTION, this);
      menu->addAction(action1);
      menu->addAction(action2);
      menu->addAction(action3);
      menu->addAction(action4);
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int id = query->value(0).toInt();
      query->prepare("SELECT relationship FROM relationships_to_incidents "
		     "WHERE relationship = :relationship AND type = :type AND incident = :id "
		     "AND coder = :coder");
      query->bindValue(":relationship", relationship);
      query->bindValue(":type", type);
      query->bindValue(":id", id);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  action3->setEnabled(false);
	}
      query->prepare("SELECT relationship FROM relationships_to_incidents "
		     "WHERE relationship = :relationship AND type = :type AND coder = :coder");
      query->bindValue(":relationship", relationship);
      query->bindValue(":type", type);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      query->first();
      if (query->isNull(0)) 
	{
	  action4->setEnabled(false);
	}
      if (QAction *action = menu->exec(globalPos)) 
	{
	  if (action->text() == EDITRELATIONSHIPACTION)
	    {
	      editRelationship();
	    }
	  if (action->text() == ASSIGNRELATIONSHIPACTION) 
	    {
	      assignRelationship();
	    }
	  else if (action->text() == UNASSIGNRELATIONSHIPACTION) 
	    {
	      unassignRelationship();
	    }
	  else if (action->text() == UNASSIGNALLACTION) 
	    {
	      unassignAll();
	    }
	}
      delete query;
      delete action1;
      delete action2;
      delete action3;
      delete menu;
    }
  else if (targetIndex.isValid())
    {
      QMenu *menu = new QMenu();
      QAction *action1 = new QAction(ADDRELATIONSHIPACTION, this);
      QAction *action2 = new QAction(EDITRELATIONSHIPTYPEACTION, this);
      menu->addAction(action1);
      menu->addAction(action2);
      if (QAction *action = menu->exec(globalPos)) 
	{
	  if (action->text() == ADDRELATIONSHIPACTION) 
	    {
	      newRelationship();
	    }
	  else if (action->text() == EDITRELATIONSHIPTYPEACTION)
	    {
	      editType();
	    }
	}
      delete action1;
      delete action2;
      delete menu;
    }
  else
    {
      QMenu *menu = new QMenu();
      QAction *action1 = new QAction(ADDRELATIONSHIPTYPEACTION, this);
      menu->addAction(action1);
      if (QAction *action = menu->exec(globalPos)) 
	{
	  if (action->text() == ADDRELATIONSHIPTYPEACTION) 
	    {
	      newType();
	    }
	}
      delete action1;
      delete menu;
    }
}

void RelationshipsWidget::unassignAll() 
{
  QPointer<QMessageBox> warningBox = new QMessageBox(this);
  warningBox->setWindowTitle("Unassign relationships"); 
  warningBox->addButton(QMessageBox::Yes);
  warningBox->addButton(QMessageBox::No);
  warningBox->setIcon(QMessageBox::Warning);
  warningBox->setText("<h2>Are you sure?</h2>");
  warningBox->setInformativeText("This will unassign the selected relationship from all incidents. "
				 "Are you sure you want to proceed?");
  if (warningBox->exec() == QMessageBox::Yes) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      QStandardItem *typeItem = currentItem->parent();
      QString relationship = currentItem->data(Qt::UserRole).toString();
      QString type = typeItem->data(Qt::DisplayRole).toString();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("DELETE FROM relationships_to_incidents "
		     "WHERE relationship = :rel AND type = :type AND coder = :coder");
      query->bindValue(":rel", relationship);
      query->bindValue(":type", type);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      query->prepare("DELETE FROM relationships_to_incidents_sources "
		     "WHERE relationship = :rel AND type = :type AND coder = :coder");
      query->bindValue(":rel", relationship);
      query->bindValue(":type", type);
      query->bindValue(":coder", _selectedCoder);
      query->exec();
      assignedModel->select();
      resetFont(relationshipsTree);
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
      query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      int id = query->value(0).toInt();
      query->exec("SELECT relationship, type, incident, coder FROM relationships_to_incidents");
      while (query->next()) 
	{
	  QString relationship = query->value(0).toString();
	  QString type = query->value(1).toString();
	  int incident = query->value(2).toInt();
	  QString coder = query->value(3).toString();
	  if (incident == id && coder == _selectedCoder) 
	    {
	      boldSelected(relationshipsTree, relationship, type);
	    }
	}
      highlightText();
      unassignRelationshipButton->setEnabled(false);
      delete query;
    }
}

void RelationshipsWidget::previousIncident() 
{
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  query->first();
  int order = query->value(0).toInt();
  if (order != 1) 
    {
      order--;
      query->prepare("UPDATE save_data "
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  delete query;
}

void RelationshipsWidget::jumpIncident() 
{
  setComment();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  AttributeIndexDialog *indexDialog = new AttributeIndexDialog(this, incidentsModel->rowCount());
  indexDialog->exec();
  int order = 0;
  if (indexDialog->getExitStatus() != 1) 
    {
      order = indexDialog->getIndex();
      QSqlQuery *query = new QSqlQuery;
      if (order > 0) 
	{
	  query->prepare("UPDATE save_data "
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
      delete query;
    }
  delete indexDialog;
}

void RelationshipsWidget::nextIncident() 
{
  setComment();
  incidentsModel->select();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  query->first();
  int order = query->value(0).toInt();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order != incidentsModel->rowCount()) 
    {
      order++;
      query->prepare("UPDATE save_data "
		     "SET relationships_record=:new");
      query->bindValue(":new", order);
      query->exec();
      retrieveData();
    }
  delete query;
}

void RelationshipsWidget::toggleMark() 
{
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  query->first();
  int order = query->value(0).toInt();
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

void RelationshipsWidget::previousMarked() 
{
  setComment();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  query->first();
  int order = query->value(0).toInt();
  query->prepare("SELECT ch_order FROM incidents "
		 "WHERE ch_order < :order AND mark = 1 ORDER BY ch_order desc");
  query->bindValue(":order", order);
  query->exec();
  query->first();
  if (order != 1) 
    {
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
    }
  delete query;
}

void RelationshipsWidget::nextMarked() 
{
  setComment();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT relationships_record FROM save_data");
  query->first();
  int order = query->value(0).toInt();
  query->prepare("SELECT ch_order FROM incidents "
		 "WHERE ch_order > :order AND mark = 1 ORDER BY ch_order asc");
  query->bindValue(":order", order);
  query->exec();
  query->first();
  incidentsModel->select();
  while(incidentsModel->canFetchMore())
    incidentsModel->fetchMore();
  if (order != incidentsModel->rowCount()) 
    {
      if (!query->isNull(0)) 
	{
	  order = query->value(0).toInt();
	  query->prepare("UPDATE save_data "
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	}
    }
  delete query;
}


void RelationshipsWidget::highlightSearch(QTextEdit *field, QString text)
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

void RelationshipsWidget::setDescriptionFilter(const QString &text) 
{
  _descriptionFilter = text;
  highlightSearch(descriptionField, text);
}

void RelationshipsWidget::previousDescription() 
{
  setComment();
  if (_descriptionFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
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
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	  highlightSearch(descriptionField, _descriptionFilter);
	}
      delete query;
    }
}

void RelationshipsWidget::nextDescription() 
{
  setComment();
  if (_descriptionFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
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
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	  highlightSearch(descriptionField, _descriptionFilter);
	}
      delete query;
    }
}

void RelationshipsWidget::editIncident()
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
      query->exec("SELECT relationships_record FROM save_data");
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

void RelationshipsWidget::setRawFilter(const QString &text) 
{
  _rawFilter = text;
  highlightSearch(rawField, text);
}

void RelationshipsWidget::previousRaw() 
{
  setComment();
  if (_rawFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
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
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	  highlightSearch(rawField, _rawFilter);
	}
      delete query;
    }
}

void RelationshipsWidget::nextRaw() 
{
  setComment();
  if (_rawFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
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
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	  highlightSearch(rawField, _rawFilter);
	}
      delete query;
    }
}

void RelationshipsWidget::setCommentFilter(const QString &text) 
{
  _commentFilter = text;
  highlightSearch(commentField, text); 
}

void RelationshipsWidget::previousComment() 
{
  setComment();
  if (_commentFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
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
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	  highlightSearch(commentField, _commentFilter); 
	}
      delete query;
    }
}

void RelationshipsWidget::nextComment() 
{
  setComment();
  if (_commentFilter != "") 
    {
      QSqlQuery *query = new QSqlQuery;
      query->exec("SELECT relationships_record FROM save_data");
      query->first();
      int order = query->value(0).toInt();
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
			 "SET relationships_record=:new");
	  query->bindValue(":new", order);
	  query->exec();
	  retrieveData();
	  highlightSearch(commentField, _commentFilter); 
	}
      delete query;
    }
}

void RelationshipsWidget::previousCoded() 
{
  setComment();
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QPersistentModelIndex currentIndex = relationshipsTreeView->currentIndex();
      QString relationship = relationshipsTreeView->currentIndex().data(Qt::UserRole).toString();
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  QStandardItem *typeItem = currentItem->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->exec("SELECT relationships_record FROM save_data");
	  query->first();
	  int currentOrder = query->value(0).toInt(); 
	  query->prepare("SELECT incident, ch_order FROM "
			 "(SELECT incident, ch_order, relationship, type "
			 "FROM relationships_to_incidents "
			 "LEFT JOIN incidents ON relationships_to_incidents.incident = incidents.id "
			 "WHERE ch_order < :order AND relationship = :relationship AND type = :type "
			 "AND coder = :coder)"
			 "ORDER BY ch_order desc");
	  query->bindValue(":order", currentOrder);
	  query->bindValue(":relationship", relationship);
	  query->bindValue(":type", currentType);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  int id = 0;
	  query->first();
	  if (!(query->isNull(0))) 
	    {
	      id = query->value(0).toInt();
	      query->prepare("SELECT ch_order FROM incidents WHERE id = :id");
	      query->bindValue(":id", id);
	      query->exec();
	      query->first();
	      int newOrder = query->value(0).toInt();
	      query->prepare("UPDATE save_data "
			     "SET relationships_record=:new");
	      query->bindValue(":new", newOrder);
	      query->exec();
	      retrieveData();
	    }
	  delete query;
	  relationshipsTreeView->setCurrentIndex(currentIndex);
	  relationshipsTreeView->selectionModel()->select(currentIndex,
							  QItemSelectionModel::SelectCurrent);
	}
    }
}

void RelationshipsWidget::nextCoded() 
{
  setComment();
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QPersistentModelIndex currentIndex = relationshipsTreeView->currentIndex();
      QString relationship = relationshipsTreeView->currentIndex().data(Qt::UserRole).toString();
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  QStandardItem *typeItem = currentItem->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->exec("SELECT relationships_record FROM save_data");
	  query->first();
	  int currentOrder = query->value(0).toInt(); 
	  query->prepare("SELECT incident, ch_order FROM "
			 "(SELECT incident, ch_order, relationship, type "
			 "FROM relationships_to_incidents "
			 "LEFT JOIN incidents ON relationships_to_incidents.incident = incidents.id "
			 "WHERE ch_order > :order AND relationship = :relationship AND type = :type "
			 "AND coder = :coder)"
			 "ORDER BY ch_order asc");
	  query->bindValue(":order", currentOrder);
	  query->bindValue(":relationship", relationship);
	  query->bindValue(":type", currentType);
	  query->bindValue(":coder", _selectedCoder);
	  query->exec();
	  int id = 0;
	  query->first();
	  if (!(query->isNull(0))) 
	    {
	      id = query->value(0).toInt();
	      query->prepare("SELECT ch_order FROM incidents WHERE id = :id");
	      query->bindValue(":id", id);
	      query->exec();
	      query->first();
	      int newOrder = query->value(0).toInt();
	      query->prepare("UPDATE save_data "
			     "SET relationships_record=:new");
	      query->bindValue(":new", newOrder);
	      query->exec();
	      retrieveData();
	    }
	  delete query;
	  relationshipsTreeView->setCurrentIndex(currentIndex);
	  relationshipsTreeView->selectionModel()->select(currentIndex,
							  QItemSelectionModel::SelectCurrent);
	}
    }
}

void RelationshipsWidget::setButtons() 
{
  if (relationshipsTreeView->currentIndex().isValid()) 
    {
      QStandardItem *currentItem = relationshipsTree->
	itemFromIndex(treeFilter->mapToSource(relationshipsTreeView->currentIndex()));
      if (currentItem->parent()) 
	{
	  QString currentRelationship = relationshipsTreeView->
	    currentIndex().data(Qt::UserRole).toString();
	  QStandardItem *typeItem = currentItem->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QSqlQuery *query = new QSqlQuery;
	  query->exec("SELECT relationships_record FROM save_data");
	  query->first();
	  int order = query->value(0).toInt();
	  query->prepare("SELECT id FROM incidents WHERE ch_order = :order ");
	  query->bindValue(":order", order);
	  query->exec();
	  query->first();
	  int id = 0;
	  if (!(query->isNull(0))) 
	    {
	      id = query->value(0).toInt();
	      assignedModel->select();
	      bool empty = false;
	      query->prepare("SELECT relationship, incident "
			     "FROM relationships_to_incidents "
			     "WHERE relationship = :rel AND type = :type AND incident = :inc "
			     "AND coder = :coder");
	      query->bindValue(":rel", currentRelationship);
	      query->bindValue(":type", currentType);
	      query->bindValue(":inc", id);
	      query->bindValue(":coder", _selectedCoder);
	      query->exec();
	      query->first();
	      empty = query->isNull(0);
	      if (!empty) 
		{
		  unassignRelationshipButton->setEnabled(true);
		}
	      else 
		{
		  unassignRelationshipButton->setEnabled(false);
		}
	      query->prepare("SELECT relationship, incident FROM "
			     "relationships_to_incidents_sources "
			     "WHERE relationship = :rel AND incident = :inc AND coder = :coder");
	      query->bindValue(":rel", currentRelationship);
	      query->bindValue(":inc", id);
	      query->bindValue(":coder", _selectedCoder);
	      query->exec();
	      query->first();
	      empty = query->isNull(0);
	      if (!empty) 
		{
		  removeTextButton->setEnabled(true);
		  resetTextsButton->setEnabled(true);
		}
	      else 
		{
		  removeTextButton->setEnabled(false);
		  resetTextsButton->setEnabled(false);
		}
	      assignRelationshipButton->setEnabled(true);
	      previousCodedButton->setEnabled(true);
	      nextCodedButton->setEnabled(true);
	      newRelationshipButton->setEnabled(false);
	      editRelationshipButton->setEnabled(true);
	      editTypeButton->setEnabled(false);
	    }
	  delete query;
	}
      else 
	{
	  newRelationshipButton->setEnabled(true);
	  editRelationshipButton->setEnabled(false);
	  assignRelationshipButton->setEnabled(false);
	  unassignRelationshipButton->setEnabled(false);
	  editTypeButton->setEnabled(true);
	  removeTextButton->setEnabled(false);
	  resetTextsButton->setEnabled(false);
	  previousCodedButton->setEnabled(false);
	  nextCodedButton->setEnabled(false);
	}
    }
  else 
    {
      newRelationshipButton->setEnabled(false);
      editRelationshipButton->setEnabled(false);
      assignRelationshipButton->setEnabled(false);
      unassignRelationshipButton->setEnabled(false);
      editTypeButton->setEnabled(false);
      removeTextButton->setEnabled(false);
      resetTextsButton->setEnabled(false);
      previousCodedButton->setEnabled(false);
      nextCodedButton->setEnabled(false);
    }
}

void RelationshipsWidget::boldSelected(QAbstractItemModel *model, QString name, QString type, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QStandardItem *currentRelationship = relationshipsTree->itemFromIndex(index);
      QString currentName = currentRelationship->data(Qt::UserRole).toString();
      if (currentRelationship->parent()) 
	{
	  QStandardItem *typeItem = currentRelationship->parent();
	  QString currentType = typeItem->data(Qt::DisplayRole).toString();
	  QFont font;
	  if (name == currentName && type == currentType) 
	    {
	      font.setBold(true);
	      currentRelationship->setFont(font);
	      font.setBold(false);
	      font.setUnderline(true);
	      currentRelationship->parent()->setFont(font);
	      font.setUnderline(false);
	    }
	}
      if (model->hasChildren(index)) 
	{
	  boldSelected(model, name, type, index);
	}
    }
}

void RelationshipsWidget::resetFont(QAbstractItemModel *model, QModelIndex parent) 
{
  for(int i = 0; i != model->rowCount(parent); i++) 
    {
      QModelIndex index = model->index(i, 0, parent);
      QString currentName = model->data(index).toString();
      QStandardItem *currentRelationship = relationshipsTree->itemFromIndex(index);
      QFont font;
      font.setBold(false);
      font.setUnderline(false);
      currentRelationship->setFont(font);
      if (model->hasChildren(index)) 
	{
	  resetFont(model, index);
	}
    }
}

void RelationshipsWidget::expandTree() 
{
  relationshipsTreeView->expandAll();
}

void RelationshipsWidget::collapseTree() 
{
  relationshipsTreeView->collapseAll();
}

void RelationshipsWidget::finalBusiness() 
{
  setComment();
}

void RelationshipsWidget::setEventGraphWidget(EventGraphWidget *eventGraphWidgetPtr) 
{
  _eventGraphWidgetPtr = eventGraphWidgetPtr;
}

void RelationshipsWidget::setAttributesWidget(AttributesWidget *attributesWidgetPtr) 
{
  _attributesWidgetPtr = attributesWidgetPtr;
}

void RelationshipsWidget::setNetworkGraphWidget(NetworkGraphWidget *networkGraphWidgetPtr) 
{
  _networkGraphWidgetPtr = networkGraphWidgetPtr;
}

void RelationshipsWidget::resetTree() 
{
  delete relationshipsTree;
  setTree();
}

void RelationshipsWidget::setCurrentCoder(QString coder)
{
  _selectedCoder = coder;
}

bool RelationshipsWidget::eventFilter(QObject *object, QEvent *event) 
{
  if (object == rawField->viewport() && event->type() == QEvent::MouseButtonRelease) 
    {
      selectText();
    }
  else if (((object == descriptionField->viewport() && descriptionField->isEnabled()) ||
	    (object == rawField->viewport() && rawField->isEnabled()) ||
	    (object == timeStampField && timeStampField->isEnabled()) ||
	    (object == sourceField && sourceField->isEnabled())) &&
	   event->type() == QEvent::ContextMenu)
    {
      QContextMenuEvent *context = (QContextMenuEvent*) event;
      QMenu *menu = new QMenu;
      QAction *editAction = new QAction(tr("Edit incident"), this);
      connect(editAction, SIGNAL(triggered()), this, SLOT(editIncident()));
      menu->addAction(editAction);
      menu->exec(context->globalPos());
      delete editAction;
      delete menu;
      return true;
    }
  else if (event->type() == QEvent::Wheel) 
    {
      QWheelEvent *wheelEvent = (QWheelEvent*) event;
      QTextEdit *textEdit = qobject_cast<QTextEdit*>(object);
      QTreeView *treeView = qobject_cast<QTreeView*>(object);
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
      else if (treeView) 
	{
	  if(wheelEvent->modifiers() & Qt::ControlModifier) 
	    {
	      if (wheelEvent->angleDelta().y() > 0) 
		{
		  QFont font = treeView->font();
		  int currentSize = font.pointSize();
		  if (currentSize <= 50) 
		    {
		      int newSize = currentSize + 1;
		      font.setPointSize(newSize);
		      treeView->setFont(font);
		    }
		}
	      else if (wheelEvent->angleDelta().y() < 0) 
		{
		  QFont font = treeView->font();
		  int currentSize = font.pointSize();
		  if (currentSize >= 10) 
		    {
		      int newSize = currentSize - 1;
		      font.setPointSize(newSize);
		      treeView->setFont(font);
		    }
		}
	    }
	}
    }
  return false;
}

