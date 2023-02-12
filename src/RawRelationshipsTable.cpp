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

#include "../include/RawRelationshipsTable.h"

RawRelationshipsTable::RawRelationshipsTable(QWidget *parent) : QWidget(parent) 
{
  _lastSortedHeader = 0;
  _lastSortedAscending = true;
  
  // We first create our model, our table, the view and the filter of the view
  relationshipsModel = new RelationalTable(this);
  relationshipsModel->setTable("relationships_to_incidents_sources");
  relationshipsModel->select();
  filter = new QSortFilterProxyModel(this);
  filter->setSourceModel(relationshipsModel);
  filter->setFilterKeyColumn(5);
  tableView = new ZoomableTableView(this);
  tableView->setModel(filter);
  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // We set the incident column to show the order variable.
  relationshipsModel->setRelation(0, QSqlRelation("relationships_to_incidents_sources", "id", "type"));
  relationshipsModel->setRelation(2, QSqlRelation("relationship_types", "name", "description"));
  relationshipsModel->setRelation(3, QSqlRelation("incidents", "id", "ch_order")); 
  
  // Then we set how the data are displayed.
  relationshipsModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Type"));
  relationshipsModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Relationship"));
  relationshipsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
  relationshipsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Incident"));
  relationshipsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Text"));
  relationshipsModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Coder"));
  tableView->horizontalHeader()->setStretchLastSection(true);
  tableView->horizontalHeader()->setSectionsMovable(true);
  tableView->horizontalHeader()->swapSections(0, 5);
  tableView->horizontalHeader()->swapSections(1, 3);
  tableView->horizontalHeader()->swapSections(2, 3);
  tableView->horizontalHeader()->swapSections(3, 5);
  tableView->horizontalHeader()->swapSections(4, 5);
  tableView->setColumnWidth(0, 100);
  tableView->setColumnWidth(1, 400);
  tableView->setColumnWidth(2, 600);
  tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
  tableView->setSelectionMode( QAbstractItemView::SingleSelection );
  tableView->verticalHeader()->setDefaultSectionSize(30);
  tableView->setWordWrap(true);
  tableView->setTextElideMode(Qt::ElideMiddle);

  // We first sort by type
  relationshipsModel->sort(0, Qt::AscendingOrder);
  
  // We add the controls.
  filterComboLabel = new QLabel(tr("<b>Pick filter column:</b>"), this);
  filterFieldLabel = new QLabel(tr("<b>Filter:</b>"), this);
  
  filterField = new QLineEdit(this);

  filterComboBox = new QComboBox(this);
  filterComboBox->addItem("Coder");
  filterComboBox->addItem("Incident");
  filterComboBox->addItem("Relationship");
  filterComboBox->addItem("Type");
  filterComboBox->addItem("Description");
  filterComboBox->addItem("Text");

  removeTextButton = new QPushButton(tr("Remove selected"), this);
  editTypeButton = new QPushButton(tr("Edit relationship type"), this);
  switchTypeButton = new QPushButton(tr("Switch relationship type"), this);
  editRelationshipButton = new QPushButton(tr("Edit relationship"), this);
  exportTableButton = new QPushButton(tr("Export visible table"), this);

  // Connecting the signals
  connect(filterField, SIGNAL(textChanged(const QString &)),
          this, SLOT(changeFilter(const QString &)));
  connect(tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)),
          this, SLOT(resetHeader(int)));
  connect(tableView->horizontalHeader(), SIGNAL(sectionDoubleClicked(int)),
          this, SLOT(sortHeader(int)));
  connect(filterComboBox, SIGNAL(currentTextChanged(const QString &)),
          this, SLOT(setFilterColumn()));
  connect(removeTextButton, SIGNAL(clicked()), this, SLOT(removeText()));
  connect(editTypeButton, SIGNAL(clicked()), this, SLOT(editType()));
  connect(switchTypeButton, SIGNAL(clicked()), this, SLOT(switchType()));
  connect(editRelationshipButton, SIGNAL(clicked()), this, SLOT(editRelationship()));
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
  filterLayout->addWidget(editTypeButton);
  filterLayout->addWidget(switchTypeButton);
  filterLayout->addWidget(editRelationshipButton);
  filterLayout->addWidget(exportTableButton);
  mainLayout->addLayout(filterLayout);

  setLayout(mainLayout);
}

