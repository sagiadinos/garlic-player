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
#include "network.h"

TNetwork::TNetwork(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent);
    manager_get           = new QNetworkAccessManager(this);
    manager_head          = new QNetworkAccessManager(this);
    manager_head_redirect = new QNetworkAccessManager(this);
    connect(manager_get, SIGNAL(finished(QNetworkReply*)), SLOT(finishedGetRequest(QNetworkReply*)));
    connect(manager_head, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
    connect(manager_head_redirect, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRedirectRequest(QNetworkReply*)));
}

void TNetwork::checkFiles(QByteArray agent, QUrl url, QFileInfo fi)
{
    setUserAgent(agent);
    setRemoteFileUrl(url);
    setLocalFileInfo(fi);
    doHttpHeadRequest();
    return;
}

void TNetwork::doHttpHeadRequest()
{
    QNetworkRequest request = prepareNetworkRequest(remote_file_url);
    manager_head->head(request);
    // download = true;
    return;
}

void TNetwork::finishedHeadRequest(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << remote_file_url << " " << reply->errorString();
        emit downloadInpossible();
        return;
    }
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
    return;
}

void TNetwork::finishedHeadRedirectRequest(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << remote_file_url << " " << reply->errorString();

    checkStatusCode(reply, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
}

QNetworkRequest TNetwork::prepareNetworkRequest(QUrl remote_url)
{
    QNetworkRequest request(remote_url);
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    return request;
}

void TNetwork::checkStatusCode(QNetworkReply *reply, int status_code)
{
    switch (status_code)
    {
        case 200:
                checkHttpHeaders(reply);
                break;
        case 304:
        default:
            qInfo(ContentManager) << " OBJECT_IS_ACTUAL resourceURI:" << remote_file_url << " no need to refresh";
            emit downloadInpossible();
        break;
    }
    return;
}

void TNetwork::checkHttpHeaders(QNetworkReply *reply)
{
    QString content_type = "";
    if (reply->hasRawHeader("Content-Type"))
        content_type = reply->rawHeader("Content-Type");

    if (!validContentType(content_type))
    {
        emit downloadInpossible();
        return;
    }

    if (reply->hasRawHeader("Last-Modified"))
    {
        QLocale   locale(QLocale::English, QLocale::UnitedStates);
        if (local_file_info.exists() &&
                local_file_info.size() == reply->rawHeader("Content-Length").toLong() &&
                local_file_info.lastModified() > locale.toDateTime(reply->rawHeader("Last-Modified"), "ddd, dd MMM yyyy hh:mm:ss 'GMT'"))
        {
            qInfo(ContentManager) << " OBJECT_IS_ACTUAL resourceURI:" << remote_file_url << " no need to refresh";
            emit downloadInpossible();
        }
        else
            doHttpGetRequest();
    }
    else
    {
        qWarning(ContentManager) << " FETCH_FAILED resourceURI:" << remote_file_url << " has no Last-Modiefied entry in header";
        emit downloadInpossible();
    }
    return;
}

void TNetwork::doHttpGetRequest()
{
    // cause it can be a redirect to a new url
    QNetworkRequest request = prepareNetworkRequest(remote_file_url);
    manager_get->get(request);
    return;
}

void TNetwork::finishedGetRequest(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << remote_file_url << " " << reply->errorString();
        emit downloadInpossible();
        return;
    }

    emit downloadPossible(reply);
    return;
}

bool TNetwork::validContentType(QString content_type)
{
    if (content_type.contains("text/html")  || content_type.contains("application/xhtml+xml"))
        return false;

    // text/texmacs is "sophisticated" support for ts-files in old debian (squeeze) distros
    if (!content_type.contains("image/") && !content_type.contains("video/") && !content_type.contains("audio/") && !content_type.contains("text/texmacs") && !content_type.contains("application/smil") && !content_type.contains("application/widget"))
    {
        qWarning() << "FETCH_FAILED resourceURI: " << remote_file_url << " has contentype " << content_type << " which is not supported.";
        return false;
    }
    return true;
}

