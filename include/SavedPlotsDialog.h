#ifndef SAVEDPLOTSDIALOG_H
#define SAVEDPLOTSDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>
#include <QMessageBox>
#include "Constants.h"

class SavedPlotsDialog : public QDialog
{
  Q_OBJECT

public:
  SavedPlotsDialog(QWidget *parent = 0, QString submittedType = QString());
  ~SavedPlotsDialog() {};

  int getExitStatus();
  QString getSelectedPlot();
		 
private slots:
  void cancelAndClose();
  void loadPlot();
  void removePlot();
  
private:
  QPointer<QLabel> titleLabel;

  QPointer<QComboBox> plotsComboBox;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> loadPlotButton;
  QPointer<QPushButton> removePlotButton;

  QString selectedPlot;
  QString type;
  int exitStatus;
};


#endif
