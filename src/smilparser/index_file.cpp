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

TIndexFile::TIndexFile()
{
    MyDownloader = new TDownloader();
    MyDownloader->setUserAgent("GARLIC");
    connect(MyDownloader, SIGNAL(downloadSucceed(QString)), SLOT(loadFromLocalToDom(QString)));
    connect(MyDownloader, SIGNAL(downloadCanceled(QString)), SLOT(loadFromLocalToDom(QString)));
}

void TIndexFile::load(QString path, TConfiguration *config)
{
    MyConfiguration = config;
    if (path.mid(0, 4) == "http" || path.mid(0, 4) == "ftp")
        loadFromHttpToDom(path);
    else
        loadFromLocalToDom(path);
    return;
}

void TIndexFile::loadFromHttpToDom(QString uri)
{

    QUrl url(uri);
    QString file_name = url.fileName();
    if (file_name == "")
        file_name = "index.smil";

    file_name = MyConfiguration->getPaths("var")+"index.smil";

    MyDownloader->setUserAgent(MyConfiguration->getUserAgent());
    MyDownloader->checkFiles(file_name, uri);
    return;
}

void TIndexFile::loadFromLocalToDom(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (!document.setContent(&file))
            qDebug() << "Fail load to a DomDocument";
        file.close();
    }
    else
        qDebug() << "Fail to open file";
    emit isLoaded();
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


QDomElement TIndexFile::getTag(QString tag_name)
{
    QDomNodeList nodelist = document.elementsByTagName(tag_name);
    QDomElement tag;
    if (nodelist.length() == 1)
       tag = nodelist.item(0).toElement();
    return tag;
}
