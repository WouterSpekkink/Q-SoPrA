#ifndef EVENTTABLEMODEL_H
#define EVENTTABLEMODEL_H

#include <QtSql>

class EventTableModel : public QSqlTableModel {
  Q_OBJECT

public:
  EventTableModel(QWidget *parent=0);
  ~EventTableModel() {};

protected:
  QVariant data(const QModelIndex &index, int role) const;

};

#endif
