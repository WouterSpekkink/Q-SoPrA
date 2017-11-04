#ifndef ENTITYDIALOG_H
#define ENTITYDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QPlainTextEdit>
#include "../include/AttributesDialog.h"
#include "../include/DataInterface.h"
#include "../include/Logger.h"

class EntityDialog : public QDialog {
  Q_OBJECT

public:
  EntityDialog(QWidget *parent = 0);
  ~EntityDialog() {};

  QString getOldName();
  Qstring getNewName();
  QString getDescription();
  int getExitstatus();
  void submitName(const QString &submitName);
  void submitDescription(const QString &submitDescription);
		      
private slots:

  void setName(const QString &newName);
  void setValue(const QString &newValue);
  void setFilter(const QString &text);
  
  void assignAttribute();
  void unassignAttribute();
  void addAttribute();
  void editAttribute();
  void removeUnusedAttributes();

  void setTree();
  void buildHierarchy();
  void cancelAndClose();
  void saveAndClose();
  
private:
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QSqlTableModel> attributesModel;
  QPointer<QSqlTableModel> assignedModel;
  QPointer<AttributeTreeFilter> treeFilter;
  
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> valueLabel;

  QPointer<QPushButton> valueButton;
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QPointer<QLineEdit> nameField;
  QPointer<QLineEdit> valueField;
  QPointer<QLineEdit> attributesFilterField;

  QPointer<QPlainTextEdit> descriptionField;

  Qstring name;
  Qstring oldName;

  int exitStatus;
};

#endif
