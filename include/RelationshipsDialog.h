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
#include "Constants.h"
#include <QSqlTableModel>
#include "EntityTableModel.h"

class RelationshipsDialog : public QDialog {
  Q_OBJECT

public:
  RelationshipsDialog(QWidget *parent = 0);
  ~RelationshipsDialog() {};

  int getExitStatus();
  
  void submitLeftEntity(QString entity);
  void submitRightEntity(QString entity);
  void submitRelationshipType(QString type);
			   
private slots:
  void filterLeftEntity(const QString &text);
  void filterRightEntity(const QString &text);
  
  void assignLeftEntity();
  void assignRightEntity();
  void addEntity();
  void editEntity();
  void editLeftAssignedEntity();
  void editRightAssignedEntity();
  void removeEntities();
  void disableAssign();
  void reset();
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<EntityTableModel> entitiesTable;
  QPointer<QListView> leftEntitiesView;
  QPointer<QListView> rightEntitiesView;
  QPointer<QSortFilterProxyModel> leftEntitiesFilter;
  QPointer<QSortFilterProxyModel> rightEntitiesFilter;
  
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> targetLabel;
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
  QPointer<QLineEdit> leftEntityFilterField;
  QPointer<QLineEdit> rightEntityFilterField;

  QString name;
  QString oldName;
  QString type;

  QString lastSelectedEntity;
  QString currentLeftEntitySelected;
  QString currentRightEntitySelected;
  QString currentLeftEntityFilter;
  QString currentRightEntityFilter;
  QString currentDirectedness;

  int exitStatus;
};

#endif
