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
    connect(MyIndexManager.data(), SIGNAL(newIndexDownloaded()), this, SLOT(loadIndex()));
    timer_id = startTimer(300000); // every 300s
}

LibFacade::~LibFacade()
{
    killTimer(timer_id);

    MyIndexManager.data()->deactivateRefresh();
}

void LibFacade::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    MyResourceMonitor.refresh();

    // write to logfile
    qInfo(Develop) << MyResourceMonitor.getTotalMemorySystem() << MyResourceMonitor.getFreeMemorySystem();
    qInfo(Develop) << MyResourceMonitor.getMemoryAppUse() << MyResourceMonitor.getMaxMemoryAppUsed();
    qInfo(Develop) << MyResourceMonitor.getThreadsNumber() << MyResourceMonitor.getMaxThreadsNumber();
}

TConfiguration *LibFacade::getConfiguration() const
{
    return MyConfiguration.data();
}

void LibFacade::init()
{
    MyInventoryTable.reset(new DB::InventoryTable(this));
    MyInventoryTable.data()->init(MyConfiguration.data()->getPaths("logs"));

    MyIndexManager.data()->init(MyConfiguration.data()->getIndexUri());
}

void LibFacade::checkForNewSmilIndex()
{
    MyIndexManager.data()->lookUpForUpdatedIndex();
}

THead *LibFacade::getHead() const
{
    return MyHead.data();
}


void LibFacade::beginSmilBodyParsing()
{
    connect(MySmil.data(), SIGNAL(startShowMedia(TMedia *)), this, SLOT(emitStartShowMedia(TMedia *)));
    connect(MySmil.data(), SIGNAL(stopShowMedia(TMedia *)), this, SLOT(emitStopShowMedia(TMedia *)));
    MySmil->beginSmilParsing(MyIndexManager->getBody());
}

void LibFacade::loadIndex()
{
    qDebug(Develop) << "start" << Q_FUNC_INFO;
    if (!MySmil.isNull())
    {
        MySmil.data()->endSmilParsing();
        MyIndexManager.data()->deactivateRefresh();
    }

    // Start with this only when it is absolutly sure that in the player component is no activity anymore.
    if (!MyIndexManager.data()->load())
        return;

    MyHead.reset(new THead(MyConfiguration.data(), this));
    MyHead.data()->setInventoryTable(MyInventoryTable.data()); // must set before parse
    MyHead.data()->parse(MyIndexManager->getHead());
    MyIndexManager.data()->activateRefresh(MyHead->getRefreshTime());

    MyMediaModel.reset(new MediaModel(this));
    MyDownloadQueue.reset(new DownloadQueue(MyConfiguration.data()->getUserAgent().toUtf8(), this));
    MyDownloadQueue.data()->setInventoryTable(MyInventoryTable.data());
    MyMediaManager.reset(new MediaManager(MyMediaModel.data(), MyDownloadQueue.data(), MyConfiguration.data(), this));

    MySmil.reset(new TSmil(MyMediaManager.data(), this));
    emit newIndexLoaded();
    qDebug(Develop) << "end " << Q_FUNC_INFO;
}

void LibFacade::emitStartShowMedia(TMedia *media)
{
    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    emit startShowMedia(media);
    qDebug(Develop) << "end" << Q_FUNC_INFO;
}

void LibFacade::emitStopShowMedia(TMedia *media)
{
    qDebug(Develop) << "begin" << Q_FUNC_INFO;
    emit stopShowMedia(media);
    qDebug(Develop) << "end" << Q_FUNC_INFO;
}

