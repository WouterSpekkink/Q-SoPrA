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

class LineageColorDialog : public QDialog {
  Q_OBJECT

public:
  LineageColorDialog(QWidget *parent = 0);
  ~LineageColorDialog() {};

  QColor getOriginColor();
  QColor getAncestorColor();
  QColor getDescendantColor();
  QColor getUnrelatedColor();
  int getExitStatus();
			    
private slots:
  void setOriginColor();
  void setAncestorColor();
  void setDescendantColor();
  void setUnrelatedColor();
  void cancelAndClose();
  void saveAndClose();
    
private:
  QPointer<QLabel> lineageLabel;

  QPointer<QPushButton> originButton;
  QPointer<QPushButton> ancestorButton;
  QPointer<QPushButton> descendantButton;
  QPointer<QPushButton> unrelatedButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QColor originColor;
  QColor ancestorColor;
  QColor descendantColor;
  QColor unrelatedColor;

  int exitStatus;
};

#endif
