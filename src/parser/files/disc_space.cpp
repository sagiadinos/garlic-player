#include "disc_space.h"

DiscSpace::DiscSpace(QString path)
{
    init(path);
}

void DiscSpace::init(QString path)
{
    storage.setPath(path);
    setBytesDeleted(0);

    if (getStorageBytesAvailable() == getStorageBytesFree()) // when root or user without quota let some percent of the available storage free
        setBytesAvailable(getStorageBytesAvailable() - (getStorageBytesTotal()/100*_percent_free));
    else
        setBytesAvailable(getStorageBytesAvailable());
}

void DiscSpace::freedSpace(qint64 size_deleted)
{
    setBytesDeleted(getBytesDeleted() + size_deleted);
    setBytesAvailable(getBytesAvailable() - size_deleted);
}

