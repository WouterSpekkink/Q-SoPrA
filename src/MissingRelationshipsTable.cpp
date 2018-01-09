#include "../include/MissingRelationshipsTable.h"


MissingRelationshipsTable::MissingRelationshipsTable(QWidget *parent) : QWidget(parent) {
  model = new QSqlQueryModel(this);
  tableView = new ZoomableTableView(this);
  tableView->setModel(model);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->setColumnHidden(0, true);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);
  tableView->setColumnWidth(2, 400);
  tableView->setColumnWidth(3, 400);
  tableView->setColumnWidth(4, 400);
  tableView->setMouseTracking(true);
  tableView->installEventFilter(this);

  // Connecting the signals
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
	  this, SLOT(resetHeader(int)));

  // TODO

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


void MissingRelationshipsTable::updateTable() {
  model->clear();
  if (mapper) {
    delete mapper;
  }
  mapper = new QSignalMapper(this);
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT id, ch_order, timestamp, description, raw, source "
	      "FROM incidents "
	      "WHERE NOT EXISTS "
	      "(SELECT incident FROM relationships_to_incidents "
	      "WHERE relationships_to_incidents.incident = incidents.id) "
	      "ORDER BY ch_order");
  model->setQuery(*query);
  delete query;
  while (model->canFetchMore()) {
    model->fetchMore();
  }
  model->insertColumn(6);
  for (int i = 0; i != tableView->verticalHeader()->count(); i++) {
    QWidget *widget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    widget->setLayout(layout);
    tableView->setIndexWidget(model->index(i, 6), widget);
    int order = tableView->model()->index(i, 1).data(Qt::DisplayRole).toInt();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT mark FROM incidents "
		   "WHERE ch_order = :order");
    query->bindValue(":order", order);
    query->exec();
    query->first();
    int mark = query->value(0).toInt();
    if (mark == 1) {
      QWidget *widget = tableView->indexWidget(model->index(i, 6));
      QCheckBox *checkBox = widget->findChild<QCheckBox*>();
      checkBox->setChecked(true);
    } else if (mark == 0) {
      QWidget *widget = tableView->indexWidget(model->index(i, 6));
      QCheckBox *checkBox = widget->findChild<QCheckBox*>();
      checkBox->setChecked(false);
    }
    connect(checkBox, SIGNAL(stateChanged(int)), mapper, SLOT(map()));
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(markIncident(int)));
    mapper->setMapping(checkBox, i);
  }
  connect(mapper, SIGNAL(mapped(int)), this, SLOT(setRow(int)));
  tableView->setColumnHidden(0, true);
  tableView->setColumnWidth(2, 400);
  tableView->setColumnWidth(3, 400);
  tableView->setColumnWidth(4, 400);
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Incident"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Timing"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Description"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Raw"));
  model->setHeaderData(5, Qt::Horizontal, QObject::tr("Source"));
  model->setHeaderData(6, Qt::Horizontal, QObject::tr("Marked"));
}

void MissingRelationshipsTable::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
}


void MissingRelationshipsTable::markIncident(int state) {
  // First we check whether rows were selected.
  int order = tableView->model()->index(currentRow, 1).data(Qt::DisplayRole).toInt();
  QSqlQuery *query = new QSqlQuery;
  if (state == 2) {
    query->prepare("UPDATE incidents SET mark = 1 "
		   "WHERE ch_order = :order");
    query->bindValue(":order", order);
    query->exec();
    delete query;
  } else if (state == 0) {
    query->prepare("UPDATE incidents SET mark = 0 "
		   "WHERE ch_order = :order");
    query->bindValue(":order", order);
    query->exec();
    delete query;
  }
}

void MissingRelationshipsTable::exportTable() {

}

void MissingRelationshipsTable::setRow(int row) {
  currentRow = row;
}

