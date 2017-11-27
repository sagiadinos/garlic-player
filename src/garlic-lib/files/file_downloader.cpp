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
    setRemoteFileUrl(url);
    if(!destination_file.open(QIODevice::WriteOnly))
        return;

    emit goingBusy();

    QNetworkRequest request = prepareNetworkRequest(url);
    network_reply = network_manager->get(request);
    connect(network_reply, &QIODevice::readyRead, this, &FileDownloader::readData);
    connect(network_reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    connect(network_reply, &QNetworkReply::finished, this, &FileDownloader::finishDownload);
    qDebug(Develop) << "started download"  << remote_file_url.toString();
}

void FileDownloader::cancelDownload()
{
    if(!network_reply)
        return;
    cleanupDownload();
    qDebug(Develop) << "manually cancelled download"  << remote_file_url.toString();
    emit backReady();
}

quint64 FileDownloader::getBytesTransfered()
{
    return bytes_transfered;
}

void FileDownloader::addBytesTransfered(quint64 add_bytes )
{
    bytes_transfered = bytes_transfered + add_bytes;
}

void FileDownloader::renameAfterDownload()
{
    // Because Qt really likes to makes my life hard and QFile::rename cannot overwrite
    QFile original_file(original_file_name);
    if (original_file.exists())
        original_file.remove();
    destination_file.rename(original_file_name);
}

void FileDownloader::cleanupDownload()
{
    network_reply->abort();
    network_reply->deleteLater();
    destination_file.close();
    destination_file.remove();
}


void FileDownloader::readData()
{
    addBytesTransfered(destination_file.write(network_reply->readAll()));
}

void FileDownloader::finishDownload()
{
    if(network_reply->error() != QNetworkReply::NoError)
    {
        cleanupDownload();
        emit downloadError(network_reply);
    }
    else
    {
        readData();
        destination_file.close();

        renameAfterDownload();
        network_reply->deleteLater();

        emit downloadSuccessful();
    }
    qDebug(Develop) << "finished download"  << remote_file_url.toString();
    emit backReady();
}

