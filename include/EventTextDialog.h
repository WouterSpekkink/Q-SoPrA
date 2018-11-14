#ifndef EVENTTEXTDIALOG_H
#define EVENTTEXTDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>

class EventTextDialog : public QDialog
{
  Q_OBJECT

public:
  EventTextDialog(QWidget *parent = 0);
  ~EventTextDialog() {};

  QString getText();
  QString getTiming();
  void submitText(const QString &submittedText);
  void submitTiming(const QString &submittedTiming);
  int getExitStatus();
		     
private slots:
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> timingLabel;
  QPointer<QLabel> textLabel;

  QPointer<QLineEdit> timingField;
  
  QPointer<QTextEdit> textField;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QString text;
  QString timing;
  QString subText;
  QString subTiming;
  int exitStatus;
};

#endif
