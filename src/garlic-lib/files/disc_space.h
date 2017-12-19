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
#ifndef DISC_SPACE_H
#define DISC_SPACE_H

#include <QObject>
#include <QStorageInfo>
#include <QDirIterator>
#include "tools/logger.h"
#include "db/inventory_table.h"

/**
 * @brief The DiscSpace is a helper class to handle deletes/cleanups for FileModel
 *
 * - calculates the free space of given directory
 * - respect possible quotas by using QStorageInfo::bytesAvailable()
 * - let an amount of percent of free for OS proposals like log files
 * - deletes files/directories and return deleted bytes
 */
class DiscSpace : public QObject
{
    Q_OBJECT
public:

    explicit DiscSpace(QString path = "./", QObject *parent=Q_NULLPTR);
    void     init(QString path);
    void     freedSpace(qint64 size_deleted);
    bool     freeDiscSpace(qint64 size_to_free);
    qint64   calculateNeededDiscSpaceToFree(qint64 size);

    // Getter/Setter
    qint64 getBytesDeleted() const {return bytes_deleted;}
    void setBytesDeleted(const quint64 &value){bytes_deleted = value;}
    qint64 getBytesAvailable() const {return bytes_available;}
    void setBytesAvailable(const quint64 &value){bytes_available = value;}
    qint64 getStorageBytesAvailable() const {return storage.bytesAvailable();}
    qint64 getStorageBytesTotal() const {return storage.bytesTotal();}
    qint64 getStorageBytesFree() const {return storage.bytesFree();}
    void setInventoryTable(DB::InventoryTable *value);

    protected:
    const int _percent_free  = 5; // let some % of total disc space free for Operating System logs, tmp etc
    bool            deleteFile(QString file_path);
    bool            deleteDirectory(QString dir_path);
    qint64          calculateDirectorySize(QString dir_path);

public slots:

protected:
    DB::InventoryTable *MyInventoryTable = Q_NULLPTR;
    QString        cache_path = "";
    quint64        bytes_deleted = 0;
    quint64        bytes_available = 0;
    QStorageInfo   storage;
    QFile          del_file;
    QFileInfo      del_file_info;
    QDir           del_dir;
signals:


};

#endif // DISC_SPACE_H
