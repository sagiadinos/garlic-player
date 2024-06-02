#ifndef I_STORAGEINFO_H
#define I_STORAGEINFO_H

#include <QStorageInfo>

class IStorageInfo
{
    public:
        virtual ~IStorageInfo() {}
        virtual quint64 bytesAvailable() const = 0;
        virtual quint64 bytesTotal() const = 0;
        virtual void refresh() = 0;
        virtual void setPath(const QString & ) = 0;
};

#endif // I_STORAGEINFO_H
