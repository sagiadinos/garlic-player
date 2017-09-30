#ifndef DISC_SPACE_H
#define DISC_SPACE_H

#include <QObject>
#include <QStorageInfo>
#include <QDirIterator>
#include <logging_categories.h>

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

    explicit DiscSpace(QString path = "./");
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

protected:
    const int _percent_free  = 5; // let some % of total disc space free for Operating System logs, tmp etc
    bool            deleteFile(QString file_path);
    bool            deleteDirectory(QString dir_path);
    qint64          calculateDirectorySize(QString dir_path);

public slots:

protected:
    QString        cache_path;
    quint64        bytes_deleted = 0;
    quint64        bytes_available = 0;
    QStorageInfo   storage;
    QFile          del_file;
    QFileInfo      del_file_info;
    QDir           del_dir;
signals:


};

#endif // DISC_SPACE_H
