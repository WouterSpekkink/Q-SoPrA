#include <QtWidgets/QApplication>
#include "./include/WelcomeDialog.h"

int main(int argc, char *argv[]) {
  QApplication ED(argc, argv);
  
  WelcomeDialog *welcome = new WelcomeDialog;
  welcome->show();
  return ED.exec();
}
