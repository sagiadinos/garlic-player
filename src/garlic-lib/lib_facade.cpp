/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
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
#include "lib_facade.h"

LibFacade::LibFacade(TConfiguration *config, QObject *parent) : QObject(parent)
{
    MyConfiguration   = config;
    MyIndexManager    = new IndexManager(new IndexModel(this),
                                         MyConfiguration,
                                         new Downloader(MyConfiguration->getUserAgent().toUtf8()),
                                         this);
    connect(MyIndexManager, SIGNAL(availableIndex()), this, SLOT(setSmilIndex()));

#ifdef QT_DEBUG
    timer_id = startTimer(20000); // every 20s
#endif
}

LibFacade::~LibFacade()
{
#ifdef QT_DEBUG
    killTimer(timer_id);
#endif
    deleteParsingObjects();
    delete MyIndexManager;
}

void LibFacade::initIndex()
{
    MyIndexManager->init(MyConfiguration->getIndexUri());
}


void LibFacade::createParsingObjects()
{

    MyHead   = new THead(MyConfiguration, this);
    connect(MyHead, SIGNAL(checkForNewIndex()), this, SLOT(checkForNewSmilIndex()));
    MediaModel    *media_model    = new MediaModel(this);
    DownloadQueue *download_queue = new DownloadQueue(MyConfiguration->getUserAgent().toUtf8(), this);
    MediaManager  *media_manager  = new MediaManager(media_model, MyConfiguration, download_queue, this);

    MySmil  = new TSmil(media_manager, this);
    connect(MySmil, SIGNAL(startShowMedia(TMedia *)), this, SLOT(emitStartShowMedia(TMedia *)));
    connect(MySmil, SIGNAL(stopShowMedia(TMedia *)), this, SLOT(emitStopShowMedia(TMedia *)));

}

void LibFacade::deleteParsingObjects()
{
    delete MyHead;
    MyHead = Q_NULLPTR;
    delete MySmil;
    MySmil = Q_NULLPTR;

}

#ifdef QT_DEBUG
void LibFacade::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    MyMemoryInfos.refresh();

    qint64  total_mem = MyMemoryInfos.getTotal();
    qint64  free_mem = MyMemoryInfos.getFree();
    double d_total = (double)total_mem / (double)1048576;
    double d_free = (double)free_mem / (double)1048576;
    qInfo() << QString("Total Memory System:: %1" ).arg(d_total, 0, 'f', 2) + " MiB" << QString("Free Memory System:: %1" ).arg(d_free, 0, 'f', 2) + " MiB";

    qint64  current_rss = MyMemoryInfos.getRSS();
    if (current_rss > max_memory_used)
        max_memory_used = current_rss;
    double d_current = (double)current_rss / (double)1048576;
    double d_max = (double)max_memory_used / (double)1048576;
    qInfo() << QString("App Memory use: <b>%1" ).arg(d_current, 0, 'f', 2) + " MiB" << QString("Max Memory App used: %1" ).arg(d_max, 0, 'f', 2) + " MiB";

    qint64  current_threads = MyGeneralInfos.countThreads();
    if (current_threads > max_threads_used)
        max_threads_used = current_threads;
    qInfo() << "Threads: " + QString::number(current_threads) << "Max Threads: " + QString::number(max_threads_used);
}
#endif

TConfiguration *LibFacade::getConfiguration() const
{
    return MyConfiguration;
}

void LibFacade::checkForNewSmilIndex()
{
    MyIndexManager->lookUpForIndex();
}

THead *LibFacade::getHead() const
{
    return MyHead;
}

void LibFacade::beginSmilParsing()
{
    MySmil->beginSmilParsing(MyIndexManager->getBody());
}

TMedia *LibFacade::getCurrentMedia()
{
    return current_media;
}

void LibFacade::setSmilIndex()
{
    if (MySmil != Q_NULLPTR)
        deleteParsingObjects();
    createParsingObjects();
    MySmil->init();
    MyHead->parse(MyIndexManager->getHead());
    emitNewIndex(MyHead->getLayout());
}

void LibFacade::emitStartShowMedia(TMedia *media)
{
    current_media = media;
    emit startShowMedia(media);
}

void LibFacade::emitStopShowMedia(TMedia *media)
{
    emit stopShowMedia(media);
}

void LibFacade::emitNewIndex(QList<Region> *region)
{
    emit newIndex(region);
}
