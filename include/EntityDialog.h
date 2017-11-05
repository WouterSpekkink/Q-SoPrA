#ifndef ENTITYDIALOG_H
#define ENTITYDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QHeaderView>
#include <QPointer>
#include <QVector>
#include <QSqlTableModel>
#include "DeselectableTreeViewEntities.h"
#include "AttributeTreeFilter.h"
#include "AttributeDialog.h"
#include "Constants.h"

class EntityDialog : public QDialog {
  Q_OBJECT

public:
  EntityDialog(QWidget *parent = 0);
  ~EntityDialog() {};

  QString getName();
  QString getDescription();
  int getExitStatus();
  void submitName(const QString &newName);
  void submitDescription(const QString &newDescription);
  void setNew();
		      
private slots:

  void setValue();
  void getValue();
  void setFilter(const QString &text);
  void expandTree();
  void collapseTree();
  
  void assignAttribute();
  void unassignAttribute();
  void addAttribute();
  void editAttribute();
  void removeUnusedAttributes();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void boldSelected(QAbstractItemModel *model, QString name = "", QModelIndex parent = QModelIndex());
  void cancelAndClose();
  void saveAndClose();
  
private:
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeViewEntities> attributesTreeView;
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QSqlTableModel> attributesModel;
  QPointer<QSqlTableModel> assignedModel;
  QPointer<AttributeTreeFilter> treeFilter;
  QPointer<AttributeDialog> attributeDialog;
  
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;

  QPointer<QPushButton> valueButton;
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QPushButton> expandButton;
  QPointer<QPushButton> collapseButton;

  QPointer<QLineEdit> nameField;
  QPointer<QLineEdit> valueField;
  QPointer<QLineEdit> attributesFilterField;

  QPointer<QTextEdit> descriptionField;

  QString name;
  QString description;
  QString oldName;
  QVector<QString> attributes;

  int exitStatus;

  bool isNew;
};

#endif
