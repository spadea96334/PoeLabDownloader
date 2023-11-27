#include "mainwindow.h"

#include <QApplication>

#include "labdownloader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    LabDownloader *loader = new LabDownloader();
    loader->downloadFile(LabDownloader::UBER);

    return a.exec();
}
