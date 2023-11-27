#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "labdownloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();

private:
    Ui::MainWindow *ui;
    LabDownloader downloader;
};

#endif // MAINWINDOW_H
