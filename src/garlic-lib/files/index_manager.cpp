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

IndexManager::IndexManager(IndexModel *im, TConfiguration *config, Downloader *nw, QObject *parent) : BaseManager(parent)
{
    MyConfiguration = config;
    MyDownloader    = nw;
    MyIndexModel    = im;
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
        qDebug(ContentManager) << "set index path first";
        return;
    }
    if (isRemote(src_index_path))
        MyDownloader->processFile(src_index_path, MyConfiguration->getPaths("cache")+"index.smil");
    else
        loadLocal(src_index_path); // src and local path are identically if src path local
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
    emit availableIndex();
}

// ==================  protected slots =======================================

void IndexManager::doSucceed(TNetworkAccess *downloader)
{
    Downloader *MyDownloader = qobject_cast<Downloader *> (downloader);
    loadLocal(MyDownloader->getLocalFileInfo().absoluteFilePath());
}
