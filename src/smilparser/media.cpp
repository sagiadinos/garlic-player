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

#include "media.h"

TMedia::TMedia(TBaseTiming * parent)
{
    Q_UNUSED(parent);
}

bool TMedia::parse(QDomElement element)
{
    root_element   = element;
    setAttributes();     // special for every media type
    setBaseParameters(); // in this class
    MyDownloader = new TDownloader();
    connect(MyDownloader, SIGNAL(downloadSucceed(QString)), SLOT(downloadSucceed(QString)));
    downloaded   = false;
    loaded       = false;
    return true;
}


QString TMedia::getRegion()
{
    return region;
}

bool TMedia::isLoaded()
{
    if (loaded)
        return true;
    return false;
}

void TMedia::prepareLoad(QString index_path, TConfiguration *config)
{
    MyConfiguration = config;
    MyDownloader->setUserAgent(MyConfiguration->getUserAgent());
    QString file_path  = getFilePath(index_path);
    if (index_path.mid(0, 4) == "http" || index_path.mid(0, 3) == "ftp") // if index-Smil is from Web-Service check if download
    {
        QFileInfo fi(file_path);
        QString   cached_file_path = MyConfiguration->getPaths("var")+QString(QCryptographicHash::hash((file_path.toUtf8()),QCryptographicHash::Md5).toHex())+ "."+fi.suffix();
        qDebug() << cached_file_path;
        if (isDownloaded(cached_file_path, file_path))
          downloadSucceed(cached_file_path);
    }
    else
        downloadSucceed(file_path);
    return;
}

bool TMedia::isDownloaded(QString cached_file_path, QString file_path)
{
    if (downloaded)
        return true;

    QFile file(cached_file_path);
    if (file.exists())
    {
        downloaded = true;
        return true;
    }

    if (!MyDownloader->downloadInProgress()) // start a download when it is not already in Progress
        MyDownloader->checkFiles(cached_file_path, file_path);
    return false;
}

void TMedia::downloadSucceed(QString local_path) // slot
{
    downloaded      = true;
    loaded          = load(local_path);
    return;
}


void TMedia::setBaseMediaAttributes()
{
    region = title = src = exec = "";
    if (root_element.hasAttribute("region"))
        region = root_element.attribute("region");
    if (root_element.hasAttribute("src"))
        src = root_element.attribute("src");
    if (root_element.hasAttribute("exec"))
        exec = root_element.attribute("exec");
    setTimingAttributes();
    return;
}

void TMedia::emitfinished() // called from finishedActiveDuration() in TBase
{
   emit finishedMedia(parent_container, this);
   return;
}


void TMedia::setBaseParameters()
{
    cache_control = filename = log_content_id = "";
    if (root_element.hasChildNodes())
    {
        QDomNodeList childs = root_element.childNodes();
        QDomElement param;
        for (int i = 0; i < childs.length(); i++)
        {
            param = childs.item(i).toElement();
            if (param.tagName() == "param")
            {
                if (param.hasAttribute("cacheControl") && param.hasAttribute("value"))
                    cache_control = param.attribute("value");
                else if (param.hasAttribute("filename") && param.hasAttribute("value"))
                    filename = param.attribute("value");
                else if (param.hasAttribute("logContentId") && param.hasAttribute("value"))
                    log_content_id = param.attribute("value");
            }
        }

    }
}


QString TMedia::getFilePath(QString index_path)
{
    if (src.mid(0, 4) == "http" || src.mid(0, 3) == "ftp") // do not use Path of index-Smil when src is a web link
        return src;
    else
        return index_path+src;
}
