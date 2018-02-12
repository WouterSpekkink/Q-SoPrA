#ifndef DESELECTABLETREEVIEWENTITIES_H
#define DESELECTABLETREEVIEWENTITIES_H

#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>
#include <QSqlQuery>
#include <QStandardItem>

class DeselectableTreeViewEntities : public QTreeView {
  Q_OBJECT

  
public:
  DeselectableTreeViewEntities(QWidget *parent);
  ~DeselectableTreeViewEntities() {};
  void resetSelection();

signals:
  void noneSelected();

protected:
  void wheelEvent(QWheelEvent *event);
  
private:
  void mousePressEvent(QMouseEvent *event);
  void dropEvent(QDropEvent *event);
};

#endif
