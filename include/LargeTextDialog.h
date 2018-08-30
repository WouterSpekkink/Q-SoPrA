#ifndef LARGETEXTDIALOG_H
#define LARGETEXTDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>

class LargeTextDialog : public QDialog
{
  Q_OBJECT

public:
  LargeTextDialog(QWidget *parent = 0);
  ~LargeTextDialog() {};

  QString getText();
  void submitText(const QString &submittedText);
  void setLabel(const QString &submittedLabel);
  int getExitStatus();
  void setEmptyAllowed(bool status);
		     
private slots:
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> textLabel;
  
  QPointer<QTextEdit> textField;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QString text;
  QString subText;
  int exitStatus;
  bool emptyAllowed;
};

#endif
