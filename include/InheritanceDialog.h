#ifndef INHERITANCEDIALOG_H
#define INHERITANCEDIALOG_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QPointer>
#include <QSet>
#include <QList>

class InheritanceDialog : public QDialog
{
  Q_OBJECT

public:
  InheritanceDialog(QWidget *parent = 0,
		    QSet<QString> attributes = QSet<QString>());
  ~InheritanceDialog();

  int getExitStatus();
  QVector<QString> getSelected();
		     
private slots:
  void selectAll();
  void deselectAll();

  void saveAndClose();
  void cancelAndClose();
  
private:
  QVector<QCheckBox*> checkBoxVector;

  QPointer<QLabel> titleLabel;

  QPointer<QPushButton> selectAllButton;
  QPointer<QPushButton> deselectAllButton;
  QPointer<QPushButton> saveCloseButton;
  QPointer<QPushButton> cancelCloseButton;

  QVector<QString> selectedAttributes;

  int exitStatus;
};

#endif
