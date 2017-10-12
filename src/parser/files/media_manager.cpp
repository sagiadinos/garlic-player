#include "media_manager.h"

MediaManager::MediaManager(MediaModel *mm, TConfiguration *config, NetworkQueue *dq, QObject *parent) : BaseManager(parent)
{
    MyConfiguration = config;
    MyNetworkQueue  = dq;
    MyMediaModel    = mm;
    connect(MyNetworkQueue, SIGNAL(succeed(QString, QString)), SLOT(doSucceed(QString, QString)));
    connect(MyNetworkQueue, SIGNAL(notcacheable(QString)), SLOT(doNotCacheable(QString)));
}

MediaManager::~MediaManager()
{
}

void MediaManager::clearQueues()
{
    MyMediaModel->clearQueue();
    MyNetworkQueue->clearQueues();
    return;
}

void MediaManager::registerFile(QString src)
{
    if (isRemote(src))
    {
        // check if Media is already available in Cache and load to get sure Player wok even if network is broken
        QString local_path = MyConfiguration->getPaths("cache")+MyMediaModel->determineHashedFilePath(src);
        QFileInfo fi(local_path);
        if (fi.exists())
            MyMediaModel->insertAvaibleFile(src, local_path); // src and local path are identically if src path local

        MyNetworkQueue->insertQueue(src, local_path);
    }
    else
        MyMediaModel->insertAvaibleFile(src, src); // src and local path are identically if src path local

}

int MediaManager::checkCacheStatus(QString src)
{
    return MyMediaModel->findStatusBySrcPath(src);
}

QString MediaManager::requestLoadablePath(QString src)
{
    return MyMediaModel->findLocalBySrcPath(src);
}


// ==================  protected methods =======================================


// ==================  protected slots =======================================

void MediaManager::doNotCacheable(QString src_file_path)
{
    MyMediaModel->insertAvaibleLink(src_file_path);
}

void MediaManager::doSucceed(QString src_file_path, QString local_file_path)
{
    MyMediaModel->insertAvaibleFile(src_file_path, local_file_path);
}
