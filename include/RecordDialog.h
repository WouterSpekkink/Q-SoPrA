#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include "EventSequenceDatabase.h"

class RecordDialog : public QDialog {
  Q_OBJECT

public:
  RecordDialog(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);
  ~RecordDialog() {};						     QString getTimeStamp();
  QString getSource();
  QString getDescription();
  QString getRaw();
  QString getComment();
  int getExitStatus();		      
		     
private slots:
  void setTimeStamp(const QString &text);
  void setSource(const QString &source);
  void saveAndClose();
  void cancelAndClose();
  
private:
  QPointer<EventSequenceDatabase> esd;
  
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
  
  QPointer<QPlainTextEdit> descriptionField;
  QPointer<QPlainTextEdit> rawField;
  QPointer<QPlainTextEdit> commentField;

  int exitStatus;

  QString timeStamp;
  QString description;
  QString raw;
  QString comment;
  QString source;
  
};


#endif
