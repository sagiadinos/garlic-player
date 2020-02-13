#include "firmware_downloader.h"

SmilHead::FirmwareDownloader::FirmwareDownloader(MainConfiguration *config, QObject *parent) : BaseManager(config, parent)
{
    MyConfiguration           = config;
    MyDownloader        = new Downloader(MyConfiguration, this);
    connect(MyDownloader, SIGNAL(succeed(TNetworkAccess *)), SLOT(doSucceed(TNetworkAccess *)));
}

void SmilHead::FirmwareDownloader::processFromUrl(QUrl firmware_url)
{
    QString file_name = "";
    if (MyConfiguration->getOS() == MyConfiguration->OS_ANDROID)
    {
        file_name = "garlic-player.apk";
    }
    else if (MyConfiguration->getOS() == MyConfiguration->OS_WINDOWS)
    {
        file_name = "garlic-player.exe";
    }
    else if (MyConfiguration->getOS() == MyConfiguration->OS_OSX)
    {
        file_name = "garlic-player.dmg";
    }
    else if (MyConfiguration->getOS() == MyConfiguration->OS_LINUX)
    {
        file_name = "garlic-player.tar.xz";
    }
    else
    {
        file_name = "garlic-player.zip";
    }
    download_file_path = MyConfiguration->getPaths("cache")+file_name;
    MyDownloader->processFile(firmware_url, download_file_path);
}

void SmilHead::FirmwareDownloader::doSucceed(TNetworkAccess *network)
{
    Q_UNUSED(network);
    emit finishedSoftwareDownload(download_file_path);
}
