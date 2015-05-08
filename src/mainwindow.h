#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sharemodel.h"
#include "sharedelegate.h"
#include <QMainWindow>
#include <QTimer>

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
    QTimer refreshTimer_;
    ShareModel shareModel_;
    ShareDelegate shareDelegate_;

private slots:
    void refresh();
};

#endif // MAINWINDOW_H
