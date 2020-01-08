#ifndef FIRMWAREUPDATE_H
#define FIRMWAREUPDATE_H

#include "base_manager.h"

namespace SmilHead
{
    class FirmwareUpdate : public BaseManager
    {
        Q_OBJECT
    public:
        explicit FirmwareUpdate(TConfiguration *config, QObject *parent = nullptr);

    protected:
        TConfiguration *MyConfiguration;
        Downloader     *FirmwareDownloader;
    protected slots:
            void doSucceed(TNetworkAccess *network);
    signals:
            void finishedFirmwareDownload();

    };
}
#endif // FIRMWAREUPDATE_H
