#ifndef DOWNLOAD_QUEUE_H
#define DOWNLOAD_QUEUE_H

#include <QObject>
#include <QQueue>
#include <QSet>
#include "network.h"
#include "disc_space.h"

/**
 * @brief The DownloadQueue class
 *  handles downloads via Network
 */
class DownloadQueue : public QObject
{
    Q_OBJECT
public:
    const int _max_download_slots   = 1;
    explicit DownloadQueue(QByteArray ua);
    void     insertQueue(QString src, QString local);

    //Getter/Setter
    QByteArray getUserAgent() const {return user_agent;}
    void       setUserAgent(const QByteArray &value) {user_agent = value;}
protected:
    bool               working = false;
    QByteArray         user_agent;
    QQueue<QPair<QString, QString> >media_queue;
    QSet<Network *>    download_slots;
    void               processQueue();
    void               emitPaths();

public slots:
    void               doSucceed(QObject *network, QIODevice *data);
    void               doNotModified(QObject *network);
    void               doNotCacheable(QObject *network);
    void               doFailed(QObject *network);

signals:
    void               succeed(QString src, QString local, QIODevice *data);
    void               notmodified(QString src);
    void               notcacheable(QString src);
    void               failed(QString src);

};

#endif // DOWNLOAD_QUEUE_H
