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

TFileManager::TFileManager(TConfiguration *config, QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent);
    MyConfiguration = config;
    MyDownloader    = new TDownloader(MyConfiguration->getUserAgent());
    connect(MyDownloader, SIGNAL(downloadSucceed(QString)), SLOT(doFinishDownload(QString)));
    connect(MyDownloader, SIGNAL(downloadCanceled(QString)), SLOT(doCancelDownload(QString)));
    connect(MyDownloader, SIGNAL(downloadFailed(QString)), SLOT(doFailDownload(QString)));
    index_path = MyConfiguration->getFullIndexPath();
}

TFileManager::~TFileManager()
{
    delete MyDownloader;
    loaded_list.clear();
    download_queue.clear();
}


void TFileManager::registerFile(QString file_path)
{
    if (index_path.mid(0, 4) == "http" || index_path.mid(0, 3) == "ftp") // if index-Smil is from Web-Service check if download
    {
        download_queue.enqueue(file_path);
        loaded_list.insert(file_path, _no_exist);
        proceedDownloadQueue();
    }
    else
    {
        loaded_list.insert(file_path, _exist);
    }
    return;
}

QString TFileManager::getLocalFilePath(QString file_path)
{
    if (index_path.mid(0, 4) == "http" || index_path.mid(0, 3) == "ftp") // if index-Smil is from Web-Service check if download
        return MyConfiguration->getPaths("var")+getHashedFilePath(file_path);
    else
        return MyConfiguration->getIndexPath()+file_path;
}

int TFileManager::checkCacheStatus(QString file_path)
{
    QHash<QString, int>::const_iterator i = loaded_list.find(file_path);
    if (i != loaded_list.end())
         return loaded_list[file_path];
    else
        return _no_exist;
}

QString TFileManager::getHashedFilePath(QString remote_path)
{
    QFileInfo fi(remote_path);
    return QString(QCryptographicHash::hash((remote_path.toUtf8()),QCryptographicHash::Md5).toHex())+ "."+fi.suffix();
}

void TFileManager::doFinishDownload(QString local_file)
{
    QHash<QString, int>::const_iterator i = loaded_list.find(local_file);
    if (i != loaded_list.end())
    {
        loaded_list[local_file] = _exist;
    }
    qDebug() << QTime::currentTime().toString() << "finished Download " << local_file;
    proceedDownloadQueue();
    return;
}

void TFileManager::doCancelDownload(QString local_file)
{
    QHash<QString, int>::const_iterator i = loaded_list.find(local_file);
    if (i != loaded_list.end())
    {
        loaded_list[local_file] = _exist;
    }
    qDebug() << QTime::currentTime().toString() << "canceled Download " << local_file;
    proceedDownloadQueue();
    return;
}

void TFileManager::doFailDownload(QString local_file)
{
    QHash<QString, int>::const_iterator i = loaded_list.find(local_file);
    if (i != loaded_list.end())
    {
        if (loaded_list[local_file] == _exist)
            loaded_list[local_file] = _outdated;
        // Todo: log a failed download and outdated file
    }
    qDebug() << QTime::currentTime().toString() << "fail Download " << local_file;
    proceedDownloadQueue();
    return;
}


void TFileManager::proceedDownloadQueue()
{
    if (download_queue.size() > 0 && !MyDownloader->downloadInProgress())
    {
        QString remote_path = download_queue.dequeue();
        MyDownloader->checkFiles(MyConfiguration->getPaths("var")+getHashedFilePath(remote_path), remote_path);
    }
    return;
}


