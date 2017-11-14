#ifndef RELATIONSHIPSDIALOG_H
#define RELATIONSHIPSDIALOG_H

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
#include "Constants.h"
#include <QSqlTableModel>
#include "EntityTableModel.h"
#include "EntityDialog.h"
#include "ZoomableListView.h"

class RelationshipsDialog : public QDialog {
  Q_OBJECT

public:
  RelationshipsDialog(QWidget *parent = 0);
  ~RelationshipsDialog() {};

  int getExitStatus();
  QString getName();
  QString getLeftEntity();
  QString getRightEntity();
  void submitName(QString name);
  void submitLeftEntity(QString entity);
  void submitRightEntity(QString entity);
  void submitType(QString type);
  void submitDescription(QString description);
  void submitDirectedness(QString directedness);					     
			   
private slots:
  void filterEntity(const QString &text);
  void assignLeftEntity();
  void assignRightEntity();
  void addEntity();
  void editEntity();
  void editLeftAssignedEntity();
  void editRightAssignedEntity();
  void removeEntities();
  void reset();
  void cancelAndClose();
  void saveAndClose();
  
private:
  QPointer<EntityTableModel> entitiesTable;
  QPointer<ZoomableListView> entitiesView;
  QPointer<QSortFilterProxyModel> entitiesFilter;
  
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> tailLabel;
  QPointer<QLabel> headLabel;
  QPointer<QLabel> selectedSourceLabel;
  QPointer<QLabel> selectedTypeLabel;
  QPointer<QLabel> selectedTargetLabel;
  QPointer<QLabel> durationLabel;

  QPointer<QPushButton> resetButton;
  QPointer<QPushButton> newEntityButton;
  QPointer<QPushButton> editEntityButton;
  QPointer<QPushButton> editLeftAssignedEntityButton;
  QPointer<QPushButton> editRightAssignedEntityButton;
  QPointer<QPushButton> removeEntitiesButton;
  QPointer<QPushButton> assignLeftEntityButton;
  QPointer<QPushButton> assignRightEntityButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QPointer<QLineEdit> durationField;
  QPointer<QLineEdit> entityFilterField;

  QString name;
  QString oldName;
  QString type;
  int exitStatus;
};

#endif
