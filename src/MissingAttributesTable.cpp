#include "../include/MissingAttributesTable.h"


MissingAttributesTable::MissingAttributesTable(QWidget *parent) : QWidget(parent) {
  model = new EventQueryModel(this);
  tableView = new ZoomableTableView(this);
  tableView->setModel(model);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnHidden(0, true);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);

  // Connecting the signals
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(resetHeader(int)));

  // Let's fetch the data now.
  updateTable();
  
  // We make the layout.
  QPointer<QVBoxLayout> mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  QPointer<QHBoxLayout> optionLayout = new QHBoxLayout;
  mainLayout->addLayout(optionLayout);
  optionLayout->setAlignment(Qt::AlignRight);
  
  setLayout(mainLayout);
  setMouseTracking(true);
}


void MissingAttributesTable::updateTable() {
  model->clear();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT * "
	      "FROM incidents "
	      "WHERE NOT EXISTS "
	      "(SELECT incident FROM attributes_to_incidents "
	      "WHERE attributes_to_incidents.incident = incidents.id) "
	      "ORDER BY ch_order");
  model->setQuery(*query);
  delete query;
  while (model->canFetchMore()) {
    model->fetchMore();
  }
  tableView->setColumnHidden(0, true);
  tableView->setColumnHidden(5, true);
  tableView->setColumnWidth(2, qobject_cast<QWidget*>(parent())->width()/15);
  tableView->setColumnWidth(3, qobject_cast<QWidget*>(parent())->width()/3);
  tableView->setColumnWidth(4, qobject_cast<QWidget*>(parent())->width()/3);
  tableView->setColumnWidth(6, qobject_cast<QWidget*>(parent())->width()/8);
  tableView->setItemDelegateForColumn(7, new CheckBoxDelegate(tableView));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Incident"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Timing"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Description"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Raw"));
  model->setHeaderData(6, Qt::Horizontal, QObject::tr("Source"));
  model->setHeaderData(7, Qt::Horizontal, QObject::tr("Marked"));
}

void MissingAttributesTable::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
}


