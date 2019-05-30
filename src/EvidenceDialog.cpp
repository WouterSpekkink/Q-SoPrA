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

#include "../include/EvidenceDialog.h"

EvidenceDialog::EvidenceDialog(int tail, int head, QString type, QString coder, QWidget *parent)
: QDialog(parent)
{
  _tail = tail;
  _head = head;
  _type = type;
  _coder = coder;
  
  typeLabel = new QLabel(tr("<b>Linkage type:</b>"), this);
  coderLabel = new QLabel(tr("<b>Coder:</b>"), this);
  typeReportLabel = new QLabel(type, this);
  coderReportLabel = new QLabel(coder, this);
  tailLabel = new QLabel(tr("<b>Tail text:</b>"), this);
  headLabel = new QLabel(tr("<b>Head text:</b>"), this);
  commentLabel = new QLabel(tr("<b>Comment:</b>"), this);

  tailTextField = new QTextEdit(this);
  tailTextField->setEnabled(false);
  headTextField = new QTextEdit(this);
  headTextField->setEnabled(false);
  commentField = new QTextEdit(this);
  commentField->setEnabled(false);
  
  closeButton = new QPushButton(tr("Close dialog"), this);

  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> detailsLayout = new QHBoxLayout;
  detailsLayout->addWidget(typeLabel);
  detailsLayout->addWidget(typeReportLabel);
  detailsLayout->addWidget(coderLabel);
  detailsLayout->addWidget(coderReportLabel);
  mainLayout->addLayout(detailsLayout);
  QPointer<QFrame> topLine = new QFrame;
  topLine->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(topLine);
  QPointer<QHBoxLayout> topFieldsLayout = new QHBoxLayout;
  QPointer<QVBoxLayout> leftTopFieldsLayout= new QVBoxLayout;
  leftTopFieldsLayout->addWidget(tailLabel);
  leftTopFieldsLayout->addWidget(tailTextField);
  topFieldsLayout->addLayout(leftTopFieldsLayout);
  QPointer<QVBoxLayout> rightTopFieldsLayout = new QVBoxLayout;
  rightTopFieldsLayout->addWidget(headLabel);
  rightTopFieldsLayout->addWidget(headTextField);
  topFieldsLayout->addLayout(rightTopFieldsLayout);
  mainLayout->addLayout(topFieldsLayout);
  mainLayout->addWidget(commentLabel);
  mainLayout->addWidget(commentField);
  mainLayout->addWidget(closeButton);
  closeButton->setMaximumWidth(closeButton->sizeHint().width());

  setLayout(mainLayout);
  getData();
}

void EvidenceDialog::getData()
{
  QSqlQuery *query = new QSqlQuery;
  query->prepare("SELECT tail_text, head_text FROM linkages_sources "
		 "WHERE tail = :tail AND head = :head AND type = :type AND coder = :coder");
  query->bindValue(":tail", _tail);
  query->bindValue(":head", _head);
  query->bindValue(":type", _type);
  query->bindValue(":coder", _coder);
  query->exec();
  query->first();
  if (!query->isNull(0))
    {
      tailTextField->setPlainText(query->value(0).toString());
    }
  if (!query->isNull(1))
    {
      headTextField->setPlainText(query->value(1).toString());
    }
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
}
