#ifndef HIERARCHYGRAPHWIDGET_H
#define HIERARCHYGRAPHWIDGET_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QVector>
#include <QPointer>
#include <QtSql>
#include <QMessageBox>
#include <QSvgGenerator>
#include <QFileDialog>
#include "EventGraphWidget.h"
#include "NodeLabel.h"
#include "BandlessGraphicsView.h"
#include "EventItem.h"
#include "Arrow.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "SavedPlotsDialog.h"
#include "AttributeColorDialog.h"
#include "MacroEvent.h"
#include "MacroLabel.h"
#include "LargeTextDialog.h"
#include "DeselectableTreeView.h"
#include "AttributeTreeFilter.h"
#include "AttributesWidget.h"
#include "AttributeDialog.h"
#include "SortFunctions.h"
#include "SupportingFunctions.h"

class AttributesWidget;

class HierarchyGraphWidget : public QDialog {
  Q_OBJECT

public:
  HierarchyGraphWidget(QWidget *parent = 0);
  ~HierarchyGraphWidget() {};
  void setOrigin(MacroEvent *submittedOrigin);
  void setEvents(QVector<EventItem*> submittedEvents);
  void setMacros(QVector<MacroEvent*> submittedMacros);
  void cleanUp();
  void setAttributesWidget(AttributesWidget *aw);
  void setEventGraph(EventGraphWidget *egw);
					   
private slots:
  void setCommentBool();
  void setComment();
  void toggleDetails();
  void toggleLegend();
  void changeModeColor(QTableWidgetItem *item);
  void retrieveData();
  void showAttributes();
  void showComments();
  void buildComponents(MacroEvent *submittedOrigin, int layer);
  void addLayer(QVector<MacroEvent*> presentLayer, QVector<MacroEvent*> partLayer, int layer);
  void exportSvg();
  void assignAttribute();
  void unassignAttribute();
  void newAttribute();
  void editAttribute();
  void removeUnusedAttributes();
  void changeFilter(const QString &text);
  void sourceText(const QString &attribute, const int &incident);
  void highlightText();
  void removeText();
  void resetTexts();
  
  void setTree();
  void resetTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void fixTree();
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void boldSelected(QAbstractItemModel *model, QString name = "", int event = -1, 
		    QString type = "", QModelIndex parent = QModelIndex());
  void setButtons();
  bool eventFilter(QObject *object, QEvent *event);
  void switchBack();
  void finalBusiness();
  
signals:
  void goToEventGraph();
  
private:
  QPointer<Scene> scene;
  QPointer<BandlessGraphicsView> view;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  QVector<QGraphicsItem*> currentData;
  AttributesWidget *attributesWidget;
  EventGraphWidget *eventGraph;
  
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> attWidget;
  QPointer<QWidget> commentWidget;
  QPointer<QWidget> legendWidget;
  
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QLabel> legendLabel;
  
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QLineEdit> valueField;

  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;
  
  QPointer<QPushButton> toggleDetailsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> seeAttributesButton;
  QPointer<QPushButton> seeCommentsButton;
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> valueButton; 
  QPointer<QPushButton> removeUnusedAttributesButton;  
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> resetTextsButton;
  QPointer<QPushButton> exitButton;
  QPointer<QPushButton> exportSvgButton;

  QPointer<DeselectableListWidget> eventListWidget;
  
  MacroEvent *origin;
  QVector<EventItem*> eventVector;
  QVector<MacroEvent*> macroVector;

  MacroEvent *selectedMacro;
  int selectedIncident;
  bool commentBool;
};

#endif
