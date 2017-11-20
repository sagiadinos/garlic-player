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
#ifndef DOWNLOAD_QUEUE_H
#define DOWNLOAD_QUEUE_H

#include <QObject>
#include <QQueue>
#include <QSet>
#include "downloader.h"
#include "disc_space.h"

/**
 * @brief The DownloadQueue class
 *  handles downloads
 */
class DownloadQueue : public QObject
{
    Q_OBJECT
public:
    const int _max_download_slots   = 5;
    explicit DownloadQueue(QByteArray ua, QObject *parent=Q_NULLPTR);
    ~DownloadQueue();
    void     insertQueue(QString src, QString local);
    void     clearQueues();

    //Getter/Setter
    QByteArray                          getUserAgent() const {return user_agent;}
    void                                setUserAgent(const QByteArray &value) {user_agent = value;}
    QQueue<QPair<QString, QString>>     getMediaQueue() {return media_queue;}
    QHash<QString, Downloader *>        getDownloadSlots(){return download_slots;}
protected:
    QByteArray                          user_agent;
    QQueue<QPair<QString, QString>>     media_queue;
    QHash<QString, Downloader *>        download_slots;
    void               processQueue();
    void               emitPaths();

public slots:
    void               doSucceed(TNetworkAccess *downloader);
    void               doNotModified(TNetworkAccess *downloader);
    void               doNotCacheable(TNetworkAccess *downloader);
    void               doFailed(TNetworkAccess *downloader);

signals:
    void               succeed(QString src, QString local);
    void               notmodified(QString src);
    void               notcacheable(QString src);
    void               failed(QString src);

};

#endif // DOWNLOAD_QUEUE_H
