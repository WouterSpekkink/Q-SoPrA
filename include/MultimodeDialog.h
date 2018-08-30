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
#include "SupportingFunctions.h"
#include "SortFunctions.h"

class MultimodeDialog : public QDialog
{
  Q_OBJECT

public:
  MultimodeDialog(QWidget *parent = 0, 
		  QVector<DirectedEdge*> *directed = new QVector<DirectedEdge*>,
		  QVector<UndirectedEdge*> *undirected = new QVector<UndirectedEdge*>);
  ~MultimodeDialog() {};

  void setModes(QVector<QString> submittedModes);
  QString getModeOne();
  QString getModeTwo();
  QString getRelationshipOne();
  QString getRelationshipTwo();
  QString getName();
  QString getDescription();
  QString getDirectedness();
  int getExitStatus();
		     
private slots:
  void setModeOne(const QString &name);
  void setModeTwo(const QString &name);
  void setRelationshipOne(const QString &name);
  void setRelationshipTwo(const QString &name);

  void checkDirectedButton();
  void checkUndirectedButton();
  
  void cancelAndClose();
  void saveAndClose();

private:
  QVector<DirectedEdge*> *pDirected;
  QVector<UndirectedEdge*> *pUndirected;

  QPointer<QLabel> modeOneLabel; // Don't forget to set tool tips.
  QPointer<QLabel> modeTwoLabel;
  QPointer<QLabel> relationshipOneLabel;
  QPointer<QLabel> relationshipTwoLabel;
  QPointer<QLabel> nameLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> directednessLabel;
  
  QPointer<QComboBox> modeOneComboBox;
  QPointer<QComboBox> modeTwoComboBox;
  QPointer<QComboBox> relationshipOneComboBox;
  QPointer<QComboBox> relationshipTwoComboBox;

  QPointer<QPushButton> directedButton;
  QPointer<QPushButton> undirectedButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QPointer<QLineEdit> nameField;

  QPointer<QTextEdit> descriptionField;

  QVector<QString> modes;

  QString name;
  QString description;
  QString modeOne;
  QString modeTwo;
  QString relationshipOne;
  QString relationshipTwo;
  QString directedness;
  
  int exitStatus;
};

#endif
