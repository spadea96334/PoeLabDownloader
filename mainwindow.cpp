#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonClicked()
{
    QObject *object = sender();
    QVariant var = object->property("labIndex");
    if (!var.isValid()) {
        return;
    }

    bool ok;
    int index = var.toInt(&ok);
    if (!ok) {
        return;
    }

    LabDownloader::LabType type = static_cast<LabDownloader::LabType>(index);
    downloader.downloadFile(type);
}
