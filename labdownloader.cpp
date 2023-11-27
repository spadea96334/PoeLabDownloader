#include "labdownloader.h"

#include <QFile>
#include <QNetworkReply>
#include <QRegularExpression>

LabDownloader::LabDownloader(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();
}

void LabDownloader::downloadFile(LabType type)
{

    QNetworkRequest request(QUrl("https://www.poelab.com/"));
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=] {
        if (reply->error() == QNetworkReply::NoError) {
            parseLabPageUrl(type, reply->readAll());
        }

        reply->deleteLater();
    });
}

void LabDownloader::onGetJsonFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QString fileName = reply->url().fileName();
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        emit error("Download json failed");
        return;
    }

    QByteArray bytes = reply->readAll();
    QFile file(fileName);

    if (!file.open(QIODevice::ReadWrite)) {
        emit error("Can't save json");
        return;
    }

    file.write(bytes);
    file.flush();
    file.close();
    emit downloadFinished();
}

void LabDownloader::getJsonUrlFinished()
{
    static const QRegularExpression JsonReg("href=\\\"(.*\\.json)\\\" download");

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        emit error(QString("Can't open json page: ") + reply->url().url());
        return;
    }

    QRegularExpressionMatch match = JsonReg.match(reply->readAll());
    if (match.lastCapturedIndex() != 1) {
        emit error("Can't find json url!");
        return;
    }

    QNetworkRequest request((QUrl(match.captured(1))));
    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &LabDownloader::onGetJsonFinished);
}

void LabDownloader::getJsonUrl(QUrl url)
{
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &LabDownloader::getJsonUrlFinished);
}

void LabDownloader::parseLabPageUrl(LabType type, QByteArray homePage)
{
    static const QRegularExpression UberLabReg("<a class=\\\"redLink\\\" href=\\\"(.*)\\\"><strong>UBER LAB</strong></a>");
    static const QRegularExpression MercLabReg("<a class=\\\"redLink\\\" href=\\\"(.*)\\\">MERC LAB</a>");
    static const QRegularExpression CruelLabReg("<a class=\\\"redLink\\\" href=\\\"(.*)\\\">CRUEL LAB</a>");
    static const QRegularExpression NormalLabReg("<a class=\\\"redLink\\\" href=\\\"(.*)\\\">NORMAL LAB</a>");

    QRegularExpression exp;
    switch (type) {
        case NORMAL:
            exp = NormalLabReg;
            break;
        case CRUEL:
            exp = CruelLabReg;
            break;
        case MERC:
            exp = MercLabReg;
            break;
        case UBER:
            exp = UberLabReg;
            break;
    }

    QRegularExpressionMatch match = exp.match(QString(homePage));

    if (match.lastCapturedIndex() != 1) {
        emit error("Can't find lab url!");
        qWarning() << "Website format changed";
        return;
    }

    getJsonUrl(QUrl(match.captured(1)));
}

