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

LibFacade::LibFacade(QObject *parent) : QObject(parent)
{
    MyConfiguration.reset(new TConfiguration(new QSettings(QSettings::IniFormat, QSettings::UserScope, "SmilControl", "garlic-player")));
    MyIndexManager.reset(new IndexManager(MyConfiguration.data(), this));
    connect(MyIndexManager.data(), SIGNAL(availableIndexLoaded()), this, SLOT(emitNewIndexLoaded()));

#ifdef QT_DEBUG
    timer_id = startTimer(20000); // every 20s
#endif
}

LibFacade::~LibFacade()
{
#ifdef QT_DEBUG
    killTimer(timer_id);
#endif

    MyIndexManager->deactivateRefresh();
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
    {
        max_memory_used = current_rss;
        max_memory_time = QTime::currentTime().toString();
    }
    double d_current = (double)current_rss / (double)1048576;
    double d_max = (double)max_memory_used / (double)1048576;
    qInfo() << QString("App Memory use: %1" ).arg(d_current, 0, 'f', 2) + " MiB" << QString("Max Memory App used: %1" ).arg(d_max, 0, 'f', 2) + " MiB at" << max_memory_time;

    qint64  current_threads = MyGeneralInfos.countThreads();
    if (current_threads > max_threads_used)
        max_threads_used = current_threads;
    qInfo() << "Threads: " + QString::number(current_threads) << "Max Threads: " + QString::number(max_threads_used);
}
#endif

TConfiguration *LibFacade::getConfiguration() const
{
    return MyConfiguration.data();
}

void LibFacade::initIndex()
{
    MyIndexManager.data()->init(MyConfiguration.data()->getIndexUri());
}

void LibFacade::checkForNewSmilIndex()
{
    MyIndexManager.data()->lookUpForIndex();
}

THead *LibFacade::getHead() const
{
    return MyHead.data();
}

void LibFacade::beginSmilBodyParsing()
{
    MySmil->beginSmilParsing(MyIndexManager->getBody());
}

void LibFacade::prepareNewLoadedIndex()
{
    // Start with this only when it is absolutly sure that in the player component is no activity anymore.
    MyIndexManager->deactivateRefresh();
    MyHead.reset(new THead(MyConfiguration.data(), this));
    MyHead.data()->parse(MyIndexManager->getHead());
    MyIndexManager.data()->activateRefresh(MyHead->getRefreshTime());

    MyMediaModel.reset(new MediaModel(this));
    MyDownloadQueue.reset(new DownloadQueue(MyConfiguration.data()->getUserAgent().toUtf8(), this));
    MyMediaManager.reset(new MediaManager(MyMediaModel.data(), MyDownloadQueue.data(), MyConfiguration.data(), this));

    MySmil.reset(new TSmil(MyMediaManager.data(), this));

    connect(MySmil.data(), SIGNAL(startShowMedia(TMedia *)), this, SLOT(emitStartShowMedia(TMedia *)));
    connect(MySmil.data(), SIGNAL(stopShowMedia(TMedia *)), this, SLOT(emitStopShowMedia(TMedia *)));
}

void LibFacade::emitStartShowMedia(TMedia *media)
{
    emit startShowMedia(media);
}

void LibFacade::emitStopShowMedia(TMedia *media)
{
    emit stopShowMedia(media);
}

void LibFacade::emitNewIndexLoaded()
{
    // to make sure player component cannot get signals
    if (!MySmil.isNull())
    {
        disconnect(MySmil.data(), SIGNAL(startShowMedia(TMedia *)), this, SLOT(emitStartShowMedia(TMedia *)));
        disconnect(MySmil.data(), SIGNAL(stopShowMedia(TMedia *)), this, SLOT(emitStopShowMedia(TMedia *)));
    }
    emit newIndexLoaded();
}
