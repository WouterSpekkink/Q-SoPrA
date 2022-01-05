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

#include "../include/WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent, EventSequenceDatabase *esdPtr) : QDialog(parent) 
{
	_esdPtr = esdPtr;
	_exitStatus = 1;

	titleLabel = new QLabel("<h1>Welcome to Q-SoPrA</h1>", this);
	titleLabel->setAlignment(Qt::AlignHCenter);
	imageLabel = new QLabel(this);
	QPixmap image(":/images/welcome_background");
	imageLabel->setPixmap(image);

	newDatabaseButton = new QPushButton("Create new database", this);
	openDatabaseButton = new QPushButton("Open existing database", this);
	exitButton = new QPushButton("Exit program", this);

	connect(newDatabaseButton, SIGNAL(clicked()), this, SLOT(newDatabase()));
	connect(openDatabaseButton, SIGNAL(clicked()), this, SLOT(openDatabase()));
	connect(exitButton, SIGNAL(clicked()), this, SLOT(quitApp()));

	QPointer<QVBoxLayout> welcomeLayout = new QVBoxLayout;
	welcomeLayout->addWidget(titleLabel);
	welcomeLayout->addWidget(imageLabel);
	QPointer<QHBoxLayout> buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(newDatabaseButton);
	buttonLayout->addWidget(openDatabaseButton);
	buttonLayout->addWidget(exitButton);
	welcomeLayout->addLayout(buttonLayout);
	setWindowTitle("Q-SoPrA (Qualitative Social Process Analysis)");
	setLayout(welcomeLayout);
}

void WelcomeDialog::newDatabase() 
{
	QString dbName = QFileDialog::getSaveFileName(this, tr("New database"),"", tr("db files (*.db)"));
	if (!dbName.trimmed().isEmpty())
	{
		if (!dbName.endsWith(".db"))
		{
			dbName.append(".db");
		}
		QFileInfo check_file(dbName);
		if (check_file.exists() && check_file.isFile())
		{
			QFile::remove(dbName);
		}
		_esdPtr->openDB(dbName);
		bool ok = _esdPtr->database.open();
		if (!ok)
		{
			QPointer<QMessageBox> errorBox = new QMessageBox(this);
			errorBox->setWindowTitle("Error");
			errorBox->setText(tr("<b>ERROR</b>"));
			errorBox->setInformativeText("Creation of new database failed.");
			errorBox->exec();
			return;
		}
		else
		{
			_esdPtr->database.exec("PRAGMA synchronous=OFF");
			_esdPtr->database.exec("PRAGMA journal_mode=MEMORY");
			QSqlQuery *query = new QSqlQuery;
			qApp->setOverrideCursor(Qt::WaitCursor); // This can take a while
			query->exec("CREATE TABLE incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"ch_order integer, "
									"timestamp text, "
									"description text, "
									"raw text, "
									"comment text, "
									"source text, "
									"mark integer)");
			query->exec("CREATE TABLE incident_attributes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text, "
									"father text)");
			query->exec("CREATE TABLE attributes_to_incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"attribute text, "
									"incident integer, "
									"value text, "
									"coder text)");
			query->exec("CREATE TABLE attributes_to_incidents_sources "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"attribute text, "
									"incident integer, "
									"source_text text, "
									"coder text)");
			query->exec("CREATE TABLE entity_attributes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text, "
									"father text)");
			query->exec("CREATE TABLE attributes_to_entities "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"attribute text, "
									"entity text, "
									"value text, "
									"new integer)");
			query->exec("CREATE TABLE entity_relationships "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"source text, "
									"target text, "
									"comment text, "
									"type text)");
			query->exec("CREATE TABLE relationship_types "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"directedness, "
									"description)");
			query->exec("CREATE TABLE relationships_to_incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"relationship text, "
									"type text, "
									"incident integer, "
									"coder text)");
			query->exec("CREATE TABLE relationships_to_incidents_sources "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"relationship text, "
									"type text, "
									"incident integer, "
									"source_text text, "
									"coder text)");
			query->exec("CREATE TABLE entities "
									"(id integer, "
									"name text, "
									"description text, "
									"father text)");
			query->exec("CREATE TABLE journal "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"time text, "
									"entry text, "
									"coder text, "
									"mark integer)");
			query->exec("CREATE TABLE linkage_types "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text, "
									"direction text)");
			query->exec("CREATE TABLE linkages "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"tail integer, "
									"head integer, "
									"type text, "
									"coder text)");
			query->exec("CREATE TABLE linkages_sources "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"tail integer, "
									"head integer, "
									"type text, "
									"coder text, "
									"istail integer, "
									"source_text text)");
			query->exec("CREATE TABLE coders "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text)");
			query->exec("INSERT INTO coders "
									"(name) "
									"VALUES ('Default')");
			query->exec("CREATE TABLE coders_to_linkage_types "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"coder text, "
									"type text, "
									"casename text, "
									"tail integer, "
									"head integer)");
			query->exec("CREATE TABLE linkage_comments "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"tail integer, "
									"head integer, "
									"comment text, "
									"coder text, "
									"type text)");
			query->exec("CREATE TABLE save_data "
									"(attributes_record integer, "
									"relationships_record integer, "
									"coder text, "
									"default_coder text)");
			query->exec("INSERT INTO save_data "
									"(attributes_record, "
									"relationships_record, "
									"coder, "
									"default_coder) "
									"VALUES (1, 1, 'Default', 'Default')");
			query->exec("CREATE TABLE linkages_sources_reference "
									"(code integer, "
									"label text)");
			query->exec("INSERT INTO linkages_sources_reference "
									"(code, label) "
									"VALUES (0, 'Head')");
			query->exec("INSERT INTO linkages_sources_reference "
									"(code, label) "
									"VALUES (1, 'Tail')");
			query->exec("CREATE TABLE saved_eg_plots "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"red integer, "
									"green integer, "
									"blue integer)");
			query->exec("CREATE TABLE saved_eg_plots_settings "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"lowerbound integer, "
									"upperbound integer, "
									"labelson integer, "
									"labelsize integer)");
			query->exec("CREATE TABLE saved_eg_plots_incident_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"ch_order integer, "
									"width integer, "
									"curxpos real, "
									"curypos real, "
									"orixpos real, "
									"oriypos real, "
									"dislodged integer, "
									"mode text, "
									"red integer,"
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE saved_eg_plots_edges "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"tail integer, "
									"head integer , "
									"tailabstractnode integer, "
									"headabstractnode integer, "
									"linkage text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE saved_eg_plots_incident_node_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"label text, "
									"curxpos integer, "
									"curypos integer, "
									"xoffset real, "
									"yoffset real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_eg_plots_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"eventid integer, "
									"ch_order integer, "
									"abstraction text, "
									"timing text, "
									"description text, "
									"comment text, "
									"width integer, "
									"mode text, "
									"curxpos real, "
									"curypos real, "
									"orixpos real, "
									"oriypos real, "
									"dislodged integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE saved_eg_plots_incidents_to_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"abstractnode integer)");
			query->exec("CREATE TABLE saved_eg_plots_embedded_incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"abstractnode integer)");
			query->exec("CREATE TABLE saved_eg_plots_abstract_nodes_to_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"son integer, "
									"father integer)");
			query->exec("CREATE TABLE saved_eg_plots_attributes_to_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"attribute text, "
									"abstractnode integer, "
									"value text)");
			query->exec("CREATE TABLE saved_eg_plots_abstract_node_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"eventid integer, "
									"label text, "
									"curxpos integer, "
									"curypos integer, "
									"xoffset real, "
									"yoffset real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_eg_plots_legend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"textred integer, "
									"textgreen integer, "
									"textblue integer, "
									"textalpha integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_eg_plots_lines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"starty real, "
									"endx real, "
									"endy real, "
									"arone integer, "
									"artwo integer, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_eg_plots_timelines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"endx real, "
									"y real, "
									"penwidth integer, "
									"majorinterval real, "
									"minordivision real, "
									"majorsize real, "
									"minorsize real, "
									"firsttick integer, "
									"lasttick integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_eg_plots_texts "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"desc text, "
									"xpos real, "
									"ypos real, "
									"width real, "
									"size int, "
									"rotation real, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_eg_plots_ellipses "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE saved_eg_plots_rects "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE saved_eg_plots_contraction "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"nodeid integer, "
									"abstract integer, "
									"xpos real, "
									"ypos real)");
			query->exec("CREATE TABLE saved_eg_plots_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"casename text, "
									"checked integer)");
			query->exec("CREATE TABLE saved_eg_plots_guides "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"horizontal integer)");
			query->exec("CREATE TABLE saved_ng_plots "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"red integer, "
									"green integer, "
									"blue integer)");
			query->exec("CREATE TABLE saved_ng_plots_network_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"entity text, "
									"description text, "
									"mode text, "
									"curxpos real, "
									"curypos real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"persistent integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE saved_ng_plots_node_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"entity text, "
									"curxpos real, "
									"curypos real, "
									"xoffset real, "
									"yoffset real, "
									"fontsize integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_ng_plots_nodelegend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer "
									"textred integer, "
									"textgreen integer, "
									"textblue integer, "
									"textalpha integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_ng_plots_edgelegend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_ng_plots_directed "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"tail text, "
									"head text, "
									"name text, "
									"comment text, "
									"type text, "
									"height integer, "
									"filtered integer, "
									"masshidden integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_ng_plots_undirected "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"tail text, "
									"head text, "
									"name text, "
									"comment text, "
									"type text, "
									"height integer, "
									"filtered integer, "
									"masshidden integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE saved_ng_plots_incidents_to_edges "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"relationship text, "
									"type text)");
			query->exec("CREATE TABLE saved_ng_plots_lines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"starty real, "
									"endx real, "
									"endy real, "
									"arone integer, "
									"artwo integer, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_ng_plots_texts "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"desc text, "
									"xpos real, "
									"ypos real, "
									"width real, "
									"size int, "
									"rotation real, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_ng_plots_ellipses "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE saved_ng_plots_rects "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE saved_ng_plots_settings "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"lowerbound integer, "
									"upperbound integer, "
									"boundson integer, "
									"weight integer, "
									"weighton integer, "
									"labelson integer)");
			query->exec("CREATE TABLE saved_ng_plots_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"casename text, "
									"checked integer)");
			query->exec("CREATE TABLE saved_ng_plots_guides "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"horizontal integer)");
			query->exec("CREATE TABLE saved_og_plots "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"distance real, "
									"red integer, "
									"green integer, "
									"blue integer)");
			query->exec("CREATE TABLE saved_og_plots_settings "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"lowerbound real, "
									"upperbound real, "
									"labelson integer, "
									"incidentlabelsonly integer, "
									"attributelabelsonly integer, "
									"labelsize integer)");
			query->exec("CREATE TABLE saved_og_plots_occurrence_items "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"ch_order integer, "
									"attribute text, "
									"width integer, "
									"curxpos real, "
									"curypos real, "
									"orixpos real, "
									"oriypos real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"hidden integer, "
									"highlighted integer, "
									"hred integer, "
									"hgreen integer, "
									"hblue integer, "
									"halpha integer, "
									"zvalue integer, "
									"perm integer, "
									"relationship integer, "
									"grouped integer)");
			query->exec("CREATE TABLE saved_og_plots_occurrence_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"attribute string, "
									"label text, "
									"curxpos integer, "
									"curypos integer, "
									"xoffset real, "
									"yoffset real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"relationship integer)");
			query->exec("CREATE TABLE saved_og_plots_legend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"relationship integer)");
			query->exec("CREATE TABLE saved_og_plots_lines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"starty real, "
									"endx real, "
									"endy real, "
									"arone integer, "
									"artwo integer, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_og_plots_timelines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"endx real, "
									"y real, "
									"penwidth integer, "
									"majorinterval real, "
									"minordivision real, "
									"majorsize real, "
									"minorsize real, "
									"firsttick integer, "
									"lasttick integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_og_plots_texts "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"desc text, "
									"xpos real, "
									"ypos real, "
									"width real, "
									"size int, "
									"rotation real, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE saved_og_plots_ellipses "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE saved_og_plots_rects "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE saved_og_plots_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"casename text, "
									"checked integer)");
			query->exec("CREATE TABLE saved_og_plots_guides "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"horizontal integer)");
			query->exec("CREATE TABLE cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text)");
			query->exec("CREATE TABLE incidents_to_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"incident integer, "
									"casename text)");
			delete query;
			qApp->restoreOverrideCursor();
			qApp->processEvents();
		}
		_exitStatus = 0;
		this->close();
	}
	else
	{
		_exitStatus = 2;
	}
}

