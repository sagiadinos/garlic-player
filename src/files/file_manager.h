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
#ifndef TFILEMANAGER_H
#define TFILEMANAGER_H

#include <QObject>
#include <QQueue>
#include <QHash>
#include "downloader.h"
#include "tools/configuration.h"

class TFileManager : public QObject
{
    Q_OBJECT
public:
    const     int        _no_exist   = 0;
    const     int        _outdated   = 1;
    const     int        _exist      = 2;
    const     int        _reloadable = 3;
    const     int        _uncachable = 4;
    TFileManager(TConfiguration *config, QObject *parent = 0);
    ~TFileManager();
    void                       clearQueues();
    void                       registerFile(QString remote_file);
    QString                    getLoadablePath(QString remote_file);
    int                        checkCacheStatus(QString remote_file);
protected:
     QHash<QString, int>       loaded_list;
     QQueue<QString>           download_queue;
     TConfiguration           *MyConfiguration;
     TDownloader              *MyDownloader;
     QString                   index_path;
     void                      proceedDownloadQueue();
     QString                   getHashedFilePath(QString remote_path);
     bool                      isFileInList(QString file_path);
     void                      insertForDownloadQueue(QString remote_file);
protected slots:
    void     doFinishDownload(QString remote_file);
    void     doCancelDownload(QString remote_file);
    void     doFailDownload(QString remote_file);
    void     doUncachable(QString remote_file);
signals:

};

#endif // TFILEMANAGER_H
