#ifndef LINEAGECOLORDIALOG_H
#define LINEAGECOLORDIALOG_H

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

class LineageColorDialog : public QDialog
{
  Q_OBJECT

public:
  LineageColorDialog(QWidget *parent = 0);
  ~LineageColorDialog() {};

  QColor getOriginFillColor();
  QColor getAncestorFillColor();
  QColor getDescendantFillColor();
  QColor getUnrelatedFillColor();
  QColor getOriginTextColor();
  QColor getAncestorTextColor();
  QColor getDescendantTextColor();
  QColor getUnrelatedTextColor();
  int getExitStatus();
			    
private slots:
  void setOriginFillColor();
  void setAncestorFillColor();
  void setDescendantFillColor();
  void setUnrelatedFillColor();
  void setOriginTextColor();
  void setAncestorTextColor();
  void setDescendantTextColor();
  void setUnrelatedTextColor();
  void cancelAndClose();
  void saveAndClose();
    
private:
  QPointer<QLabel> lineageLabel;
  QPointer<QLabel> fillColorOneLabel;
  QPointer<QLabel> fillColorTwoLabel;
  QPointer<QLabel> fillColorThreeLabel;
  QPointer<QLabel> fillColorFourLabel;
  QPointer<QLabel> textColorOneLabel;
  QPointer<QLabel> textColorTwoLabel;
  QPointer<QLabel> textColorThreeLabel;
  QPointer<QLabel> textColorFourLabel;
  
  QPointer<QPushButton> originFillButton;
  QPointer<QPushButton> ancestorFillButton;
  QPointer<QPushButton> descendantFillButton;
  QPointer<QPushButton> unrelatedFillButton;
  QPointer<QPushButton> originTextButton;
  QPointer<QPushButton> ancestorTextButton;
  QPointer<QPushButton> descendantTextButton;
  QPointer<QPushButton> unrelatedTextButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QColor originFillColor;
  QColor ancestorFillColor;
  QColor descendantFillColor;
  QColor unrelatedFillColor;
  QColor originTextColor;
  QColor ancestorTextColor;
  QColor descendantTextColor;
  QColor unrelatedTextColor;

  int exitStatus;
};

#endif
