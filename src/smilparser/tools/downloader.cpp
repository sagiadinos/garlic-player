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

TDownloader::TDownloader(QObject * parent)
{
    Q_UNUSED(parent);
    manager_get  = new QNetworkAccessManager();
    manager_head = new QNetworkAccessManager();
    connect(manager_get, SIGNAL(finished(QNetworkReply*)), SLOT(finishedGetRequest(QNetworkReply*)));
    connect(manager_head, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
    user_agent = QByteArray("");
    smil_index = false;
}

void TDownloader::setUserAgent(QString ua)
{
    user_agent = QByteArray(ua.toUtf8());
    return;
}

void TDownloader::checkFiles(QString local, QString remote)
{
    if (user_agent != "")
    {
        remote_file   = QUrl(remote);
        local_file    = QFileInfo(local);
        doHttpHeadRequest();
    }
    else
        qDebug() << "User agent is empty\r";

    return;
}


void TDownloader::doHttpGetRequest()
{
    QNetworkRequest request(remote_file);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    manager_get->get(request);
    return;
}

void TDownloader::doHttpHeadRequest()
{
    QNetworkRequest request(remote_file);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    manager_head->head(request);
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


void TDownloader::finishedHeadRequest(QNetworkReply *reply)
{
    if (reply->hasRawHeader("Last-Modified"))
    {
        QLocale   locale(QLocale::English, QLocale::UnitedStates);
        QDateTime last_modified_remote = locale.toDateTime(reply->rawHeader("Last-Modified"), "ddd, dd MMM yyyy hh:mm:ss 'GMT'");
        QDateTime last_modified_local  = local_file.lastModified();
        long      length_remote        = reply->rawHeader("Content-Length").toLong();
        long      length_local         = local_file.size();

        if (local_file.exists() && length_remote == length_local && last_modified_local > last_modified_remote)
            emit downloadCanceled();
        else
            doHttpGetRequest();
    }
    else
        emit downloadCanceled();

    reply->deleteLater();
    return;
}

void TDownloader::finishedGetRequest(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error())
        qDebug() << "Download of " << url.toString() << " failed: " << reply->errorString() << "\r";
    else
    {
        saveToDisk(reply);
    }
    reply->deleteLater();
    emit downloadSucceed();
    return;

}

QString TDownloader::getFileNameFromUrl()
{
    QString path = remote_file.path();
    return QFileInfo(path).fileName();
}

