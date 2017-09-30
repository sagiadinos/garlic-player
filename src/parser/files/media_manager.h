#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <QObject>
#include "media_model.h"
#include "configuration.h"
#include "network_queue.h"

/**
 * @brief The MediaManager class is the "interface" for handling playlist media
 *        It interacts between the parser an
 *
 * - get from parser a file request and delegates it to NetworkQueue not available
 * - transfer media files to MediaModel
 * - answers to a parser request with a playable filepath
 */
class MediaManager : public QObject
{
    Q_OBJECT
public:
    explicit MediaManager(MediaModel *fm, TConfiguration *config, NetworkQueue *dq);
    ~MediaManager();
    void                  clearQueues();
    void                  registerFile(QString src);
    QString               requestLoadablePath(QString src);
    int                   checkCacheStatus(QString src);

protected:
    TConfiguration       *MyConfiguration;
    NetworkQueue         *MyNetworkQueue;
    MediaModel           *MyMediaModel;
    bool                  isRemote(QString src);
protected slots:
    void                  doSucceed(QString src_file_path, QString local_file_path);
    void                  doNotCacheable(QString src_file_path);
};

#endif // MEDIA_MANAGER_H
