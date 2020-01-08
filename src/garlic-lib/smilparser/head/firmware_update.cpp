#include "firmware_update.h"

SmilHead::FirmwareUpdate::FirmwareUpdate(TConfiguration *config, QObject *parent) : BaseManager(parent)
{
    MyConfiguration           = config;
    FirmwareDownloader        = new Downloader(MyConfiguration, this);
    connect(FirmwareDownloader, SIGNAL(succeed(TNetworkAccess *)), SLOT(doSucceedParse(TNetworkAccess *)));

}

void SmilHead::FirmwareUpdate::doSucceed(TNetworkAccess *network)
{
    Q_UNUSED(network);
    emit finishedFirmwareDownload();
}
