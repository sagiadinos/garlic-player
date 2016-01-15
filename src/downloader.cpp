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

Downloader::Downloader(QObject * parent)
{
    Q_UNUSED(parent);
    connect(&manager_get, SIGNAL(finished(QNetworkReply*)), SLOT(finishedGetRequest(QNetworkReply*)));
    connect(&manager_head, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
    user_agent = QByteArray("ADAPI/1.0 (UUID:f9d65c88-e4cd-43b4-89eb-5c338e54bcae; NAME:Player with known UUID) SK8855-ADAPI/2.0.5 (MODEL:XMP-330)");
    smil_index = false;
}

void Downloader::getIndex(QString index_url)
{
    url        = QUrl(index_url);
    smil_index = true;
    doHttpHeadRequest();
    return;
}

void Downloader::getMedia(QString media_url)
{
    url        = QUrl(media_url);
    smil_index = false;
    doHttpHeadRequest();
    return;
}

void Downloader::doHttpGetRequest()
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    QNetworkReply *reply = manager_get.get(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif
}

void Downloader::doHttpHeadRequest()
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    QNetworkReply *reply = manager_head.head(request);

#ifndef QT_NO_SSL
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << filename << " for writing: " << file.errorString() << "\r";
        return false;
    }
    file.write(data->readAll());
    file.close();
    return true;
}

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_SSL
    foreach (const QSslError &error, sslErrors) fprintf(stderr, "SSL error: %s\r", qPrintable(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

void Downloader::finishedHeadRequest(QNetworkReply *reply)
{
    if (reply->hasRawHeader("Last-Modified"))
    {
        QFileInfo old_file("var/index.smil");
        QLocale   locale(QLocale::English, QLocale::UnitedStates);
        QDateTime last_modified_remote = locale.toDateTime(reply->rawHeader("Last-Modified"), "ddd, dd MMM yyyy hh:mm:ss 'GMT'");
        QDateTime last_modified_local  = old_file.lastModified();
        long      length_remote        = reply->rawHeader("Content-Length").toLong();
        long      length_local         = old_file.size();

        qDebug() << "Remote: " << last_modified_remote.toString("ddd, dd MMM yyyy hh:mm:ss") << length_remote << "Bytes \r";
        qDebug() << "Local: "  << last_modified_local.toString ("ddd, dd MMM yyyy hh:mm:ss") << length_local  << "Bytes \r";

        if (old_file.exists() && length_remote == length_local && last_modified_local > last_modified_remote)
            qDebug() << "do nothing" << "\r";
        else
        {
            qDebug() << "write new" << "\r";
            doHttpGetRequest();
        }
    }
    reply->deleteLater();
}

void Downloader::finishedGetRequest(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error())
        qDebug() << "Download of " << url.toString() << " failed: " << reply->errorString() << "\r";
    else
    {
        QString filename = "var/index.smil";
        if (!smil_index)
           filename = "var/media/"+getFileNameFromUrl();
        if (filename != "")
        {
            saveToDisk(filename, reply);
            qDebug() << filename << " was saved" << "\r";
        }
        else
            qDebug() << "Nothing saved cause filename was empty" << "\r";
    }
    reply->deleteLater();
    return;

}

QString Downloader::getFileNameFromUrl()
{
    QString path = url.path();
    return QFileInfo(path).fileName();
}

