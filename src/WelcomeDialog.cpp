#include "../include/WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent) : QDialog(parent) {
  titleLabel = new QLabel("<h1>Start menu</h1>");
  newDataBaseButton = new QPushButton("Create new database");
  openDataBaseButton = new QPushButton("Open existing database");
  exitButton = new QPushButton("Exit program");

  connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

  QPointer<QVBoxLayout> welcomeLayout = new QVBoxLayout;
  welcomeLayout->addWidget(titleLabel);
  welcomeLayout->addWidget(newDataBaseButton);
  welcomeLayout->addWidget(openDataBaseButton);
  welcomeLayout->addWidget(exitButton);
  setWindowTitle("Event Dataset");
  setLayout(welcomeLayout);  
}
