#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    connect(&downloader, &LabDownloader::downloadFinished, this, &MainWindow::onDownloadFinished);
    connect(&downloader, &LabDownloader::error, this, &MainWindow::showMessage);
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

void MainWindow::onDownloadFinished()
{
    showMessage("Download finished!!");
}

void MainWindow::showMessage(QString message)
{
    QMessageBox *messageBox = new QMessageBox();
    messageBox->setText(message);
    messageBox->exec();
    messageBox->deleteLater();
}
