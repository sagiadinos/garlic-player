/************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/
#include "downloader.h"

Downloader::Downloader(QByteArray agent, QObject *parent) : TNetworkAccess(agent, parent)
{
    manager_head.reset(new QNetworkAccessManager(this));
    connect(manager_head.data(), SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
}

Downloader::~Downloader()
{
    if (!MyFileDownloader.isNull())
         MyFileDownloader.data()->cancelDownload();
}

void Downloader::processFile(QUrl url, QFileInfo fi)
{
    setRemoteFileUrl(url);
    setLocalFileInfo(fi);
    manager_head.data()->head(prepareNetworkRequest(remote_file_url));
    return;
}

QFileInfo Downloader::getLocalFileInfo()
{
    return local_file_info;
}

void Downloader::finishedHeadRequest(QNetworkReply *reply)
{
    qDebug(Develop) << "finished HEAD Request for " << remote_file_url.toString();
    if (reply->error() != QNetworkReply::NoError)
    {
        handleNetworkError(reply);
        reply->deleteLater();
        return;
    }

    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status_code != 301)
        checkStatusCode(reply, status_code);
    else
    {
        // change remote_file_url with new redirect address
        QUrl remote_file_url_301(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());
        QNetworkRequest request = prepareNetworkRequest(remote_file_url_301);

        manager_head_redirect.reset(new QNetworkAccessManager(this));
        connect(manager_head_redirect.data(), SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRedirectRequest(QNetworkReply*)));
        manager_head_redirect.data()->head(request);
    }
    reply->deleteLater();
    return;
}

void Downloader::finishedHeadRedirectRequest(QNetworkReply *reply)
{
    qDebug(Develop) << "finished Redirect Request for " << remote_file_url.toString();
    if (reply->error() != QNetworkReply::NoError)
    {
        handleNetworkError(reply);
        return;
    }
    checkStatusCode(reply, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
    reply->deleteLater();
    return;
}

void Downloader::checkStatusCode(QNetworkReply *reply, int status_code)
{
    switch (status_code)
    {
        case 200:
                checkHttpHeaders(reply);
                break;
        case 304:
        default:
            qDebug(Develop) << "status code: " << status_code << "not handled for " << remote_file_url.toString();
            emit notmodified(this);
        break;
    }
    return;
}

void Downloader::checkHttpHeaders(QNetworkReply *reply)
{
    QString content_type = "";
    if (reply->hasRawHeader("Content-Type"))
        content_type = reply->rawHeader("Content-Type");

    if (content_type.contains("text/html")  || content_type.contains("application/xhtml+xml"))
    {
        qDebug(Develop) << remote_file_url.toString() << " no need for caching " << remote_file_url.toString();
        emit notcacheable(this);
        reply->deleteLater();
        return;
    }

    if (!validContentType(content_type))
    {
        qWarning(Develop)<< remote_file_url.toString()  << " has contentype " << content_type << " which is not supported.";
        reply->deleteLater();
        emit failed(this);
        return;
    }

    if (!reply->hasRawHeader("Last-Modified"))
    {
        handleNetworkError(reply);
        emit failed(this);
        return;
    }

    qint64 remote_size = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
    // we need to check for size and last Modified, cause a previous index smil on the server can have a older Date and would not be loaded
    if (local_file_info.exists() &&
        local_file_info.size() ==  remote_size &&
        local_file_info.lastModified().toUTC() > reply->header(QNetworkRequest::LastModifiedHeader).toDateTime())
    {
        qDebug(Develop) << remote_file_url.toString() << " no need for update";
        emit notmodified(this);
        reply->deleteLater();
        return;
    }

    DiscSpace MyDiscSpace(local_file_info.absolutePath());
    qint64 calc = MyDiscSpace.calculateNeededDiscSpaceToFree(remote_size);
    if (calc > 0 && !MyDiscSpace.freeDiscSpace(calc))
    {
        handleNetworkError(reply);
        emit failed(this);
        return;
    }


    startDownload();

    return;
}

void Downloader::startDownload()
{
    manager_get.reset(new QNetworkAccessManager(this));
    MyFileDownloader.reset(new FileDownloader(manager_get.data(), getUserAgent(), this));
    connect(MyFileDownloader.data(), SIGNAL(downloadSuccessful()), SLOT(doDownloadSuccessFul()));
    connect(MyFileDownloader.data(), SIGNAL(downloadError(QNetworkReply *)), SLOT(doDownloadError(QNetworkReply *)));
    MyFileDownloader->startDownload(remote_file_url, local_file_info.absoluteFilePath());
}

void Downloader::doDownloadSuccessFul()
{
    QStringList list;
    list  << "resourceURI" << remote_file_url.toString()
          << "contentLength" << QString::number(local_file_info.size())
          << "lastModifiedTime" << local_file_info.lastModified().toString(Qt::ISODate);

    qInfo(ContentManager) << Logger::getInstance().createEventLogMetaData("OBJECT_UPDATED", list);
    emit succeed(this);
}

void Downloader::doDownloadError(QNetworkReply *reply)
{
    handleNetworkError(reply);
}

bool Downloader::validContentType(QString content_type)
{
    if (!content_type.contains("image/") && !content_type.contains("video/") && !content_type.contains("audio/") && !content_type.contains("application/smil") && !content_type.contains("application/widget"))
    {
        return false;
    }
    return true;
}

void Downloader::handleNetworkError(QNetworkReply *reply)
{
    QStringList list;
    if (local_file_info.exists())
    {
        list << "resourceURI" << remote_file_url.toString()
             << "errorMessage" << reply->errorString()
             << "transferLength" << QString::number(determineBytesTransfered())
             << "lastCachedLength" << QString::number(local_file_info.size())
             << "lastCachedModifiedTime" << local_file_info.lastModified().toString(Qt::ISODate);
        qWarning(ContentManager) << Logger::getInstance().createEventLogMetaData("FETCH_FAILED",list);
    }
    else
    {
        list << "resourceURI" << remote_file_url.toString()
             << "errorMessage" << reply->errorString()
             << "transferLength" << QString::number(determineBytesTransfered());
        qCritical(ContentManager) << Logger::getInstance().createEventLogMetaData("FETCH_FAILED",list);
    }

    qDebug(Develop) << " Download failed " << remote_file_url.toString();
    reply->deleteLater();
    emit failed(this);
}

quint64 Downloader::determineBytesTransfered()
{
    quint64 bytes_transfered = 0;
    if (!MyFileDownloader.isNull()) // maybe we get here almost after HEAD and there exists no download attempt
        bytes_transfered = MyFileDownloader.data()->getBytesTransfered();

    return bytes_transfered;
}
