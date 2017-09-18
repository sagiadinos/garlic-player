#ifndef FILE_MODEL_H
#define FILE_MODEL_H

#include <QObject>
#include <QStorageInfo>

#include "../ext/quazip/JlCompress.h"
#include "configuration.h"
#include "network.h"
#include "disc_space.h"

/**
 * @brief The FileModel class handles the file management and use the Network-class as helper
 *
 * - decides if a file should be downloaded into cache or is local and can be used direct from his place
 * - handle download into cache and check/organise the neccesary space deleting oldest files
 * - emit a path to FileController
 */
class FileModel : public QObject
{
    Q_OBJECT
public:
    explicit        FileModel(Network *network, QString cache);
    void            registerFile(QString src);
    bool            isRemote(QString src);

    //Getter/Setter
    QString         getCachePath() const{return cache_path;}
    void            setCachePath(const QString &value){cache_path = value;}
    QString         getSrcFilePath() const{return src_file_path;}
    void            setSrcFilePath(const QString &value){src_file_path = value;}
    QFileInfo       getLocalFileInfo() const{return local_file_info;}
    void            setLocalFileInfo(const QFileInfo value){local_file_info = value;}
    QString         getLocalFilePath() const{return local_file_path;}
    void            setLocalFilePath(const QString &value){local_file_path = value;}

protected:
    QStorageInfo    storage;
    Network        *MyNetwork;
    QString         cache_path;
    QString         src_file_path, local_file_path;
    QFileInfo       local_file_info;
    DiscSpace       MyDiscSpace;
    qint64          calculateNeededDiscSpace(qint64 size);
    bool            freeDiscSpace(qint64 size_to_free);
    void            checkForExtension();
    bool            extractWgt();
    QString         determineHashedFilePath();
    qint64          getBytesAvailable(){return storage.bytesAvailable();}
    bool            deleteFile(QString file_path);
    bool            deleteDirectory(QString dir_path);
    qint64          calculateDirectorySize(QString dir_path);
protected slots:
    void            saveToDisk(QIODevice *data);
    void            emitPaths();

signals:
    void            succeed(QString src, QString local);
    void            failed(QString src);

};

#endif // FILE_MODEL_H
