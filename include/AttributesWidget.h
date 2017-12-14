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
#include <QHeaderView>
#include <QMessageBox>
#include "AttributeDialog.h"
#include "DeselectableTreeView.h"
#include "AttributeIndexDialog.h"
#include "AttributeTreeFilter.h"
#include "EventGraphWidget.h"
#include "MainWindow.h"
#include "MacroEvent.h"
#include "EventGraphWidget.h"

class EventGraphWidget;

class AttributesWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  AttributesWidget(QWidget *parent=0);
  ~AttributesWidget() {};
  void resetTree();

private slots:
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
  void changeFilter(const QString &text);
  void retrieveData();
  void newAttribute();
  void editAttribute();
  void sourceText(const QString &attribute, const int &incident);
  void highlightText();
  void assignAttribute();
  void unassignAttribute();
  void removeText();
  void resetTexts();
  void removeUnusedAttributes();
  void setTree();
  void expandTree();
  void collapseTree();
  void findChildren(QString father, QVector<QString> *children);
  void previousCoded();
  void nextCoded();
  void setButtons();
  void buildHierarchy(QStandardItem *top, QString name);
  void boldSelected(QAbstractItemModel *model, QString name = "", QModelIndex parent = QModelIndex());
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void finalBusiness();
  void setValueButton();
  void setValue();
  void getValue();
  void fixTree();
  bool eventFilter(QObject *object, QEvent *event);
  void setEventGraph(EventGraphWidget *egw);
  
private:
  QPointer<AttributeDialog> attributeDialog;
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QSqlTableModel> attributesModel;
  QPointer<QSqlTableModel> assignedModel;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  EventGraphWidget *eventGraph;
  
  QPointer<QLabel> indexLabel;
  QPointer<QLabel> markLabel;
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> descriptionFilterLabel;
  QPointer<QLabel> rawFilterLabel;
  QPointer<QLabel> commentFilterLabel;
  QPointer<QLabel> attributeFilterLabel;
  QPointer<QLabel> valueLabel;

  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> descriptionFilterField;
  QPointer<QLineEdit> rawFilterField;
  QPointer<QLineEdit> commentFilterField;
  QPointer<QLineEdit> attributeFilterField;
  QPointer<QLineEdit> valueField;

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
  QPointer<QPushButton> newAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;
  QPointer<QPushButton> valueButton;
  QPointer<QPushButton> expandTreeButton;
  QPointer<QPushButton> collapseTreeButton;
  QPointer<QPushButton> previousCodedButton;
  QPointer<QPushButton> nextCodedButton;
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> resetTextsButton;
  
  QString descriptionFilter;
  QString rawFilter;
  QString commentFilter;

  bool commentBool;
};


#endif
