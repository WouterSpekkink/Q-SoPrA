#ifndef RELATIONSHIPTYPEDIALOG_H
#define RELATIONSHIPTYPEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QPointer>
#include <QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QDateTime>
#include <QEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QtSql>
#include <QSqlQuery>
#include "Constants.h"

class RelationshipTypeDialog : public QDialog
{
  Q_OBJECT

public:
  RelationshipTypeDialog(QWidget *parent = 0);
  ~RelationshipTypeDialog() {};

  QString getName();
  QString getDescription();
  QString getDirectedness();
  int getExitStatus();
  void submitName(const QString &submittedName);
  void submitDescription(const QString &submittedDescription);
  void submitDirectedness(const QString &submittedDirectedness);
		     
private slots:
  void setName(const QString &newName);
  
  void checkDirectedButton();
  void checkUndirectedButton();
  
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> directednessLabel;

  QPointer<QPushButton> directedButton;
  QPointer<QPushButton> undirectedButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QPointer<QLineEdit> nameField;

  QPointer<QTextEdit> descriptionField;

  QString name;
  QString subName;
  QString directedness;
  QString description;

  int exitStatus;
};

#endif
