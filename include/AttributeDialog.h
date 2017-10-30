#ifndef ATTRIBUTEDIALOG_H
#define ATTRIBUTEDIALOG_H

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTextEdit>
#include "EventSequenceDatabase.h"

class AttributeDialog : public QDialog {
  Q_OBJECT
  
public:
  AttributeDialog(QWidget *parent = 0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);
  ~AttributeDialog() {};
 
  void submitName(const QString &submittedName);
  void setDescription(const QString &newDescription);				      
  QString getName();
  QString getDescription();		   
  int getExitStatus();
			  
private slots:
  void setName(const QString &newName);
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<EventSequenceDatabase> esd;

  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QPointer<QLineEdit> nameField;

  QPointer<QTextEdit> descriptionField;

  QString name;
  QString description;

  int exitStatus;
};

#endif
