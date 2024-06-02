#ifndef WRAPPER_STORAGEINFO_H
#define WRAPPER_STORAGEINFO_H

#include "i_storageinfo.h"

class WrapperStorageInfo : public IStorageInfo
{
    public:
        WrapperStorageInfo();
        ~WrapperStorageInfo() override = default;

        quint64 bytesAvailable() const override;
        quint64 bytesTotal() const override;
        void refresh() override;
        void setPath(const QString &path) override;

    private:
        QStorageInfo MyStorageInfo;
};


#endif // WRAPPER_STORAGEINFO_H
