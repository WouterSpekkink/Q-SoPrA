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
#include <QPlainTextEdit>
#include <QHeaderView>
#include <QPointer>
#include <QVector>
#include <QSqlTableModel>
#include "DeselectableTreeView.h"
#include "AttributeTreeFilter.h"

class EntityDialog : public QDialog {
  Q_OBJECT

public:
  EntityDialog(QWidget *parent = 0);
  ~EntityDialog() {};

  QString getName();
  QString getDescription();
  int getExitStatus();
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
  void buildHierarchy(QStandardItem *top, QString name);
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

  QPointer<QLineEdit> nameField;
  QPointer<QLineEdit> valueField;
  QPointer<QLineEdit> attributesFilterField;

  QPointer<QPlainTextEdit> descriptionField;

  QString name;
  QString description;
  QString oldName;
  QVector<QString> attributes;

  int exitStatus;
};

#endif
