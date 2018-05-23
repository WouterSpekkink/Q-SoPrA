#ifndef EVENTWIDTHDIALOG_H
#define EVENTWIDTHDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QIntValidator>
#include <QPointer>
#include <string>

class EventWidthDialog : public QDialog {
  Q_OBJECT

public:
  EventWidthDialog(QWidget *parent = 0, int = 1);
  ~EventWidthDialog() {};
  int getWidth();
  int getExitStatus();		

private slots:
  void go();
  void cancel();
  void setWidthText(const QString newWidth);

private:
  QPointer<QLabel> widthLabel;
  QPointer<QLineEdit> widthField;
  QPointer<QPushButton> goButton;
  QPointer<QPushButton> cancelButton;
  int width;
  int exitStatus;
};

#endif
