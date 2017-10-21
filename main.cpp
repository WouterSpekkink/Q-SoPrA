#include <QtWidgets/QApplication>
#include "./include/WelcomeDialog.h"
#include "./include/MainDialog.h"
#include "./include/EventSequenceDatabase.h"

int main(int argc, char *argv[]) {
  QApplication ED(argc, argv);
  EventSequenceDatabase *esd = new EventSequenceDatabase;
  
  WelcomeDialog *welcome = new WelcomeDialog(esd);
  welcome->exec();
  if (welcome->getExitStatus() != 1) {
    MainDialog *dialog = new MainDialog(esd);
    dialog->exec();
  } else {
    return ED.exec();
  }
  return ED.exec();
}
