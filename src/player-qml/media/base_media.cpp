#include "base_media.h"

BaseMedia::BaseMedia(QObject *parent) : QObject(parent)
{
}

QString BaseMedia::determineQmlFillMode(QString fill_mode)
{
    if (fill_mode == "fill")
       return "Stretch";
    else if (fill_mode == "meet")
        return "PreserveAspectCrop";
    else if (fill_mode == "meetbest")
        return "PreserveAspectFit";
    else
        return "Pad";
}

QQuickItem *BaseMedia::createMediaItem(QQmlComponent *mc, QString str)
{
    mc->setData(str.toUtf8(), QUrl());
    double d = (double)getCurrentRSS() / (double)1048576;
    qDebug() << QString( "%1" ).arg(d, 0, 'f', 2) << " MByte RAM used";
    return qobject_cast<QQuickItem *>(mc->create());
}

qint64 BaseMedia::getCurrentRSS()
{
#if defined Q_OS_WIN32
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (qint64)info.WorkingSetSize;

#elif defined Q_OS_DARWIN
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
        return (qint64)0L;		/* Can't access? */
    return (qint64)info.resident_size;

#elif defined Q_OS_LINUX // work on Android too
    QFile fp("/proc/self/statm");
    if (!fp.open(QIODevice::ReadOnly))
        return 0;		/* Can't open? */
    QTextStream in(&fp);
    QStringList fields = in.readLine().split(" ");
    fp.close();
    return fields.at(1).toLong() * (qint64)sysconf(_SC_PAGESIZE);
#else
    /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (qint64)0L;			/* Unsupported. */
#endif
}
