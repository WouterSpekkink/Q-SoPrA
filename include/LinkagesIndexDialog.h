#ifndef LINKAGESINDEXDIALOG_H
#define LINKAGESINDEXDIALOG_H

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
#include <sstream>
#include "Constants.h"

class LinkagesIndexDialog : public QDialog {
  Q_OBJECT

public:
  LinkagesIndexDialog(QWidget *parent = 0, int size = 1, const QString &direction = PAST);
  ~LinkagesIndexDialog() {};
  int getTailIndex();
  int getHeadIndex();
  int getExitStatus();		

private slots:
  void go();
  void cancel();
  void setTailIndexText(const QString &newIndex);
  void setHeadIndexText(const QString &newIndex);
  void evaluateIndexes();

private:
  QPointer<QLabel> tailIndexLabel;
  QPointer<QLabel> headIndexLabel;
  QPointer<QLineEdit> tailIndexField;
  QPointer<QLineEdit> headIndexField;
  QPointer<QPushButton> goButton;
  QPointer<QPushButton> cancelButton;
  int max;
  int submittedSize;
  int tailIndex;
  int headIndex;
  int exitStatus;
  QString tailIndexText;
  QString headIndexText;
  QString submittedDirection;
};

#endif
