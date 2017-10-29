#ifndef ATTRIBUTESWIDGET_H
#define ATTRIBUTESWIDGET_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QSqlTableModel>
#include "EventSequenceDatabase.h"
#include "AttributeDialog.h"

class AttributesWidget : public QWidget {
  Q_OBJECT
  friend class MainWindow;

public:
  AttributesWidget(QWidget *parent=0, EventSequenceDatabase *submittedEsd = new EventSequenceDatabase);
  ~AttributesWidget() {};

private slots:
  void previousIncident();
  void nextIncident();
  void retrieveData();
  void newAttribute();
  
private:
  QPointer<EventSequenceDatabase> esd;
  QPointer<AttributeDialog> attributeDialog;
  QPointer<QSqlTableModel> incidentsModel;
  QPointer<QSqlTableModel> attributesModel;
  
  QPointer<QLabel> timeStampLabel;
  QPointer<QLabel> sourceLabel;
  QPointer<QLabel> descriptionLabel;
  QPointer<QLabel> rawLabel;
  QPointer<QLabel> commentLabel;

  QPointer<QLineEdit> timeStampField;
  QPointer<QLineEdit> sourceField;

  QPointer<QTextEdit> descriptionField;
  QPointer<QTextEdit> rawField;
  QPointer<QTextEdit> commentField;

  QPointer<QPushButton> previousIncidentButton;
  QPointer<QPushButton> nextIncidentButton;
  QPointer<QPushButton> newAttributeButton;
  
};


#endif
