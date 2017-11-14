#include "../include/ZoomableTableView.h"

ZoomableTableView::ZoomableTableView(QWidget *parent) : QTableView(parent) {};

void ZoomableTableView::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() & Qt::ControlModifier) {
    if (event->angleDelta().y() > 0) {
      QFont font = this->font();
      int currentSize = font.pointSize();
      if (currentSize <= 50) {
	int newSize = currentSize + 1;
	font.setPointSize(newSize);
	this->setFont(font);
      }
      return;
    } else if (event->angleDelta().y() < 0) {
      QFont font = this->font();
      int currentSize = font.pointSize();
      if (currentSize >= 10) {
	int newSize = currentSize - 1;
	font.setPointSize(newSize);
	this->setFont(font);
      }
      return;
    }
  } else {
    QTableView::wheelEvent(event);
  }
}
