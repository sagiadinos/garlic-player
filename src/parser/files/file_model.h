#ifndef FILE_MODEL_H
#define FILE_MODEL_H

#include <QObject>
#include <QMap>

#include "configuration.h"
#include "download_queue.h"
#include "disc_space.h"
#include "wgt.h"

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
    const     int        _no_exist   = 0;
    const     int        _exist      = 1;
    const     int        _reloadable = 2;
    const     int        _uncachable = 3;
    explicit        FileModel(TConfiguration *config);
    bool            isRemote(QString src);
    void            registerFile(QString src_file_path);
    QString         findBySrcMediaPath(QString src_file_path);

    //Getter/Setter
    QString         getCachePath() const{return cache_path;}
    void            setCachePath(const QString &value){cache_path = value;}
    QMap<QString, QPair<QString, int> > getAvailableMediaList() const {return available_media_list;}
    void setAvailableMediaList(const QMap<QString, QPair<QString, int> > &value) {available_media_list = value;}
    DownloadQueue *getDownloadQueue(){return MyDownloadQueue;}
protected:

    TConfiguration *MyConfiguration;
    DownloadQueue  *MyDownloadQueue;
    QString         cache_path;
    DiscSpace       MyDiscSpace;
    void            handleRemoteFile(QString src_file_path);
    void            handleExistingLocalFile(QString src_file_path, QString local_file_path);
    QString         determineCachePathByMediaExtension(QString src_file_path, QString local_file_path);
    bool            saveToDisk(QString src_file_path, QString local_file_path, QIODevice *data);
    QString         determineHashedFilePath(QString src_file_path);
    QMap<QString, QPair<QString, int>>           available_media_list;
    QMap<QString, QPair<QString, int>>::iterator i_available_media_list;
protected slots:
    void            doSucceed(QString src_file_path, QString local_file_path, QIODevice *data);
    void            doNotCacheable(QString src_file_path);
signals:
    void            newIndex(QString);

};

#endif // FILE_MODEL_H
