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
#ifndef TDOWNLOADER_H
#define TDOWNLOADER_H

#include <QFile>
#include <QLocale>
#include <QDir>
#include <QFileInfo>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

/**
 * @brief should get the "old" maybe local md5 named file in var and the path to the new file
 *  should send an signal when a new file is downloaded
 */
class TDownloader: public QObject
{
    Q_OBJECT
    QNetworkAccessManager *manager_head, *manager_head_redirect, *manager_get;

public:
    TDownloader(QString ua, QObject * parent = 0);
    ~TDownloader();
    virtual void checkFiles(QString local, QString remote, QString src);
    virtual bool downloadInProgress();

private slots:
    void finishedGetRequest(QNetworkReply *reply);
    void finishedHeadRequest(QNetworkReply *reply);
    void finishedHeadRedirectRequest(QNetworkReply *reply);

private:
    QUrl           remote_file_url;
    QString        remote_file_path, src_file_path;
    bool           download;
    QByteArray     user_agent;
    QFileInfo      local_file_info;
    void           checkHttpHeaders(QNetworkReply *reply);
    void           checkStatusCode(QNetworkReply *reply, int status_code);
    void           doHttpGetRequest();
    void           doHttpHeadRequest();
    bool           saveToDisk(QIODevice *data);
    void           emitNoModified();
    void           emitUnCachable();
    void           emitDownloadFailed(QString error_message);
signals:
    void downloadSucceed(QString);
    void noModified(QString); // when file is in cache cancel download
    void uncachable(QString); // when file is in cache cancel download
    void downloadFailed(QString);

};
#endif // TDOWNLOADER_H
