#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QMessageBox>
#include <QWheelEvent>
#include "EventSequenceDatabase.h"
#include "Constants.h"

class RecordDialog : public QDialog {
  Q_OBJECT

public:
  RecordDialog(QWidget *parent=0,
	       EventSequenceDatabase *submittedEsd = new EventSequenceDatabase,
	       QString type = NEW);
  ~RecordDialog() {};

  QString getTimeStamp();
  QString getSource();
  QString getDescription();
  QString getRaw();
  QString getComment();

  void setTimeStamp(QString &text);
  void setSource(QString &text);
  void setDescription(QString &text);
  void setRaw(QString &text);
  void setComment(QString &text);
  void initialize();
  
  int getExitStatus();		      
  bool eventFilter(QObject *object, QEvent *event);
								
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
  
  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;

  int exitStatus;

  QString timeStamp;
  QString description;
  QString raw;
  QString comment;
  QString source;
  
};


#endif
