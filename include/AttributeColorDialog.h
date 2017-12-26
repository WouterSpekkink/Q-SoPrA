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
#include "DeselectableTreeView.h"
#include "AttributeTreeFilter.h"

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
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void changeFilter(const QString &text);
  void setColor();
  void setTextColor();
  void setAttribute();
  void cancelAndClose();
  void saveAndClose();
    
private:
  QPointer<QLabel> attributeLabel;

  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<QLabel> attributesFilterLabel;
  
  QPointer<QLineEdit> attributesFilterField;
  
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
