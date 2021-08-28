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

#include "free_disc_space.h"

FreeDiscSpace::FreeDiscSpace(SystemInfos::DiscSpace *ds, QObject *parent) : QObject(parent)
{
    MyDiscSpace = ds;
}

void FreeDiscSpace::init(QString cp)
{
    cache_path = cp;
    MyDiscSpace->recalculate();
}

void FreeDiscSpace::setInventoryTable(DB::InventoryTable *value)
{
    MyInventoryTable = value;
}

void FreeDiscSpace::freedSpace(qint64 size_deleted)
{
    bytes_deleted += size_deleted;
}

qint64 FreeDiscSpace::calculateNeededDiscSpaceToFree(qint64 size_new_file)
{
    MyDiscSpace->recalculate();
    qint64 ret = MyDiscSpace->getBytesFree() - size_new_file - 10;

    if (ret >= 0)
        return 0;

    return ret * -1; // when there is need to delete some files is negative so change the sign
}

bool FreeDiscSpace::freeDiscSpace(qint64 size_to_free)
{
    QMap<qint64, QFileInfo> dir_list = sortCacheContentsByLastRead();
    QMapIterator<qint64, QFileInfo> i(dir_list);
    i.toBack(); // Map ist sorted ASC, but we need DESC so iterate from back
    while (i.hasPrevious())
    {
        i.previous();

        if (i.value().isDir())
        {
            deleteDirectory(i.value().absoluteFilePath());
            deleteFile(i.value().absoluteFilePath()+".wgt");
        }
        else
        {
            deleteFile(i.value().absoluteFilePath());
        }

        qInfo(ContentManager) << "OBJECT_REMOVED resourceUri: " << i.value().absoluteFilePath() << " removed.";

        // delete entry from db
        if (MyInventoryTable != Q_NULLPTR)
            MyInventoryTable->deleteByCacheName(i.value().fileName());

        if (size_to_free < bytes_deleted)
            break;
    }

    if (size_to_free > bytes_deleted) // if it is not possible to delete target bytes
    {
        qCritical(ContentManager) << "FAILED_FREEING_SPACE - Only " << bytes_deleted << "bytes from needed" << size_to_free << "bytes could be deleted";
        return false;
    }
    qInfo(ContentManager) << "OBJECTS_REMOVED "<< bytes_deleted << " Bytes of old content were deleted";
    return true;
}

bool FreeDiscSpace::deleteFile(QString file_path)
{
    QFile del_file(file_path);

    if (!del_file.exists()) // not an error. to prevent eventually inconsistencies e.g. wgt <=> wgt directory
        return true;

    qint64 del_size = del_file.size();
    if (!del_file.remove())
    {
        qWarning(Develop) << file_path << " cannot be removed " << del_file.errorString();
        return false;
    }
    freedSpace(del_size);
    return true;
}

bool FreeDiscSpace::deleteDirectory(QString dir_path)
{
    QDir del_dir(dir_path);

    if (!del_dir.exists()) // not an error. to prevent eventually inconsistencies e.g wgt <=> wgt directory
        return true;

    quint64 dir_size = calculateDirectorySize(dir_path);
    if (!del_dir.removeRecursively())
    {
        qWarning(Develop) << dir_path << " cannot be removed";
        return false;
    }
    freedSpace(dir_size);
    return true;
}

qint64 FreeDiscSpace::calculateDirectorySize(QString dir_path)
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

QMap<qint64, QFileInfo> FreeDiscSpace::sortCacheContentsByLastRead()
{
    QDir dir(cache_path);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); // do not work, when used in entryInfoList overloaded params directly so set here first
    QFileInfoList dir_list = dir.entryInfoList();

    QMap<qint64, QFileInfo> map;
    for(int i = 0; i < dir_list.size(); i++)
    {
        map.insert(dir_list.at(i).lastRead().toMSecsSinceEpoch(), dir_list.at(i));
    }

    return map;
}

