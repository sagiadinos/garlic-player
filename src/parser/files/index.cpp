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


#include "index.h"

TIndexManager::TIndexManager(TConfiguration *config, QObject *parent)
{
    Q_UNUSED(parent);
    MyConfiguration = config;
    MyDownloader    = new TDownloader(MyConfiguration);
    connect(MyDownloader, SIGNAL(downloadSucceed(QString)), SLOT(loadAfterDownload(QString)));
    connect(MyDownloader, SIGNAL(noModified(QString)), SLOT(checkForLoaded(QString)));
}

void TIndexManager::load()
{
    QString path = MyConfiguration->getIndexUri();
    if (path.mid(0, 4) == "http" || path.mid(0, 4) == "ftp")
        MyDownloader->checkFiles(determineLocalFileName(path), path, path);
    else
        checkForLoaded(path);
}

QDomElement TIndexManager::getHead()
{
    return getTag("head");
}

QDomElement TIndexManager::getBody()
{
    return getTag("body");
}


// ========================== protected methods =============================================

void TIndexManager::checkForLoaded(QString file_path)
{
    if (!actual_index.exists())
        loadFromLocal(determineLocalFileName(file_path));
    return;
}


bool TIndexManager::isIndexEqual(QString file_path)
{
    QFileInfo new_index(file_path);
    return (actual_index == new_index);
}

void TIndexManager::loadAfterDownload(QString filename)
{
    loadFromLocal(determineLocalFileName(filename));
    return;
}

QString TIndexManager::determineLocalFileName(QString uri)
{
    QUrl url(uri);
    QString file_name = url.fileName();
    if (file_name == "")
        file_name = "index.smil";
    return MyConfiguration->getPaths("cache")+"index.smil";
}


void TIndexManager::loadFromLocal(QString file_path)
{
    QFile file(file_path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (document.setContent(&file))
        {
            actual_index.setFile(file_path);
            emit isLoaded();
        }
        else
            qCritical(ContentManager) << "resourceUri:" << file_path << "Fail load to a DomDocument";
        file.close();
    }
    else
        qCritical(ContentManager) << "resourceUri:" << file_path << "Fail load";
    return;
}

QDomElement TIndexManager::getTag(QString tag_name)
{
    QDomNodeList nodelist = document.elementsByTagName(tag_name);
    QDomElement tag;
    if (nodelist.length() == 1)
       tag = nodelist.item(0).toElement();
    return tag;
}

