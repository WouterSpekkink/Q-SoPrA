#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QProgressBar>
#include <QtWidgets/QDialog>

class ProgressBar : public QDialog
{

    Q_OBJECT

public:
    ProgressBar(QWidget *parent=0, int min=0, int max=0);
    void setProgress(int value);

    ~ProgressBar() {}

private:
    QProgressBar *progress;
    int minimum;
    int maximum;

};

#endif
