#include "network_queue.h"

NetworkQueue::NetworkQueue(QByteArray ua, QObject *parent) : QObject(parent)
{
    setUserAgent(ua);
}

NetworkQueue::~NetworkQueue()
{
    clearQueues();
}

void NetworkQueue::clearQueues()
{
    media_queue.clear();
    qDeleteAll(download_slots);
    download_slots.clear();
}


void NetworkQueue::insertQueue(QString src, QString local)
{
    media_queue.enqueue(qMakePair(src, local));
    processQueue();
}

void NetworkQueue::processQueue()
{
    if (download_slots.size() < _max_download_slots && media_queue.size() > 0)
    {
        Network *MyNetwork = new Network(getUserAgent(), this);
        connect(MyNetwork, SIGNAL(succeed(QObject *)), SLOT(doSucceed(QObject *)));
        connect(MyNetwork, SIGNAL(notcacheable(QObject*)), SLOT(doNotCacheable(QObject *)));
        connect(MyNetwork, SIGNAL(notmodified(QObject*)), SLOT(doNotModified(QObject *)));
        connect(MyNetwork, SIGNAL(failed(QObject *)), SLOT(doFailed(QObject *)));

        download_slots.insert(MyNetwork);

        QPair<QString, QString> paths = media_queue.dequeue();

        MyNetwork->processFile(QUrl(paths.first), QFileInfo(paths.second));
    }
}

void NetworkQueue::doSucceed(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit succeed(MyNetwork->getRemoteFileUrl().toString(), MyNetwork->getLocalFileInfo().absoluteFilePath());
    download_slots.remove(MyNetwork);
    MyNetwork->deleteLater();
    processQueue();
}

void NetworkQueue::doFailed(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit failed(MyNetwork->getRemoteFileUrl().toString());
    download_slots.remove(MyNetwork);
    MyNetwork->deleteLater();
    processQueue();
}

void NetworkQueue::doNotCacheable(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit notcacheable(MyNetwork->getRemoteFileUrl().toString());
    download_slots.remove(MyNetwork);
    MyNetwork->deleteLater();
    processQueue();
}

void NetworkQueue::doNotModified(QObject *network)
{
    Network *MyNetwork = qobject_cast<Network *> (network);
    emit notmodified(MyNetwork->getRemoteFileUrl().toString());
    download_slots.remove(MyNetwork);
    MyNetwork->deleteLater();
    processQueue();
}
