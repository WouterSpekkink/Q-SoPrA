#include "../include/GraphicsView.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

GraphicsView::GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) {
  panSpeed = 4;
  scaleFact = 1;
}

void GraphicsView::resizeEvent(QResizeEvent *) {};

void GraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    pan = true;
    setCursor(Qt::ClosedHandCursor);
    lastMousePos = event->pos();
    return;
  } else {
    QGraphicsView::mousePressEvent(event);
  }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    pan = false;
    setCursor(Qt::ArrowCursor);
    lastMousePos = event->pos();

    return;
  } else {
    QGraphicsView::mouseReleaseEvent(event);
  }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
  if (pan) {
    QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(lastMousePos);
    
    mouseDelta *= scaleFact;
    mouseDelta *= panSpeed;
    
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - mouseDelta.x(),  VIEW_HEIGHT / 2 - mouseDelta.y());
    centerOn(mapToScene(newCenter));
    lastMousePos = event->pos();
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    QRectF currentRect = this->scene()->itemsBoundingRect();
    currentRect.setX(currentRect.x() - 50);
    currentRect.setY(currentRect.y() - 50);
    currentRect.setWidth(currentRect.width() + 100);
    currentRect.setHeight(currentRect.height() + 100);

    this->scene()->setSceneRect(currentRect);
    update();
    return;
  } else {
    QRectF currentRect = this->scene()->itemsBoundingRect();
    currentRect.setX(currentRect.x() - 50);
    currentRect.setY(currentRect.y() - 50);
    currentRect.setWidth(currentRect.width() + 100);
    currentRect.setHeight(currentRect.height() + 100);
    this->scene()->setSceneRect(currentRect);
    QGraphicsView::mouseMoveEvent(event);
  }
}

void GraphicsView::wheelEvent(QWheelEvent* event) {
  if (event->modifiers() & Qt::ControlModifier) {
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;
    if (event->delta() > 0) {
      this->scale(scaleFactor, scaleFactor);
    } else {
      this->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
  } else if (event->modifiers() & Qt::ShiftModifier) {
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->delta());
  } else {
    QGraphicsView::wheelEvent(event);
  }
}
