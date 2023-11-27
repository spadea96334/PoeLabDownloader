#ifndef LABDOWNLOADER_H
#define LABDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>

class LabDownloader : public QObject
{
    Q_OBJECT
public:
    enum LabType {
        NORMAL,
        CRUEL,
        MERC,
        UBER
    };Q_ENUM(LabType)

    explicit LabDownloader(QObject *parent = nullptr);
    void downloadFile(LabType type);

signals:
    void downloadFinished();
    void error(QString message);

private slots:
    void onGetJsonFinished();
    void getJsonUrlFinished();

private:
    QNetworkAccessManager *manager;

    void getJsonUrl(QUrl url);
    void parseLabPageUrl(LabType type, QByteArray homePage);
};

#endif // LABDOWNLOADER_H
