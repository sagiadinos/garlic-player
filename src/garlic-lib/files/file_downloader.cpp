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

void FileDownloader::startDownload(QUrl url, QString file_name)
{
    if(network_reply)
        return;
    original_file_name = file_name;
    destination_file.setFileName(file_name+".tmp"); // add suffix to prevent overwriting

    if(!destination_file.open(QIODevice::WriteOnly))
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
    destination_file.write(data);
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
        destination_file.write(network_reply->readAll());
        destination_file.close();
        destination_file.rename(original_file_name+".tmp", original_file_name); // remove tmp;
        network_reply->deleteLater();

        emit downloadSuccessful();
    }
    emit backReady();
}

void FileDownloader::cleanupDownload()
{
    network_reply->abort();
    network_reply->deleteLater();
    destination_file.close();
    destination_file.remove();
}