void RawRelationshipsTable::updateTable() 
{
  relationshipsModel->select();
  while (relationshipsModel->canFetchMore()) 
  {
    relationshipsModel->fetchMore();
  }
}

void RawRelationshipsTable::resetHeader(int header) 
{
  tableView->verticalHeader()->resizeSection(header, 30);
  updateTable();
}

void RawRelationshipsTable::sortHeader(int header) 
{
  if (header == _lastSortedHeader)
  {
    if (_lastSortedAscending)
    {
      relationshipsModel->sort(header, Qt::DescendingOrder);
    }
    else
    {
      relationshipsModel->sort(header, Qt::AscendingOrder);
    }
    _lastSortedAscending = !_lastSortedAscending;
  }
  else
  {
    relationshipsModel->sort(header, Qt::AscendingOrder);
    _lastSortedAscending = true;
  }
  _lastSortedHeader = header;
  updateTable();
}

void RawRelationshipsTable::changeFilter(const QString &text) 
{
  QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
  filter->setFilterRegularExpression(regExp);
  updateTable();
}

void RawRelationshipsTable::setFilterColumn() 
{
  if (filterComboBox->currentText() == "Coder") 
  {
    filter->setFilterKeyColumn(5);
  }
  else if (filterComboBox->currentText() == "Incident") 
  {
    filter->setFilterKeyColumn(3);
  }
  else if (filterComboBox->currentText() == "Relationship") 
  {
    filter->setFilterKeyColumn(1);
  }
  else if (filterComboBox->currentText() == "Type") 
  {
    filter->setFilterKeyColumn(0);
  }
  else if (filterComboBox->currentText() == "Description")
  {
    filter->setFilterKeyColumn(2);
  }

  else if (filterComboBox->currentText() == "Text") 
  {
    filter->setFilterKeyColumn(4);
  }
}

void RawRelationshipsTable::removeText() 
{
  if (tableView->currentIndex().isValid()) 
  {
    QPointer<QMessageBox> warningBox = new QMessageBox(this);
    warningBox->setWindowTitle("Removing text");
    warningBox->addButton(QMessageBox::Yes);
    warningBox->addButton(QMessageBox::No);
    warningBox->setIcon(QMessageBox::Warning);
    warningBox->setText("<h2>Are you sure?</h2>");
    warningBox->setInformativeText("Removing a fragment of text cannot be undone. "
                                   "The text will also be removed from the relationships view. "
                                   "Are you sure you want to proceed?");
    if (warningBox->exec() == QMessageBox::Yes)
    {
      int row = tableView->currentIndex().row();
      QString type = tableView->model()->index(row, 3).data(Qt::DisplayRole).toString();
      QString relationship = tableView->model()->index(row, 2).data(Qt::DisplayRole).toString();
      QString order = tableView->model()->index(row, 1).data(Qt::DisplayRole).toString();
      QSqlQuery *query = new QSqlQuery;
      query->prepare("SELECT id FROM incidents "
                     "WHERE ch_order = :order");
      query->bindValue(":order", order);
      query->exec();
      query->first();
      QString incident = query->value(0).toString();
      QString text = tableView->model()->index(row, 5).data(Qt::DisplayRole).toString();
      QString coder = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
      query->prepare("DELETE FROM relationships_to_incidents_sources "
                     "WHERE relationship = :relationship AND type = :type "
                     "AND incident = :incident AND source_text = :text AND coder = :coder");
      query->bindValue(":relationship", relationship);
      query->bindValue(":type", type);
      query->bindValue(":incident", incident);
      query->bindValue(":text", text);
      query->bindValue(":coder", coder);
      query->exec();
      delete query;
    }
    relationshipsModel->select();
    updateTable();
  }
}

