#ifndef LINKAGETYPEDIALOG_H
#define LINKAGETYPEDIALOG_H

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextEdit>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "SupportingFunctions.h"

class LinkageTypeDialog : public QDialog
{
  Q_OBJECT
  
public:
  LinkageTypeDialog(QWidget *parent = 0);
  ~LinkageTypeDialog() {};

  void submitName(const QString &submittedName);
  void submitDescription (const QString &submittedDescription);
  void submitQuestion(const QString &sumbittedQuestion);
  void submitDirection (const QString &submittedDirection);
  QString getName();
  QString getDescription();
  QString getQuestion();
  QString getDirection();
  void fixDirection();
  int getExitStatus();

private slots:
  void setName(const QString &newName);
  void checkPastButton();
  void checkFutureButton();
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> questionLabel;
  QPointer<QLabel> directionLabel;

  QPointer<QPushButton> pastButton;
  QPointer<QPushButton> futureButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QPointer<QLineEdit> nameField;
  
  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> questionField;

  QString name;
  QString subName;
  QString description;
  QString question;
  QString direction;

  int exitStatus;
};

#endif
