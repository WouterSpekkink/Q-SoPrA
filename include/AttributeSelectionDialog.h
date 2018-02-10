#ifndef ATTRIBUTESELECTIONDIALOG_H
#define ATTRIBUTESELECTIONDIALOG_H

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

class AttributeSelectionDialog : public QDialog {
  Q_OBJECT

public:
  AttributeSelectionDialog(QWidget *parent = 0, QString submittedType = DEFAULT);
  ~AttributeSelectionDialog() {};

  QString getAttribute();
  int getExitStatus();
  bool getChecked();
			    
private slots:
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
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

  QPointer<QCheckBox> includeValuesCheckBox; 
  
  QString chosenAttribute;

  QString type;

  int exitStatus;
  bool checked;
};

#endif
