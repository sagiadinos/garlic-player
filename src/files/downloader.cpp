/*************************************************************************************
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

TDownloader::TDownloader(QString ua, QObject * parent)
{
    Q_UNUSED(parent);
    manager_get           = new QNetworkAccessManager();
    manager_head          = new QNetworkAccessManager();
    manager_head_redirect = new QNetworkAccessManager();
    connect(manager_get, SIGNAL(finished(QNetworkReply*)), SLOT(finishedGetRequest(QNetworkReply*)));
    connect(manager_head, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
    connect(manager_head_redirect, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRedirectRequest(QNetworkReply*)));
    // ToDo: Support Redirects
    // connect(manager_head, SIGNAL(redirected(QNetworkReply*)), SLOT(doRedirect(QNetworkReply*)));
    user_agent = QByteArray(ua.toUtf8());
    download = false;
}

TDownloader::~TDownloader()
{
 //   delete manager_get;
 //   delete manager_head;
 //   delete manager_head_redirect;
}

bool TDownloader::downloadInProgress()
{
    return download;
}

void TDownloader::checkFiles(QString local, QString remote, QString src)
{
    if (local != "" && remote != "")
    {
        src_file_path      = src;
        remote_file_path   = remote;
        remote_file_url    = QUrl(remote);
        local_file_info    = QFileInfo(local);
        doHttpHeadRequest();
    }
    return;
}


void TDownloader::doHttpHeadRequest()
{
    QNetworkRequest request(remote_file_url);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    manager_head->head(request);
    download = true;
    return;
}

void TDownloader::finishedHeadRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status_code != 301)
            checkStatusCode(reply, status_code);
        else
        {
            // change remote_file_url with new redirect address
            remote_file_url = QUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());
            QNetworkRequest request(remote_file_url);
            request.setRawHeader(QByteArray("User-Agent"), user_agent);
            manager_head_redirect->head(request);
        }
    }
    else
        emitDownloadFailed(reply->errorString());
    return;
}


void TDownloader::finishedHeadRedirectRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
        checkStatusCode(reply, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
    else
        emitDownloadFailed(reply->errorString());
    return;
}

void TDownloader::checkStatusCode(QNetworkReply *reply, int status_code)
{
    switch (status_code)
    {
        case 200:
                checkHttpHeaders(reply);
                break;
        case 304:
                emitNoModified();
        default:
                emitNoModified();
    }
    return;
}

void TDownloader::checkHttpHeaders(QNetworkReply *reply)
{
    QString content_type = "";
    if (reply->hasRawHeader("Content-Type"))
        content_type = reply->rawHeader("Content-Type");

    // text/texmacs is "sophisticated" support for ts-files
    if (content_type.contains("image/") || content_type.contains("video/") || content_type.contains("audio/") || content_type.contains("text/texmacs") || content_type.contains("application/smil"))
    {
        if (reply->hasRawHeader("Last-Modified"))
        {
            QLocale   locale(QLocale::English, QLocale::UnitedStates);
            if (local_file_info.exists() &&
                    local_file_info.size() == reply->rawHeader("Content-Length").toLong() &&
                    local_file_info.lastModified() > locale.toDateTime(reply->rawHeader("Last-Modified"), "ddd, dd MMM yyyy hh:mm:ss 'GMT'"))
            {
                emitNoModified();
            }
            else
                doHttpGetRequest();
        }
        else
            emitDownloadFailed(" has no Last-Modified in http header");
    }
    else
    {
        if (content_type.contains("text/html") || content_type.contains("application/xhtml+xml"))
            emitUnCachable();
        else
            emitDownloadFailed("no supported content type: "+ content_type);

    }
    return;
}

void TDownloader::doHttpGetRequest()
{
    // cause it can be a riderect to a new url
    QNetworkRequest request(remote_file_url);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    manager_get->get(request);
    return;
}

void TDownloader::finishedGetRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
        saveToDisk(reply);
    else
        emitDownloadFailed(reply->errorString());
    return;

}

bool TDownloader::saveToDisk(QIODevice *data)
{
    download = false; // must be first cause sometimes file_manager run into race condition during save
    QFile file(local_file_info.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << local_file_info.absoluteFilePath() << " for writing: " << file.errorString() << "\r";
        return false;
    }
    file.write(data->readAll());
    file.close();

    qDebug() << QTime::currentTime().toString() << src_file_path << " was modified and written locally";
    emit downloadSucceed(src_file_path);
    return true;
}

void TDownloader::emitNoModified()
{
    download = false;
    emit noModified(src_file_path);
    qDebug() << QTime::currentTime().toString() << src_file_path << " was not modified";
    return;
}

void TDownloader::emitUnCachable()
{
    download = false;
    emit uncachable(src_file_path);
}

void TDownloader::emitDownloadFailed(QString error_message)
{
    download = false;
    qDebug() << QTime::currentTime().toString() << src_file_path << " download failed " << error_message;
    emit downloadFailed(src_file_path);
    return;
}
