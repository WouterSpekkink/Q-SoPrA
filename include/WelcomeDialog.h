#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QPointer>

class WelcomeDialog : public QDialog {
  Q_OBJECT

public:
  WelcomeDialog(QWidget *parent=0);

private slots:

private:
  QPointer<QLabel> titleLabel;
  QPointer<QPushButton> newDataBaseButton;
  QPointer<QPushButton> openDataBaseButton;
  QPointer<QPushButton> exitButton;
};

#endif
