#ifndef NETWORKGRAPHWIDGET
#define NETWORKGRAPHWIDGET

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
#include <QHeaderView>
#include <QStandardItemModel>
#include <fstream>
#include "DeselectableTreeViewEntities.h"
#include "NodeLabel.h"
#include "GraphicsView.h"
#include "NetworkNode.h"
#include "DirectedEdge.h"
#include "UndirectedEdge.h"
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "SavedPlotsDialog.h"
#include "NetworkNodeLabel.h"
#include "AttributeColorDialog.h"
#include "MultimodeDialog.h"
#include "DeselectableListWidget.h"
#include "AttributeTreeFilter.h"
#include "AttributeDialog.h"
#include "MergeRelationshipsDialog.h"
#include "SortFunctions.h"
#include "SupportingFunctions.h"
#include "NodeSettingsDialog.h"
#include "LargeTextDialog.h"
#include "LineObject.h"
#include "EllipseObject.h"
#include "RectObject.h"
#include "TextObject.h"

class NetworkGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  NetworkGraphWidget(QWidget *parent = 0);
  ~NetworkGraphWidget() {};

  void resetTree();
  void checkCongruency();
  void getTypes();
  bool typesPresent();
		 
private slots:
  void toggleDetails();
  void toggleGraphicsControls();
  void toggleLegend();

  void retrieveData();
  void setTree();
  void buildHierarchy(QStandardItem *top, QString name);
  void resetFont(QAbstractItemModel *model, QModelIndex parent = QModelIndex());
  void boldSelected(QAbstractItemModel *model, QString name = "",
		    QString entity = "", QModelIndex parent = QModelIndex());
  void fixTree(QString entity);
  void setFilter(const QString &text);
  void previousDataItem();
  void nextDataItem();

  void setValueButton();
  void setValue();
  void getValue();

  void assignAttribute();
  void unassignAttribute();
  void addAttribute();
  void editAttribute();
  void removeUnusedAttributes();
  
  void multimodeTransformation();
  void removeMode();
  void setModeButtons(QTableWidgetItem *item);
  void disableModeButtons();
  void restoreModeColors();
  void moveModeUp();
  void moveModeDown();
  
  void mergeRelationships();
  void setFilterButtons(QTableWidgetItem *item);
  void disableFilterButtons();
  void activateFilter();
  void deactivateFilter();
  void hideType();
  void showType();
  void setVisibility();
  void processHeights();
  
  void getEntities();
  void plotEntities();
  void getDirectedEdges();
  void plotDirectedEdges(QString type, QColor color);
  void getUndirectedEdges();
  void plotUndirectedEdges(QString type, QColor color);
  
  void colorByAttribute();
  void setNodeColor();
  void setLabelColor();
  void setBackgroundColor();
  void changeEdgeColor(QTableWidgetItem *item);
  void changeModeColor(QTableWidgetItem *item);
  void toggleLabels();
  void increaseLabelSize();
  void decreaseLabelSize();
  
  void findChildren(QString father, QVector<QString> *children);

  void processLowerRange(int value);
  void processUpperRange(int value);
  void setRangeControls();

  void exportSvg();
  void exportNodes();
  void exportEdges();
  void saveCurrentPlot();
  void seePlots();
  void setChangeLabel();
  void updateEdges();
  
  void simpleLayout();
  void circularLayout();
  void expandLayout();
  void contractLayout();
  void processMoveItems(QGraphicsItem *item, QPointF pos);

  void processNetworkNodeContextMenu(const QString &action);
  void hideCurrentNode();
  void setNodePersistence(bool status);

  void processNetworkGraphContextMenu(const QString &action, const QPoint &pos);
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
  void changeEllipseFillColor();
  void deleteEllipse();
  void processRectContextMenu(const QString &action);
  void changeRectColor();
  void changeRectFillColor();
  void deleteRect();
  void duplicateLine();
  void duplicateText();
  void duplicateEllipse();
  void duplicateRect();

  void setPlotButton();
  void plotNewGraph();
  void addRelationshipType();
  void removeRelationshipType();
  void cleanUp();
  void setButtons();
  bool eventFilter(QObject *object, QEvent *event);
  void finalBusiness();
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QVector<NetworkNode*> nodeVector;
  QVector<NetworkNode*> currentData;
  QVector<DirectedEdge*> directedVector;
  QVector<UndirectedEdge*> undirectedVector;
  QVector<NetworkNodeLabel*> labelVector;
  QVector<LineObject*> lineVector;
  QVector<TextObject*> textVector;
  QVector<EllipseObject*> ellipseVector;
  QVector<RectObject*> rectVector;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> legendWidget;

  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeViewEntities> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;
  
  QPointer<QLabel> typeLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;
  QPointer<QLabel> nodeLegendLabel;
  QPointer<QLabel> edgeLegendLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  QPointer<QLabel> plotLabel;
  QPointer<QLabel> changeLabel;
  QPointer<QLabel> incongruencyLabel;
  
  QPointer<QComboBox> typeComboBox;

  QPointer<DeselectableListWidget> nodeListWidget;
  QPointer<DeselectableListWidget> edgeListWidget;

  QPointer<QLineEdit> nameField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QLineEdit> valueField;
  
  QPointer<QTextEdit> descriptionField;
  
  QPointer<QPushButton> plotButton;
  QPointer<QPushButton> addButton;
  QPointer<QPushButton> toggleLabelsButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> toggleDetailsButton;
  QPointer<QPushButton> previousNodeButton;
  QPointer<QPushButton> nextNodeButton;
  QPointer<QPushButton> valueButton; 
  QPointer<QPushButton> assignAttributeButton;
  QPointer<QPushButton> unassignAttributeButton;
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> editAttributeButton;
  QPointer<QPushButton> removeUnusedAttributesButton;  
  QPointer<QPushButton> colorByAttributeButton;
  QPointer<QPushButton> nodeColorButton;
  QPointer<QPushButton> labelColorButton;
  QPointer<QPushButton> backgroundColorButton;
  QPointer<QPushButton> increaseFontSizeButton;
  QPointer<QPushButton> decreaseFontSizeButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> exportNodesButton;
  QPointer<QPushButton> exportEdgesButton;
  QPointer<QPushButton> setFilteredButton;
  QPointer<QPushButton> unsetFilteredButton;
  QPointer<QPushButton> hideTypeButton;
  QPointer<QPushButton> showTypeButton;
  QPointer<QPushButton> multimodeButton;
  QPointer<QPushButton> removeModeButton;
  QPointer<QPushButton> mergeButton;
  QPointer<QPushButton> removeTypeButton;
  QPointer<QPushButton> simpleLayoutButton;
  QPointer<QPushButton> circularLayoutButton;
  QPointer<QPushButton> expandLayoutButton;
  QPointer<QPushButton> contractLayoutButton;
  QPointer<QPushButton> savePlotButton;
  QPointer<QPushButton> seePlotsButton;
  QPointer<QPushButton> restoreModeColorsButton;
  QPointer<QPushButton> moveModeUpButton;
  QPointer<QPushButton> moveModeDownButton;
  
  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;

  QVector<QString> presentTypes; 

  QString selectedType;
  QString selectedEntityName;

  int minOrder;
  int maxOrder;
  int vectorPos;
  
  bool labelsShown;
  bool massMove;
};


#endif
