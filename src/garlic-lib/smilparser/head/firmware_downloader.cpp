/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2021 Nikolaos Sagiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/
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
