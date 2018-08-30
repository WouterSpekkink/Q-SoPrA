#ifndef NODESETTINGSDIALOG_H
#define NODESETTINGSDIALOG_H

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

class NodeSettingsDialog : public QDialog
{
  Q_OBJECT

public:
  NodeSettingsDialog(QWidget *parent = 0,
		     QVector<QString> submittedEntities = QVector<QString>(),
		     QVector<QString> submittedDescriptions = QVector<QString>(),
		     QVector<QString> submittedModes = QVector<QString>());
  ~NodeSettingsDialog() {};
  
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
  
  QVector<QString> entities;
  QVector<QString> descriptions;
  QVector<QString> modes;
  
};


#endif
