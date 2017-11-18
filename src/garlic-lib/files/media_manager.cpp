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
#include "media_manager.h"
#include "media_manager.h"

MediaManager::MediaManager(MediaModel *mm, DownloadQueue *dq, TConfiguration *config, QObject *parent) : BaseManager(parent)
{
    MyConfiguration  = config;
    MyDownloadQueue  = dq;
    MyMediaModel     = mm;
    connect(MyDownloadQueue, SIGNAL(succeed(QString, QString)), SLOT(doSucceed(QString, QString)));
    connect(MyDownloadQueue, SIGNAL(notcacheable(QString)), SLOT(doNotCacheable(QString)));
}

MediaManager::~MediaManager()
{
}

void MediaManager::clearQueues()
{
    delete MyDownloadQueue;
    delete MyMediaModel;
    return;
}

void MediaManager::registerFile(QString src)
{
    if (isRemote(src))
    {
        handleRemoteFile(src);
        return;
    }

    if (isRelative(src)) // when media is relative we need the Indexpath set in front
    {
        src = MyConfiguration->getIndexPath() + src;
        if (isRemote(src)) // check if new media path could be on remote server
        {
            handleRemoteFile(src);
            return;
        }
    }

    MyMediaModel->insertAvaibleFile(src, src); // src and local path are identically if src path is local
}

int MediaManager::checkCacheStatus(QString src)
{
    if (!isRemote(src) && isRelative(src)) // when media is relative we need the Indexpath set in front
        src = MyConfiguration->getIndexPath() + src;

    return MyMediaModel->findStatusBySrcPath(src);
}


QString MediaManager::requestLoadablePath(QString src)
{
    if (!isRemote(src) && isRelative(src)) // when media is relative we need the Indexpath set in front
        src = MyConfiguration->getIndexPath() + src;

    QString ret = MyMediaModel->findLocalBySrcPath(src);
    removeReadyPrefix(ret);

    return ret;
}


// ==================  protected methods =======================================

void MediaManager::handleRemoteFile(QString src)
{
    QString hashed_file_name = MyMediaModel->determineHashedFilePath(src);

    QString local_path = MyConfiguration->getPaths("cache") + hashed_file_name;
    QFileInfo fi(local_path);
    if (fi.exists()) // use cached, cause network could be unreachable
        MyMediaModel->insertAvaibleFile(src, local_path);

    // "ready_" as prefix prevents overwriting of files which could be accessed in same time
    MyDownloadQueue->insertQueue(src, MyConfiguration->getPaths("cache") + "ready_"+hashed_file_name);
}

void MediaManager::removeReadyPrefix(QString file_path)
{
    QFileInfo fi(file_path);
    QFile     file_ready(fi.dir().absolutePath()+ "/ready_" + fi.fileName());

    if (file_ready.exists())
        file_ready.rename(file_path);
}

// ==================  protected slots =======================================

void MediaManager::doNotCacheable(QString src_file_path)
{
    MyMediaModel->insertAvaibleLink(src_file_path);
}

void MediaManager::doSucceed(QString src_file_path, QString local_file_path)
{
    QFileInfo fi(local_file_path);
    // remove ready_prefix
    QString real_local_file_path = fi.dir().absolutePath()+ "/" + fi.fileName().mid(6); // ready_   has 6 characters;

    MyMediaModel->insertAvaibleFile(src_file_path, real_local_file_path);
}
