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

#include "../include/AddEvidenceDialog.h"

AddEvidenceDialog::AddEvidenceDialog(int tail, int head, QString type, QString coder, QWidget *parent)
: QDialog(parent)
{
  _tail = tail;
  _head = head;
  _type = type;
  _coder = coder;
  _comment = QString();
  _exitStatus = 1;
  
  typeLabel = new QLabel(tr("<b>Linkage type:</b>"), this);
  coderLabel = new QLabel(tr("<b>Coder:</b>"), this);
  typeReportLabel = new QLabel(type, this);
  coderReportLabel = new QLabel(coder, this);
  tailLabel = new QLabel(tr("<b>Tail text:</b>"), this);
  headLabel = new QLabel(tr("<b>Head text:</b>"), this);
  commentLabel = new QLabel(tr("<b>Comment:</b>"), this);

  tailTextField = new QTextEdit(this);
  tailTextField->setReadOnly(true);
  headTextField = new QTextEdit(this);
  headTextField->setReadOnly(true);
  commentField = new QTextEdit(this);

  markEvidenceButton = new QPushButton(tr("Mark Evidence"), this);
  markEvidenceButton->setEnabled(false);
  clearEvidenceButton = new QPushButton(tr("Clear Evidence"), this);
  clearEvidenceButton->setEnabled(false);
  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  saveCloseButton = new QPushButton(tr("Save"), this);

  connect(markEvidenceButton, SIGNAL(clicked()), this, SLOT(markEvidence()));
  connect(clearEvidenceButton, SIGNAL(clicked()), this, SLOT(clearEvidence()));
  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

  headTextField->installEventFilter(this);
  tailTextField->installEventFilter(this);
  tailTextField->viewport()->installEventFilter(this);
  headTextField->viewport()->installEventFilter(this);
  
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> detailsLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> detailsLeftLayout = new QHBoxLayout;
  QPointer<QHBoxLayout> detailsRightLayout = new QHBoxLayout;
  detailsLeftLayout->addWidget(typeLabel);
  detailsLeftLayout->addWidget(typeReportLabel);
  detailsLeftLayout->setAlignment(Qt::AlignLeft);
  detailsLayout->addLayout(detailsLeftLayout);
  detailsRightLayout->addWidget(coderLabel);
  detailsRightLayout->addWidget(coderReportLabel);
  detailsRightLayout->setAlignment(Qt::AlignLeft);
  detailsLayout->addLayout(detailsRightLayout);
  mainLayout->addLayout(detailsLayout);
  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftTopFieldsLayout= new QVBoxLayout;
  leftTopFieldsLayout->addWidget(tailLabel);
  leftTopFieldsLayout->addWidget(tailTextField);
  tailTextField->setMinimumWidth(500);
  topFieldsLayout->addLayout(leftTopFieldsLayout);
  QPointer<QVBoxLayout> middleLayout = new QVBoxLayout;
  middleLayout->addWidget(markEvidenceButton);
  markEvidenceButton->setMaximumWidth(markEvidenceButton->sizeHint().width());
  middleLayout->setAlignment(markEvidenceButton, Qt::AlignHCenter | Qt::AlignVCenter);
  middleLayout->addWidget(clearEvidenceButton);
  clearEvidenceButton->setMaximumWidth(clearEvidenceButton->sizeHint().width());
  middleLayout->setAlignment(clearEvidenceButton, Qt::AlignHCenter | Qt::AlignVCenter);
  topFieldsLayout->addLayout(middleLayout);
  QPointer<QVBoxLayout> rightTopFieldsLayout = new QVBoxLayout;
  rightTopFieldsLayout->addWidget(headLabel);
  rightTopFieldsLayout->addWidget(headTextField);
  headTextField->setMinimumWidth(500);
  topFieldsLayout->addLayout(rightTopFieldsLayout);
  mainLayout->addLayout(topFieldsLayout);
  mainLayout->addWidget(commentLabel);
  mainLayout->addWidget(commentField);
  QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(cancelCloseButton);
  cancelCloseButton->setMaximumWidth(cancelCloseButton->sizeHint().width());
  buttonLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);
  
  getData();
}

