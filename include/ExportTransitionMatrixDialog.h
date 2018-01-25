#ifndef EXPORTTRANSITIONMATRIXDIALOG_H
#define EXPORTTRANSITIONMATRIXDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "SupportingFunctions.h"

class ExportTransitionMatrixDialog : public QDialog {
  Q_OBJECT

public:
  ExportTransitionMatrixDialog(QWidget *parent = 0);
  ~ExportTransitionMatrixDialog() {};

  bool isMode();
  bool isProbability();
  int getExitStatus();
		      
private slots:
  void setModes();
  void setAttributes();
  void setRaw();
  void setProb();
  
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> probLabel;

  QPointer<QPushButton> modesButton;
  QPointer<QPushButton> attributesButton;
  QPointer<QPushButton> rawButton;
  QPointer<QPushButton> probButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  int exitStatus;
};

#endif