void WelcomeDialog::openDatabase() 
{
	QString dbName = QFileDialog::getOpenFileName(this, tr("Select database"),"", tr("db files (*.db)"));
	if (!dbName.trimmed().isEmpty())
	{
		_esdPtr->openDB(dbName);
		bool ok = _esdPtr->database.open();
		if (!ok)
		{
			QPointer<QMessageBox> errorBox = new QMessageBox(this);
			errorBox->setWindowTitle("Error");
			errorBox->setText(tr("<b>ERROR</b>"));
			errorBox->setInformativeText("Could not open the database.");
			errorBox->exec();
			delete errorBox;
			return;
		}
		else
		{
			/*
				 Let's make sure that we create all tables that are not yet present.
				 For example, this could be the case after updates of the program.
			*/
			_esdPtr->database.exec("PRAGMA synchronous=OFF");
			_esdPtr->database.exec("PRAGMA journal_mode=MEMORY");
			QSqlQuery *query = new QSqlQuery;
			qApp->setOverrideCursor(Qt::WaitCursor); // This can take a while
			query->exec("CREATE TABLE IF NOT EXISTS incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"ch_order integer, "
									"timestamp text, "
									"description text, "
									"raw text, "
									"comment text, "
									"source text, "
									"mark integer)");
			query->exec("CREATE TABLE IF NOT EXISTS incident_attributes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text, "
									"father text)");
			query->exec("CREATE TABLE IF NOT EXISTS attributes_to_incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"attribute text, "
									"incident integer, "
									"value text, "
									"coder text)");
			query->exec("CREATE TABLE IF NOT EXISTS attributes_to_incidents_sources "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"attribute text, "
									"incident integer, "
									"source_text text, "
									"coder text)");
			query->exec("CREATE TABLE IF NOT EXISTS entity_attributes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text, "
									"father text)");
			query->exec("CREATE TABLE IF NOT EXISTS attributes_to_entities "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"attribute text, "
									"entity text, "
									"value text, "
									"new integer)");
			query->exec("CREATE TABLE IF NOT EXISTS entity_relationships "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"source text, "
									"target text, "
									"comment text, "
									"type text)");
			query->exec("CREATE TABLE IF NOT EXISTS relationship_types "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"directedness, "
									"description)");
			query->exec("CREATE TABLE IF NOT EXISTS relationships_to_incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"relationship text, "
									"type text, "
									"incident integer, "
									"coder text)");
			query->exec("CREATE TABLE IF NOT EXISTS relationships_to_incidents_sources "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"relationship text, "
									"type text, "
									"incident integer, "
									"source_text text, "
									"coder text)");
			query->exec("CREATE TABLE IF NOT EXISTS entities "
									"(id integer, "
									"name text, "
									"description text, "
									"father text)");
			query->exec("CREATE TABLE IF NOT EXISTS journal "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"time text, "
									"entry text "
									"coder text "
									"mark integer)");
			query->exec("CREATE TABLE IF NOT EXISTS linkage_types "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text, "
									"direction text)");
			query->exec("CREATE TABLE IF NOT EXISTS linkages "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"tail integer, "
									"head integer, "
									"type text, "
									"coder text)");
			query->exec("CREATE TABLE IF NOT EXISTS linkages_sources "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"tail integer, "
									"head integer, "
									"type text, "
									"coder text, "
									"istail integer, "
									"source_text text)");
			query->exec("CREATE TABLE IF NOT EXISTS coders "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text)");
			query->exec("CREATE TABLE IF NOT EXISTS coders_to_linkage_types "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"coder text, "
									"type text, "
									"casename text, "
									"tail integer, "
									"head integer)");
			query->exec("CREATE TABLE IF NOT EXISTS linkage_comments "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"tail integer, "
									"head integer, "
									"comment text, "
									"coder text, "
									"type text)");
			query->exec("CREATE TABLE IF NOT EXISTS save_data "
									"(attributes_record integer, "
									"relationships_record integer, "
									"coder text, "
									"default_coder text)");
			query->exec("SELECT * FROM save_data");
			query->first();
			if (query->isNull(0))
			{
				query->exec("INSERT INTO save_data "
										"(attributes_record, "
										"relationships_record, "
										"coder, "
										"default_coder) "
										"VALUES (1, 1, 'Default', 'Default')");
			}
			query->exec("CREATE TABLE IF NOT EXISTS linkages_sources_reference "
									"(code integer, "
									"label text)");
			query->exec("SELECT * FROM linkages_sources_reference");
			query->first();
			if (query->isNull(0))
			{
				query->exec("INSERT INTO linkages_sources_reference "
										"(code, label) "
										"VALUES (0, 'Head')");
				query->exec("INSERT INTO linkages_sources_reference "
										"(code, label) "
										"VALUES (1, 'Tail')");
			}
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"red integer, "
									"green integer, "
									"blue integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_settings "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"lowerbound integer, "
									"upperbound integer, "
									"labelson integer, "
									"labelsize integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_incident_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer , "
									"ch_order integer, "
									"width integer, "
									"curxpos real, "
									"curypos real, "
									"orixpos real, "
									"oriypos real, "
									"dislodged integer, "
									"mode text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_edges "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"tail integer, "
									"head integer, "
									"tailabstractnode integer, "
									"headabstractnode integer, "
									"linkage text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_incident_node_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"label text, "
									"curxpos real, "
									"curypos real, "
									"xoffset real, "
									"yoffset real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"eventid integer, "
									"ch_order integer, "
									"abstraction text, "
									"timing text, "
									"description text, "
									"comment text, "
									"width integer, "
									"mode text, "
									"curxpos real, "
									"curypos real, "
									"orixpos real, "
									"oriypos real, "
									"dislodged integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_incidents_to_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"abstractnode integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_embedded_incidents "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"abstractnode integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_abstract_nodes_to_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"son integer, "
									"father integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_attributes_to_abstract_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"attribute text, "
									"abstractnode integer, "
									"value text)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_abstract_node_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"eventid integer, "
									"label text, "
									"curxpos integer, "
									"curypos integer, "
									"xoffset real, "
									"yoffset real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_legend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"textred integer, "
									"textgreen integer, "
									"textblue integer, "
									"textalpha integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_lines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"starty real, "
									"endx real, "
									"endy real, "
									"arone integer, "
									"artwo integer, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_timelines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"endx real, "
									"y real, "
									"penwidth integer, "
									"majorinterval real, "
									"minordivision real, "
									"majorsize real, "
									"minorsize real, "
									"firsttick integer, "
									"lasttick integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_texts "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"desc text, "
									"xpos real, "
									"ypos real, "
									"width real, "
									"size int, "
									"rotation real, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_ellipses "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_rects "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_contraction "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"nodeid integer, "
									"abstract integer, "
									"xpos real, "
									"ypos real)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"casename text, "
									"checked integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_eg_plots_guides "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"horizontal integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"red integer, "
									"green integer, "
									"blue integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_network_nodes "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"entity text, "
									"description text, "
									"mode text, "
									"curxpos real, "
									"curypos real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"persistent integer, "
									"masshidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_node_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"entity text, "
									"curxpos real, "
									"curypos real, "
									"xoffset real, "
									"yoffset real, "
									"fontsize integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_nodelegend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"textred integer, "
									"textgreen integer, "
									"textblue integer, "
									"textalpha integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_edgelegend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_directed "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"tail text, "
									"head text, "
									"name text, "
									"comment text, "
									"type text, "
									"height integer, "
									"filtered integer, "
									"masshidden integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_undirected "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"tail text, "
									"head text, "
									"name text, "
									"comment text, "
									"type text, "
									"height integer, "
									"filtered integer, "
									"masshidden integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_incidents_to_edges "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"relationship text, "
									"type text)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_lines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"starty real, "
									"endx real, "
									"endy real, "
									"arone integer, "
									"artwo integer, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_texts "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"desc text, "
									"xpos real, "
									"ypos real, "
									"width real, "
									"size int, "
									"rotation real, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_ellipses "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_rects "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_settings "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"lowerbound integer, "
									"upperbound integer, "
									"boundson integer, "
									"weight integer, "
									"weighton integer, "
									"labelson integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"casename text, "
									"checked integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_ng_plots_guides "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"horizontal integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"distance, "
									"red integer, "
									"green integer, "
									"blue integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_settings "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"lowerbound real, "
									"upperbound real, "
									"labelson integer, "
									"incidentlabelsonly integer, "
									"attributelabelsonly integer, "
									"labelsize integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_occurrence_items "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"ch_order integer, "
									"attribute text, "
									"width integer, "
									"curxpos real, " // Current x position (scenePos)
									"curypos real, " // Current y position (scenePos)
									"orixpos real, " // Original x position
									"oriypos real, " // Original y position
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"hidden integer, "
									"highlighted integer, "
									"hred integer, "
									"hgreen integer, "
									"hblue integer, "
									"halpha integer, "
									"zvalue integer, "
									"perm integer, "
									"relationship integer, "
									"grouped integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_occurrence_labels "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"incident integer, "
									"attribute text, "
									"label text, "
									"curxpos integer, "
									"curypos integer, "
									"xoffset real, "
									"yoffset real, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"zvalue integer, "
									"hidden integer, "
									"relationship integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_legend "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"name text, "
									"tip text, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"relationship integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_lines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"starty real, "
									"endx real, "
									"endy real, "
									"arone integer, "
									"artwo integer, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_timelines "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"startx real, "
									"endx real, "
									"y real, "
									"penwidth integer, "
									"majorinterval real, "
									"minordivision real, "
									"majorsize real, "
									"minorsize real, "
									"firsttick integer, "
									"lasttick integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_texts "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"desc text, "
									"xpos real, "
									"ypos real, "
									"width real, "
									"size int, "
									"rotation real, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_ellipses "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_rects "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"xpos real, "
									"ypos real, "
									"topleftx real, "
									"toplefty real, "
									"toprightx real, "
									"toprighty real, "
									"bottomleftx real, "
									"bottomlefty real, "
									"bottomrightx real, "
									"bottomrighty real, "
									"rotation real, "
									"penwidth integer, "
									"penstyle integer, "
									"zvalue integer, "
									"red integer, "
									"green integer, "
									"blue integer, "
									"alpha integer, "
									"fillred integer, "
									"fillgreen integer, "
									"fillblue integer, "
									"fillalpha integer)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"coder text, "
									"casename text, "
									"checked integer)");
			query->exec("CREATE TABLE IF NOT EXISTS cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"name text, "
									"description text)");
			query->exec("CREATE TABLE IF NOT EXISTS incidents_to_cases "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"incident integer, "
									"casename text)");
			query->exec("CREATE TABLE IF NOT EXISTS saved_og_plots_guides "
									"(id integer PRIMARY KEY AUTOINCREMENT, "
									"plot text, "
									"xpos real, "
									"ypos real, "
									"horizontal integer)");
			// And now we check if the correct columns exist and create them otherwise
			QVector<QString> columns;
			query->exec("PRAGMA table_info(incidents)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("ch_order"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN ch_order integer;");
			}
			if (!columns.contains("timestamp"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN timestamp text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN description text;");
			}
			if (!columns.contains("raw"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN raw text;");
			}
			if (!columns.contains("comment"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN comment text;");
			}
			if (!columns.contains("source"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN source text;");
			}
			if (!columns.contains("mark"))
			{
				query->exec("ALTER TABLE incidents ADD COLUMN mark integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(incident_attributes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE incident_attributes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE incident_attributes ADD COLUMN name text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE incident_attributes ADD COLUMN description text;");
			}
			if (!columns.contains("father"))
			{
				query->exec("ALTER TABLE incident_attributes ADD COLUMN father text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(attributes_to_incidents)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE attributes_to_incidents "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("attribute"))
			{
				query->exec("ALTER TABLE attributes_to_incidents ADD COLUMN attribute text;");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE attributes_to_incidents ADD COLUMN incident integer;");
			}
			if (!columns.contains("value"))
			{
				query->exec("ALTER TABLE attributes_to_incidents ADD COLUMN value text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE attributes_to_incidents ADD COLUMN coder text;");
				query->exec("UPDATE attributes_to_incidents SET coder = 'Default'");
			}
			columns.clear();
			query->exec("PRAGMA table_info(attributes_to_incidents_sources)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE attributes_to_incidents_sources "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("attribute"))
			{
				query->exec("ALTER TABLE attributes_to_incidents_sources ADD COLUMN attribute text;");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE attributes_to_incidents_sources ADD COLUMN incident integer;");
			}
			if (!columns.contains("source_text"))
			{
				query->exec("ALTER TABLE attributes_to_incidents_sources ADD COLUMN source_text text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE attributes_to_incidents_sources ADD COLUMN coder text;");
				query->exec("UPDATE attributes_to_incidents_sources SET coder = 'Default'");
			}
			columns.clear();
			query->exec("PRAGMA table_info(entity_attributes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE entity_attributes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE entity_attributes ADD COLUMN name text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE entity_attributes ADD COLUMN description text;");
			}
			if (!columns.contains("father"))
			{
				query->exec("ALTER TABLE entity_attributes ADD COLUMN father text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(attributes_to_entities)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE attributes_to_entities "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("attribute"))
			{
				query->exec("ALTER TABLE attributes_to_entities ADD COLUMN attribute text;");
			}
			if (!columns.contains("entity"))
			{
				query->exec("ALTER TABLE attributes_to_entities ADD COLUMN entity text;");
			}
			if (!columns.contains("value"))
			{
				query->exec("ALTER TABLE attributes_to_entities ADD COLUMN value text;");
			}
			if (!columns.contains("new"))
			{
				query->exec("ALTER TABLE attributes_to_entities ADD COLUMN new integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(entity_relationships)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE entity_relationships "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE entity_relationships ADD COLUMN name text;");
			}
			if (!columns.contains("source"))
			{
				query->exec("ALTER TABLE entity_relationships ADD COLUMN source text;");
			}
			if (!columns.contains("target"))
			{
				query->exec("ALTER TABLE entity_relationships ADD COLUMN target text;");
			}
			if (!columns.contains("comment"))
			{
				query->exec("ALTER TABLE entity_relationships ADD COLUMN comment text;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE entity_relationships ADD COLUMN type text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(relationship_types)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE relationship_types "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE relationship_types ADD COLUMN name text;");
			}
			if (!columns.contains("directedness"))
			{
				query->exec("ALTER TABLE relationship_types ADD COLUMN directedness text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE relationship_types ADD COLUMN description text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(relationships_to_incidents)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE relationships_to_incidents "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("relationship"))
			{
				query->exec("ALTER TABLE relationships_to_incidents ADD COLUMN relationship text;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE relationships_to_incidents ADD COLUMN type text;");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE relationships_to_incidents ADD COLUMN incident integer;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE relationships_to_incidents ADD COLUMN coder text;");
				query->exec("UPDATE relationships_to_incidents SET coder = 'Default'");
			}
			columns.clear();
			query->exec("PRAGMA table_info(relationships_to_incidents_sources)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE relationships_to_incidents_sources "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("relationship"))
			{
				query->exec("ALTER TABLE relationships_to_incidents_sources "
										"ADD COLUMN relationship text;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE relationships_to_incidents_sources "
										"ADD COLUMN type text;");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE relationships_to_incidents_sources "
										"ADD COLUMN incident integer;");
			}
			if (!columns.contains("source_text"))
			{
				query->exec("ALTER TABLE relationships_to_incidents_sources "
										"ADD COLUMN source_text text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE relationships_to_incidents_sources ADD COLUMN coder text;");
				query->exec("UPDATE relationships_to_incidents_sources SET coder = 'Default'");
			}
			columns.clear();
			query->exec("PRAGMA table_info(entities)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE entities "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE entities ADD COLUMN name text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE entities ADD COLUMN description text;");
			}
			if (!columns.contains("father"))
			{
				query->exec("ALTER TABLE entities ADD COLUMN father text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(journal)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE journal "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("time"))
			{
				query->exec("ALTER TABLE journal ADD COLUMN time text;");
			}
			if (!columns.contains("entry"))
			{
				query->exec("ALTER TABLE journal ADD COLUMN entry text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE journal ADD COLUMN coder text;");
				query->exec("UPDATE journal SET coder = 'Default'");
			}
			if (!columns.contains("mark"))
			{
				query->exec("ALTER TABLE journal ADD COLUMN mark integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(linkage_types)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE linkage_types "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE linkage_types ADD COLUMN name text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE linkage_types ADD COLUMN description text;");
			}
			if (!columns.contains("direction"))
			{
				query->exec("ALTER TABLE linkage_types ADD COLUMN direction text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(linkages)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE linkages "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("tail"))
			{
				query->exec("ALTER TABLE linkages ADD COLUMN tail integer;");
			}
			if (!columns.contains("head"))
			{
				query->exec("ALTER TABLE linkages ADD COLUMN head integer;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE linkages ADD COLUMN type text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE linkages ADD COLUMN coder text;");
				query->exec("UPDATE linkages SET coder = 'Default'");
			}
			columns.clear();
			query->exec("PRAGMA table_info(linkages_sources)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE linkages_sources "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("tail"))
			{
				query->exec("ALTER TABLE linkages_sources ADD COLUMN tail integer;");
			}
			if (!columns.contains("head"))
			{
				query->exec("ALTER TABLE linkages_sources ADD COLUMN head integer;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE linkages_sources ADD COLUMN type text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE linkages_sources ADD COLUMN coder text;");
				query->exec("UPDATE linkages_sources SET coder = 'Default'");
			}
			if (!columns.contains("istail"))
			{
				query->exec("ALTER TABLE linkages_sources ADD COLUMN istail integer;");
			}
			if (!columns.contains("source_text"))
			{
				query->exec("ALTER TABLE linkages_sources ADD COLUMN source_text text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(coders)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE coders "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE coders ADD COLUMN name text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(coders_to_linkage_types)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE coders_to_linkage_types "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE coders_to_linkage_types ADD COLUMN coder text;");
				query->exec("UPDATE coders_to_linkage_types SET coder = 'Default'");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE coders_to_linkage_types ADD COLUMN type text;");
			}
			if (!columns.contains("casename"))
			{
				query->exec("ALTER TABLE coders_to_linkage_types ADD COLUMN casename text;");
				query->exec("UPDATE coders_to_linkage_types SET casename = 'All cases'");
			}
			if (!columns.contains("tail"))
			{
				query->exec("ALTER TABLE coders_to_linkage_types ADD COLUMN tail integer;");
			}
			if (!columns.contains("head"))
			{
				query->exec("ALTER TABLE coders_to_linkage_types ADD COLUMN head integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(linkage_comments)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE linkage_comments "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("tail"))
			{
				query->exec("ALTER TABLE linkage_comments ADD COLUMN tail integer;");
			}
			if (!columns.contains("head"))
			{
				query->exec("ALTER TABLE linkage_comments ADD COLUMN head integer;");
			}
			if (!columns.contains("comment"))
			{
				query->exec("ALTER TABLE linkage_comments ADD COLUMN comment text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE linkage_comments ADD COLUMN coder text;");
				query->exec("UPDATE linkage_comments SET coder = 'Default'");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE linkage_comments ADD COLUMN type text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(save_data)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("attributes_record"))
			{
				query->exec("ALTER TABLE save_data ADD COLUMN attributes_record integer;");
				query->exec("UPDATE save_data SET attributes_record = 1");
			}
			if (!columns.contains("relationships_record"))
			{
				query->exec("ALTER TABLE save_data ADD COLUMN relationships_record integer;");
				query->exec("UPDATE save_data SET relationships_record = 1");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE save_data ADD COLUMN coder text;");
				query->exec("UPDATE save_data SET coder = 'Default'");
			}
			if (!columns.contains("default_coder"))
			{
				query->exec("ALTER TABLE save_data ADD COLUMN default_coder text;");
				query->exec("UPDATE save_data SET default_coder = 'Default'");
			}
			columns.clear();
			query->exec("PRAGMA table_info(linkages_sources_reference)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			bool referencesmissing = false;
			if (!columns.contains("code"))
			{
				query->exec("ALTER TABLE linkages_sources_reference ADD COLUMN code integer;");
				referencesmissing = true;
			}
			if (!columns.contains("label"))
			{
				query->exec("ALTER TABLE linkages_sources_reference ADD COLUMN label text;");
				referencesmissing = true;
			}
			if (referencesmissing)
			{
				query->exec("DELETE FROM linkages_sources_reference");
				query->exec("INSERT INTO linkages_sources_reference "
										"(code, label) "
										"VALUES (0, 'Head')");
				query->exec("INSERT INTO linkages_sources_reference "
										"(code, label) "
										"VALUES (1, 'Tail')");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots SET coder = 'Default'");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots ADD COLUMN blue integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_settings)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_settings "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_settings ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_settings ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_settings SET coder = 'Default'");
			}
			if (!columns.contains("red"))
				if (!columns.contains("lowerbound"))
				{
					query->exec("ALTER TABLE saved_eg_plots_settings ADD COLUMN lowerbound integer;");
				}
			if (!columns.contains("upperbound"))
			{
				query->exec("ALTER TABLE saved_eg_plots_settings ADD COLUMN upperbound integer;");
			}
			if (!columns.contains("labelson"))
			{
				query->exec("ALTER TABLE saved_eg_plots_settings ADD COLUMN labelson integer;");
			}
			if (!columns.contains("labelsize"))
			{
				query->exec("ALTER TABLE saved_eg_plots_settings ADD COLUMN labelsize integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_incident_nodes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_incident_nodes SET coder = 'Default'");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN incident integer;");
			}
			if (!columns.contains("ch_order"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN ch_order integer;");
			}
			if (!columns.contains("width"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN width integer;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN curypos real;");
			}
			if (!columns.contains("orixpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN orixpos real;");
			}
			if (!columns.contains("oriypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN oriypos real;");
			}
			if (!columns.contains("dislodged"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN dislodged integer;");
			}
			if (!columns.contains("mode"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN mode text;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_eg_plots_incident_nodes SET zvalue = 3");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN hidden integer;");
			}
			if (!columns.contains("masshidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_nodes ADD COLUMN masshidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_edges)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_edges SET coder = 'Default'");
			}
			if (!columns.contains("tail"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN tail integer;");
			}
			if (!columns.contains("head"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN head integer;");
			}
			if (!columns.contains("tailabstractnode"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN tailabstractnode integer;");
			}
			if (!columns.contains("headabstractnode"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN headabstractnode integer;");
			}
			if (!columns.contains("linkage"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN linkage text;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_eg_plots_edges SET zvalue = 2");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN hidden integer;");
			}
			if (!columns.contains("masshidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_edges ADD COLUMN masshidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_incident_node_labels)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_incident_node_labels SET coder = 'Default'");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN incident integer;");
			}
			if (!columns.contains("label"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN label text;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN curypos real;");
			}
			if (!columns.contains("xoffset"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN xoffset real;");
			}
			if (!columns.contains("yoffset"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN yoffset real;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_eg_plots_incident_node_labels "
										"SET zvalue = 4");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incident_node_labels "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_abstract_nodes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_abstract_nodes SET coder = 'Default'");
			}
			if (!columns.contains("eventid"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN eventid integer;");
			}
			if (!columns.contains("ch_order"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN ch_order integer;");
			}
			if (!columns.contains("abstraction"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN abstraction text;");
			}
			if (!columns.contains("timing"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN timing text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN description text;");
			}
			if (!columns.contains("comment"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN comment text;");
			}
			if (!columns.contains("width"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN width integer;");
			}
			if (!columns.contains("mode"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN mode text;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN curypos real;");
			}
			if (!columns.contains("orixpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN orixpos real;");
			}
			if (!columns.contains("oriypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN oriypos real;");
			}
			if (!columns.contains("dislodged"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN dislodged integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_eg_plots_abstract_nodes "
										"SET zvalue = 3");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes "
										"ADD COLUMN hidden integer;");
			}
			if (!columns.contains("masshidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes ADD COLUMN masshidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_incidents_to_abstract_nodes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incidents_to_abstract_nodes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incidents_to_abstract_nodes "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incidents_to_abstract_nodes "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_incidents_to_abstract_nodes SET coder = 'Default'");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incidents_to_abstract_nodes "
										"ADD COLUMN incident integer;");
			}
			if (!columns.contains("abstractnode"))
			{
				query->exec("ALTER TABLE saved_eg_plots_incidents_to_abstract_nodes "
										"ADD COLUMN abstractnode integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_embedded_incidents)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_embedded_incidents "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_embedded_incidents "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_embedded_incidents ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_embedded_incidents SET coder = 'Default'");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE saved_eg_plots_embedded_incidents "
										"ADD COLUMN incident integer;");
			}
			if (!columns.contains("abstractnode"))
			{
				query->exec("ALTER TABLE saved_eg_plots_embedded_incidents "
										"ADD COLUMN abstractnode integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_abstract_nodes_to_abstract_nodes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes_to_abstract_nodes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes_to_abstract_nodes "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes_to_abstract_nodes "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_abstract_nodes_to_abstract_nodes "
										"SET coder = 'Default'");
			}
			if (!columns.contains("son"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes_to_abstract_nodes "
										"ADD COLUMN son integer;");
			}
			if (!columns.contains("father"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_nodes_to_abstract_nodes "
										"ADD COLUMN father integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_attributes_to_abstract_nodes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_attributes_to_abstract_nodes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_attributes_to_abstract_nodes "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_attributes_to_abstract_nodes "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_attributes_to_abstract_nodes "
										"SET coder = 'Default'");
			}
			if (!columns.contains("attribute"))
			{
				query->exec("ALTER TABLE saved_eg_plots_attributes_to_abstract_nodes "
										"ADD COLUMN attribute text;");
			}
			if (!columns.contains("abstractnode"))
			{
				query->exec("ALTER TABLE saved_eg_plots_attributes_to_abstract_nodes "
										"ADD COLUMN abstractnode integer;");
			}
			if (!columns.contains("value"))
			{
				query->exec("ALTER TABLE saved_eg_plots_attributes_to_abstract_nodes "
										"ADD COLUMN value text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_abstract_node_labels)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_abstract_node_labels SET coder = 'Default'");
			}
			if (!columns.contains("eventid"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN eventid integer;");
			}
			if (!columns.contains("label"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN label text;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN curypos real;");
			}
			if (!columns.contains("xoffset"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN xoffset real;");
			}
			if (!columns.contains("yoffset"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN yoffset real;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_abstract_node_labels "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_legend)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_legend "
										"SET coder = 'Default'");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN name text;");
			}
			if (!columns.contains("tip"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN tip text;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("textred"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN textred integer;");
			}
			if (!columns.contains("textgreen"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN textgreen integer;");
			}
			if (!columns.contains("textblue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN textblue integer;");
			}
			if (!columns.contains("textalpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN textalpha integer;");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_eg_plots_legend "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_lines)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_lines "
										"SET coder = 'Default'");
			}
			if (!columns.contains("startx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN startx real;");
			}
			if (!columns.contains("starty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN starty real;");
			}
			if (!columns.contains("endx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN endx real;");
			}
			if (!columns.contains("endy"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN endy real;");
			}
			if (!columns.contains("arone"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN blue arone integer;");
			}
			if (!columns.contains("artwo"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN artwo integer;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_lines "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_timelines)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_timelines "
										"SET coder = 'Default'");
			}
			if (!columns.contains("startx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN startx real;");
			}
			if (!columns.contains("endx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN endx real;");
			}
			if (!columns.contains("y"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN y real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("majorinterval"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN majorinterval real;");
			}
			if (!columns.contains("minordivision"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN minordivision real;");
			}
			if (!columns.contains("majorsize"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN majorsize real;");
			}
			if (!columns.contains("minorsize"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN minorsize real;");
			}
			if (!columns.contains("firsttick"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN firsttick integer;");
			}
			if (!columns.contains("lasttick"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN lasttick integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_timelines "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_texts)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_texts "
										"SET coder = 'Default'");
			}
			if (!columns.contains("desc"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN desc text;");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("width"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN width real;");
			}
			if (!columns.contains("size"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN size int;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_texts "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_ellipses)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_ellipses "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("topleftx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN topleftx real;");
			}
			if (!columns.contains("toplefty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN toplefty real;");
			}
			if (!columns.contains("toprightx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN toprightx real;");
			}
			if (!columns.contains("toprighty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN toprighty real;");
			}
			if (!columns.contains("bottomleftx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN bottomleftx real;");
			}
			if (!columns.contains("bottomlefty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN bottomlefty real;");
			}
			if (!columns.contains("bottomrightx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN bottomrightx real;");
			}
			if (!columns.contains("bottomrighty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN bottomrighty real;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("fillred"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN fillred integer;");
			}
			if (!columns.contains("fillgreen"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN fillgreen integer;");
			}
			if (!columns.contains("fillblue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN fillblue integer;");
			}
			if (!columns.contains("fillalpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_ellipses "
										"ADD COLUMN fillalpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_rects)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_rects "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("topleftx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN topleftx real;");
			}
			if (!columns.contains("toplefty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN toplefty real;");
			}
			if (!columns.contains("toprightx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN toprightx real;");
			}
			if (!columns.contains("toprighty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN toprighty real;");
			}
			if (!columns.contains("bottomleftx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN bottomleftx real;");
			}
			if (!columns.contains("bottomlefty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN bottomlefty real;");
			}
			if (!columns.contains("bottomrightx"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN bottomrightx real;");
			}
			if (!columns.contains("bottomrighty"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN bottomrighty real;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("fillred"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN fillred integer;");
			}
			if (!columns.contains("fillgreen"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN fillgreen integer;");
			}
			if (!columns.contains("fillblue"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN fillblue integer;");
			}
			if (!columns.contains("fillalpha"))
			{
				query->exec("ALTER TABLE saved_eg_plots_rects "
										"ADD COLUMN fillalpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_contraction)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_contraction "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_contraction "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_contraction "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_contraction "
										"SET coder = 'Default'");
			}
			if (!columns.contains("nodeid"))
			{
				query->exec("ALTER TABLE saved_eg_plots_contraction "
										"ADD COLUMN nodeid integer;");
			}
			if (!columns.contains("abstract"))
			{
				query->exec("ALTER TABLE saved_eg_plots_contraction "
										"ADD COLUMN abstract integer;");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_contraction "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_contraction "
										"ADD COLUMN ypos real;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_cases)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_cases "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_cases "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_cases "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_cases "
										"SET coder = 'Default'");
			}
			if (!columns.contains("casename"))
			{
				query->exec("ALTER TABLE saved_eg_plots_cases "
										"ADD COLUMN casename text;");
			}
			if (!columns.contains("checked"))
			{
				query->exec("ALTER TABLE saved_eg_plots_cases "
										"ADD COLUMN checked integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_eg_plots_guides)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_eg_plots_guides "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_eg_plots_guides "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_eg_plots_guides "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_eg_plots_guides "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_guides "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_eg_plots_guides "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("horizontal"))
			{
				query->exec("ALTER TABLE saved_eg_plots_guides "
										"ADD COLUMN horizontal integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots "
										"SET coder = 'Default'");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots "
										"ADD COLUMN blue integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_network_nodes)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_network_nodes "
										"SET coder = 'Default'");
			}
			if (!columns.contains("entity"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN entity text;");
			}
			if (!columns.contains("description"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN description text;");
			}
			if (!columns.contains("mode"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN mode text;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN curypos real;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_ng_plots_network_nodes "
										"SET zvalue = 3");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN hidden integer;");
			}
			if (!columns.contains("persistent"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN persistent integer;");
			}
			if (!columns.contains("masshidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_network_nodes "
										"ADD COLUMN masshidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_node_labels)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_node_labels "
										"SET coder = 'Default'");
			}
			if (!columns.contains("entity"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN entity text;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN curypos real;");
			}
			if (!columns.contains("xoffset"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN xoffset real;");
			}
			if (!columns.contains("yoffset"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN yoffset real;");
			}
			if (!columns.contains("fontsize"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN fontsize integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_ng_plots_node_labels "
										"SET zvalue = 4");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_node_labels "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_nodelegend)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_nodelegend "
										"SET coder = 'Default'");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN name text;");
			}
			if (!columns.contains("tip"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN tip text;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("textred"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN textred integer;");
			}
			if (!columns.contains("textgreen"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN textgreen integer;");
			}
			if (!columns.contains("textblue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN textblue integer;");
			}
			if (!columns.contains("textalpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN textalpha integer;");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_nodelegend "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_edgelegend)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_edgelegend "
										"SET coder = 'Default'");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN name text;");
			}
			if (!columns.contains("tip"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN tip text;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_edgelegend "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_directed)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_directed "
										"SET coder = 'Default'");
			}
			if (!columns.contains("tail"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN tail text;");
			}
			if (!columns.contains("head"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN head text;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN name text;");
			}
			if (!columns.contains("comment"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN comment text;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN type text;");
			}
			if (!columns.contains("height"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN height integer;");
			}
			if (!columns.contains("filtered"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN filtered integer;");
			}
			if (!columns.contains("masshidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN masshidden integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_ng_plots_directed "
										"SET zvalue = 2");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_directed "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_undirected)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_undirected "
										"SET coder = 'Default'");
			}
			if (!columns.contains("tail"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN tail text;");
			}
			if (!columns.contains("head"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN head text;");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN name text;");
			}
			if (!columns.contains("comment"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN comment text;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN type text;");
			}
			if (!columns.contains("height"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN height integer;");
			}
			if (!columns.contains("filtered"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN filtered integer;");
			}
			if (!columns.contains("masshidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN masshidden integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN zvalue integer;");
				query->exec("UPDATE saved_ng_plots_undirected "
										"SET zvalue = 2");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_ng_plots_undirected "
										"ADD COLUMN hidden integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_incidents_to_edges)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_incidents_to_edges "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_incidents_to_edges "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_incidents_to_edges "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_incidents_to_edges "
										"SET coder = 'Default'");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE saved_ng_plots_incidents_to_edges "
										"ADD COLUMN incident integer;");
			}
			if (!columns.contains("relationship"))
			{
				query->exec("ALTER TABLE saved_ng_plots_incidents_to_edges "
										"ADD COLUMN relationship text;");
			}
			if (!columns.contains("type"))
			{
				query->exec("ALTER TABLE saved_ng_plots_incidents_to_edges "
										"ADD COLUMN type text;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_lines)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_lines "
										"SET coder = 'Default'");
			}
			if (!columns.contains("startx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN startx real;");
			}
			if (!columns.contains("starty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN starty real;");
			}
			if (!columns.contains("endx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN endx real;");
			}
			if (!columns.contains("endy"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN endy real;");
			}
			if (!columns.contains("arone"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN blue arone integer;");
			}
			if (!columns.contains("artwo"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN artwo integer;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_lines "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_texts)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_texts "
										"SET coder = 'Default'");
			}
			if (!columns.contains("desc"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN desc text;");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("width"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN width real;");
			}
			if (!columns.contains("size"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN size int;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_texts "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_ellipses)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_ellipses "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("topleftx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN topleftx real;");
			}
			if (!columns.contains("toplefty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN toplefty real;");
			}
			if (!columns.contains("toprightx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN toprightx real;");
			}
			if (!columns.contains("toprighty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN toprighty real;");
			}
			if (!columns.contains("bottomleftx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN bottomleftx real;");
			}
			if (!columns.contains("bottomlefty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN bottomlefty real;");
			}
			if (!columns.contains("bottomrightx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN bottomrightx real;");
			}
			if (!columns.contains("bottomrighty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN bottomrighty real;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("fillred"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN fillred integer;");
			}
			if (!columns.contains("fillgreen"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN fillgreen integer;");
			}
			if (!columns.contains("fillblue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN fillblue integer;");
			}
			if (!columns.contains("fillalpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_ellipses "
										"ADD COLUMN fillalpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_rects)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_rects "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("topleftx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN topleftx real;");
			}
			if (!columns.contains("toplefty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN toplefty real;");
			}
			if (!columns.contains("toprightx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN toprightx real;");
			}
			if (!columns.contains("toprighty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN toprighty real;");
			}
			if (!columns.contains("bottomleftx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN bottomleftx real;");
			}
			if (!columns.contains("bottomlefty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN bottomlefty real;");
			}
			if (!columns.contains("bottomrightx"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN bottomrightx real;");
			}
			if (!columns.contains("bottomrighty"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN bottomrighty real;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("fillred"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN fillred integer;");
			}
			if (!columns.contains("fillgreen"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN fillgreen integer;");
			}
			if (!columns.contains("fillblue"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN fillblue integer;");
			}
			if (!columns.contains("fillalpha"))
			{
				query->exec("ALTER TABLE saved_ng_plots_rects "
										"ADD COLUMN fillalpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_settings)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_settings "
										"SET coder = 'Default'");
			}
			if (!columns.contains("lowerbound"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN lowerbound integer;");
			}
			if (!columns.contains("upperbound"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN upperbound integer;");
			}
			if (!columns.contains("boundson"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN boundson integer;");
			}
			if (!columns.contains("weight"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN weight integer;");
			}
			if (!columns.contains("weightson"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN weightson integer;");
			}
			if (!columns.contains("labelson"))
			{
				query->exec("ALTER TABLE saved_ng_plots_settings "
										"ADD COLUMN labelson integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_cases)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_cases "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_cases "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_cases "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_cases "
										"SET coder = 'Default'");
			}
			if (!columns.contains("casename"))
			{
				query->exec("ALTER TABLE saved_ng_plots_cases "
										"ADD COLUMN casename text;");
			}
			if (!columns.contains("checked"))
			{
				query->exec("ALTER TABLE saved_ng_plots_cases "
										"ADD COLUMN checked integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_ng_plots_guides)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_ng_plots_guides "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_ng_plots_guides "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_ng_plots_guides "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_ng_plots_guides "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_guides "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_ng_plots_guides "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("horizontal"))
			{
				query->exec("ALTER TABLE saved_ng_plots_guides "
										"ADD COLUMN horizontal integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots "
										"SET coder = 'Default'");
			}
			if (!columns.contains("distance"))
			{
				query->exec("ALTER TABLE saved_og_plots "
										"ADD COLUMN distance text;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots "
										"ADD COLUMN blue integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_settings)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_settings "
										"SET coder = 'Default'");
			}
			if (!columns.contains("lowerbound"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN lowerbound real;");
			}
			if (!columns.contains("upperbound"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN upperbound real;");
			}
			if (!columns.contains("labelson"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN labelson integer;");
			}
			if (!columns.contains("incidentlabelsonly"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN incidentlabelsonly integer;");
			}
			if (!columns.contains("attributelabelsonly"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN attributelabelsonly integer;");
			}
			if (!columns.contains("labelsize"))
			{
				query->exec("ALTER TABLE saved_og_plots_settings "
										"ADD COLUMN labelsize integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_occurrence_items)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_occurrence_items "
										"SET coder = 'Default'");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN incident integer;");
			}
			if (!columns.contains("ch_order"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN ch_order integer;");
			}
			if (!columns.contains("attribute"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN attribute text;");
			}
			if (!columns.contains("width"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN width integer;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN curypos real;");
			}
			if (!columns.contains("orixpos"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN orixpos real;");
			}
			if (!columns.contains("oriypos"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN oriypos real;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN hidden integer;");
			}
			if (!columns.contains("highlighted"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN highlighted integer;");
			}
			if (!columns.contains("hred"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN hred integer;");
			}
			if (!columns.contains("hgreen"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN hgreen integer;");
			}
			if (!columns.contains("hblue"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN hblue integer;");
			}
			if (!columns.contains("halpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN halpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("perm"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN perm integer;");
			}
			if (!columns.contains("relationship"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN relationship integer;");
			}
			if (!columns.contains("grouped"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_items "
										"ADD COLUMN grouped integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_occurrence_labels)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_occurrence_labels "
										"SET coder = 'Default'");
			}
			if (!columns.contains("incident"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN incident integer;");
			}
			if (!columns.contains("attribute"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN attribute text;");
			}
			if (!columns.contains("label"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN label text;");
			}
			if (!columns.contains("curxpos"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN curxpos real;");
			}
			if (!columns.contains("curypos"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN curypos real;");
			}
			if (!columns.contains("xoffset"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN xoffset real;");
			}
			if (!columns.contains("yoffset"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN yoffset real;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("hidden"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN hidden integer;");
			}
			if (!columns.contains("relationship"))
			{
				query->exec("ALTER TABLE saved_og_plots_occurrence_labels "
										"ADD COLUMN relationship integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_legend)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_legend "
										"SET coder = 'Default'");
			}
			if (!columns.contains("name"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN name text;");
			}
			if (!columns.contains("tip"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN tip text;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("relationship"))
			{
				query->exec("ALTER TABLE saved_og_plots_legend "
										"ADD COLUMN relationship integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_lines)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_lines "
										"SET coder = 'Default'");
			}
			if (!columns.contains("startx"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN startx real;");
			}
			if (!columns.contains("starty"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN starty real;");
			}
			if (!columns.contains("endx"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN endx real;");
			}
			if (!columns.contains("endy"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN endy real;");
			}
			if (!columns.contains("arone"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN blue arone integer;");
			}
			if (!columns.contains("artwo"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN artwo integer;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_lines "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_timelines)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_timelines "
										"SET coder = 'Default'");
			}
			if (!columns.contains("startx"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN startx real;");
			}
			if (!columns.contains("endx"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN endx real;");
			}
			if (!columns.contains("y"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN y real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("majorinterval"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN majorinterval real;");
			}
			if (!columns.contains("minordivision"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN minordivision real;");
			}
			if (!columns.contains("majorsize"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN majorsize real;");
			}
			if (!columns.contains("minorsize"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN minorsize real;");
			}
			if (!columns.contains("firsttick"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN firsttick integer;");
			}
			if (!columns.contains("lasttick"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN lasttick integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_timelines "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_texts)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_texts "
										"SET coder = 'Default'");
			}
			if (!columns.contains("desc"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN desc text;");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("width"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN width real;");
			}
			if (!columns.contains("size"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN size int;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_texts "
										"ADD COLUMN alpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_ellipses)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_ellipses "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("topleftx"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN topleftx real;");
			}
			if (!columns.contains("toplefty"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN toplefty real;");
			}
			if (!columns.contains("toprightx"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN toprightx real;");
			}
			if (!columns.contains("toprighty"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN toprighty real;");
			}
			if (!columns.contains("bottomleftx"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN bottomleftx real;");
			}
			if (!columns.contains("bottomlefty"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN bottomlefty real;");
			}
			if (!columns.contains("bottomrightx"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN bottomrightx real;");
			}
			if (!columns.contains("bottomrighty"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN bottomrighty real;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN alpha integer;");
			}
			if (!columns.contains("fillred"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN fillred integer;");
			}
			if (!columns.contains("fillgreen"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN fillgreen integer;");
			}
			if (!columns.contains("fillblue"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN fillblue integer;");
			}
			if (!columns.contains("fillalpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_ellipses "
										"ADD COLUMN fillalpha integer;");
			}
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_rects)");
			while (query->next())
			{
				columns.push_back(query->value(1).toString());
			}
			if (!columns.contains("id"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
			}
			if (!columns.contains("plot"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN plot text;");
			}
			if (!columns.contains("coder"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN coder text;");
				query->exec("UPDATE saved_og_plots_rects "
										"SET coder = 'Default'");
			}
			if (!columns.contains("xpos"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN xpos real;");
			}
			if (!columns.contains("ypos"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN ypos real;");
			}
			if (!columns.contains("topleftx"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN topleftx real;");
			}
			if (!columns.contains("toplefty"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN toplefty real;");
			}
			if (!columns.contains("toprightx"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN toprightx real;");
			}
			if (!columns.contains("toprighty"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN toprighty real;");
			}
			if (!columns.contains("bottomleftx"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN bottomleftx real;");
			}
			if (!columns.contains("bottomlefty"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN bottomlefty real;");
			}
			if (!columns.contains("bottomrightx"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN bottomrightx real;");
			}
			if (!columns.contains("bottomrighty"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN bottomrighty real;");
			}
			if (!columns.contains("rotation"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN rotation real;");
			}
			if (!columns.contains("penwidth"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN penwidth integer;");
			}
			if (!columns.contains("penstyle"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN penstyle integer;");
			}
			if (!columns.contains("zvalue"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN zvalue integer;");
			}
			if (!columns.contains("red"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN red integer;");
			}
			if (!columns.contains("green"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN green integer;");
			}
			if (!columns.contains("blue"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN blue integer;");
			}
			if (!columns.contains("alpha"))
			{
				query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN alpha integer;");
	    }
			if (!columns.contains("fillred"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN fillred integer;");
	    }
			if (!columns.contains("fillgreen"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN fillgreen integer;");
	    }
			if (!columns.contains("fillblue"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN fillblue integer;");
	    }
			if (!columns.contains("fillalpha"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_rects "
										"ADD COLUMN fillalpha integer;");
	    }
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_cases)");
			while (query->next())
	    {
	      columns.push_back(query->value(1).toString());
	    }
			if (!columns.contains("id"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_cases "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
	    }
			if (!columns.contains("plot"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_cases "
										"ADD COLUMN plot text;");
	    }
			if (!columns.contains("coder"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_cases "
										"ADD COLUMN coder text;");
	      query->exec("UPDATE saved_og_plots_cases "
										"SET coder = 'Default'");
	    }
			if (!columns.contains("casename"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_cases "
										"ADD COLUMN casename text;");
	    }
			if (!columns.contains("checked"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_cases "
										"ADD COLUMN checked integer;");
	    }
			columns.clear();
			query->exec("PRAGMA table_info(saved_og_plots_guides)");
			while (query->next())
	    {
	      columns.push_back(query->value(1).toString());
	    }
			if (!columns.contains("id"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_guides "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
	    }
			if (!columns.contains("plot"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_guides "
										"ADD COLUMN plot text;");
	    }
			if (!columns.contains("coder"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_guides "
										"ADD COLUMN coder text;");
	      query->exec("UPDATE saved_og_plots_guides "
										"SET coder = 'Default'");
	    }
			if (!columns.contains("xpos"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_guides "
										"ADD COLUMN xpos real;");
	    }
			if (!columns.contains("ypos"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_guides "
										"ADD COLUMN ypos real;");
	    }
			if (!columns.contains("horizontal"))
	    {
	      query->exec("ALTER TABLE saved_og_plots_guides "
										"ADD COLUMN horizontal integer;");
	    }
			columns.clear();
			query->exec("PRAGMA table_info(cases)");
			while (query->next())
	    {
	      columns.push_back(query->value(1).toString());
	    }
			if (!columns.contains("id"))
	    {
	      query->exec("ALTER TABLE cases "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
	    }
			if (!columns.contains("name"))
	    {
	      query->exec("ALTER TABLE cases "
										"ADD COLUMN name text;");
	    }
			if (!columns.contains("description"))
	    {
	      query->exec("ALTER TABLE cases "
										"ADD COLUMN description text;");
	    }
			columns.clear();
			query->exec("PRAGMA table_info(incidents_to_cases)");
			while (query->next())
	    {
	      columns.push_back(query->value(1).toString());
	    }
			if (!columns.contains("id"))
	    {
	      query->exec("ALTER TABLE incidents_to_cases "
										"ADD COLUMN id integer PRIMARY KEY AUTOINCREMENT;");
	    }
			if (!columns.contains("incident"))
	    {
	      query->exec("ALTER TABLE incidents_to_cases "
										"ADD COLUMN incident integer;");
	    }
			if (!columns.contains("casename"))
	    {
	      query->exec("ALTER TABLE incidents_to_cases "
										"ADD COLUMN casename text;");
	    }
			columns.clear();
			query->exec("SELECT default_coder FROM save_data");
			query->first();
			QString currentDefault = query->value(0).toString();
			query->prepare("SELECT name FROM coders "
										 "WHERE name = :name");
			query->bindValue(":name", currentDefault);
			query->exec();
			query->first();
			if (query->isNull(0))
	    {
	      query->prepare("INSERT INTO coders (name) "
											 "VALUES (:name)");
	      query->bindValue(":name", currentDefault);
	      query->exec();
	    }
			qApp->restoreOverrideCursor();
			qApp->processEvents();
			_exitStatus = 0;
			delete query;
			this->close();
		}
	}
  else 
	{
		_exitStatus = 2;
	}
}

void WelcomeDialog::quitApp() 
{
  _exitStatus = 1;
  this->close();
}

int WelcomeDialog::getExitStatus() 
{
  return _exitStatus;
}
