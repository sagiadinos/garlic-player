#include "download_factory.h"

Files::DownloadFactory::DownloadFactory(QObject *parent) : QObject(parent)
{

}

Downloader *Files::DownloadFactory::createDownloader(MainConfiguration *config, QObject *parent)
{
    return new Downloader(config, parent);
}
