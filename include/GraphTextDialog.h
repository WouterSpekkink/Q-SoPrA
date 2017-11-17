#ifndef GRAPHTEXTDIALOG_H
#define GRAPHTEXTDIALOG_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QMessageBox>
#include <QPointer>
#include <QtSql>

class GraphTextDialog : public QDialog {
  Q_OBJECT

public:
  GraphTextDialog(QWidget *parent = 0);
  ~GraphTextDialog() {};

  QString getText();
  void submitText(const QString &submittedText);
  int getExitStatus();

private slots:
  void cancelAndClose();
  void saveAndClose();

private:
  QPointer<QLabel> textLabel;
  
  QPointer<QLineEdit> textField;

  QPointer<QPushButton> cancelCloseButton;
  QPointer<QPushButton> saveCloseButton;

  QString text;
  QString subText;
  int exitStatus;
};

#endif
