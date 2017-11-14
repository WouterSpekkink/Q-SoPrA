#ifndef RELATIONSHIPSWIDGET_H
#define RELATIONSHIPSWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QDropEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QWheelEvent>
#include "AttributeDialog.h"
#include "DeselectableTreeView.h"
#include "RelationshipTypeDialog.h"
#include "RelationshipTreeFilter.h"
#include "RelationshipsDialog.h"
#include "AttributeIndexDialog.h"

class RelationshipsWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  RelationshipsWidget(QWidget *parent=0);
  ~RelationshipsWidget() {};

private slots:
  void retrieveData();
  void setCommentBool();
  void setComment();
  void previousIncident();
  void nextIncident();
  void jumpIncident();
  void toggleMark();
  void previousMarked();
  void nextMarked();
  void setDescriptionFilter(const QString &text);
  void previousDescription();
  void nextDescription();
  void setRawFilter(const QString &text);
  void previousRaw();
  void nextRaw();
  void setCommentFilter(const QString &text);
  void previousComment();
  void nextComment();
  void previousCoded();
  void nextCoded();
  void changeFilter(const QString &text);
  void setCommentButton();
  void submitRelationshipComment();
  void getComment();
  void newType();
  void editType();
  void newRelationship();
  void editRelationship();
  void removeUnusedRelationships();
  void assignRelationship();
  void unassignRelationship();
  void resetTexts();
  void setButtons();
  void sourceText(const QString &relationship, const int &incident);
  void highlightText();
  void setTree();
  void boldSelected(QAbstractItemModel *model, QString name = "", QModelIndex parent = QModelIndex());
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void expandTree();
  void collapseTree();
  void finalBusiness();
  bool eventFilter(QObject *object, QEvent *event); 

private:
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QSqlTableModel> typeModel;
  QPointer<QSqlTableModel> relationshipsModel;
  QPointer<QSqlTableModel> assignedModel;
  QPointer<QStandardItemModel> relationshipsTree;
  QPointer<DeselectableTreeView> relationshipsTreeView;
  QPointer<RelationshipTypeDialog> typeDialog;
  QPointer<RelationshipTreeFilter> treeFilter;
  
  QPointer<QLabel> indexLabel;
  QPointer<QLabel> markLabel;
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> relationshipsLabel;
  QPointer<QLabel> descriptionFilterLabel;
  QPointer<QLabel> rawFilterLabel;
  QPointer<QLabel> commentFilterLabel;
  QPointer<QLabel> relationshipFilterLabel;
  QPointer<QLabel> relationshipCommentLabel;

  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> descriptionFilterField;
  QPointer<QLineEdit> rawFilterField;
  QPointer<QLineEdit> commentFilterField;
  QPointer<QLineEdit> relationshipFilterField;
  QPointer<QLineEdit> relationshipCommentField;

  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;

  QPointer<QPushButton> previousIncidentButton;
  QPointer<QPushButton> nextIncidentButton;
  QPointer<QPushButton> jumpButton;
  QPointer<QPushButton> markButton;
  QPointer<QPushButton> previousMarkedButton;
  QPointer<QPushButton> nextMarkedButton;
  QPointer<QPushButton> descriptionPreviousButton;
  QPointer<QPushButton> descriptionNextButton;
  QPointer<QPushButton> rawPreviousButton;
  QPointer<QPushButton> rawNextButton;
  QPointer<QPushButton> commentPreviousButton;
  QPointer<QPushButton> commentNextButton;
  QPointer<QPushButton> previousCodedButton;
  QPointer<QPushButton> nextCodedButton;
  QPointer<QPushButton> submitRelationshipCommentButton;
  QPointer<QPushButton> newTypeButton;
  QPointer<QPushButton> editTypeButton;
  QPointer<QPushButton> newRelationshipButton;
  QPointer<QPushButton> editRelationshipButton;
  QPointer<QPushButton> removeUnusedRelationshipsButton;
  QPointer<QPushButton> assignRelationshipButton;
  QPointer<QPushButton> unassignRelationshipButton;
  QPointer<QPushButton> expandTreeButton;
  QPointer<QPushButton> collapseTreeButton;
  QPointer<QPushButton> resetTextsButton;

  QString descriptionFilter;
  QString rawFilter;
  QString commentFilter;

  bool commentBool;
};

#endif
