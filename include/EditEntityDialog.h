#ifndef EDITENTITYDIALOG_H
#define EDITENTITYDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QPointer>
#include <QMessageBox>
#include <QSqlTableModel>
#include "EntitiesFilter.h"
#include "Constants.h"
#include "EntityTableModel.h"
#include "EntityDialog.h"
#include "ZoomableListView.h"
#include "SupportingFunctions.h"

/*
  I need to forward declare the class here because this header is also included in the
  EntitiesFilter header.
*/
class EntitiesFilter;

class EditEntityDialog : public QDialog {
  Q_OBJECT
  friend class EntitiesFilter;

public:
  EditEntityDialog(QWidget *parent = 0);
  ~EditEntityDialog() {};

  int getEntityEdited();
  QString getName();
  QString getLeftEntity();
  QString getRightEntity();
					       
private slots:
  void filterEntity(const QString &text);
  void addEntity();
  void editEntity();
  void updateAfterEdit(const QString name, const QString description, const QString oldName);
  void removeEntities();
  void setButtons();
  void updateTable();
  void closeThis();
  
private:
  QPointer<QLabel> filterLabel;

  QPointer<EntityTableModel> entitiesTable;
  QPointer<ZoomableListView> entitiesView;
  QPointer<EntitiesFilter> entitiesFilter;
    
  QPointer<QPushButton> newEntityButton;
  QPointer<QPushButton> editEntityButton;
  QPointer<QPushButton> removeEntitiesButton;
  QPointer<QPushButton> closeButton;
  
  QPointer<QLineEdit> entityFilterField;

  int entityEdited;

  bool fresh;
};

#endif
