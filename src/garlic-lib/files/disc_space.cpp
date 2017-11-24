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

#include "disc_space.h"

DiscSpace::DiscSpace(QString path, QObject *parent) : QObject(parent)
{
    init(path);
}

void DiscSpace::init(QString path)
{
    cache_path = path;
    storage.setPath(cache_path);
    setBytesDeleted(0);
    qint64 i = getStorageBytesAvailable();
    if (i == getStorageBytesFree()) // when root or user without quota let some percent of the available storage free
        setBytesAvailable(getStorageBytesAvailable() - (getStorageBytesTotal()/100*_percent_free));
    else
        setBytesAvailable(getStorageBytesAvailable());
}

void DiscSpace::freedSpace(qint64 size_deleted)
{
    setBytesDeleted(getBytesDeleted() + size_deleted);
    setBytesAvailable(getBytesAvailable() - size_deleted);
}

qint64 DiscSpace::calculateNeededDiscSpaceToFree(qint64 size_new_file)
{
    qint64 ret = getBytesAvailable() - size_new_file - 10;

    if (ret >= 0)
        return 0;

    return ret * -1; // when there is need to delete some files is negative so change the sign
}

bool DiscSpace::freeDiscSpace(qint64 size_to_free)
{
    QDir dir(cache_path);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); // do not work, when used in entryInfoList overloaded params directly so set hiere first
    dir.setSorting(QDir::Time | QDir::Reversed);
    QFileInfoList dirList = dir.entryInfoList();
    for(int i = 0; i < dirList.size(); i++)
    {
        if (dirList.at(i).isFile())
        {
            if (!deleteFile(dirList.at(i).absoluteFilePath()))
                return false;
        }
        if (dirList.at(i).isDir())
        {
            if (!deleteDirectory(dirList.at(i).absoluteFilePath()))
                return false;

            // delete corresponding wgt here to prevent inconsistencies e.g. deleted dir, but not wgt - or vice versa
            if (!deleteFile(dirList.at(i).absoluteFilePath()+".wgt"))
                return false;
        }
        qInfo(ContentManager) << "OBJECT_REMOVED resourceUri: " << dirList.at(i).absoluteFilePath() << " removed.";
        if (size_to_free < getBytesDeleted())
            break;
    }

    if (size_to_free > getBytesDeleted()) // if it is not possible to delete target bytes
    {
        qCritical(ContentManager) << "FAILED_FREEING_SPACE - Only " << getBytesDeleted() << "bytes from needed" << size_to_free << "bytes could be deleted";
        return false;
    }
    qInfo(ContentManager) << "OBJECTS_REMOVED "<< getBytesDeleted() << " Bytes of old content were deleted";
    return true;
}

bool DiscSpace::deleteFile(QString file_path)
{
    QFile del_file(file_path);

    if (!del_file.exists()) // not an error. to prevent eventually inconsistencies e.g. wgt <=> wgt directory
        return true;

    qint64 del_size = del_file.size();
    if (!del_file.remove())
    {
        qDebug(Develop) << file_path << " cannot be removed " << del_file.errorString();
        return false;
    }
    freedSpace(del_size);
    return true;
}

bool DiscSpace::deleteDirectory(QString dir_path)
{
    QDir del_dir(dir_path);

    if (!del_dir.exists()) // not an error. to prevent eventually inconsistencies e.g wgt <=> wgt directory
        return true;

    quint64 dir_size = calculateDirectorySize(dir_path);
    if (!del_dir.removeRecursively())
    {
        qDebug(Develop) << dir_path << " cannot be removed";
        return false;
    }
    freedSpace(dir_size);
    return true;
}

qint64 DiscSpace::calculateDirectorySize(QString dir_path)
{
    QDirIterator it(dir_path, QDir::Files, QDirIterator::Subdirectories);
    QFileInfo fi;
    qint64 dir_size = 0;
    while (it.hasNext())
    {
        fi.setFile(it.next());
        dir_size += fi.size();
    }
    return dir_size;
}


