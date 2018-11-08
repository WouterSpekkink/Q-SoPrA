#ifndef ABSTRACTIONDIALOG_H
#define ABSTRACTIONDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include "../include/SortFunctions.h"
#include "../include/EventGraphWidget.h"
#include "../include/InheritanceDialog.h"

class AbstractionDialog : public QDialog
{
  Q_OBJECT

public:
  AbstractionDialog(QWidget *parent = 0,
		    QVector<EventItem*> submittedEvents = QVector<EventItem*>(),
		    QVector<MacroEvent*> submittedMacros = QVector<MacroEvent*>(),
		    QVector<QGraphicsItem*> submittedData = QVector<QGraphicsItem*>(),
		    QVector<QString> submittedTypes = QVector<QString>(),
		    QString submittedCoder = QString());
  ~AbstractionDialog() {};

  QVector<EventItem*> getCollectedIncidents();
  QString getConstraint();
  QString getDescription();
  QString getAttribute();
  bool isEntity();
  bool isInheriting();
  QVector<QString> getInheritance();
  int getExitStatus();
		     
private slots:
  void selectAttribute();
  void clearAttribute();
  void inheritAttributes();
  void prepareEvents();
  void checkConstraints(QVector<EventItem*> submittedIncidents);
  void evaluateConstraints();
  void processPathsCheck();
  void processSemipathsCheck();
  void processNoPathsCheck();
  void findChildren(QString father, QVector<QString> *children, bool entity);
  void findHeadsLowerBound(QSet<int> *mark, int currentIncident, int lowerLimit, QString type);
  void findHeadsUpperBound(QSet<int> *mark, int currentIncident, int upperLimit, QString type);
  void findUndirectedPaths(QSet<int> *mark, QSet<int> *submittedItems,
			   int lowerLimit, int upperLimit, QString type);
  void findTailsUpperBound(QSet<int> *mark, int currentIncident, int upperLimit, QString type);
  void findTailsLowerBound(QSet<int> *mark, int currentIncident, int lowerLimit, QString type);
  QVector<bool> checkLinkagePresence(QVector<int> incidentIds);

  void cancelAndClose();
  void saveAndClose();
  
private:
  QVector<EventItem*> eventVector;
  QVector<MacroEvent*> macroVector;
  QVector<QGraphicsItem*> currentData;
  QVector<QString> presentTypes;
  QVector<EventItem*> collectedIncidents;
  QVector<QString> inheritedAttributes;

  QPointer<QLabel> constraintsLabel;
  QPointer<QLabel> attributeOptionsLabel;
  QPointer<QLabel> attributeLabel;
  QPointer<QLabel> attributeInheritanceLabel;
  QPointer<QLabel> chosenAttributeLabel;
  QPointer<QLabel> descriptionLabel;

  QPointer<QCheckBox> pathsBasedCheckBox;
  QPointer<QCheckBox> semiPathsBasedCheckBox;
  QPointer<QCheckBox> noConstraintsCheckBox;

  QPointer<QTextEdit> eventDescriptionField;
  
  QPointer<QPushButton> setAttributeButton;
  QPointer<QPushButton> clearAttributeButton;
  QPointer<QPushButton> inheritAttributesButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;
  
  QString selectedAttribute;
  QString selectedCoder;
  QString chosenConstraint;
  QString eventDescription;
  bool attributeIsEntity;
  int exitStatus;
  bool semiPathsAllowed;
  bool pathsAllowed;
  bool inheritance;
  
};

#endif
