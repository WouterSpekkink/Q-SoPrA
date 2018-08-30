#ifndef SIMPLETEXTDIALOG_H
#define SIMPLETEXTDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>

class SimpleTextDialog : public QDialog
{
  Q_OBJECT

public:
  SimpleTextDialog(QWidget *parent = 0);
  ~SimpleTextDialog() {};

  QString getText();
  void submitText(const QString &submittedText);
  void setLabel(const QString &submittedLabel);
  int getExitStatus();

private slots:
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> textLabel;
  
  QPointer<QLineEdit> textField;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QString text;
  QString subText;
  int exitStatus;
};

#endif
