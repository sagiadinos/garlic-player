#include "wrapper_storageinfo.hpp"

WrapperStorageInfo::WrapperStorageInfo()
{

}

quint64 WrapperStorageInfo::bytesAvailable() const
{
    return MyStorageInfo.bytesAvailable();
}

quint64 WrapperStorageInfo::bytesTotal() const
{
    return MyStorageInfo.bytesTotal();
}

void WrapperStorageInfo::refresh()
{
    MyStorageInfo.refresh();
}

void WrapperStorageInfo::setPath(const QString &path)
{
    MyStorageInfo.setPath(path);
}
