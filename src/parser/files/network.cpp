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

Network::Network(QByteArray agent)
{
    setUserAgent(agent);
    manager_get           = new QNetworkAccessManager(this);
    manager_head          = new QNetworkAccessManager(this);
    manager_head_redirect = new QNetworkAccessManager(this);
    connect(manager_get, SIGNAL(finished(QNetworkReply*)), SLOT(finishedGetRequest(QNetworkReply*)));
    connect(manager_head, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRequest(QNetworkReply*)));
    connect(manager_head_redirect, SIGNAL(finished(QNetworkReply*)), SLOT(finishedHeadRedirectRequest(QNetworkReply*)));
}

void Network::processFile(QUrl url, QFileInfo fi)
{
    setRemoteFileUrl(url);
    setLocalFileInfo(fi);
    doHttpHeadRequest();
    return;
}

void Network::doHttpHeadRequest()
{
    manager_head->head(prepareNetworkRequest(remote_file_url));
    return;
}

void Network::finishedHeadRequest(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        handleNetworkError(reply);
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
        manager_head_redirect->head(request);
    }
    return;
}

void Network::finishedHeadRedirectRequest(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        handleNetworkError(reply);
        return;
    }
    checkStatusCode(reply, reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
    return;
}

QNetworkRequest Network::prepareNetworkRequest(QUrl remote_url)
{
    QNetworkRequest request(remote_url);
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    request.setRawHeader(QByteArray("User-Agent"), user_agent);
    return request;
}

void Network::checkStatusCode(QNetworkReply *reply, int status_code)
{
    switch (status_code)
    {
        case 200:
                checkHttpHeaders(reply);
                break;
        case 304:
        default:
            qInfo(ContentManager) << " OBJECT_IS_ACTUAL resourceURI:" << remote_file_url.toString()  << " no need to refresh";
            emit notmodified(this);
        break;
    }
    return;
}

void Network::checkHttpHeaders(QNetworkReply *reply)
{
    QString content_type = "";
    if (reply->hasRawHeader("Content-Type"))
        content_type = reply->rawHeader("Content-Type");

    if (content_type.contains("text/html")  || content_type.contains("application/xhtml+xml"))
    {
        emit notcacheable(this);
        return;
    }

    if (!validContentType(content_type))
    {
        emit failed(this);
        return;
    }

    if (reply->hasRawHeader("Last-Modified"))
    {
        // we need to check for size and last Modified, cause a previous index smil on the server can have a older Date and would not be loaded
        if (local_file_info.exists() &&
            local_file_info.size() ==  reply->header(QNetworkRequest::ContentLengthHeader).toInt() &&
            local_file_info.lastModified().toUTC() > reply->header(QNetworkRequest::LastModifiedHeader).toDateTime())
        {
            qInfo(ContentManager) << " OBJECT_IS_ACTUAL resourceURI:" << remote_file_url.toString() << " no need for update";
            emit notmodified(this);
        }
        else
            doHttpGetRequest();
    }
    else
    {
        qWarning(ContentManager) << " FETCH_FAILED resourceURI:" << remote_file_url.toString() << " has no Last-Modified entry in header";
        emit failed(this);
    }
    return;
}

void Network::doHttpGetRequest()
{
    // cause it can be a redirect to a new url
    QNetworkRequest request = prepareNetworkRequest(remote_file_url);
    manager_get->get(request);
    return;
}

void Network::finishedGetRequest(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        handleNetworkError(reply);
        return;
    }

    if (!saveToDisc(reply)) // reply is QIODevice
         emit failed(this);

    emit succeed(this);
    return;
}

bool Network::saveToDisc(QIODevice *data)
{
    QFile file(local_file_info.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) // Delete/Truncate previous content
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << remote_file_url.toString() << " could not be saved " << file.errorString();
        return false;
    }
    DiscSpace MyDiscSpace(local_file_info.absolutePath());
    qint64 calc = MyDiscSpace.calculateNeededDiscSpaceToFree(data->size());
    if (calc > 0 && !MyDiscSpace.freeDiscSpace(calc))
    {
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << remote_file_url.toString()  << " could not be saved " << file.errorString() << "cause space cannot be freed";
        return false;
    }

    file.write(data->readAll()); // should overwrites automatic existing files
    file.close();
    qInfo(ContentManager) << "OBJECT_UPDATED resourceURI:" << remote_file_url.toString() << " contentLenght: " << local_file_info.size();
    return true;
}


bool Network::validContentType(QString content_type)
{
    // text/texmacs is "sophisticated" support for ts-files in old debian (squeeze) distros
    if (!content_type.contains("image/") && !content_type.contains("video/") && !content_type.contains("audio/") && !content_type.contains("text/texmacs") && !content_type.contains("application/smil") && !content_type.contains("application/widget"))
    {
        qCritical() << "FETCH_FAILED resourceURI: " << remote_file_url.toString()  << " has contentype " << content_type << " which is not supported.";
        return false;
    }
    return true;
}

void Network::handleNetworkError(QNetworkReply *reply)
{
    if (local_file_info.exists())
        qWarning(ContentManager) << "FETCH_FAILED resourceURI: " << remote_file_url.toString()  << " " << reply->errorString();
    else
        qCritical(ContentManager) << "FETCH_FAILED resourceURI: " << remote_file_url.toString()  << " " << reply->errorString();
    emit failed(this);
}
