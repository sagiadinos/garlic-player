#ifndef FIRMWAREUPDATE_H
#define FIRMWAREUPDATE_H

#include "base_manager.h"

namespace SmilHead
{
    class FirmwareDownloader : public Files::BaseManager
    {
        Q_OBJECT
    public:
        explicit FirmwareDownloader(MainConfiguration *config, QObject *parent = nullptr);
        void processFromUrl(QUrl firmware_url);

    protected:
        Downloader     *MyDownloader;
        QString         download_file_path = "";
    protected slots:
            void doSucceed(TNetworkAccess *network);
    signals:
            void finishedSoftwareDownload(QString file_path);

    };
}
#endif // FIRMWAREUPDATE_H
