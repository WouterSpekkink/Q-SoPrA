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
#include <QSql>
#include "EventSequenceDatabase.h"
#include "Constants.h"

class WelcomeDialog : public QDialog
{
  Q_OBJECT

public:
  WelcomeDialog(QWidget *parent = 0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);				  
  ~WelcomeDialog() {};

  int getExitStatus();		     
		     
private slots:
  void newDatabase();
  void openDatabase();
  void quitApp();

private:
  QPointer<EventSequenceDatabase> esd;

  QPointer<QLabel> imageLabel;
  QPointer<QLabel> titleLabel;

  QPointer<QPushButton> newDatabaseButton;
  QPointer<QPushButton> openDatabaseButton;
  QPointer<QPushButton> exitButton;

  int exitStatus;
};

#endif
