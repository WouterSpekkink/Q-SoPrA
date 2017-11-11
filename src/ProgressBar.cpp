#include "../include/ProgressBar.h"
#include <QtWidgets/QHBoxLayout>

ProgressBar::ProgressBar(QWidget *parent, int min, int max)
    : QDialog(parent), minimum(min), maximum(max)
{
  progress = new QProgressBar(this);
    progress->setMinimum(minimum);
    progress->setMaximum(maximum);
    QHBoxLayout *barLayout = new QHBoxLayout;
    barLayout->addWidget(progress);
    setLayout(barLayout);
    setWindowTitle(tr("Importing data"));
    setFixedHeight(sizeHint().height());
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setFixedWidth(300);
}


void ProgressBar::setProgress(int value) 
{
    progress->setValue(value);
}
