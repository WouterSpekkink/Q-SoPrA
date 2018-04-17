#include <QtWidgets/QApplication>
#include "./include/WelcomeDialog.h"
#include "./include/MainWindow.h"
#include "./include/EventSequenceDatabase.h"

int main(int argc, char *argv[]) {
  QApplication QSoPrA(argc, argv);

  QString version = QString("0.8.7.1");
  QSoPrA.setApplicationVersion(version);

  QSharedPointer<EventSequenceDatabase> esd(new EventSequenceDatabase);
  
  WelcomeDialog *welcome = new WelcomeDialog(0, esd.data());
  welcome->exec();
  if (welcome->getExitStatus() == 0) {
    MainWindow *window = new MainWindow(0, esd.data());
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    delete welcome;
  } else if (welcome->getExitStatus() == 1) {
    return 0;
    delete welcome;
  }
  return QSoPrA.exec();
}
