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

class HierarchyGraphWidget : public QDialog {
  Q_OBJECT

public:
  HierarchyGraphWidget(QWidget *parent = 0);
  ~HierarchyGraphWidget() {};
  void setOrigin(MacroEvent *submittedOrigin);
  void setEvents(QVector<EventItem*> submittedEvents);
  void setMacros(QVector<MacroEvent*> submittedMacros);

private slots:
  void toggleDetails();
  void showAttributes();
  void showComments();
  void buildComponents(MacroEvent *submittedOrigin, int layer);
  void cleanUp();
  
private:
  QPointer<Scene> scene;
  QPointer<GraphicsView> view;
  QPointer<QStandardItemModel> attributesTree;
  QPointer<DeselectableTreeView> attributesTreeView;
  QPointer<AttributeTreeFilter> treeFilter;

  QPointer<QWidget> infoWidget;
  QPointer<QWidget> attWidget;
  QPointer<QWidget> commentWidget;
  
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;
  QPointer<QLabel> attributesLabel;
  QPointer<QLabel> attributesFilterLabel;
  QPointer<QLabel> valueLabel;
  
  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;
  QPointer<QLineEdit> attributesFilterField;
  QPointer<QLineEdit> valueField;

  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;
  
  QPointer<QPushButton> toggleDetailsButton;
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

  MacroEvent *origin;
  QVector<EventItem*> eventVector;
  QVector<MacroEvent*> macroVector;
};

#endif
