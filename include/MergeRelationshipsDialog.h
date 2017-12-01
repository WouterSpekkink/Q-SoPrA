#ifndef MERGERELATIONSHIPSDIALOG_H
#define MERGERELATIONSHIPSDIALOG_H

#include <QtWidgets>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QPointer>
#include <QColorDialog>
#include <QtSql>
#include "Constants.h"
#include "DirectedEdge.h"
#include "UndirectedEdge.h"

class MergeRelationshipsDialog : public QDialog {
  Q_OBJECT

public:
  MergeRelationshipsDialog(QWidget *parent = 0,
			   QVector<DirectedEdge*> *directed = new QVector<DirectedEdge*>,
			   QVector<UndirectedEdge*> *undirected = new QVector<UndirectedEdge*>);
  ~MergeRelationshipsDialog() {};

  void setRelationships(QVector<QString> submittedRelationships);
  QString getRelOne();
  QString getRelTwo();
  QString getName();
  QString getDescription();
  QString getDirectedness();
  int getExitStatus();
  
private slots:
  void setRelOne(const QString &name);
  void setRelTwo(const QString &name);

  void cancelAndClose();
  void saveAndClose();

private:
  QVector<DirectedEdge*> *pDirected;
  QVector<UndirectedEdge*> *pUndirected;
  
  QPointer<QLabel> relOneLabel;
  QPointer<QLabel> relTwoLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;

  QPointer<QComboBox> relOneComboBox;
  QPointer<QComboBox> relTwoComboBox;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QPointer<QLineEdit> nameField;

  QPointer<QTextEdit> descriptionField;

  QString name;
  QString description;
  QString relOne;
  QString relTwo;
  QString directedness;
  
  int exitStatus;
};

#endif
