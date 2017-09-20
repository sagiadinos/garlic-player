#include "download_queue.h"

DownloadQueue::DownloadQueue(QByteArray ua)
{
    setUserAgent(ua);
}

void DownloadQueue::insertQueue(QString src, QString local)
{
    media_queue.enqueue(qMakePair(src, local));
    processQueue();
}

void DownloadQueue::processQueue()
{
    if (download_slots.size() <= _max_download_slots && media_queue.size() > 0)
    {
        Network *MyNetwork = new Network(user_agent);
        QPair<QString, QString> paths = media_queue.dequeue();
        MyNetwork->processFile(QUrl(paths.first), QFileInfo(paths.second));
        connect(MyNetwork, SIGNAL(succeed(QObject *, QIODevice *)), SLOT(doSucceed(QObject *, QIODevice *)));
        connect(MyNetwork, SIGNAL(notcacheable(QObject*)), SLOT(doNotCacheable(QObject *)));
        connect(MyNetwork, SIGNAL(notmodified(QObject*)), SLOT(doNotModified(QObject *)));
        connect(MyNetwork, SIGNAL(failed(QObject *)), SLOT(doFailed(QObject *)));
        download_slots.insert(MyNetwork);
    }
}

void DownloadQueue::doSucceed(QObject *network, QIODevice *data)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit succeed(MyNetwork->getRemoteFileUrl().toString(), MyNetwork->getLocalFileInfo().absoluteFilePath(), data);
    download_slots.remove(MyNetwork);
    processQueue();
}

void DownloadQueue::doFailed(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit failed(MyNetwork->getRemoteFileUrl().toString());
    download_slots.remove(MyNetwork);
    processQueue();
}

void DownloadQueue::doNotCacheable(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit notcacheable(MyNetwork->getRemoteFileUrl().toString());
    download_slots.remove(MyNetwork);
    processQueue();
}


void DownloadQueue::doNotModified(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit notmodified(MyNetwork->getRemoteFileUrl().toString());
    download_slots.remove(MyNetwork);
    processQueue();
}
