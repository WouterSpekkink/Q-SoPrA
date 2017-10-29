#ifndef DESELECTABLETREEVIEW_H
#define DESELECTABLETREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>

class DeselectableTreeView : public QTreeView {
public:
  DeselectableTreeView(QWidget *parent);
  ~DeselectableTreeView() {};

private:
  void mousePressEvent(QMouseEvent *event);
};

#endif
