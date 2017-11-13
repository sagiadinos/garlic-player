#include "file_downloader.h"

FileDownloader::FileDownloader(QNetworkAccessManager *nam, QByteArray agent, QObject* parent) : TNetworkAccess(agent, parent), network_manager(nam)
{
}

FileDownloader::~FileDownloader()
{
    //destructor cancels the ongoing dowload (if any)
    if(network_reply)
    {
        cleanupDownload();
    }
}

void FileDownloader::startDownload(QUrl url, QString fileName)
{
    if(network_reply)
        return;

    destinationFile.setFileName(fileName);

    if(!destinationFile.open(QIODevice::WriteOnly))
        return;

    emit goingBusy();

    QNetworkRequest request = prepareNetworkRequest(url);
    network_reply = network_manager->get(request);
    connect(network_reply, &QIODevice::readyRead, this, &FileDownloader::readData);
    connect(network_reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    connect(network_reply, &QNetworkReply::finished, this, &FileDownloader::finishDownload);
}

void FileDownloader::cancelDownload()
{
    if(!network_reply)
        return;
    cleanupDownload();
    emit backReady();
}

void FileDownloader::readData()
{
    QByteArray data= network_reply->readAll();
    destinationFile.write(data);
}

void FileDownloader::finishDownload()
{
    if(network_reply->error() != QNetworkReply::NoError)
    {
        cleanupDownload();
        emit downloadError(network_reply->errorString());
    }
    else
    {
        destinationFile.write(network_reply->readAll());
        destinationFile.close();
        network_reply->deleteLater();
        emit downloadSuccessful();
    }
    emit backReady();
}

void FileDownloader::cleanupDownload()
{
    network_reply->abort();
    network_reply->deleteLater();
    destinationFile.close();
    destinationFile.remove();
}
