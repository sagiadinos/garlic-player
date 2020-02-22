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

Files::MediaManager::MediaManager(MediaModel *mm, DownloadQueue *dq, MainConfiguration *config, QObject *parent) : BaseManager(config, parent)
{
    MyDownloadQueue  = dq;
    MyMediaModel     = mm;
    connect(MyDownloadQueue, SIGNAL(succeed(QString, QString)), SLOT(doSucceed(QString, QString)));
    connect(MyDownloadQueue, SIGNAL(notcacheable(QString)), SLOT(doNotCacheable(QString)));
}

Files::MediaManager::~MediaManager()
{

}

void Files::MediaManager::clearQueues()
{
    delete MyDownloadQueue;
    delete MyMediaModel;
    return;
}

void Files::MediaManager::registerFile(QString src)
{
    if (isRelative(src)) // when media is relative we need the Indexpath set in front
    {
        src = MyConfiguration->getIndexPath() + src;
    }
    if (isRemote(src))
    {
        handleRemoteFile(src);
        return;
    }

    // if we reach here the can be a local file or a "data:" string
    // look at https://en.wikipedia.org/wiki/Data_URI_scheme
    MyMediaModel->insertAsLocalFile(src);
}

void Files::MediaManager::registerAsUncachable(QString src)
{
    MyMediaModel->insertAsUncachable(src);
}

int Files::MediaManager::checkCacheStatus(QString src)
{
    if (!isRemote(src) && isRelative(src)) // when media is relative we need the Indexpath set in front
    {
        src = MyConfiguration->getIndexPath() + src;
    }
    int status = MyMediaModel->findStatusBySrcPath(src);
    if (status == MEDIA_MODIFIED && !isCurrentlyPlaying(src))
    {
        renameDownloadedFile(requestLoadablePath(src));
        MyMediaModel->setStatusBySrcPath(src, MEDIA_CACHED);
    }
    return status;
}

void Files::MediaManager::insertCurrentlyPlaying(QString path)
{
    currently_playing.insert(path);
}

void Files::MediaManager::removeCurrentlyPlaying(QString path)
{
    currently_playing.remove(path);
}

bool Files::MediaManager::isCurrentlyPlaying(QString path)
{
    return (currently_playing.find(path) != currently_playing.end());
}


QString Files::MediaManager::requestLoadablePath(QString src)
{
    // check if it is in cache first
    if (!isRemote(src) && isRelative(src)) // when media is relative we need the Indexpath set in front
    {
        src = MyConfiguration->getIndexPath() + src;
    }
    return MyMediaModel->findLocalBySrcPath(src);
}

// ==================  protected methods =======================================

void Files::MediaManager::handleRemoteFile(QString src)
{
    QString local_path = MyConfiguration->getPaths("cache") + MyMediaModel->determineHashedFilePath(src);
    QFileInfo fi(local_path);
    if (fi.exists() && MyMediaModel->findLocalBySrcPath(src) == "") // use cached, cause network could be unreachable
    {
        MyMediaModel->insertCacheableFile(src, local_path);
    }
    MyDownloadQueue->insertQueue(src, local_path);
}

// ==================  protected slots =======================================

void Files::MediaManager::doNotCacheable(QString src_file_path)
{
    MyMediaModel->insertAsUncachable(src_file_path);
}

void Files::MediaManager::doSucceed(QString src_file_path, QString local_file_path)
{
    if (MyMediaModel->findLocalBySrcPath(src_file_path) == "")
    {
        MyMediaModel->insertCacheableFile(src_file_path, local_file_path);
    }
    else
    {
        MyMediaModel->setStatusBySrcPath(src_file_path, MEDIA_MODIFIED);
    }
}
