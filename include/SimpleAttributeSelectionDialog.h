#ifndef SIMPLEATTRIBUTESELECTIONDIALOG_H
#define SIMPLEATTRIBUTESELECTIONDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QPointer>
#include <QtSql>
#include "Constants.h"
#include "DeselectableTreeView.h"
#include "AttributeTreeFilter.h"
#include "SupportingFunctions.h"

class SimpleAttributeSelectionDialog : public QDialog
{
  Q_OBJECT

public:
  SimpleAttributeSelectionDialog(QWidget *parent = 0, QString submittedType = DEFAULT);
  ~SimpleAttributeSelectionDialog() {};

  QString getAttribute();
  int getExitStatus();
  bool isEntity();
			    
private slots:
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void buildEntities(QStandardItem *top, QString name);
  void changeFilter(const QString &text);
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
  
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QString chosenAttribute;

  QString type;

  int exitStatus;
  bool entity;
};

#endif
