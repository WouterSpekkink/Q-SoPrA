#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPointer>
#include <QFileInfo>
#include <QtSql>
#include "EventSequenceDatabase.h"

class WelcomeDialog : public QDialog {
  Q_OBJECT

public:
  WelcomeDialog(EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);				  
  ~WelcomeDialog() {};

  int getExitStatus();		     
		     
private slots:
  void newDatabase();

  void quitApp();

private:
  QPointer<QLabel> titleLabel;

  QPointer<QPushButton> newDatabaseButton;
  QPointer<QPushButton> openDatabaseButton;
  QPointer<QPushButton> exitButton;

  EventSequenceDatabase *esd;

  int exitStatus;
};

#endif
