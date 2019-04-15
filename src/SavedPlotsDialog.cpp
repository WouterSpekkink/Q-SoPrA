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
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Q-SoPrA.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "../include/SavedPlotsDialog.h"

SavedPlotsDialog::SavedPlotsDialog(QWidget *parent, QString type) : QDialog(parent) 
{
  _selectedPlot = DEFAULT;
  _exitStatus = 1;
  _type = type;

  titleLabel = new QLabel(tr("<b>Select plot:</b>"), this);

  plotsComboBox = new QComboBox(this);
  plotsComboBox->addItem(DEFAULT);

  QSqlQuery *query = new QSqlQuery;
  if (_type == EVENTGRAPH) 
    {
      query->exec("SELECT plot FROM saved_eg_plots");
    }
  else if (_type == NETWORKGRAPH) 
    {
      query->exec("SELECT plot FROM saved_ng_plots");
    }
  else if (_type == OCCURRENCEGRAPH) 
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
  _exitStatus = 1;
  this->close();
}

void SavedPlotsDialog::loadPlot() 
{
  if (plotsComboBox->currentText() == DEFAULT)
    {
      QPointer <QMessageBox> warningBox = new QMessageBox(this);
      warningBox->addButton(QMessageBox::Ok);
      warningBox->setIcon(QMessageBox::Warning);
      warningBox->setText("<b>No plot selected</b>");
      warningBox->setInformativeText("You need to select a plot to load it.");
      warningBox->exec();
      delete warningBox;
    }
  else
    {
      _exitStatus = 0;
      _selectedPlot = plotsComboBox->currentText();
      this->close();
    }
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
	  _exitStatus = 2;
	  _selectedPlot = plotsComboBox->currentText();
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
  return _exitStatus;
}

QString SavedPlotsDialog::getSelectedPlot() 
{
  return _selectedPlot;
}
