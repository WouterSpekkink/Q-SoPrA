#ifndef ATTRIBUTECOLORDIALOG_H
#define ATTRIBUTECOLORDIALOG_H

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

class AttributeColorDialog : public QDialog {
  Q_OBJECT

public:
  AttributeColorDialog(QWidget *parent = 0, QString submittedType = DEFAULT);
  ~AttributeColorDialog() {};

  QColor getColor();
  QColor getTextColor();
  QString getAttribute();
  int getExitStatus();
			    
private slots:
  void getAttributes();
  void setColor();
  void setTextColor();
  void setAttribute(const QString &newAttribute);
  void cancelAndClose();
  void saveAndClose();
  void addChildren(QString father, int indent);
    
private:
  QPointer<QLabel> attributeLabel;

  QPointer<QListWidget> attributeListWidget;

  QPointer<QPushButton> colorButton;
  QPointer<QPushButton> textColorButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QColor chosenColor;
  QColor chosenTextColor;
  QString chosenAttribute;

  QString type;

  int exitStatus;
};

#endif
