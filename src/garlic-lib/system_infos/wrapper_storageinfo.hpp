#ifndef WRAPPER_STORAGEINFO_HPP
#define WRAPPER_STORAGEINFO_HPP

#include "i_storageinfo.hpp"

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


#endif // WRAPPER_STORAGEINFO_HPP