void AddEvidenceDialog::getData()
{
  QTextCursor currentTailPos = tailTextField->textCursor();
  QTextCursor currentHeadPos = headTextField->textCursor();
  QTextCharFormat format;
  format.setFontWeight(QFont::Normal);
  format.setUnderlineStyle(QTextCharFormat::NoUnderline);
  tailTextField->selectAll();
  tailTextField->textCursor().mergeCharFormat(format);
  QTextCursor tailCursor = tailTextField->textCursor();
  tailCursor.movePosition(QTextCursor::Start);
  tailTextField->setTextCursor(tailCursor);
  headTextField->selectAll();
  headTextField->textCursor().mergeCharFormat(format);
  QTextCursor headCursor = headTextField->textCursor();
  headCursor.movePosition(QTextCursor::Start);
  headTextField->setTextCursor(headCursor);
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT raw, ch_order FROM incidents "
		 "WHERE id = :tail");
  query->bindValue(":tail", _tail);
  query->exec();
  query->first();
  query->prepare("SELECT istail, source_text FROM linkages_sources "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", _tail);
  query->bindValue(":head", _head);
  query->bindValue(":type", _type);
  query->bindValue(":coder", _coder);
  query->exec();
  while (query->next())
    {
      bool isTail = false;
      if (query->value(0).toInt() == true)
	{
	  isTail = true;
	}
      if (!query->isNull(1) && isTail)
	{
	  QString tailText = query->value(1).toString();
	  QVector<QString> tailBlocks = splitLines(tailText);
	  QVectorIterator<QString> it(tailBlocks);
	  while (it.hasNext()) 
	    {
	      QString currentLine = it.next();
	      while (tailTextField->find(currentLine, QTextDocument::FindWholeWords)) 
		{
		  format.setFontWeight(QFont::Bold);
		  format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
		  format.setUnderlineColor(Qt::blue);
		  tailTextField->textCursor().mergeCharFormat(format);
		}
	    }
	  tailCursor = tailTextField->textCursor();
	  tailCursor.movePosition(QTextCursor::Start);
	  tailTextField->setTextCursor(tailCursor);
	  clearEvidenceButton->setEnabled(true);
	}
      else if (!query->isNull(1) && !isTail)
	{
	  QString headText = query->value(1).toString();
	  QVector<QString> headBlocks = splitLines(headText);
	  QVectorIterator<QString> it(headBlocks);
	  while (it.hasNext()) 
	    {
	      QString currentLine = it.next();
	      while (headTextField->find(currentLine, QTextDocument::FindWholeWords)) 
		{
		  format.setFontWeight(QFont::Bold);
		  format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
		  format.setUnderlineColor(Qt::blue);
		  headTextField->textCursor().mergeCharFormat(format);
		}
	    }
	  headCursor = headTextField->textCursor();
	  headCursor.movePosition(QTextCursor::Start);
	  headTextField->setTextCursor(headCursor);
	  clearEvidenceButton->setEnabled(true);
	}
    }
  currentTailPos.movePosition(QTextCursor::Start);
  currentHeadPos.movePosition(QTextCursor::Start);
  query->prepare("SELECT comment FROM linkage_comments "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", _tail);
  query->bindValue(":head", _head);
  query->bindValue(":type", _type);
  query->bindValue(":coder", _coder);
  query->exec();
  query->first();
  if (!query->isNull(0))
    {
      commentField->setPlainText(query->value(0).toString());
    }
  delete query;
  tailTextField->verticalScrollBar()->setValue(0);
  headTextField->verticalScrollBar()->setValue(0);
}

void AddEvidenceDialog::cancelAndClose()
{
  _exitStatus = 1;
  this->close();
}

void AddEvidenceDialog::saveAndClose()
{
  QString currentComment = commentField->toPlainText();
  if (currentComment != "")
    {
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT comment FROM linkage_comments "
		     "WHERE type = :type AND tail = :tail AND head = :head");
      query->bindValue(":type", _type);
      query->bindValue(":tail", _tail);
      query->bindValue(":head", _head);
      query->exec();
      query->first();
      if (!query->isNull(0)) 
	{
	  query->prepare("UPDATE linkage_comments "
			 "SET comment = :comment, coder = :coder "
			 "WHERE type = :type AND tail = :tail AND head = :head");
	}
      else
	{
	  query->prepare("INSERT INTO linkage_comments (comment, coder, type, tail, head) "
			 "VALUES (:comment, :coder, :type, :tail, :head)");
	}
      query->bindValue(":comment", currentComment);
      query->bindValue(":coder", _coder);
      query->bindValue(":type", _type);
      query->bindValue(":tail", _tail);
      query->bindValue(":head", _head);
      query->exec();      
      delete query;
      _comment = _coder + " - " + currentComment;
    }
  if (!_markedTailEvidence.empty() || !_markedHeadEvidence.empty())
    {
      storeEvidence();
    }
  _exitStatus = 0;
  this->close();
}

