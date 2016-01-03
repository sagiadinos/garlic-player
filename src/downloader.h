/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-.control.com>
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

#include <QFile>
#include <QLocale>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QUrl>

#ifndef DOWNLOADER_H
#define DOWNLOADER_H


class Downloader: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager_head, manager_get;

public:
    Downloader(QObject * parent = 0);
public slots:
    void getIndex(QString index_url);
    void getMedia(QString media_url);

private slots:
    void finishedGetRequest(QNetworkReply *reply);
    void finishedHeadRequest(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

private:
    QUrl url;
    QByteArray user_agent;
    bool smil_index;
    QString getFileNameFromUrl();
    void doHttpGetRequest();
    void doHttpHeadRequest();
    bool saveToDisk(const QString &filename, QIODevice *data);
};
#endif // DOWNLOADER_H
