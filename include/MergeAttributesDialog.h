#ifndef MERGEATTRIBUTESDIALOG_H
#define MERGEATTRIBUTESDIALOG_H

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

class MergeAttributesDialog : public QDialog {
  Q_OBJECT

public:
  MergeAttributesDialog(QWidget *parent = 0,
			QString submittedOrigin = DEFAULT,
			QString submittedType = DEFAULT);
  ~MergeAttributesDialog() {};

  QString getAttribute();
  int getExitStatus();
			    
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

  QString chosenAttribute;

  QString type;
  QString origin;

  int exitStatus;
};

#endif
