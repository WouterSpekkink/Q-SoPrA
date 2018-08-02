#include "../include/RawAttributesTable.h"

RawAttributesTable::RawAttributesTable(QWidget *parent) : QWidget(parent) {
  // We first create our model, our table, the view and the filter of the view
  attributesModel = new QueryModel(this);

  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(attributesModel);
  filter->setFilterKeyColumn(0);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  // Then we set how the data are displayed.
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->horizontalHeader()->setSectionsMovable(true);
  tableView->horizontalHeader()->swapSections(0, 2);
  tableView->horizontalHeader()->swapSections(1, 2);
  tableView->setColumnWidth(0, 200);
  tableView->setColumnWidth(1, 600);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);
  
  attributesModel->setQuery("SELECT name, incident_attributes.description, ch_order, source_text "
			    "FROM incident_attributes "
			    "INNER JOIN attributes_to_incidents_sources ON " 
			    "incident_attributes.name = attributes_to_incidents_sources.attribute "
			    "LEFT JOIN incidents ON "
			    "attributes_to_incidents_sources.incident = incidents.id "
			    "UNION ALL "
			    "SELECT name, entities.description, ch_order, source_text "
			    "FROM entities "
			    "INNER JOIN attributes_to_incidents_sources ON "
			    "entities.name = attributes_to_incidents_sources.attribute "
			    "LEFT JOIN incidents ON "
			    "attributes_to_incidents_sources.incident = incidents.id "
			    "ORDER BY name ASC");
  attributesModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Attribute"));
  attributesModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));
  attributesModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Incident"));
  attributesModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Text"));
  updateTable();
  
  // We add the controls.
  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Attributes");
  filterComboBox->addItem("Incidents");
  filterComboBox->addItem("Descriptions");
  filterComboBox->addItem("Texts");

  removeTextButton = new QPushButton(tr("Remove selected"), this);
  editAttributeButton = new QPushButton(tr("Edit attribute"), this);
  exportTableButton = new QPushButton(tr("Export visible table"), this);

  // Connecting the signals
  connect(filterField, SIGNAL(textChanged(const QString &)),
	  this, SLOT(changeFilter(const QString &)));
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(resetHeader(int)));
  connect(tableView->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(sortHeader(int)));
  connect(filterComboBox, SIGNAL(currentIndexChanged(const QString &)),
	  this, SLOT(setFilterColumn()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(editAttributeButton, SIGNAL(clicked()), this, SLOT(editAttribute()));
  connect(exportTableButton, SIGNAL(clicked()), this, SLOT(exportTable()));
  
  // And we create the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  QPointer<QHBoxLayout>filterLayout = new QHBoxLayout;
  filterLayout->addWidget(filterComboLabel);
  filterLayout->addWidget(filterComboBox);
  filterLayout->addWidget(filterFieldLabel);
  filterLayout->addWidget(filterField);
  filterLayout->addWidget(removeTextButton);
  filterLayout->addWidget(editAttributeButton);
  filterLayout->addWidget(exportTableButton);
  mainLayout->addLayout(filterLayout);

  setLayout(mainLayout);
}

void RawAttributesTable::updateTable() {
  attributesModel->query().exec();
  while (attributesModel->canFetchMore()) {
    attributesModel->fetchMore();
  }
}

void RawAttributesTable::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
  updateTable();
}

