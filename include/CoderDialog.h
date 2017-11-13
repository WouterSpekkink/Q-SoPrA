#ifndef CODERDIALOG_H
#define CODERDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>

class CoderDialog : public QDialog {
  Q_OBJECT

public:
  CoderDialog(QWidget *parent=0);
  ~CoderDialog() {};

  QString getName();
  void submitName(const QString &submittedName);		   
  int getExitStatus();					       
					       
private slots:
  void cancelAndClose();
  void saveAndClose();
  
private:
  QPointer<QLabel> nameLabel;

  QPointer<QLineEdit> nameField;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QString name;
  QString subName;
  int exitStatus;
  
};

#endif
