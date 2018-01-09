#include "../include/MissingAttributesTable.h"

MissingAttributesTable::MissingAttributesTable(QWidget *parent) : QWidget(parent) {
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
  
  markButton = new QPushButton(tr("Mark selected"), this);
  exportTableButton = new QPushButton(tr("Export visible table"), this);

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
  optionLayout->addWidget(markButton);
  markButton->setMaximumWidth(markButton->sizeHint().width());
  optionLayout->addWidget(exportTableButton);
  exportTableButton->setMaximumWidth(exportTableButton->sizeHint().width());
  mainLayout->addLayout(optionLayout);
  optionLayout->setAlignment(Qt::AlignRight);
  
  setLayout(mainLayout);
  setMouseTracking(true);
}


void MissingAttributesTable::updateTable() {
  model->clear();
  QSqlQuery *query = new QSqlQuery;
  query->exec("SELECT id, ch_order, timestamp, description, raw, source "
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
    checkBox->installEventFilter(this);
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(markIncident(int)));
  }
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

void MissingAttributesTable::resetHeader(int header) {
  tableView->verticalHeader()->resizeSection(header, 30);
}


void MissingAttributesTable::markIncident(int state) {
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

void MissingAttributesTable::exportTable() {

}

void MissingAttributesTable::setRow(int row) {
  currentRow = row;
}

bool MissingAttributesTable::eventFilter(QObject *object, QEvent *event) {
  QMouseEvent *mEvent = (QMouseEvent*) event;
  QCheckBox *checkBox = qobject_cast<QCheckBox*>(object);
  if (checkBox){ 
    qDebug() << checkBox->pos();
  }
  int row = tableView->rowAt(tableView->mapFromGlobal(QCursor::pos()).y());
  setRow(row);
}