void RawAttributesTable::sortHeader(int header) {
  if (header == 0) {
    attributesModel->setQuery("SELECT name, incident_attributes.description, ch_order, source_text "
			      "FROM incident_attributes "
			      "INNER JOIN attributes_to_incidents_sources ON " 
			      "incident_attributes.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "UNION ALL "
			      "SELECT name, entities.description, ch_order, source_text "
			      "FROM entities "
			      "INNER JOIN attributes_to_incidents_sources ON "
			      "entities.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "ORDER BY name ASC");
  } else if (header == 1) {
    attributesModel->setQuery("SELECT name, incident_attributes.description, ch_order, source_text "
			      "FROM incident_attributes "
			      "INNER JOIN attributes_to_incidents_sources ON " 
			      "incident_attributes.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "UNION ALL "
			      "SELECT name, entities.description, ch_order, source_text "
			      "FROM entities "
			      "INNER JOIN attributes_to_incidents_sources ON "
			      "entities.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "ORDER BY incident_attributes.description ASC");
  } else if (header == 2) {
    attributesModel->setQuery("SELECT name, incident_attributes.description, ch_order, source_text "
			      "FROM incident_attributes "
			      "INNER JOIN attributes_to_incidents_sources ON " 
			      "incident_attributes.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "UNION ALL "
			      "SELECT name, entities.description, ch_order, source_text "
			      "FROM entities "
			      "INNER JOIN attributes_to_incidents_sources ON "
			      "entities.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "ORDER BY ch_order ASC");
  } else if (header == 3) {
    attributesModel->setQuery("SELECT name, incident_attributes.description, ch_order, source_text "
			      "FROM incident_attributes "
			      "INNER JOIN attributes_to_incidents_sources ON " 
			      "incident_attributes.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "UNION ALL "
			      "SELECT name, entities.description, ch_order, source_text "
			      "FROM entities "
			      "INNER JOIN attributes_to_incidents_sources ON "
			      "entities.name = attributes_to_incidents_sources.attribute "
			      "LEFT JOIN incidents ON "
			      "attributes_to_incidents_sources.incident = incidents.id "
			      "ORDER BY source_text ASC");
  }
  updateTable();
}

void RawAttributesTable::changeFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  filter->setFilterRegExp(regExp);
  updateTable();
}

void RawAttributesTable::setFilterColumn() {
  if (filterComboBox->currentText() == "Attributes") {
    filter->setFilterKeyColumn(0);
  } else if (filterComboBox->currentText() == "Descriptions") {
    filter->setFilterKeyColumn(1);
  } else if (filterComboBox->currentText() == "Incidents") {
    filter->setFilterKeyColumn(2);
  } else if (filterComboBox->currentText() == "Texts") {
    filter->setFilterKeyColumn(3);
  }
}

void RawAttributesTable::removeText() {
  if (tableView->currentIndex().isValid()) {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing a fragment of text cannot be undone. "
				   "The text will also be removed from the attributes view. "
				   "Are you sure you want to proceed?");
    if (warningBox->exec() == QMessageBox::Yes) {
      int row = tableView->currentIndex().row();
      QString attribute = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
      QString order = tableView->model()->index(row, 2).data(Qt::DisplayRole).toString();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT id FROM incidents "
		     "WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      QString incident = query->value(0).toString();
      QString text = tableView->model()->index(row, 3).data(Qt::DisplayRole).toString();
      query->prepare("DELETE FROM attributes_to_incidents_sources "
		     "WHERE attribute = :attribute "
		     "AND incident = :incident AND source_text = :text");
      query->bindValue(":attribute", attribute);
      query->bindValue(":incident", incident);
      query->bindValue(":text", text);
      query->exec();
      delete query;
    }
    updateTable();
  }
}

void RawAttributesTable::editAttribute() {
  if (tableView->currentIndex().isValid()) {
    // First we need to check whether the selected attribute is an incident attribute or an entity
    bool entity = false;
    int row = tableView->currentIndex().row();
    QString attribute = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
    QSqlQuery *query = new QSqlQuery;    
    query->prepare("SELECT name FROM entities WHERE name = :attribute");
    query->bindValue(":attribute", attribute);
    query->exec();
    query->first();
    if (!query->isNull(0)) {
      entity = true;
    }
    if (entity) {
      query->prepare("SELECT description FROM entities WHERE name = :name");
      query->bindValue(":name", attribute);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QPointer<EntityDialog> entityDialog = new EntityDialog(this);
      entityDialog->submitName(attribute);
      entityDialog->submitDescription(description);
      entityDialog->exec();
      if (entityDialog->getExitStatus() == 0) {
	QString newName = entityDialog->getName();
	description = entityDialog->getDescription();
	updateEntityAfterEdit(newName, description, attribute);
	query->prepare("UPDATE attributes_to_incidents "
		       "SET attribute = :newname "
		       "WHERE attribute = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":oldname", attribute);
	query->exec();
	query->prepare("UPDATE attributes_to_incidents_sources "
		       "SET attribute = :newname "
		       "WHERE attribute = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":oldname", attribute);
	query->exec();
	query->prepare("UPDATE saved_eg_plots_attributes_to_macro_events "
		       "SET attribute = :newname "
		       "WHERE attribute = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":oldname", attribute);
	query->exec();
      }
      delete entityDialog;
    } else {
      query->prepare("SELECT description FROM incident_attributes WHERE name = :name");
      query->bindValue(":name", attribute);
      query->exec();
      query->first();
      QString description = query->value(0).toString();
      QPointer<AttributeDialog> attributeDialog = new AttributeDialog(this, INCIDENT);
      attributeDialog->submitName(attribute);
      attributeDialog->setDescription(description);
      attributeDialog->exec();
      if (attributeDialog->getExitStatus() == 0) {
	QString newName = attributeDialog->getName();
	description = attributeDialog->getDescription();
	query->prepare("UPDATE incident_attributes "
		       "SET name = :newname, description = :newdescription "
		       "WHERE name = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":newdescription", description);
	query->bindValue(":oldname", attribute);
	query->exec();
	query->prepare("UPDATE incident_attributes "
		       "SET father = :newname "
		       "WHERE father = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":oldname", attribute);
	query->exec();
	query->prepare("UPDATE attributes_to_incidents "
		       "SET attribute = :newname "
		       "WHERE attribute = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":oldname", attribute);
	query->exec();
	query->prepare("UPDATE attributes_to_incidents_sources "
		       "SET attribute = :newname "
		       "WHERE attribute = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":oldname", attribute);
	query->exec();
	query->prepare("UPDATE saved_eg_plots_attributes_to_macro_events "
		       "SET attribute = :newname "
		       "WHERE attribute = :oldname");
	query->bindValue(":newname", newName);
	query->bindValue(":oldname", attribute);
	query->exec();
	eventGraph->resetTree();
	attributesWidget->resetTree();
      }
      delete attributeDialog;
    }
    delete query;
    updateTable();
  }
}

void RawAttributesTable::updateEntityAfterEdit(const QString name,
					       const QString description,
					       const QString former) {
  QSqlQuery *query = new QSqlQuery;
  // Update the entity itself.
  query->prepare("UPDATE entities "
		 "SET name = :name, description = :description "
		 "WHERE name = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();
  // Update the parent entities
  query->prepare("UPDATE entities "
		 "SET father = :name, description = :description "
		 "WHERE father = :former");
  query->bindValue(":name", name);
  query->bindValue(":description", description);
  query->bindValue(":former", former);
  query->exec();
  // Update attributes.
  query->prepare("UPDATE attributes_to_entities "
		 "SET entity = :new "
		 "WHERE entity = :old");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();
  // Update entities assigned as attributes
  query->prepare("UPDATE attributes_to_incidents "
		 "SET attribute = :new "
		 "WHERE attribute = :old ");
  query->bindValue(":new", name);
  query->bindValue(":old", former);
  query->exec();
  /*
    Next up are the relationships in which the entity already participates.
    First, let us update all the relationships where the current entity is a source.
  */
  if (name != former) {
    query->prepare("SELECT name, target, type "
		   "FROM entity_relationships WHERE source = :current");
    query->bindValue(":current", former);
    query->exec();
    while (query->next()) {
      QString oldRelationship = query->value(0).toString();
      QString target = query->value(1).toString();
      QString type = query->value(2).toString();
      QSqlQuery *query2 = new QSqlQuery();
      query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
      query2->bindValue(":type", type);
      query2->exec();
      query2->first();
      QString directedness = query2->value(0).toString();
      QString arrow = "";
      if (directedness == UNDIRECTED) {
	arrow = "<-->";
      } else if (directedness == DIRECTED) {
	arrow = "--->";
      }
      QString newRelationship = name + arrow + target;
      query2->prepare("UPDATE entity_relationships "
		      "SET source = :source, name = :name "
		      "WHERE source = :oldSource AND name = :oldRelationship");
      query2->bindValue(":source", name);
      query2->bindValue(":name", newRelationship);
      query2->bindValue(":oldSource", former);
      query2->bindValue(":oldRelationship", oldRelationship);
      query2->exec();
      query2->prepare("UPDATE relationships_to_incidents "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query2->exec();
      query2->prepare("UPDATE relationships_to_incidents_sources "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query2->exec();
      delete query2;
    }
    // And then the relationships where the entity is a target.
    query->prepare("SELECT name, source, type "
		   "FROM entity_relationships WHERE target = :current");
    query->bindValue(":current", former);
    query->exec();
    while (query->next()) {
      QString oldRelationship = query->value(0).toString();
      QString source = query->value(1).toString();
      QString type = query->value(2).toString();
      QSqlQuery *query2 = new QSqlQuery();
      query2->prepare("SELECT directedness FROM relationship_types WHERE name = :type");
      query2->bindValue(":type", type);
      query2->exec();
      query2->first();
      QString directedness = query2->value(0).toString();
      QString arrow = "";
      if (directedness == UNDIRECTED) {
	arrow = "<-->";
      } else if (directedness == DIRECTED) {
	arrow = "--->";
      }
      QString newRelationship = source + arrow + name;
      query2->prepare("UPDATE entity_relationships "
		      "SET target = :target, name = :name "
		      "WHERE target = :oldTarget AND name = :oldRelationship");
      query2->bindValue(":target", name);
      query2->bindValue(":name", newRelationship);
      query2->bindValue(":oldTarget", former);
      query2->bindValue(":oldRelationship", oldRelationship);
      query2->exec();
      query2->prepare("UPDATE relationships_to_incidents "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query2->exec();
      query2->prepare("UPDATE relationships_to_incidents_sources "
		      "SET relationship = :new "
		      "WHERE relationship = :old");
      query2->bindValue(":new", newRelationship);
      query2->bindValue(":old", oldRelationship);
      query2->exec();
      delete query2;
    }
    delete query;
  }
  /* 
     If an entity is edited, we should communicate this change both to the
     relationships widget and the event graph. The tree of the Hierachy Graph Widget
     is reconstructed every time it is switched to, so we do not need to reset it
     explicitly.
  */
  relationshipsWidget->resetTree();
  attributesWidget->resetTree();
  eventGraph->resetTree();
}

void RawAttributesTable::exportTable() {
  updateTable();
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"",
						  tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) {
    if(!fileName.endsWith(".csv")) {
      fileName.append(".csv");
    }
    // And we create a file outstream.  
    std::ofstream fileOut(fileName.toStdString().c_str());
    // We first write the header.
    fileOut << "Incident" << ","
	    << "Attribute" << ","
	    << "Description" << ","
	    << "Text" << "\n";
    // Then we iterate through the visible table.
    for (int i = 0; i != tableView->verticalHeader()->count(); i++) {
      QString attribute = tableView->model()->index(i, 0).data(Qt::DisplayRole).toString();
      QString description = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
      QString incident = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
      QString text = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
      fileOut << incident.toStdString() << ","
	      << "\"" << doubleQuote(attribute).toStdString() << "\"" << ","
	      << "\"" << doubleQuote(description).toStdString() << "\"" << ","
	      << "\"" << doubleQuote(text).toStdString() << "\"" << "\n";
    }
    // And that should be it!
    fileOut.close();
  }
}

void RawAttributesTable::setEventGraph(EventGraphWidget *egw) {
  eventGraph = egw;
}

void RawAttributesTable::setAttributesWidget(AttributesWidget *aw) {
  attributesWidget = aw;
}

void RawAttributesTable::setRelationshipsWidget(RelationshipsWidget *rw) {
  relationshipsWidget = rw;
}

