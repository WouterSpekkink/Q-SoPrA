#include <QtWidgets/QApplication>
#include "./include/WelcomeDialog.h"
#include "./include/MainWindow.h"
#include "./include/EventSequenceDatabase.h"

int main(int argc, char *argv[]) {
  QApplication QSoPrA(argc, argv);
  QSoPrA.setWindowIcon(QIcon("./images/Q-SoPrA.png"));
  QString version = QString("0.8.9.5");
  QSoPrA.setApplicationVersion(version);

  QSharedPointer<EventSequenceDatabase> esd(new EventSequenceDatabase);
  
  WelcomeDialog *welcome = new WelcomeDialog(0, esd.data());
  welcome->exec();
  if (welcome->getExitStatus() == 0) {
    MainWindow *window = new MainWindow(0);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    delete welcome;
  } else if (welcome->getExitStatus() == 1) {
    return 0;
    delete welcome;
  } else if (welcome->getExitStatus() == 2) {
    QPointer<QMessageBox> messageBox = new QMessageBox(0);
    messageBox->setText("<h2>Could not open database</h2>");
    messageBox->setInformativeText("An error occurred while attempting "
				   "to create/open the selected "
				   "database file. The program will "
				   "now exit.");
    delete welcome;
    delete messageBox;
    return 0;
  }
  return QSoPrA.exec();
}
