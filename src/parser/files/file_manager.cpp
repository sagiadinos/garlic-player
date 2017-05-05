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

TFileManager::TFileManager(TConfiguration *config)
{
    MyConfiguration = config;
    MyDownloader    = new TDownloader(MyConfiguration->getUserAgent());
    connect(MyDownloader, SIGNAL(downloadSucceed(QString)), SLOT(doFinishDownload(QString)));
    connect(MyDownloader, SIGNAL(noModified(QString)), SLOT(doCancelDownload(QString)));
    connect(MyDownloader, SIGNAL(downloadFailed(QString)), SLOT(doFailDownload(QString)));
    connect(MyDownloader, SIGNAL(uncachable(QString)), SLOT(doUncachable(QString)));
    index_uri = MyConfiguration->getIndexUri();
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

void TFileManager::registerFile(QString src)
{
    qDebug(ContentManager) << " REGISTER_FILE resourceURI:" << src;
    // Use Cache only when files are on web
    if (isRemote(index_uri) || isRemote(src))
    {
        if (!isFileInList(src))
        {
            loaded_list.insert(src, _no_exist);
            insertForDownloadQueue(src);
        }
       else if (loaded_list[src] != _uncachable)
           insertForDownloadQueue(src);
    }
    else
    {
        if (!isFileInList(src))
            loaded_list.insert(src, _exist);
    }
    return;
}


QString TFileManager::getLoadablePath(QString src)
{
    int  status = checkCacheStatus(src);
    QString ret = "";

    if (isRemote(index_uri) || isRemote(src))
    {
        if (status != _uncachable)
        {
           ret = MyConfiguration->getPaths("cache")+getHashedFilePath(src);
           if (src.mid(src.length()-4, 4) == ".wgt")
           {
                #if defined  Q_OS_WIN
                    ret = "file:/"+ret.mid(0, ret.length()-4)+"/index.html"; // Windows needs file://absolute_path
                #else
                    ret = "file://"+ret.mid(0, ret.length()-4)+"/index.html"; // Linux needs file:///absolute_path
                #endif
           }
        }
        else
        {
            if (isRemote(index_uri))
                ret = src;
            else
                ret =  MyConfiguration->getIndexPath()+src;
        }
    }
    else
        if (src.mid(0, 1) != "/" ) // when relative path
            ret = MyConfiguration->getIndexPath()+src;
        else
            ret = src;

    return ret;
}

int TFileManager::checkCacheStatus(QString src)
{
    if (download_queue.size() > 0) // to be sure not forget a download
        proceedDownloadQueue();

    if (isFileInList(src))
        return loaded_list[src];
    else
        return _no_exist;
}

// ==================  protected methods =======================================


QString TFileManager::determineFullRemoteFilePath(QString src)
{
    if (isRemote(src)) // when relative path
        return src;
    else
        return MyConfiguration->getIndexPath()+src;
}

void TFileManager::proceedDownloadQueue()
{
    if (download_queue.size() > 0 && !MyDownloader->downloadInProgress())
    {
        QString src = download_queue.dequeue();

        MyDownloader->checkFiles(MyConfiguration->getPaths("cache")+getHashedFilePath(src), determineFullRemoteFilePath(src), src);
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

void TFileManager::insertForDownloadQueue(QString src)
{
    download_queue.enqueue(src);
    proceedDownloadQueue();
    return;
}

bool TFileManager::isRemote(QString file_path)
{
    // cachable if indexURI is remote and filepath
    if (file_path.mid(0, 4) == "http" || file_path.mid(0,3) == "ftp") // when relative path
        return true;
    else
        return false;
}

// =================== proteced slots =========================================

void TFileManager::doFinishDownload(QString src)
{
    if (isFileInList(src))
        loaded_list[src] = _exist;
    proceedDownloadQueue();
    return;
}

void TFileManager::doCancelDownload(QString src)
{
    if (isFileInList(src))
        loaded_list[src] = _exist;
    proceedDownloadQueue();
    return;
}

void TFileManager::doFailDownload(QString src)
{
    if (isFileInList(src) && loaded_list[src] == _exist)
        loaded_list[src] = _outdated;
    proceedDownloadQueue();
    return;
}

void TFileManager::doUncachable(QString src)
{
    if (isFileInList(src))
       loaded_list[src] = _uncachable;
    proceedDownloadQueue();
    return;
}


