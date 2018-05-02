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
#include <QSqlTableModel>
#include "EntitiesFilter.h"
#include "Constants.h"
#include "EntityTableModel.h"
#include "EntityDialog.h"
#include "ZoomableListView.h"
#include "SupportingFunctions.h"
#include "EventGraphWidget.h"
#include "OccurrenceGraphWidget.h"
#include "AttributesWidget.h"

class EntitiesFilter;
class AttributesWidget;

class RelationshipsDialog : public QDialog {
  Q_OBJECT
  friend class EntitiesFilter;
  friend class RelationshipsWidget;

public:
  RelationshipsDialog(QWidget *parent = 0);
  ~RelationshipsDialog() {};

  int getExitStatus();
  int getEntityEdited();
  QString getName();
  QString getLeftEntity();
  QString getRightEntity();
  void submitName(QString name);
  void submitLeftEntity(QString entity);
  void submitRightEntity(QString entity);
  void submitType(QString type);
  void submitDescription(QString description);
  void submitDirectedness(QString directedness);					     
  void setEventGraph(EventGraphWidget *egw);
  void setOccurrenceGraph(OccurrenceGraphWidget *ogw);
  void setAttributesWidget(AttributesWidget *aw);
						     
private slots:
  void filterEntity(const QString &text);
  void assignLeftEntity();
  void assignRightEntity();
  void addEntity();
  void editEntity();
  void updateAfterEdit(const QString name, const QString description, const QString oldName);
  void editLeftAssignedEntity();
  void editRightAssignedEntity();
  void removeEntities();
  void reset();
  void updateTable();
  void setButtons();
  void cancelAndClose();
  void saveAndClose();
    
private:
  QPointer<EntityTableModel> entitiesTable;
  QPointer<ZoomableListView> entitiesView;
  QPointer<EntitiesFilter> entitiesFilter;
  EventGraphWidget *eventGraph;
  OccurrenceGraphWidget *occurrenceGraph;
  AttributesWidget *attributesWidget;
  
  QPointer<QLabel> filterLabel;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> tailLabel;
  QPointer<QLabel> headLabel;
  QPointer<QLabel> selectedSourceLabel;
  QPointer<QLabel> selectedTypeLabel;
  QPointer<QLabel> selectedTargetLabel;

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
  
  QPointer<QLineEdit> entityFilterField;

  QString name;
  QString oldName;
  QString type;
  
  int exitStatus;
  int entityEdited;

  bool fresh;
};

#endif
