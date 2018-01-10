#include "../include/RawAttributesTable.h"

RawAttributesTable::RawAttributesTable(QWidget *parent) : QWidget(parent) {
  // We first create our model, our table, the view and the filter of the view
  attributesModel = new RelationalTable(this);
  attributesModel->setTable("attributes_to_incidents_sources");
  attributesModel->select();
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(attributesModel);
  filter->setFilterKeyColumn(0);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  // We set a few columns to display the data a bit differently.
  attributesModel->setRelation(0,
			       QSqlRelation("attributes_to_incidents_sources", "id", "attribute"));
  attributesModel->setRelation(1, QSqlRelation("incident_attributes", "name", "description")); 
  attributesModel->setRelation(2, QSqlRelation("incidents", "id", "ch_order")); 
  
  // Then we set how the data are displayed.
  attributesModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Attribute"));
  attributesModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));
  attributesModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Incident"));
  attributesModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Text"));
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->horizontalHeader()->setSectionsMovable(true);
  tableView->horizontalHeader()->swapSections(0, 2);
  tableView->setColumnWidth(0, 200);
  tableView->setColumnWidth(1, 600);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);

  // We first sort by attribute
  attributesModel->sort(0, Qt::AscendingOrder);
  
  // We add the controls.
  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Incidents");
  filterComboBox->addItem("Attributes");
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
  
  // We fetch and sort the data.
  updateTable();

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
  while (attributesModel->canFetchMore()) {
    attributesModel->fetchMore();
  }
}

void RawAttributesTable::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
  updateTable();
}

void RawAttributesTable::sortHeader(int header) {
  attributesModel->sort(header, Qt::AscendingOrder);
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
    attributesModel->select();
    updateTable();
  }
}

void RawAttributesTable::editAttribute() {
  if (tableView->currentIndex().isValid()) {
    int row = tableView->currentIndex().row();
    QString attribute = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
    QSqlQuery *query = new QSqlQuery;
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
    delete query;
    delete attributeDialog;
    updateTable();
  }
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
