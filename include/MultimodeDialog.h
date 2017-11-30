#ifndef MULTIMODEDIALOG_H
#define MULTIMODEDIALOG_H

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

class MultimodeDialog : public QDialog {
  Q_OBJECT

public:
  MultimodeDialog(QWidget *parent = 0, 
		  QVector<DirectedEdge*> *directed = new QVector<DirectedEdge*>,
		  QVector<UndirectedEdge*> *undirected = new QVector<UndirectedEdge*>);
  ~MultimodeDialog() {};

  void setModes(QVector<QString> submittedModes);
  QString getModeOne();
  QString getModeTwo();
  QString getRelationship();
  QString getName();
  QString getDescription();
  int getExitStatus();
		     
private slots:
  void setModeOne(const QString &name);
  void setModeTwo(const QString &name);
  void setRelationship(const QString &name);
  
  void cancelAndClose();
  void saveAndClose();

private:
  QVector<DirectedEdge*> *pDirected;
  QVector<UndirectedEdge*> *pUndirected;

  QPointer<QLabel> modeOneLabel; // Don't forget to set tool tips.
  QPointer<QLabel> modeTwoLabel;
  QPointer<QLabel> relationshipLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  
  QPointer<QComboBox> modeOneComboBox;
  QPointer<QComboBox> modeTwoComboBox;
  QPointer<QComboBox> relationshipComboBox;
  
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QPointer<QLineEdit> nameField;

  QPointer<QTextEdit> descriptionField;

  QVector<QString> modes;

  QString name;
  QString description;
  QString modeOne;
  QString modeTwo;
  QString relationship;
  
  int exitStatus;
};

#endif
