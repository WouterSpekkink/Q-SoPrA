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
#include "Scene.h"
#include "Constants.h"
#include "SimpleTextDialog.h"
#include "ProgressBar.h"
#include "SavedPlotsDialog.h"

class NetworkGraphWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  NetworkGraphWidget(QWidget *parent = 0);
  ~NetworkGraphWidget() {};

private slots:
  void getEntities();
  void plotEntities();
  void getDirectedEdges();
  void plotDirectedEdges();

  void simpleLayout();
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QVector<NetworkNode*> nodeVector;
  QVector<DirectedEdge*> directedVector;
};


#endif
