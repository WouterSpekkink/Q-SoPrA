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
#include "DeselectableTreeView.h"
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

class NetworkGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  NetworkGraphWidget(QWidget *parent = 0);
  ~NetworkGraphWidget() {};

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
  void mergeRelationships();
  void setFilterButtons(QTableWidgetItem *item);
  void disableFilterButtons();
  void activateFilter();
  void deactivateFilter();
  void hideType();
  void showType();
  void setVisibility();
  
  void getEntities();
  void plotEntities();
  void getDirectedEdges();
  void plotDirectedEdges(QString type, QColor color);
  void getUndirectedEdges();
  void plotUndirectedEdges(QString type, QColor color);
  void getTypes();

  void colorByAttribute();
  void setNodeColor();
  void setLabelColor();
  void setBackgroundColor();
  void toggleLabels();
  void increaseLabelSize();
  void decreaseLabelSize();

  void findChildren(QString father, QVector<QString> *children);

  void processLowerRange(int value);
  void processUpperRange(int value);
  void setRangeControls();

  void exportSvg();

  void simpleLayout();
  void setPlotButton();
  void plotNewGraph();
  void addRelationshipType();
  void cleanUp();
  bool eventFilter(QObject *object, QEvent *event);
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QVector<NetworkNode*> nodeVector;
  QVector<NetworkNode*> currentData;
  QVector<DirectedEdge*> directedVector;
  QVector<UndirectedEdge*> undirectedVector;
  QVector<NetworkNodeLabel*> labelVector;
  QPointer<QWidget> infoWidget;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> legendWidget;

  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
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
  QPointer<QPushButton> setFilteredButton;
  QPointer<QPushButton> unsetFilteredButton;
  QPointer<QPushButton> hideTypeButton;
  QPointer<QPushButton> showTypeButton;
  QPointer<QPushButton> multimodeButton;
  QPointer<QPushButton> mergeButton;
  QPointer<QPushButton> simpleLayoutButton;
  
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
};


#endif
