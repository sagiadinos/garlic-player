#ifndef DISKSPACE_H
#define DISKSPACE_H

#ifdef QT_NO_DEBUG
    #define TESTABLE
#else
    #define TESTABLE virtual
#endif

#include <QStorageInfo>
#include "tools/logger.h"

namespace SystemInfos
{
    class DiscSpace : public QObject
    {
            Q_OBJECT
        public:
            DiscSpace(QStorageInfo *si, QObject *parent = nullptr);
            TESTABLE void     init(QString path);
            TESTABLE void     recalculate();
            TESTABLE quint64 getBytesTotal();
            TESTABLE quint64 getBytesFree();
            TESTABLE quint64 getBytesLocked();
            TESTABLE quint64 getBytesAvailable();
    protected:
            const float    _max_fill    = 0.96F; // let 4% of total disc space free for Operating System logs, tmp etc
            TESTABLE quint64        calculateFreeBytes();
                     quint64        bytes_locked = 0;
                     quint64        bytes_total  = 0;
                     quint64        bytes_free   = 0;
                     QString        cache_path   = "";
                     QStorageInfo  *MyStorage;

    };
}
#endif // DISKSPACE_H
