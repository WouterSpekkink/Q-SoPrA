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
#include "LargeTextDialog.h"
#include "LineObject.h"
#include "EllipseObject.h"
#include "RectObject.h"
#include "TextObject.h"

class AttributesWidget;

class HierarchyGraphWidget : public QDialog {
  Q_OBJECT
  friend class MainWindow;

public:
  HierarchyGraphWidget(QWidget *parent = 0);
  ~HierarchyGraphWidget() {};
  void setOrigin(MacroEvent *submittedOrigin);
  void setEvents(QVector<EventItem*> submittedEvents);
  void setMacros(QVector<MacroEvent*> submittedMacros);
  void setEdges(QVector<Arrow*> submittedEvents);
  void getEdges();
  void cleanUp();
  void setAttributesWidget(AttributesWidget *aw);
  void setEventGraph(EventGraphWidget *egw);
  void resetTree();
					   
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
  void selectText();
  void sourceAttributeText(const QString &attribute, const int &incident);
  void highlightText();
  void removeText();
  void resetTexts();
  void toggleLinkages();
  void toggleHierarchy();

  void processHierarchyGraphContextMenu(const QString &action, const QPoint &pos);
  void addLineObject(bool arrow1, bool arrow2, const QPointF &pos);
  void addTextObject(const QPointF &pos);
  void addEllipseObject(const QPointF &pos);
  void addRectObject(const QPointF &pos);
  void processLineContextMenu(const QString &action);
  void changeLineColor();
  void toggleArrow1();
  void toggleArrow2();
  void deleteLine();
  void processTextContextMenu(const QString &action);
  void changeText();
  void changeTextColor();
  void deleteText();
  void processEllipseContextMenu(const QString &action);
  void changeEllipseColor();
  void deleteEllipse();
  void processRectContextMenu(const QString &action);
  void changeRectColor();
  void deleteRect();
  
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void buildEntities(QStandardItem *top, QString name);
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

  QVector<LineObject*> lineVector;
  QVector<TextObject*> textVector;
  QVector<EllipseObject*> ellipseVector;
  QVector<RectObject*> rectVector;
  
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
  QPointer<QPushButton> toggleLinkagesButton;
  QPointer<QPushButton> toggleHierarchyButton;

  QPointer<DeselectableListWidget> eventListWidget;
  
  MacroEvent *origin;
  QVector<EventItem*> eventVector;
  QVector<MacroEvent*> macroVector;
  QVector<Arrow*> edgeVector;
  
  MacroEvent *selectedMacro;
  int selectedIncident;
  bool commentBool;
  bool showLinkages;
  bool showHierarchy;
};

#endif
