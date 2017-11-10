#ifndef DESELECTABLETREEVIEW_H
#define DESELECTABLETREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>
#include <QSqlQuery>
#include <QStandardItem>

class DeselectableTreeView : public QTreeView {
  Q_OBJECT

  
public:
  DeselectableTreeView(QWidget *parent);
  ~DeselectableTreeView() {};
  void resetSelection();

signals:
  void noneSelected();
  
private:
  void mousePressEvent(QMouseEvent *event);
  void dropEvent(QDropEvent *event);
};

#endif
