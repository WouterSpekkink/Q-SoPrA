#ifndef MERGERELATIONSHIPSDIALOG_H
#define MERGERELATIONSHIPSDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"
#include "DirectedEdge.h"
#include "UndirectedEdge.h"
#include "SupportingFunctions.h"

class MergeRelationshipsDialog : public QDialog {
  Q_OBJECT

public:
  MergeRelationshipsDialog(QWidget *parent = 0,
			   QVector<DirectedEdge*> *directed = new QVector<DirectedEdge*>,
			   QVector<UndirectedEdge*> *undirected = new QVector<UndirectedEdge*>,
			   QVector<QString> submittedRelationships = QVector<QString>(),
			   QVector<QString> submittedDirections = QVector<QString>());
  ~MergeRelationshipsDialog() {};

  void setRelationships(QVector<QString> submittedRelationships);
  QVector<QString> getTypes();
  QString getName();
  QString getDescription();
  QString getDirectedness();
  int getExitStatus();
  
private slots:
  void cancelAndClose();
  void saveAndClose();

private:
  QVector<DirectedEdge*> *pDirected;
  QVector<UndirectedEdge*> *pUndirected;
  
  QPointer<QLabel> relationshipsLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;

  QVector<QPointer<QCheckBox>> relationships;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QPointer<QLineEdit> nameField;

  QPointer<QTextEdit> descriptionField;

  QVector<QString> types;
  QVector<QString> directions;
  QString name;
  QString description;
  QString directedness;
  
  int exitStatus;
};

#endif
