#ifndef ATTRIBUTESWIDGET_H
#define ATTRIBUTESWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QDropEvent>
#include "EventSequenceDatabase.h"
#include "AttributeDialog.h"
#include "DeselectableTreeView.h"

class AttributesWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  AttributesWidget(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);
  ~AttributesWidget() {};

private slots:
  void previousIncident();
  void nextIncident();
  void retrieveData();
  void newAttribute();
  void assignAttribute();
  void unassignAttribute();
  void removeUnusedAttributes();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void boldSelected(QAbstractItemModel *model, QString name = "", QModelIndex parent = QModelIndex());
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void treeOrder(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void finalBusiness();
  
private:
  QPointer<EventSequenceDatabase> esd;
  QPointer<AttributeDialog> attributeDialog;
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QSqlTableModel> attributesModel;
  QPointer<QSqlTableModel> assignedModel;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> attributesLabel;

  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;

  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;

  QPointer<QPushButton> previousIncidentButton;
  QPointer<QPushButton> nextIncidentButton;
  QPointer<QPushButton> newAttributeButton;
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;
  
};


#endif