void RawRelationshipsTable::exportTable() 
{
  updateTable();
  // We let the user set the file name and location.
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save table"),"", tr("csv files (*.csv)"));
  if (!fileName.trimmed().isEmpty()) 
  {
    if(!fileName.endsWith(".csv"))
    {
      fileName.append(".csv");
    }
    // And we create a file outstream.
    std::ofstream fileOut(fileName.toStdString().c_str());
    // We first write the header.
    fileOut << "Coder" << ","
            << "Incident" << ","
            << "Relationship" << ","
            << "Type" << ","
            << "Description" << ","
            << "Text" << "\n";
    // Then we iterate through the visible table.
    for (int i = 0; i != tableView->verticalHeader()->count(); i++)
    {
      QString coder = tableView->model()->index(i, 5).data(Qt::DisplayRole).toString();
      QString type = tableView->model()->index(i, 0).data(Qt::DisplayRole).toString();
      QString relationship = tableView->model()->index(i, 1).data(Qt::DisplayRole).toString();
      QString description = tableView->model()->index(i, 2).data(Qt::DisplayRole).toString();
      QString incident = tableView->model()->index(i, 3).data(Qt::DisplayRole).toString();
      QString text = tableView->model()->index(i, 4).data(Qt::DisplayRole).toString();
      fileOut << "\"" << coder.toStdString() << "\"" << ","
              << incident.toStdString() << ","
              << "\"" << doubleQuote(relationship).toStdString() << "\"" << ","
              << "\"" << doubleQuote(type).toStdString() << "\"" << ","
              << "\"" << doubleQuote(description).toStdString() << "\"" << ","
              << "\"" << doubleQuote(text).toStdString() << "\"" << "\n";
    }
    // And that should be it!
    fileOut.close();
  }
}

void RawRelationshipsTable::editType() 
{
  if (tableView->currentIndex().isValid()) 
  {
    int row = tableView->currentIndex().row();
    QString type = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT directedness, description FROM relationship_types WHERE name = :name");
    query->bindValue(":name", type);
    query->exec();
    query->first();
    QString directedness = query->value(0).toString();
    QString description = query->value(1).toString();
    QPointer<RelationshipTypeDialog> typeDialog = new RelationshipTypeDialog(this);
    typeDialog->submitName(type);
    typeDialog->submitDescription(description);
    typeDialog->submitDirectedness(directedness);
    typeDialog->exec();
    if (typeDialog->getExitStatus() == 0)
    {
      QString newName = typeDialog->getName();
      description = typeDialog->getDescription();
      directedness = typeDialog->getDirectedness();
      query->prepare("UPDATE relationship_types SET name = :newname, "
                     "directedness = :newdirectedness, description = :newdescription "
                     "WHERE name = :oldname");
      query->bindValue(":newname", newName);
      query->bindValue(":newdirectedness", directedness);
      query->bindValue(":newdescription", description);
      query->bindValue(":oldname", type);
      query->exec();
      query->prepare("UPDATE entity_relationships SET type = :newtype WHERE type = :oldtype");
      query->bindValue(":newtype", newName);
      query->bindValue(":oldtype", type);
      query->exec();
      query->prepare("UPDATE relationships_to_incidents SET type = :newtype WHERE type = :oldtype");
      query->bindValue(":newtype", newName);
      query->bindValue(":oldtype", type);
      query->exec();
      query->prepare("UPDATE relationships_to_incidents_sources "
                     "SET type = :newtype WHERE type = :oldtype");
      query->bindValue(":newtype", newName);
      query->bindValue(":oldtype", type);
      query->exec();
    }
    delete query;
    updateTable();
    _relationshipsWidgetPtr->resetTree();
  }
}

