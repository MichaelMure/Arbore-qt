#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sharemodel.h"
#include "sharedelegate.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ShareModel shareModel_;
    ShareDelegate shareDelegate_;

    void refresh() const;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *);
};

#endif // MAINWINDOW_H
