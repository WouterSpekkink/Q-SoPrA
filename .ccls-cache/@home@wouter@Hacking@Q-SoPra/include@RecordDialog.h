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

#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QMessageBox>
#include <QWheelEvent>
#include <QShortcut>
#include "TextEdit.h"
#include "EventSequenceDatabase.h"
#include "Constants.h"

class RecordDialog : public QDialog
{
  Q_OBJECT

public:
  // Constructor and destructor
  RecordDialog(QWidget *parent = 0);
  ~RecordDialog() {};

  // Setters
  void setTimeStamp(QString &timeStamp);
  void setSource(QString &source);
  void setDescription(QString &description);
  void setRaw(QString &raw);
  void setComment(QString &comment);
  void initialize();

  // Getters
  QString getTimeStamp();
  QString getSource();
  QString getDescription();
  QString getRaw();
  QString getComment();
  int getExitStatus();
  								
private slots:
  // Private member functions
  void saveAndClose();
  void cancelAndClose();
  bool eventFilter(QObject *object, QEvent *event);

private:
  // Interface elements
  QPointer<QLabel> titleLabel;
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QPushButton> saveRecordButton;
  QPointer<QPushButton> cancelButton;
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<TextEdit> descriptionField;
  QPointer<TextEdit> rawField;
  QPointer<TextEdit> commentField;
  QPointer<QShortcut> simplifyShortcut;

  // Private variables
  int _exitStatus;
  QString _timeStamp;
  QString _description;
  QString _raw;
  QString _comment;
  QString _source;
  
};


#endif
