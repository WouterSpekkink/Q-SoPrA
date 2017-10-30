#include <QtWidgets/QApplication>
#include "./include/WelcomeDialog.h"
#include "./include/MainWindow.h"
#include "./include/EventSequenceDatabase.h"

int main(int argc, char *argv[]) {
  QApplication ED(argc, argv);
  EventSequenceDatabase *esd = new EventSequenceDatabase;
  
  WelcomeDialog *welcome = new WelcomeDialog(0, esd);
  welcome->exec();
  if (welcome->getExitStatus() == 0) {
    MainWindow *window = new MainWindow(0, esd);
    window->show();
  } else if (welcome->getExitStatus() == 1) {
    return 0;
  }
  return ED.exec();
}
