#ifndef MODECOLORDIALOG_H
#define MODECOLORDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"

class ModeColorDialog : public QDialog
{
  Q_OBJECT

public:
  ModeColorDialog(QWidget *parent = 0,
		  QColor submittedFillColor = QColor(),
		  QColor submittedTextColor = QColor());
  ~ModeColorDialog() {};
  
  QColor getFillColor();
  QColor getTextColor();
  int getExitStatus();

private slots:
  void setFillColor();
  void setTextColor();
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> infoLabel;
  QPointer<QLabel> fillColorLabel;
  QPointer<QLabel> textColorLabel;

  QPointer<QPushButton> fillColorButton;
  QPointer<QPushButton> textColorButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QColor fillColor;
  QColor textColor;

  int exitStatus;
};

#endif
