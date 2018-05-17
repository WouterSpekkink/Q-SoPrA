#ifndef RELATIONSHIPCOLORDIALOG_H
#define RELATIONSHIPCOLORDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"
#include "DeselectableTreeView.h"
#include "SupportingFunctions.h"
#include "RelationshipTreeFilter.h"

class RelationshipColorDialog : public QDialog {
  Q_OBJECT

public:
  RelationshipColorDialog(QWidget *parent = 0);
  ~RelationshipColorDialog() {};

  QColor getColor();
  QColor getTextColor();
  QString getRelationship();
  QString getType();
  int getExitStatus();
			    
private slots:
  void setTree();
  void changeFilter(const QString &text);
  void setColor();
  void setTextColor();
  void setRelationship();
  void cancelAndClose();
  void saveAndClose();
    
private:
  QPointer<QLabel> relationshipLabel;

  QPointer<QStandardItemModel> relationshipsTree;
  QPointer<DeselectableTreeView> relationshipsTreeView;
  QPointer<RelationshipTreeFilter> treeFilter;
  QPointer<QLabel> relationshipsFilterLabel;
  QPointer<QLabel> colorOneLabel;
  QPointer<QLabel> colorTwoLabel;
  
  QPointer<QLineEdit> relationshipsFilterField;
  
  QPointer<QPushButton> colorButton;
  QPointer<QPushButton> textColorButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QColor chosenColor;
  QColor chosenTextColor;
  QString chosenRelationship;
  QString relationshipType;

  QString type;

  int exitStatus;
};

#endif