void AddEvidenceDialog::selectTailText() 
{
  if (tailTextField->textCursor().selectedText().trimmed() != "") 
    {
      int end = 0;
      int begin = 0;
      QTextCursor selectCursor = tailTextField->textCursor();
      if (tailTextField->textCursor().anchor() >= tailTextField->textCursor().position()) 
	{
	  begin = tailTextField->textCursor().position();
	  end = tailTextField->textCursor().anchor();
	}
      else 
	{
	  begin = tailTextField->textCursor().anchor();
	  end = tailTextField->textCursor().position();
	}
      selectCursor.setPosition(begin);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      tailTextField->setTextCursor(selectCursor);
      while (tailTextField->textCursor().selectedText()[0].isSpace()) 
	{
	  begin++;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  tailTextField->setTextCursor(selectCursor);
	}
      while (tailTextField->textCursor().selectedText()[tailTextField->textCursor().
						       selectedText().length() - 1].isSpace()) 
	{
	  end--;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  tailTextField->setTextCursor(selectCursor);
	}
      selectCursor.setPosition(begin);
      selectCursor.movePosition(QTextCursor::StartOfWord);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      if (!tailTextField->toPlainText()[end].isPunct()) 
	{
	  selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	}
      tailTextField->setTextCursor(selectCursor);
    }
  markEvidenceButton->setEnabled(false);
  if (tailTextField->textCursor().selectedText() != "" ||
      headTextField->textCursor().selectedText() != "")
    {
      markEvidenceButton->setEnabled(true);
    }
}

void AddEvidenceDialog::selectHeadText() 
{
  if (headTextField->textCursor().selectedText().trimmed() != "") 
    {
      int end = 0;
      int begin = 0;
      QTextCursor selectCursor = headTextField->textCursor();
      if (headTextField->textCursor().anchor() >= headTextField->textCursor().position()) 
	{
	  begin = headTextField->textCursor().position();
	  end = headTextField->textCursor().anchor();
	}
      else 
	{
	  begin = headTextField->textCursor().anchor();
	  end = headTextField->textCursor().position();
	}
      selectCursor.setPosition(begin);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      headTextField->setTextCursor(selectCursor);
      while (headTextField->textCursor().selectedText()[0].isSpace()) 
	{
	  begin++;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  headTextField->setTextCursor(selectCursor);
	}
      while (headTextField->textCursor().selectedText()[headTextField->textCursor().
						       selectedText().length() - 1].isSpace()) 
	{
	  end--;
	  selectCursor.setPosition(begin);
	  selectCursor.setPosition(end, QTextCursor::KeepAnchor);
	  headTextField->setTextCursor(selectCursor);
	}
      selectCursor.setPosition(begin);
      selectCursor.movePosition(QTextCursor::StartOfWord);
      selectCursor.setPosition(end, QTextCursor::KeepAnchor);
      if (!headTextField->toPlainText()[end].isPunct()) 
	{
	  selectCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	}
      headTextField->setTextCursor(selectCursor);
    }
  markEvidenceButton->setEnabled(false);
  if (tailTextField->textCursor().selectedText() != "" ||
      headTextField->textCursor().selectedText() != "")
    {
      markEvidenceButton->setEnabled(true);
    }
}

void AddEvidenceDialog::markEvidence()
{
  QString tailText = tailTextField->textCursor().selectedText().trimmed();
  QString headText = headTextField->textCursor().selectedText().trimmed();
  if (tailText != "")
    {
      _markedTailEvidence.push_back(tailText);
    }
  if (headText != "")
    {
      _markedHeadEvidence.push_back(headText);
    }
  QTextCursor tailCursor = tailTextField->textCursor();
  tailCursor.clearSelection();
  tailTextField->setTextCursor(tailCursor);
  QTextCursor headCursor = headTextField->textCursor();
  headCursor.clearSelection();
  headTextField->setTextCursor(headCursor);
  highlightText();
}

void AddEvidenceDialog::clearEvidence()
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail, head FROM coders_to_linkage_types "
		 "WHERE coder = :coder AND type = :type");
  query->bindValue(":coder", _coder);
  query->bindValue(":type", _type);
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
  query->bindValue(":type", _type);
  query->bindValue(":coder", _coder);
  query->exec();
  _markedTailEvidence.clear();
  _markedHeadEvidence.clear();
  delete query;
  highlightText();
}

