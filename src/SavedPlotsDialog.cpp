#include "../include/SavedPlotsDialog.h"

SavedPlotsDialog::SavedPlotsDialog(QWidget *parent, QString submittedType) : QDialog(parent) 
{
  selectedPlot = DEFAULT;
  exitStatus = 1;
  type = submittedType;

  titleLabel = new QLabel(tr("<b>Select plot:</b>"), this);

  plotsComboBox = new QComboBox(this);
  plotsComboBox->addItem(DEFAULT);

  QSqlQuery *query = new QSqlQuery;
  if (type == EVENTGRAPH) 
    {
      query->exec("SELECT plot FROM saved_eg_plots");
    }
  else if (type == NETWORKGRAPH) 
    {
      query->exec("SELECT plot FROM saved_ng_plots");
    }
  else if (type == OCCURRENCEGRAPH) 
    {
      query->exec("SELECT plot FROM saved_og_plots");
    }
  while (query->next()) 
    {
      QString currentName = query->value(0).toString();
      plotsComboBox->addItem(currentName);
    }
  delete query;

  cancelCloseButton = new QPushButton(tr("Cancel"), this);
  loadPlotButton = new QPushButton(tr("Load selected plot"), this);
  removePlotButton = new QPushButton(tr("Delete selected plot"), this);
  removePlotButton->setStyleSheet("QPushButton {color: red}");

  connect(cancelCloseButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  connect(loadPlotButton, SIGNAL(clicked()), this, SLOT(loadPlot()));
  connect(removePlotButton, SIGNAL(clicked()), this, SLOT(removePlot()));

  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  QPointer<QHBoxLayout> boxLayout = new QHBoxLayout;
  boxLayout->addWidget(titleLabel);
  boxLayout->addWidget(plotsComboBox);
  mainLayout->addLayout(boxLayout);
  QPointer<QHBoxLayout> optionsLayout = new QHBoxLayout;
  optionsLayout->addWidget(cancelCloseButton);
  optionsLayout->addWidget(loadPlotButton);
  optionsLayout->addWidget(removePlotButton);
  mainLayout->addLayout(optionsLayout);

  setLayout(mainLayout);
  setWindowTitle("Saved plots");
}

void SavedPlotsDialog::cancelAndClose() 
{
  exitStatus = 1;
  this->close();
}

void SavedPlotsDialog::loadPlot() 
{
  exitStatus = 0;
  selectedPlot = plotsComboBox->currentText();
  this->close();
}

void SavedPlotsDialog::removePlot() 
{
  if (plotsComboBox->currentText() != DEFAULT) 
    {
      QPointer<QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Yes);
      warningBox->addButton(QMessageBox::No);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<h2>Are you sure?</h2>");
      warningBox->setInformativeText("Removing a plot cannot be undone. "
				     "Are you sure you want to remove this plot?");
      if (warningBox->exec() == QMessageBox::Yes) 
	{
	  exitStatus = 2;
	  selectedPlot = plotsComboBox->currentText();
	  delete warningBox;
	  this->close();
	}
      else 
	{
	  delete warningBox;
	  return;
	}
    }
  else 
    {
      return;
    } 
}

int SavedPlotsDialog::getExitStatus() 
{
  return exitStatus;
}

QString SavedPlotsDialog::getSelectedPlot() 
{
  return selectedPlot;
}
