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
#include "index_manager.h"

IndexManager::IndexManager(TConfiguration *config, QObject *parent) : BaseManager(parent)
{
    MyConfiguration = config;
    MyDownloader    = new Downloader(MyConfiguration->getUserAgent().toUtf8(), this);
    MyIndexModel    = new IndexModel(this);
    connect(MyDownloader, SIGNAL(succeed(TNetworkAccess *)), SLOT(doSucceed(TNetworkAccess *)));
}

void IndexManager::init(QString src)
{
    src_index_path = src;
    loadLocal(MyConfiguration->getLastPlayedIndexPath()); // load last Index to be sure that the player can contunue working if Downloader failed
}

void IndexManager::lookUpForIndex()
{
    // if nothing is loaded look first at cache if there is an Index
    if (src_index_path == "")
    {
        qWarning(ContentManager) << "set index path first";
        return;
    }
    if (isRemote(src_index_path))
        MyDownloader->processFile(src_index_path, MyConfiguration->getPaths("cache")+"index.smil");
    else
        loadLocal(src_index_path); // src and local path are identically if src path local
}

void IndexManager::activateRefresh(int value)
{
    refresh_time   = value;
    timer_id       = startTimer(refresh_time*1000);
}

void IndexManager::deactivateRefresh()
{
    if (timer_id == 0)
        return;
    refresh_time   = 0;
    killTimer(timer_id);
}

QDomElement IndexManager::getHead()
{
    return MyIndexModel->getHead();
}

QDomElement IndexManager::getBody()
{
    return MyIndexModel->getBody();
}

// ==================  protected methods =======================================

void IndexManager::loadLocal(QString local_path)
{
    if (!MyIndexModel->loadDocument(local_path))
        return;
    MyConfiguration->setLastPlayedIndexPath(local_path);
    emit availableIndexLoaded();
}

void IndexManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer_id)
        lookUpForIndex();
}

// ==================  protected slots =======================================

void IndexManager::doSucceed(TNetworkAccess *downloader)
{
    Downloader *MyDownloader = qobject_cast<Downloader *> (downloader);
    loadLocal(MyDownloader->getLocalFileInfo().absoluteFilePath());
}
