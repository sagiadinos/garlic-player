#include "disc_space.hpp"

SystemInfos::DiscSpace::DiscSpace(IStorageInfo *si, QObject *parent) : QObject(parent)
{
    MyStorage = si;
}

void SystemInfos::DiscSpace::init(QString path)
{
    cache_path = path;
    MyStorage->setPath(cache_path);

    // Remark: We get screwed, when a disc quota < storage.bytesAvailable()
    // but a disc quota should be untypical use case for digital signage player
    bytes_total  = static_cast<quint64> (MyStorage->bytesTotal() *_max_fill);
    bytes_locked = MyStorage->bytesTotal() - bytes_total;
    bytes_free   = calculateFreeBytes();
}

void SystemInfos::DiscSpace::recalculate()
{
    MyStorage->refresh();
    bytes_free = calculateFreeBytes();
}

quint64 SystemInfos::DiscSpace::getBytesTotal()
{
    return bytes_total;
}

quint64 SystemInfos::DiscSpace::getBytesFree()
{
    return bytes_free;
}

quint64 SystemInfos::DiscSpace::getBytesLocked()
{
    return bytes_locked;
}

quint64 SystemInfos::DiscSpace::calculateFreeBytes()
{
    qint64 tmp = MyStorage->bytesAvailable() - bytes_locked;
    if (tmp < 0)
        return 0;

    return tmp;
}
