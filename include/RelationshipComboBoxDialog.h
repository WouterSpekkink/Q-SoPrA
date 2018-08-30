#ifndef RELATIONSHIPCOMBOBOXDIALOG_H
#define RELATIONSHIPCOMBOBOXDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "SupportingFunctions.h"

class RelationshipComboBoxDialog : public QDialog
{
  Q_OBJECT
  
public:
  RelationshipComboBoxDialog(QWidget *parent = 0);
  ~RelationshipComboBoxDialog() {};

  QString getSelection();
  int getExitStatus();
  bool tailSelected();
  bool headSelected();

private slots:
  void getRelationships();
  void setControls(const QString &selection);
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> selectionLabel;
  
  QPointer<QComboBox> selectionComboBox;

  QPointer<QCheckBox> includeTailCheckBox;
  QPointer<QCheckBox> includeHeadCheckBox;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QString selection;

  int exitStatus;
  bool tail;
  bool head;
};

#endif
