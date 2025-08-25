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

#include "../include/EvidenceDialog.h"

EvidenceDialog::EvidenceDialog(int tail, int head, QString type, QString coder, QWidget *parent)
  : QDialog(parent)
{
  _tail = tail;
  _head = head;
  _type = type;
  _coder = coder;
  _comment = QString();
  
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
  
  saveCloseButton = new QPushButton(tr("Close dialog"), this);

  connect(saveCloseButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

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
  QPointer<QVBoxLayout> rightTopFieldsLayout = new QVBoxLayout;
  rightTopFieldsLayout->addWidget(headLabel);
  rightTopFieldsLayout->addWidget(headTextField);
  headTextField->setMinimumWidth(500);
  topFieldsLayout->addLayout(rightTopFieldsLayout);
  mainLayout->addLayout(topFieldsLayout);
  mainLayout->addWidget(commentLabel);
  mainLayout->addWidget(commentField);
  mainLayout->addWidget(saveCloseButton);
  saveCloseButton->setMaximumWidth(saveCloseButton->sizeHint().width());

  setLayout(mainLayout);
  getData();
}

void EvidenceDialog::getData()
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
  query->prepare("SELECT raw FROM incidents "
                 "WHERE id = :tail");
  query->bindValue(":tail", _tail);
  query->exec();
  query->first();
  tailTextField->setPlainText(query->value(0).toString());
  query->prepare("SELECT raw FROM incidents "
                 "WHERE id = :head");
  query->bindValue(":head", _head);
  query->exec();
  query->first();
  headTextField->setPlainText(query->value(0).toString());
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

void EvidenceDialog::saveAndClose()
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
  this->close();
}

QString EvidenceDialog::getComment()
{
  return _comment;
}
