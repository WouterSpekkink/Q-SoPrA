#ifndef EVENTNODESETTINGSDIALOG_H
#define EVENTNODESETTINGSDIALOG_H

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QTableWidget>
#include <QFileDialog>
#include <QTableView>
#include <QPointer>
#include <fstream>
#include "ZoomableTableView.h"
#include "SortFunctions.h"
#include "ProgressBar.h"
#include "AttributeSelectionDialog.h"
#include "ComboBoxDialog.h"
#include "MacroEvent.h"
#include "MacroLabel.h"
#include "Constants.h"

class EventNodeSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  EventNodeSettingsDialog(QWidget *parent = 0,
			  QVector<QString> submittedIds = QVector<QString>(),
			  QVector<QString> submittedLabels = QVector<QString>(),
			  QVector<QString> submittedTimings = QVector<QString>(),
			  QVector<QString> submittedDescriptions = QVector<QString>(),
			  QVector<QString> submittedComments = QVector<QString>(),
			  QVector<QString> submittedTypes = QVector<QString>(),
			  QVector<QString> submittedModes = QVector<QString>(),
			  QVector<QString> submittedX = QVector<QString>(),
			  QVector<QString> submittedY = QVector<QString>(),
			  QVector<MacroEvent*> submittedMacros = QVector<MacroEvent*>());
  ~EventNodeSettingsDialog() {};
  
private slots:
  void cancelAndClose();
  void exportAndClose();
  void addAttribute();
  void removeAttribute();
  void findChildren(QString father, QVector<QString> *children);
  
private:
  QPointer<QTableWidget> tableWidget;
  
  QPointer<QPushButton> addAttributeButton;
  QPointer<QPushButton> removeAttributeButton;
  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> exportCloseButton;

  QVector<QString> ids;
  QVector<QString> labels;
  QVector<QString> timings;
  QVector<QString> descriptions;
  QVector<QString> comments;
  QVector<QString> types;
  QVector<QString> modes;
  QVector<QString> xCoords;
  QVector<QString> yCoords;
  QVector<MacroEvent*> macros;
};

#endif
