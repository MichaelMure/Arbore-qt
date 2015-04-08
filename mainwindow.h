#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "downloadmodel.h"
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
    DownloadModel downloadModel_;

private slots:
    void refresh();
};

#endif // MAINWINDOW_H