void RawRelationshipsTable::switchType() 
{
  if (tableView->currentIndex().isValid()) 
  {
    int row = tableView->currentIndex().row();
    QString type = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
    QString relationship = tableView->model()->index(row, 1).data(Qt::DisplayRole).toString();
    QString incident = tableView->model()->index(row, 3).data(Qt::DisplayRole).toString();
    QVector<QString> relationshipTypes;
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT name FROM relationship_types ORDER BY name ASC");
    while (query->next())
    {
      QString current = query->value(0).toString();
      relationshipTypes.push_back(current);
    }
    QPointer<ComboBoxDialog> comboBoxDialog = new ComboBoxDialog(this, relationshipTypes);
    comboBoxDialog->setWindowTitle("Switch relationship type");
    comboBoxDialog->exec();
    if (comboBoxDialog->getExitStatus() == 0)
    {
      QString newType = comboBoxDialog->getSelection();
      query->prepare("UPDATE entity_relationships "
                     "SET type = :newType "
                     "WHERE type = :oldType AND name = :relationship");
      query->bindValue(":newType", newType);
      query->bindValue(":oldType", type);
      query->bindValue(":relationship", relationship);
      query->exec();
      query->prepare("UPDATE relationships_to_incidents "
                     "SET type = :newType "
                     "WHERE type = :oldType AND incident = :incident "
                     "AND relationship = :relationship");
      query->bindValue(":newType", newType);
      query->bindValue(":oldType", type);
      query->bindValue(":incident", incident);
      query->bindValue(":relationship", relationship);
      query->exec();
      query->prepare("UPDATE relationships_to_incidents_sources "
                     "SET type = :newType "
                     "WHERE type = :oldType AND incident = :incident "
                     "AND relationship = :relationship");
      query->bindValue(":newType", newType);
      query->bindValue(":oldType", type);
      query->bindValue(":incident", incident);
      query->bindValue(":relationship", relationship);
      query->exec();
    }
    delete query;
    delete comboBoxDialog;
    _relationshipsWidgetPtr->resetTree();
    updateTable();
  }
}

void RawRelationshipsTable::editRelationship() 
{
  if (tableView->currentIndex().isValid()) 
  {
    int row = tableView->currentIndex().row();
    QString type = tableView->model()->index(row, 0).data(Qt::DisplayRole).toString();
    QString relationship = tableView->model()->index(row, 1).data(Qt::DisplayRole).toString();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT directedness, description FROM relationship_types WHERE name = :name");
    query->bindValue(":name", type);
    query->exec();
    query->first();
    QString directedness = query->value(0).toString();
    QString description = query->value(1).toString();
    query->prepare("SELECT source, target FROM entity_relationships WHERE name = :name");
    query->bindValue(":name", relationship);
    query->exec();
    query->first();
    QString leftEntity = query->value(0).toString();
    QString rightEntity = query->value(1).toString();
    QPointer<RelationshipsDialog> relationshipsDialog = new RelationshipsDialog(this);
    relationshipsDialog->submitType(type);
    relationshipsDialog->submitDescription(description);
    relationshipsDialog->submitDirectedness(directedness);
    relationshipsDialog->submitLeftEntity(leftEntity);
    relationshipsDialog->submitRightEntity(rightEntity);
    relationshipsDialog->submitName(relationship);
    relationshipsDialog->exec();
    if (relationshipsDialog->getExitStatus() == 0)
    {
      QString name = relationshipsDialog->getName();
      QString leftEntity = relationshipsDialog->getLeftEntity();
      QString rightEntity = relationshipsDialog->getRightEntity();
      query->prepare("UPDATE entity_relationships "
                     "SET name = :name, source = :leftEntity, target = :rightEntity, type = :type "
                     "WHERE name = :oldName");
      query->bindValue(":name", name);
      query->bindValue(":leftEntity", leftEntity);
      query->bindValue(":rightEntity", rightEntity);
      query->bindValue(":type", type);
      query->bindValue(":oldName", relationship);
      query->exec();
      query->prepare("UPDATE relationships_to_incidents "
                     "SET relationship = :newName "
                     "WHERE name = :oldName AND type = :type");
      query->bindValue(":newName", name);
      query->bindValue(":oldName", relationship);
      query->bindValue(":type", type);
      query->exec();
      query->prepare("UPDATE relationships_to_incidents_sources "
                     "SET relationship = :newName "
                     "WHERE name = :oldName AND type = :type");
      query->bindValue(":newName", name);
      query->bindValue(":oldName", relationship);
      query->bindValue(":type", type);
      query->exec();
    }
    if (relationshipsDialog->getEntityEdited() == 1)
    {
      _relationshipsWidgetPtr->resetTree();
    }
    delete relationshipsDialog;
    delete query;
  }
}

void RawRelationshipsTable::setRelationshipsWidget(RelationshipsWidget *relationshipsWidgetPtr) 
{
  _relationshipsWidgetPtr = relationshipsWidgetPtr;
}
