#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "downloadmodel.h"
#include "downloaddelegate.h"
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
    DownloadDelegate downloadDelegate_;

private slots:
    void refresh();
};

#endif // MAINWINDOW_H