void AddEvidenceDialog::highlightText() 
{
  QTextCursor currentTailPos = tailTextField->textCursor();
  QTextCursor currentHeadPos = headTextField->textCursor();
  int tailBarPos = tailTextField->verticalScrollBar()->value();
  int headBarPos = headTextField->verticalScrollBar()->value();
  QTextCharFormat format;
  format.setFontWeight(QFont::Normal);
  format.setUnderlineStyle(QTextCharFormat::NoUnderline);
  tailTextField->selectAll();
  tailTextField->textCursor().mergeCharFormat(format);
  QTextCursor tailCursor = tailTextField->textCursor();
  tailCursor.movePosition(QTextCursor::Start);
  tailTextField->setTextCursor(tailCursor);
  headTextField->selectAll();
  headTextField->textCursor().mergeCharFormat(format);
  QTextCursor headCursor = headTextField->textCursor();
  headCursor.movePosition(QTextCursor::Start);
  headTextField->setTextCursor(headCursor);
  QVectorIterator<QString> it(_markedTailEvidence);
  while (it.hasNext())
  {
    QString tailText = it.next();
    QVector<QString> tailBlocks = splitLines(tailText);
    QVectorIterator<QString> it2(tailBlocks);
    while (it2.hasNext()) 
      {
	QString currentLine = it2.next();
	while (tailTextField->find(currentLine, QTextDocument::FindWholeWords)) 
	  {
	    format.setFontWeight(QFont::Bold);
	    format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
	    format.setUnderlineColor(Qt::blue);
	    tailTextField->textCursor().mergeCharFormat(format);
	  }
      }
    tailCursor = tailTextField->textCursor();
    tailCursor.movePosition(QTextCursor::Start);
    tailTextField->setTextCursor(tailCursor);
  }
  tailTextField->setTextCursor(currentTailPos);
  QVectorIterator<QString> it3(_markedHeadEvidence);
  while (it3.hasNext())
    {
      QString headText = it3.next();
      QVector<QString> headBlocks = splitLines(headText);
      QVectorIterator<QString> it4(headBlocks);
      while (it4.hasNext()) 
	{
	  QString currentLine = it4.next();
	  while (headTextField->find(currentLine, QTextDocument::FindWholeWords)) 
	    {
	      format.setFontWeight(QFont::Bold);
	      format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
	      format.setUnderlineColor(Qt::blue);
	      headTextField->textCursor().mergeCharFormat(format);
	    }
	}
      headCursor = headTextField->textCursor();
      headCursor.movePosition(QTextCursor::Start);
      headTextField->setTextCursor(headCursor);
    }
  headTextField->setTextCursor(currentHeadPos);
  tailTextField->verticalScrollBar()->setValue(tailBarPos);
  headTextField->verticalScrollBar()->setValue(headBarPos);
  if (_markedTailEvidence.empty() && _markedHeadEvidence.empty())
    {
      clearEvidenceButton->setEnabled(false);
    }
  else
    {
      clearEvidenceButton->setEnabled(true);
    }
}

void AddEvidenceDialog::storeEvidence()
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("DELETE FROM linkages_sources "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", _tail);
  query->bindValue(":head", _head);
  query->bindValue(":type", _type);
  query->bindValue(":coder", _coder);
  query->exec();
  QVectorIterator<QString> it(_markedTailEvidence);
  while (it.hasNext())
    {
      QString currentEvidence = it.next();
      query->prepare("INSERT INTO linkages_sources "
		     "(tail, head, type, coder, istail, source_text) "
		     "VALUES (:tail, :head, :type, :coder, :istail, :source_text)");
      query->bindValue(":tail", _tail);
      query->bindValue(":head", _head);
      query->bindValue(":type", _type);
      query->bindValue(":coder", _coder);
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
      query->bindValue(":tail", _tail);
      query->bindValue(":head", _head);
      query->bindValue(":type", _type);
      query->bindValue(":coder", _coder);
      query->bindValue(":istail", 0);
      query->bindValue(":source_text", currentEvidence);
      query->exec();
    }
  delete query;
}

bool AddEvidenceDialog::eventFilter(QObject *object, QEvent *event) 
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
  else if (object == tailTextField->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
      selectTailText();	
    }
  else if (object == headTextField->viewport() && event->type() == QEvent::MouseButtonRelease)
    {
      selectHeadText();
    }
  return false;
}

int AddEvidenceDialog::getExitStatus()
{
  return _exitStatus;
}

QString AddEvidenceDialog::getComment()
{
  return _comment;
}
