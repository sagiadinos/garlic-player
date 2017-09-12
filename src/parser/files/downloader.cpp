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

TDownloader::TDownloader(TConfiguration *config)
{
    MyConfiguration = config;
    manager_get           = new QNetworkAccessManager(this);
    manager_head          = new QNetworkAccessManager(this);
    manager_head_redirect = new QNetworkAccessManager(this);
    connect(manager_get, SIGNAL(finished(QNetworkReply*)), SLOT(finishedGetRequest(QNetworkReply*)));
    connect(manager_head, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
    connect(manager_head_redirect, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRedirectRequest(QNetworkReply*)));
    // ToDo: Support media Redirects
    // connect(manager_head, SIGNAL(redirected(QNetworkReply*)), SLOT(doRedirect(QNetworkReply*)));
    user_agent = QByteArray(MyConfiguration->getUserAgent().toUtf8());
    download = false;
}

TDownloader::~TDownloader()
{
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
    QNetworkRequest request = prepareNetworkRequest(remote_file_url);
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
            QNetworkRequest request = prepareNetworkRequest(remote_file_url);
            manager_head_redirect->head(request);
        }
    }
    else
        emitDownloadFailed("FETCH_FAILED resourceURI: " + src_file_path +" "+ reply->errorString());
    return;
}


void TDownloader::finishedHeadRedirectRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
        checkStatusCode(reply, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
    else
        emitDownloadFailed("FETCH_FAILED resourceURI: " + src_file_path +" "+ reply->errorString());
    return;
}

QNetworkRequest TDownloader::prepareNetworkRequest(QUrl remote_url)
{
    QNetworkRequest request(remote_url);
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    return request;
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
    if (content_type.contains("image/") || content_type.contains("video/") || content_type.contains("audio/") || content_type.contains("text/texmacs") || content_type.contains("application/smil") || content_type.contains("application/widget"))
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
        {
            emitDownloadFailed("FETCH_FAILED resourceURI:" + src_file_path + " no supported content type: "+ content_type);
        }
    }
    else
    {
        if (content_type.contains("text/html") || content_type.contains("application/xhtml+xml"))
            emitUnCachable();
        else
            emitDownloadFailed("FETCH_FAILED resourceURI:" + src_file_path + " no supported content type: " + content_type);
    }
    return;
}

void TDownloader::doHttpGetRequest()
{
    // cause it can be a redirect to a new url
    QNetworkRequest request = prepareNetworkRequest(remote_file_url);
    manager_get->get(request);
    return;
}

void TDownloader::finishedGetRequest(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
        saveToDisk(reply);
    else
        emitDownloadFailed("FETCH_FAILED resourceURI:" + src_file_path + " " + reply->errorString());
    return;

}

void TDownloader::saveToDisk(QIODevice *data)
{
    download = false; // must be first cause sometimes file_manager run into race condition during save
    QFile file(local_file_info.absoluteFilePath());
    if (!file.open(QIODevice::ReadWrite))
    {
        emitDownloadFailed("FETCH_FAILED resourceURI:" + src_file_path + " could not be saved " + file.errorString());
        return;
    }

    handleFreeDiskSpace(data);

    file.write(data->readAll());
    file.close();

    if (local_file_info.suffix() == "wgt")
    {
        if (!extractWgt())
        {
            emitDownloadFailed("FETCH_FAILED resourceURI:" + src_file_path + " Widget can not be extracted");
            return;
        }
    }

    qInfo(ContentManager) << "OBJECT_UPDATED resourceURI:" << src_file_path << " was modified and written locally";
    emit downloadSucceed(src_file_path);
    return;
}

void TDownloader::handleFreeDiskSpace(QIODevice *data)
{
    QStorageInfo storage(MyConfiguration->getPaths("cache"));
    if (data->size() > storage.bytesAvailable())
    {
        QDir dir(MyConfiguration->getPaths("cache"));
        QFileInfoList dirList = dir.entryInfoList(QDir::NoDotAndDotDot, QDir::Time | QDir::Reversed);
        for(int i = 0; i < dirList.size(); i++)
        {
            if (dirList.at(i).exists()) // cause it could be an already deleted wgt file
            {
                if (dirList.at(i).isFile())
                {
                    QFile del_file(dirList.at(i).absoluteFilePath());
                    del_file.remove();
                }
                else // a dir dir has a corresponding wgt file
                {
                    QDir del_dir((dirList.at(i).absoluteFilePath()));
                    del_dir.removeRecursively();
                    // delete corresponding wgt
                    QFile del_file(dirList.at(i).absoluteFilePath()+".wgt");
                    del_file.remove();
                }
            }
            if (data->size() < storage.bytesAvailable())
                break;
        }
    }
}

bool TDownloader::extractWgt()
{
    QuaZip zip(local_file_info.absoluteFilePath());
    bool   ret = zip.open(QuaZip::mdUnzip);
    if (ret)
    {
        QString folder_path = local_file_info.absolutePath()+"/"+local_file_info.baseName();
        QDir dir(folder_path);
        if (dir.exists())
           dir.removeRecursively();
        dir.mkdir(folder_path);
        JlCompress::extractDir(local_file_info.absoluteFilePath(), folder_path+"/");
        zip.close();
    }
    return ret;
}

void TDownloader::emitNoModified()
{
    download = false;
    qDebug(ContentManager) << " OBJECT_IS_ACTUAL resourceURI:" << src_file_path << " no need to refresh";
    emit noModified(src_file_path);
    return;
}

void TDownloader::emitUnCachable()
{
    download = false;
    qDebug(ContentManager) << "resourceURI:" << src_file_path << " should not be cached";
    emit uncachable(src_file_path);
}

void TDownloader::emitDownloadFailed(QString Msg)
{
    download = false;
    QFile file(local_file_info.absoluteFilePath());
    if (file.exists())
        qWarning(ContentManager) << Msg;
    else
        qCritical(ContentManager) << Msg;
    emit downloadFailed(src_file_path);
    return;
}
