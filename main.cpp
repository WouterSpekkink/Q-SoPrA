/*

Qualitative Social Process Analysis (Q-SoPrA)
Copyright (C) 2019 University of Manchester  

This file is part of Q-SoPrA.

Q-SoPrA is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Q-SoPrA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.2

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QtWidgets/QApplication>
#include "./include/WelcomeDialog.h"
#include "./include/MainWindow.h"
#include "./include/EventSequenceDatabase.h"

int main(int argc, char *argv[]) {
  QApplication::setStyle("fusion");
  QApplication QSoPrA(argc, argv);
  QFont font("Lato");
  font.setStyleHint(QFont::Monospace);
  font.setPointSize(12);
  QApplication::setFont(font);
  QSoPrA.setWindowIcon(QIcon("./images/Q-SoPrA.png"));
  QString version = QString("0.9");
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
