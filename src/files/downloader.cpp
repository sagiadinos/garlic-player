/********************************************
*****************************************
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
    manager_get  = new QNetworkAccessManager();
    manager_head = new QNetworkAccessManager();
    connect(manager_get, SIGNAL(finished(QNetworkReply*)), SLOT(finishedGetRequest(QNetworkReply*)));
    connect(manager_head, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
    // ToDo: Support Redirects
    // connect(manager_head, SIGNAL(redirected(QNetworkReply*)), SLOT(doRedirect(QNetworkReply*)));
    user_agent = QByteArray(ua.toUtf8());
    download = false;
}

TDownloader::~TDownloader()
{
    delete manager_get;
    delete manager_head;
}


bool TDownloader::downloadInProgress()
{
    return download;
}

void TDownloader::checkFiles(QString local, QString remote)
{
    if (local != "" && remote != "")
    {
        remote_file   = QUrl(remote);
        local_file    = QFileInfo(local);
        doHttpHeadRequest();
    }
    return;
}


void TDownloader::doHttpHeadRequest()
{
    QNetworkRequest request(remote_file);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    manager_head->head(request);
    download = true;
    return;
}

void TDownloader::finishedHeadRequest(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << "Download of " << reply->url().toString() << " failed: " << reply->errorString() << "\r";
        download = false;
        emit downloadFailed(remote_file.toString());
    }
    else
    {
        int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status_code == 200)
        {
            if (reply->hasRawHeader("Last-Modified"))
            {
                QLocale   locale(QLocale::English, QLocale::UnitedStates);
                if (local_file.exists() &&
                        local_file.size() == reply->rawHeader("Content-Length").toLong() &&
                        local_file.lastModified() > locale.toDateTime(reply->rawHeader("Last-Modified"), "ddd, dd MMM yyyy hh:mm:ss 'GMT'"))
                {
                    download = false;
                    emit downloadCanceled(remote_file.toString());
                }
                else
                    doHttpGetRequest();
            }
        }
        else // eg 304
        {
            download = false;
            emit downloadCanceled(remote_file.toString());
        }
    }
    return;
}

void TDownloader::doHttpGetRequest()
{
    QNetworkRequest request(remote_file);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    manager_get->get(request);
    return;
}

void TDownloader::finishedGetRequest(QNetworkReply *reply)
{
    download = false;
    if (reply->error())
    {
        qDebug() << "Download of " << reply->url().toString() << " failed: " << reply->errorString() << "\r";
        emit downloadFailed(remote_file.toString());
    }
    else
    {
        saveToDisk(reply);
        emit downloadSucceed(remote_file.toString());
    }
    return;

}

bool TDownloader::saveToDisk(QIODevice *data)
{
    QFile file(local_file.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << local_file.absoluteFilePath() << " for writing: " << file.errorString() << "\r";
        return false;
    }
    file.write(data->readAll());
    file.close();
    return true;
}



QString TDownloader::getFileNameFromUrl()
{
    QString path = remote_file.path();
    return QFileInfo(path).fileName();
}

