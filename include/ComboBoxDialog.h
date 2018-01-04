#ifndef COMBOBOXDIALOG_H
#define COMBOBOXDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "SupportingFunctions.h"

class ComboBoxDialog : public QDialog {
  Q_OBJECT
  
public:
  ComboBoxDialog(QWidget *parent = 0);
  ~ComboBoxDialog() {};

  QString getType();
  int getExitStatus();

private slots:
  void getTypes();
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> typesLabel;
  
  QPointer<QComboBox> typesComboBox;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QString chosenType;

  int exitStatus;
};

#endif
