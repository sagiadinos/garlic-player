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

#include "index_file.h"

TIndexFile::TIndexFile(TConfiguration *config, QObject *parent)
{
    Q_UNUSED(parent);
    MyConfiguration = config;
    MyDownloader    = new TDownloader(MyConfiguration->getUserAgent());
    loaded          = false;
    connect(MyDownloader, SIGNAL(downloadSucceed(QString)), SLOT(loadAfterDownload(QString)));
    connect(MyDownloader, SIGNAL(noModified(QString)), SLOT(checkForLoaded(QString)));
}

void TIndexFile::load()
{
    QString path = MyConfiguration->getIndexUri();
    if (path.mid(0, 4) == "http" || path.mid(0, 4) == "ftp")
        loadFromHttpToDom(path);
    else
        loadFromLocalToDom(path);
    return;
}

QDomElement TIndexFile::getHead()
{
    return getTag("head");
}

QDomElement TIndexFile::getBody()
{
    return getTag("body");
}


// ========================== protected methods =============================================


void TIndexFile::loadAfterDownload(QString filename)
{
    loadFromLocalToDom(determineLocalFileName(filename));
    return;
}


QString TIndexFile::determineLocalFileName(QString uri)
{
    QUrl url(uri);
    QString file_name = url.fileName();
    if (file_name == "")
        file_name = "index.smil";
    return MyConfiguration->getPaths("var")+"index.smil";
}


void TIndexFile::checkForLoaded(QString filename)
{
    if (!loaded)
        loadFromLocalToDom(determineLocalFileName(filename));
    return;
}

void TIndexFile::loadFromHttpToDom(QString uri)
{
    MyDownloader->checkFiles(determineLocalFileName(uri), uri);
    return;
}

void TIndexFile::loadFromLocalToDom(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (document.setContent(&file))
        {
            emit isLoaded();
            loaded = true;
        }
        else
            qDebug() << "Fail load to a DomDocument";
        file.close();
    }
    else
        qDebug() << QTime::currentTime().toString() << "fail to load " << filename;
    return;
}

QDomElement TIndexFile::getTag(QString tag_name)
{
    QDomNodeList nodelist = document.elementsByTagName(tag_name);
    QDomElement tag;
    if (nodelist.length() == 1)
       tag = nodelist.item(0).toElement();
    return tag;
}

