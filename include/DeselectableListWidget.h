#ifndef DESELECTABLELISTWIDGET_H
#define DESELECTABLELISTWIDGET_H

#include <QTableWidget>
#include <QMouseEvent>

class DeselectableListWidget : public QTableWidget
{
  Q_OBJECT

public:
  DeselectableListWidget(QWidget *parent);
  ~DeselectableListWidget() {};
  void resetSelection();

signals:
  void noneSelected();

private:
  void mousePressEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);
};

  
#endif
