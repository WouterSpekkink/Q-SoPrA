#ifndef EVENTGRAPHWIDGET
#define EVENTGRAPHWIDGET

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QVector>
#include <QPointer>
#include <QtSql>
#include <QMessageBox>
#include <QSvgGenerator>
#include <QFileDialog>
#include <math.h>
#include <QGraphicsTextItem>
#include <QColorDialog>
#include <vector>
#include "NodeLabel.h"
#include "GraphicsView.h"
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
#include "DeselectableListWidget.h"
#include "LineageColorDialog.h"
#include "OccurrenceGraphWidget.h"

class AttributesWidget;

class EventGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;
  
public:
  EventGraphWidget(QWidget *parent = 0);
  ~EventGraphWidget() {};

  QVector<MacroEvent*> getMacros();
  QVector<EventItem*> getEventItems();
  void resetTree();
  void checkCongruency();

private slots:
  void setCommentBool();
  void setComment();
  void toggleDetails();
  void toggleGraphicsControls();
  void toggleLegend();
  void removeMode();
  void setModeButton(QTableWidgetItem *item);
  void disableModeButton();
  void restoreModeColors();
  void retrieveData();
  void showAttributes();
  void showComments();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void boldSelected(QAbstractItemModel *model, QString name = "", int event = -1, 
		    QString type = "", QModelIndex parent = QModelIndex());
  void sourceText(const QString &attribute, const int &incident);
  void removeText();
  void resetTexts();
  void setButtons();
  void fixTree();
  void changeFilter(const QString &text);
  void assignAttribute();
  void unassignAttribute();
  void highlightText();
  void newAttribute();
  void editAttribute();
  void removeUnusedAttributes();  
  void seeIncidents();
  void previousDataItem();
  void nextDataItem();
  void getEvents();
  void plotEvents();
  void getEdges(QString coder, QString type);
  void plotEdges();
  void getLabels();
  void addLabels();
  void cleanUp();	     
  void increaseWidth(EventItem *item);
  void decreaseWidth(EventItem *item);
  void increasePos(EventItem *item);
  void decreasePos(EventItem *item);
  void increaseWidth(MacroEvent *item);
  void decreaseWidth(MacroEvent *item);
  void increasePos(MacroEvent *item);
  void decreasePos(MacroEvent *item);
  void increaseDistance();
  void decreaseDistance();
  void processMoveItems(QGraphicsItem *item, QPointF pos);
  void setPlotButton();
  void getLinkageDetails();
  void plotGraph();
  void setCompareButton();
  void compare();
  void getCompareEdges(QString coder, QString type);
  void plotCompareEdges();
  void saveCurrentPlot();
  void seePlots();
  void setChangeLabel();
  void plotLabels();
  void processLowerRange(int value);
  void processUpperRange(int value);
  void setVisibility();
  void setRangeControls();
  void exportSvg();
  void colorByAttribute();
  void findChildren(QString father, QVector<QString> *children);
  void setEventColor();
  void setLabelColor();
  void setBackgroundColor();
  void changeModeColor(QTableWidgetItem *item);
  void processEventItemContextMenu(const QString &action);
  void colligateEvents(QString constraint);
  void disaggregateEvent();
  bool checkConstraints(QVector<EventItem*> incidents, QString constraint);
  void rewireLinkages(MacroEvent *macro, QVector<EventItem*> incidents);
  void recolorEvents();
  void recolorLabels();
  void colorLineage();

  void findAncestors(QColor ancestor, QGraphicsItem *origin);
  void findDescendants(QColor descendant, QGraphicsItem *origin);
  
  void settleEvent();
  void makeParallel();
  void normalizeDistance();
  void closeGap();
  void processArrowContextMenu(const QString &action);
  void removeLinkage();
  void keepLinkage();
  void acceptLinkage();
  void rejectLinkage();
  void ignoreLinkage();
  void removeNormalLinkage();
  void findPastPaths(QSet<int> *mark, int currentIncident);
  void findUndirectedPaths(QSet<int> *mark, QSet<int> *submittedItems);
  void findFuturePaths(QSet<int> *mark, int currentIncident);
  bool eventFilter(QObject *object, QEvent *event);
  void setAttributesWidget(AttributesWidget* aw);
  void setOccurrenceGraph(OccurrenceGraphWidget* ogw);
  void finalBusiness();
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> attWidget;
  QPointer<QWidget> commentWidget;
  QPointer<QWidget> legendWidget;
  QVector<EventItem*> eventVector;
  QVector<MacroEvent*> macroVector;
  QVector<QGraphicsItem*> currentData;
  QVector<Arrow*> edgeVector;
  QVector<Arrow*> compareVector;
  QVector<NodeLabel*> nodeLabelVector;
  QVector<MacroLabel*> macroLabelVector;
  AttributesWidget *attributesWidget;
  OccurrenceGraphWidget *occurrenceGraph;
  
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  
  QPointer<QLabel> coderLabel;
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> plotLabel;
  QPointer<QLabel> changeLabel;
  QPointer<QLabel> compareLabel;
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;
  QPointer<QLabel> indexLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QLabel> incongruencyLabel;
  QPointer<QLabel> legendLabel;
  
  QPointer<QPushButton> plotButton;
  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> compareButton;
  QPointer<QPushButton> increaseDistanceButton;
  QPointer<QPushButton> decreaseDistanceButton;
  QPointer<QPushButton> toggleDetailsButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> previousEventButton;
  QPointer<QPushButton> nextEventButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> plotLabelsButton;
  QPointer<QPushButton> colorByAttributeButton;
  QPointer<QPushButton> eventColorButton;
  QPointer<QPushButton> labelColorButton;
  QPointer<QPushButton> backgroundColorButton;
  QPointer<QPushButton> seeIncidentsButton;
  QPointer<QPushButton> seeAttributesButton;
  QPointer<QPushButton> seeCommentsButton;
  QPointer<QPushButton> valueButton; 
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;  
  QPointer<QPushButton> removeTextButton;
  QPointer<QPushButton> resetTextsButton;
  QPointer<QPushButton> removeModeButton;
  QPointer<QPushButton> restoreModeColorsButton;
  
  QPointer<DeselectableListWidget> eventListWidget;
  
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QLineEdit> valueField;

  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;

  QPointer<QComboBox> coderComboBox;
  QPointer<QComboBox> typeComboBox;
  QPointer<QComboBox> compareComboBox;

  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;
  
  QString selectedCoder;
  QString selectedCompare;
  QString selectedType;
  MacroEvent *selectedMacro;
  
  int distance;
  int vectorPos;
  int selectedIncident;

  bool labelsVisible;
  bool commentBool;
};

#endif
