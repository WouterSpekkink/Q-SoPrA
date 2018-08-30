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

class ComboBoxDialog : public QDialog
{
  Q_OBJECT
  
public:
  ComboBoxDialog(QWidget *parent = 0, QVector<QString> contents = QVector<QString>());
  ~ComboBoxDialog() {};

  QString getSelection();
  int getExitStatus();

private slots:
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> selectionLabel;
  
  QPointer<QComboBox> selectionComboBox;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QString selection;

  int exitStatus;
};

#endif
