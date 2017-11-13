#ifndef ATTRIBUTEINDEXDIALOG_H
#define ATTRIBUTEINDEXDIALOG_H

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

class AttributeIndexDialog : public QDialog {
  Q_OBJECT

public:
  AttributeIndexDialog(QWidget *parent = 0, int = 1);
  ~AttributeIndexDialog() {};
  int getIndex();
  int getExitStatus();		

private slots:
  void go();
  void cancel();
  void setIndexText(const QString newIndex);

private:
  QPointer<QLabel> indexLabel;
  QPointer<QLineEdit> indexField;
  QPointer<QPushButton> goButton;
  QPointer<QPushButton> cancelButton;
  int index;
  int exitStatus;
};

#endif
