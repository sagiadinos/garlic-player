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
#include "file_manager.h"

TFileManager::TFileManager(TConfiguration *config, QObject *parent)
{
    Q_UNUSED(parent);
    MyConfiguration = config;
    MyDownloader    = new TDownloader(MyConfiguration->getUserAgent());
    connect(MyDownloader, SIGNAL(downloadSucceed(QString)), SLOT(doFinishDownload(QString)));
    connect(MyDownloader, SIGNAL(noModified(QString)), SLOT(doCancelDownload(QString)));
    connect(MyDownloader, SIGNAL(downloadFailed(QString)), SLOT(doFailDownload(QString)));
    connect(MyDownloader, SIGNAL(uncachable(QString)), SLOT(doUncachable(QString)));
    index_path = MyConfiguration->getIndexUri();
}

TFileManager::~TFileManager()
{
//    delete MyDownloader;
    clearQueues();
}

void TFileManager::clearQueues()
{
    loaded_list.clear();
    download_queue.clear();
    return;
}

void TFileManager::registerFile(QString remote_file)
{
    // Use Cache only when files are on web
    if (index_path.mid(0, 4) == "http" || index_path.mid(0, 3) == "ftp") // if index-Smil is from Web-Service check if download
    {
        if (!isFileInList(remote_file))
        {
            loaded_list.insert(remote_file, _no_exist);
            insertForDownloadQueue(remote_file);
        }
        else
        {
            if (loaded_list[remote_file] != _uncachable)
                insertForDownloadQueue(remote_file);
        }
    }
    else
    {
        if (!isFileInList(remote_file))
            loaded_list.insert(remote_file, _exist);
    }
    return;
}

QString TFileManager::getLoadablePath(QString remote_file)
{
    int  status = checkCacheStatus(remote_file);
    QString ret = "";

    if (index_path.mid(0, 4) == "http" || index_path.mid(0, 3) == "ftp") // if index-Smil is from Web-Service check if download
    {
        if (status != _uncachable)
            ret = MyConfiguration->getPaths("var")+getHashedFilePath(remote_file);
        else
        {
            if (remote_file.mid(0, 4) == "http" || remote_file.mid(0, 3) == "ftp") // if index-Smil is from Web-Service check if download
                ret = remote_file;
            else
                ret =  MyConfiguration->getIndexPath()+remote_file;
        }
    }
    else
        ret = MyConfiguration->getIndexPath()+remote_file;

    return ret;
}

int TFileManager::checkCacheStatus(QString remote_file)
{
    if (isFileInList(remote_file))
        return loaded_list[remote_file];
    else
        return _no_exist;
}

// ==================  protected methods =======================================

void TFileManager::proceedDownloadQueue()
{
    if (download_queue.size() > 0 && !MyDownloader->downloadInProgress())
    {
        QString remote_path = download_queue.dequeue();
        MyDownloader->checkFiles(MyConfiguration->getPaths("var")+getHashedFilePath(remote_path), remote_path);
    }
    return;
}

QString TFileManager::getHashedFilePath(QString remote_path)
{
    QFileInfo fi(remote_path);
    return QString(QCryptographicHash::hash((remote_path.toUtf8()),QCryptographicHash::Md5).toHex())+ "."+fi.suffix();
}

bool TFileManager::isFileInList(QString file_path)
{
    QHash<QString, int>::iterator i = loaded_list.find(file_path);
    if (i == loaded_list.end())
        return false;
    else
        return true;
}

void TFileManager::insertForDownloadQueue(QString remote_file)
{
    download_queue.enqueue(remote_file);
    proceedDownloadQueue();
    return;
}

// =================== proteced slots =========================================

void TFileManager::doFinishDownload(QString remote_file)
{
    if (isFileInList(remote_file))
        loaded_list[remote_file] = _exist;
    proceedDownloadQueue();
    return;
}

void TFileManager::doCancelDownload(QString remote_file)
{
    if (isFileInList(remote_file))
        loaded_list[remote_file] = _exist;
    proceedDownloadQueue();
    return;
}

void TFileManager::doFailDownload(QString remote_file)
{
    if (isFileInList(remote_file) && loaded_list[remote_file] == _exist)
        loaded_list[remote_file] = _outdated;
    proceedDownloadQueue();
    return;
}

void TFileManager::doUncachable(QString remote_file)
{
    if (isFileInList(remote_file))
       loaded_list[remote_file] = _uncachable;
    proceedDownloadQueue();
    return;
}


