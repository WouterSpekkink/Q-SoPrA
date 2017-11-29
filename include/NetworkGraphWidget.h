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

class NetworkGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  NetworkGraphWidget(QWidget *parent = 0);
  ~NetworkGraphWidget() {};

private slots:
  void toggleGraphicsControls();
  void toggleLegend();

  void setFilterButtons(QListWidgetItem *item);
  void activateFilter();
  void deactivateFilter();
  
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
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QVector<NetworkNode*> nodeVector;
  QVector<DirectedEdge*> directedVector;
  QVector<UndirectedEdge*> undirectedVector;
  QVector<NetworkNodeLabel*> labelVector;
  QPointer<QWidget> graphicsWidget;
  QPointer<QWidget> legendWidget;

  QPointer<QLabel> typeLabel;
  QPointer<QLabel> upperRangeLabel;
  QPointer<QLabel> lowerRangeLabel;
  QPointer<QLabel> nodeLegendLabel;
  QPointer<QLabel> edgeLegendLabel;
  
  QPointer<QComboBox> typeComboBox;

  QPointer<QListWidget> nodeListWidget;
  QPointer<QListWidget> edgeListWidget;

  QPointer<QPushButton> plotButton;
  QPointer<QPushButton> addButton;
  QPointer<QPushButton> toggleLabelsButton;
  QPointer<QPushButton> toggleGraphicsControlsButton;
  QPointer<QPushButton> toggleLegendButton;
  QPointer<QPushButton> colorByAttributeButton;
  QPointer<QPushButton> nodeColorButton;
  QPointer<QPushButton> labelColorButton;
  QPointer<QPushButton> backgroundColorButton;
  QPointer<QPushButton> increaseFontSizeButton;
  QPointer<QPushButton> decreaseFontSizeButton;
  QPointer<QPushButton> exportSvgButton;
  QPointer<QPushButton> setFilteredButton;
  QPointer<QPushButton> unsetFilteredButton;
  
  QPointer<QDial> lowerRangeDial;
  QPointer<QDial> upperRangeDial;
  QPointer<QSpinBox> lowerRangeSpinBox;
  QPointer<QSpinBox> upperRangeSpinBox;
  
  QVector<QString> presentTypes; 

  QString selectedType;

  int minOrder;
  int maxOrder;

  bool labelsShown;

};


#endif
