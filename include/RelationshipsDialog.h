#ifndef RelationshipsDialog.h
#define RelationshipsDialog.h

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

/*
  TO DO:
  - I should add QSortFilterProxyModels to filter the listwidgets

  - Probably I should use QListView

*/

class RelationsDialog : public QDialog {
  Q_OBJECT

public:
  RelationshipsDialog(QWidget *parent = 0);
  ~RelationshipsDialog() {};

  void submitLeftEntity(QString entity);
  void submitRightEntity(QString entity);
  void submitRelationshipType(QString type);
			   
private slots:
  void setCurrentEntityLeft(QListWidgetItem *item);
  void setCurrentEntityRight(QListWidgetItem *item);

  void filterLeftEntity(const QString &text);
  void filterRightEntity(const QString &text);
  
  void assignLeftEntity();
  void assignRelType();
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
  QPointer<QSqlTableModel> entitiesTable;
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
  QString typeLabel;

  QString lastSelectedEntity;
  QString currentLeftEntity;
  QString currentRightEntity;
  QString currentLeftEntitySelected;
  QString currentRightEntitySelected;
  QString currentLeftEntityFilter;
  QString currentRightEntityFilter;
  QString currentDirectedness;
};

#endif
