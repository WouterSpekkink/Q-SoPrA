#include "../include/RawRelationshipsTable.h"

RawRelationshipsTable::RawRelationshipsTable(QWidget *parent) : QWidget(parent) {
  // We first create our model, our table, the view and the filter of the view
  relationshipsModel = new EventTableModel(this);
  relationshipsModel->setTable("relationships_to_incidents_sources");
  relationshipsModel->select();
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(relationshipsModel);
  filter->setFilterKeyColumn(1);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  // Then we set how the data are displayed.
  relationshipsModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Relationship"));
  relationshipsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
  relationshipsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Incident"));
  relationshipsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Text"));
  tableView->setColumnHidden(0, true);
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnWidth(1, 400);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);

  // We add the controls.
  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Relationships");
  filterComboBox->addItem("Types");
  filterComboBox->addItem("Incidents");
  filterComboBox->addItem("Texts");

  removeTextButton = new QPushButton(tr("Remove selected"), this);
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
  filterLayout->addWidget(exportTableButton);
  mainLayout->addLayout(filterLayout);

  setLayout(mainLayout);
}

void RawRelationshipsTable::updateTable() {
  // We sort by relationship
  relationshipsModel->sort(1, Qt::AscendingOrder);
  while (relationshipsModel->canFetchMore()) {
    relationshipsModel->fetchMore();
  }
}

void RawRelationshipsTable::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
}

void RawRelationshipsTable::sortHeader(int header) {
  relationshipsModel->sort(header, Qt::AscendingOrder);
}

void RawRelationshipsTable::changeFilter(const QString &text) {
  QRegExp regExp(text, Qt::CaseInsensitive);
  filter->setFilterRegExp(regExp);
}

void RawRelationshipsTable::setFilterColumn() {
  if (filterComboBox->currentText() == "Relationships") {
    filter->setFilterKeyColumn(1);
  } else if (filterComboBox->currentText() == "Types") {
    filter->setFilterKeyColumn(2);
  } else if (filterComboBox->currentText() == "Incidents") {
    filter->setFilterKeyColumn(3);
  } else if (filterComboBox->currentText() == "Texts") {
    filter->setFilterKeyColumn(4);
  }
}

void RawRelationshipsTable::removeText() {
  if (tableView->currentIndex().isValid()) {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing a fragment of text cannot be undone. "
				   "The text will also be removed from the relationships view. "
				   "Are you sure you want to proceed?");
    if (warningBox->exec() == QMessageBox::Yes) {
      int row = tableView->currentIndex().row();
      QString relationship = tableView->model()->index(row, 1).data(Qt::DisplayRole).toString();
      QString type = tableView->model()->index(row, 2).data(Qt::DisplayRole).toString();
      QString incident = tableView->model()->index(row, 3).data(Qt::DisplayRole).toString();
      QString text = tableView->model()->index(row, 4).data(Qt::DisplayRole).toString();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("DELETE FROM relationships_to_incidents_sources "
		     "WHERE relationship = :relationship AND type = :type "
		     "AND incident = :incident AND source_text = :text");
      query->bindValue(":relationship", relationship);
      query->bindValue(":type", type);
      query->bindValue(":incident", incident);
      query->bindValue(":text", text);
      query->exec();
      delete query;
    }
    relationshipsModel->select();
    updateTable();
  }
}

void RawRelationshipsTable::exportTable() {
  updateTable();
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) {
    if(!fileName.endsWith(".csv")) {
      fileName.append(".csv");
    }
    // And we create a file outstream.  
    std::ofstream fileOut(fileName.toStdString().c_str());
    // We first write the header.
    fileOut << "Relationship" << ","
	    << "Type" << ","
	    << "Incident" << ","
	    << "Text" << "\n";
    // Then we iterate through the visible table.
    for (int i = 0; i != tableView->verticalHeader()->count(); i++) {
      QString relationship = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
      QString type = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
      QString incident = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
      QString text = tableView->model()->index(i, 4).data(Qt::DisplayRole).toString();
      fileOut << "\"" << relationship.toStdString() << "\"" << ","
	      << "\"" << type.toStdString() << "\"" << ","
	      << incident.toStdString() << ","
	      << "\"" << text.toStdString() << "\"" << "\n";
    }
    // And that should be it!
    fileOut.close();
  }
}
