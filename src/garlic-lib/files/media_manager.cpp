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
    if (isRemote(src))
        return MyMediaModel->findStatusBySrcPath(src);

    if (isRelative(src)) // when media is relative we need the Indexpath set in front
        src = MyConfiguration->getIndexPath() + src;

    return MyMediaModel->findStatusBySrcPath(src);
}


QString MediaManager::requestLoadablePath(QString src)
{
    if (isRemote(src))
        return MyMediaModel->findLocalBySrcPath(src);

    if (isRelative(src)) // when media is relative we need the Indexpath set in front
        src = MyConfiguration->getIndexPath() + src;

    return MyMediaModel->findLocalBySrcPath(src);
}


// ==================  protected methods =======================================

void MediaManager::handleRemoteFile(QString src)
{
    // check if Media is already available in Cache and load to get sure Player wok even if network is broken
    QString local_path = MyConfiguration->getPaths("cache")+MyMediaModel->determineHashedFilePath(src);
    QFileInfo fi(local_path);
    if (fi.exists())
        MyMediaModel->insertAvaibleFile(src, local_path);

    MyDownloadQueue->insertQueue(src, local_path);
}

// ==================  protected slots =======================================

void MediaManager::doNotCacheable(QString src_file_path)
{
    MyMediaModel->insertAvaibleLink(src_file_path);
}

void MediaManager::doSucceed(QString src_file_path, QString local_file_path)
{
    MyMediaModel->insertAvaibleFile(src_file_path, local_file_path);
}
